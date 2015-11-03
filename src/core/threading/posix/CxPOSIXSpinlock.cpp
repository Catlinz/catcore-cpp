#include "core/threading/posix/CxPOSIXSpinlock.h"
#include "core/common/CxMem.h"

namespace cat {

	
	CxSpinlock::CxSpinlock(const CxSpinlock &) {
		CXD_CRASH("Do not pass CxSpinlock by value!");
	}

	CxSpinlock & CxSpinlock::operator=(const CxSpinlock &) {
		CXD_CRASH("Do not pass CxSpinlock by value!");
		return *this;
	}

	void CxSpinlock::destroy() {
		if (mp_spin != 0) {
			CxI32 error = pthread_spin_destroy(mp_spin);
			mem::free(mp_spin);
			CXD_IF_ERR((error != 0), "Failed to destroy CxSpinlock ( %d).", error);
		}
	}

	void CxSpinlock::initialize() {
		if (mp_spin == 0) {
			mp_spin = (pthread_spinlock_t *)mem::alloc(sizeof(pthread_spinlock_t));
			CxI32 error = pthread_spin_init(mp_spin, 0);
			if (error != 0) {
				CXD_ERR("Failed to initialise CxSpinlock (%d).", error);
				mem::free(mp_spin);
			}
		}
		else { CXD_ERR("Cannot initialize Spinlock more than once."); }
	}
} // namespace Cat
