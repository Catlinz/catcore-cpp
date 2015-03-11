#include "core/defer/timer.h"

namespace Cat {

	Timer::Timer(Size queueSize, Size averageNumActions) {
		m_nextActionID = 0;		
		m_singularInputQueue.initWithCapacity(queueSize, TimedActionPtr::nullPtr());
		m_repeatedInputQueue.initWithCapacity(queueSize, TimedActionPtr::nullPtr());
		m_messageQueue.initWithCapacity(averageNumActions*1.5 + (queueSize*2),
												  InternalMessage1Arg<Number64>());		
		m_nodeStore.initWithBlockSize(averageNumActions);		
		m_singular.initAsRoot();
		m_repeated.initAsRoot();		
	}

	Timer::~Timer() {
		m_singularInputQueue.clear();
		m_repeatedInputQueue.clear();
			
		m_singular.initAsRoot();
		m_repeated.initAsRoot();		
	}

	void Timer::consumeInputQueues() {
		PtrNode<TimedActionPtr>* node;		
		/* Put any queued singular actions on the the running queue. */
		while (!m_singularInputQueue.isEmpty()) {
			node = m_nodeStore.alloc(&m_singular, m_singularInputQueue.pop());
			node->ptr->initialize();
			node->ptr->onInitialize();
		}

		/* Put any queued repeated actions on the the running queue. */
		while (!m_repeatedInputQueue.isEmpty()) {
			node = m_nodeStore.alloc(&m_repeated, m_repeatedInputQueue.pop());
			node->ptr->initialize();
			node->ptr->onInitialize();
		}

	}	
	  
	
	void Timer::processMessages() {

		while (!m_messageQueue.isEmpty()) {
			InternalMessage1Arg<Number64> message = m_messageQueue.pop();
			switch(message.typeID) {
			case kTMRemoveSingularAction:
				m_lock.lock();
				findAndRemoveSingularAction(message.arg1.u64);				
				m_lock.unlock();				
				break;
			case kTMRemoveRepeatedAction:
				m_lock.lock();
				findAndRemoveRepeatedAction(message.arg1.u64);
				m_lock.unlock();				
				break;
			default:
				DWARN("Unrecognized message typeID: " << message.typeID << "!");				
				break;
			}			
		}
	}
	

	void Timer::tick() {

		PtrNode<TimedActionPtr>* node;

		/* Put any queued singular actions on the the running queue. */
		while (!m_singularInputQueue.isEmpty()) {
			node = m_nodeStore.alloc(&m_singular, m_singularInputQueue.pop());
			node->ptr->initialize();
			node->ptr->onInitialize();
		}

		/* Put any queued repeated actions on the the running queue. */
		while (!m_repeatedInputQueue.isEmpty()) {
			node = m_nodeStore.alloc(&m_repeated, m_repeatedInputQueue.pop());
			node->ptr->initialize();
			node->ptr->onInitialize();
		}

		/* Process any waiting messages */
		processMessages();

		/* Check the singular timers first */
	   node = m_singular.next;
		PtrNode<TimedActionPtr>* next = NIL;		
		while (node != &m_singular) {
			next = node->next;			
			if (node->ptr->shouldFire()) {				
				node->ptr->fire();
				m_nodeStore.free(node);				
			}
			node = next;
		}

		/* Now check the repeated timers. */
		node = m_repeated.next;
		while (node != &m_repeated) {
			next = node->next;
			if (node->ptr->shouldFire()) {				
				if (node->ptr->fire()) { 
					node->ptr->setNextFireTime();					
				} else { /* Should remove if returns false */
					m_nodeStore.free(node);
				}			  
			}
			node = next;			
		}
	}

	void Timer::findAndRemoveSingularAction(U64 actionID) {
		PtrNode<TimedActionPtr>* node = m_singular.next;
		PtrNode<TimedActionPtr>* next = NIL;
		while (node != &m_singular) {
			next = node->next;
			if (node->ptr->actionID() == actionID) {
				node->ptr->remove();				
				m_nodeStore.free(node);
				return;				
			}
			node = next;
		}
	}

	void Timer::findAndRemoveRepeatedAction(U64 actionID) {
		PtrNode<TimedActionPtr>* node = m_repeated.next;
		PtrNode<TimedActionPtr>* next = NIL;		
		while (node != &m_repeated) {
			next = node->next;			
			if (node->ptr->actionID() == actionID) {
				node->ptr->remove();
				m_nodeStore.free(node);
				return;				
			}
			node = next;			
		}
	}
	
		

} // namespace Cat
