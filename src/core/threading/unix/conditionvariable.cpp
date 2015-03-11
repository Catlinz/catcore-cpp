#include <cstdlib>
#include "core/threading/unix/conditionvariable.h"

namespace Cat {

	ConditionVariable::ConditionVariable() {
		int error = 0;
		error = pthread_cond_init(&m_cv, NIL);
		if (error != 0) {
			DERR("Could not initialize ConditionVariable.  pthread_cond_init failed with code " << error << "!");
		}
	}

	ConditionVariable::~ConditionVariable() {
		int error = pthread_cond_destroy(&m_cv);
		if (error != 0) {
			DERR("Could not destroy ConditionVariable.  pthread_cond_destroy failed with code: " << error << "!");
		}
	}
} // namespace Cat
