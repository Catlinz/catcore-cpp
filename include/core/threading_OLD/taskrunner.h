#ifndef CAT_CORE_THREADING_TASKRUNNER_H
#define CAT_CORE_THREADING_TASKRUNNER_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file taskrunner.h: 
 * @brief Contains the definition for the TaskRunner class.
 *
 * @author Catlin Zilinski
 * @date Mar 13, 2014
 */

#include "core/corelib.h"
#include "core/threading/threaddefs.h"
#include "core/threading/conditionvariable.h"
#include "core/threading/spinlock.h"
#include "core/threading/taskqueuenode.h"
#include "core/util/simplequeue.h"

namespace Cat {

	/**
	 * @class TaskRunner taskrunner.h "core/threading/taskrunner.h"
	 * @brief A class to run tasks on a single thread.
	 * The TaskRunner is designed to encapsulate a single thread which can be 
	 * tasks one at a time, until completion.
	 *
	 * @since Mar 13, 2014
	 * @version 1
	 * @author Catlin Zilinski
	 */
	class TaskRunner {

	  public:		
		enum TaskRunnerState {
			kTRSNotStarted = 0x0,
			kTRSFailedToStart,
			kTRSRunning,
			kTRSWillTerminate,
			kTRSTerminated,
		};

		enum TaskRunnerMessage {
			kTRMNoMessage = 0x0,
			kTRMClearAllWaitingTasks,
			kTRMTerminateTaskRunner
		};		

		class TRMessage {
		  public:
			TaskRunnerMessage type;
			
			TRMessage() : type(kTRMNoMessage) {}
			TRMessage(TaskRunnerMessage pType)
				: type(pType) {				
			}

			inline Boolean operator==(const TRMessage& other) const {			
				return (type == other.type);
			}
			inline Boolean operator!=(const TRMessage& other) const {
				return (type != other.type);
			}
		};		

		/**
		 * @brief Initializes an empty TaskRunner with no taskes.
		 */
		TaskRunner() :
			m_state(kTRSNotStarted), m_oid(0), m_pName(NIL),
			m_numFree(0), m_numUsed(0),  m_pNodeStorage(NIL) {}

		/**
		 * @brief Create a new Task Runner with the specified name.
		 * @param name The name of the Task Runner.
		 */
		TaskRunner(const Char* name, Size queueSize = 32);		

		/**
		 * @brief Makes sure to stop all the taskes, the thread, etc.
		 */
		~TaskRunner();

		/**
		 * @brief Clear all tasks from the input queue.
		 * This method removes all tasks from the input queue from when the message 
		 * is processed (not neccessarily from when posted).
		 * @return True if the message was posted sucessfully.
		 */
		inline Boolean clearAllWaitingTasks() {
			Boolean success = false;			
			m_syncLock.lock();
			success = m_messageQueue.push(TRMessage(kTRMClearAllWaitingTasks));
			m_syncLock.broadcast();
			m_syncLock.unlock();
#if defined (DEBUG)
			if (!success) {				
				DWARN("Failed to put kTRMClearAllWaitingTasks message on Queue, queue full!");
			}			
#endif /* DEBUG */
			return success;
		}

		/**
		 * @brief Check to see if there are any queued tasks.
		 * Checks both the input queue and the queued queue.
		 * @return True if there are any avaialable tasks to run.
		 */
		inline Boolean hasQueued() const {
			return (m_queued.next != &m_queued || !m_inputQueue.isEmpty());
		}

		/**
		 * @brief Check to see if The input queue is full.
		 * @return True if the input queue is full.
		 */
		inline Boolean hasFullQueue() const {
			return m_inputQueue.isFull();
		}

		/**
		 * @brief Check to see if there are any running taskes.
		 * @return true if there are any running taskes.
		 */
		inline Boolean hasRunningTask() { return m_running.notNull(); }	

		/**
		 * @brief Check to see if the taskRunner is running.
		 * @return True if the TaskRunner is running.
		 */
		inline Boolean isRunning() const {
			return (m_state == kTRSRunning || m_state == kTRSWillTerminate);			
		}

		/**
		 * @brief Check to see if the taskRunner is stopped.
		 * @return True if the TaskRunner is not running.
		 */
		inline Boolean isStopped() const {
			return (m_state == kTRSNotStarted || m_state == kTRSTerminated);
		}

		/**
		 * @brief Get the name of the task manager.
		 * @return The name of the task manager.
		 */
		inline const Char* name() const { return m_pName; }

		/**
		 * @brief Get the OID of the task manager.
		 * @return The OID of the task manager.
		 */
		inline OID oID() const { return m_oid; }

