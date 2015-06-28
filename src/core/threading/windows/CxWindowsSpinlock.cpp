#include "core/threading/windows/CxWindowsSpinlock.h"
#include "core/common/CxAtomic.h"
#include "core/common/CxMem.h"

namespace cat {

	CxSpinlock::CxSpinlock() : mp_spinlock(0), mp_retainCount(0) {
		mp_spinlock = (CRITICAL_SECTION *)mem::alloc(sizeof(CRITICAL_SECTION));
		InitializeCriticalSectionAndSpinCount(mp_spinlock, CX_WINDOWS_SPINLOCK_TRIES);
		
		mp_retainCount = (CxI32 *)mem::alloc(sizeof(CxI32));
		atomic::incr32(*mp_retainCount);
	}

	CxSpinlock::CxSpinlock(const CxSpinlock& in_src) {
		mp_spinlock = in_src.mp_spinlock;
		mp_retainCount = in_src.mp_retainCount;
		if (mp_retainCount != 0) { atomic::incr32(*mp_retainCount); }
	}

	CxSpinlock::~CxSpinlock() { tryDestroy(); }

	CxSpinlock& CxSpinlock::operator=(const CxSpinlock& in_src) {
		if (mp_spinlock != 0 && mp_retainCount != 0) { tryDestroy(); }
		mp_spinlock = p_src.mp_spinlock;
		mp_retainCount = m_src.mp_retainCount;
		if (mp_retainCount != 0) { atomic::incr32(*mp_retainCount); }
	}

	void CxSpinlock::tryDestroy() {
		if (atomic::decr32(*mp_retainCount) <= 0) {
			DeleteCriticalSection(mp_spinlock);
			mem::free(mp_spinlock);
			mem::free(mp_retainCount);
		}
	}
	
} // namespace cat
