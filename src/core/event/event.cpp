#include "core/event/event.h"

namespace Cat {
	const Char* Event::typeToString(Type type) {
		switch(type) {
		case kENoEvent:
			return "kENoEvent";
			break;
		case kEKeyDown:
			return "kEKeyDown";
			break;
		case kEKeyUp:
			return "kEKeyUp";
			break;
		case kEKeyboardEventMask:
			return "kEKeyboardEventMask";
			break;
		case kEMouseDown:
			return "kEMouseDown";
			break;
		case kEMouseUp:
			return "kEMouseUp";
			break;
		case kEMouseDragged:
			return "kEMouseDragged";
			break;
		case kEMouseMove:
			return "kEMouseMove";
			break;
		case kEScrollWheel:
			return "kEScrollWheel";
			break;
		case kEMouseEventMask:
			return "kEMouseEventMask";
			break;
		case kEInputEventMask:
			return "kEInputEventMask";
			break;
		case kEUIChangeEvent:
			return "kEUIChangeEvent";
			break;
		case kEUICloseEvent:
			return "kEUICloseEvent";
			break;
		case kEUIDragEnterEvent:
			return "kEUIDragEnterEvent";
			break;
		case kEUIDragLeaveEvent:
			return "kEUIDragLeaveEvent";
			break;
		case kEUIDragMoveEvent:
			return "kEUIDragMoveEvent";
			break;
		case kEUIDropEvent:
			return "kEUIDropEvent";
			break;
		case kEUIEnterEvent:
			return "kEUIEnterEvent";
			break;
		case kEUIFocusInEvent:
			return "kEUIFocusInEvent";
			break;
		case kEUIFocusOutEvent:
			return "kEUIFocusOutEvent";
			break;
		case kEUIHideEvent:
			return "kEUIHideEvent";
			break;
		case kEUILeaveEvent:
			return "kEUILeaveEvent";
			break;
		case kEUIMoveEvent:
			return "kEUIMoveEvent";
			break;
		case kEUIResizeEvent:
			return "kEUIResizeEvent";
			break;
		case kEUIShowEvent:
			return "kEUIShowEvent";
			break;
		case kEUIInternalChangeEvent:
			return "kEUIInternalChangeEvent";
			break;
		case kEUIEventMask:
			return "kEUIEventMask";
			break;
		}

		return "UnknownEvent";		
	}

#ifdef DEBUG
	std::ostream& operator<<(std::ostream& out, const Event& e) {
		return out << "Event [type: " << Event::typeToString(e.type())
					  << ", time: " << e.time()
					  << ", accepted: " << e.isAccepted()
					  << ", canCombine: " << e.canCombineMultipleEvents()
					  << ", shouldPropagate: " << e.shouldPropagate()
					  << "]";		
	}
#endif //DEBUG
	
} // namespace Cat
