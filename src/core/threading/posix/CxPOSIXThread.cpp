#include <unistd.h>
#include <sched.h>
#include "core/threading/CxThread.h"
#include "core/common/CxAtomic.h"

namespace cat {
	
	CxThread::CxThread()
		: m_exitStatus(-1), m_status(CxThread::kNotStarted),
		  m_msgs(CxThread::kNoMsg), m_err(CxErr::kNoError) {}
	
	CxThread::CxThread(const Handle &in_thread)
		: m_handle(in_thread), m_exitStatus(-1), m_status(CxThread::kRunning),
		  m_msgs(CxThread::kNoMsg), m_err(CxErr::kNoError) {}

	CxThread::~CxThread() {
		CXD_IF_ERR((isRunning()), "Should not delete running thread.");
	}

	CxBool CxThread::operator==(const CxThread &in_t2) const {
		return pthread_equal(m_handle, in_t2) != 0;
	}

	CxBool CxThread::operator!=(const CxThread &in_t2) const {
		return pthread_equal(m_handle, in_t2) == 0;
	}

	void CxThread::exit(CxI32 in_exitStatus) {
		m_exitStatus = in_exitStatus;
		CxMemAddr a;
		a.addr = (CxAddr)m_exitStatus;
		pthread_exit(a.ptr);
	}

	void CxThread::quit() {
		atomic::or32(m_msgs, kShouldQuitMsg);
	}

	CxThread* CxThread::self() {
		return new CxThread(pthread_self());
	}

	CxThread::Handle CxThread::selfID() {
		return pthread_self();
	}

	CxBool CxThread::start() {
		pthread_attr_t attr;  pthread_attr_init(&attr);
		// Explict for portability.
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		CxI32 error = pthread_create(&m_handle, &attr, cx_thread_start_entry__, (void *)this);
		pthread_attr_destroy(&attr);

		if (error != 0) {
			switch(error) {
			case EAGAIN: m_err = CxErr::kThreadLimitError;  break;
			case EINVAL: m_err = CxErr::kInvalidArgument;  break;
			case EPERM: m_err = CxErr::kInvalidPermissions;  break;
			default: m_err = CxErr::kUnknownError;  break;
			}
			,,
			CXD_ERR("Failed to start thread (%d) ('%s').", error, CxErr::str(m_err));
			m_status = CxThread::kFailedToStart;
			return false;
		}
		else { return true; }
	}

	CxI32 CxThread::wait() {
	   CxMemAddr ret_val;  ret_val.addr = 0;
		CxI32 e = pthread_join(m_handle, &(ret_val.ptr));
		if (e != EDEADLK) { return (CxI32)ret_val.addr; }
		else {
			CX_ERR("Deadlock detected in pthread_join.");
			return -1;
		}
	}

	void CxThread::yield() {
#if defined (_POSIX_PRIORITY_SCHEDULING)
		sched_yield();
#endif
	}

} // namespace cat
