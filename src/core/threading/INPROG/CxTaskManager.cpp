#include "core/threading/CxTaskManager.h"

namespace cat {

	CxTaskManager::CxTaskManager(CxI32 in_maxRunners) {
		m_runners = CxHashMap<CxChar *, CxTaskRunner *>(in_maxRunners);
	}

	CxTaskManager::~CxTaskManager() {
		stopRunnersAndWait();
		CxHashMap<CxChar *, CxTaskRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if (*itr != 0) { delete (*itr);  *itr = 0; }
		}
	}

	CxTaskRunner * CxTaskManager::createRunner(const CxChar *in_name) {
		if (m_runners.size() < m_runners.capacity()) {
			if (!m_runners.contains(in_name)) {
				CxHashMap<Char *, CxTaskRunner *>::Itr itr =
					m_runners.insert(in_name, new CxTaskRunner(in_name));
				return *itr;
			}
			else {
				CXD_WARN("Cannot create runner '%s', already created.", in_name);
				return 0;
			}
		}
		else {
			CXD_WARN("Cannot create runner '%s', max runners reached.", in_name);
			return 0;
		}
	}

	CxTaskPtr CxTaskManager::queue(const CxTask *in_task) {
		CxHashMap<CxChar *, CxTaskRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if ((*itr)->accepts(in_task)) {
				return (*itr)->queue(in_task);
			}
		}
		CXD_WARN("Cannot queue task (%ll), no runners accept it.", in_task->pid());
		return CxTaskPtr();
	}

	void CxTaskManager::startRunners() {
		CxHashMap<CxChar *, CxTaskRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
		   (*itr)->start();
		}
	}

	void CxTaskManager::stopRunners() {
		CxHashMap<CxChar *, CxTaskRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
		   (*itr)->quit();
		}
	}

	CxBool CxTaskManager::stopRunnersAndWait() {
		stopRunners();
		
		CxBool success = true;
		CxHashMap<CxChar *, CxTaskRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
		   success &= (*itr)->wait();
		}
		return success;
	}

} // namespace cat
