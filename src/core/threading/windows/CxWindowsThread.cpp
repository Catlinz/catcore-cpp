#include "core/threading/CxThread.h"
#include "core/common/CxAtomic.h"

namespace cat {
	
	CxThread::CxThread()
		: mp_winHandle(0), m_handle(0), m_exitStatus(-1),
		  m_status(CxThread::kNotStarted), m_msgs(CxThread::kNoMsg),
		  m_err(CxErr::kNoError) {}
	
	CxThread::CxThread(const Handle &in_thread)
		: mp_winHandle(0), m_handle(in_thread), m_exitStatus(-1),
		  m_status(CxThread::kRunning), m_msgs(CxThread::kNoMsg),
		  m_err(CxErr::kNoError) {}

	CxThread::CxThread(HANDLE in_thread)
		: mp_winHandle(in_thread), m_handle(0), m_exitStatus(-1),
		  m_status(CxThread::kRunning), m_msgs(CxThread::kNoMsg),
		  m_err(CxErr::kNoError) {
		m_handle = GetCurrentThreadId();
	}
	
	CxThread::~CxThread() {
		CXD_IF_ERR((isRunning()), "Should not delete running thread.");
		if (mp_winHandle != 0) {
			CloseHandle(mp_winHandle);
			mp_winHandle = 0;
		}
	}

	CxBool CxThread::operator==(const CxThread &in_t2) const {
		return m_handle == in_t2.m_handle;
	}

	CxBool CxThread::operator!=(const CxThread &in_t2) const {
		return m_handle != in_t2.m_handle;
	}

	void CxThread::exit(CxI32 in_exitStatus) {
		m_exitStatus = in_exitStatus;
		ExitThread((CxI32)in_exitStatus);
	}

	void CxThread::quit() {
		atomic::or32(m_msgs, kShouldQuitMsg);
	}

	CxThread* CxThread::self() {
		return new CxThread(GetCurrentThreadId());
	}

	CxThread::Handle CxThread::selfID() {
		return GetCurrentThreadId();
	}

	CxBool CxThread::start() {
		mp_winHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cx_thread_start_entry__, (void *)this, 0, &m_handle);
		if (mp_winHandle == 0) {
			DWORD error = GetLastError();
			switch(error) {
			case ERROR_TOO_MANY_TCBS:
			case ERROR_MAX_THRDS_REACHED:
			case ERROR_TOO_MANY_THREADS:
				m_err = CxErr::kThreadLimitError;  break;
			case ERROR_INVALID_ADDRESS:
			case ERROR_BAD_THREADID_ADDR:
				m_err = CxErr::kInvalidAddress;  break;
			case ERROR_NOT_ENOUGH_MEMORY:
				m_err = CxErr::kOutOfMemory;  break;
			default: m_err = CxErr::kUnknownError; break;
			}
			CXD_ERR("Failed to start thread (%d) ('%s').", error, CxErr::str(m_err));
			CloseHandle(mp_winHandle);  mp_winHandle = 0;
			m_handle = 0;  return false;
		}
		else { return true; }
	}

	CxI32 CxThread::wait() {
		if (mp_winHandle == 0) {
			mp_winHandle = OpenThread(SYNCHRONIZE, 0, m_handle);
		}

		if (mp_winHandle != 0) {
			DWORD reason = 0;
			do {
				reason = WaitForSingleObject(mp_winHandle, INFINITE);
				if (reason == WAIT_OBJECT_0) { return m_exitStatus; }
			} while (reason != WAIT_TIMEOUT);
			
			/* If get here, error happened while trying to join */
			DWORD error = GetLastError();
			CXD_ERR("Failed to join on thread, error code: %d.", error);
			m_err = CxErr::kThreadJoinError;
			return -1;
		}
		else {
			CXD_ERR("Failed to get handle for thread to join!");
			m_err = CxErr::kInvalidHandle;
			return -1;
		}
	}

	void CxThread::yield() { SwitchToThread(); }
} // namespace cat
