#ifndef CX_CORE_THREADING_CXPROCESSRUNNER_H
#define CX_CORE_THREADING_CXPROCESSRUNNER_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxProcessRunner.h: 
 * @brief Contains the definition for the CxProcessRunner class.
 *
 * @author Catlin Zilinski
 * @date Aug 16, 2015
 */

#include "core/Cx.h"
#include "core/common/CxMsg.h"
#include "core/common/CxDualQueue.h"
#include "core/threading/CxConditionVariable.h"
#include "core/threading/CxSpinlock.h"

namespace cat {

	/**
	 * @class CxProcessRunner CxProcessRunner.h "core/threading/CxProcessRunner.h"
	 * @brief A class capable of running multiple tasks at a time on a single thread.
	 * The CxProcessRunner is designed to encapsulate a single thread which can be 
	 * executing multiple tasks at once by giving each task a certain amount of 
	 * time in which to execute, before moving onto the next task.
	 *
	 * @since Mar 4, 2014
	 * @version 2
	 * @author Catlin Zilinski
	 */
	class CxProcessRunner {
	  public:
		
		enum MsgEnum {
			kNoMsg = 0x0, kTerminateProcessMsg,
			kPauseProcessMsg, kResumeProcessMsg,
			kTerminateAllProcessesMsg, kTerminateProcessRunnerMsg,
		};		

		/** @brief Initializes an empty ProcessRunner with no processes. */
		CxProcessRunner() : m_state(kNotStarted) {}

		/**
		 * @brief Create a new Process Runner with the specified name.
		 * @param in_id The name of the Process Runner.
		 * @param inopt_queueSize The (optional) size of the process queue.
		 */
		CxProcessRunner(const CxChar* in_id, CxI32 inopt_queueSize = 32);		

		/** @brief Makes sure to stop all the processes, the thread, etc. */
		~CxProcessRunner();		

		/**
		 * @brief Find the specified process.
		 * This method does not search the removed processes, only the 
		 * running and paused processes.
		 * @param in_id The (hash of the) id of the process.
		 * @return A pointer to the process or null if not foudn.
		 */
		CxProcessPtr getProcess(CxU32 in_id);

		/** @see getProcess(CxU32) */
		CX_FORCE_INLINE CxProcessPtr getProcess(const CxChar* in_id) {
			return getProcess(CxStrID::hash(in_id));
		}
		
		/** @return True if there are any paused processes. */
		CX_FORCE_INLINE CxBool hasPaused() const { return m_paused.count() != 0; }	

		/** @return True if there are any removed processes. */
		CX_FORCE_INLINE CxBool hasRemoved() const { return m_removed.count() != 0; }

		/** @return True if there are any running processes. */
		CX_FORCE_INLINE CxBool hasRunning() const { return m_running.count() != 0; }

		/** @return A reference to the ID of the process runner */
		CX_FORCE_INLINE const CxStrID & id() const { return m_id; }
		
		/** @return True if the ProcessRunner is running. */
		inline Boolean isRunning() const {
			return (m_state == kPMSRunning || m_state == kPMSWillTerminate);			
		}

		/** @return True if the ProcessRunner is not running. */
		inline Boolean isStopped() const {
			return (m_state == kPMSNotStarted || m_state == kPMSTerminated);
		}

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
		 * @brief Get the number of processes the process runner is runner.
		 * This returns running and paused processes.
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

	  private:

	   CxStrID m_id;
		CxThread m_thread;
		ConditionVariable m_syncLock;		

		CxDualQueue<ProcessPtr> m_inputQueue;
		CxDualQueue< CxMsgOne<U32> > m_messageQueue;
		CxPODVector<Process *> m_running;
		CxPODVector<Process *> m_paused;
		CxPODVector<Process *> m_removed;

		StateEnum m_state;


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
			
	};
	
} // namespace cat

#endif // CX_CORE_THREADING_CXPROCESSRUNNER_H


