#include <assert.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/io/iomanager.h"
#include "core/threading/asynctaskrunner.h"


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

namespace cc {


	void testIOManagerCreateAndDestroy() {
		BEGIN_TEST;
		
		assert(!IOManager::getInstance());
		IOManager::initializeIOManagerInstance();
		assert(IOManager::getInstance());
		assert(IOManager::getInstance()->getTaskRunner());
		assert(IOManager::getInstance()->getTaskRunner()->getNumberOfThreads() == 1);

		IOManager::initializeIOManagerInstance();
		assert(IOManager::getInstance());
		assert(IOManager::getInstance()->getTaskRunner());

		IOManager::destroyIOManagerInstance();
		assert(!IOManager::getInstance());

		IOManager::destroyIOManagerInstance();
		assert(!IOManager::getInstance());
		
		FINISH_TEST;
	}

}

int main(int argc, char** argv) {
	cc::testIOManagerCreateAndDestroy();
	return 0;
}


