#include "core/threading/asyncresult.h"
#include "core/threading/asynctask.h"

namespace Cat {

	Boolean AsyncResult::waitForResult() {
		m_resultSync.lock();
		while (m_pTask && !m_complete) {
			m_resultSync.wait();
		}
		m_resultSync.unlock();
		return (m_complete);
	}

	void AsyncResult::destroy() {
		m_resultSync.lock();
		if (m_pTask) {
			m_pTask->resultDestroyed();
			m_pTask = NIL;
		}
		m_resultSync.unlock();
	}

	void AsyncResult::detach() {
		m_resultSync.lock();
		m_pTask = NIL;
		// Broadcast incase someone is waiting for the result.
		m_resultSync.broadcast();
		m_resultSync.unlock();
	}
	
	


	

} // namespace Cat
