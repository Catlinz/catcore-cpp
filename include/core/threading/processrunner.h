#ifndef CAT_CORE_THREADING_PROCESSRUNNER_H
#define CAT_CORE_THREADING_PROCESSRUNNER_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file processrunner.h: 
 * @brief Contains the definition for the ProcessRunner class.
 *
 * @author Catlin Zilinski
 * @date Mar 11, 2014
 */

#include "core/corelib.h"
#include "core/threading/threaddefs.h"
#include "core/threading/conditionvariable.h"
#include "core/threading/spinlock.h"
#include "core/threading/processqueue.h"
#include "core/util/simplequeue.h"

namespace Cat {

	/**
	 * @class ProcessRunner processrunner.h "core/threading/processrunner.h"
	 * @brief A class capable of running multiple tasks at a time on a single thread.
	 * The ProcessRunner is designed to encapsulate a single thread which can be 
	 * executing multiple tasks at once by giving each task a certain amount of 
	 * time in which to execute, before moving onto the next task.
	 *
	 * @since Mar 4, 2014
	 * @version 1
	 * @author Catlin Zilinski
	 */
	class ProcessRunner {

	  public:		
		enum ProcessRunnerState {
			kPMSNotStarted = 0x0,
			kPMSFailedToStart,
			kPMSRunning,
			kPMSWillTerminate,
			kPMSTerminated,
		};

		enum ProcessRunnerMessage {
			kPMMNoMessage = 0x0,
			kPMMTerminateProcess,
			kPMMPauseProcess,
			kPMMResumeProcess,
			kPMMTerminateAllProcesses,
			kPMMTerminateProcessRunner
		};		

		class PMMessage {
		  public:
			ProcessRunnerMessage type;
			OID pid;
			
			PMMessage() : type(kPMMNoMessage), pid(0) {}
			PMMessage(ProcessRunnerMessage pType, OID pPid = 0)
				: type(pType), pid(pPid) {				
			}

			inline Boolean operator==(const PMMessage& other) const {			
				return (type == other.type && pid == other.pid);
			}
			inline Boolean operator!=(const PMMessage& other) const {
				return (type != other.type || pid != other.pid);
			}
		};		

		/**
		 * @brief Initializes an empty ProcessRunner with no processes.
		 */
		ProcessRunner() :
			m_state(kPMSNotStarted), m_oid(0), m_pName(NIL), m_numFree(0), m_numUsed(0), m_pNodeStorage(NIL) {}

		/**
		 * @brief Create a new Process Runner with the specified name.
		 * @param name The name of the Process Runner.
		 */
		ProcessRunner(const Char* name, Size queueSize = 32);		

		/**
		 * @brief Makes sure to stop all the processes, the thread, etc.
		 */
		~ProcessRunner();		

		/**
		 * @brief Find the specified process.
		 * @param pid The pid of the process.
		 */
		inline ProcessPtr getProcess(OID pid) {
			ProcessQueueNode* node = m_index.find(pid);
			if (node) {
				return node->process;
			}
			else {
				return ProcessPtr::nullPtr();
			}			
		}		

		/**
		 * @brief Find the specified process.
		 * @param name The name of the process.
		 */
		inline ProcessPtr getProcess(const Char* name) {
			return getProcess(crc32(name));
		}

		/**
		 * @brief Check to see if there is free room in the queue.
		 * @return true if there is free room in the queue.
		 */
		inline Boolean hasFreeRoom() { return m_free.next != &m_free; }

		/**
		 * @brief Check to see if there are any paused processes.
		 * @return true if there are any paused processes.
		 */
		inline Boolean hasPaused() { return m_paused.next != &m_paused; }	

		/**
		 * @brief Check to see if there are any removed processes.
		 * @return true if there are any removed processes.
		 */
		inline Boolean hasRemoved() { return m_removed.next != &m_removed; }

		/**
		 * @brief Check to see if there are any running processes.
		 * @return true if there are any running processes.
		 */
		inline Boolean hasRunning() { return m_running.next != &m_running; }	

		/**
		 * @brief Check to see if the processRunner is running.
		 * @return True if the ProcessRunner is running.
		 */
		inline Boolean isRunning() const {
			return (m_state == kPMSRunning || m_state == kPMSWillTerminate);			
		}

		/**
		 * @brief Check to see if the processRunner is stopped.
		 * @return True if the ProcessRunner is not running.
		 */
		inline Boolean isStopped() const {
			return (m_state == kPMSNotStarted || m_state == kPMSTerminated);
		}

