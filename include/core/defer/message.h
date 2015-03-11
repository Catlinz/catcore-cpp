#ifndef CAT_CORE_DEFER_MESSAGE_H
#define CAT_CORE_DEFER_MESSAGE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file message.h
 *	@brief Allows use of a message for defering actions.
 *
 * @author Catlin Zilinski
 * @date Mar 16, 2014
 */

#define MESSAGE_DATA_SIZE 16

#include "core/corelib.h"
#include "core/time/time.h"
#include "core/defer/messagetypes.h"

namespace Cat {

	/**
	 * @interface Message message.h "core/defer/message.h"
	 * @brief A Message allowing for deferred handling of actions.
	 *
	 * The Message class allows for posting of messages / events and 
	 * the assignment of listeners to listen for these messages.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 16, 2014
	 */
	class Message {		
	  public:
		/**
		 * @brief Create a NIL message
		 */
		inline Message()
			: m_type(MessageType::kMTNoMessage),
			  m_time(0), m_pSender(NIL) {}		

		/**
		 * @brief Create a new Message with the specified message type.
		 * @param p_type The type of message.
		 * @param p_data The optional data to pass to the message hander.
		 */
		Message(I32 p_type, Byte* p_data = NIL);

		/**
		 * @brief Create a new Message with the specified message type.
		 * @param type The type of message.
		 * @param sender The object that send the message.
		 * @param data The optional data to pass to the message hander.
		 */
		Message(I32 p_type, void* p_sender, Byte* p_data = NIL);

		/**
		 * @brief Overloaded equality operator.  Just checks type.
		 * @return True if the messages are of the same type.
		 */
		inline Boolean operator==(const Message& rval) const {
			return m_type == rval.m_type;
		}

			/**
		 * @brief Overloaded INequality operator.  Just checks type.
		 * @return True if the messages are NOT of the same type.
		 */
		inline Boolean operator!=(const Message& rval) const {
			return m_type != rval.m_type;
		}	

		/**
		 * @brief Get the Data pointer for the Message.
		 * @return The Data pointer for the message.
		 */
		inline Byte* data() { return m_pData; }

		/** 
		 * @brief Check to see if the message is from a specified sender.
		 * @param sender The object to check if it is the Sender.
		 * @return True if sender send the message.
		 */
		inline Boolean isFromSender(VPtr sender) const {
			return m_pSender == sender;
		}		

		/**
		 * @brief Get the Data pointer for the Message.
		 * @return The Data pointer for the message.
		 */
		inline Byte* readData() { return m_pData; }

		/**
		 * @brief Get the sender object of the message, if any.
		 * @return A pointer to the object that sent the message, or NIL.
		 */
		inline VPtr sender() { return m_pSender; }		

		/**
		 * @brief Set the data stored in the message.
		 * @param p_data A void pointer to kMessageDataSize bytes of memory.
		 */
		void setData(Byte* p_data);

		/**
		 * @brief Get the time the message was posted.
		 * @return The time the message was posted.
		 */
		inline U64 time() const { return m_time; }	

		/**
		 * @brief Get the type of the message.
		 * @return The type of the message.
		 */
		inline I32 type() const { return m_type; }

	  private:
		I32 m_type;
		U64 m_time;
		VPtr m_pSender;		
		Byte m_pData[MESSAGE_DATA_SIZE];
		
		
	};
	
} // namepsace cc

#endif // CAT_CORE_DEFER_MESSAGE_H
