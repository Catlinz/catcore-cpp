#include "core/common/CxUUIDBlock.h"

namespace cat {

	CxUUIDBlock::CxUUIDBlock()
		: m_next(1), m_last(0), m_isValid(false), m_isFetching(false) {}

	void CxUUIDBlock::set(CxU64 in_start, CxU64 in_end) {
		m_lock.lock();
		m_next = in_start;
		m_last = in_end;
		m_isValid = true;
		m_lock.unlock();

		m_fetchLock.lock();
		m_isFetching = false;
		m_fetchWait.broadcast();
		m_fetchLock.unlock();
	}

	CxBool CxUUIDBlock::shouldFetchNewIDBlock() {
		CxBool should_fetch = false;
		m_fetchLock.lock();
		if (!m_isFetching && !m_isValid) {
			should_fetch = true;
			m_isFetching = true;
		}
		m_fetchLock.unlock();
		return should_fetch;
	}

	void CxUUIDBlock::waitForNewIDBlock() {
		m_fetchLock.lock();
		while (m_isFetching || !m_isValid) {
			m_fetchWait.wait(m_fetchLock);
		}
		m_fetchLock.unlock();
	}

} // namespace cat
