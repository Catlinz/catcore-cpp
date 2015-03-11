#include "core/threading/win32/mutex.h"

namespace Cat {

	Mutex::Mutex() : m_pMutex(0), m_pRetainCount(0) {
		m_pMutex = (CRITICAL_SECTION*)malloc(sizeof(CRITICAL_SECTION));
		InitializeCriticalSectionAndSpinCount(m_pMutex, 0);
		m_pRetainCount = new AtomicI32();
		m_pRetainCount->increment();
	}

	Mutex::Mutex(const Mutex& p_src) {
		m_pMutex = p_src.m_pMutex;
		m_pRetainCount = p_src.m_pRetainCount;
		m_pRetainCount->increment();
	}

	Mutex::~Mutex() {
		tryDestroy();
	}

	Mutex& Mutex::operator=(const Mutex& p_src) {
		p_src.m_pRetainCount->increment();
		if (m_pMutex && m_pRetainCount) {
			tryDestroy();
		}
		m_pMutex = p_src.m_pMutex;
		m_pRetainCount = m_src.m_pRetainCount;
	}

	void Mutex::tryDestroy() {
		if (m_pRetainCount->decrement() <= 0) {
			DeleteCriticalSection(&m_mutex);
			free(m_pMutex);
			m_pMutex = 0;
			delete m_pRetainCount;
			m_pRetainCount = 0;
		}
	}
	
} // namespace Cat
