#include "core/threading/windows/CxWindowsMutex.h"
#include "core/common/CxMem.h"

namespace cat {

	CxMutex::CxMutex(const CxMutex &) {
		CXD_CRASH("Do not pass CxMutex by value!");
	}

	CxMutex & CxMutex::operator=(const CxMutex &) {
		CXD_CRASH("Do not pass CxMutex by value!");
		return *this;
	}

	
	void CxMutex::destroy() {
		if (mp_mutex) {
			DeleteCriticalSection(mp_mutex);
			mem::free(mp_mutex);
		}
	}

	void CxMutex::initialize() {
		if (mp_mutex == 0) {
			mp_mutex = (CRITICAL_SECTION*)mem::alloc(sizeof(CRITICAL_SECTION));
			InitializeCriticalSectionAndSpinCount(mp_mutex, 0);
		}
		else {
			CXD_ERR("Cannot initialise mutex more than once.");
		}
	}
	
} // namespace cat
