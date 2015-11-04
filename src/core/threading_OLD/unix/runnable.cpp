#include <cstdlib>
#include <cstdio>
#include "core/threading/unix/runnable.h"


namespace Cat {
	
	char* Runnable::getInfo() const {
		char* str = (char*) malloc(sizeof(char)*256);
		sprintf(str, "Default Runnable info: (%p): [error: %d (%s), destroyable: %d]", this, getError(), getStringErrorCode(getError()), isDestroyable());  
		return str;	
	}

	RunnableFunc::RunnableFunc(I32 (*func)(VPtr), VPtr data, Boolean destroyDataOnFinish) {
		func_ = func;
		data_ = data;
		setDestroyable(destroyDataOnFinish);
	}
	RunnableFunc::~RunnableFunc() {
		destroy();	
	}

	I32 RunnableFunc::run() {
		if (func_) {
			return func_(data_);
		}
		return -1;
	}

	/**
	 * The method just cleans up pointers before the object is deleted.
	 */
	void RunnableFunc::destroy() {
		data_ = NIL;
		func_ = NIL;
	}

	char* RunnableFunc::getInfo() const {
		char* str = (char*) malloc(sizeof(char)*256);
		sprintf(str, "RunnableFunc(%p): [error: %d (%s), destroyDataOnFinish: %d, data: %p]", this, getError(), getStringErrorCode(getError()), isDestroyable(), data_);  
		return str;	
	}

	RunnableFunc* RunnableFunc::createRunnableFuncToDestroyOnCompletion(I32 (*func)(VPtr), VPtr data) {
		return new RunnableFunc(func, data, true); 
	}



	VPtr runnable_func_entry__(VPtr data) {
		Runnable* runnable = (Runnable*)data;
		I32 ret = runnable->run();
		if (runnable->isDestroyable()) {
			runnable->destroy();
			delete runnable;
		}
		pthread_exit((VPtr)ret);
	}


}

#ifdef DEBUG
std::ostream& operator<<(std::ostream& out, cc::Runnable* runnable) {
	char* str = runnable->getInfo();
	out << "IRunnable[" << str << "]";
	free(str);
	return out;
}
#endif //DEBUG

