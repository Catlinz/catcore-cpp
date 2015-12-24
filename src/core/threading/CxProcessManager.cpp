#include "core/threading/CxProcessManager.h"

namespace cat {

	CxProcessManager::CxProcessManager(CxI32 in_maxRunners) {
		m_runners = CxHashMap<CxChar *, CxProcessRunner *>(in_maxRunners);
	}

	CxProcessManager::~CxProcessManager() {
		stopRunnersAndWait();
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if (*itr != 0) { delete (*itr);  *itr = 0; }
		}
	}

	CxProcessRunner * CxProcessManager::createRunner(const CxChar *in_name, CxI32 inopt_maxProcesses) {
		if (m_runners.size() < m_runners.capacity()) {
			if (!m_runners.contains(in_name)) {
				CxHashMap<Char *, CxProcessRunner *>::Itr itr =
					m_runners.insert(in_name, new CxProcessRunner(in_name, inopt_maxProcesses));
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

	CxProcessPtr CxProcessManager::getProcess(CxU64 in_process) {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
		   CxProcessPtr p = (*itr)->getProcess(in_process);
			if (p != 0) { return p; }
		}
		CXD_WARN("No process with ID (%ll).", in_process);
		return CxProcessPtr();
	}

	CxProcessPtr CxProcessManager::getProcess(const CxChar *in_process) {
		if (in_process != 0) {
			CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
			for (; itr != m_runners.end(); ++itr) {
				CxProcessPtr p = (*itr)->getProcess(in_process);
				if (p != 0) { return p; }
			}
			CXD_WARN("No process with name '%s'.", in_process);
		}
		else { CXD_ERR("Cannot get process from null name."); }
		return CxProcessPtr();
	}

	CxBool CxProcessManager::pause(CxU64 in_process) {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if ((*itr)->hasProcess(in_process)) {
				return (*itr)->pause(in_process);
			}
		}
		CXD_WARN("No process to pause with ID (%ll).", in_process);
		return false;
	}

	CxBool CxProcessManager::pause(const CxChar *in_process) {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if ((*itr)->hasProcess(in_process)) {
				return (*itr)->pause(in_process);
			}
		}
		CXD_WARN("No process to pause with name (%s).", in_process);
		return false;
	}

	CxBool CxProcessManager::queue(const CxProcess *in_process) {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if ((*itr)->accepts(in_process)) {
				return (*itr)->queue(in_process);
			}
		}
		CXD_WARN("Cannot queue process (%ll), no runners accept it.", in_process->pid());
		return false;
	}

	CxBool CxProcessManager::resume(CxU64 in_process) {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if ((*itr)->hasProcess(in_process)) {
				return (*itr)->resume(in_process);
			}
		}
		CXD_WARN("No process to resume with ID (%ll).", in_process);
		return false;
	}

	CxBool CxProcessManager::resume(const CxChar *in_process) {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if ((*itr)->hasProcess(in_process)) {
				return (*itr)->resume(in_process);
			}
		}
		CXD_WARN("No process to resume with name (%s).", in_process);
		return false;
	}

	void CxProcessManager::startRunners() {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
		   (*itr)->start();
		}
	}

	void CxProcessManager::stopRunners() {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
		   (*itr)->quit();
		}
	}

	CxBool CxProcessManager::stopRunnersAndWait() {
		stopRunners();
		
		CxBool success = true;
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
		   success &= (*itr)->wait();
		}
		return success;
	}

	CxBool CxProcessManager::terminate(CxU64 in_process) {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if ((*itr)->hasProcess(in_process)) {
				return (*itr)->terminate(in_process);
			}
		}
		CXD_WARN("No process to terminate with ID (%ll).", in_process);
		return false;
	}

	CxBool CxProcessManager::terminate(const CxChar *in_process) {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
			if ((*itr)->hasProcess(in_process)) {
				return (*itr)->terminate(in_process);
			}
		}
		CXD_WARN("No process to terminate with name (%s).", in_process);
		return false;
	}

	void CxProcessManager::terminateAll() {
		CxHashMap<CxChar *, CxProcessRunner *>::Itr itr = m_runners.begin();
		for (; itr != m_runners.end(); ++itr) {
		   (*itr)->terminateAll();
		}
	}
	
} // namespace cat
