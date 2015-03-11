#include <cstdlib>
#include "core/threading/unix/mutex.h"


namespace Cat {
	
	/**
	 * Initialize the PThreads mutex object.
	 */
	Mutex::Mutex() {
		int error = pthread_mutex_init(&m_mutex, NIL);

		if (error != 0) {
			DERR("Could not initialize Mutex.  pthread_mutex_init failed with code: " << error << "!");
		}
	}

	/**
	 * Destroy the pthread mutex object
	 */
	Mutex::~Mutex() {
		int error = pthread_mutex_destroy(&m_mutex);
		if (error != 0) {
			DERR("Could not destroy Mutex.  pthread_mutex_destroy failed with code: " << error << "!");
		}

	}

}

