#include "core/threading/windows/CxWindowsSpinlock.h"
#include "core/common/CxMem.h"

namespace cat {

	CxSpinlock& CxSpinlock::operator=(const CxSpinlock& in_src) {
		if ((m_flags & kSpinlockStatic) != 0) { destroy(); }
		
		mp_spinlock = in_src.mp_spinlock;
		m_flags = in_src.m_flags;
		CXD_IF_ERR(((m_flags & kSpinlockStatic) != 0), "DO NOT COPY STATIC SPINLOCKS FFS!");
		return *this;
	}

	void CxSpinlock::destroy() {
		if (mp_spinlock != 0) {
			DeleteCriticalSection(mp_spinlock);
			mem::free(mp_spinlock);
		}
		m_flags = 0;
	}

	void CxSpinlock::initialize() {
		if ((m_flags & kSpinlockInitialised) == 0) {
			mp_spinlock = (CRITICAL_SECTION *)mem::alloc(sizeof(CRITICAL_SECTION));
			InitializeCriticalSectionAndSpinCount(mp_spinlock, CX_WINDOWS_SPINLOCK_TRIES);
			m_flags |= kSpinlockInitialized;
		}
	}
} // namespace cat
