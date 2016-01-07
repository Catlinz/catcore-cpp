#ifndef CX_ENGINE_EVENTS_CXINPUTEVENT_H
#define CX_ENGINE_EVENTS_CXINPUTEVENT_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxInputEvent.h
 * Classes for storing and processing input events.
 *
 * @author Catlin Zilinski
 * @date Jan 3, 2016
 */

#include "core/Cx.h"

namespace cat {

	/** @brief The types of input events. */
	enum CxInputEventType {
		/* System event types */
		kCxDeviceChangeEvent = 0x1,
		kCxDPIChangeEvent = 0x2,

		kCxSetCursorEvent = 0x21,
			
			
		kCxWindowCreateEvent = 0x30,
		kCxWindowGotFocusEvent = 0x31,
		kCxWindowLostFocusEvent = 0x32,
		kCxWindowCloseRequestEvent = 0x33,
		kCxWindowMinimizeEvent = 0x34,
		kCxWindowMaximizeEvent = 0x35,
		kCxWindowRestoreEvent = 0x36,
		kCxWindowResizeEvent = 0x37,
		kCxWindowMoveEvent = 0x38,

		/* Keyboard input events */
		kCxKeyPressEvent = 0x60,
		kCxKeyCharEvent = 0x61,
			
		/* Mouse input events */
		kCxMouseClickEvent = 0x90,
		kCxMouseScrollEvent = 0x91,
		kCxMouseMoveEvent = 0x92,
		
		kCxMouseEnterEvent = 0x93,
		kCxMouseLeaveEvent = 0x94,
	};
		

	struct CxKeyEvent {
		CxI32 scanCode;
		CxI8 keyCode;
		CxI8 action;
		CxI8 _padding[2];
	};

	struct CxCharEvent {
		CxI32 codepoint;
	};

	struct CxMouseEvent {
		CxF64 x, y;
		CxI8 button;
		CxI8 action;
		CxI8 _padding[2];
	};

	struct CxWindowEvent {
		CxI32 x;
		CxI32 y;
	};
	

	/**
	 * @class CxInputEvent CxInputEvent.h "engine/events/CxInputEvent.h"
	 * 
	 * The CxInputEvent stores a union of the different types of input events.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Jan 3, 2016
	 */
	class CxInputEvent {
	  public:
		/** @brief A union that holds all the different types of events. */
		union EventUnion {
			CxKeyEvent key;
			CxCharEvent chr;
			CxMouseEvent mouse;
			CxWindowEvent win;
		};

		/** @brief Create a null event */
		CxInputEvent();

		/** @return A reference to the data for the input event. */
		CX_FORCE_INLINE EventUnion & data() { return m_event; }
		CX_FORCE_INLINE const EventUnion & data() const { return m_event; }
		
		/** @return The keyboard modifiers active during the event. */
		CX_FORCE_INLINE CxI8 modifiers() const { return m_mods; }
		
		/** @return The type of the event. */
		CX_FORCE_INLINE CxI8 type() const { return m_type; }

		/**
		 * @param in_type The type of event.
		 * @return A new event of the given type with no additional data set.
		 */
		CX_FORCE_INLINE static CxInputEvent event(CxI8 in_type) {
			return CxInputEvent(in_type);
		}

		/** 
		 * @param in_scanCode The scancode for the key (OEM dependant).
		 * @param in_keyCode the mapped key code (CX_KEY_...) for the event.
		 * @param in_action The action for the event (press, release).
		 * @param in_mods The keyboard modifiers active at the time.
		 * @return A new keyboard keypress event. 
		 */
		CX_FORCE_INLINE static CxInputEvent keyPress(CxI32 in_scanCode, CxI8 in_keyCode, CxI8 in_action, CxI8 in_mods) {
			return CxInputEvent(in_scanCode, in_keyCode, in_action, (CxI8)kCxKeyPressEvent, in_mods);
		}

		/**
		 * @param in_char The (more or less) UTF-32 codepoint that was produced.
		 * @param in_mods The keyboard modifiers active at the time.
		 * @return A new keybord character event.
		 */
		CX_FORCE_INLINE static CxInputEvent keyChar(CxI32 in_codepoint, CxI8 in_mods) {
			return CxInputEvent(in_codepoint, (CxI8)kCxKeyCharEvent, in_mods);
		}

