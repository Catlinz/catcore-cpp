#include "core/threading/posix/CxPOSIXSpinlock.h"

namespace cat {

	CxSpinlock::CxSpinlock() {
		CxI32 error = pthread_spin_init(&m_spinlock, NIL);
		CXD_IF_ERR((error != 0), "Failed to initialise CxSpinlock.  pthread_spin_init failed with code: %d.", error);
	}

	CxSpinlock::~CxSpinlock() {
		CxI32 error = pthread_spin_destroy(&m_spinlock);
		CXD_IF_ERR((error != 0), "Failed to destroy CxSpinlock.  pthread_spin_destroy failed with code: %d.", error);
	}

} // namespace Cat