		/**
		 * @brief Get the name of the process manager.
		 * @return The name of the process manager.
		 */
		inline const Char* name() const { return m_pName; }

		/**
		 * @brief Get the number of free nodes in the runner.
		 * @return The number of free nodes.
		 */
		inline U32 numFreeNodes() const { return m_numFree; }		

		/**
		 * @brief Get the number of used nodes in the runner.
		 * @return The number of used nodes.
		 */
		inline U32 numUsedNodes() const { return m_numUsed; }
		
		/**
		 * @brief Get the OID of the process manager.
		 * @return The OID of the process manager.
		 */
		inline OID oID() const { return m_oid; }

		/**
		 * @brief Pause the specified process.
		 * @param pid The process ID to pause.
		 * @return True if the message was posted to pause the process.
		 */
		inline Boolean pauseProcess(OID pid) {
			Boolean success = false;			
			m_syncLock.lock();
			success = m_messageQueue.push(PMMessage(kPMMPauseProcess, pid));
			m_syncLock.broadcast();			
			m_syncLock.unlock();
#if defined (DEBUG)
			if (!success) {				
				DWARN("Failed to put kPMMPauseProcess (" << pid << ")  message on Queue, queue full!");
			}			
#endif /* DEBUG */
			return success;			
		}

		/**
		 * @brief Pause the specified process.
		 * @param pid The process ID to pause.
		 * @return True if the message was posted to pause the process.
		 */
		inline Boolean pauseProcess(const Char* name) {
			return pauseProcess(crc32(name));
		}


		/**
		 * @brief Get the number of processes the process manager is managing.
		 * This returns running, paused and removed.
		 * @return The number of processes.
		 */
		inline U32 processCount() const { return m_numUsed; }
		
		/**
		 * @brief Enter the processing Thread loop.  DONT EVER CALL THIS.
		 */
		void processingLoop();

		/**
		 * @brief Process any received messages.
		 */
		void processMessages();		
		
		/**
		 * @brief Add a new process to the process manager.
		 * @param process The process to add to be run.
		 */
		inline Boolean queueProcess(const ProcessPtr& process) {
			Boolean success = false;			
			m_syncLock.lock();
			if (m_state == kPMSRunning || m_state == kPMSNotStarted) {
				success = m_inputQueue.push(process);
			}			
			m_syncLock.broadcast();			
			m_syncLock.unlock();
#if defined (DEBUG)
			if (!success) {
				if (m_state != kPMSRunning || m_state != kPMSNotStarted) {
					
					DWARN("Failed to queue process " << process->name() << " Runner in UNuseable state!");
				}
				else {
					DWARN("Failed to queue process " << process->name() << " input queue full!");
				}
			}			
#endif /* DEBUG */
			return success;			
		}

		/**
		 * @brief Resume the specified process.
		 * @param pid The process ID to resume.
		 * @return True if the message was posted to resume the process.
		 */
		inline Boolean resumeProcess(OID pid) {
			Boolean success = false;			
			m_syncLock.lock();
			success = m_messageQueue.push(PMMessage(kPMMResumeProcess, pid));
			m_syncLock.broadcast();			
			m_syncLock.unlock();
#if defined (DEBUG)
			if (!success) {				
				DWARN("Failed to put kPMMResumeProcess (" << pid << ")  message on Queue, queue full!");
			}			
#endif /* DEBUG */
			return success;			
		}

		/**
		 * @brief Resume the specified process.
		 * @param pid The process ID to resume.
		 * @return True if the message was posted to resume the process.
		 */
		inline Boolean resumeProcess(const Char* name) {
			return resumeProcess(crc32(name));
		}
		
		/**
		 * @brief Method to start the process manager's threaded loop.
		 * @return kPMSRunning if succeeded, else, kPMSFailedToStart.
		 */
		ProcessRunnerState run();		
	
		/**
		 * @brief The method that runs each process for the given amount of time.
		 * @param timeForEachProcess The base time to run each process for.
		 */	  
		void runProcesses(U32 timeForEachProcess);

		/**
		 * @brief Get the state of the process manager.
		 * @return The current state of the process manager.
		 */
		inline ProcessRunnerState state() const { return m_state; }		

