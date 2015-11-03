#include "core/threading/posix/CxPOSIXBarrier.h"
#include "core/common/CxMem.h"

#if defined(CX_APPLE) || !defined(_POSIX_BARRIERS)
#  include "pthread_barrier.cpp"
#endif // No Pthread barries.

namespace cat {

	CxBool CxBarrier::destroy() {
		if (mp_barrier != 0) {
			CxI32 error = pthread_barrier_destroy(mp_barrier);
			if (error != 0) {
				switch(error) {
				case EBUSY:
					CXD_ERR("Tried to destroy barrier in use.");
					return false;
				case EINVAL:
					CXD_ERR("Tried to destroy invalid barrier.");
				default:
					CXD_ERR("Unknown error (%d) destroying barrier.", error);
				}
				mem::free(mp_barrier);  m_count = 0;
				return false;
			}
			else {
				mem::free(mp_barrier);  m_count = 0;
				return true; }
		}
		else { return true; }
	}

	void CxMutex::initialize(CxU32 in_count) {
		if (in_count == 0) {
			CXD_ERR("Cannot initialise barrier with zero count.");  return;
		}

		if (mp_barrier == 0) {
			mp_barrier = (pthread_barrier_t *)mem::alloc(sizeof(pthread_barrier_t));
			CxI32 error = pthread_barrier_init(mp_barrier, 0, in_count);
			if (error == 0) { m_count = in_count; }
			else {
#if defined(CX_DEBUG)
				switch(error) {
				case EAGAIN: CXD_ERR("Not enough resources.");  break;
				case ENOMEM: CXD_ERR("Not enough memory.");  break;
				case EBUSY: CXD_ERR("Barrier in use.");  break;
				default: CXD_ERR("Unknown error.");  break;
				}
#endif // CX_DEBUG
				mem::free(mp_barrier);
			}
		}
		else { CXD_ERR("Cannot initialize Barrier more than once."); }
	}

} // namespace cat

