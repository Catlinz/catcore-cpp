#ifndef CAT_CORE_DEFER_TIMER_H
#define CAT_CORE_DEFER_TIMER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file timer.h
 *	@brief Enables the use of timed actions.
 *
 * @author Catlin Zilinski
 * @date Mar 18, 2014
 */

#include "core/util/simplequeue.h"
#include "core/threading/spinlock.h"
#include "core/defer/timedaction.h"
#include "core/util/internalmessage.h"
#include "core/util/ptrnodestore.h"

namespace Cat {

	/**
	 * @interface Timer timer.h "core/defer/timer.h"
	 * @brief A class allowing for the use of timed actions.
	 *
	 * The Timer class has the ability to create and manage timed actions, 
	 * enabling things to happen after a certain amount of time, or repeat
	 * every period of time.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 18, 2014
	 */
	class Timer {		
	  public:
		enum TimerMessageTypeID {
			kTMNoMessage,			
			kTMRemoveSingularAction,
			kTMRemoveRepeatedAction,
		};
		
		/**
		 * @brief Create a new Timer with the specified attributes.
		 * @param queueSize The number of events that can enter be created at once.
		 */
		Timer(Size queueSize, Size averageNumActions = 32);

		/**
		 * @brief The destructor, makes sure to delete all actions.
		 */
		~Timer();

		/**
		 * @brief Take any waiting actions off the queue and add them.
		 */
		void consumeInputQueues();	

		/**
		 * @brief Process any waiting messages.
		 */
		void processMessages();

		/**
		 * @brief Register a single-shot timer action.
		 * @param action The TimedAction to register as a single-shot action.
		 * @return Non-zero actionID if success, 0 on failure.
		 */
		inline U64 registerSingular(TimedActionPtr& action) {
			U64 retVal = 0;
			action->setNextFireTimeFromCurrentTime();
			m_lock.lock();
			action->setActionID(++m_nextActionID);	
			if (m_singularInputQueue.push(action)) {
				retVal = m_nextActionID;				
			}
#if defined (DEBUG)
			else {				
				DWARN("Failed to register singular action, queue full!");
			}			
#endif /* DEBUG */
			m_lock.unlock();	

			return retVal;
		}

		/**
		 * @brief Register a repeated timer action.
		 * @param action The TimedAction to register as a repeated action.
		 * @return Non-zero actionID if success, 0 on failure.
		 */
		inline U64 registerRepeated(TimedActionPtr& action) {
			U64 retVal = 0;
			action->setNextFireTimeFromCurrentTime();
			m_lock.lock();
			action->setActionID(++m_nextActionID);	
			if (m_repeatedInputQueue.push(action)) {
				retVal = m_nextActionID;				
			}
#if defined (DEBUG)
			else {				
				DWARN("Failed to register repeated action, queue full!");
			}			
#endif /* DEBUG */
			m_lock.unlock();	
			return retVal;
		}

		/**
		 * @brief Check to see if any timers have fired, and if so, deal with them.
		 */
		void tick();	

		/**
		 * @brief Remove a singular action from the Timer by its actionID.
		 * @param actionID The actionID of the action to remove.
		 * @return True if the message was posted to remove the action.
		 */
		inline Boolean unregisterSingular(U64 actionID) {
			Boolean success = false;
			Number64 aID;
			aID.u64 = actionID;			
			m_lock.lock();
			success = m_messageQueue.push(
				InternalMessage1Arg<Number64>(kTMRemoveSingularAction, aID)
				);
			m_lock.unlock();			
#if defined (DEBUG)
			if (!success) {
				DWARN("Failed to unregister singular action, queue full!");
			}
#endif /* DEBUG */
			return success;
		}

		/**
		 * @brief Remove a repeated action from the Timer by its actionID.
		 * @param actionID The actionID of the action to remove.
		 * @return True if the message was posted to remove the action.
		 */
		inline Boolean unregisterRepeated(U64 actionID) {
			Boolean success = false;
			Number64 aID;
			aID.u64 = actionID;			
			m_lock.lock();
			success = m_messageQueue.push(
				InternalMessage1Arg<Number64>(kTMRemoveRepeatedAction, aID)
				);
			m_lock.unlock();			
#if defined (DEBUG)
			if (!success) {
				DWARN("Failed to unregister repeated action, queue full!");
			}
#endif /* DEBUG */
			return success;
		}

#if defined (DEBUG)
		inline SimpleQueue<TimedActionPtr>* singularQueue() { return &m_singularInputQueue; }
		inline SimpleQueue<TimedActionPtr>* repeatedQueue() { return &m_repeatedInputQueue; }
		inline SimpleQueue< InternalMessage1Arg<Number64> >* messageQueue() { return &m_messageQueue; }
		inline PtrNodeStore<TimedActionPtr>* nodeStore() { return &m_nodeStore; }
		inline PtrNode<TimedActionPtr>* singular() { return &m_singular; }
		inline PtrNode<TimedActionPtr>* repeated() { return &m_repeated; }
#endif /* DEBUG */
	  private:
		void findAndRemoveSingularAction(U64 actionID);
		void findAndRemoveRepeatedAction(U64 actionID);		

		U64 m_nextActionID;		
		
		SimpleQueue<TimedActionPtr> m_singularInputQueue;
		SimpleQueue<TimedActionPtr> m_repeatedInputQueue;
		
		SimpleQueue< InternalMessage1Arg<Number64> > m_messageQueue;
		
		PtrNodeStore<TimedActionPtr> m_nodeStore;
		PtrNode<TimedActionPtr> m_singular;
		PtrNode<TimedActionPtr> m_repeated;
		
		Spinlock m_lock;		
		
		

	};

} // namepsace cc

#endif // CAT_CORE_DEFER_MESSAGEQUEUE_H
