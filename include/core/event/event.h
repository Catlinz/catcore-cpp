#ifndef CAT_CORE_EVENT_EVENT_H
#define CAT_CORE_EVENT_EVENT_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file event.h
 * @brief The basic base class for an Event object.
 *
 * @author Catlin Zilinski
 * @date June 3, 2014
 */

#include "core/time/time.h"
namespace Cat {

	/**
	 * @class Event event.h "core/event/event.h"
	 * @brief The basic base class for an Event object.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since June 3, 2014
	 */
	class Event {
	  public:

		/**
		 * @brief An enum of all the possible event types.
		 */
		enum Type {
			kENoEvent = 0x0,
			
			/* Keyboard events */
			kEKeyDown = 0x1,
			kEKeyUp = 0x2,
			kEKeyboardEventMask = 0x3,

			/* mouse events */
			kEMouseDown = 0x4,
			kEMouseUp = 0x8,
			kEMouseDragged = 0xC,
			kEMouseMove = 0x10,
			kEScrollWheel = 0x14,
			kEMouseEventMask = 0x1C,
			
			kEInputEventMask = 0xFF,

			/* UI Events */
			kEUIChangeEvent = 0x100,
			kEUICloseEvent = 0x200,
			kEUIDragEnterEvent = 0x300,
			kEUIDragLeaveEvent = 0x400,
			kEUIDragMoveEvent = 0x500,
			kEUIDropEvent = 0x600,
			kEUIEnterEvent = 0x700,
			kEUIFocusInEvent = 0x800,
			kEUIFocusOutEvent = 0x900,
			kEUIHideEvent = 0xa00,
			kEUILeaveEvent = 0xb00,
			kEUIMoveEvent = 0xc00,
			kEUIResizeEvent = 0xd00,
			kEUIShowEvent = 0xe00,
			kEUIInternalChangeEvent = 0xf00,
			kEUIEventMask = 0xFF00,
			
		};

		/**
		 * @brief Create a null event.
		 */
		inline Event()
			: m_pAcceptedBy(NIL), m_type(kENoEvent), m_accepted(false),
			  m_canCombineMultipleEvents(false), m_shouldPropagate(false) {}		
		
		/**
		 * @brief Create a new event with the specified type.
		 * @param type The type of event we are creating.
		 * @param canCombine Whether or not multiple events can be combined (optional, default = false).
		 * @param shouldPropagate Whether or not this event should be able to propagate. (default = true).
		 */
		inline Event(Type type, Boolean canCombine = false,
						 Boolean shouldPropagate = true)
			: m_pAcceptedBy(NIL), m_type(type), m_accepted(false),
			  m_canCombineMultipleEvents(canCombine),
			  m_shouldPropagate(shouldPropagate) {
			m_time.setToCurrentTime();			
		}

		/**
		 * @brief Create a new event with the specified type and time.
		 * @param type The type of event we are creating.
		 * @param time The time at which the event happened.
		 * @param canCombine Whether or not multiple events can be combined (optional, default = false).
		 * @param shouldPropagate Whether or not this event should be able to propagate. (default = true).
		 */
		inline Event(Type type, const TimeVal& time, Boolean canCombine = false,
						 Boolean shouldPropagate = true)
			: m_pAcceptedBy(NIL), m_type(type), m_time(time), m_accepted(false),
			  m_canCombineMultipleEvents(canCombine),
			m_shouldPropagate(shouldPropagate) {}

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~Event() {}

		/**
		 * @brief Overloaded equality operator. 
		 * The equality operator compares the type and time only.
		 * @param rval The Event to compare to.
		 * @return True if the two events are equal.
		 */
		virtual inline Boolean operator==(const Event& event) const {
			return m_type == event.m_type && m_time == event.m_time;
		}

		/**
		 * @brief Overloaded inequality operator. 
		 * The equality operator compares the type and time only.
		 * @param rval The Event to compare to.
		 * @return True if the two events are NOT equal.
		 */
		virtual inline Boolean operator!=(const Event& event) const {
			return m_type != event.m_type || m_time != event.m_time;
		}
		

