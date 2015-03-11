#include <assert.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/threading/thread.h"
#include "core/threading/asynctask.h"
#include "core/threading/asyncresult.h"
#include "core/threading/asynctaskrunner.h"
#include "core/threading/asyncrunnable.h"


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

#define SIZE_01 1000

namespace cc {

	I32 inty = 0;
	AsyncTask* g_task = NIL;
	
	I32 testFuncOne(VPtr data) {
		std::cout << "Starting..." << std::endl;
		sleep(2);
		inty = 5;
		std::cout << "Finished!" << std::endl;
		return 0;
	}

	I32 testFuncTwo(VPtr data) {
		std::cout << "Starting Two..." << std::endl;
		inty = 5;
		std::cout << "Finished Two!" << std::endl;
		return 0;
	}

	I32 asyncDeleteTask(VPtr data) {
		std::cout << "Starting Delete Task Async..." << std::endl;
		AsyncTask* task = g_task;
		sleep(2);
		if (task) {
			task->destroy();
		}
	
		std::cout << "Finished Delete Task Async..." << std::endl;
		return 0;
	}

	I32 asyncDeleteResult(VPtr data) {
		std::cout << "Starting Delete Result Async..." << std::endl;
		AsyncTask* task = g_task;
		sleep(2);
		AsyncResult* result = task->getResult();
		if (result) {
			result->destroy();
			delete result;
		}
		inty = 6;
		std::cout << "Finished Delete Result Async..." << std::endl;
		return 0;
	}




	void testAsyncResultWaitForResult() {
		BEGIN_TEST;

		inty = 0;
		AsyncTaskRunner* runner = new AsyncTaskRunner(1);
		assert(runner->getNumberOfThreads() == 1);


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

		result = runner->run(task);
		assert(result->waitForResult());
		assert(inty == 5);
		assert(task->getInfo() == NIL);

		assert(result);
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);

		result->destroy();
		delete result;
		delete task;

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

		result = runner->run(task);
		assert(inty == 0);
		assert(result->waitForResult());
		assert(inty == 5);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		assert(result);
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);

		result->destroy();
		assert(!task->getResult());
		assert(!result->getTask());
		delete result;
		delete task;

		inty = 0;
		runny->run();
		assert(inty == 5);

		delete runny;

		// ############ TASK RUNNING #3 (Wait no sleep) ##################### //
		inty = 0;

		runny = new RunnableFunc(testFuncTwo, NIL);

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

		result = runner->run(task);
		sleep(1);
		assert(inty == 5);
		assert(result->hasResult());
		assert(result->waitForResult());
		assert(inty == 5);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		assert(result);
		assert(result->getTask() == task);
		assert(result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);

		result->destroy();
		assert(!task->getResult());
		assert(!result->getTask());
		delete result;
		delete task;

		inty = 0;
		runny->run();
		assert(inty == 5);

		delete runny;

		// ############ TASK RUNNING #4 (Wait on deleted task) ##################### //
		inty = 0;

		runny = new RunnableFunc(testFuncTwo, NIL);

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

		g_task = task;
		Runnable* runToDeleteTask = new RunnableFunc(asyncDeleteTask, NIL);
		Thread::run(runToDeleteTask);

		assert(inty == 0);
		assert(!result->hasResult());
		assert(result->getTask());
		assert(!result->waitForResult());
		assert(inty == 0);
		assert(!result->getTask());
		assert(!result->hasResult());
		delete result;
		delete runToDeleteTask;

		inty = 0;
		runny->run();
		assert(inty == 5);

		delete runny;

		// ############ TASK RUNNING #5 (Wait on null (already deleted) task) ##################### //
		inty = 0;

		runny = new RunnableFunc(testFuncTwo, NIL);

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

		task->destroy();
		delete task;

		assert(inty == 0);
		assert(!result->hasResult());
		assert(!result->getTask());
		assert(!result->waitForResult());
		assert(inty == 0);
		assert(!result->getTask());
		assert(!result->hasResult());
		delete result;

		inty = 0;
		runny->run();
		assert(inty == 5);

		delete runny;



		// ################### TASK RUNNING #6 (With errors) ################### //
		inty = 0;
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

		runner->run(task);
		assert(inty == 0);
		assert(result->waitForResult());
		assert(inty == 5);
		assert(task->getError() != 0);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;
		
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
		delete runner;

		FINISH_TEST;
	}

	void testAsyncResultDeleteResultBeforeRun() {
		BEGIN_TEST;

		inty = 0;
		AsyncTaskRunner* runner = new AsyncTaskRunner(1);
		assert(runner->getNumberOfThreads() == 1);


		// ############ TASK RUNNING #1 (simple destroy result before run) ##################### //
		AsyncTask* task = AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne));
		AsyncResult* result = task->getResult();

		assert(result);
		assert(result->getTask() == task);
		assert(!result->hasResult());
		assert(!result->hasError());
		assert(result->getResult() == NIL);
		assert(!task->isDestroyable());
		assert(task->getPriority() == 0);

		result->destroy();
		assert(!task->getResult());

		result = runner->run(task);
		assert(!result);
		while (inty != 5) {
			sleep(1);
		}
		sleep(1);
		assert(!result);
		assert(inty == 5);
		delete task;
		delete result;

		// ############ TASK RUNNING #2 (destroy and delete result before run) ##################### //
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

		result->destroy();
		delete result;

		result = runner->run(task);
		assert(!result);
		assert(inty == 0);
		while (inty != 5) {
			sleep(1);
		}
		sleep(1);
		assert(inty == 5);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		assert(!task->getResult());
		delete task;

		inty = 0;
		runny->run();
		assert(inty == 5);

		delete runny;

		// ############ TASK RUNNING #3 (delete result while running) ##################### //
		inty = 0;

		runny = new RunnableFunc(asyncDeleteResult, NIL);
		task = AsyncRunnable::createAsyncRunnable(runny, 1);
		g_task = task;
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

		result = runner->run(task);
		assert(result);
		assert(inty == 0);
		while (inty != 6) {
			sleep(1);
		}
		sleep(1);
		assert(inty == 6);
		assert(!task->getResult());
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		inty = 0;
		runny->run();
		assert(inty == 6);
	
		delete task;
		delete runny;


		// ################### TASK RUNNING #4 (deleted task with errors) ################### //
		inty = 0;
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

		task->setError(666);

		result->destroy();
		delete result;

		result = runner->run(task);
		assert(!result);
		assert(inty == 0);
		while (inty != 5) {
			sleep(1);
		}
		sleep(1);
		assert(inty == 5);
		assert(task->getError() != 0);
		assert(task->getInfo() != NIL);
		std::cout << task->getInfo() << std::endl;

		assert(!task->getResult());

		inty = 0;
		runny->run();
		assert(inty == 5);

		delete task;
		delete runny;
		delete runner;

		FINISH_TEST;
	}


	
	
}

int main(int argc, char** argv) {
	cc::testAsyncResultWaitForResult();
	cc::testAsyncResultDeleteResultBeforeRun();
	
	return 0;
}

