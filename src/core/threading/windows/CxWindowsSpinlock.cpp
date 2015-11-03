#include "core/threading/windows/CxWindowsSpinlock.h"
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
		if (mp_spin) {
			DeleteCriticalSection(mp_spin);
			mem::free(mp_spin);
		}
	}

		void CxSpinlock::initialize() {
		if (mp_spin == 0) {
			mp_spin = (CRITICAL_SECTION*)mem::alloc(sizeof(CRITICAL_SECTION));
			InitializeCriticalSectionAndSpinCount(mp_spin, CX_WINDOWS_SPINLOCK_TRIES);
		}
		else {
			CXD_ERR("Cannot initialise Spinlock more than once.");
		}
	}

} // namespace cat
