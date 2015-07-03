#ifndef CX_CORE_THREADING_CXPROCESS_H
#define CX_CORE_THREADING_CXPROCESS_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *	
 *	@file CxProcess.h
 *	@brief Contains the interface for a Process.
 * 
 * A Process is run via a process manager.
 *
 * @author Catlin Zilinski
 * @date July 1, 2015
 */

#include "core/Cx.h"
#include "core/common/CxInvasiveStrongPtr.h"
#include "core/common/CxAtomic.h"
#include "core/common/CxHash.h"

namespace Cat {

	/**
	 * @interface CxProcess CxProcess.h "core/threading/CxProcess.h"
	 * @brief The interface for a CxProcess to be run on a CxProcessManager.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Mar 4, 2014
	 */
	class CxProcess {		
	  public:		
		enum State {
			kNotStarted = 0x0,
			kRunning,
			kPaused,
			kTerminated,
			kWillBeRemoved,
			kRemoved,			
			kSucceeded,
			kFailed,
		};

		/** @brief All implementing classes should call this. */
		CX_FORCE_INLINE CxProcess()
			: m_pid(0), mp_name(NIL), m_priority(1),
			  m_priorityModifier(1), m_state(CxProcess::kNotStarted) {}

		CX_FORCE_INLINE CxProcess(OID pid)
			: m_pid(pid), mp_name(NIL), m_priority(1),
			  m_priorityModifier(1), m_state(CxProcess::kNotStarted) {}
		
		CxProcess(const char *name);
		
		/** @brief Virtual destructor, takes care of the name. */
		virtual ~CxProcess();

