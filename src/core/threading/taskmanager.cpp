#include "core/threading/taskmanager.h"

namespace Cat {

	TaskManager::TaskManager(Size maxTaskRunners) {
		m_runners.initWithCapacityAndLoadFactor(
			maxTaskRunners, 0.5f, NIL
			);
	}

	TaskManager::~TaskManager() {
		StaticMap<TaskRunner*>::Cell* data = m_runners.arrayData();
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->terminateTaskRunner();
				data[i].value->waitForTermination();
			}
		}		
		m_runners.eraseAll();
	}

	TaskPtr TaskManager::queueTask(const TaskPtr& task) {
		StaticMap<TaskRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				if (!data[i].value->hasFullQueue()) {
					return data[i].value->queueTask(task);
				}
			}
		}
		return TaskPtr::nullPtr();		
	}

	void TaskManager::startTaskRunners() {
		StaticMap<TaskRunner*>::Cell* data = m_runners.arrayData();
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->run();
				data[i].value->waitUntilStarted();				
			}
		}
	}
	
	
	void TaskManager::clearAllWaitingTasks() {
		StaticMap<TaskRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->clearAllWaitingTasks();
			}
		}
	}

	void TaskManager::terminateAllTaskRunners() {
		StaticMap<TaskRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->terminateTaskRunner();				
			}
		}
	}
	
	
	Boolean TaskManager::waitForAllTaskRunnersToTerminate() {
		StaticMap<TaskRunner*>::Cell* data = m_runners.arrayData();	
		for (U32 i = 0; i < m_runners.arrayLength(); i++) {
			if (data[i].key != 0) {
				data[i].value->waitForTermination();				
			}
		}
		return true;
	}	

} // namespace Cat
