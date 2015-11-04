#include "core/threading/threadmanager.h"

namespace Cat {
	ThreadManager* ThreadManager::singleton_instance_ = NIL;

	ThreadManager::ThreadManager() {
		runner_ = new AsyncTaskRunner(NUMBER_OF_TASK_RUNNERS);
	}

	ThreadManager::~ThreadManager() {
		if (runner_) {
			delete runner_;
			runner_ = NIL;
		}
	}

	void ThreadManager::initializeThreadManagerInstance() {
		if (!singleton_instance_) {
			singleton_instance_ = new ThreadManager();
		} else {
			DWARN("Cannot intialize singleton ThreadManager class more than once!");
		}
	}

	void ThreadManager::destroyThreadManagerInstance() {
		if (singleton_instance_) {
			delete singleton_instance_;
			singleton_instance_ = NIL;
		} else {
			DWARN("Cannot destroy already destroyed ThreadManager singleton class!");
		}
	}
} // namespace Cat
