#ifndef CAT_CORE_DEFER_MESSAGEHANDLER_H
#define CAT_CORE_DEFER_MESSAGEHANDLER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file messagehandler.h
 *	@brief Allows use of a message for defering actions.
 *
 * @author Catlin Zilinski
 * @date Apr 12, 2014
 */

#include "core/corelib.h"
#include "core/defer/message.h"

namespace Cat {

	/**
	 * @interface MessageHandler messagehandler.h "core/defer/messagehandler.h"
	 * @brief A MessageHandler allowing for deferred handling of actions.
	 *
	 * The MessageHandler encapuslates a static callback method and an object.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Mar 16, 2014
	 */
	class MessageHandler {		
	  public:
		/**
		 * @brief Create a NIL MessageHandler
		 */
		MessageHandler()
			: m_pObject(NIL), m_pRequiredSender(NIL), m_pFunc(NIL),
			  m_active(false) {}		

		/**
		 * @brief Create a new MessageHandler with the specified message type.
		 * @param type The type of message.
		 * @param func The function to handle the message.
		 */
		MessageHandler(VPtr object, void (*func)(VPtr, Byte*))
			: m_pObject(object), m_pRequiredSender(NIL), m_pFunc(func),
			  m_active(true) {}

		/**
		 * @brief Create a new MessageHandler with the specified message type.
		 * @param type The type of message.
		 * @param requiredSender Only respond to messages from this object.
		 * @param func THe function to handle the message
		 */
		MessageHandler(VPtr object, VPtr requiredSender,
							void (*func)(VPtr, Byte*))
			: m_pObject(object), m_pRequiredSender(requiredSender),
			  m_pFunc(func), m_active(true) {}

		/**
		 * @brief Overloaded equality operator.
		 * @return True if the Message Handlers are the same.
		 */
		inline Boolean operator==(const MessageHandler& rval) const {
			return ((m_pObject == rval.m_pObject) &&
					  (m_pFunc == rval.m_pFunc) &&
					  (m_pRequiredSender == rval.m_pRequiredSender));
		}

			/**
		 * @brief Overloaded INequality operator.  Just checks type.
		 * @return True if the messages are NOT of the same type.
		 */
		inline Boolean operator!=(const MessageHandler& rval) const {
			return ((m_pObject != rval.m_pObject) ||
					  (m_pFunc != rval.m_pFunc) ||
					  (m_pRequiredSender != rval.m_pRequiredSender));
		}	

		/**
		 * @brief Handle a specific message received.
		 * @param message The Message to process.
		 */
		inline void handleMessage(Message& message) {
			m_pFunc(m_pObject, message.readData());
		}

		/**
		 * @brief Check to see if the handler is active or not.
		 * @return True if the handler is active and should be called.
		 */
		inline Boolean isActive() const { return m_active; }

		/**
		 * @brief Set whether or not the handler is active.
		 * @param active Whether or not the handler is active and should be called.
		 */
		inline void setActive(Boolean active) { m_active = active; }

		/**
		 * @brief Check to see if the handler should handle the message or not.
		 * @return True if the handler should handle a message.
		 */
		inline Boolean shouldHandleMessage(VPtr sender) const {
			return (m_active &&
					  (m_pRequiredSender == NIL || (m_pRequiredSender == sender)));
		}		

	  private:
	   VPtr m_pObject;
		VPtr m_pRequiredSender;
		void (*m_pFunc)(VPtr, Byte*);		
		Boolean m_active;
		
	};
	
} // namepsace cc

#endif // CAT_CORE_DEFER_MESSAGEHANDLER_H