		/**
		 * @brief Mark the event as having been accepted by an event handler.
		 */
		inline void accept() {
			m_accepted = true;
		}

		/**
		 * @brief Mark the event as having been accepted by an event handler.
		 * @param object The object that accepted the event.
		 */
		inline void accept(VPtr object) {
			m_pAcceptedBy = object;			
			m_accepted = true;
		}

		/**
		 * @brief Get the object that accepted the event (if any).
		 * @return The object that accepted the event (if any).
		 */
		inline VPtr acceptedBy() const {
			return m_pAcceptedBy;
		}		

		/**
		 * @brief Check to see if we can combine mulitple events of this type.
		 * @return True if we can combine multiple events of this type.
		 */
		inline Boolean canCombineMultipleEvents() const {
			return m_canCombineMultipleEvents;
		}

		/**
		 * @brief Method to implement to be able to combine multiple events.
		 * This method combines the argument, a later event of the same time, 
		 * with this event.
		 * @param event The other later event to combine with this one.
		 */
		virtual void combine(Event* event) {}		

		/**
		 * @brief Mark the event as NOT having been accepted by an event handler.
		 */
		inline void ignore() {
			m_accepted = false;
		}

		/**
		 * @brief Check to see if the event has been handled or not.
		 * @return True if the event has been handled.
		 */
		inline Boolean isAccepted() const {
			return m_accepted;
		}

		/**
		 * @brief Check to see if the event is an input event.
		 * @return True if the event is an input event.
		 */
		inline Boolean isInputEvent() const {
			return (m_type & kEInputEventMask) != 0;
		}

		/**
		 * @brief Check to see if the event is a keyboard event.
		 * @return True if the event is a keyboard event.
		 */
		inline Boolean isKeyboardEvent() const {
			return (m_type & kEKeyboardEventMask) != 0;
		}

		/**
		 * @brief Check to see if we actually have an event or not.
		 * @return True if the event type is kENone.
		 */
		inline Boolean isNull() const {
			return m_type == kENoEvent;
		}		

		/**
		 * @brief Check to see if the event is a mouse event.
		 * @return True if the event is a mouse event.
		 */
		inline Boolean isMouseEvent() const {
			return (m_type & kEMouseEventMask) != 0;
		}

		/**
		 * @brief Check to see if the event is a UI event.
		 * @return True if the event is a UI event.
		 */
		inline Boolean isUIEvent() const {
			return (m_type & kEUIEventMask) != 0;
		}

		/**
		 * @brief Check to see whether or not the event should propagate.
		 * @return true If the event should propagate.
		 */
		inline Boolean shouldPropagate() const {
			return (!m_accepted && m_shouldPropagate);
		}

		/**
		 * @brief Stop the event from propagating upwards.
		 */
		inline void stopPropagation() {
			m_shouldPropagate = false;
		}

		/**
		 * @brief Get the type at which the event occured.
		 * @return The time at which the event occurred.
		 */
		inline const TimeVal& time() const {
			return m_time;
		}		

		/**
		 * @brief Get the type of the event.
		 * @return The type of the event.
		 */
		inline Type type() const {
			return m_type;
		}

		/**
		 * @brief Convert an event type enum value to a string and return it
		 * @param type The event type.
		 * @return The const string representing the event type.
		 */
		static const Char* typeToString(Type type);

	  protected:
		VPtr m_pAcceptedBy;		
		Type m_type;
		TimeVal m_time;		
		Boolean m_accepted;
		Boolean m_canCombineMultipleEvents;
		Boolean m_shouldPropagate;
	};

#ifdef DEBUG
	std::ostream& operator<<(std::ostream& out, const Event& e);	
#endif // DEBUG

} // namespace Cat

#endif // CAT_CORE_EVENT_EVENT_H

		

		
