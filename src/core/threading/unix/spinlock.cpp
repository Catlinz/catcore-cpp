#include "core/threading/unix/spinlock.h"

namespace Cat {

	Spinlock::Spinlock() {
		int error = pthread_spin_init(m_spinlock, NIL);
		if (error != 0) {
			DERR("Could not initialize Spinlock.  pthread_spin_init failed with code " << error << "!");
		} 
	}

	Spinlock::~Spinlock() {
		int error = pthread_spin_destroy(m_spinlock);
		if (error != 0) {
			DERR("Could not destroy Spinlock.  pthread_spin_destroy failed with code: " << error << "!");
		}
	}

} // namespace Cat
