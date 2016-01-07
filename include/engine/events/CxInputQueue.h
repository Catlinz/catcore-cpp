#ifndef CX_ENGINE_EVENTS_CXINPUTQUEUE_H
#define CX_ENGINE_EVENTS_CXINPUTQUEUE_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxInputQueue.h
 * Code for storing received input events until they are ready to be
 * processed.
 *
 * @author Catlin Zilinski
 * @date Jan 3, 2016
 */

#include "core/Cx.h"
#include "core/common/CxDualQueue.h"
#include "engine/events/CxInputEvent.h"

namespace cat {
	
	/**
	 * @class CxInputQueue CxInputQueue.h "engine/events/CxInputQueue.h"
	 * 
	 * The CxInputQueue class receives and stores events for processing
	 * by the application.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Jan 3, 2016
	 */
	class CxInputQueue : public CxDualQueue<CxInputEvent, CxSpinlock> {
	  public:
			/** @brief Create an empty queue */
		CxInputQueue();

		/**
		 * @brief Create a new queue with the specified capacity.
		 * @param in_capacity The capacity to give the queue.
		 */
		CxInputQueue(CxI32 in_capacity);

		/**
		 * @brief Copy constructor, tries to copy the queue.
		 * @param in_src The source queue to create a copy of.
		 */
		CxInputQueue(const CxInputQueue& in_src);

		/**
		 * @brief Move constructor, tries to steal the queue.
		 * @param in_src The source queue to move data from.
		 */
		CxInputQueue(CxInputQueue &&in_src);

		/**
		 * @brief Replaces contents with copy of other queue.
		 * @param in_src The queue to copy from.
		 * @return A reference to this queue.
		 */
		CxInputQueue& operator=(const CxInputQueue& in_src);

		/**
		 * @brief Steals the contents of the source queue.
		 * @param in_src The queue to move data from.
		 * @return A reference to this queue.
		 */
		CxInputQueue& operator=(CxInputQueue &&in_src);

	  private:
	};

} // namespace cat

#endif // CX_ENGINE_EVENTS_CXINPUTQUEUE_H
