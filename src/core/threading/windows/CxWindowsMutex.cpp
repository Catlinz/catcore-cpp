#include "core/threading/windows/CxWindowsMutex.h"
#include "core/common/CxMem.h"
#include "core/common/CxAtomic.h"

namespace cat {

	CxMutex::CxMutex() : mp_mutex(0), mp_refCount(0) {
		mp_mutex = (CRITICAL_SECTION*)mem::alloc(sizeof(CRITICAL_SECTION));
		InitializeCriticalSectionAndSpinCount(mp_mutex, 0);
		
		mp_refCount = (CxI32 *)mem::alloc(sizeof(CxI32));
		atomic::incr32(mp_refCount);
	}

	CxMutex::CxMutex(const CxMutex& in_src) : mp_refCount(0) {
		mp_mutex = in_src.mp_mutex;
	   mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
	}

	CxMutex::~CxMutex() { tryDestroy(); }

	CxMutex& CxMutex::operator=(const CxMutex& in_src) {
	   if (mp_refCount != 0) { tryDestroy(); }
		mp_mutex = in_src.mp_mutex;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
		return *this;
	}

	void CxMutex::tryDestroy() {
		CxI32 *i_ptr = mp_refCount;
		if (atomic::decr32(i_ptr) <= 0) {
			mp_refCount = 0;
			mem::free(i_ptr);

			DeleteCriticalSection(mp_mutex);
			mem::free(mp_mutex);
		}
	}
	
} // namespace Cat
