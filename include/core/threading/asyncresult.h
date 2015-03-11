#ifndef CAT_CORE_THREADING_ASYNCRESULT_H
#define CAT_CORE_THREADING_ASYNCRESULT_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *	
 *	@file asyncresult.h
 *	@brief Contains the interface for the AsyncResult.
 * 
 * An AsyncResult is attached to an 
 * AsyncTask that is run by an AsyncTaskRunner.
 *
 * @author: Catlin Zilinski
 * @date: Sept 20, 2013
 */

#include "core/corelib.h"
#include "core/threading/conditionvariable.h"

namespace Cat {

	class AsyncTask;

	/**
	 * @interface AsyncResult asyncresult.h "core/threading/asyncresult.h"
	 * @brief The AsyncResult interface defines the result of an Asynchronous operation.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Sept 20, 2013
	 */
	class AsyncResult {
	  public:
		
		AsyncResult() : m_errno(0), m_complete(false), m_pTask(NIL) {}
		AsyncResult(AsyncTask* task) 
			: m_errno(0), m_complete(false), m_pTask(task) {}
				
		virtual ~AsyncResult() { m_pTask = NIL; }
				
		/**
		 * Waits until the task as finished running and then returns to 
		 * signal that the task has completed.
		 * @return True if the task has completed.
		 */
		virtual Boolean waitForResult();
		
		/**
		 * @brief Tests whether the task has completed or not.
		 * @return True if the task has completed and the results are available.
		 */
		inline Boolean hasResult() { return m_complete; }		
		
		/**
		 * @brief Gets the result of the task.
		 * @return The result of the task.
		 */
		virtual VPtr getResult() = 0;

		/** 
		 * @brief Sets the error value in the result.
		 * @param errno The error value to set it to.
		 */
		inline void setError(I32 error) { m_errno = error; }
		
		/**
		 * @brief Gets the current error value.
		 * @return The current error value.
		 */
		inline I32 getError() const { return m_errno; }
		
		/**
		 * @brief Tests whether the task finished with an error.
		 * @return True if the task finished with an error.
		 */
		inline Boolean hasError() { return m_errno != 0; }
		
		
		/**
		 * @brief Gets the task that the result is associated with.
		 * @return The AsyncTask that the result is associated with.
		 */
		virtual AsyncTask* getTask() { return m_pTask; }
		
		/**
		 * Called to free up memory from the result and / or signal to the task 
		 * that the result has been destroyed.
		 */
		virtual void destroy();
		
		/**
		 * Called when the task is destroyed to notify the Result that the task is 
		 * no longer available.
		 */
		virtual void detach();		

	  protected: 
		inline ConditionVariable& resultSync() { return m_resultSync; }
		inline void setComplete(Boolean complete) { m_complete = complete; }
		inline void setTask(AsyncTask* task) { m_pTask = task; }
				
	  private:
		I32					m_errno;
		Boolean				m_complete;
		AsyncTask*			m_pTask;
		ConditionVariable m_resultSync;
	};

} // namespace Cat

#endif // CAT_CORE_THREADING_ASYNCRESULT_H
