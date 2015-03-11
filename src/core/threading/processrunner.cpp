#include "core/threading/processrunner.h"
#include "core/threading/thread.h"
#if defined (DEBUG)
#include <assert.h>
#endif

namespace Cat {

	ProcessRunner::ProcessRunner(const Char* name, Size queueSize) {
		m_state = kPMSNotStarted;		
		m_pName = copy(name);
		m_oid = crc32(name);		
		m_inputQueue.initWithCapacity(queueSize, ProcessPtr::nullPtr());
		m_messageQueue.initWithCapacity((U32)(queueSize * 1.5), PMMessage());

		m_pNodeStorage = new ProcessQueueNode[queueSize*3];
		m_index.initWithCapacity(queueSize*3);
		m_running.initAsRoot();
		m_paused.initAsRoot();
		m_removed.initAsRoot();
		m_free.initAsRoot();
		m_numFree = m_numUsed = 0;
		
		for (U32 i = 0; i < (queueSize * 3); i++) {
			m_pNodeStorage[i].init(&m_free);
			m_index.setAtIndex(i, 0, &(m_pNodeStorage[i]));
			m_numFree++;			
		}	
	}

	ProcessRunner::~ProcessRunner() {
		if (m_pName) { /* Must have been initialzed */			
			terminateProcessRunner();
			waitForTermination();
			m_index.clear();
			m_messageQueue.clear();	
			clearInputQueue();			

			m_running.initAsRoot();
			m_paused.initAsRoot();
			m_removed.initAsRoot();
			m_free.initAsRoot();

			if (m_pNodeStorage) {				
				delete[] m_pNodeStorage;
				m_pNodeStorage = NIL;
			}
			
			if (m_pName) {
				free(m_pName);
				m_pName = NIL;
			}
		}			
	}

	ProcessRunner::ProcessRunnerState ProcessRunner::run() {
		m_syncLock.lock();
		if (Thread::runProcessRunner(this) != NIL) {
			m_state = kPMSRunning;
			DMSG("Process runner " << name() << " started.");
		} else {
			m_state = kPMSFailedToStart;
		}
		m_syncLock.broadcast();
		m_syncLock.unlock();				
		return m_state;
	}

	void ProcessRunner::processMessages() {
		PMMessage message;
		ProcessPtr processPtr;
		ProcessQueueNode* node = NIL;		
		while (!m_messageQueue.isEmpty() &&
				 !(m_state == kPMSWillTerminate || m_state == kPMSTerminated)) {

			message = m_messageQueue.pop();

			switch(message.type) {
				
			case kPMMTerminateProcess:				
				node = m_index.find(message.pid);				
				if (node && node->process.notNull()) {
					if (node->process->isPaused()) {
						node->realloc(&m_running);
					}					
					node->process->terminate();
				}
				break;
				
		   case kPMMPauseProcess:
				node = m_index.find(message.pid);
				if (node && node->process.notNull()) {
					if (!node->process->isPaused()) {
						node->process->pause();
						node->process->onPause();						
						node->realloc(&m_paused);
					}					
				}
				break;

			case kPMMResumeProcess:
				node = m_index.find(message.pid);
				if (node && node->process.notNull()) {
					if (node->process->isPaused()) {
						node->process->resume();
						node->process->onResume();
						node->realloc(&m_running);
					}					
				}
				break;
				
			case kPMMTerminateAllProcesses:
				m_syncLock.lock();				
				clearInputQueue();				
				terminateRunningProcesses();				
				terminatePausedProcesses();
				m_syncLock.unlock();				
				break;
				
			case kPMMTerminateProcessRunner:
				m_syncLock.lock();
				terminateRunningProcesses();				
				terminatePausedProcesses();	
				clearInputQueue();				
				if (m_state == kPMSNotStarted) {
					m_state = kPMSTerminated;
				}				
				else if (m_state == kPMSRunning) {					
					m_state = kPMSWillTerminate;
				}				
				m_syncLock.unlock();				
				break;
				
			default:
				break;
			}
		}
	}

	void ProcessRunner::processingLoop() {
		m_syncLock.lock();
		/* Sync up to make sure have started running */
		m_syncLock.unlock();

		/* Now enter the processing loop. */
		Boolean loopity = true;
		while (loopity) {		
			if (hasRunning() || !m_inputQueue.isEmpty() || !m_messageQueue.isEmpty() || hasRemoved()) {
				runProcesses(1);
			} else {
				m_syncLock.lock();
				while (m_inputQueue.isEmpty() && m_state == kPMSRunning && m_messageQueue.isEmpty()) {
					m_syncLock.wait();
				}
				/* We only want to exit the loop if the process queue is empty, 
				 * since we have to terminate all the processes before we exit.
				 */
				if (!hasRunning() && !hasRemoved() && m_state != kPMSRunning) {
					loopity = false; /* Break out of the loop */
				}				
				m_syncLock.unlock();
			}			
		}

		/* Signal anything waiting on the process runner to terminate */
		m_syncLock.lock();
		m_state = kPMSTerminated;		
		/* Ensure no waiting, removed or paused */
		clearInputQueue();		
		m_messageQueue.clear();
		clearProcesses();		
		DMSG("Process Runner " << name() << " terminated!" << std::flush);		
		m_syncLock.broadcast();
		m_syncLock.unlock();
	}