		/**
		 * @brief Mark all the processes for termination.
		 * This method only terminates all the running processes, it does not 
		 * stop the Process manager from running.
		 */
		inline Boolean terminateAllProcesses() {
			Boolean success = false;
			m_syncLock.lock();			
			success = m_messageQueue.push(PMMessage(kPMMTerminateAllProcesses));			
			m_syncLock.unlock();
#if defined (DEBUG)
			if (!success) {				
				DWARN("Failed to put kPMMTerminateAllProcesses message on Queue, queue full!");
			}			
#endif /* DEBUG */
			return success;			
		}

		/**
		 * @brief Terminate the specified process.
		 * @param pid The process ID to terminate.
		 * @return True if the message was posted to terminate the process.
		 */
		inline Boolean terminateProcess(OID pid) {
			Boolean success = false;			
			m_syncLock.lock();
			success = m_messageQueue.push(PMMessage(kPMMTerminateProcess, pid));
			m_syncLock.broadcast();			
			m_syncLock.unlock();
#if defined (DEBUG)
			if (!success) {				
				DWARN("Failed to put kPMMTerminateProcess (" << pid << ")  message on Queue, queue full!");
			}			
#endif /* DEBUG */
			return success;			
		}

		/**
		 * @brief Terminate the specified process.
		 * @param pid The process ID to terminate.
		 * @return True if the message was posted to terminate the process.
		 */
		inline Boolean terminateProcess(const Char* name) {
			return terminateProcess(crc32(name));
		}

		/**
		 * @brief Terminate the process manager itself.
		 * This method terminates all the processs, along with the thread running
		 * the process manager as well.
		 */
		inline Boolean terminateProcessRunner() {
			Boolean success = false;			
			m_syncLock.lock();
			success = m_messageQueue.push(PMMessage(kPMMTerminateProcessRunner));
			m_syncLock.broadcast();
			m_syncLock.unlock();
#if defined (DEBUG)
			if (!success) {				
				DWARN("Failed to put kPMMTerminateProcessRunner message on Queue, queue full!");
			}			
#endif /* DEBUG */
			return success;
		}		

		/**
		 * @brief Get a pointer to the thread handle for this process manager.
		 * @return A pointer to the thread handle.
		 */
		inline ThreadHandle* threadPtr() { return &m_thread; }

		/**
		 * @brief Wait for the processRunner to stop running.
		 * @return True if the processRunner is no longer running.
		 */
		Boolean waitForTermination();

			/**
		 * @brief Wait for the processRunner to finished starting.
		 * @return True if the processRunner is running.
		 */
		Boolean waitUntilStarted();

		/* ################################################### */
		/* ################### DEBUG METHODS ################# */
		/* ################################################### */

#if defined (DEBUG)
		SimpleQueue<ProcessPtr>* inputQueue() { return &m_inputQueue; }
		ProcessQueueNode* removedQueue() { return &m_removed; }
		SimpleQueue<PMMessage>* messageQueue() { return &m_messageQueue; }
		ProcessQueueNode* runningQueue() { return &m_running; }
		ProcessQueueNode* pausedQueue() { return &m_paused; }
#endif // DEBUG

	  private:
		void addRunningProcess(const ProcessPtr& process);
		void clearProcesses();
		void clearInputQueue();	
		void terminateRunningProcesses();
		void terminatePausedProcesses();
		inline void checkForChildAndRemoveIfNeeded(ProcessPtr& process) {
			if (process->hasChild()) {
				ProcessPtr child = process->takeChild();
				child->terminate();
				child->onTermination();
			}
		}
		inline void checkForParentAndRemoveIfNeeded(ProcessPtr& process) {
			if (process->parent().notNull()) {
				ProcessPtr parent = process->parent();
				parent->detachChild();
			}
		}	
			

		ProcessRunnerState m_state;
		OID					  m_oid;		
		Char*					  m_pName;		
		ConditionVariable	  m_syncLock;		
		ThreadHandle		  m_thread;

		U32 m_numFree;
		U32 m_numUsed;		
		SimpleQueue<ProcessPtr>		   m_inputQueue;
		SimpleQueue<PMMessage>		   m_messageQueue;
		ProcessQueueIndex m_index;		
		ProcessQueueNode m_free;		
		ProcessQueueNode m_running;
		ProcessQueueNode m_paused;
		ProcessQueueNode m_removed;
		ProcessQueueNode* m_pNodeStorage;
	};
	
} // namespace Cat

#endif // CAT_CORE_THREADING_PROCESSRUNNER_H


