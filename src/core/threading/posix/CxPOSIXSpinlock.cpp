#include "core/threading/posix/CxPOSIXSpinlock.h"

namespace cat {

	CxSpinlock::CxSpinlock() : mp_refCount(0) {
		CxI32 error = pthread_spin_init(&m_spinlock, NIL);
		CXD_IF_ERR((error != 0), "Failed to initialise CxSpinlock.  pthread_spin_init failed with code: %d.", error);

		mp_refCount = (CxI32 *)mem::alloc(sizeof(CxI32));
		atomic::incr32(mp_refCount);
	}

	
	CxSpinlock::CxSpinlock(const CxSpinlock& in_src) : mp_refCount(0) {
		m_spinlock = in_src.m_spinlock;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
	}


	CxSpinlock::~CxSpinlock() { tryDestroy(); }

	CxSpinlock& CxSpinlock::operator=(const CxSpinlock& in_src) {
		if (mp_refCount != 0) { tryDestroy(); }
		m_spinlock = in_src.m_spinlock;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
		return *this;
	}

	void CxSpinlock::tryDestroy() {
		CxI32 *i_ptr = mp_refCount;
		if (atomic::decr32(i_ptr) <= 0) {
			mp_refCount = 0;
			mem::free(i_ptr);
			
			CxI32 error = pthread_spin_destroy(&m_spinlock);
			CXD_IF_ERR((error != 0), "Failed to destroy CxSpinlock.  pthread_spin_destroy failed with code: %d.", error);
		}
	}

} // namespace Cat
