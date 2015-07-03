#include "core/threading/posix/CxPOSIXMutex.h"

namespace cat {

	CxMutex& CxMutex::operator=(const CxMutex& in_src) {
		if ((m_flags & kMutexStatic) != 0) { destroy(); }
		
		m_mutex = in_src.m_mutex;
		m_flags = in_src.m_flags;
		CXD_IF_ERR(((m_flags & kMutexStatic) != 0), "DO NOT COPY STATIC MUTEXES FFS!");
		return *this;
	}

	void CxMutex::destroy() {
		CxI32 error = pthread_mutex_destroy(&m_mutex);
		CXD_IF_ERR((error != 0), "Failed to destroy CxMutex.  pthread_mutex_destroy failed with code %d.", error);
		m_flags = 0;
	}

	void CxMutex::initialize() {
		if ((m_flags & kMutexInitialised) == 0) {
			CxI32 error = pthread_mutex_init(&m_mutex, NIL);
			CXD_IF_ERR((error != 0), "Failed to initialise CxMutex.  pthread_mutex_init failed with code %d.", error);
			m_flags |= kMutexInitialized;
		}
	}

} // namespace cat

