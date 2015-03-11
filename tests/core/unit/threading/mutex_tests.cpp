#include <assert.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/threading/mutex.h"
#include "core/threading/thread.h"


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

#define SIZE_01 1000

namespace cc {

	Mutex mutexOne;
	I32 counter = 0;
	I32 array[SIZE_01];
	I32 carray[SIZE_01];
	I32 idx = 0;
	ThreadHandle handles[SIZE_01];

	I32 testFuncOne(VPtr data) {
		counter++;
		int y = 0;
		I32* val = (I32*)data;
		mutexOne.lock();
		(*val)++;
		for(int i = rand() % 100000 - idx*1000; i > 0; i--) {
			y = sqrt(i);	
		}

		carray[idx] = counter;
		array[idx++] = *val;
		mutexOne.unlock();
		y = y*y;
		return 0;
	}

	void testMutexSimpleTest() {
		BEGIN_TEST;
		
		I32 value = 0;

		for(int i = 0; i < SIZE_01; i++) {
			handles[i] = *(Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne, &value)));
		}
		for (int i = 0; i < SIZE_01; i++) {
			Thread::join(&handles[i]);
		}
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

	void testMutexError() {
		BEGIN_TEST;

		Mutex moo;
		assert(moo.lock());
		assert(moo.unlock());
		assert(moo.unlock());
		
	
		FINISH_TEST;
	}

	void testMutexSpeed() {
		BEGIN_TEST;

		Mutex moo;
		int y;

		clock_t start, end, total1, total2;
		start = clock();
		for (I32 i = 0; i < 100000; i++) {
			moo.lock();
			y = sqrt(i);
			moo.unlock();
		}
		end = clock();

		total1 = end - start;

		std::cout << "Total time was " << total1 << " ticks!" << std::endl;

		FINISH_TEST;

	}

	
}

int main(int argc, char** argv) {
	cc::testMutexSimpleTest();
	cc::testMutexError();
	cc::testMutexSpeed();
	return 0;
}

