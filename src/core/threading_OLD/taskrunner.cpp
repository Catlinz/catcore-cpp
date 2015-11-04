#include "core/threading/taskrunner.h"
#include "core/threading/thread.h"
#if defined (DEBUG)
#include <assert.h>
#endif

namespace Cat {

	TaskRunner::TaskRunner(const Char* name, Size queueSize) {
		m_state = kTRSNotStarted;		
		m_pName = copy(name);
		m_oid = crc32(name);		
		m_inputQueue.initWithCapacity(queueSize, TaskPtr::nullPtr());
		m_messageQueue.initWithCapacity((U32)(queueSize), TRMessage());

		m_pNodeStorage = new TaskQueueNode[queueSize];
		m_free.initAsRoot();
		m_queued.initAsRoot();		
		m_numFree = m_numUsed = 0;
		
		for (U32 i = 0; i < (queueSize); i++) {
			m_pNodeStorage[i].init(&m_free);
			m_numFree++;			
		}	
	}

	TaskRunner::~TaskRunner() {
		if (m_pName) { /* Must have been initialzed */			
			terminateTaskRunner();
			waitForTermination();
			m_messageQueue.clear();

			clearInputAndQueue();			
			removeRunningTask();

			m_queued.initAsRoot();
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

	TaskRunner::TaskRunnerState TaskRunner::run() {
		m_syncLock.lock();
		if (Thread::runTaskRunner(this) != NIL) {
			m_state = kTRSRunning;
			DMSG("Task runner " << name() << " started.");
		} else {
			m_state = kTRSFailedToStart;
		}
		m_syncLock.broadcast();
		m_syncLock.unlock();				
		return m_state;
	}

	void TaskRunner::processMessages() {
		TRMessage message;
		TaskPtr taskPtr;	
		while (!m_messageQueue.isEmpty() &&
				 !(m_state == kTRSTerminated)) {

			message = m_messageQueue.pop();

			switch(message.type) {
				
			case kTRMClearAllWaitingTasks:
				m_syncLock.lock();				
				clearInputAndQueue();
				m_syncLock.unlock();				
				break;
				
			case kTRMTerminateTaskRunner:
				m_syncLock.lock();
				removeRunningTask();					
				clearInputAndQueue();
				if (m_state == kTRSNotStarted) {
					m_state = kTRSTerminated;
				}				
				m_syncLock.unlock();				
				break;
				
			default:
				break;
			}
		}
	}

	void TaskRunner::taskRunLoop() {
		m_syncLock.lock();
		/* Sync up to make sure have started running */
		m_syncLock.unlock();

		/* Now enter the tasking loop. */
		Boolean loopity = true;
		while (loopity) {		
			if (hasQueued() || !m_messageQueue.isEmpty()) {
				runNextTask();
			} else {
				m_syncLock.lock();
				while (m_inputQueue.isEmpty() && m_state == kTRSRunning && m_messageQueue.isEmpty()) {
					m_syncLock.wait();
				}
				/* We only want to exit the loop if the task queue is empty, 
				 * since we have to terminate all the taskes before we exit.
				 */
				if (!hasQueued() && m_state != kTRSRunning) {
					loopity = false; /* Break out of the loop */
				}				
				m_syncLock.unlock();
			}			
		}

		/* Signal anything waiting on the task runner to terminate */
		m_syncLock.lock();
		m_state = kTRSTerminated;		
		/* Ensure no waiting */
	   clearInputAndQueue();		
		m_messageQueue.clear();
		removeRunningTask();		
		DMSG("Task Runner " << name() << " terminated!" << std::flush);		
		m_syncLock.broadcast();
		m_syncLock.unlock();
	}

	void TaskRunner::runNextTask() {

		/* If there are any messages, process them */
		if (!m_messageQueue.isEmpty()) {
			processMessages();				
		}

		/* If there are any waiting tasks in the input queue, pull them into 
		 * the queued list. */
		/* If there are any processes waiting to be added, add them */
		while (!m_inputQueue.isEmpty() && m_numFree > 0) {
			addTaskToQueue(m_inputQueue.pop());			
		}

		/* If there is a task to run, run it. */
		if (m_queued.next != &m_queued) {
			m_running = m_queued.next->task;
			m_queued.next->dealloc(&m_free);
			m_numFree++;
			m_numUsed--;			
		}

		if (m_running.notNull()) {
			/* Initialize the task if we need to */
			if (!m_running->isInitialized()) {
				m_running->initialize();
				m_running->onInitialize();				
			}

			if (m_running->state() == Task::kTSRunning) {
				m_running->run();
			}

			if (m_running->isDead()) {				
				TaskPtr child;
				
				switch(m_running->state()) {
					
				case Task::kTSTerminated:
					m_running->onTermination();
					break;
					
				case Task::kTSSucceeded:
					m_running->onSuccess();
					/* Add the child task if one exists */
					child = m_running->child();
					if (child.notNull()) {
						if (m_numFree > 0) {
							addTaskToQueue(child);							
						}					
					}
					break;
					
				case Task::kTSFailed:
					m_running->onFailure();				
					break;
					
				default:
					break;					
				}

				/* Remove the task */
				removeRunningTask();								
			}
		}
	}

	Boolean TaskRunner::waitForTermination() {
		m_syncLock.lock();
		while (m_state == kTRSRunning || m_state == kTRSWillTerminate) {
			m_syncLock.wait();
		}
		m_syncLock.unlock();
		return (m_state == kTRSTerminated);		
	}

	Boolean TaskRunner::waitUntilStarted() {
		m_syncLock.lock();
		while (m_state == kTRSNotStarted) {
			m_syncLock.wait();
		}
		m_syncLock.unlock();
		return (m_state == kTRSRunning);		
	}

	void TaskRunner::addTaskToQueue(const TaskPtr& task) {		
		TaskQueueNode* node = m_free.next;
		node->alloc(&m_queued, task);
		m_numFree--;
		m_numUsed++;
	}

	void TaskRunner::removeRunningTask() {
		if (m_running.notNull()) {
			if (m_running->state() != Task::kTSSucceeded) {
				checkForChildAndRemoveIfNeeded(m_running);				
			}
			if (!m_running->isInitialized() || m_running->isAlive()) {
				m_running->terminate();
				m_running->onTermination();
			}
			checkForParentAndRemoveIfNeeded(m_running);			
			m_running->remove();
			m_running.setNull();			
		}
	}
	
		

	void TaskRunner::clearInputAndQueue() {
		TaskPtr task;		
		while(!m_inputQueue.isEmpty()) {
			task = m_inputQueue.pop();
			if (task.notNull()) {				
				checkForChildAndRemoveIfNeeded(task);			
				task->terminate();
				task->onTermination();
			}			
		}
		
		TaskQueueNode* node = m_queued.next;
		TaskQueueNode* next = NIL;	 
		while (node != &m_queued) {
			next = node->next;						
			if (node->task.notNull()) {
				checkForChildAndRemoveIfNeeded(node->task);
				checkForParentAndRemoveIfNeeded(node->task);				
				node->task->terminate();				
				node->task->onTermination();
			}			
			node->dealloc(&m_free);
			node = next;
			m_numFree++;	
		}
		
		m_numUsed = 0;		
		m_queued.initAsRoot();
	}	
	
	
	
} // namespace Cat
