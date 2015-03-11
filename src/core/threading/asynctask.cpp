#include "core/threading/asynctask.h"
#include "core/threading/asyncresult.h"

namespace Cat {
	
	void AsyncTask::destroy() {
		if (m_pResult) {
			m_pResult->detach();
			if (m_destroyResultOnDestruction) {
				delete m_pResult;
			}
		}
		m_pResult = NIL;
	}

	void AsyncTask::onError() {
		if (m_pResult) {
			m_pResult->setError(getError());
		}
		
	}	

	

} // namespace Cat
