#ifndef CAT_CORE_SIGNAL_SIGNALHANDLER
#define CAT_CORE_SIGNAL_SIGNALHANDLER
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file signalhandler.h
 *	@brief A handler to handle emitting signals.
 *
 * @author Catlin Zilinski
 * @date Apr 30, 2014
 */

#include "core/signal/signaldata.h"

namespace Cat {

	/**
	 * @class SignalHandler signalhandler.h "ui/core/signalhandler.h"
	 *	@brief A handler to handle emitting signals from UI elements.
	 *
	 * The SignalHandler simply stores an object and function pointer 
	 * to send the signal to.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Apr 30, 2014
	 */
	class SignalHandler {		
	  public:
		/**
		 * @brief Create a new NIL SignalHandler.
		 */
		SignalHandler() :
			m_pFunc(NIL), m_pObject(NIL) {}

		/**
		 * @brief Create a new SignalHandler with the specified object and function.
		 * @param func The function pointer to call.
		 * @param obj A pointer to the object to call the slot on.
		 */
		SignalHandler(void (*func)(VPtr, SignalData&), VPtr obj)
			: m_pFunc(func), m_pObject(obj) {}

		/**
		 * @brief Check for equality.
		 * Checks to see if the function pointers point to the same addresses, and
		 * if the object is the same object (or both NIL).
		 * @return True if the SignalHandlers are considered equal.
		 */
		inline Boolean operator==(const SignalHandler& other) {
			return (m_pFunc == other.m_pFunc &&
					  m_pObject == other.m_pObject);
		}

		/**
		 * @brief Check for not equality.
		 * @see operator==(const SignalHandler&)
		 * @return True if the SignalHandlers are considered equal.
		 */
		inline Boolean operator!=(const SignalHandler& other) {
			return (m_pFunc != other.m_pFunc ||
					  m_pObject != other.m_pObject);
		}		

		/**
		 * @brief Call The handler on the specified data.
		 * @param data The SignalData object to pass to the slot.
		 */
		inline void call(SignalData& data) {
			m_pFunc(m_pObject, data);
		}

	  private:
		void (*m_pFunc)(VPtr, SignalData&);
	   VPtr m_pObject;
		
	};
	
} // namepsace cc

#endif // CAT_CORE_SIGNAL_SIGNALHANDLER
