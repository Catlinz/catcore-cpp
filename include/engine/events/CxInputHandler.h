#ifndef CX_ENGINE_EVENTS_CXINPUTHANDLER_H
#define CX_ENGINE_EVENTS_CXINPUTHANDLER_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxInputHandler.h
 * Base class for handling input events.
 *
 * @author Catlin Zilinski
 * @date Jan 7, 2016
 */

#include "core/Cx.h"

namespace cat {

	class CxInputEvent;

	/**
	 * @class CxInputHandler CxInputHandler.h "engine/events/CxInputHandler.h"
	 * 
	 * The CxInputHandler is the base class of the handlers for 
	 * input events.  
	 *
	 * The input handlers work like a stack, so that when a input 
	 * handler is registered for a specific input event, it is 
	 * put ontop of the stack of input handlers for that event, 
	 * and if the handler on the top of the stack doesn't handle the
	 * event, then it gets passed to the next handler on the stack, 
	 * until either the event is handled or we reach the bottom of
	 * the stack.  This way we can override only part of the 
	 * functionality of a handler (e.g., override how only certain
	 * key presses are handled) and don't have to reimplement everything.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Jan 7, 2016
	 */
	class CxInputHandler {
	  public:
		/** @brief Default constructor to initialise to zero */
		CxInputHandler();

		/** @brief Virtual destructor for sub classes */
		virtual ~CxInputHandler();

		/**
		 * @brief Pass event to the event handler stack.
		 * @param in_e The event to handle.
		 * @return True if any of the handlers in the stack handled the event.
		 */
		CxBool input(const CxInputEvent &in_e);

		/** @return The next handler on the stack. */
		CX_FORCE_INLINE CxInputHandler * next() { return mp_next; }

		/**
		 * @brief Push this handler onto the top of a stack.
		 * @param in_top The previous top of the stack.
		 * @return A pointer to this handler (now the top of the stack).
		 */
		CxInputHandler * push(CxInputHandler *in_top);
		
	  protected:
		CxInputHandler *mp_next;

		/**
		 * @brief Method for subclasses to implement to handle events.
		 * @param in_e The event to handle.
		 * @return True if the event was handled by the handler.
		 */
		virtual CxBool handleInput(const CxInputEvent &in_e) = 0;
	};

	CX_FORCE_INLINE CxBool CxInputHandler::input(const CxInputEvent &in_e) {
		if (handleInput(in_e)) { return true; }
		else if (mp_next != 0) {
			CxInputHandler *h = mp_next;
			do {
				if (h->handleInput(in_e)) { return true; }
				else { h = h->mp_next; }
			} while (h);
			
			return false;
		}
	}
		
} // namespace cat

#endif // CX_ENGINE_EVENTS_CXINPUTHANDLER_H
