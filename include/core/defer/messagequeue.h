#ifndef CAT_CORE_DEFER_MESSAGEQUEUE_H
#define CAT_CORE_DEFER_MESSAGEQUEUE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file messagequeue.h
 *	@brief Allows use of a message queue for defering actions.
 *
 * @author Catlin Zilinski
 * @date Mar 16, 2014
 */

#include "core/defer/messagehandler.h"
#include "core/util/simplequeue.h"
#include "core/util/list.h"
#include "core/threading/spinlock.h"
#include "core/util/internalmessage.h"


namespace Cat {

	/**
	 * @interface MessageQueue messagequeue.h "core/defer/messagequeue.h"
	 * @brief A MessageQueue allowing for deferred handling of actions.
	 *
	 * The MessageQueue class allows for posting of messages / events and 
	 * the assignment of listeners to listen for these messages.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 16, 2014
	 */
	class MessageQueue {		
	  public:
		enum MQInternalMessageType {
			kMQIMTNoMessage = 0x0,
			kMQIMTRemoveMessageHandler = 0x1,
		};		
			
		/**
		 * @brief Create an empty MessageQueue.
		 */
		MessageQueue();

		/**
		 * @brief Create a new MessageQueue with the specified capacity.
		 * @param capacity The number of events this queue can handle.
		 */
		MessageQueue(U32 capacity, I32 maxMessageTypeID = 64);

		/**
		 * @brief Destroys all unprocessed messages and all handlers.
		 */
		~MessageQueue();

		/**
		 * @brief Get A pointer to the global MessageQueue.
		 * @return A pointer to the global MessageQueue (or NIL if not initialised).
		 */
		static inline MessageQueue* global() {
			return s_pGlobal;
		}

		/**
		 * @brief Get the maxMessageTypeID for the queue.
		 * @return The MaxMessageTypeID for the queue.
		 */
		inline I32 maxMessageTypeID() const {
			return m_maxMessageTypeID;
		}		

		/**
		 * @brief Post a message to the Queue to be processes next run.
		 * @param message The message to add to the queue.
		 * @return True if the message was added to the queue.
		 */
		inline Boolean postMessage(const Message& message) {
			Boolean success = false;			
			m_lock.lock();			
			success = m_pMessages->push(message);
			m_lock.unlock();
			return success;			
		}

		/**
		 * @brief Process any messages on the queue.
		 */
		void processMessages();

		/**
		 * @brief Attach a message handler to handle the specified type of messages.
		 * @param messageTypeID The type of messages to handle.
		 * @param handler The MessageHandler.
		 */
		void registerMessageHandler(I32 messageTypeID, const MessageHandler& handler);
		
		/** 
		 * @brief Remove a message handler from the specified type of messages.
		 * @param messageTypeID The type of messages handler to remove.
		 * @param handler The MessageHandler to remove. 
		 * @return True if the message to remove the handler was posted.
		 */
		Boolean removeMessageHandler(I32 messageTypeID, const MessageHandler& handler);

		/**
		 * @brief Call any listening handlers on the message immediatly.
		 * @param message The Message to handle.
		 */
		void triggerMessage(Message& message);
		inline void triggerMessage(const Message& message) {
			Message msg = message;
			triggerMessage(msg);
		}		
			

#if defined (DEBUG)
		SimpleQueue<Message>* messagesOne() { return &m_messagesOne; }
		SimpleQueue<Message>* messagesTwo() { return &m_messagesTwo; }
		SimpleQueue<Message>* messages() { return m_pMessages; }
		SimpleQueue<Message>* processing() { return m_pProcessing; }
		List<MessageHandler>* handlers() { return m_pHandlers; }
#endif // DEBUG

		/**
		 * @brief Method to process any internal messages, such as adding listeners.
		 */
		void processInternalMessages();


		/**
		 * @brief Static method to initialise the global messaging queue.
		 * @param p_maxMessageTypeId The maximum value of the message type ID that the queue will be expected to handle.
		 */
		static void initialiseGlobalMessageQueue(I32 p_maxMessageTypeId);

		/**
		 * @brief Static method to destroy the global messaging queue.
		 */
		static void destroyGlobalMessageQueue();
		
	  private:
		void initHandlerList();
		void clearAllHandlers();
				
		
		Spinlock m_lock;		
		SimpleQueue<Message> m_messagesOne;
		SimpleQueue<Message> m_messagesTwo;
		SimpleQueue<Message>* m_pMessages;
		SimpleQueue<Message>* m_pProcessing;
		I32 m_maxMessageTypeID;		
		List<MessageHandler>* m_pHandlers;

		SimpleQueue< InternalMessage2Args<I32, MessageHandler> > m_internalMessageQueue;

		static MessageQueue* s_pGlobal; /**< A global message queue. */

	};

} // namepsace cc

#endif // CAT_CORE_DEFER_MESSAGEQUEUE_H
