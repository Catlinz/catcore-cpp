#include "core/threading/windows/CxWindowsMutex.h"
#include "core/common/CxMem.h"

namespace cat {

	CxMutex& CxMutex::operator=(const CxMutex& in_src) {
		if ((m_flags & kMutexStatic) != 0) { destroy(); }
		
		mp_mutex = in_src.mp_mutex;
		m_flags = in_src.m_flags;
		CXD_IF_ERR(((m_flags & kMutexStatic) != 0), "DO NOT COPY STATIC MUTEXES FFS!");
		return *this;
	}
	
	void CxMutex::destroy() {
		if (mp_mutex) {
			DeleteCriticalSection(mp_mutex);
			mem::free(mp_mutex);
		}
		m_flags = 0;
	}

	void CxMutex::initialize() {
		if ((m_flags & kMutexInitialised) == 0) {
			mp_mutex = (CRITICAL_SECTION*)mem::alloc(sizeof(CRITICAL_SECTION));
			InitializeCriticalSectionAndSpinCount(mp_mutex, 0);
			m_flags |= kMutexInitialized;
		}
	}
	
} // namespace cat
