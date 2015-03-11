#include <assert.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/threading/asynctask.h"
#include "core/threading/asyncresult.h"
#include "core/threading/asyncrunnable.h"


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

#define SIZE_01 1000

namespace cc {

	I32 inty = 0;
	
	I32 testFuncOne(VPtr data) {
		std::cout << "Starting..." << std::endl;
		inty = 5;
		std::cout << "Finished!" << std::endl;
		return 0;
	}

	void testCreateAsyncRunnable() {
		BEGIN_TEST;

		AsyncTask* task = AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne));

		assert(task->getResult());
		assert(task->getResult()->getTask() == task);
		assert(!(task->getResult()->hasResult()));
		assert(!(task->getResult()->hasError()));
		assert(!task->isDestroyable());
		assert(task->getPriority() == 0);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;


		task->onCompletion();
		delete task;

		Runnable* runny = new RunnableFunc(testFuncOne, NIL);

		task = AsyncRunnable::createAsyncRunnable(runny, 1);

		assert(task->getResult());
		assert(task->getResult()->getTask() == task);
		assert(!(task->getResult()->hasResult()));
		assert(!(task->getResult()->hasError()));
		assert(!task->isDestroyable());
		assert(task->getPriority() == 1);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		delete task;

		FINISH_TEST;
		
	}

	void testDestroyAsyncRunnableAndDestroyAsyncResult() {
		BEGIN_TEST;

		// ######################### DESTROY #1 ################### //
		AsyncTask* task = AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne));
		AsyncResult* result = task->getResult();

		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);
		assert(!task->isDestroyable());
		assert(task->getPriority() == 0);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;


		task->onCompletion();
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(!result->hasError());
		delete task;
		assert(result->getTask() == NIL);
		assert(result->hasResult());
		assert(!result->hasError());
		result->destroy();
		delete result;

		// ######################### DESTROY #2 ################### //

		Runnable* runny = new RunnableFunc(testFuncOne, NIL);

		task = AsyncRunnable::createAsyncRunnable(runny, 1);
		result = task->getResult();

		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);
		assert(!task->isDestroyable());
		assert(task->getPriority() == 1);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		task->onCompletion();
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(!result->hasError());

		result->destroy();
		assert(!task->getResult());
		assert(!result->getTask());
		delete result;
		delete task;
		delete runny;

		// ######################### DESTROY #3 ################### //

		runny = new RunnableFunc(testFuncOne, NIL);
		task = AsyncRunnable::createAsyncRunnable(runny, 1);
		result = task->getResult();
		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);
		assert(!task->isDestroyable());
		assert(task->getPriority() == 1);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		task->setError(23);
		assert(!result->hasError());
		task->onError();

		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(result->hasError());


		task->onCompletion();
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(result->hasError());
		delete task;
		assert(result->getTask() == NIL);
		assert(result->hasResult());
		assert(result->hasError());
		result->destroy();
		delete result;
		delete runny;

		// ######################### DESTROY #5 ################### //
		task = AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne));
		task->setDestroyResultOnTaskDestruction(true);
		result = task->getResult();

		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);
		assert(!task->isDestroyable());
		assert(task->getPriority() == 0);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;


		task->onCompletion();
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(!result->hasError());

		task->destroy();
		assert(!task->getResult());
		delete task;

		FINISH_TEST;
		
	}



	void testAsyncRunnableRunAndCheckAsyncResult() {
		BEGIN_TEST;

		// ############ TASK RUNNING #1 ##################### //
		AsyncTask* task = AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne));
		AsyncResult* result = task->getResult();

		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);
		assert(!task->isDestroyable());
		assert(task->getPriority() == 0);
		
		task->run();
		assert(inty == 5);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);

		task->onCompletion();
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(!result->hasError());
		delete task;
		assert(result->getTask() == NIL);
		assert(result->hasResult());
		assert(!result->hasError());
		result->destroy();
		delete result;

		// ############ TASK RUNNING #2 ##################### //
		inty = 0;

		Runnable* runny = new RunnableFunc(testFuncOne, NIL);

		task = AsyncRunnable::createAsyncRunnable(runny, 1);
		result = task->getResult();

		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);
		assert(!task->isDestroyable());
		assert(task->getPriority() == 1);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		task->run();
		assert(inty == 5);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);

		task->onCompletion();
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(!result->hasError());

		result->destroy();
		assert(!task->getResult());
		assert(!result->getTask());
		delete result;
		delete task;

		inty = 0;
		runny->run();
		assert(inty == 5);

		delete runny;

		// ################### TASK RUNNING #3 (With errors) ################### //
		runny = new RunnableFunc(testFuncOne, NIL);
		task = AsyncRunnable::createAsyncRunnable(runny, 1);
		result = task->getResult();
		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);
		assert(!task->isDestroyable());
		assert(task->getPriority() == 1);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		task->setError(23);
		assert(!result->hasError());
		assert(result->getTask() == task);
		assert(!result->hasResult());

		
		task->run();
		assert(inty == 5);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;
		
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(result->hasError());

		task->onCompletion();
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(result->hasError());
		delete task;
		assert(result->getTask() == NIL);
		assert(result->hasResult());
		assert(result->hasError());
		result->destroy();
		delete result;
		
		inty = 0;
		runny->run();
		assert(inty == 5);

		delete runny;

		FINISH_TEST;
	}


	
}

int main(int argc, char** argv) {
	cc::testCreateAsyncRunnable();
	cc::testDestroyAsyncRunnableAndDestroyAsyncResult();
	cc::testAsyncRunnableRunAndCheckAsyncResult();
	
	return 0;
}

