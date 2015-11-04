#include "core/threading/asyncrunnable.h"

namespace Cat {
	AsyncRunnable::AsyncRunnable(Runnable* runnable, U32 priority, Boolean destroyOnFinish) {
		runnable_ = runnable;
		setPriority(priority);
		setDestroyable(destroyOnFinish);
		setResult(reinterpret_cast<AsyncResult*>(new AsyncRunnableResult(this)));
	}

	AsyncRunnable::~AsyncRunnable() {
		destroy();
	}

	I32 AsyncRunnable::run() {
		I32 retVal = runnable_->run();
		if (getError() != 0) {
			onError();	
		} else {
			onSuccess();
		}
		return retVal;
	}


	void AsyncRunnable::destroy() {
		AsyncTask::destroy();
		runnable_ = NIL;		
	}


	void AsyncRunnable::onCompletion() {
		if (runnable_ && runnable_->isDestroyable()) {
			runnable_->destroy();
			delete runnable_;
			runnable_ = NIL;
		}

		if (getResult()) {
			reinterpret_cast<AsyncRunnableResult*>(getResult())->taskCompleted();
		}
	}

	char* AsyncRunnable::getInfo() const {
		if (!runnable_) return NIL;
		return runnable_->getInfo();
	}

	AsyncRunnable* AsyncRunnable::createAsyncRunnable(Runnable* runnable, U32 priority) {
		return new AsyncRunnable(runnable, priority, false);
	}

	//
	// ############### ASYNC RUNNABLE RESULT ##################### //
	//

	void AsyncRunnableResult::taskCompleted() {
		resultSync().lock();
	   setComplete(true);		
		resultSync().broadcast();
	   resultSync().unlock();
	}

} // namespace Cat

