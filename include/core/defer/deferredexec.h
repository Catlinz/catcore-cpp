#ifndef CAT_CORE_DEFER_DEFERREDEXEC_H
#define CAT_CORE_DEFER_DEFERREDEXEC_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file deferredexec.h
 *	@brief Simple queue and object that allow for deferred execution of a function.
 *
 * @author Catlin Zilinski
 * @date Oct 3, 2014
 */

#include "core/util/simplequeue.h"
#include "core/threading/spinlock.h"

namespace Cat {

	/**
	 * @class DeferredExecCall deferredexec.h "core/defer/deferredexec.h"
	 * @brief A DeferredExecCall allowing for deferred handling of function exections.
	 *
	 * The DeferredExecCall stores a static function, an object to give the function and
	 * an IntegralType to send as data.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 3, 2014
	 */
	class DeferredExecCall {
	  public:
		/**
		 * @brief Create a null object.
		 */
		DeferredExecCall();

		/**
		 * @brief Create a deffered exec call with the specified values.
		 * @param p_func The function point to call.
		 * @param p_obj The object to give to the function.
		 * @param p_data The data to give to the function.
		 */
		DeferredExecCall(void (*p_func)(void*, IntegralType),
							  void* p_obj, IntegralType p_data);

		/**
		 * @brief Overloaded equality operator.
		 * @param p_other The other DeferredExecCall to compare to.
		 * @return True if they are equal.
		 */
		inline Boolean operator==(const DeferredExecCall& p_other) const {
			return (m_pFunc == p_other.m_pFunc &&
					  m_pObj == p_other.m_pObj &&
					  m_data.vPtr == p_other.m_data.vPtr);
		}

			/**
		 * @brief Overloaded non-equality operator.
		 * @param p_other The other DeferredExecCall to compare to.
		 * @return True if they are NOT equal.
		 */
		inline Boolean operator!=(const DeferredExecCall& p_other) const {
			return (m_pFunc != p_other.m_pFunc ||
					  m_pObj != p_other.m_pObj ||
					  m_data.vPtr != p_other.m_data.vPtr);
		}

		/**
		 * @brief Method to execute the deferred exection call.
		 */
		inline void execute() {
			m_pFunc(m_pObj, m_data);
		}

		/**
		 * @brief Static helper method to create a deffered call with void* data.
		 * @param p_func The function to call.
		 * @param p_obj The object to pass to the function.
		 * @param p_data The void* data.
		 * @return A DeferredExecCall object.
		 */
		static inline DeferredExecCall create(void (*p_func)(void*, IntegralType),
									void* p_obj, void* p_data) {
			IntegralType data;
			data.vPtr = p_data;
			return DeferredExecCall(p_func, p_obj, data);
		}
		

	  private:
		void (*m_pFunc)(void*, IntegralType);
	   void* m_pObj;
		IntegralType m_data;
	};
	

	/**
	 * @class DeferredExec deferredexec.h "core/defer/deferredexec.h"
	 * @brief A DeferredExec allowing for deferred handling of function exections.
	 *
	 * The DeferredExec class manages a queue of DeferredExecCall's that 
	 * correspond to function calls.  It just uses static function points and
	 * void pointers for data for speed and simplicity.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 3, 2014
	 */
	class DeferredExec {		
	  public:
		/**
		 * @brief Create an empty DeferredExec queue.
		 */
		DeferredExec();

		/**
		 * @brief Create a new DeferredExec queue with the specified capacity.
		 * @param capacity The number of events this queue can handle.
		 */
		DeferredExec(Size p_capacity);

		/**
		 * @brief Destroys all unexecuted functions.
		 */
		~DeferredExec();

		/**
		 * @brief Get A pointer to the global DeferredExec queue.
		 * @return A pointer to the global DeferredExec (or NIL if not initialised).
		 */
		static inline DeferredExec* global() {
			return s_pGlobal;
		}

		/**
		 * @brief Post a message to the Queue to be processes next run.
		 * @param message The message to add to the queue.
		 * @return True if the message was added to the queue.
		 */
		inline Boolean postCall(const DeferredExecCall& p_call) {
			Boolean success = false;			
			m_lock.lock();			
			success = m_pCalls->push(p_call);
			m_lock.unlock();
			return success;			
		}

		/**
		 * @brief Executes any deferredExecCalls on the queue.
		 */
		void executeCalls();
		
		/**
		 * @brief Static method to initialise the global messaging queue.
		 * @param p_maxMessageTypeId The maximum value of the message type ID that the queue will be expected to handle.
		 */
		static void initialiseGlobalDeferredExecQueue(Size p_capacity);

		/**
		 * @brief Static method to destroy the global messaging queue.
		 */
		static void destroyGlobalDeferredExecQueue();
		
	  private:
		
		Spinlock m_lock;		
		SimpleQueue<DeferredExecCall> m_callsOne;
		SimpleQueue<DeferredExecCall> m_callsTwo;
		SimpleQueue<DeferredExecCall>* m_pCalls;
		SimpleQueue<DeferredExecCall>* m_pProcessing;

		static DeferredExec* s_pGlobal; /**< A global message queue. */

	};

} // namepsace cc

#endif // CAT_CORE_DEFER_DEFERREDEXEC_H
