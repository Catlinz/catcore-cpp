#include "core/threading/posix/CxPOSIXConditionVariable.h"
#include "core/common/CxMem.h"
#include "core/common/CxAtomic.h"

namespace cat {

	CxConditionVariable::CxConditionVariable() : mp_refCount(0) {
		CxI32 error = pthread_cond_init(&m_cv, NIL);
		CXD_IF_ERR((error != 0), "Failed to initialise CxConditionVariable.  pthread_cond_init failed with code %d.", error);

		mp_refCount = (CxI32 *)mem::alloc(sizeof(CxI32));
		atomic::incr32(mp_refCount);
	}

	CxConditionVariable::CxConditionVariable(const CxConditionVariable& in_src) : mp_refCount(0) {
		m_cv = in_src.m_cv;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
	}

	CxConditionVariable::~CxConditionVariable() { tryDestroy(); }

	CxConditionVariable& CxConditionVariable::operator=(const CxConditionVariable& in_src) {
		if (mp_refCount != 0) { tryDestroy(); }
		m_cv = in_src.m_cv;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
		return *this;
	}

	void CxConditionVariable::tryDestroy() {
		CxI32 *i_ptr = mp_refCount;
		if (atomic::decr32(i_ptr) <= 0) {
			mp_refCount = 0;
			mem::free(i_ptr);
			
			CxI32 error = pthread_cond_destroy(&m_cv);
			CXD_IF_ERR((error != 0), "Failed to destroy CxConditionVariable.  pthread_cond_destroy failed with code %d.", error);
		}
	}
} // namespace cat
