#include "core/threading/posix/CxPOSIXMutex.h"
#include "core/common/CxMem.h"
#include "core/common/CxAtomic.h"

namespace cat {
	CxMutex::CxMutex() : mp_refCount(0) {
		CxI32 error = pthread_mutex_init(&m_mutex, NIL);
		CXD_IF_ERR((error != 0), "Failed to initialise CxMutex.  pthread_mutex_init failed with code %d.", error);

		mp_refCount = (CxI32 *)mem::alloc(sizeof(CxI32));
		atomic::incr32(mp_refCount);
	}

	CxMutex::CxMutex(const CxMutex& in_src) : mp_refCount(0) {
		m_mutex = in_src.m_mutex;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
	}

	CxMutex::~CxMutex() { tryDestroy(); }

	CxMutex& CxMutex::operator=(const CxMutex& in_src) {
		if (mp_refCount != 0) { tryDestroy(); }
		m_mutex = in_src.m_mutex;
		mp_refCount = in_src.mp_refCount;
		if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
		return *this;
	}

	void CxMutex::tryDestroy() {
		CxI32 *i_ptr = mp_refCount;
		if (atomic::decr32(i_ptr) <= 0) {
			mp_refCount = 0;
			mem::free(i_ptr);
			
			CxI32 error = pthread_mutex_destroy(&m_mutex);
			CXD_IF_ERR((error != 0), "Failed to destroy CxMutex.  pthread_mutex_destroy failed with code %d.", error);
		}
	}
} // namespace cat

