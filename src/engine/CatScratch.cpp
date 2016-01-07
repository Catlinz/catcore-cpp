#include "engine/CatScratch.h"

namespace cat {

	CatScratchEngine *CatScratch = 0;

	CatScratchEngine::CatScratchEngine() {}

	CxInputHandler * CatScratchEngine::popKeyPressInputHandler() {
		CxInputHandler *h = mp_keyPress;
		if (h != 0) { mp_keyPress = h->next(); }  return h;
	}
	CxInputHandler * CatScratchEngine::popKeyCharInputHandler() {
		CxInputHandler *h = mp_keyChar;
		if (h != 0) { mp_keyChar = h->next(); }  return h;
	}

	CxInputHandler * CatScratchEngine::popMouseClickInputHandler() {
		CxInputHandler *h = mp_mouseClick;
		if (h != 0) { mp_mouseClick = h->next(); }  return h;
	}
	CxInputHandler * CatScratchEngine::popScrollInputHandler() {
		CxInputHandler *h = mp_scroll;
		if (h != 0) { mp_scroll = h->next(); }  return h;
	}
	CxInputHandler * CatScratchEngine::popMouseMoveInputHandler() {
		CxInputHandler *h = mp_mouseMove;
		if (h != 0) { mp_mouseMove = h->next(); }  return h;
	}
	CxInputHandler * CatScratchEngine::popMouseEnterInputHandler() {
		CxInputHandler *h = mp_mouseEnter;
		if (h != 0) { mp_mouseEnter = h->next(); }  return h;
	}
	CxInputHandler * CatScratchEngine::popMouseLeaveInputHandler() {
		CxInputHandler *h = mp_mouseLeave;
		if (h != 0) { mp_mouseLeave = h->next(); }  return h;
	}
		
	CxInputHandler * CatScratchEngine::popResizeInputHandler() {
		CxInputHandler *h = mp_resize;
		if (h != 0) { mp_resize = h->next(); }  return h;
	}

	void CatScratchEngine::processInput() {
		/* Swap the read and write queues. */
		m_inputQ.swap();

		/* process all the events. */
		while (!m_inputQ.isReadEmpty()) {
			CxInputEvent &e = m_inputQ.peekRead();

			switch(e.type()) {
			case kCxKeyPressEvent:
				if (mp_keyboard->input(e.key.keyCode, e.key.action, e.modifiers())) {
					if (mp_keyPress != 0) { mp_keyPress->input(e); }
				}
				break;
				
			case kCxKeyCharEvent:
				break;

			case kCxMouseClickEvent:
				break;

			case kCxMouseMoveEvent:
				break;

			case kCxMouseScrollEvent:
				break;

			case kCxMouseEnterEvent:
				break;

			case kCxMouseLeaveEvent:
				break;

			case kCxDeviceChangeEvent:
				break;

			case kCxDPIChangeEvent:
				break;

			case kCxSetCursorEvent:
				break;

			case kCxWindowCreateEvent:
				break;

			case kCxWindowGotFocusEvent:
				break;

			case kCxWindowLostFocusEvent:
				break;

			case kCxWindowCloseRequestEvent:
				break;

			case kCxWindowMinimizeEvent:
				break;

			case kCxWindowMaximizeEvent:
				break;

			case kCxWindowRestoreEvent:
				break;

			case kCxWindowResizeEvent:
				break;

			case kCxWindowMoveEvent:
				break;

			default:
				CXD_ERR("Unknown event (%d) found in event queue!.", e.type());
				break;
			}

			m_inputQ.nextRead();
		}
	}

	void CatScratchEngine::registerKeyPressInputHandler(CxInputHandler *in_h) {
	   mp_keyPress = in_h->push(mp_keyPress);
	}
	void CatScratchEngine::registerKeyCharInputHandler(CxInputHandler *in_h) {
		mp_keyChar = in_h->push(mp_keyChar);
	}

	void CatScratchEngine::registerMouseClickInputHandler(CxInputHandler *in_h) {
		mp_mouseClick = in_h->push(mp_mouseClick);
	}
	void CatScratchEngine::registerScrollInputHandler(CxInputHandler *in_h) {
		mp_scroll = in_h->push(mp_scroll);
	}
	void CatScratchEngine::registerMouseMoveInputHandler(CxInputHandler *in_h) {
		mp_mouseMove = in_h->push(mp_mouseMove);
	}
	void CatScratchEngine::registerMouseEnterInputHandler(CxInputHandler *in_h) {
		mp_mouseEnter = in_h->push(mp_mouseEnter);
	}
	void CatScratchEngine::registerMouseLeaveInputHandler(CxInputHandler *in_h) {
		mp_mouseLeave = in_h->push(mp_mouseLeave);
	}
		
	void CatScratchEngine::registerResizeInputHandler(CxInputHandler *in_h) {
		mp_resize = in_h->push(mp_resize);
	}

	CatScratchEngine * CatScratchEngine::createInstance() {
		CatScratchEngine *e = new CatScratchEngine();
		
		e->initializeInput();

		return e;
	}

	CxBool CatScratchEngine::initializeEngine() {
		CatScratch = createInstance();
	}

	void CatScratchEngine::initializeInput() {
		m_inputQ = CxInputQueue(32);
		
		mp_keyPress = mp_keyChar = 0;
		mp_mouseClick = mp_scroll = mp_mouseMove = 0;
		mp_mouseEnter = mp_mouseLeave = 0;
		mp_resize = 0;
	}
	
} // namespace cat
