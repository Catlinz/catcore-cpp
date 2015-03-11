#include <assert.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/threading/runnable.h"
#include "core/threading/thread.h"

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

#define SIZE_01 1000

namespace cc {

	I32 inty = 0;
	
	I32 testFuncOne(VPtr data) {
		std::cout << "Starting..." << std::endl;
		sleep(2);
		inty = 5;
		std::cout << "Finished!" << std::endl;
		return 0;
	}

	I32 testFuncTwo(VPtr data) {
		std::cout << "Starting Two..." << std::endl;
		sleep(1);
		I32* amount = (I32*)data;
		inty += *amount;
		std::cout << "Finished Two!" << std::endl;
		return 0;
	}


	void testCreateAndRunRunnableFunc() {
		BEGIN_TEST;

		Runnable* run1 = RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne, NIL);
		assert(inty == 0);
		Thread::run(run1);
		Thread::join(run1->getThread());
		assert(inty == 5);
		inty = 0;
		assert(inty == 0);
		std::cout << run1->getInfo() << std::endl;

		Runnable* run2 = new RunnableFunc(testFuncOne, NIL);
		inty = 0;
		assert(inty == 0);
		Thread::run(run2);
		Thread::join(run2->getThread());
		assert(inty == 5);
		assert(run2->run() != -1);
		std::cout << run2->getInfo() << std::endl;
		delete run2;

		I32 amount = 10;
		Runnable* run3 = RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncTwo, (VPtr)&amount);
		inty = 0;
		assert(inty == 0);
		Thread::run(run3);
		Thread::join(run3->getThread());
		assert(inty == amount);
		assert(inty == amount);
		std::cout << run3->getInfo() << std::endl;

		amount = 12;
		Runnable* run4 = new RunnableFunc(testFuncTwo, (VPtr)&amount);
		inty = 0;
		assert(inty == 0);
		Thread::run(run4);
		Thread::join(run4->getThread());
		assert(inty == amount);
		assert(run4->run() != -1);
		assert(inty == amount*2);
		std::cout << run4->getInfo() << std::endl;
		delete run4;

	
		FINISH_TEST;
	}

	
	
}

int main(int argc, char** argv) {
	cc::testCreateAndRunRunnableFunc();

	return 0;
}

