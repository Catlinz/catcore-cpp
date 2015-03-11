#include <assert.h>
#include <ctime>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/memory/poolmemoryallocator.h"

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)



namespace cc {
	class TestOne {
		public:	
			TestOne(I32 count = -1) {
				if (count >= 0) {
					D(std::cout << "[" << std::dec << count << "] TestOne constructor (" << std::hex << (Addr)this << ")" << std::endl);
				}
				else
					D(std::cout << "TestOne constructor!" << std::endl);
				var_one_ = 1;
				var_two_ = 2;
				var_three_ = 3.3;
			}
			~TestOne() {
				D(std::cout << "TestOne Destructor!" << std::endl);
			}
	
			U32 varOne() const { return var_one_; }
			U32 varTwo() const { return var_two_; }
			F32 varThree() const { return var_three_; }
	
	
		private:
			U32 var_one_;
			U32 var_two_;
			F32 var_three_;
	};

	class TestTwo {
		public:
			TestTwo(I32 count = -1) {
				var_one_ = 1;
				var_two_ = 2;
				var_three_ = 3.3;
			}
			
			U32 varOne() const { return var_one_; }
			U32 varTwo() const { return var_two_; }
			F32 varThree() const { return var_three_; }

	
		private:
			U32 var_one_;
			U32 var_two_;
			F32 var_three_;
	};

	
	void testPoolMemoryAllocatorCreation() {
		BEGIN_TEST;
		PoolMemoryAllocator* alloc = ::new PoolMemoryAllocator(sizeof(TestOne), 10, 4);
		assert(alloc->getBlockSize() == sizeof(TestOne));
		assert(alloc->getNumberOfBlocks() == 10);
		assert(alloc->getOID() == 0);
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}	
	void testPoolMemoryAllocatorAllocation() {
		BEGIN_TEST;		
		PoolMemoryAllocator* listAlloc = ::new PoolMemoryAllocator(sizeof(TestOne), 100, 4);
		assert(listAlloc->getBlockSize() == sizeof(TestOne));
		assert(listAlloc->getNumberOfBlocks() == 100);
		MemoryAllocator* alloc = (MemoryAllocator*)listAlloc;
		assert(alloc->getOID() == 0);

	
		TestOne* test1 = new (*alloc) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		D(std::cout << (Addr)test1 << std::endl);
		test1->~TestOne();
		alloc->dealloc(test1);
	
		test1 = new (*alloc) TestOne;
			
		D(std::cout << (Addr)test1 << std::endl);

		TestOne* test2 = new (*alloc) TestOne;
	
		D(std::cout << (Addr)test2 << std::endl);


		TestOne* test3 = new (*alloc) TestOne;
	
		D(std::cout << (Addr)test3 << std::endl);

		assert((Addr)test2 == ((Addr)test1) + sizeof(TestOne));
		assert((Addr)test3 == ((Addr)test2) + sizeof(TestOne));


		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3f);

		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		test1->~TestOne();
		alloc->dealloc(test1);
		test2->~TestOne();
		alloc->dealloc(test2);
		test3->~TestOne();
		alloc->dealloc(test3);

