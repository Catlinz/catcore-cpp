#include "core/defer/messagequeue.h"

namespace Cat {

	MessageQueue* MessageQueue::s_pGlobal = NIL;

	MessageQueue::MessageQueue()
		: m_pMessages(NIL), m_pProcessing(NIL), m_maxMessageTypeID(0),
		  m_pHandlers(NIL) {
	}

	MessageQueue::MessageQueue(U32 capacity, I32 maxMessageTypeID) {
		m_messagesOne.initQueueWithCapacityAndNull(capacity, Message());
		m_messagesTwo.initQueueWithCapacityAndNull(capacity, Message());
		m_pMessages = &m_messagesOne;
		m_pProcessing = &m_messagesTwo;
		m_maxMessageTypeID = maxMessageTypeID;
		initHandlerList();
		m_internalMessageQueue.initQueueWithCapacityAndNull(
			capacity,
			InternalMessage2Args<I32, MessageHandler>()
			);
	}

	MessageQueue::~MessageQueue() {
		m_lock.lock();		
		if (m_messagesOne.capacity() > 0) {			
			m_messagesOne.clear();
		}
		if (m_messagesTwo.capacity() > 0) {			
			m_messagesTwo.clear();
		}	  
		m_pMessages = NIL;
		m_pProcessing = NIL;		
		if (m_pHandlers) {			
			delete[] m_pHandlers;
			m_pHandlers = NIL;			
		}
		m_maxMessageTypeID = 0;
		m_lock.unlock();		
	}

	void MessageQueue::processMessages() {
		/* Swap the queues to prevent infinite queuing */
		SimpleQueue<Message>* tmpForSwap = m_pProcessing;		
		m_lock.lock();
		m_pProcessing = m_pMessages;
		m_pMessages = tmpForSwap;	
		m_lock.unlock();
		/* Process any internal messages. */
		processInternalMessages();		

		while (!m_pProcessing->isEmpty()) {
			triggerMessage(m_pProcessing->pop());
		}
	}

	void MessageQueue::triggerMessage(Message& message) {
		if (m_pHandlers[message.type()].size() > 0) {
			List<MessageHandler>::Iterator itr = m_pHandlers[message.type()].begin();
			while (itr.isValid()) {
				if (itr.val().shouldHandleMessage(message.sender())) {
					itr.val().handleMessage(message);
				}
				itr.next();
			}
		}		
	}

	void MessageQueue::initHandlerList() {
		m_pHandlers = new List<MessageHandler>[m_maxMessageTypeID+1];		
		for (I32 i = 0; i <= m_maxMessageTypeID; i++) {
			m_pHandlers[i].setNullValue(MessageHandler());
		}
	}

	void MessageQueue::clearAllHandlers() {
		for (I32 i = 0; i <= m_maxMessageTypeID; i++) {
			m_pHandlers[i].clear();
		}
	}

	void MessageQueue::processInternalMessages() {
		InternalMessage2Args<I32, MessageHandler> message;		
		while (!m_internalMessageQueue.isEmpty()) {
			message = m_internalMessageQueue.pop();
			switch(message.typeID) {
			case kMQIMTRemoveMessageHandler:
				m_lock.lock();
				m_pHandlers[message.arg1].remove(message.arg2);
				m_lock.unlock();
				break;
			default:
				break;
			}
		}
	}

	Boolean MessageQueue::removeMessageHandler(I32 messageTypeID,
															 const MessageHandler& handler) {
#if defined (DEBUG)
		if (messageTypeID > m_maxMessageTypeID) {
			DERR("MessageTypeID "
				  << messageTypeID
				  << " larger than maxMessageTypeID "
				  << m_maxMessageTypeID
				  << " for MessageQueue.  Removal failed!");
			return false;				
		}
#endif /* DEBUG */
		Boolean success = false;			
		m_lock.lock();		  
		success = m_internalMessageQueue.push(
			InternalMessage2Args<I32, MessageHandler>(kMQIMTRemoveMessageHandler,
																	messageTypeID, handler));
		if (success) {
			m_pHandlers[messageTypeID].get(handler).setActive(false);
		}			
		m_lock.unlock();
		return success;			
	}

	void MessageQueue::registerMessageHandler(I32 messageTypeID,
															const MessageHandler& handler) {
#if defined (DEBUG)
		if (messageTypeID > m_maxMessageTypeID) {
			DERR("MessageTypeID "
				  << messageTypeID
				  << " larger than maxMessageTypeID "
				  << m_maxMessageTypeID
				  << " for MessageQueue.  Prepare for explosion!");
		}
#endif /* DEBUG */
		m_lock.lock();			
		m_pHandlers[messageTypeID].append(handler);
		m_lock.unlock();			
	}

	void MessageQueue::initialiseGlobalMessageQueue(I32 p_maxMessageTypeId) {
		if (s_pGlobal) {
			DWARN("Cannot initialise global MessageQueue more than once!");
		}
		else {
			s_pGlobal = new MessageQueue(p_maxMessageTypeId);
		}
	}
	
	void MessageQueue::destroyGlobalMessageQueue() {
		if (s_pGlobal) {
			delete s_pGlobal;
			s_pGlobal = NIL;
		}
		else {
			DWARN("Cannot delete already deleted global MessageQueue!");
		}
	}
	

} // namespace Cat
