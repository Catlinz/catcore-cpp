#ifndef CAT_CORE_THREADING_UNIX_RUNNABLE_H
#define CAT_CORE_THREADING_UNIX_RUNNABLE_H
/**
 * @copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file runnable.h
 *
 * Contains the Interface for the Runnable types and the 
 * RunnableFunc class for running functions in new threads.
 *
 * @author Catlin Zilinski
 * @date Nov 27, 2013
 */

#include "core/threading/unix/threaddefs.h"

namespace Cat {
	
	/**
	 * @interface Runnable runnable.h "core/threading/runnable.h"
    * @brief An interface for running something in a separate thread.
	 *
	 * The Runnable interface contains the methods a class must implement to 
	 * be able to be run in a new thread.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since June
	 */
	class Runnable {
	  public:

		/**
		 * @brief Default constructor to initialize to no error and not destroyable.
		 */
		Runnable() 
			: m_errno(0), m_destroyable(false) {}
		
		/**
		 * @brief The destructor, just calls the destroy() method.
		 */
		virtual ~Runnable() { destroy(); }

		/**
		 * @brief Called to run the object in a thread.
		 * @return A status code of some kind.
		 */
		virtual I32 run() = 0;
		/**
		 * @brief Method to clean up the Runnable to be deleted.
		 * The method must be safe to be called more than once.
		 */
		virtual void destroy() {}

		/**
		 * @brief Method to see if the Runnable should be deleted after it finishes running.
		 */
		virtual Boolean isDestroyable() const { return m_destroyable; }

		/**
		 * @brief Return a pointer ot the ThreadHandle associated with this Runnable.
		 * @return A Pointer to the ThreadHandle of the thread running the Runnable.
		 */
		inline ThreadHandle* getThread() { return &m_thread; }
		
		/**
		 * @brief Sets the thread handle to the specified handle.
		 * @param handle The ThreadHandle to set it to.
		 */
		inline void setThread(ThreadHandle handle) { m_thread = handle; }
		
		/**
		 * @brief Returns the error code for this Runnable, if there is one.
		 * @return The currently set error value.
		 */
		inline I32 getError() const { return m_errno; }
		
		/**
		 * @brief Sets the current error value for this Runnable.
		 * @param error The error value to set the current error value to.
		 */
		inline void setError(I32 error) { m_errno = error; }
		
		/**
		 * @brief A method used to get identifying information about the Runnable for 
		 * use in debugging threading.
		 * @return An identifying string for the Runnable.
		 */
		virtual char* getInfo() const;

	  protected:
		inline void setDestroyable(Boolean destroyable) { m_destroyable = destroyable; }

	  private:
		I32					m_errno;
		Boolean				m_destroyable;
		ThreadHandle		m_thread;				
	};

	/**
	 * A simple class to encapsulate a function to run in a new thread.
	 */
	class RunnableFunc : public Runnable {
	  public:
		/**
		 * Creates a new RunnableFunc using the specified function and optional data
		 * @param func The function that will be run.
		 * @param data The optional data to pass to func.
		 * @param destroyDataOnFinish If true, any data passed in will be deleted after the thread terminates.
		 */
		RunnableFunc(I32 (*func)(VPtr), VPtr data = NIL, Boolean destroyDataOnFinish = false);
		~RunnableFunc();

		I32 run();
		void destroy();

		char* getInfo() const;

		/**
		 * Simple helper static method to create wrap a function in a RunnableFunc that is destroyed 
		 * when the function finishes.
		 * @param func The function to run in a new thread.
		 * @param data The data to pass to the function (optional).
		 * @return A pointer to a new RunnableFunc.
		 */
		static RunnableFunc* createRunnableFuncToDestroyOnCompletion(I32 (*func)(VPtr), VPtr data = NIL);

	  private:
		VPtr				data_;	/**< The data to be passed to func */
		I32 (*func_)(VPtr);		/**< The function to be run */
	};

	/**
	 * The entry point for a thread running an IRunnable object
	 */
	VPtr runnable_func_entry__(VPtr data);

} // namespace Cat

#ifdef DEBUG
std::ostream& operator<<(std::ostream& out, cc::Runnable* runnable);
#endif //DEBUG



#endif // CAT_CORE_THREADING_UNIX_RUNNABLE_H