		/**
		 * @brief Add a child process to this process.
		 * The Child processes will be added to the process manager upon 
		 * the successful completion of the parent process.
		 * @param process The Child process to attach to this process.
		 */
		CX_FORCE_INLINE void attachChild(const CxInvasiveStrongPtr<CxProcess>& process) {
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
		CX_FORCE_INLINE void fail() { m_state = CxProcess::kFailed; }

		/**
		 * @brief Get the amount of time this process should run for.
		 * The amount of time a process can run for depends on the 
		 * priority of the process, and the number of times the process
		 * has been run already.
		 * @return The amount of time this process should run for.
		 */
		CX_FORCE_INLINE CxI32 getRequestedRunTime(CxU32 in_baseTime) const {
			return in_baseTime * m_priority * (m_priorityModifier);			
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
		 * @param in_priorityMod The value to set the priorityModifier to.
		 */
		CX_FORCE_INLINE void setPriorityModifier(CxI32 in_priorityMod) {
			m_priorityModifier = in_priorityMod;
		}		
		
		/** @brief Initialize the process. */
		virtual void initialize();

		/** @return True if the process is still alive. */
		CX_FORCE_INLINE CxBool isAlive() const {
			return (m_state == CxProcess::kRunning ||
					  m_state == CxProcess::kPaused);
		}

		/** @return True if the process is dead. */
		CX_FORCE_INLINE CxBool isDead() const {
			return ( m_state == CxProcess::kSucceeded ||
						m_state == CxProcess::kFailed ||
						m_state == CxProcess::kTerminated ||
						m_state == CxProcess::kWillBeRemoved );
		}

		/** @return True if the process has been initialized already. */
		CX_FORCE_INLINE CxBool isInitialized() const {
			return (m_state != CxProcess::kNotStarted);
		}		

		/** @return True if the process is paused. */
		CX_FORCE_INLINE CxBool isPaused() const {
			return (m_state == CxProcess::kPaused);
		}

		/** @return The name of the process, or null if no name. */
		CX_FORCE_INLINE const CxChar * name() const { return mp_name; }		

		/** @return The value of the priority modifier for the process. */
		CX_FORCE_INLINE CxI32 priorityModifier() const { return m_priorityModifier; }

		/** @brief Method to call when the process has failed. */
		virtual void onFailure() {}

		/** @brief Method to override to handle initialization. */
		virtual void onInitialize() {}

		/** @brief Method to override to handle pausing. */
		virtual void onPause() {}		

		/** @brief Method to override to handle resuming. */
		virtual void onResume() {}		

		/** @brief Method to call when the process has successfully completed. */
		virtual void onSuccess() {}

		/** @brief Method to call when the process has been terminated. */
		virtual void onTermination() {}	

		/** @return The parent process of a child process, if there is one. */
		CX_FORCE_INLINE const CxInvasiveStrongPtr<CxProcess>& parent() const {
			return m_pParent;
		}

		/** @brief Method to indicate the process is paused. */
		CX_FORCE_INLINE void pause() {
			if (m_state == kPSRunning) { m_state = CxProcess::kPaused; }
		}

		/** @return The process ID for the process. */
		CX_FORCE_INLINE CxU32 pid() const { return m_pid; }		

		/** @brief Mark the process as going to be removed. */
		CX_FORCE_INLINE void markForRemoval() { m_state = CxProcess::kWillBeRemoved; }		

		/** @return The priority of a process. (higher = higher priority). */
		CX_FORCE_INLINE CxI32 priority() const { return m_priority; }

		/**
		 * @brief Decrement the retainCount by one.
		 * @return True if there are no more references to the Process.
		 */
		CX_FORCE_INLINE CxBool release() { return (atomic::decr32(m_refCount) <= 0); }

		/** @brief Mark the process as having been removed. */
		CX_FORCE_INLINE void remove() { m_state = CxProcess::kRemoved; }		
		
		/** @brief Method to indicate the process has resumed. */
		CX_FORCE_INLINE void resume() {
			if (m_state == CxProcess::kPaused) { m_state = CxProcess::kRunning; }
		}

		/** @brief Increase the retain count by one. */
		CX_FORCE_INLINE void retain() { atomic::incr32(m_refCount); }

		/** @return The Retain count for the process. */
		CX_FORCE_INLINE CxI32 retainCount() const { return m_refCount; }

		/**
		 * @brief Run the process for the specified amount of time.
		 * @param length The amount of time the process has been allocated.
		 */
		virtual void run(CxU32 in_time) = 0;		

		/** @brief Set the priority of the process. */
		CX_FORCE_INLINE void setPriority(CxI32 in_priority) {
			m_priority = in_priority;
		}
		
		/** @return The current state of the process. */
		CX_FORCE_INLINE CxProcess::State state() const { return m_state; }

		/** @brief Method to call to indicate the process has succeeded. */
		CX_FORCE_INLINE void succeed() { m_state = CxProcess::kSucceeded; }

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
		CX_FORCE_INLINE void terminate() {
			if (m_state == CxProcess::kRunning || m_state == CxProcess::kPaused ||
				 m_state == CxProcess::kNotStarted) {				
				m_state = CxProcess::kTerminated;
			}			
		}	

		/**
		 * @brief Check to see if the process has been removed from the list.
		 * @return True if the process was removed from the list.
		 */
		CX_FORCE_INLINE CxBool wasRemoved() const {
			return (m_state == CxProcess::kRemoved ||
					  m_state == CxProcess::kWillBeRemoved);
		}
		
	  protected:
		CX_FORCE_INLINE void setState(ProcessState state)  { m_state = state; }
		CX_FORCE_INLINE void setParent(CxProcess* parent) {
			m_pParent = CxInvasiveStrongPtr<CxProcess>(parent);
		}
		CX_FORCE_INLINE void removeParent() { m_pParent.setNull(); }
		
	  private:
		OID                        m_pid;
		Char*                      mp_name;		
		AtomicI32		 			   m_retainCount;
		I32							   m_priority;
		I32                        m_priorityModifier;		
		InvasiveStrongPtr<CxProcess> m_pParent;
		ProcessState	  			   m_state;
		InvasiveStrongPtr<CxProcess> m_pChild;

	};

	typedef CxInvasiveStrongPtr<CxProcess> CxProcessPtr;	

} // namespace cat

#endif // CX_CORE_THREADING_CXPROCESS_H
