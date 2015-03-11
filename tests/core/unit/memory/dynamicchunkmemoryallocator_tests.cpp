#include <assert.h>
#include <ctime>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/memory/dynamicchunkmemoryallocator.h"
#include "core/memory/chunkmemoryallocator.h"


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)



namespace cc {
	class TestOne12Bytes {
		public:	
			TestOne12Bytes(I32 count = 1) {
				var_one_ = count;
				var_two_ = 2;
				var_three_ = 3.3;
			}
			~TestOne12Bytes() {
			}
	
			U32 varOne() const { return var_one_; }
			U32 varTwo() const { return var_two_; }
			F32 varThree() const { return var_three_; }
	
	
		private:
			U32 var_one_;
			U32 var_two_;
			F32 var_three_;
	};

	class TestTwo20Bytes {
		public:
			TestTwo20Bytes(I32 count = -1) {
				var_one_ = 1;
				var_two_ = 2;
				var_three_ = 3.3;
			}
			
			U64 varOne() const { return var_one_; }
			U64 varTwo() const { return var_two_; }
			F32 varThree() const { return var_three_; }

	
		private:
			U64 var_one_;
			U64 var_two_;
			F32 var_three_;
	};

	
	void testDynamicChunkMemoryAllocatorCreation() {
		BEGIN_TEST;
		DynamicChunkMemoryAllocator* alloc = ::new DynamicChunkMemoryAllocator();
		assert(alloc->getNumberOfAllocators() == 0);
		assert(alloc->getDefaultNumberOfBlocks() == 32);
		assert(alloc->getOID() == 0);
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}	

	void testDynamicChunkMemoryAllocatorAllocation() {
		BEGIN_TEST;		
		DynamicChunkMemoryAllocator* dChunkAlloc = ::new DynamicChunkMemoryAllocator();
		assert(dChunkAlloc->getNumberOfAllocators() == 0);
		assert(dChunkAlloc->getDefaultNumberOfBlocks() == 32);
		assert(dChunkAlloc->getOID() == 0);
		MemoryAllocator* alloc = (MemoryAllocator*)dChunkAlloc;
		assert(alloc->getOID() == 0);

		
		TestOne12Bytes* test1 = new (*alloc, 4) TestOne12Bytes;
		assert(dChunkAlloc->getNumberOfAllocators() == 1);
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->doesOwnMemory(test1));
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		D(std::cout << (Addr)test1 << std::endl);
		test1->~TestOne12Bytes();
		alloc->dealloc(test1);
	
