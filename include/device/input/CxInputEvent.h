#ifndef CX_DEVICE_INPUT_CXINPUTEVENT_H
#define CX_DEVICE_INPUT_CXINPUTEVENT_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxInputEvent.h
 * Classes for storing and processing input events.
 *
 * @author Catlin Zilinski
 * @date Jan 2, 2016
 */

#include "core/Cx.h"

namespace cat {

	struct CxKeyEvent {
		CxI32 scanCode;
		CxI8 keyCode;
		CxI8 action;
		CxI8 _padding[2];
	};

	struct CxMouseEvent {
		CxF64 x, y;
		CxI8 button;
		CxI8 action;
		CxI8 _padding[2];
	};

	/**
	 * @class CxInputEvent CxInputEvent.h "device/input/CxInputEvent.h"
	 * 
	 * The CxInputEvent stores a union of the different types of input events.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Jan 2, 2016
	 */
	class CxInputEvent {
	  public:
		/** @brief A union that holds all the different types of events. */
		union EventUnion {
			CxKeyEvent key;
			CxMouseEvent mouse;
		};

	  private:
		CxWindow *mp_window;
		EventUnion m_event;
		CxI8 m_type;
		CxI8 m_mods;
		
	};

} // namespace cat

#endif // CX_DEVICE_INPUT_CXINPUTEVENT_H
