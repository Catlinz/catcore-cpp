#ifndef CAT_CORE_THREADING_PROCESS_H
#define CAT_CORE_THREADING_PROCESS_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file process.h
 *	@brief Contains the interface for a Process.
 * 
 * A Process is run via a process manager.
 *
 * @author Catlin Zilinski
 * @date Mar 4, 2014
 */

#include "core/corelib.h"
#include "core/util/invasivestrongptr.h"
#include "core/threading/atomic.h"

namespace Cat {

	/**
	 * @interface Process process.h "core/threading/process.h"
	 * @brief The interface for a Process to be run on a ProcessManager.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 4, 2014
	 */
	class Process {		
	  public:		
		enum ProcessState {
			kPSNotStarted	= 0x0,
			kPSRunning,
			kPSPaused,
			kPSTerminated,
			kPSWillBeRemoved,
			kPSRemoved,			
			kPSSucceeded,
			kPSFailed,
		};

		/**
		 * @brief All implementing classes should call this.
		 */
		Process()
			: m_pid(0), m_pName(NIL), m_priority(1),
			  m_priorityModifier(1), m_state(kPSNotStarted) {}

		Process(OID pid)
			: m_pid(pid), m_pName(NIL), m_priority(1),
			  m_priorityModifier(1), m_state(kPSNotStarted) {}
		
		Process(const Char* name)
			: m_priority(1),
			  m_priorityModifier(1), m_state(kPSNotStarted) {
			m_pName = copy(name);
			m_pid = crc32(name);
		}
		
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~Process() {
			if (m_pName) {
				free(m_pName);
				m_pName = NIL;
			}			
		}

		/**
		 * @brief Add a child process to this process.
		 * The Child processes will be added to the process manager upon 
		 * the successful completion of the parent process.
		 * @param process The Child process to attach to this process.
		 */
		inline void attachChild(const InvasiveStrongPtr<Process>& process) {
			m_pChild = process;
			if (m_pChild.notNull()) {
				m_pChild->setParent(this);
			}			
		}

		/**
		 * @brief Get a reference to the child process.
		 * @return The Child process (if any attached).
		 */
		inline const InvasiveStrongPtr<Process>& child() const {
			return m_pChild;
		}

		/**
		 * @brief Remove the child process without returning it.
		 */
		inline void detachChild() {
			if (m_pChild.notNull()) {
				m_pChild->removeParent();				
			}	
			m_pChild.setNull();
		}	

		/** 
		 * @brief Method to indicate the process has failed.
		 */
		inline void failed() {				
			m_state = kPSFailed;
		}

		/**
		 * @brief Get the amount of time this process should run for.
		 * The amount of time a process can run for depends on the 
		 * priority of the process, and the number of times the process
		 * has been run already.
		 * @return The amount of time this process should run for.
		 */
		inline I32 getRequestedRunTime(U32 baseTime) const {
			return baseTime * m_priority * (m_priorityModifier);			
		}
		
		/**
		 * @brief Check whether the process has any children or not.
		 * @return True if the process has 1 or more children.
		 */
		inline Boolean hasChild() const {
			return m_pChild.notNull();			
		}

		/**
		 * @brief Set the priority modifier value.
		 * @param priorityMod The value to set the priorityModifier to.
		 */
		inline void setPriorityModifier(I32 priorityMod) {
			m_priorityModifier = priorityMod;
		}		
		
		/**
		 * @brief Initialize the process.
		 */
		virtual void initialize() {					
			m_state = kPSRunning;			
		}

		/**
		 * @brief Check to see if the process is still alive.
		 * @return True if the process is still alive.
		 */
		inline Boolean isAlive() const {
			return (m_state == kPSRunning || m_state == kPSPaused);
		}

		/**
		 * @brief Check to see if the process is dead.
		 * @return True if the process is dead.
		 */
		inline Boolean isDead() const {
			return ( m_state == kPSSucceeded ||
						m_state == kPSFailed ||
						m_state == kPSTerminated ||
						m_state == kPSWillBeRemoved );
		}

		/**
		 * @brief Check to see if the process is initialized.
		 * @return True if the process has been initialized already.
		 */
		inline Boolean isInitialized() const {
			return (m_state != kPSNotStarted);
		}		

		/**
		 * @brief Check to see if the process is pasued.
		 * @return True if the process is paused.
		 */
		inline Boolean isPaused() const {
			return (m_state == kPSPaused);
		}

		/**
		 * @brief Get the name of the process, if it has one.
		 * @return The name or NIL.
		 */
		inline const Char* name() const { return m_pName; }		

		/**
		 * @brief Get the value of the priorityModifier for the process.
		 * @return The value of the priority modifier for the process.
		 */
		inline I32 priorityModifier() const { return m_priorityModifier; }

