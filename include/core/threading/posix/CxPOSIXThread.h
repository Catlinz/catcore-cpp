#ifndef CX_CORE_THREADING_POSIX_CXPOSIXTHREAD_H
#define CX_CORE_THREADING_POSIX_CXPOSIXTHREAD_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxPOSIXThread.h: 

 * File that contains the core includes / definitions for the 
 * unix threading libraries, including a static Thread class 
 * used for creating and manipulating threads.
 *
 * @author Catlin Zilinski
 * @date Aug 16, 2015
 */

#include <pthread.h>

#include "core/Cx.h"
#include "core/common/CxInvasiveStrongPtr.h"

namespace cat {

	typedef void * (CxPOSIXThreadEntry)(void *);

	class CxProcessRunner;
	class CxTaskRunner;
	
	/**
	 * @class CxThread CxPOSIXThread.h "core/threading/posix/CxPOSIXThread.h"
	 *
	 * The CxThread class is used as a handle to threads, and has 
	 * static methods to do things like joining on another thread 
	 * and starting certain specific types of threads.
	 *
	 * @since July 23, 2013
	 * @version 4
	 * @author Catlin Zilinski
	 */
	class CxThread {
	  public:

		/** @brief The typedef for the format of a thread entry function */
		typedef CxI32 (Entry)(void *);

		/** @brief An enum of thread statuses. */
		enum StatusEnum {
			kInvalidStatus = 0x0,
			kFailedToStart = 0x1,

			kNotStarted = 0x100,
						 
			kRunning = 0x200,
			kWaiting = 0x201,
			kWillFinish = 0x210,
			kWillTerminate = 0x211,
						 
			kFinished = 0x300,
			kTerminated = 0x301,
		};

		/** @brief Create a new null thread handle. */
		CX_FORCE_INLINE CxThread() : mp_threadInfo(0) {}
		
		/** @brief Create a new Thread handle for the specified thread. */
		CX_FORCE_INLINE CxThread(const pthread_t &in_thread);

		/** @brief Copy constructor, just copies the thread handle */
		CX_FORCE_INLINE CxThread(const CxThread &in_src);

		/** @brief Destructor, decrements the reference count */
		CX_FORCE_INLINE ~CxThread() { CxDeleteISPtr(mp_threadInfo); }

		/** @brief Overloaded assignment operator, increments reference count */
		CX_FORCE_INLINE CxThread & operator=(const CxThread &in_src);
		
		/** @return True if the two threads reference the SAME thread. */
	   CX_FORCE_INLINE CxBool operator==(const CxThread &in_t2) const {
			return pthread_equal(m_handle, in_t2.m_handle) != 0;
		}

		/** @return True if the two threads reference the DIFFERENT threads. */
	   CX_FORCE_INLINE CxBool operator!=(const CxThread &in_t2) const {
			return pthread_equal(m_handle, in_t2.m_handle) == 0;
		}
		
		/**
		 * @brief Try and get the exit status for the thread object.
		 * @return The exit status for the thread object (or -1 if invalid).
		 */
		CX_FORCE_INLINE CxI32 exitStatus() const {
			return (mp_threadInfo != 0) ? mp_threadInfo->exSt : -1;
		}

		/**
		 * @brief Try and set the status for the thread object.
		 * Can fail if the thread object has a null thread status.
		 * @param in_status The new status for the thread.
		 */
		CX_FORCE_INLINE void setExitStatus(CxI32 in_exitStatus) {
			if (mp_threadInfo != 0) { mp_threadInfo->exSt = in_exitStatus; }
		}

		/**
		 * @brief Try and set the status for the thread object.
		 * Can fail if the thread object has a null thread status.
		 * @param in_status The new status for the thread.
		 */
		CX_FORCE_INLINE void setStatus(Thread::StatusEnum in_status) {
			if (mp_threadInfo != 0) { mp_threadInfo->st = in_status; }
		}

		/**
		 * @brief Try and get the status for the thread object.
		 * @return The status for the thread object (or zero if invalid).
		 */
		CX_FORCE_INLINE CxThread::StatusEnum status() const {
			return (mp_threadInfo != 0) ? mp_threadInfo->st : kInvalidStatus;
		}

		/**
		 * @brief Runs a specified function in a new thread.
		 * This method takes in a function (and optional data to give to
		 * the function) to run in a new thread, and returns the 
		 * CxThread object representing that thread.
		 * @param in_entry The function to run in the new thread.
		 * @param in_data The optional data to give to the function.
		 * @return The CxThread object representing the new thread.
		 */
		static CxThread run(CxThread::Entry in_entry, void * in_data = 0);
		
		/**
		 * @brief Runs a specified ProcessRunner in a new thread.
		 * @param in_runner The ProcessRunner to run.
		 * @return 0 if the thread started properly, otherwise, an error code.
		 */
		static CxI32 runProcessRunner(CxProcessRunner* in_runner);

		/**
		 * @brief Runs a specified TaskRunner in a new thread.
		 * @param in_runner The TaskRunner to run.
		 * @return 0 if the thread started properly, otherwise, an error code.
		 */
		static CxI32 runTaskRunner(CxTaskRunner* in_runner);

		/**
		 * @brief Joins with another thread.
		 * Calling join on another thread will cause the current thread to 
		 * block until the other thread has terminated.
		 * @param in_handle The ThreadHandle of the thread to join on.
		 * @return True if succesfull, false if error.
		 */
		static CxBool join(CxThread &in_handle);

		/** @return A handle to the currently running thread. */
	   CX_FORCE_INLINE static CxThread self() { return CxThread(pthread_self()); }

	  private:

		class CxThreadInfo {
		  public:
			CxI32 exSt;
			CxThread::StatusEnum st;
			CX_FORCE_INLINE CxThreadInfo() : exSt(-1), st(0) { CX_ISPTR_INIT; }
			CX_FORCE_INLINE CxThreadInfo(CxThread::StatusEnum in_st)
				: exSt(-1), st(in_st) { CX_ISPTR_INIT; }
			CX_ISPTR_METHODS;
		  private:
			CX_ISPTR_FIELDS;
		};
		
		CxThreadInfo *mp_threadInfo;
		pthread_t m_handle;

		CX_FORCE_INLINE initThreadInfo(Thread::statusEnum in_status);
	};

	CX_FORCE_INLINE CxThread::CxThread(const pthread_t &in_thread)
		: mp_threadInfo(0), m_handle(in_thread) {}

	CX_FORCE_INLINE CxThread::CxThread(const CxThread &in_src)
		: mp_threadInfo(in_src.mp_threadInfo) m_handle(in_thread.m_handle) {
		if (mp_threadInfo != 0) { mp_threadInfo.retain(); }
	}

	CX_FORCE_INLINE CxThread & CxThread::operator=(const CxThread &in_src) {
		CxDeleteISPtr(mp_threadInfo);
		mp_threadInfo = in_src;
		m_handle = in_src.m_handle;
		if (mp_threadInfo != 0) { mp_threadInfo.retain(); }
		return *this;
	}

	CX_FORCE_INLINE void CxThread::initThreadInfo(Thread::StatusEnum in_status) {
		CXD_IF_ERR(mp_threadInfo != 0, "ThreadInfo should be null!");
		mp_threadInfo = new CxThreadInfo(in_status);
	}

	void * cx_func_entry__(void *in_data);
	void * cx_process_runner_func_entry__(void *in_data);
	void * cx_task_runner_func_entry__(void *in_data);
}

#endif // CX_CORE_THREADING_POSIX_CXPOSIXTHREAD_H