		/** 
		 * @param in_button The mouse button responsible for the event.
		 * @param in_action The action for the event (press, release).
		 * @param in_mods The keyboard modifiers active at the time.
		 * @return A new mouse click event. 
		 */
		CX_FORCE_INLINE static CxInputEvent mouseClick(CxI8 in_button, CxI8 in_action, CxI8 in_mods) {
			return CxInputEvent(0.0, 0.0, in_button, in_action, (CxI8)kCxMouseClickEvent, in_mods);
		}

		/** 
		 * @param in_deltaX The amount scrolled on the horizontal axis.
		 * @param in_deltaY The amount scrolled on the vertical axis.
		 * @param in_mods The keyboard modifiers active at the time.
		 * @return A new mouse scroll event.
		 */
		CX_FORCE_INLINE static CxInputEvent scrollWheel(CxF64 in_deltaX, CxF64 in_deltaY, CxI8 in_mods) {
			return CxInputEvent(in_deltaX, in_deltaY, 0, 0, (CxI8)kCxMouseScrollEvent, in_mods);
		}

		/** 
		 * @param in_x The current position or amount moved on the horizontal axis.
		 * @param in_y The current position or amount moved on the vertical axis.
		 * @param in_mods The keyboard modifiers active at the time.
		 * @return A new mouse move event.
		 */
		CX_FORCE_INLINE static CxInputEvent mouseMove(CxF64 in_x, CxF64 in_y, CxI8 in_mods) {
			return CxInputEvent(in_x, in_y, 0, 0, (CxI8)kCxMouseMoveEvent, in_mods);
		}

		/**
		 * @param in_width The new width of the window.
		 * @param in_height The new height of the window.
		 * @return A new window resize event.
		 */
		CX_FORCE_INLINE static CxInputEvent resize(CxI32 in_width, CxI32 in_height) {
			return CxInputEvent(in_width, in_height, (CxI8)kCxWindowResizeEvent);
		}

		/**
		 * @param in_x The new x-position of the window
		 * @param in_y the new y-position of the window.
		 * @return A new window move event.
		 */
		CX_FORCE_INLINE static CxInputEvent move(CxI32 in_x, CxI32 in_y) {
			return CxInputEvent(in_x, in_y, (CxI8)kCxWindowMoveEvent);
		}


	  protected:
		EventUnion m_event;
		CxI8 m_type;
		CxI8 m_mods;

		/** @brief Create a new simple event */
		CX_FORCE_INLINE CxInputEvent(CxI8 in_type) : m_type(in_type), m_mods(0) {}

		/** @brief Create a new keyboard keypress event. */
		CX_FORCE_INLINE CxInputEvent(CxI32 in_scanCode, CxI8 in_keyCode, CxI8 in_action, CxI8 in_type, CxI8 in_mods)
			: m_type(in_type), m_mods(in_mods) {
			m_event.key.scanCode = in_scanCode;
			m_event.key.keyCode = in_keyCode;
			m_event.key.action = in_action;
		}

		/** @brief Create a new keyboard character event. */
		CX_FORCE_INLINE CxInputEvent(CxI32 in_codepoint, CxI8 in_type, CxI8 in_mods)
			: m_type(in_type), m_mods(in_mods) {
			m_event.chr.codepoint = in_codepoint;
		}

		/** @brief Create a new mouse event. */
		CX_FORCE_INLINE CxInputEvent(CxF64 in_x, CxF64 in_y, CxI8 in_button, CxI8 in_action, CxI8 in_type, CxI8 in_mods)
			: m_type(in_type), m_mods(in_mods) {
			m_event.mouse.x = in_x;
			m_event.mouse.y = in_y;
			m_event.mouse.button = in_button;
			m_event.mouse.action = in_action;
		}

		/** @brief Create a new window event */
		CX_FORCE_INLINE CxInputEvent(CxI32 in_x, CxI32 in_y, CxI8 in_type)
			: m_type(in_type), m_mods(0) {
			m_event.win.x = in_x;
			m_event.win.y = in_y;
		}
	};

} // namespace cat

#endif // CX_ENGINE_EVENTS_CXINPUTEVENT_H
