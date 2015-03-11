#include <assert.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/threading/conditionvariable.h"
#include "core/threading/thread.h"


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

#define SIZE_01 1000

namespace cc {

	ConditionVariable condy;
	I32 counter = 0;
	I32 array[SIZE_01];
	I32 carray[SIZE_01];
	I32 idx = 0;
	I32 waked = 0;
	ThreadHandle handles[SIZE_01];

	I32 testFuncOne(VPtr data) {
		counter++;
		int y = 0;
		I32* val = (I32*)data;
		condy.lock();
		(*val)++;
		for(int i = rand() % 100000 - idx*1000; i > 0; i--) {
			y = sqrt(i);	
		}

		carray[idx] = counter;
		array[idx++] = *val;
		if (idx % 50 == 0) {
			condy.signal();
		}
		condy.unlock();
		y = y*y;
		return 0;
	}

	I32 testFuncWaiter(VPtr data) {
		I32* val = (I32*)data;
		condy.lock();
		std::cout << "Starting waiting!" << std::endl << std::flush;
		while (idx < SIZE_01) {
			condy.wait();
			std::cout << "Waky waky!" << std::endl << std::flush;
			std::cout << "idx = " << idx << std::endl << std::flush;
		}
		std::cout << "All done waiting!" << std::endl << std::flush;
		condy.unlock();
		return 0;

	}

	I32 testTwoFunc(VPtr data) {
		counter++;
		int y = 0;
		I32* val = (I32*)data;
		condy.lock();
		(*val)++;
		for(int i = rand() % 100000 - idx*1000; i > 0; i--) {
			y = sqrt(i);	
		}

		carray[idx] = counter;
		array[idx++] = *val;
		if (idx >= SIZE_01) {
			condy.broadcast();
		}
		condy.unlock();
		y = y*y;
		return 0;
	
	}

	I32 testTwoWaiter(VPtr data) {
		I32* val = (I32*)data;
		condy.lock();
		std::cout << "Waiting..." << std::endl << std::flush;
		while (idx < SIZE_01) {
			condy.wait();
			std::cout << "Waky waky " << waked++ << "!" << std::endl << std::flush;
			std::cout << "idx = " << idx << std::endl << std::flush;
		}
		condy.unlock();
		return 0;
	}


	void testConditionVariableSimpleTest() {
		BEGIN_TEST;
		
		I32 value = 0;

		ThreadHandle *waiter = (Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncWaiter, &value)));

		for(int i = 0; i < SIZE_01; i++) {
			handles[i] = *(Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne, &value)));
		}
		for (int i = 0; i < SIZE_01; i++) {
			Thread::join(&handles[i]);
		}
		Thread::join(waiter);
		idx = array[0];
		Boolean error = false;
		for (int i = 1; i < SIZE_01; i++) {
			if (array[i] <= idx) {
				DERR("Array[" << i << "] = " << array[i] << " is less than Array[" << i-1 << "] = " << idx << "!");
				error = true;
			}
			idx = array[i];
		}
		if (error) {
			DERR("Array stuffs not in order!");
		} else {
			DMSG("Array stuffs in order!");
		}

		idx = carray[0];
		error = false;
		for (int i = 1; i < SIZE_01; i++) {
			if (carray[i] <= idx) {
				error = true;
			}
			idx = carray[i];
		}
		if (error) {
			DMSG("CArray stuffs not in order!");
		} else {
			DMSG("CArray stuffs in order!");
		}
	
		FINISH_TEST;
	}

	void testConditionVariableBroadcastTest() {
		BEGIN_TEST;
		
		I32 value = 0;
		idx = 0;
		counter = 0;

		ThreadHandle waiters[5];

		waiters[0] = *(Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testTwoWaiter, &value)));
		waiters[1] = *(Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testTwoWaiter, &value)));
		waiters[2] = *(Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testTwoWaiter, &value)));
		waiters[3] = *(Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testTwoWaiter, &value)));
		waiters[4] = *(Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testTwoWaiter, &value)));


		for(int i = 0; i < SIZE_01; i++) {
			handles[i] = *(Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testTwoFunc, &value)));
		}
		for (int i = 0; i < SIZE_01; i++) {
			Thread::join(&handles[i]);
		}
		Thread::join(&waiters[0]);
		Thread::join(&waiters[1]);
		Thread::join(&waiters[2]);
		Thread::join(&waiters[3]);
		Thread::join(&waiters[4]);

		assert(waked == 5);

		idx = array[0];
		Boolean error = false;
		for (int i = 1; i < SIZE_01; i++) {
			if (array[i] <= idx) {
				DERR("Array[" << i << "] = " << array[i] << " is less than Array[" << i-1 << "] = " << idx << "!");
				error = true;
			}
			idx = array[i];
		}
		if (error) {
			DERR("Array stuffs not in order!");
		} else {
			DMSG("Array stuffs in order!");
		}

		idx = carray[0];
		error = false;
		for (int i = 1; i < SIZE_01; i++) {
			if (carray[i] <= idx) {
				error = true;
			}
			idx = carray[i];
		}
		if (error) {
			DMSG("CArray stuffs not in order!");
		} else {
			DMSG("CArray stuffs in order!");
		}
	
		FINISH_TEST;
	}

	
}

int main(int argc, char** argv) {
	cc::testConditionVariableSimpleTest();
	cc::testConditionVariableBroadcastTest();
	return 0;
}