		test1 = new (*alloc, 4) TestOne12Bytes;
		assert(dChunkAlloc->getNumberOfAllocators() == 1);
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->doesOwnMemory(test1));
			
		D(std::cout << (Addr)test1 << std::endl);

		TestOne12Bytes* test2 = new (*alloc, 4) TestOne12Bytes;
		assert(dChunkAlloc->getNumberOfAllocators() == 1);
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->doesOwnMemory(test2));
	
		D(std::cout << (Addr)test2 << std::endl);


		TestOne12Bytes* test3 = new (*alloc, 4) TestOne12Bytes;
		assert(dChunkAlloc->getNumberOfAllocators() == 1);
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->doesOwnMemory(test3));
	
		D(std::cout << (Addr)test3 << std::endl);

		assert((Addr)test2 == ((Addr)test1) + 16);
		assert((Addr)test3 == ((Addr)test2) + 16);


		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3f);

		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		test1->~TestOne12Bytes();
		alloc->dealloc(test1);
		test2->~TestOne12Bytes();
		alloc->dealloc(test2);
		test3->~TestOne12Bytes();
		alloc->dealloc(test3);

		alloc->free();
		delete alloc;
		FINISH_TEST;

	}

	void testDynamicChunkMemoryAllocatorDeallocation() {
		BEGIN_TEST;		
		DynamicChunkMemoryAllocator* dChunkAlloc = ::new DynamicChunkMemoryAllocator(3);
		assert(dChunkAlloc->getNumberOfAllocators() == 0);
		assert(dChunkAlloc->getDefaultNumberOfBlocks() == 3);
		assert(dChunkAlloc->getOID() == 0);
		MemoryAllocator* alloc = (MemoryAllocator*)dChunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test1 = new (*alloc, 4) TestOne12Bytes;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		TestOne12Bytes* test2 = new (*alloc, 4) TestOne12Bytes;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3f);
		TestOne12Bytes* test3 = new (*alloc, 4) TestOne12Bytes;
		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		alloc->dealloc(test2);

		TestOne12Bytes* test4 = new (*alloc, 4) TestOne12Bytes;
		assert(test4->varOne() == 1);
		assert(test4->varTwo() == 2);
		assert(test4->varThree() == 3.3f);

		alloc->dealloc(test4);

		test4 = new (*alloc, 4) TestOne12Bytes;
		assert(test4->varOne() == 1);
		assert(test4->varTwo() == 2);
		assert(test4->varThree() == 3.3f);

		alloc->dealloc(test4);
		alloc->dealloc(test1);
		alloc->dealloc(test3);

		test1 = new (*alloc, 4) TestOne12Bytes;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		test2 = new (*alloc, 4) TestOne12Bytes;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3f);
		test3 = new (*alloc, 4) TestOne12Bytes;
		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		assert(alloc->alloc(sizeof(TestOne12Bytes), 4) == NIL);
		
		TestTwo20Bytes* test5 = new (*alloc, 8) TestTwo20Bytes;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		TestTwo20Bytes* test6 = new (*alloc, 8) TestTwo20Bytes;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3f);
		TestTwo20Bytes* test7 = new (*alloc, 8) TestTwo20Bytes;
		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		assert(alloc->alloc(sizeof(TestTwo20Bytes), 8) == NIL);

		alloc->dealloc(test3);
		assert(alloc->alloc(sizeof(TestTwo20Bytes), 8) == NIL);
		test3 = new (*alloc, 4) TestOne12Bytes;
		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		alloc->dealloc(test5);
		assert(alloc->alloc(sizeof(TestOne12Bytes), 4) == NIL);

		test7 = new (*alloc, 8) TestTwo20Bytes;
		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		alloc->free();
		delete alloc;
		FINISH_TEST;

	}


	void testDynamicChunkMemoryAllocatorMixedAllocation() {
		BEGIN_TEST;		
		DynamicChunkMemoryAllocator* dChunkAlloc = ::new DynamicChunkMemoryAllocator();
		assert(dChunkAlloc->getNumberOfAllocators() == 0);
		assert(dChunkAlloc->getDefaultNumberOfBlocks() == 32);
		assert(dChunkAlloc->getOID() == 0);
		MemoryAllocator* alloc = (MemoryAllocator*)dChunkAlloc;
		assert(alloc->getOID() == 0);

		assert(!dChunkAlloc->canFit(sizeof(TestOne12Bytes)));
		TestOne12Bytes* test1 = new (*alloc, 4) TestOne12Bytes;
		assert(dChunkAlloc->getNumberOfAllocators() == 1);
		assert(dChunkAlloc->canFit(sizeof(TestOne12Bytes)));
		assert(!dChunkAlloc->canFit(sizeof(TestTwo20Bytes)));
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->getBlockSize() == 16);
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->doesOwnMemory(test1));

		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		D(std::cout << (Addr)test1 << std::endl);
		test1->~TestOne12Bytes();
		alloc->dealloc(test1);
	
		test1 = new (*alloc, 4) TestOne12Bytes;
		assert(dChunkAlloc->getNumberOfAllocators() == 1);
		assert(dChunkAlloc->canFit(sizeof(TestOne12Bytes)));
		assert(!dChunkAlloc->canFit(sizeof(TestTwo20Bytes)));
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->getBlockSize() == 16);
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->doesOwnMemory(test1));

			
		D(std::cout << (Addr)test1 << std::endl);

		TestTwo20Bytes* test2 = new (*alloc, 8) TestTwo20Bytes;
		assert(dChunkAlloc->getNumberOfAllocators() == 2);
		assert(dChunkAlloc->canFit(sizeof(TestOne12Bytes)));
		assert(dChunkAlloc->canFit(sizeof(TestTwo20Bytes)));
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->getBlockSize() == 16);
		assert(dChunkAlloc->getChunk(sizeof(TestTwo20Bytes))->getBlockSize() == 32);
		assert(dChunkAlloc->getChunk(sizeof(TestTwo20Bytes))->doesOwnMemory(test2));
		assert(!dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->doesOwnMemory(test2));

	
		D(std::cout << (Addr)test2 << std::endl);

		TestOne12Bytes* test3 = new (*alloc, 4) TestOne12Bytes;
		assert(dChunkAlloc->getNumberOfAllocators() == 2);
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->getBlockSize() == 16);
		assert(dChunkAlloc->getChunk(sizeof(TestTwo20Bytes))->getBlockSize() == 32);
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->doesOwnMemory(test3));
		assert(!dChunkAlloc->getChunk(sizeof(TestTwo20Bytes))->doesOwnMemory(test3));

	
		D(std::cout << (Addr)test3 << std::endl);

		TestTwo20Bytes* test4 = new (*alloc, 8) TestTwo20Bytes;
		assert(dChunkAlloc->getNumberOfAllocators() == 2);
		assert(dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->getBlockSize() == 16);
		assert(dChunkAlloc->getChunk(sizeof(TestTwo20Bytes))->getBlockSize() == 32);
		assert(dChunkAlloc->getChunk(sizeof(TestTwo20Bytes))->doesOwnMemory(test4));
		assert(!dChunkAlloc->getChunk(sizeof(TestOne12Bytes))->doesOwnMemory(test4));


		D(std::cout << (Addr)test4 << std::endl);


		assert((Addr)test3 == ((Addr)test1) + 16);
		assert((Addr)test4 == ((Addr)test2) + 32);		


		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3f);

		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		assert(test4->varOne() == 1);
		assert(test4->varTwo() == 2);
		assert(test4->varThree() == 3.3f);


		test1->~TestOne12Bytes();
		alloc->dealloc(test1);
		test2->~TestTwo20Bytes();
		alloc->dealloc(test2);
		test3->~TestOne12Bytes();
		alloc->dealloc(test3);
		test4->~TestTwo20Bytes();
		alloc->dealloc(test4);
		
		assert(dChunkAlloc->canFit(sizeof(TestOne12Bytes)));
		assert(dChunkAlloc->canFit(sizeof(TestTwo20Bytes)));
		assert(!dChunkAlloc->canFit(33));


		alloc->free();
		delete alloc;
		FINISH_TEST;

	}

	void testDynamicChunkMemoryAllocatorMaxChunks() {
		BEGIN_TEST;		
		DynamicChunkMemoryAllocator* dChunkAlloc = ::new DynamicChunkMemoryAllocator(2);
		assert(dChunkAlloc->getNumberOfAllocators() == 0);
		assert(dChunkAlloc->getDefaultNumberOfBlocks() == 2);
		assert(dChunkAlloc->getOID() == 0);
		MemoryAllocator* alloc = (MemoryAllocator*)dChunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test1 = new (*alloc, 4) TestOne12Bytes;
		TestOne12Bytes* test2 = new (*alloc, 4) TestOne12Bytes;
		assert(alloc->alloc(sizeof(TestOne12Bytes), 4) == NIL);

		TestTwo20Bytes* test3 = new (*alloc, 8) TestTwo20Bytes;
		TestTwo20Bytes* test4 = new (*alloc, 8) TestTwo20Bytes;
		assert(alloc->alloc(sizeof(TestTwo20Bytes), 8) == NIL);

		alloc->free();

		test1 = new (*alloc, 4) TestOne12Bytes;
		test2 = new (*alloc, 4) TestOne12Bytes;
		assert(alloc->alloc(sizeof(TestOne12Bytes), 4) == NIL);

		dChunkAlloc->addChunk(sizeof(TestTwo20Bytes), 3);
		test3 = new (*alloc, 8) TestTwo20Bytes;
		test4 = new (*alloc, 8) TestTwo20Bytes;
		assert(alloc->alloc(sizeof(TestTwo20Bytes), 8) != NIL);

		assert(dChunkAlloc->getNumberOfAllocators() == 2);
		dChunkAlloc->freeChunk(sizeof(TestTwo20Bytes));
		assert(dChunkAlloc->getNumberOfAllocators() == 1);
		dChunkAlloc->freeChunk(sizeof(TestOne12Bytes));
		assert(dChunkAlloc->getNumberOfAllocators() == 0);


		alloc->free();
		delete alloc;
		FINISH_TEST;
	}


	void testDynamicChunkMemoryAllocatorAllocationOneHundredWithReset() {
		BEGIN_TEST;
		DynamicChunkMemoryAllocator* dChunkAlloc = ::new DynamicChunkMemoryAllocator(100);
		assert(dChunkAlloc->getNumberOfAllocators() == 0);
		assert(dChunkAlloc->getDefaultNumberOfBlocks() == 100);
		assert(dChunkAlloc->getOID() == 0);
		MemoryAllocator* alloc = (MemoryAllocator*)dChunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test;
		TestOne12Bytes* prev = new (*alloc, 4) TestOne12Bytes(0);
		for(I32 i = 1; i < 100; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
			assert(test->varOne() == i);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			assert(prev->varOne() == i-1);
			assert(prev->varTwo() == 2);
			assert(prev->varThree() == 3.3f);

			assert((Addr)test == ((Addr)prev) + 16);
			prev = test;

		}
		D(std::cout << "Reseting allocator" << std::endl);
		alloc->reset();

		prev = new (*alloc, 4) TestOne12Bytes(0);
		for(I32 i = 1; i < 100; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
			assert(test->varOne() == i);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);

			assert(prev->varOne() == i-1);
			assert(prev->varTwo() == 2);
			assert(prev->varThree() == 3.3f);

			assert((Addr)test == ((Addr)prev) + 16);
			prev = test;

		}

		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testDynamicChunkMemoryAllocatorAllocationSpeed() {
		BEGIN_TEST;
		DynamicChunkMemoryAllocator* dChunkAlloc = ::new DynamicChunkMemoryAllocator(10000);
	assert(dChunkAlloc->getNumberOfAllocators() == 0);
		assert(dChunkAlloc->getDefaultNumberOfBlocks() == 10000);
		assert(dChunkAlloc->getOID() == 0);
		MemoryAllocator* alloc = (MemoryAllocator*)dChunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 10000; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
			assert(test->varOne() == i);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished DynamicChunkMemoryAllocators in " << std::dec << total1 << " ticks!" << std::endl);

		start = clock();
		for(I32 i = 1; i < 10000; i++) {
			test = new TestOne12Bytes(i);
			assert(test->varOne() == i);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			delete test;
		}
		end = clock();
		total2 = end-start;
		D(std::cout << "Finished New Allocations in " << std::dec << total2 << " ticks!" << std::endl);

		D(std::cout << "DynamicChunkMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testDynamicChunkMemoryAllocatorAllocationAndDeallocationSpeed() {
		BEGIN_TEST;
		DynamicChunkMemoryAllocator* dChunkAlloc = ::new DynamicChunkMemoryAllocator(10000);
		assert(dChunkAlloc->getNumberOfAllocators() == 0);
		assert(dChunkAlloc->getDefaultNumberOfBlocks() == 10000);
		assert(dChunkAlloc->getOID() == 0);
		MemoryAllocator* alloc = (MemoryAllocator*)dChunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 10000; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
			assert(test->varOne() == i);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			alloc->dealloc(test);
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished PoolMemoryAllocations in " << std::dec << total1 << " ticks!" << std::endl);

		start = clock();
		for(I32 i = 1; i < 10000; i++) {
			test = ::new TestOne12Bytes(i);
			assert(test->varOne() == i);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			delete test;
			
		}
		end = clock();
		total2 = end-start;
		D(std::cout << "Finished New Allocations in " << std::dec << total2 << " ticks!" << std::endl);

		D(std::cout << "DynamicChunkMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testDynamicChunkMemoryAllocatorAllocationSpeedMalloc() {
		BEGIN_TEST;
		DynamicChunkMemoryAllocator* dChunkAlloc = ::new DynamicChunkMemoryAllocator(10000);
		assert(dChunkAlloc->getNumberOfAllocators() == 0);
		assert(dChunkAlloc->getDefaultNumberOfBlocks() == 10000);
		assert(dChunkAlloc->getOID() == 0);
		MemoryAllocator* alloc = (MemoryAllocator*)dChunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 10000; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
			assert(test->varOne() == i);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished PoolMemoryAllocations in " << std::dec << total1 << " ticks!" << std::endl);

		start = clock();
		for(I32 i = 1; i < 10000; i++) {
			test = new (malloc(sizeof(TestOne12Bytes))) TestOne12Bytes(i);
			assert(test->varOne() == i);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			free(test);
		}
		end = clock();
		total2 = end-start;
		D(std::cout << "Finished Malloc Allocations in " << std::dec << total2 << " ticks!" << std::endl);

		D(std::cout << "DynamicChunkMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

}



int main(int argc, char** argv) {
	cc::testDynamicChunkMemoryAllocatorCreation();
	cc::testDynamicChunkMemoryAllocatorAllocation();
	cc::testDynamicChunkMemoryAllocatorDeallocation();
	cc::testDynamicChunkMemoryAllocatorMixedAllocation();
	cc::testDynamicChunkMemoryAllocatorMaxChunks();
	cc::testDynamicChunkMemoryAllocatorAllocationOneHundredWithReset();
	cc::testDynamicChunkMemoryAllocatorAllocationSpeed();
	cc::testDynamicChunkMemoryAllocatorAllocationAndDeallocationSpeed();
	cc::testDynamicChunkMemoryAllocatorAllocationSpeedMalloc();


	return 0;
}