		/**
		 * @brief Method to call when the process has failed.
		 */
		virtual void onFailure() {}

		/**
		 * @brief Method to override to handle initialization.
		 */
		virtual void onInitialize() {}

		/**
		 * @brief Method to override to handle pausing.
		 */
		virtual void onPause() {}		

		/**
		 * @brief Method to override to handle resuming.
		 */
		virtual void onResume() {}		

		/**
		 * @brief Method to call when the process has successfully completed.
		 */
		virtual void onSuccess() {}

		/**
		 * @brief Method to call when the process has been terminated.
		 */
		virtual void onTermination() {}	

		/**
		 * @brief method to get the parent process of a child process.
		 * @return The Parent process or NIL.
		 */
		inline const InvasiveStrongPtr<Process>& parent() const {
			return m_pParent;
		}

		/**
		 * @brief Method to indicate the process is paused.
		 */
		inline void pause() {
			if (m_state == kPSRunning) {				
				m_state = kPSPaused;
			}
		}

		/**
		 * @brief Get the process ID for the process.
		 * @return The pid.
		 */
		inline OID pID() const { return m_pid; }		

		/**
		 * @brief Mark the process as going to be removed.
		 */
		inline void markForRemoval() { m_state = kPSWillBeRemoved; }		

		/**
		 * @brief Get the priority of the process.
		 * @return The priority of a process. (higher = higher priority).
		 */
		inline I32 priority() const { return m_priority; }

		/**
		 * @brief Increase the retain count by one.
		 */
		inline void retain() {
		   m_retainCount.increment();			
		}

		/**
		 * @brief Decrement the retainCount by one.
		 * @return True if there are no more references to the Process.
		 */
		inline Boolean release() {
		   m_retainCount.decrement();	
			if (m_retainCount.val() <= 0) {
				return true;
			}
			return false;
		}

		/**
		 * @brief Mark the process as having been removed.
		 */
		inline void remove() { m_state = kPSRemoved; }		
		
		/**
		 * @brief Method to indicate the process has resumed.
		 */
		inline void resume() {
			if (m_state == kPSPaused) {
				m_state = kPSRunning;
			}			
		}	

		/**
		 * @brief Get the retain count for the Process.
		 * @return The Retain count for the process.
		 */
		inline I32 retainCount() const { return m_retainCount.val(); }

		/**
		 * @brief Run the process for the specified amount of time.
		 * @param length The amount of time the process has been allocated.
		 */
		virtual void run(U32 time) = 0;		

		/**
		 * @brief Set the priority of the process.
		 * @param priority The new priority of the process.
		 */
		inline void setPriority(I32 priority) {
			m_priority = priority;
		}
		
		/**
		 * @brief Check the state of the process.
		 * @return The current state of the process.
		 */
		inline ProcessState state() {  return m_state; }

		/**
		 * @brief Method to call to indicate the process has succeeded.
		 */
		inline void succeeded() {
			m_state = kPSSucceeded;		
		}

		/**
		 * @brief Remove the child process and return it.
		 * @return The child process, if one was attached.
		 */
		inline InvasiveStrongPtr<Process> takeChild() {
			InvasiveStrongPtr<Process> child = m_pChild;
			if (m_pChild.notNull()) {
				m_pChild->removeParent();				
			}	
			m_pChild.setNull();
			return child;
		}

		/**
		 * @brief terminate the process.
		 * Can be called while the process is running or before.
		 */
		inline void terminate() {
			if (m_state == kPSRunning ||
				 m_state == kPSPaused ||
				 m_state == kPSNotStarted
				) {				
				m_state = kPSTerminated;
			}			
		}	

		/**
		 * @brief Check to see if the process has been removed from the list.
		 * @return True if the process was removed from the list.
		 */
		inline Boolean wasRemoved() const {
			return (m_state == kPSRemoved || m_state == kPSWillBeRemoved);
		}
		
	  protected:
		inline void setState(ProcessState state)  {
			m_state = state;
		}
		inline void setParent(Process* parent) {
			m_pParent = InvasiveStrongPtr<Process>(parent);
		}
		inline void removeParent() {
			m_pParent.setNull();
		}

	  private:
		OID                        m_pid;
		Char*                      m_pName;		
		AtomicI32		 			   m_retainCount;
		I32							   m_priority;
		I32                        m_priorityModifier;		
		InvasiveStrongPtr<Process> m_pParent;
		ProcessState	  			   m_state;
		InvasiveStrongPtr<Process> m_pChild;		
	};

	typedef InvasiveStrongPtr<Process> ProcessPtr;	

} // namespace Cat

#endif // CAT_CORE_THREADING_PROCESS_H
