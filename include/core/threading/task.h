#ifndef CAT_CORE_THREADING_TASK_H
#define CAT_CORE_THREADING_TASK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file task.h
 *	@brief Contains the interface for a Task.
 * 
 * A Task is run via a task manager.
 *
 * @author Catlin Zilinski
 * @date Mar 14, 2014
 */

#include "core/corelib.h"
#include "core/util/invasivestrongptr.h"
#include "core/threading/atomic.h"
#include "core/string/stringutils.h"

namespace Cat {

	/**
	 * @interface Task task.h "core/threading/task.h"
	 * @brief The interface for a Task to be run on a TaskManager.
	 *
	 * Tasks are designed to be run to completion, unlike Processes, which 
	 * are designed to run for longer and execute in chunks.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 12, 2014
	 */
	class Task {		
	  public:		
		enum TaskState {
			kTSNotStarted	= 0x0,
			kTSRunning,
			kTSTerminated,
			kTSRemoved,			
			kTSSucceeded,
			kTSFailed,
		};

		/**
		 * @brief All implementing Task classes should call this.
		 */
		Task()
			: m_oid(0), m_pName(NIL), m_priority(1),
			  m_state(kTSNotStarted) {}

		Task(OID oid)
			: m_oid(oid), m_pName(NIL), m_priority(1),
			  m_state(kTSNotStarted) {}
		
		Task(const Char* name)
			: m_priority(1),
			  m_state(kTSNotStarted) {
			m_pName = StringUtils::copy(name);
			m_oid = crc32(name);
		}
		
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~Task() {
			m_pName = StringUtils::free(m_pName);	
		}

		/**
		 * @brief Add a child task to this task.
		 * The Child taskes will be added to the task manager upon 
		 * the successful completion of the parent task.
		 * @param task The Child task to attach to this task.
		 */
		inline void attachChild(const InvasiveStrongPtr<Task>& task) {
			m_pChild = task;
			if (m_pChild.notNull()) {
				m_pChild->setParent(this);
			}			
		}

		/**
		 * @brief Get a reference to the child task.
		 * @return The Child task (if any attached).
		 */
		inline const InvasiveStrongPtr<Task>& child() const {
			return m_pChild;
		}

		/**
		 * @brief Remove the child task without returning it.
		 */
		inline void detachChild() {
			if (m_pChild.notNull()) {
				m_pChild->removeParent();				
			}	
			m_pChild.setNull();
		}	

		/** 
		 * @brief Method to indicate the task has failed.
		 */
		inline void failed() {				
			m_state = kTSFailed;
		}
		
		/**
		 * @brief Check whether the task has any children or not.
		 * @return True if the task has 1 or more children.
		 */
		inline Boolean hasChild() const {
			return m_pChild.notNull();			
		}

		/**
		 * @brief Initialize the task.
		 */
		virtual void initialize() {					
			m_state = kTSRunning;			
		}

		/**
		 * @brief Check to see if the task is still alive.
		 * @return True if the task is still alive.
		 */
		inline Boolean isAlive() const {
			return (m_state == kTSRunning);
		}

		/**
		 * @brief Check to see if the task is dead.
		 * @return True if the task is dead.
		 */
		inline Boolean isDead() const {
			return ( m_state == kTSSucceeded ||
						m_state == kTSFailed ||
						m_state == kTSTerminated );
		}

		/**
		 * @brief Check to see if the task is initialized.
		 * @return True if the task has been initialized already.
		 */
		inline Boolean isInitialized() const {
			return (m_state != kTSNotStarted);
		}		

		/**
		 * @brief Get the name of the task, if it has one.
		 * @return The name or NIL.
		 */
		inline const Char* name() const { return m_pName; }		

		/**
		 * @brief Method to call when the task has failed.
		 */
		virtual void onFailure() {}

		/**
		 * @brief Method to override to handle initialization.
		 */
		virtual void onInitialize() {}

		/**
		 * @brief Method to call when the task has successfully completed.
		 */
		virtual void onSuccess() {}

		/**
		 * @brief Method to call when the task has been terminated.
		 */
		virtual void onTermination() {}	

		/**
		 * @brief method to get the parent task of a child task.
		 * @return The Parent task or NIL.
		 */
		inline const InvasiveStrongPtr<Task>& parent() const {
			return m_pParent;
		}

		/**
		 * @brief Get the task ID for the task.
		 * @return The oid.
		 */
		inline OID oID() const { return m_oid; }		

		/**
		 * @brief Get the priority of the task.
		 * @return The priority of a task. (higher = higher priority).
		 */
		inline I32 priority() const { return m_priority; }

		/**
		 * @brief Increase the retain count by one.
		 */
		inline void retain() { m_retainCount.increment(); }

		/**
		 * @brief Decrement the retainCount by one.
		 * @return True if there are no more references to the Task.
		 */
		inline Boolean release() {
			m_retainCount.decrement();			
			if (m_retainCount.val() <= 0) {
				return true;
			}
			return false;
		}

		/**
		 * @brief Mark the task as having been removed.
		 */
		inline void remove() { m_state = kTSRemoved; }		
	
		/**
		 * @brief Get the retain count for the Task.
		 * @return The Retain count for the task.
		 */
		inline I32 retainCount() const { return m_retainCount.val(); }

		/**
		 * @brief Run the task to completion.
		 */
		virtual void run() = 0;		

		/**
		 * @brief Set the priority of the task.
		 * @param priority The new priority of the task.
		 */
		inline void setPriority(I32 priority) {
			m_priority = priority;
		}
		
		/**
		 * @brief Check the state of the task.
		 * @return The current state of the task.
		 */
		inline TaskState state() {  return m_state; }

		/**
		 * @brief Method to call to indicate the task has succeeded.
		 */
		inline void succeeded() {
			m_state = kTSSucceeded;		
		}

		/**
		 * @brief Remove the child task and return it.
		 * @return The child task, if one was attached.
		 */
		inline InvasiveStrongPtr<Task> takeChild() {
			InvasiveStrongPtr<Task> child = m_pChild;
			if (m_pChild.notNull()) {
				m_pChild->removeParent();				
			}	
			m_pChild.setNull();
			return child;
		}

		/**
		 * @brief terminate the task.
		 * Can be called while the task is running or before.
		 */
		inline void terminate() {
			if (m_state == kTSRunning ||
				 m_state == kTSNotStarted
				) {				
				m_state = kTSTerminated;
			}			
		}	

		/**
		 * @brief Check to see if the task has been removed from the list.
		 * @return True if the task was removed from the list.
		 */
		inline Boolean wasRemoved() const {
			return (m_state == kTSRemoved);
		}
		
	  private:
		inline void setState(TaskState state)  {
			m_state = state;
		}
		inline void setParent(Task* parent) {
			m_pParent = InvasiveStrongPtr<Task>(parent);
		}
		inline void removeParent() {
			m_pParent.setNull();
		}

		OID                        m_oid;
		Char*                      m_pName;		
		AtomicI32					   m_retainCount;
		I32							   m_priority;
		InvasiveStrongPtr<Task> m_pParent;
		TaskState	  			   m_state;
		InvasiveStrongPtr<Task> m_pChild;		
	};

	typedef InvasiveStrongPtr<Task> TaskPtr;	

} // namespace Cat

#endif // CAT_CORE_THREADING_TASK_H
