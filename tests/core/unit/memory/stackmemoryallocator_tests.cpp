#include <assert.h>
#include <ctime>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/memory/stackmemoryallocator.h"

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)



namespace cc {
	class TestOne {
		public:	
			TestOne(I32 count = -1) {
				if (count >= 0)
					D(std::cout << "[" << std::dec << count << "] TestOne constructor (" << std::hex << (Addr)this << ")" << std::endl);
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

	class TestThree {
		public:
			TestThree() {
				var_one_ = 2;
				var_two_ = 3;
			}

			U64 varOne() const { return var_one_; }
			U32 varTwo() const { return var_two_; }


		private:
			U64 var_one_;
			U32 var_two_;
	};

	class TestFour {
		public:
			TestFour() {
				var_one_ = 2.33;
			}

			F32 varOne() const { return var_one_; }


		private:
			F32 var_one_;
	};


	
	void testStackMemoryAllocatorCreation() {
		BEGIN_TEST;
		StackMemoryAllocator* alloc = ::new StackMemoryAllocator(sizeof(TestOne)*10);
		assert(alloc->getStackSize() == sizeof(TestOne)*10);
		assert(alloc->getOID() == 0);

		alloc->free();
		delete alloc;
		FINISH_TEST;
	}	
	void testStackMemoryAllocatorAllocation() {
		BEGIN_TEST;		
		StackMemoryAllocator* stackAlloc = ::new StackMemoryAllocator(sizeof(TestOne)*100);
		assert(stackAlloc->getStackSize() == sizeof(TestOne)*100);
		assert(stackAlloc->getOID() == 0);

		MemoryAllocator* alloc = (MemoryAllocator*)stackAlloc;
		assert(alloc->getOID() == 0);
	
		TestOne* test1 = new (*alloc, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		D(std::cout << (Addr)test1 << std::endl);
		test1->~TestOne();
		alloc->dealloc();
	
		test1 = new (*alloc, 4) TestOne;
			
		D(std::cout << (Addr)test1 << std::endl);

		TestOne* test2 = new (*alloc, 4) TestOne;
	
		D(std::cout << (Addr)test2 << std::endl);


		TestOne* test3 = new (*alloc, 4) TestOne;
	
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
		test2->~TestOne();
		test3->~TestOne();
		alloc->dealloc();

		alloc->free();
		delete alloc;
		FINISH_TEST;

	}

	void testStackMemoryAllocatorAllocationOneHundredWithReset() {
		BEGIN_TEST;
		StackMemoryAllocator* stackAlloc = ::new StackMemoryAllocator(sizeof(TestTwo)*110);
		assert(stackAlloc->getStackSize() == sizeof(TestTwo)*110);
		MemoryAllocator* alloc = (MemoryAllocator*)stackAlloc;
		assert(alloc->getOID() == 0);

		TestTwo* test;
		TestTwo* prev = new (*alloc, 4) TestTwo;
		for(I32 i = 1; i < 100; i++) {
			test = new (*alloc, 4) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);

			assert(prev->varOne() == 1);
			assert(prev->varTwo() == 2);
			assert(prev->varThree() == 3.3f);

			assert((Addr)test == ((Addr)prev) + sizeof(TestTwo));
			prev = test;

		}
		D(std::cout << "Reseting allocator" << std::endl);
		alloc->reset();

		prev = new (*alloc, 4) TestTwo;
		for(I32 i = 1; i < 100; i++) {
			test = new (*alloc, 4) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);

			assert(prev->varOne() == 1);
			assert(prev->varTwo() == 2);
			assert(prev->varThree() == 3.3f);

			assert((Addr)test == ((Addr)prev) + sizeof(TestTwo));
			prev = test;

		}

		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testStackMemoryAllocatorAllocationSpeed() {
		BEGIN_TEST;
		StackMemoryAllocator* stackAlloc = ::new StackMemoryAllocator(sizeof(TestTwo)*1010);
		assert(stackAlloc->getStackSize() == sizeof(TestTwo)*1010);
		MemoryAllocator* alloc = (MemoryAllocator*)stackAlloc;
		assert(alloc->getOID() == 0);
		
		TestTwo* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new (*alloc, 4) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished StackMemoryAllocations in " << std::dec << total1 << " ticks!" << std::endl);

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

		D(std::cout << "StackMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testStackMemoryAllocatorAllocationAndDeallocationSpeed() {
		BEGIN_TEST;
		StackMemoryAllocator* stackAlloc = ::new StackMemoryAllocator(sizeof(TestTwo)*1010);
		assert(stackAlloc->getStackSize() == sizeof(TestTwo)*1010);
		MemoryAllocator* alloc = (MemoryAllocator*)stackAlloc;
		assert(alloc->getOID() == 0);

		TestTwo* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			stackAlloc->mark();
			test = new (*alloc, 4) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			alloc->dealloc();
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished StackMemoryAllocations in " << std::dec << total1 << " ticks!" << std::endl);

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

		D(std::cout << "StackMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testStackMemoryAllocatorAllocationSpeedMalloc() {
		BEGIN_TEST;
		StackMemoryAllocator* stackAlloc = ::new StackMemoryAllocator(sizeof(TestTwo)*1010);
		assert(stackAlloc->getStackSize() == sizeof(TestTwo)*1010);
		MemoryAllocator* alloc = (MemoryAllocator*)stackAlloc;
		assert(alloc->getOID() == 0);

		TestTwo* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new (*alloc, 4) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished StackMemoryAllocations in " << std::dec << total1 << " ticks!" << std::endl);

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

		D(std::cout << "StackMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testStackMemoryAllocatorMarkers() {
		BEGIN_TEST;
		StackMemoryAllocator* stackAlloc = ::new StackMemoryAllocator(sizeof(TestTwo)*1010);
		assert(stackAlloc->getStackSize() == sizeof(TestTwo)*1010);
		MemoryAllocator* alloc = (MemoryAllocator*)stackAlloc;
		assert(alloc->getOID() == 0);

		TestTwo* test;
		MemAddr mark0, mark1, mark2, mark3;
		mark0 = stackAlloc->getNextBlock();

		for(I32 i = 1; i < 10; i++) {
			test = new (*alloc, 4) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		stackAlloc->mark();
		mark1 = stackAlloc->getNextBlock();

		for(I32 i = 1; i < 10; i++) {
			test = new (*alloc, 4) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		stackAlloc->mark();
		mark2 = stackAlloc->getNextBlock();

		for(I32 i = 1; i < 10; i++) {
			test = new (*alloc, 4) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		stackAlloc->mark();
		mark3 = stackAlloc->getNextBlock();

		for(I32 i = 1; i < 10; i++) {
			test = new (*alloc, 4) TestTwo(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}

		alloc->dealloc();
		assert(stackAlloc->getNextBlock().addr == mark3.addr);
		alloc->dealloc();
		assert(stackAlloc->getNextBlock().addr == mark2.addr);
		alloc->dealloc();
		assert(stackAlloc->getNextBlock().addr == mark1.addr);
		alloc->dealloc();
		assert(stackAlloc->getNextBlock().addr == mark0.addr);

		delete alloc;
		FINISH_TEST;
	}

	void testStackMemoryAllocatorAllocationDifferentSizes() {
		BEGIN_TEST;
		StackMemoryAllocator* stackAlloc = ::new StackMemoryAllocator(sizeof(TestThree)*1010);
		assert(stackAlloc->getStackSize() == sizeof(TestThree)*1010);
		MemoryAllocator* alloc = (MemoryAllocator*)stackAlloc;
		assert(alloc->getOID() == 0);

		assert(sizeof(U64) == 8);

		TestOne* t1 = new(*alloc, 4) TestOne;
		TestTwo* t2 = new(*alloc, 4) TestTwo;
		TestThree* t3 = new(*alloc, 8) TestThree;
		TestFour* t4 = new(*alloc, 4) TestFour;
		TestTwo* t5 = new(*alloc, 4) TestTwo(39339);

		assert(t1->varOne() == 1);
		assert(t1->varTwo() == 2);
		assert(t1->varThree() == 3.3f);

		assert(t2->varOne() == 1);
		assert(t2->varTwo() == 2);
		assert(t2->varThree() == 3.3f);

		assert(t3->varOne() == 2);
		assert(t3->varTwo() == 3);

		assert(t4->varOne() == 2.33f);

		assert(t5->varOne() == 1);
		assert(t5->varTwo() == 2);
		assert(t5->varThree() == 3.3f);

		delete alloc;
		FINISH_TEST;
	}




}

int main(int argc, char** argv) {
	cc::testStackMemoryAllocatorCreation();
	cc::testStackMemoryAllocatorAllocation();
	cc::testStackMemoryAllocatorAllocationOneHundredWithReset();
	cc::testStackMemoryAllocatorAllocationSpeed();
	cc::testStackMemoryAllocatorAllocationAndDeallocationSpeed();
	cc::testStackMemoryAllocatorAllocationSpeedMalloc();
	cc::testStackMemoryAllocatorMarkers();
	cc::testStackMemoryAllocatorAllocationDifferentSizes();

	return 0;
}

