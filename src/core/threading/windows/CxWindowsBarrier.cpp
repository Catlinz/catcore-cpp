#include "core/threading/windows/CxWindowsBarrier.h"
#include "core/common/CxMem.h"

namespace cat {

	CxBarrier::CxBarrier(const CxBarrier &) {
		CXD_CRASH("Do not pass CxBarrier by value!");
	}

	CxBarrier & CxBarrier::operator=(const CxBarrier &) {
		CXD_CRASH("Do not pass CxBarrier by value!");
		return *this;
	}

	
	void CxBarrier::destroy() {
		if (mp_barrier) {
			DeleteSynchronizationBarrier(mp_barrier);
			mem::free(mp_barrier);
			m_count = 0;
		}
	}

	void CxBarrier::initialize(CxU32 in_count) {
		if (mp_barrier == 0) {
			mp_barrier = (SYNCHRONIZATION_BARRIER*)mem::alloc(sizeof(SYNCHRONIZATION_BARRIER));
			InitializeSynchronizationBarrier(mp_barrier, in_count, -1);
			m_count = in_count;
		}
		else {
			CXD_ERR("Cannot initialise Barrier more than once.");
		}
	}
	
} // namespace cat
