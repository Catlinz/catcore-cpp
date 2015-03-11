#include "core/io/iomanager.h"
#include "core/threading/asynctaskrunner.h"

namespace Cat {

	IOManager* IOManager::singleton_instance_ = NIL;

	IOManager::IOManager() {
		runner_ = new AsyncTaskRunner(1);
	}

	IOManager::~IOManager() {
		if (runner_) {
			delete runner_;
			runner_ = NIL;
		}
	}

	void IOManager::initializeIOManagerInstance() {
		if (!singleton_instance_) {
			singleton_instance_ = new IOManager();
		} else {
			DWARN("Cannot initialize singleton IOManager class more than once!");
		}
	}

	void IOManager::destroyIOManagerInstance() {
		if (singleton_instance_) {
			delete singleton_instance_;
			singleton_instance_ = NIL;
		} else {
			DWARN("Cannot destroy already destroyed IOManager singleton instance!");
		}
	}

} // namespace Cat
