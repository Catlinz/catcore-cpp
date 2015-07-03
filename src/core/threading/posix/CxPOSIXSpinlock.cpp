#include "core/threading/posix/CxPOSIXSpinlock.h"

namespace cat {

	
	CxSpinlock& CxSpinlock::operator=(const CxSpinlock& in_src) {
		if ((m_flags & kSpinlockStatic) != 0) { destroy(); }
		
		m_spinlock = in_src.m_spinlock;
		m_flags = in_src.m_flags;
		CXD_IF_ERR(((m_flags & kSpinlockStatic) != 0), "DO NOT COPY STATIC SPINLOCKS FFS!");
		return *this;
	}

	void CxSpinlock::destroy() {
		CxI32 error = pthread_spin_destroy(&m_spinlock);
		CXD_IF_ERR((error != 0), "Failed to destroy CxSpinlock.  pthread_spin_destroy failed with code %d.", error);
		m_flags = 0;
	}

	void CxSpinlock::initialize() {
		if ((m_flags & kSpinlockInitialised) == 0) {
			CxI32 error = pthread_spin_init(&m_spinlock, NIL);
			CXD_IF_ERR((error != 0), "Failed to initialise CxSpinlock.  pthread_spin_init failed with code %d.", error);
			m_flags |= kSpinlockInitialized;
		}
	}

} // namespace Cat
