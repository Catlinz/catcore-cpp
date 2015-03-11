#include <assert.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/threading/asynctaskrunner.h"
#include "core/threading/asynctask.h"
#include "core/threading/asyncresult.h"
#include "core/threading/asyncrunnable.h"
#include "core/threading/spinlock.h"


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

#define SIZE_01 1000

namespace cc {

	I32 inty = 0;

	Spinlock locky;
	
	I32 testFuncOne(VPtr data) {
		D(std::cout << "Starting Two..." << std::endl << std::flush);

		sleep(1);
		inty = 5;
		D(std::cout << "Finishing Two..." << std::endl << std::flush);
		return 0;
	}

	I32 testFuncTwo(VPtr data) {
		D(std::cout << "Starting Two..." << std::endl << std::flush);
		sleep(1);
		locky.lock();
		inty++;
		locky.unlock();
		D(std::cout << "Finishing Two..." << std::endl << std::flush);
		return 0;
	}



	void testAsyncTaskRunnerStartAndFinish() {
		BEGIN_TEST;
		
		AsyncTaskRunner* runner = new AsyncTaskRunner();
		sleep(2);
		assert(runner->getNumberOfThreads() == 8);

		delete runner;
	
		FINISH_TEST;
	}

	void testAsyncTaskRunnerRunSimpleFunctions() {
		BEGIN_TEST;
		
		inty = 0;
		AsyncTaskRunner* runner = new AsyncTaskRunner();
		assert(runner->getNumberOfThreads() == 8);
		sleep(1);

		AsyncResult* result = NIL;

		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->getTask()->setDestroyResultOnTaskDestruction(true);
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->getTask()->setDestroyResultOnTaskDestruction(true);
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->getTask()->setDestroyResultOnTaskDestruction(true);
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->getTask()->setDestroyResultOnTaskDestruction(true);
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->getTask()->setDestroyResultOnTaskDestruction(true);
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->getTask()->setDestroyResultOnTaskDestruction(true);
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->getTask()->setDestroyResultOnTaskDestruction(true);
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->getTask()->setDestroyResultOnTaskDestruction(true);

		sleep(1);
			
		
		delete runner;

		D(std::cout << "inty = " << inty << std::endl << std::flush);

		assert(inty == 8);
	
		FINISH_TEST;
	}

	void testAsyncTaskRunnerRunMoreFunctions() {
		BEGIN_TEST;
		
		inty = 0;
		AsyncTaskRunner* runner = new AsyncTaskRunner();
		assert(runner->getNumberOfThreads() == 8);

		AsyncResult* result = NIL;

		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;
		result = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		assert(result && result->getTask());
		result->destroy();
		delete result;


		sleep(1);
		
		
		delete runner;

		D(std::cout << "inty = " << inty << std::endl << std::flush);

		assert(inty == 11);
	
		FINISH_TEST;
	}

	void testAsyncTaskRunnerRunMoreFunctionsWithOnlyTwoRunnersAndAlcohol() {
		BEGIN_TEST;
		
		inty = 0;
		AsyncTaskRunner* runner = new AsyncTaskRunner(2);
		assert(runner->getNumberOfThreads() == 2);

		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));


		sleep(1);
		
		
		delete runner;

		D(std::cout << "inty = " << inty << std::endl << std::flush);

		assert(inty == 11);
	
		FINISH_TEST;
	}

	void testAsyncTaskRunnerRunMoreFunctionsWithOnlyTwoRunnersAndWaitForResults() {
		BEGIN_TEST;
		
		inty = 0;
		AsyncTaskRunner* runner = new AsyncTaskRunner(2);
		assert(runner->getNumberOfThreads() == 2);

		AsyncResult **result = (AsyncResult**)malloc(sizeof(AsyncResult*) * 11);

		result[0] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[1] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[2] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[3] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[4] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[5] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[6] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[7] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[8] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[9] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		result[10] = runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));

		for (int i = 0; i < 11; i++) {
			assert(result[i]->waitForResult());
		}
		
		assert(inty == 11);
		D(std::cout << "inty = " << inty << std::endl << std::flush);

		for (int i =0 ; i < 11; i++) {
			result[i]->destroy();
			delete result[i];
		}
		free(result);
			
		delete runner;	
		FINISH_TEST;
	}


	void testAsyncTaskRunnerRunAndStopAndDelete() {
		BEGIN_TEST;
		
		inty = 0;
		AsyncTaskRunner* runner = new AsyncTaskRunner(2);
		assert(runner->getNumberOfThreads() == 2);

		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo)));

		sleep(1);
		
		runner->stop();
		delete runner;

		D(std::cout << "inty = " << inty << std::endl << std::flush);

		assert(inty == 6);
	
		FINISH_TEST;
	}





	
}

int main(int argc, char** argv) {
	cc::testAsyncTaskRunnerStartAndFinish();
	cc::testAsyncTaskRunnerRunSimpleFunctions();
	cc::testAsyncTaskRunnerRunMoreFunctions();
	cc::testAsyncTaskRunnerRunMoreFunctionsWithOnlyTwoRunnersAndAlcohol();
	cc::testAsyncTaskRunnerRunMoreFunctionsWithOnlyTwoRunnersAndWaitForResults();
	cc::testAsyncTaskRunnerRunAndStopAndDelete();

	return 0;
}


