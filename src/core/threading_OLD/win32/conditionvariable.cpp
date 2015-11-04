#include "core/threading/win32/conditionvariable.h"

namespace Cat {

	ConditionVariable::ConditionVariable() : m_pCV(0), m_pRetainCount(0) {
		m_pCV = (CONDITION_VARIABLE*)malloc(sizeof(CONDITION_VARIABLE));
		InitializeConditionVariable(m_pCV);
		m_pRetainCount = new AtomicI32();
		m_pRetainCount->increment();
	}

	ConditionVariable::ConditionVariable(const ConditionVariable& p_src) {
		m_pCV = p_src.m_pCV;
		m_pRetainCount = p_src.m_pRetainCount;
		m_pRetainCount->increment();
	}

	ConditionVariable::~ConditionVariable() {
		tryDestroy();
	}

	ConditionVariable& ConditionVariable::operator=(const ConditionVariable& p_src) {
		p_src.m_pRetainCount->increment();
		if (m_pCV && m_pRetainCount) {
			tryDestroy();
		}
		m_pCV = p_src.m_pCV;
		m_pRetainCount = m_src.m_pRetainCount;
	}

	void ConditionVariable::tryDestroy() {
		if (m_pRetainCount->decrement() <= 0) {
			DeleteCriticalSection(&m_mutex);
			free(m_pCV);
			m_pCV = 0;
			delete m_pRetainCount;
			m_pRetainCount = 0;
		}
	}
	
} // namespace Cat
