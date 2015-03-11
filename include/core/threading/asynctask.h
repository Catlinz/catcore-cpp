#ifndef CAT_CORE_THREADING_ASYNCTASK_H
#define CAT_CORE_THREADING_ASYNCTASK_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * asynctask.h: Contains the definition for the AsyncTask for running tasks using 
 * an AsyncTaskRunner.
 *
 * Author: Catlin Zilinski
 * Date: July 22, 2013
 */

#include "core/threading/runnable.h"

namespace Cat {

	class AsyncResult;

	/**
	 * The AsyncTask interface gives a Runnable a priority, a obtainable result, 
	 * and methods to execute on success, completion and error.
	 */
	class AsyncTask : public Runnable {
	  public:
		/**
		 * @brief Default constructor just initializes default values for variables.
		 */
		AsyncTask()
			: m_priority(1), m_destroyResultOnDestruction(false), m_pResult(NIL) {}
		
		/**
		 * @brief Default destructor just calls the destroy() method.
		 */
		virtual ~AsyncTask() { destroy(); }	

		/**
		 * @brief Default behaviour of destroy() is just to detach the result from the task.
		 */
		virtual void destroy();

		/**
		 * @brief Default behaviour is to simply set the result to NIL in the task.
		 */
		virtual void resultDestroyed() { m_pResult = NIL; }

		/**
		 * @brief Sets the priority of the task.
		 * @param priority The priority to set to.
		 */
		virtual void setPriority(U32 priority) { m_priority = priority; }
						
		/**
		 * Gets the priority of the task.  
		 * @return The priority of the task.
		 */
		virtual U32 getPriority() const { return m_priority; }
		
		/**
		 * Gets the Async result for the task.
		 * @return The AsyncResult for the task.
		 */
		inline AsyncResult* getResult() { return m_pResult; }
		
		/**
		 * Method to be executed just before the task begins running.
		 * This method is called automatically by the AsyncTaskRunner, the 
		 * AsyncTask's run method should not call this method unless it is re-entrant.
		 */
		virtual void onStart() {}
		/**
		 * Method to be executed on successful completion of the task.  This method will 
		 * not be executed automatically by the AsyncTaskRunner, so should be called at the end of
		 * the run method of the AsyncTask.
		 */
		virtual void onSuccess() {}
		/**
		 * Method to be executed just after the task finishes running.
		 * This method is called automatically by the AsyncTaskRunner, the 
		 * AsyncTask's run method should not call this method unless it is re-entrant.
		 */
		virtual void onCompletion() {}
		/**
		 * Method to be executed on error when running the task.  This method will 
		 * not be executed automatically by the AsyncTaskRunner, so should be called from the run
		 * method of the AsyncTask.  Default behaviour is to set the error value in the result.
		 */
		virtual void onError();
		
		/**
		 * Set whether or not the associated AsyncResult should be destroyed when 
		 * the AsyncTask is destroyed.
		 * @param True if the AsyncResult should be destroyed with the AsyncTask.
		 */
		virtual void setDestroyResultOnTaskDestruction(Boolean destroy) { m_destroyResultOnDestruction = destroy; }
		
		/**
		 * @brief Tells if the result will be destroyed when the task has been destroyed.
		 * @return True if the result will be destroyed on task destruction.
		 */
		virtual Boolean willDestroyResultOnTaskDestruction() const { return m_destroyResultOnDestruction; }
		

	  protected:
		inline void setResult(AsyncResult* result) { m_pResult = result; }
		
				
	  private:
		U32					m_priority;
		Boolean				m_destroyResultOnDestruction;
		AsyncResult*		m_pResult;
		
	};

} // namespace Cat

#endif // CAT_CORE_THREADING_ASYNCTASK_H



