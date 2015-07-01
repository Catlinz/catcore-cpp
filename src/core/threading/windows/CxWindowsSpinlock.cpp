#include "core/threading/windows/CxWindowsSpinlock.h"
#include "core/common/CxAtomic.h"
#include "core/common/CxMem.h"

namespace cat {

	CxSpinlock::CxSpinlock() : mp_spinlock(0), mp_refCount(0) {
		mp_spinlock = (CRITICAL_SECTION *)mem::alloc(sizeof(CRITICAL_SECTION));
		InitializeCriticalSectionAndSpinCount(mp_spinlock, CX_WINDOWS_SPINLOCK_TRIES);
		
		mp_refCount = (CxI32 *)mem::alloc(sizeof(CxI32));
		atomic::incr32(mp_refCount);
	}

	CxSpinlock::CxSpinlock(const CxSpinlock& in_src)
		: mp_spinlock(0), mp_refCount(0) {
		mp_spinlock = in_src.mp_spinlock;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
	}

	CxSpinlock::~CxSpinlock() { tryDestroy(); }

	CxSpinlock& CxSpinlock::operator=(const CxSpinlock& in_src) {
		if (mp_refCount != 0) { tryDestroy(); }
		mp_spinlock = in_src.mp_spinlock;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
		return *this;
	}

	void CxSpinlock::tryDestroy() {
		CxI32 *i_ptr = mp_refCount;
		if (atomic::decr32(i_ptr) <= 0) {
			mp_refCount = 0;
			mem::free(i_ptr);
			
			DeleteCriticalSection(mp_spinlock);
			mem::free(mp_spinlock);
		}
	}
	
} // namespace cat
