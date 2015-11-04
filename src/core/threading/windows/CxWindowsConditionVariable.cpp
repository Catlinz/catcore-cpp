#include "core/threading/windows/CxWindowsConditionVariable.h"
#include "core/common/CxMem.h"

namespace cat {

	CxConditionVariable& CxConditionVariable::operator=(const CxConditionVariable& in_src) {
		if ((m_flags & kConditionVariableStatic) != 0) { destroy(); }
		
		mp_cv = in_src.mp_cv;
		m_flags = in_src.m_flags;
		CXD_IF_ERR(((m_flags & kConditionVariableStatic) != 0), "DO NOT COPY STATIC CONDITION VARIABLES FFS!");
		return *this;
	}

	void CxConditionVariable::destroy() {
		if (mp_cv != 0) {
			mem::free(mp_cv);
		}
		m_flags = 0;
	}

	void CxConditionVariable::initialize() {
		if (mp_cv == 0) {
			mp_cv = (CONDITION_VARIABLE*)mem::alloc(sizeof(CONDITION_VARIABLE));
			InitializeConditionVariable(mp_cv);
		}
		else {
			CXD_ERR("Cannot initialise Condition Variable more than once.");
		}
	}
	
} // namespace cat
