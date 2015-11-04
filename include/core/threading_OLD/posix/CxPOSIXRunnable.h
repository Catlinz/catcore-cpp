#ifndef CX_CORE_THREADING_POSIX_CXPOSIXRUNNABLE_H
#define CX_CORE_THREADING_POSIX_CXPOSIXRUNNABLE_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxPOSIXRunnable.h
 *
 * Contains the Interface for the CxRunnable types and the 
 * CxRunnableFunc class for running functions in new threads.
 *
 * @author Catlin Zilinski
 * @date June 28, 2015
 */

#include "core/Cx.h"
#include <pthread.h>

namespace cat {
	
	/**
	 * @interface CxRunnable CxPOSIXRunnable.h "core/threading/posix/CxPOSIXRunnable.h"
    * @brief An interface for running something in a separate thread.
	 *
	 * The Runnable interface contains the methods a class must implement to 
	 * be able to be run in a new thread.
	 *
	 * @author Catlin Zilinski
	 * @version 3
	 * @since July 28, 2013
	 */
	class CxRunnable {
	  public:

		/** @brief Default constructor to initialize to no error and not destroyable. */
		CxRunnable() : m_errno(0), m_destroyable(false) {}
		
		/** @brief The destructor, just calls the destroy() method. */
		virtual ~CxRunnable() { destroy(); }

		/**
		 * @brief Called to run the object in a thread.
		 * @return A status code of some kind.
		 */
		virtual I32 run() = 0;
		
		/**
		 * @brief Method to clean up the CxRunnable to be deleted.
		 * The method must be safe to be called more than once.
		 */
		virtual void destroy() {}

		/**
		 * @brief Method to see if the CxRunnable should be deleted after it finishes running.
		 */
		virtual Boolean isDestroyable() const { return m_destroyable; }

		/**
		 * @brief Return a pointer ot the ThreadHandle associated with this CxRunnable.
		 * @return A Pointer to the ThreadHandle of the thread running the CxRunnable.
		 */
		inline ThreadHandle* getThread() { return &m_thread; }
		
		/**
		 * @brief Sets the thread handle to the specified handle.
		 * @param handle The ThreadHandle to set it to.
		 */
		inline void setThread(ThreadHandle handle) { m_thread = handle; }
		
		/**
		 * @brief Returns the error code for this CxRunnable, if there is one.
		 * @return The currently set error value.
		 */
		inline I32 getError() const { return m_errno; }
		
		/**
		 * @brief Sets the current error value for this CxRunnable.
		 * @param error The error value to set the current error value to.
		 */
		inline void setError(I32 error) { m_errno = error; }
		
		/**
		 * @brief A method used to get identifying information about the CxRunnable for 
		 * use in debugging threading.
		 * @return An identifying string for the CxRunnable.
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
	class CxRunnableFunc : public CxRunnable {
	  public:
		/**
		 * Creates a new CxRunnableFunc using the specified function and optional data
		 * @param func The function that will be run.
		 * @param data The optional data to pass to func.
		 * @param destroyDataOnFinish If true, any data passed in will be deleted after the thread terminates.
		 */
		CxRunnableFunc(I32 (*func)(VPtr), VPtr data = NIL, Boolean destroyDataOnFinish = false);
		~CxRunnableFunc();

		I32 run();
		void destroy();

		char* getInfo() const;

		/**
		 * Simple helper static method to create wrap a function in a CxRunnableFunc that is destroyed 
		 * when the function finishes.
		 * @param func The function to run in a new thread.
		 * @param data The data to pass to the function (optional).
		 * @return A pointer to a new CxRunnableFunc.
		 */
		static CxRunnableFunc* createRunnableFuncToDestroyOnCompletion(I32 (*func)(VPtr), VPtr data = NIL);

	  private:
		VPtr				data_;	/**< The data to be passed to func */
		I32 (*func_)(VPtr);		/**< The function to be run */
	};

	/**
	 * The entry point for a thread running an ICxRunnable object
	 */
	VPtr runnable_func_entry__(VPtr data);

} // namespace Cat

#ifdef DEBUG
std::ostream& operator<<(std::ostream& out, cc::CxRunnable* runnable);
#endif //DEBUG



#endif // CX_CORE_THREADING_POSIX_CXPOSIXRUNNABLE_H

