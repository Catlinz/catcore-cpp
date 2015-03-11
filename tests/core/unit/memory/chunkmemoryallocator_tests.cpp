#include <assert.h>
#include <ctime>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/memory/chunkmemoryallocator.h"

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)



namespace cc {
	class TestOne12Bytes {
		public:	
			TestOne12Bytes(I32 count = -1) {
				var_one_ = 1;
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

	
	void testChunkMemoryAllocatorCreation() {
		BEGIN_TEST;
		ChunkMemoryAllocator* alloc = ::new ChunkMemoryAllocator(16, 10);
		assert(alloc->getBlockSize() == 16);
		assert(alloc->getNumberOfBlocks() == 10);
		assert(alloc->getOID() == 0);
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}	
	void testChunkMemoryAllocatorAllocation() {
		BEGIN_TEST;		
		ChunkMemoryAllocator* chunkAlloc = ::new ChunkMemoryAllocator(16, 100);
		assert(chunkAlloc->getBlockSize() == 16);
		assert(chunkAlloc->getNumberOfBlocks() == 100);
		MemoryAllocator* alloc = (MemoryAllocator*)chunkAlloc;
		assert(alloc->getOID() == 0);

		MemAddr addr1, addr2;
		addr1 = chunkAlloc->getNextBlock();

		TestOne12Bytes* test1 = new (*alloc, 4) TestOne12Bytes;
		addr2.ptr = (VPtr)test1;
		assert(addr1.addr == addr2.addr);
		assert(chunkAlloc->doesOwnMemory(test1));
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		D(std::cout << (Addr)test1 << std::endl);
		test1->~TestOne12Bytes();
		alloc->dealloc(test1);
	
		test1 = new (*alloc, 4) TestOne12Bytes;
		assert(chunkAlloc->doesOwnMemory(test1));
			
		D(std::cout << (Addr)test1 << std::endl);

		TestOne12Bytes* test2 = new (*alloc, 4) TestOne12Bytes;
		assert(chunkAlloc->doesOwnMemory(test2));
	
		D(std::cout << (Addr)test2 << std::endl);


		TestOne12Bytes* test3 = new (*alloc, 4) TestOne12Bytes;
		assert(chunkAlloc->doesOwnMemory(test3));
	
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

	void testChunkMemoryAllocatorMixedAllocation() {
		BEGIN_TEST;		
		ChunkMemoryAllocator* chunkAlloc = ::new ChunkMemoryAllocator(32, 100);
		assert(chunkAlloc->getBlockSize() == 32);
		assert(chunkAlloc->getNumberOfBlocks() == 100);
		MemoryAllocator* alloc = (MemoryAllocator*)chunkAlloc;
		assert(alloc->getOID() == 0);

		MemAddr addr1, addr2;
		addr1 = chunkAlloc->getNextBlock();

		TestOne12Bytes* test1 = new (*alloc, 4) TestOne12Bytes;
		addr2.ptr = (VPtr)test1;
		assert(addr1.addr == addr2.addr);
		assert(chunkAlloc->doesOwnMemory(test1));
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		D(std::cout << (Addr)test1 << std::endl);
		test1->~TestOne12Bytes();
		alloc->dealloc(test1);
	
		addr1 = chunkAlloc->getNextBlock();
		test1 = new (*alloc, 4) TestOne12Bytes;
		assert(chunkAlloc->doesOwnMemory(test1));
		addr2.ptr = (VPtr)test1;
		assert(addr1.addr == addr2.addr);

			
		D(std::cout << (Addr)test1 << std::endl);

		addr1 = chunkAlloc->getNextBlock();
		TestTwo20Bytes* test2 = new (*alloc, 8) TestTwo20Bytes;
		assert(chunkAlloc->doesOwnMemory(test2));
		addr2.ptr = (VPtr)test2;
		assert(addr1.addr == addr2.addr);
	
		D(std::cout << (Addr)test2 << std::endl);

		addr1 = chunkAlloc->getNextBlock();
		TestOne12Bytes* test3 = new (*alloc, 4) TestOne12Bytes;
		assert(chunkAlloc->doesOwnMemory(test3));
		addr2.ptr = (VPtr)test3;
		assert(addr1.addr == addr2.addr);
	
		D(std::cout << (Addr)test3 << std::endl);

		addr1 = chunkAlloc->getNextBlock();
		TestTwo20Bytes* test4 = new (*alloc, 8) TestTwo20Bytes;
		assert(chunkAlloc->doesOwnMemory(test4));
		addr2.ptr = (VPtr)test4;
		assert(addr1.addr == addr2.addr);
	
		D(std::cout << (Addr)test4 << std::endl);


		assert((Addr)test2 == ((Addr)test1) + 32);
		assert((Addr)test3 == ((Addr)test2) + 32);
		assert((Addr)test4 == ((Addr)test3) + 32);



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


		alloc->free();
		delete alloc;
		FINISH_TEST;

	}

	void testChunkMemoryAllocatorDoesOwnMemory() {
		BEGIN_TEST;		
		ChunkMemoryAllocator* chunkAlloc = ::new ChunkMemoryAllocator(32, 100);
		assert(chunkAlloc->getBlockSize() == 32);
		assert(chunkAlloc->getNumberOfBlocks() == 100);
		MemoryAllocator* alloc = (MemoryAllocator*)chunkAlloc;
		assert(alloc->getOID() == 0);

		MemAddr addr1, addr2;
		addr1 = chunkAlloc->getNextBlock();

		TestOne12Bytes* test1 = new (*alloc, 4) TestOne12Bytes;
		addr2.ptr = (VPtr)test1;
		assert(addr1.addr == addr2.addr);
		assert(chunkAlloc->doesOwnMemory(test1));
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		D(std::cout << (Addr)test1 << std::endl);
		test1->~TestOne12Bytes();
		alloc->dealloc(test1);
	
		addr1 = chunkAlloc->getNextBlock();
		test1 = new (*alloc, 4) TestOne12Bytes;
		assert(chunkAlloc->doesOwnMemory(test1));
		addr2.ptr = (VPtr)test1;
		assert(addr1.addr == addr2.addr);

		TestOne12Bytes* test1Outside = new TestOne12Bytes();
		assert(!chunkAlloc->doesOwnMemory(test1Outside));


			
		D(std::cout << (Addr)test1 << std::endl);

		addr1 = chunkAlloc->getNextBlock();
		TestTwo20Bytes* test2 = new (*alloc, 8) TestTwo20Bytes;
		assert(chunkAlloc->doesOwnMemory(test2));
		addr2.ptr = (VPtr)test2;
		assert(addr1.addr == addr2.addr);

		TestTwo20Bytes* test2Outside = new TestTwo20Bytes();
		assert(!chunkAlloc->doesOwnMemory(test2Outside));

	
		D(std::cout << (Addr)test2 << std::endl);

		addr1 = chunkAlloc->getNextBlock();
		TestOne12Bytes* test3 = new (*alloc, 4) TestOne12Bytes;
		assert(chunkAlloc->doesOwnMemory(test3));
		addr2.ptr = (VPtr)test3;
		assert(addr1.addr == addr2.addr);

		TestOne12Bytes* test3Outside = new TestOne12Bytes();
		assert(!chunkAlloc->doesOwnMemory(test3Outside));

	
		D(std::cout << (Addr)test3 << std::endl);

		addr1 = chunkAlloc->getNextBlock();
		TestTwo20Bytes* test4 = new (*alloc, 8) TestTwo20Bytes;
		assert(chunkAlloc->doesOwnMemory(test4));
		addr2.ptr = (VPtr)test4;
		assert(addr1.addr == addr2.addr);

		TestTwo20Bytes* test4Outside = new TestTwo20Bytes();
		assert(!chunkAlloc->doesOwnMemory(test4Outside));

	
		D(std::cout << (Addr)test4 << std::endl);


		assert((Addr)test2 == ((Addr)test1) + 32);
		assert((Addr)test3 == ((Addr)test2) + 32);
		assert((Addr)test4 == ((Addr)test3) + 32);



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

		delete test1Outside;
		delete test2Outside;
		delete test3Outside;
		delete test4Outside;



		alloc->free();
		delete alloc;
		FINISH_TEST;

	}



	void testChunkMemoryAllocatorAllocationOneHundredWithReset() {
		BEGIN_TEST;
		ChunkMemoryAllocator* chunkAlloc = ::new ChunkMemoryAllocator(16, 100);
		assert(chunkAlloc->getBlockSize() == 16);
		assert(chunkAlloc->getNumberOfBlocks() == 100);
		assert(chunkAlloc->getOID() == 0);

		MemoryAllocator* alloc = (MemoryAllocator*)chunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test;
		TestOne12Bytes* prev = new (*alloc, 4) TestOne12Bytes;
		for(I32 i = 1; i < 100; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);

			assert(prev->varOne() == 1);
			assert(prev->varTwo() == 2);
			assert(prev->varThree() == 3.3f);

			assert((Addr)test == ((Addr)prev) + 16);
			prev = test;

		}
		D(std::cout << "Reseting allocator" << std::endl);
		alloc->reset();

		prev = new (*alloc, 4) TestOne12Bytes;
		for(I32 i = 1; i < 100; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);

			assert(prev->varOne() == 1);
			assert(prev->varTwo() == 2);
			assert(prev->varThree() == 3.3f);

			assert((Addr)test == ((Addr)prev) + 16);
			prev = test;

		}

		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testChunkMemoryAllocatorAllocationSpeed() {
		BEGIN_TEST;
		ChunkMemoryAllocator* chunkAlloc = ::new ChunkMemoryAllocator(16, 1000);
		assert(chunkAlloc->getBlockSize() == 16);
		assert(chunkAlloc->getNumberOfBlocks() == 1000);
		MemoryAllocator* alloc = (MemoryAllocator*)chunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished ChunkMemoryAllocators in " << std::dec << total1 << " ticks!" << std::endl);

		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new TestOne12Bytes(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			delete test;
		}
		end = clock();
		total2 = end-start;
		D(std::cout << "Finished New Allocations in " << std::dec << total2 << " ticks!" << std::endl);

		D(std::cout << "ChunkMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testChunkMemoryAllocatorAllocationAndDeallocationSpeed() {
		BEGIN_TEST;
		ChunkMemoryAllocator* chunkAlloc = ::new ChunkMemoryAllocator(16, 1000);
		assert(chunkAlloc->getBlockSize() == 16);
		assert(chunkAlloc->getNumberOfBlocks() == 1000);
		MemoryAllocator* alloc = (MemoryAllocator*)chunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
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
			test = ::new TestOne12Bytes(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			delete test;
			
		}
		end = clock();
		total2 = end-start;
		D(std::cout << "Finished New Allocations in " << std::dec << total2 << " ticks!" << std::endl);

		D(std::cout << "ChunkMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

	void testChunkMemoryAllocatorAllocationSpeedMalloc() {
		BEGIN_TEST;
		ChunkMemoryAllocator* chunkAlloc = ::new ChunkMemoryAllocator(16, 1000);
		assert(chunkAlloc->getBlockSize() == 16);
		assert(chunkAlloc->getNumberOfBlocks() == 1000);
		MemoryAllocator* alloc = (MemoryAllocator*)chunkAlloc;
		assert(alloc->getOID() == 0);

		TestOne12Bytes* test;
		clock_t start, end, total1, total2;
		start = clock();
		for(I32 i = 1; i < 1000; i++) {
			test = new (*alloc, 4) TestOne12Bytes(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f); 
		}
		end = clock();
		total1 = end-start;
		D(std::cout << "Finished PoolMemoryAllocations in " << std::dec << total1 << " ticks!" << std::endl);

		start = clock();
		for(I32 i = 1; i < 1000; i++) {

			test = new (malloc(sizeof(TestOne12Bytes))) TestOne12Bytes(i);
			assert(test->varOne() == 1);
			assert(test->varTwo() == 2);
			assert(test->varThree() == 3.3f);
			free(test);
		}
		end = clock();
		total2 = end-start;
		D(std::cout << "Finished Malloc Allocations in " << std::dec << total2 << " ticks!" << std::endl);

		D(std::cout << "ChunkMemoryAllocator is = " << std::dec << ((F64)total2/(F64)total1) << " times faster" << std::endl);
	
		alloc->free();
		delete alloc;
		FINISH_TEST;
	}

}



int main(int argc, char** argv) {
	cc::testChunkMemoryAllocatorCreation();
	cc::testChunkMemoryAllocatorAllocation();
	cc::testChunkMemoryAllocatorMixedAllocation();
	cc::testChunkMemoryAllocatorDoesOwnMemory();
	cc::testChunkMemoryAllocatorAllocationOneHundredWithReset();
	cc::testChunkMemoryAllocatorAllocationSpeed();
	cc::testChunkMemoryAllocatorAllocationAndDeallocationSpeed();
	cc::testChunkMemoryAllocatorAllocationSpeedMalloc();


	return 0;
}

