#ifndef CAT_CORE_DEFER_DEFERREDACTION_H
#define CAT_CORE_DEFER_DEFERREDACTION_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file deferredaction.h
 *	@brief The base class for a DefferedAction.
 *
 * @author Catlin Zilinski
 * @date July 18, 2014
 */

#include "core/corelib.h"

namespace Cat {

	class DeferredAction;	
	
	/**
	 * @class DeferredActionHandler deferredaction.h "core/defer/deferredaction.h"
	 *	@brief The interface for a class to handle deferred actions.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since July 18, 2014
	 */
	class DeferredActionHandler {
	  public:
		/**
		 * @brief Method to implement to handle a DeferredAction.
		 * @param action The DeferredAction to handle.
		 */
		virtual Boolean handleDeferredAction(DeferredAction* action) = 0;
	};		

	/**
	 * @class DeferredAction deferredaction.h "core/defer/deferredaction.h"
	 *	@brief The base class for a DeferredAction.
	 *
	 * A DeferredAction is simply an object that holds an object that
	 * implements the DeferredActionHandler interface, a status, and a value, so that
	 * an action can be taken with regards to that value and/or status. 
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since July 18, 2014
	 */
	class DeferredAction {		
	  public:
		/**
		 * @brief Create an empty deferredAction.
		 */
		DeferredAction()
			: m_pHandler(NIL), m_action(0) {
			m_value.vPtr = NIL;
		}

		/**
		 * @brief Create a new DeferredAction.
		 * @param handler The DeferredActionHandler to handle the action.
		 */
		DeferredAction(DeferredActionHandler* handler)
			: m_pHandler(handler), m_action(0) {
			m_value.vPtr = NIL;
		}

		/**
		 * @brief Create a new DeferredAction with a value.
		 * @param handler The DeferredActionHandler to handle the action.
		 * @param value The IntegralValue to store in the handler.
		 */
		DeferredAction(DeferredActionHandler* handler, IntegralType value)
			: m_pHandler(handler), m_value(value), m_action(0) {
		}

		/**
		 * @brief Virtual destructor so can be subclassed.
		 */
		virtual ~DeferredAction();

		/**
		 * @brief Method to handle the DeferredAction.
		 * @return True if the DeferredAction was handled.
		 */
		inline Boolean handle() {
			D_CONDERR((m_pHandler == NIL),
						 "Cannot call handle() when DeferredAction has null handler!");
			return m_pHandler->handleDeferredAction(this);
		}		

		/**
		 * @brief Get the handler stored in the DeferredAction.
		 * @return The handler stored in the DeferredAction.
		 */
		inline DeferredActionHandler* handler() const {
			return m_pHandler;
		}

		/**
		 * @brief Set the action stored in the DeferredAction.
		 * @param statusValue The value to set the action to.
		 */
		inline void setAction(I32 actionValue) {
			m_action = actionValue;			
		}

		/**
		 * @brief Set the value stored in the DeferredAction.
		 * @param pValue The value to store.
		 */
		inline void setValue(IntegralType pValue) {
			m_value = pValue;			
		}	

		/**
		 * @brief Get the action stored in the DeferredAction.
		 * @return The action stored in the DeferredAction.
		 */
		inline I32 action() const {
			return m_action;
		}	

		/**
		 * @brief Get the value stored in the DeferredAction.
		 * @return The value stored in the DeferredAction.
		 */
		inline IntegralType value() const {
			return m_value;
		}			
		

	  protected:
		DeferredActionHandler* m_pHandler;		
		IntegralType m_value;
		I32 m_action;		
	};

} // namespace Cat

#endif // CAT_CORE_DEFER_DEFERREDACTION_H
