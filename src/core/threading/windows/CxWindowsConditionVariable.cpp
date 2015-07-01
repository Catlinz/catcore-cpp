#include "core/threading/windows/CxWindowsConditionVariable.h"
#include "core/common/CxMem.h"
#include "core/common/CxAtomic.h"

namespace cat {

	CxConditionVariable::CxConditionVariable()
		: mp_cv(0), mp_refCount(0) {
		mp_cv = (CONDITION_VARIABLE*)mem::alloc(sizeof(CONDITION_VARIABLE));
		InitializeCxConditionVariable(mp_cv);
		
		mp_refCount = (CxI32 *)mem::alloc(sizeof(CxI32));
		atomic::incr32(mp_refCount);
	}

	CxConditionVariable::CxConditionVariable(const CxConditionVariable& in_src)
		: mp_refCount(0) {
		mp_cv = in_src.mp_cv;
	   mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
	}

	CxConditionVariable::~CxConditionVariable() { tryDestroy(); }

	CxConditionVariable& CxConditionVariable::operator=(const CxConditionVariable& in_src) {
		if (mp_refCount != 0) { tryDestroy(); }
		mp_cv = in_src.mp_cv;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
		return *this;
	}

	void CxConditionVariable::tryDestroy() {
		CxI32 *i_ptr = mp_refCount;
		if (atomic::decr32(i_ptr) <= 0) {
			mp_refCount = 0;
			mem::free(i_ptr);

			DeleteCriticalSection(mp_cv);
			mem::free(mp_cv);
		}
	}
	
} // namespace cat
