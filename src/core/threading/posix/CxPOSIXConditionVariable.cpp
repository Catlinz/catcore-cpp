#include "core/threading/posix/CxPOSIXConditionVariable.h"
#include "core/common/CxMem.h"
#include "core/common/CxAtomic.h"

namespace cat {

	CxConditionVariable::CxConditionVariable(const CxConditionVariable &) {
		CXD_CRASH("Do not pass CxConditionVariable by value!");
	}

	CxConditionVariable & CxConditionVariable::operator=(const CxConditionVariable &) {
		CXD_CRASH("Do not pass CxConditionVariable by value!");
		return *this;
	}

	void CxConditionVariable::destroy() {
		if (mp_cv != 0) {
			CxI32 error = pthread_cond_destroy(mp_cv);
			mem::free(mp_cv);
			CXD_IF_ERR((error != 0), "Failed to destroy CxConditionVariable (%d).", error);
		}
	}

	void CxConditionVariable::initialize() {
		if (mp_cv == 0) {
			mp_cv = (pthread_cond_t *)mem::alloc(sizeof(pthread_cond_t));
			CxI32 error = pthread_cond_init(mp_cv, 0);
			if (error != 0) {
				CXD_ERR("Failed to initialise CxConditionVariable (%d).", error);
				mem::free(mp_cv);
			}
		}
		else { CXD_ERR("Cannot initialize CV more than once."); }
	}
	}
} // namespace cat
