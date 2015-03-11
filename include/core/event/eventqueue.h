#ifndef CAT_CORE_EVENT_EVENTQUEUE_H
#define CAT_CORE_EVENT_EVENTQUEUE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file eventqueue.h
 *	@brief Allows use of a Event queue for processing events.
 *
 * @author Catlin Zilinski
 * @date June 6, 2014
 */

#include "core/event/event.h"
#include "core/util/simplequeue.h"
#include "core/threading/spinlock.h"


namespace Cat {

	/**
	 * @interface EventQueue eventqueue.h "core/event/eventqueue.h"
	 *	@brief Allows use of a Event queue for processing events.
	 *
	 * The EventQueue class allows for posting of events.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since June 6, 2014
	 */
	class EventQueue {		
	  public:
			
		/**
		 * @brief Create an empty EventQueue.
		 */
		EventQueue()
			: m_pCurrentEventQueue(NIL), m_pProcessing(NIL) {
		}

		/**
		 * @brief Create a new EventQueue with the specified capacity.
		 * @param capacity The number of events this queue can handle.
		 */
		EventQueue(Size capacity);

		/**
		 * @brief Destroys all unprocessed events.
		 */
		~EventQueue();

		/**
		 * @brief Post a event to the Queue to be processes next run.
		 * @param event The event to add to the queue.
		 * @return True if the event was added to the queue.
		 */
		inline Boolean postEvent(Event* event) {
			Boolean success = false;			
			m_lock.lock();
			if (event->canCombineMultipleEvents() &&
				 !m_pCurrentEventQueue->isEmpty() && 
				 m_pCurrentEventQueue->peekLast()->type() == event->type()) {
				m_pCurrentEventQueue->peekLast()->combine(event);
				delete event;
			   success = true;				
			}
			else {				
				success = m_pCurrentEventQueue->push(event);
			}			
			m_lock.unlock();
			return success;			
		}

		/**
		 * @brief Process any events on the queue.
		 */
		void processEvents();

		/**
		 * @brief Handle a specified event without putting it on the queue first.
		 * @param event The Event to handle.
		 * @return true if the event is handled (should always be true I thinks).
		 */
		Boolean handleEvent(Event* event);		
			

#if defined (DEBUG)
		SimpleQueue<Event*>* eventsOne() { return &m_eventQueueOne; }
		SimpleQueue<Event*>* eventsTwo() { return &m_eventQueueTwo; }
		SimpleQueue<Event*>* events() { return m_pCurrentEventQueue; }
		SimpleQueue<Event*>* processing() { return m_pProcessing; }
#endif // DEBUG

		/**
		 * @brief Method to destroy the global event queue.
		 */
		static void destroyGlobalEventQueue();

		/**
		 * @brief Get the global event queue.
		 * @return A pointer to the global event queue.
		 */
		static inline EventQueue* global() {
			return s_pGlobalEventQueue;
		}		

		/**
		 * @brief Method to initialise the global event queue.
		 * @param capacity The capacity for the global event queue to have.
		 */		
		static void initialiseGlobalEventQueue(Size capacity);
		

	  private:
		SimpleQueue<Event*>* m_pCurrentEventQueue;
		SimpleQueue<Event*>* m_pProcessing;
		
		Spinlock m_lock;		
		SimpleQueue<Event*> m_eventQueueOne;
		SimpleQueue<Event*> m_eventQueueTwo;

		static EventQueue* s_pGlobalEventQueue;		

	};

} // namepsace cc

#endif // CAT_CORE_EVENT_EVENTQUEUE_H
