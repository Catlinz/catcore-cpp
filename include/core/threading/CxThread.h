#ifndef CX_CORE_THREADING_CXTHREAD_H
#define CX_CORE_THREADING_CXTHREAD_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxThread.h
 * @brief Contains the definition for the CxThread class.
 *
 * @author Catlin Zilinski
 * @date Nov 3, 2015
 */

#include "core/Cx.h"

#if defined (CX_WINDOWS)
#  include <Windows.h>
#else // POSIX
#  include <pthread.h>
#endif

namespace cat {

	/**
	 * @class CxThread CxThread.h "core/threading/CxThread.h"
	 *
	 * The CxThread class is used as a handle to threads, and has 
	 * static methods to do things like joining on another thread 
	 * and starting certain specific types of threads.
	 *
	 * @since July 23, 2013
	 * @version 5
	 * @author Catlin Zilinski
	 */
	class CxThread {
	  public:

#if defined (CX_WINDOWS)
		typedef DWORD Handle;
#else // POSIX
		typedef pthread_t Handle;
#endif
		/** @brief An enum of thread statuses. */
		enum Status {
			kInvalid = 0x0,
			kNotStarted = 0x1,

			kFailedToStart = 0x100,

			kRunningStatusMark = 0x200,
			kRunning = 0x201,
			kWaiting = 0x202,
			kWillFinish = 0x210,
			kWillTerminate = 0x211,

			kFinishedStatusMark = 0x300,
			kFinished = 0x301,
			kTerminated = 0x302,
		};

		/** @brief An enum of messages that can be sent to the thread */
		enum Msg {
			kNoMsg = 0,
			kShouldQuitMsg = 1 << 0,
		};

		/** @brief Create a new null thread handle. */
		CxThread();

		/** @brief Create a new Thread handle for the specified thread. */
		CxThread(const Handle &in_thread);

#if defined CX_WINDOWS
		/** @brief Create a new Thread handle for the specified thread. */
		CxThread(HANDLE in_thread);
#endif

		/**
		 * Destructor, doesnt stop a running thread, and should
		 * not be called on a running thread. 
		 */
		~CxThread();

		/** @return True if the two threads reference the SAME thread. */
		CxBool operator==(const CxThread &in_t2) const;

		/** @return True if the two threads reference the DIFFERENT threads. */
		CxBool operator!=(const CxThread &in_t2) const;

		/** 
		 * @brief Set the exit code for the thread and exit.
		 * THIS SHOULD ONLY EVER BE CALLED BY THE ENTRY FUNCTIONS.
		 */
		void exit(CxI32 in_exitStatus);

		/** @return The exit status for the thread object (or -1 if invalid). */
		CX_FORCE_INLINE CxI32 exitStatus() const { return m_exitStatus; }

		/** @return True if the thread is finished, false if running or not started. */
		CX_FORCE_INLINE CxBool isFinished() const {
			return m_status > kFinishedStatusMark;
		}

		/** @return True if the thread is running, false otherwise. */
		CX_FORCE_INLINE CxBool isRunning() const {
			return m_status > kRunningStatusMark && m_status < kFinishedStatusMark;
		}

		/**
		 * @brief Indicate to the thread that it should quit.
		 * This method may or may not tell the thread to quit, it depends on the 
		 * threads run() and quit() implementations.
		 */
		virtual void quit() {}

		/** @return A handle to the currently running thread. */
		static CxThread * self();

		/** @return A platform dependant handle to the currently running thread. */
		static CxThread::Handle selfID();

		/** @brief Set the thread status.  (Should only be called by the thread) */
		CX_FORCE_INLINE void setStatus(Status in_status) { m_status = in_status; }

		/**
		* @brief Method for subclasses to implement.
		* This method is the method called by the thread that is created
		* by the start() method.  The return value of this method will be
		* used as the exit code for the thread.
		* @return The exit code for the thread.
		*/
		virtual CxI32 run() { return 0; }
		
		/**
		 * @brief Start the thread.
		 * This method creates the thread and executes the run() method in that new thread.
		 * @return True if the thread was started successfully.
		 */
		CxBool start();

		/** @return The status of the thread. */
		CX_FORCE_INLINE Status status() const { return m_status; }

		/**
		 * @brief Wait for this thread to terminate.
		 * If the thread has not been started, will return 0.
		 * @return The exit status for the terminated thread.
		 */
		CxI32 wait();

		/** @brief Causes the current thread to yield the processor to another thread. */
		static void yield();
		
	  protected:
#if defined (CX_WINDOWS)
		HANDLE mp_winHandle;
#endif
		CxThread::Handle m_handle;
		CxI32 m_exitStatus;
		Status m_status;
		CxI32 m_msgs;
		CxErr::Code m_err;

		/** 
		 * @brief Sets the status but only if in a specified previous status.
		 * @param in_cur The current status the thread has to be in.
		 * @param in_new The new status IF the current status is in_cur.
		 * @return True if the status was set.
		 */
		CX_INLINE CxBool switchStatus(Status in_cur, Status in_prev) {
			if (m_status == in_cur) { m_status = in_prev;  return true; }
			else { return false; }
		}
	};

#if defined(CX_WINDOWS)
	DWORD cx_thread_start_entry__(void *in_data);
#else // POSIX
	void * cx_thread_start_entry__(void *in_data);
#endif
}

#endif // CX_CORE_THREADING_CXTHREAD_H



