#include "core/threading/win32/spinlock.h"

namespace Cat {

	Spinlock::Spinlock() : m_pSpinlock(0), m_pRetainCount(0) {
		m_pSpinlock = (CRITICAL_SECTION*)malloc(sizeof(CRITICAL_SECTION));
		InitializeCriticalSectionAndSpinCount(m_pSpinlock, CAT_SPINLOCK_TRIES);
		m_pRetainCount = new AtomicI32();
		m_pRetainCount->increment();
	}

	Spinlock::Spinlock(const Spinlock& p_src) {
		m_pSpinlock = p_src.m_pSpinlock;
		m_pRetainCount = p_src.m_pRetainCount;
		m_pRetainCount->increment();
	}

	Spinlock::~Spinlock() {
		tryDestroy();
	}

	Spinlock& Spinlock::operator=(const Spinlock& p_src) {
		p_src.m_pRetainCount->increment();
		if (m_pSpinlock && m_pRetainCount) {
			tryDestroy();
		}
		m_pSpinlock = p_src.m_pSpinlock;
		m_pRetainCount = m_src.m_pRetainCount;
	}

	void Spinlock::tryDestroy() {
		if (m_pRetainCount->decrement() <= 0) {
			DeleteCriticalSection(&m_mutex);
			free(m_pSpinlock);
			m_pSpinlock = 0;
			delete m_pRetainCount;
			m_pRetainCount = 0;
		}
	}
	
} // namespace Cat