	void ProcessRunner::runProcesses(U32 timeForEachProcess) {
		
		/* Decrement the count in any removed processes, and remove the ones with zero count */
		if (hasRemoved()) {
			ProcessQueueNode* rnode = m_removed.next;
			ProcessQueueNode* rnext = NIL;			
			while (rnode != &m_removed) {
				rnext = rnode->next;
				if (rnode->count <= 0) {
					/* If node is a child of a parent, remove the child so references can go to 0 */
					if (rnode->process.notNull()) {
						checkForParentAndRemoveIfNeeded(rnode->process);						
					}
					
					rnode->dealloc(&m_free);
					m_numFree++;
					m_numUsed--;					
				}
				else {
					rnode->count--;
				}
				rnode = rnext;				
			}
		}

		/* If there are any processes waiting to be added, add them */
		while (!m_inputQueue.isEmpty() && hasFreeRoom()) {
			addRunningProcess(m_inputQueue.pop());			
		}

		if (!m_messageQueue.isEmpty()) {
			processMessages();
		}				
		ProcessQueueNode* node = m_running.next;
		ProcessQueueNode* next = NIL;		
			
		while (node != &m_running) {
			next = node->next;			
#if defined (DEBUG)
			I32 retainCount = node->process.retainCount();
#endif
			ProcessPtr& process = node->process;
#if defined (DEBUG)
			assert(process.retainCount() == retainCount);
#endif

			/* Initialize the process if we need to */
			if (!process->isInitialized()) {
				process->initialize();
				process->onInitialize();				
			}

			if (process->state() == Process::kPSRunning) {
				process->run(process->getRequestedRunTime(timeForEachProcess));
			}

			if (process->isDead()) {
				Boolean removeProcess = true;				
				ProcessPtr child;				
				switch(process->state()) {
				case Process::kPSTerminated:
					process->onTermination();
					break;
				case Process::kPSSucceeded:
					process->onSuccess();
					/* Try and Add the child process if one exists */
					child = process->child();
					if (child.notNull()) {
						if (hasFreeRoom()) {
							addRunningProcess(child);						
						}
						else {
							removeProcess = false;
						}						
					}
					break;
				case Process::kPSFailed:
					process->onFailure();				
					break;
				default:
					break;					
				}

				/* Remove the process */
				if (removeProcess) {
					if (node->process->state() != Process::kPSSucceeded) {
					   checkForChildAndRemoveIfNeeded(node->process);						
					}					
					node->process->markForRemoval();
					m_index.remove(node->process->pID());
					node->realloc(&m_removed);
					node->count = 1;					
				}
			}
		   node = next;			
		}		
	}

	Boolean ProcessRunner::waitForTermination() {
		m_syncLock.lock();
		while (m_state == kPMSRunning || m_state == kPMSWillTerminate) {
			m_syncLock.wait();
		}
		m_syncLock.unlock();
		return (m_state == kPMSTerminated);		
	}

	Boolean ProcessRunner::waitUntilStarted() {
		m_syncLock.lock();
		while (m_state == kPMSNotStarted) {
			m_syncLock.wait();
		}
		m_syncLock.unlock();
		return (m_state == kPMSRunning);		
	}

	void ProcessRunner::addRunningProcess(const ProcessPtr& process) {
		ProcessQueueNode* node = m_free.next;
		node->alloc(&m_running, process);
		m_numFree--;
		m_numUsed++;
		m_index.set(process->pID(), node);
	}

	void ProcessRunner::clearProcesses() {
		m_index.clear();
		ProcessQueueNode* node = m_running.next;
		ProcessQueueNode* next = NIL;	 
		while (node != &m_running) {
			next = node->next;
			if (node->process.notNull()) {
				checkForChildAndRemoveIfNeeded(node->process);
				checkForParentAndRemoveIfNeeded(node->process);				
				node->process->terminate();
				node->process->onTermination();
			}			
			node->dealloc(&m_free);
			node = next;
			m_numFree++;	
		}
		
		node = m_paused.next;
		next = NIL;	 
		while (node != &m_paused) {
			next = node->next;
			if (node->process.notNull()) {
				checkForChildAndRemoveIfNeeded(node->process);
				checkForParentAndRemoveIfNeeded(node->process);
				node->process->terminate();
				node->process->onTermination();
			}
			node->dealloc(&m_free);
			node = next;
			m_numFree++;			
		}

		node = m_removed.next;
		next = NIL;	 
		while (node != &m_removed) {
			next = node->next;
			if (node->process.notNull()) {
				checkForChildAndRemoveIfNeeded(node->process);
				checkForParentAndRemoveIfNeeded(node->process);
			}
			node->dealloc(&m_free);
			node = next;
			m_numFree++;	
		}
		m_numUsed = 0;		
		m_running.initAsRoot();
		m_paused.initAsRoot();
		m_removed.initAsRoot();
	}

	void ProcessRunner::clearInputQueue() {
		ProcessPtr process;		
		while(!m_inputQueue.isEmpty()) {
			process = m_inputQueue.pop();
			if (process.notNull()) {
				checkForChildAndRemoveIfNeeded(process);			
			}
			process->terminate();
			process->onTermination();
		}
	}
	
	void ProcessRunner::terminateRunningProcesses() {
		ProcessQueueNode* node = m_running.next;		
		while (node != &m_running) {
			if (node->process.notNull()) {
				node->process->terminate();
			}
			node = node->next;					
		}
	}
	
	void ProcessRunner::terminatePausedProcesses() {
		ProcessQueueNode* node = m_paused.next;
		ProcessQueueNode* next = NIL;		
		while (node != &m_paused) {
			next = node->next;			
			if (node->process.notNull()) {
				node->process->terminate();
				node->realloc(&m_running);				
			}
			node = next;			
		}
	}
	
	
} // namespace Cat
