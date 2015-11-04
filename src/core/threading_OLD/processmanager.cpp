#include "core/threading/processmanager.h"

namespace Cat {

	ProcessManager::ProcessManager(Size maxProcessRunners) {
		m_runners.initWithCapacityAndLoadFactor(
			maxProcessRunners, 0.5f, NIL
			);
	}

	ProcessManager::~ProcessManager() {
		StaticMap<ProcessRunner*>::Cell* data = m_runners.arrayData();
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->terminateProcessRunner();
				data[i].value->waitForTermination();
			}
		}		
		m_runners.eraseAll();
	}

	ProcessPtr ProcessManager::getProcess(OID pid) {
		StaticMap<ProcessRunner*>::Cell* data = m_runners.arrayData();
		ProcessPtr processPtr;		
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				processPtr = data[i].value->getProcess(pid);
				if (processPtr.notNull()) {
					return processPtr;
				}				
			}
		}
		return processPtr;		
	}

	void ProcessManager::pauseProcess(OID pid) {
		StaticMap<ProcessRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				ProcessPtr processPtr = data[i].value->getProcess(pid);
				if (processPtr.notNull()) {
					data[i].value->pauseProcess(pid);
					return;					
				}				
			}
		}
	}

	void ProcessManager::resumeProcess(OID pid) {
		StaticMap<ProcessRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				ProcessPtr processPtr = data[i].value->getProcess(pid);
				if (processPtr.notNull()) {
					data[i].value->resumeProcess(pid);
					return;					
				}				
			}
		}
	}

	void ProcessManager::startProcessRunners() {
		StaticMap<ProcessRunner*>::Cell* data = m_runners.arrayData();
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->run();
				data[i].value->waitUntilStarted();				
			}
		}
	}
	
	
	void ProcessManager::terminateAllProcesses() {
		StaticMap<ProcessRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->terminateAllProcesses();
			}
		}
	}

	void ProcessManager::terminateProcess(OID pid) {
		StaticMap<ProcessRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				ProcessPtr processPtr = data[i].value->getProcess(pid);
				if (processPtr.notNull()) {
					data[i].value->terminateProcess(pid);
					return;					
				}				
			}
		}
	}

	void ProcessManager::terminateAllProcessRunners() {
		StaticMap<ProcessRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->terminateProcessRunner();				
			}
		}
	}
	
	
	Boolean ProcessManager::waitForAllProcessRunnersToTerminate() {
		StaticMap<ProcessRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->waitForTermination();				
			}
		}
		return true;
	}	

} // namespace Cat
