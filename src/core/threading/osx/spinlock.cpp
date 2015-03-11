#include "core/threading/osx/spinlock.h"

namespace Cat {

	Spinlock::Spinlock() {
		m_spinlock = OS_SPINLOCK_INIT;
	}

	Spinlock::~Spinlock() {
	}


} // namespace Cat
