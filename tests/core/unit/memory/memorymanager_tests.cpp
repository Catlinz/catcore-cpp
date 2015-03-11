#include <assert.h>
#include <ctime>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/memory/memorymanager.h"
#include "core/memory/stackmemoryallocator.h"
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
				var_one_ = 1;
				var_two_ = 2;
				var_three_ = 3.3;
			}
			~TestOne() {
			
			}

			void setTimesTwo() {
				var_one_ *= 2;
				var_two_ *= 2;
				var_three_ *= 2.0f;
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
				if (count >= 0) {
					D(std::cout << "[" << std::dec << count << "] TestTwo constructor (" << std::hex << (Addr)this << ")" << std::endl);
				}
				var_one_ = 1;
				var_two_ = 2;
				var_three_ = 3.3;
			}
			~TestTwo() {
			
			}
	
			U32 varOne() const { return var_one_; }
			U32 varTwo() const { return var_two_; }
			F64 varThree() const { return var_three_; }
	
	
		private:
			U32 var_one_;
			U32 var_two_;
			F64 var_three_;
	};

		class TestThree {
		public:	
			TestThree(I32 count = -1) {
				if (count >= 0) {
					D(std::cout << "[" << std::dec << count << "] TestThree constructor (" << std::hex << (Addr)this << ")" << std::endl);
				}
				var_one_ = 1;
				var_two_ = 2;
				var_three_ = 3.3f;
			}
			~TestThree() {
			
			}
	
			U64 varOne() const { return var_one_; }
			U64 varTwo() const { return var_two_; }
			F32 varThree() const { return var_three_; }
	
	
		private:
			U64 var_one_;
			U64 var_two_;
			F32 var_three_;
	};


	
	void testMemoryManagerInitialize() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance();

		MemoryManager* m = MemoryManager::getInstance();
		assert(m);

		assert(len(m) == 0);

		MemoryManager::destroyMemoryManagerInstance();

		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}

	void testCreatePoolAllocator() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance();
		MemoryManager* m = MemoryManager::getInstance();
		assert(m);
		assert(len(m) == 0);

		OID alloc_id = m->createPoolAllocator(sizeof(TestOne), 100, 4); 
		MemoryAllocator* alloc = m->get(alloc_id);
		assert(alloc);
		assert(alloc->getOID() == alloc_id);
		assert(len(m) == 1);

		TestOne* test1 = new (*alloc) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		alloc->dealloc(test1);

		test1 = new (*alloc) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		OID alloc2_id = m->createPoolAllocator(sizeof(TestTwo), 100, 8);

		MemoryAllocator* alloc2 = m->get(alloc2_id);
		assert(alloc2);
		assert(alloc2->getOID() == alloc2_id);

		assert(len(m) == 2);

		TestTwo* test2 = new (*alloc2) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		alloc2->dealloc(test2);
		
		m->free(alloc_id);
		assert(!m->get(alloc_id));
		assert(len(m) == 1);

		alloc2 = m->get(alloc2_id);

		test2 = new (*alloc2) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);

		alloc2->dealloc(test2);

		m->free(alloc2_id);
		assert(!m->get(alloc2_id));
		assert(len(m) == 0);


		MemoryManager::destroyMemoryManagerInstance();
		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}

	void testCreateStackAllocator() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance();
		MemoryManager* m = MemoryManager::getInstance();
		assert(m);
		assert(len(m) == 0);

		OID alloc_id = m->createStackAllocator(sizeof(TestOne)*100); 
		MemoryAllocator* alloc = m->get(alloc_id);
		assert(alloc);
		assert(alloc->getOID() == alloc_id);
		assert(len(m) == 1);

		TestOne* test1 = new (*alloc, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		alloc->dealloc();

		test1 = new (*alloc, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		OID alloc2_id = m->createStackAllocator(sizeof(TestTwo)*100);

		MemoryAllocator* alloc2 = m->get(alloc2_id);
		assert(alloc2);
		assert(alloc2->getOID() == alloc2_id);
		assert(len(m) == 2);

		TestTwo* test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		alloc2->dealloc();
		
		m->free(alloc_id);
		assert(!m->get(alloc_id));
		assert(len(m) == 1);

		alloc2 = m->get(alloc2_id);
		assert(alloc2->getOID() == alloc2_id);

		test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		((StackMemoryAllocator*)alloc2)->mark();

		test1 = new (*alloc2, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);


		alloc2->dealloc();

		m->free(alloc2_id);
		assert(!m->get(alloc2_id));
		assert(len(m) == 0);


		MemoryManager::destroyMemoryManagerInstance();
		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}

	void testCreateChunkAllocator() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance();
		MemoryManager* m = MemoryManager::getInstance();
		assert(m);
		assert(len(m) == 0);

		OID alloc_id = m->createChunkAllocator(16, 10); 
		MemoryAllocator* alloc = m->get(alloc_id);
		assert(alloc);
		assert(alloc->getOID() == alloc_id);
		assert(len(m) == 1);

		TestOne* test1 = new (*alloc, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		alloc->dealloc(test1);

		test1 = new (*alloc, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		OID alloc2_id = m->createChunkAllocator(32, 10);

		MemoryAllocator* alloc2 = m->get(alloc2_id);
		assert(alloc2);
		assert(alloc2->getOID() == alloc2_id);

		assert(len(m) == 2);

		TestTwo* test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		alloc2->dealloc(test2);
		
		m->free(alloc_id);
		assert(!m->get(alloc_id));
		assert(len(m) == 1);

		alloc2 = m->get(alloc2_id);

		test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);

		alloc2->dealloc(test2);

		m->free(alloc2_id);
		assert(!m->get(alloc2_id));
		assert(len(m) == 0);


		MemoryManager::destroyMemoryManagerInstance();
		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}

	void testCreateDynamicChunkAllocator() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance();
		MemoryManager* m = MemoryManager::getInstance();
		assert(m);
		assert(len(m) == 0);

		OID alloc_id = m->createDynamicChunkAllocator(); 
		MemoryAllocator* alloc = m->get(alloc_id);
		assert(alloc);
		assert(alloc->getOID() == alloc_id);
		assert(len(m) == 1);

		TestOne* test1 = new (*alloc, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		alloc->dealloc(test1);

		test1 = new (*alloc, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		TestTwo* test2 = new (*alloc, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);

		TestThree* test3 = new (*alloc, 8) TestThree;
		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		OID alloc2_id = m->createDynamicChunkAllocator(3);

		MemoryAllocator* alloc2 = m->get(alloc2_id);
		assert(alloc2);
		assert(alloc2->getOID() == alloc2_id);

		assert(len(m) == 2);

		test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		alloc2->dealloc(test2);
		
		m->free(alloc_id);
		assert(!m->get(alloc_id));
		assert(len(m) == 1);

		alloc2 = m->get(alloc2_id);

		test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);

		alloc2->dealloc(test2);

		test3 = new (*alloc2, 8) TestThree;
		assert(test3->varOne() == 1);
		assert(test3->varTwo() == 2);
		assert(test3->varThree() == 3.3f);

		alloc2->dealloc(test3);

		m->free(alloc2_id);
		assert(!m->get(alloc2_id));
		assert(len(m) == 0);


		MemoryManager::destroyMemoryManagerInstance();
		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}


	void testCreateMultipleAllocatorTypes() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance();
		MemoryManager* m = MemoryManager::getInstance();
		assert(m);
		assert(len(m) == 0);

		OID alloc_id = m->createPoolAllocator(sizeof(TestOne), 100, 4); 
		MemoryAllocator* alloc = m->get(alloc_id);
		assert(alloc);
		assert(alloc->getOID() == alloc_id);
		assert(len(m) == 1);

		TestOne* test1 = new (*alloc) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		alloc->dealloc(test1);

		test1 = new (*alloc) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		OID alloc2_id = m->createStackAllocator(sizeof(TestTwo)*100);

		MemoryAllocator* alloc2 = m->get(alloc2_id);
		assert(alloc2);
		assert(alloc2->getOID() == alloc2_id);
		assert(len(m) == 2);

		TestTwo* test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		alloc2->dealloc();
		
		m->free(alloc_id);
		assert(!m->get(alloc_id));
		assert(len(m) == 1);

		alloc2 = m->get(alloc2_id);
		assert(alloc2->getOID() == alloc2_id);

		test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		((StackMemoryAllocator*)alloc2)->mark();

		test1 = new (*alloc2, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);


		alloc2->dealloc();

		m->free(alloc2_id);
		assert(!m->get(alloc2_id));
		assert(len(m) == 0);


		MemoryManager::destroyMemoryManagerInstance();
		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}

	void testCreateStackAndPoolAllocatorAndFreeObject() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance();
		MemoryManager* m = MemoryManager::getInstance();
		assert(m);
		assert(len(m) == 0);

		OID alloc_id = m->createPoolAllocator(sizeof(TestOne), 100, 4); 
		MemoryAllocator* alloc = m->get(alloc_id);
		assert(alloc);
		assert(alloc->getOID() == alloc_id);
		assert(len(m) == 1);

		TestOne* test1 = new (*alloc) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		alloc->dealloc(test1);

		test1 = new (*alloc) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		OID alloc2_id = m->createStackAllocator(sizeof(TestTwo)*100);

		MemoryAllocator* alloc2 = m->get(alloc2_id);
		assert(alloc2);
		assert(alloc2->getOID() == alloc2_id);
		assert(len(m) == 2);

		TestTwo* test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		alloc2->dealloc();
		
		m->free(alloc);
		assert(!m->get(alloc_id));
		assert(len(m) == 1);

		alloc2 = m->get(alloc2_id);
		assert(alloc2->getOID() == alloc2_id);

		test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		((StackMemoryAllocator*)alloc2)->mark();

		test1 = new (*alloc2, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);


		alloc2->dealloc();

		m->free(alloc2);
		assert(!m->get(alloc2_id));
		assert(len(m) == 0);


		MemoryManager::destroyMemoryManagerInstance();
		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}


	void testCreateAndDestroyBeforeFreeing() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance();
		MemoryManager* m = MemoryManager::getInstance();
		assert(m);
		assert(len(m) == 0);

		TestOne* test1; 

		// Create the Stack allocator
		OID alloc2_id = m->createStackAllocator(sizeof(TestTwo)*100);

		MemoryAllocator* alloc2 = m->get(alloc2_id);
		assert(alloc2);
		assert(alloc2->getOID() == alloc2_id);
		assert(len(m) == 1);

		// Create a TestTwo object with the stack allocator
		TestTwo* test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		alloc2->dealloc();

		// Stack allocator now empty
		
		alloc2 = m->get(alloc2_id);

		// Create a TestOne object with the stack allocator
		test1 = new (*alloc2, 4) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		((StackMemoryAllocator*)alloc2)->mark();

		// Create a TestTwo object with the stack allocator
		test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);
		
		alloc2->dealloc();
		// Stack allocator should now contain the TestOne object

		test1->setTimesTwo();
		assert(test1->varOne() == 2);
		assert(test1->varTwo() == 4);
		assert(test1->varThree() == 3.3f*2.0f);

		// Create a new TestTwo object with stack allocator
		test2 = new (*alloc2, 8) TestTwo;
		assert(test2->varOne() == 1);
		assert(test2->varTwo() == 2);
		assert(test2->varThree() == 3.3);

		// Make sure that the TestOne is preserved.
		assert(test1->varOne() == 2);
		assert(test1->varTwo() == 4);
		assert(test1->varThree() == 3.3f*2.0f);


		OID alloc_id = m->createPoolAllocator(sizeof(TestOne), 100, 4); 
		MemoryAllocator* alloc = m->get(alloc_id);
		assert(alloc);
		assert(alloc->getOID() == alloc_id);
		assert(len(m) == 2);

		test1 = new (*alloc) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);
		alloc->dealloc(test1);

		test1 = new (*alloc) TestOne;
		assert(test1->varOne() == 1);
		assert(test1->varTwo() == 2);
		assert(test1->varThree() == 3.3f);

		assert(len(m) == 2);
		MemoryManager::destroyMemoryManagerInstance();
		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}

	void testCreateWithMaxAllocators() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance(2);
		MemoryManager* m = MemoryManager::getInstance();
		assert(m);
		assert(len(m) == 0);
		assert(m->getMaxAllocators() == 2);

		OID a_1 = m->createStackAllocator(1234);
		assert(a_1 > 0);
		assert(len(m) == 1);

		OID a_2 = m->createStackAllocator(1234);
		assert(a_1 > 0);
		assert(len(m) == 2);

		OID a_3 = m->createStackAllocator(1234);
		assert(a_3 == 0);
		assert(!m->get(a_3));
		assert(len(m) == 2);

		m->free(a_1);
		assert(!m->get(a_1));
		assert(m->get(a_2));
		assert(len(m) == 1);

		a_3 = m->createStackAllocator(1234);
		assert(a_3 > 0);
		assert(len(m) == 2);

		m->free(a_2);
		assert(!m->get(a_2));
		assert(m->get(a_3));
		assert(len(m) == 1);

		m->free(a_3);
		assert(!m->get(a_3));
		assert(len(m) == 0);

		a_2 = m->createStackAllocator(1234);
		assert(a_1 > 0);
		assert(len(m) == 1);

		m->free(a_2);
		assert(!m->get(a_2));
		assert(len(m) == 0);


		a_3 = m->createStackAllocator(1234);
		assert(a_3 > 0);
		assert(len(m) == 1);

		a_1 = m->createStackAllocator(1234);
		assert(a_1 > 0);
		assert(len(m) == 2);

		a_2 = m->createStackAllocator(1234);
		assert(a_2 == 0);
		assert(!m->get(a_2));
		assert(len(m) == 2);


		MemoryManager::destroyMemoryManagerInstance();
		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}

	void testMemoryManagerGetInstance() {
		BEGIN_TEST;

		MemoryManager::initializeMemoryManagerInstance(2);
		MemoryManager *m1, *m2, *m3, *m4; 
		m1 = MemoryManager::getInstance();
		assert(m1);
		assert(len(m1) == 0);
		assert(m1->getMaxAllocators() == 2);

		OID a_1 = m1->createStackAllocator(1234);
		assert(a_1 > 0);
		assert(len(m1) == 1);

		m2 = MemoryManager::getInstance();

		OID a_2 = m2->createStackAllocator(1234);
		assert(a_1 > 0);
		assert(len(m2) == 2);

		m3 = MemoryManager::getInstance();

		OID a_3 = m3->createStackAllocator(1234);
		assert(a_3 == 0);
		assert(!m3->get(a_3));
		assert(len(m3) == 2);

		m4 = MemoryManager::getInstance();

		m4->free(a_1);
		assert(!m4->get(a_1));
		assert(m4->get(a_2));
		assert(len(m4) == 1);



		a_3 = m2->createStackAllocator(1234);
		assert(a_3 > 0);
		assert(len(m2) == 2);

		m1->free(a_2);
		assert(!m1->get(a_2));
		assert(m1->get(a_3));
		assert(len(m1) == 1);

		m4->free(a_3);
		assert(!m4->get(a_3));
		assert(len(m4) == 0);

		a_2 = m3->createStackAllocator(1234);
		assert(a_1 > 0);
		assert(len(m3) == 1);

		m1->free(a_2);
		assert(!m2->get(a_2));
		assert(len(m3) == 0);


		a_3 = m4->createStackAllocator(1234);
		assert(a_3 > 0);
		assert(len(m1) == 1);

		a_1 = m2->createStackAllocator(1234);
		assert(a_1 > 0);
		assert(len(m4) == 2);

		a_2 = m3->createStackAllocator(1234);
		assert(a_2 == 0);
		assert(!m1->get(a_2));
		assert(len(m2) == 2);


		MemoryManager::destroyMemoryManagerInstance();
		assert(!MemoryManager::getInstance());

		FINISH_TEST;

	}
}

int main(int argc, char** argv) {
	cc::testMemoryManagerInitialize();
	cc::testCreatePoolAllocator();
	cc::testCreateStackAllocator();
	cc::testCreateChunkAllocator();
	cc::testCreateDynamicChunkAllocator();
	cc::testCreateMultipleAllocatorTypes();
	cc::testCreateStackAndPoolAllocatorAndFreeObject();
	cc::testCreateAndDestroyBeforeFreeing();
	cc::testCreateWithMaxAllocators();
	cc::testMemoryManagerGetInstance();
	

	return 0;
}

