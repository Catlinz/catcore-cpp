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


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

#define SIZE_01 1000

namespace cc {

	I32 inty = 0;

	ThreadHandle mainT;
	ThreadHandle otherT;
	
	I32 testFuncOne(VPtr data) {
		std::cout << "Starting..." << std::endl;
		otherT = Thread::self();
		sleep(5);
		inty = 5;
		std::cout << "Finished!" << std::endl;
		return 0;
	}


	void testThreadRunAndJoin() {
		BEGIN_TEST;
	
		mainT = Thread::self();
		ThreadHandle* handle = (Thread::run(RunnableFunc::createRunnableFuncToDestroyOnCompletion(testFuncOne)));
		Thread::join(handle);
		assert(inty == 5);
		assert(equals(mainT, Thread::self()));
		assert(!equals(otherT, Thread::self()));
		
		FINISH_TEST;
	}
	
}

int main(int argc, char** argv) {
	cc::testThreadRunAndJoin();
	return 0;
}

