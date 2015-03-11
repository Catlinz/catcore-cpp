#include "core/event/eventqueue.h"
#include "device/core/devicemanager.h"
#include "device/mouse/mouse.h"
#include "device/keyboard/keyboard.h"
#include "ui/core/uimanager.h"
#include "engine/catztoy.h"

namespace Cat {

	EventQueue* EventQueue::s_pGlobalEventQueue = NIL;	
	
	EventQueue::EventQueue(Size capacity) {
		m_eventQueueOne.initQueueWithCapacityAndNull(capacity, NIL);
		m_eventQueueTwo.initQueueWithCapacityAndNull(capacity, NIL);
		m_pCurrentEventQueue = &m_eventQueueOne;
		m_pProcessing = &m_eventQueueTwo;
	}

	EventQueue::~EventQueue() {
		m_lock.lock();
		m_eventQueueOne.eraseAll();
		m_eventQueueTwo.eraseAll();		
		m_pCurrentEventQueue = m_pProcessing = NIL;
		m_lock.unlock();		
	}

	void EventQueue::processEvents() {
		/* Swap the queues to prevent infinite queuing */
		SimpleQueue<Event*>* tmpForSwap = m_pProcessing;		
		m_lock.lock();
		m_pProcessing = m_pCurrentEventQueue;
		m_pCurrentEventQueue = tmpForSwap;	
		m_lock.unlock();
		
		Event* event = NIL;		
		while (!m_pProcessing->isEmpty()) {
			event = m_pProcessing->pop();		
			if (event->isKeyboardEvent()) {
				DeviceManager::instance()->keyboard()->handleEvent(event);
			}
			else if (event->isMouseEvent()) {
				DeviceManager::instance()->mouse()->handleEvent(event);
			}
			else if (event->isUIEvent()) {
				CatzToy::instance()->ui()->handleEvent(event);
			}
			else {			
				DERR("Unknown event in queue " << *event << "!");			
			}
			delete event;
		}		
	}

	Boolean EventQueue::handleEvent(Event* event) {
		if (event->isKeyboardEvent()) {
			return DeviceManager::instance()->keyboard()->handleEvent(event);
		}
		else if (event->isMouseEvent()) {
			return DeviceManager::instance()->mouse()->handleEvent(event);
		}
		else if (event->isUIEvent()) {
		   return CatzToy::instance()->ui()->handleEvent(event);
		}
		else {			
			DERR("Unknown event in queue " << event << "!");
			return false;			
		}
	}

	void EventQueue::destroyGlobalEventQueue() {
		if (s_pGlobalEventQueue) {
			delete s_pGlobalEventQueue;
			s_pGlobalEventQueue = NIL;
		}
		else {
			DWARN("Cannot destroy already destroyed global event queue!");
		}
	}	

	void EventQueue::initialiseGlobalEventQueue(Size capacity) {
		if (!s_pGlobalEventQueue) {
			s_pGlobalEventQueue = new EventQueue(capacity);			
		}
		else {
			DWARN("Cannot initialise global event queue more than once!");
		}
	}	
	
} // namespace Cat
