#include <assert.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/threading/threadmanager.h"
#include "core/threading/asynctask.h"
#include "core/threading/spinlock.h"
#include "core/threading/asyncrunnable.h"


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

namespace cc {

	I32 inty = 0;

	Spinlock locky;
	
	I32 testFuncOne(VPtr data) {
		D(std::cout << "Starting One..." << std::endl << std::flush);
		sleep(1);
		locky.lock();
		inty++;
		locky.unlock();
		D(std::cout << "Finishing One..." << std::endl << std::flush);
		return 0;
	}


	void testThreadManagerAsyncTaskRunner() {
		BEGIN_TEST;
		
		inty = 0;
		ThreadManager::initializeThreadManagerInstance();
		ThreadManager* tm = ThreadManager::getInstance();
		assert(tm);

		AsyncTaskRunner* runner = tm->getTaskRunner();
		assert(runner);
		assert(runner->getNumberOfThreads() == NUMBER_OF_TASK_RUNNERS);

		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne)));
		runner->run(AsyncRunnable::createAsyncRunnable(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne)));

		sleep(1);
		
		ThreadManager::destroyThreadManagerInstance();
		assert(ThreadManager::getInstance() == NIL);

		D(std::cout << "inty = " << inty << std::endl << std::flush);
		assert(inty == 6);
	
		FINISH_TEST;
	}

	void testThreadManagerInitializeAndDestroyInvalid() {
		BEGIN_TEST;
		
		inty = 0;
		ThreadManager::initializeThreadManagerInstance();
		sleep(1);
		ThreadManager* tm = ThreadManager::getInstance();
		assert(tm);
		AsyncTaskRunner* runner = tm->getTaskRunner();
		assert(runner);
		assert(runner->getNumberOfThreads() == NUMBER_OF_TASK_RUNNERS);

		ThreadManager::initializeThreadManagerInstance();
		ThreadManager* tm2 = ThreadManager::getInstance();
		assert(tm == tm2);
		AsyncTaskRunner* runner2 = tm->getTaskRunner();
		assert(runner2);
		assert(runner2->getNumberOfThreads() == NUMBER_OF_TASK_RUNNERS);

		assert(runner2 == runner);
		sleep(1);
		
		ThreadManager::destroyThreadManagerInstance();
		assert(ThreadManager::getInstance() == NIL);
		ThreadManager::destroyThreadManagerInstance();
		assert(ThreadManager::getInstance() == NIL);
		assert(inty == 0);
	
		FINISH_TEST;
	}






	
}

int main(int argc, char** argv) {
	cc::testThreadManagerAsyncTaskRunner();

	cc::testThreadManagerInitializeAndDestroyInvalid();
	return 0;
}