		alloc->free();
		delete alloc;
		FINISH_TEST;

	}

	void testPoolMemoryAllocatorAllocationOneHundredWithReset() {
		BEGIN_TEST;
		PoolMemoryAllocator* listAlloc = ::new PoolMemoryAllocator(sizeof(TestOne), 100, 4);
		assert(listAlloc->getBlockSize() == sizeof(TestOne));
		assert(listAlloc->getNumberOfBlocks() == 100);
		assert(listAlloc->getOID() == 0);

		MemoryAllocator* alloc = (MemoryAllocator*)listAlloc;
		assert(alloc->getOID() == 0);

		TestOne* test;
		TestOne* prev = new (*alloc) TestOne;
		for(I32 i = 1; i < 100; i++) {
			test = new (*alloc) TestOne(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);

			assert(prev->varOne() == 1);
			assert(prev->varTwo() == 2);
			assert(prev->varThree() == 3.3f);

			assert((Addr)test == ((Addr)prev) + sizeof(TestOne));
			prev = test;

		}
		D(std::cout << "Reseting allocator" << std::endl);
		alloc->reset();

		prev = new (*alloc) TestOne;
		for(I32 i = 1; i < 100; i++) {
			test = new (*alloc) TestOne(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);

			assert(prev->varOne() == 1);
			assert(prev->varTwo() == 2);
			assert(prev->varThree() == 3.3f);

			assert((Addr)test == ((Addr)prev) + sizeof(TestOne));
			prev = test;

		}

		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testPoolMemoryAllocatorAllocationSpeed() {
		BEGIN_TEST;
		PoolMemoryAllocator* listAlloc = ::new PoolMemoryAllocator(sizeof(TestOne), 1000, 4);
		assert(listAlloc->getBlockSize() == sizeof(TestOne));
		assert(listAlloc->getNumberOfBlocks() == 1000);
		MemoryAllocator* alloc = (MemoryAllocator*)listAlloc;
		assert(alloc->getOID() == 0);

		TestTwo* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new (*alloc) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished PoolMemoryAllocators in " << std::dec << total1 << " ticks!" << std::endl);

		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			delete test;
		}
		end = clock();
		total2 = end-start;
		D(std::cout << "Finished New Allocations in " << std::dec << total2 << " ticks!" << std::endl);

		D(std::cout << "PoolMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testPoolMemoryAllocatorAllocationAndDeallocationSpeed() {
		BEGIN_TEST;
		PoolMemoryAllocator* listAlloc = ::new PoolMemoryAllocator(sizeof(TestOne), 1000, 4);
		assert(listAlloc->getBlockSize() == sizeof(TestOne));
		assert(listAlloc->getNumberOfBlocks() == 1000);
		MemoryAllocator* alloc = (MemoryAllocator*)listAlloc;
		assert(alloc->getOID() == 0);

		TestTwo* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new (*alloc) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			alloc->dealloc(test);
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished PoolMemoryAllocations in " << std::dec << total1 << " ticks!" << std::endl);

		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = ::new TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			delete test;
			
		}
		end = clock();
		total2 = end-start;
		D(std::cout << "Finished New Allocations in " << std::dec << total2 << " ticks!" << std::endl);

		D(std::cout << "PoolMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testPoolMemoryAllocatorAllocationSpeedMalloc() {
		BEGIN_TEST;
		PoolMemoryAllocator* listAlloc = ::new PoolMemoryAllocator(sizeof(TestOne), 1000, 4);
		assert(listAlloc->getBlockSize() == sizeof(TestOne));
		assert(listAlloc->getNumberOfBlocks() == 1000);
		MemoryAllocator* alloc = (MemoryAllocator*)listAlloc;
		assert(alloc->getOID() == 0);

		TestTwo* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new (*alloc) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished PoolMemoryAllocations in " << std::dec << total1 << " ticks!" << std::endl);

		start = clock();
		for(I32 i = 1; i < 1000; i++) {

			test = new (malloc(sizeof(TestTwo))) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			free(test);
		}
		end = clock();
		total2 = end-start;
		D(std::cout << "Finished Malloc Allocations in " << std::dec << total2 << " ticks!" << std::endl);

		D(std::cout << "PoolMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

}

int main(int argc, char** argv) {
	cc::testPoolMemoryAllocatorCreation();
	cc::testPoolMemoryAllocatorAllocation();
	cc::testPoolMemoryAllocatorAllocationOneHundredWithReset();
	cc::testPoolMemoryAllocatorAllocationSpeed();
	cc::testPoolMemoryAllocatorAllocationAndDeallocationSpeed();
	cc::testPoolMemoryAllocatorAllocationSpeedMalloc();


	return 0;
}