		/**
		 * @brief Enter the tasking Thread loop.  DONT EVER CALL THIS.
		 */
		void taskRunLoop();

		/**
		 * @brief Process any received messages.
		 */
		void processMessages();		
		
		/**
		 * @brief Add a new task to the task manager.
		 * @param task The task to add to be run.
		 * @return A strong pointer to the task added or NIL if not added.
		 */
		inline TaskPtr queueTask(const TaskPtr& task) {
			Boolean success = false;			
			m_syncLock.lock();
			if (m_state == kTRSRunning || m_state == kTRSNotStarted) {
				success = m_inputQueue.push(task);
			}			
			m_syncLock.broadcast();			
			m_syncLock.unlock();
			if (success) {
				return task;
			}
			else {
#if defined (DEBUG)
				if (m_state != kTRSRunning || m_state != kTRSNotStarted) {					
					DWARN("Failed to queue task " << task->name() << " Task Runner in UNuseable state!");
				}
				else {
					DWARN("Failed to queue task " << task->name() << " input queue full!");
				}
#endif /* DEBUG */
				return TaskPtr::nullPtr();
			}			
		}
		
		/**
		 * @brief Method to start the task manager's threaded loop.
		 * @return kTRSRunning if succeeded, else, kTRSFailedToStart.
		 */
		TaskRunnerState run();		
	
		/**
		 * @brief The method that runs the next task on the queue.
		 */	  
		void runNextTask();

		/**
		 * @brief Get the state of the task manager.
		 * @return The current state of the task manager.
		 */
		inline TaskRunnerState state() const { return m_state; }		

		/**
		 * @brief Terminate the task manager itself.
		 * This method terminates all the tasks, along with the thread running
		 * the task manager as well.
		 */
		inline Boolean terminateTaskRunner() {
			Boolean success = false;			
			m_syncLock.lock();
			success = m_messageQueue.push(TRMessage(kTRMTerminateTaskRunner));
			if (success && m_state == kTRSRunning) {
				m_state = kTRSWillTerminate;
			}			
			m_syncLock.broadcast();
			m_syncLock.unlock();
#if defined (DEBUG)
			if (!success) {				
				DWARN("Failed to put kTRMTerminateTaskRunner message on Queue, queue full!");
			}			
#endif /* DEBUG */
			return success;
		}		

		/**
		 * @brief Get a pointer to the thread handle for this task manager.
		 * @return A pointer to the thread handle.
		 */
		inline ThreadHandle* threadPtr() { return &m_thread; }

		/**
		 * @brief Wait for the taskRunner to stop running.
		 * @return True if the taskRunner is no longer running.
		 */
		Boolean waitForTermination();

			/**
		 * @brief Wait for the taskRunner to finished starting.
		 * @return True if the taskRunner is running.
		 */
		Boolean waitUntilStarted();

		/* ################################################### */
		/* ################### DEBUG METHODS ################# */
		/* ################################################### */

#if defined (DEBUG)
		inline SimpleQueue<TaskPtr>* inputQueue() { return &m_inputQueue; }
		inline SimpleQueue<TRMessage>* messageQueue() { return &m_messageQueue; }
		inline TaskQueueNode* queuedRoot() { return &m_queued; }
		inline U32 numFree() const { return m_numFree; }
		inline U32 numUsed() const { return m_numUsed; }
#endif // DEBUG

	  private:
		void addTaskToQueue(const TaskPtr& task);		
		void removeRunningTask();
		void clearInputAndQueue();
		inline void checkForChildAndRemoveIfNeeded(TaskPtr& task) {
			if (task->hasChild()) {
				TaskPtr child = task->takeChild();
				child->terminate();
				child->onTermination();
			}
		}
		inline void checkForParentAndRemoveIfNeeded(TaskPtr& task) {
			if (task->parent().notNull()) {
				TaskPtr parent = task->parent();
				parent->detachChild();
			}
		}		
			
		
					
				

		TaskRunnerState     m_state;
		OID					  m_oid;		
		Char*					  m_pName;		
		ConditionVariable	  m_syncLock;		
		ThreadHandle		  m_thread;

		U32 m_numFree;
		U32 m_numUsed;		

		SimpleQueue<TaskPtr>	  m_inputQueue;
		SimpleQueue<TRMessage> m_messageQueue;
		TaskPtr			        m_running;
		TaskQueueNode          m_free;
		TaskQueueNode          m_queued;
		TaskQueueNode*         m_pNodeStorage;		
		
	};
	
} // namespace Cat

#endif // CAT_CORE_THREADING_TASKRUNNER_H


