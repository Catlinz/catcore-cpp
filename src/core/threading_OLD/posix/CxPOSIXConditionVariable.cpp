#include "core/threading/posix/CxPOSIXConditionVariable.h"
#include "core/common/CxMem.h"
#include "core/common/CxAtomic.h"

namespace cat {

	CxConditionVariable& CxConditionVariable::operator=(const CxConditionVariable& in_src) {
		if ((m_flags & kConditionVariableStatic) != 0) { destroy(); }
		
		m_cv = in_src.m_cv;
		m_flags = in_src.m_flags;
		CXD_IF_ERR(((m_flags & kConditionVariableStatic) != 0), "DO NOT COPY STATIC CONDITION VARIABLES FFS!");
		return *this;
	}

	void CxConditionVariable::destroy() {
		CxI32 error = pthread_cond_destroy(&m_cv);
		CXD_IF_ERR((error != 0), "Failed to destroy CxConditionVariable.  pthread_cond_destroy failed with code %d.", error);
		m_flags = 0;
	}

	void CxConditionVariable::initialize() {
		if ((m_flags & kConditionVariableInitialised) == 0) {
			CxI32 error = pthread_cond_init(&m_cv, NIL);
			CXD_IF_ERR((error != 0), "Failed to initialise CxConditionVariable.  pthread_cond_init failed with code %d.", error);
			m_flags |= kConditionVariableInitialized;
		}
	}
} // namespace cat
