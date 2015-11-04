#include "core/threading/posix/CxPOSIXMutex.h"
#include "core/common/CxMem.h"

namespace cat {

	CxMutex::CxMutex(const CxMutex &) {
		CXD_CRASH("Do not pass CxMutex by value!");
	}

	CxMutex & CxMutex::operator=(const CxMutex &) {
		CXD_CRASH("Do not pass CxMutex by value!");
		return *this;
	}

	void CxMutex::destroy() {
		if (mp_mutex != 0) {
			CxI32 error = pthread_mutex_destroy(mp_mutex);
			mem::free(mp_mutex);
			CXD_IF_ERR((error != 0), "Failed to destroy CxMutex (%d).", error);
		}
	}

	void CxMutex::initialize() {
		if (mp_mutex == 0) {
			mp_mutex = (pthread_mutex_t *)mem::alloc(sizeof(pthread_mutex_t));
			CxI32 error = pthread_mutex_init(mp_mutex, 0);
			if (error != 0) {
				CXD_ERR("Failed to initialise CxMutex (%d).", error);
				mem::free(mp_mutex);
			}
		}
		else { CXD_ERR("Cannot initialize mutex more than once."); }
	}

} // namespace cat

