#ifndef CAT_CORE_DEFER_TIMEDACTION_H
#define CAT_CORE_DEFER_TIMEDACTION_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file timedaction.h
 *	@brief Contains the interface for a TimedAction.
 * 
 * A TimedAction is run via a TimedAction manager.
 *
 * @author Catlin Zilinski
 * @date Mar 4, 2014
 */

#include "core/corelib.h"
#include "core/util/invasivestrongptr.h"
#include "core/threading/atomic.h"
#include "core/time/time.h"

namespace Cat {

	/**
	 * @interface TimedAction timedaction.h "core/defer/timedaction.h"
	 * @brief The interface for a TimedAction, to be executed by a Timer.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 18, 2014
	 */
	class TimedAction {		
	  public:		
		enum TimedActionState {
			kTASWaiting	= 0x0,
			kTASActive,
			kTASExecuting,
			kTASPaused,
			kTASTerminated,
			kTASRemoved,			
		};

		/**
		 * @brief All implementing classes should call this.
		 */
		TimedAction()
			: m_actionID(0), m_oid(0), m_pName(NIL), m_state(kTASWaiting) {}

		TimedAction(OID oid)
			: m_actionID(0), m_oid(oid), m_pName(NIL), m_state(kTASWaiting) {}
		TimedAction(OID oid, const TimeVal& timeToWait)
			: m_actionID(0), m_oid(oid), m_pName(NIL), m_state(kTASWaiting),
			  m_timeToWait(timeToWait) {}

		TimedAction(const Char* name)
			: m_actionID(0), m_oid(0), m_pName(NIL), m_state(kTASWaiting) {
			m_pName = copy(name);
			m_oid = crc32(name);
		}
		TimedAction(const Char* name, const TimeVal& timeToWait)
			: m_actionID(0), m_oid(0), m_pName(NIL), m_state(kTASWaiting),
			  m_timeToWait(timeToWait) {
			m_pName = copy(name);
			m_oid = crc32(name);
		}
		
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~TimedAction() {
			if (m_pName) {
				free(m_pName);
				m_pName = NIL;
			}			
		}

		/**
		 * @brief Get the actionID for the TimedAction.
		 * @return The actionID assigned by the Timer.
		 */
		inline U64 actionID() const { return m_actionID; }

		/**
		 * @brief Method to call when the Timer fires.
		 * @return For a repeated process, if should keep repeated, returns True.
		 */
		virtual Boolean fire() = 0;

		/**
		 * @brief Method to check to see if the action should fire based on the current time.
		 * @return True if the action should fire.
		 */
		inline Boolean shouldFire() const {
			return Time::compareRaw(Time::currentTimeRaw(), m_nextFireTime) > 0;			
		}

		/**
		 * @brief Initialize the TimedAction (after has been added to the Timer).
		 */
		inline void initialize() {
			m_state = kTASActive;
		}		
		
		/**
		 * @brief Get the name of the TimedAction, if it has one.
		 * @return The name or NIL.
		 */
		inline const Char* name() const { return m_pName; }

		/**
		 * @brief Get the next firing time in platform dependant time units.
		 * @return The next fireing time in platform dependant time units.
		 */
		inline RawTimeVal nextFireTime() const { return m_nextFireTime; }		

		/**
		 * @brief Method to override to handle initialization.
		 */
		virtual void onInitialize() {}

		/**
		 * @brief Method to indicate the TimedAction is paused.
		 */
		inline void pause() {
			m_state = kTASPaused;
		}

		/**
		 * @brief Get the object ID for the TimedAction.
		 * @return The oid.
		 */
		inline OID oID() const { return m_oid; }		

		/**
		 * @brief Increase the retain count by one.
		 */
		inline void retain() {
		   m_retainCount.increment();			
		}

		/**
		 * @brief Decrement the retainCount by one.
		 * @return True if there are no more references to the TimedAction.
		 */
		inline Boolean release() {
		   m_retainCount.decrement();	
			if (m_retainCount.val() <= 0) {
				return true;
			}
			return false;
		}

		/**
		 * @brief Mark the TimedAction as having been removed.
		 */
		inline void remove() { m_state = kTASRemoved; }		
		
		/**
		 * @brief Method to indicate the TimedAction has resumed.
		 */
		inline void resume() {
		   m_state = kTASActive;			
		}	

		/**
		 * @brief Get the retain count for the TimedAction.
		 * @return The Retain count for the TimedAction.
		 */
		inline I32 retainCount() const { return m_retainCount.val(); }

		/**
		 * @brief Set the actionID for the TimedAction.
		 * @param actionID The actionID assigned by the Timer.
		 */
		inline void setActionID(U64 actionID) { m_actionID = actionID; }

		/**
		 * @brief Sets the next time the action will fire based on the previous fire time.
		 */
		inline void setNextFireTime() {
			m_nextFireTime = m_nextFireTime + m_timeToWait.raw();
		}

		/**
		 * @brief Sets the next time the action will fire based on the current time.
		 */
		inline void setNextFireTimeFromCurrentTime() {
			m_nextFireTime = Time::currentTimeRaw() + m_timeToWait.raw();
		}

		/**
		 * @brief Sets the next time the action will fire to the specified TimeVal.
		 * @param nextFireTime The next time to fire.
		 */
		inline void setNextFireTime(const TimeVal& nextFireTime) {
			m_nextFireTime = nextFireTime.raw();			
		}

		/**
		 * @brief Set the amount of time to wait before firing.
		 * @return timeToWait The amount of time to wait as a TimeVal.
		 */
		inline void setTimeToWait(const TimeVal& timeToWait) {
			m_timeToWait = timeToWait;
		}		
		
		/**
		 * @brief Check the state of the TimedAction.
		 * @return The current state of the TimedAction.
		 */
		inline TimedActionState state() {  return m_state; }

		/**
		 * @brief Get the amount of time to wait before firing.
		 * @return The amount of time to wait as a TimeVal.
		 */
		inline const TimeVal& timeToWait() const { return m_timeToWait; }		

		/**
		 * @brief Check to see if the TimedAction has been removed from the list.
		 * @return True if the TimedAction was removed from the list.
		 */
		inline Boolean wasRemoved() const {
			return (m_state == kTASRemoved);
		}
		
	  private:
		inline void setState(TimedActionState state)  {
			m_state = state;
		}
	
		U64                        m_actionID;		
		OID                        m_oid;
		Char*                      m_pName;		
		AtomicI32		 			   m_retainCount;
		TimedActionState	  		   m_state;
		TimeVal                    m_timeToWait;
		RawTimeVal                 m_nextFireTime;		
	};

	typedef InvasiveStrongPtr<TimedAction> TimedActionPtr;	

} // namespace Cat

#endif // CAT_CORE_DEFER_TIMEDACTION_H
