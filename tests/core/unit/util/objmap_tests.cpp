#include "core/testcore.h"
#include "core/util/objmap.h"

namespace cc {

	Size destructorsCalled = 0;

	class TestObj {
		public:
			TestObj() : m_pName(NIL), m_oid(0) { }
			TestObj(const Char* name) {
				m_pName = copy(name);
				m_oid = crc32(m_pName);
			}

			~TestObj() {
				if (m_pName) {
					free(m_pName);
					m_pName = NIL;
				}
				destructorsCalled++;
			}

			inline OID getOID() const { return m_oid; }
			inline const Char* getName() const { return m_pName; }

			inline static U32 getMemoryAlignment() { return (sizeof(Char*) > sizeof(OID)) ? sizeof(Char*) : sizeof(OID); }

		private:
			Char* m_pName;
			OID	m_oid;
	};

	class TestCallable : public Callable {
		public:
			TestCallable(TestObj* obj) : m_numEqual(0), m_pObj(obj) {}

			void call() {
				// Do nothing. 
			}
			void call(VPtr data) {
				TestObj* testobj = reinterpret_cast<TestObj*>(data);
				if ( eq(m_pObj->getName(), testobj->getName()) ) {
					m_numEqual++;	
				}
			}

			U32 getNumEqual() const { return m_numEqual; }

			void reset() { m_numEqual = 0; }

			void setTestObj(TestObj* testobj) { 
				m_pObj = testobj;
			}

		private:
			U32		m_numEqual;
			TestObj* m_pObj;

	};


	void testObjMapBasicCreateAndDestroy() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		TestObj* obj = new TestObj("meow");
		assert( !map->contains(obj->getOID()) );

		delete map;

		map = new ObjMap<TestObj>(100);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 100 );
		assert( map->getLoadFactor() == 0.8f );
		assert( map->getNumBuckets() == 125 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		delete map;

		map = new ObjMap<TestObj>(100, 0.5f);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 100 );
		assert( map->getLoadFactor() == 0.5f );
		assert( map->getNumBuckets() == 200 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		delete map;

			
		FINISH_TEST;
	}

	void testObjMapBasicReserveNoObjects() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 32 );
		assert( map->getLoadFactor() == 0.8f );
		assert( map->getNumBuckets() == 40 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		TestObj* obj = new TestObj("meow");
		assert( !map->contains(obj->getOID()) );

		map->reserve(20);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 32 );
		assert( map->getLoadFactor() == 0.8f );
		assert( map->getNumBuckets() == 40 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		map->reserve(100);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 100 );
		assert( map->getLoadFactor() == 0.8f );
		assert( map->getNumBuckets() == 125 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );


		delete map;

		map = new ObjMap<TestObj>(100, 0.5f);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 100 );
		assert( map->getLoadFactor() == 0.5f );
		assert( map->getNumBuckets() == 200 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		map->reserve(99);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 100 );
		assert( map->getLoadFactor() == 0.5f );
		assert( map->getNumBuckets() == 200 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		map->reserve(101);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 101 );
		assert( map->getLoadFactor() == 0.5f );
		assert( map->getNumBuckets() == 202 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		delete map;

			
		FINISH_TEST;
	}

	void testObjMapBasicInsert() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(obj1) && !map->contains(obj1->getOID()) && !map->contains("meow") );
		assert( !map->contains(obj2) && !map->contains(obj2->getOID()) && !map->contains("moo") );
		assert( !map->contains(obj3) && !map->contains(obj3->getOID()) && !map->contains("moo2") );
		assert( !map->contains(obj4) && !map->contains(obj4->getOID()) && !map->contains("meow0") );
		assert( !map->contains(obj5) && !map->contains(obj5->getOID()) && !map->contains("mooo") );
		assert( !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj4);
		map->insert(obj5);
		map->insert(obj3);

		assert( !map->isEmpty() );
		assert( map->getSize() == 5 && size(map) == 5 );

		assert( map->contains(obj1) && map->contains(obj1->getOID()) && map->contains("meow") );
		assert( map->contains(obj2) && map->contains(obj2->getOID()) && map->contains("moo") );
		assert( map->contains(obj3) && map->contains(obj3->getOID()) && map->contains("moo2") );
		assert( map->contains(obj4) && map->contains(obj4->getOID()) && map->contains("meow0") );
		assert( map->contains(obj5) && map->contains(obj5->getOID()) && map->contains("mooo") );
		assert( map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );

		assert( !map->isEmpty() && map->getSize() == 5 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		delete map;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
			
		FINISH_TEST;
	}

	void testObjMapBasicReserveWithObjects() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 32 );
		assert( map->getLoadFactor() == 0.8f );
		assert( map->getNumBuckets() == 40 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("cowAAAAA");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj4);
		map->insert(obj5);
		map->insert(obj3);

		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );

		map->reserve(20);
		assert( !map->isEmpty() );
		assert( map->getSize() == 5 );
		assert( map->getCapacity() == 32 );
		assert( map->getLoadFactor() == 0.8f );
		assert( map->getNumBuckets() == 40 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );


		map->reserve(100);
		assert( !map->isEmpty() );
		assert( map->getSize() == 5 );
		assert( map->getCapacity() == 100 );
		assert( map->getLoadFactor() == 0.8f );
		assert( map->getNumBuckets() == 125 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );

		delete map;

		map = new ObjMap<TestObj>(100, 0.5);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 100 );
		assert( map->getLoadFactor() == 0.5 );
		assert( map->getNumBuckets() == 200 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj4);
		map->insert(obj5);
		map->insert(obj3);


		map->reserve(99);
		assert( !map->isEmpty() );
		assert( map->getSize() == 5 );
		assert( map->getCapacity() == 100 );
		assert( map->getLoadFactor() == 0.5 );
		assert( map->getNumBuckets() == 200 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );

		map->reserve(101);
		assert( !map->isEmpty() );
		assert( map->getSize() == 5 );
		assert( map->getCapacity() == 101 );
		assert( map->getLoadFactor() == 0.5 );
		assert( map->getNumBuckets() == 202 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );


		delete map;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;

		FINISH_TEST;
	}

	void testObjMapBasicAutoIncreaseCapacity() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>(5, 0.5);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 );
		assert( map->getCapacity() == 5 );
		assert( map->getLoadFactor() == 0.5 );
		assert( map->getNumBuckets() == 10 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("cowAAAAA");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj4);
		map->insert(obj5);
		map->insert(obj3);

		assert( !map->isEmpty() && size(map) == 5 && map->getCapacity() == 5  && map->getLoadFactor() == 0.5 && map->getNumBuckets() == 10 );
		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );

		TestObj* obj6 = new TestObj("Chachacha");

		map->insert(obj6);
		assert( !map->isEmpty() && size(map) == 6 && map->getCapacity() == 10  && map->getLoadFactor() == 0.5 && map->getNumBuckets() == 20 );
		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );
		assert( map->at("Chachacha") == obj6 );

		map->clear();
		assert( map->isEmpty() && size(map) == 0 && map->getCapacity() == 10  && map->getLoadFactor() == 0.5 && map->getNumBuckets() == 20 );


		delete map;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
		delete obj6;


			
		FINISH_TEST;
	}



	void testObjMapBasicRemove() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		TestObj* obj = new TestObj("meow");
		assert( !map->contains(obj->getOID()) );

		map->insert(obj);

		map->remove(obj);

		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(obj->getOID()) );

		map->insert(obj);

		TestObj* obj2 = new TestObj("Moo");

		map->insert(obj2);

		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		map->remove(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		map->remove(obj2);
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->insert(obj2);
		map->insert(obj);

		map->remove(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		map->remove(obj2);
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->remove(obj);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );

		map->insert(obj2);
		map->insert(obj);

		map->remove(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		map->remove(obj2);
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->insert(obj);
		map->insert(obj);
		map->insert(obj2);
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		map->remove(obj2);
		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));

		map->remove(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));

		map->remove(obj);
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));


		map->insert(obj);
		map->insert(obj2);
		map->insert(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		map->remove(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		map->remove(obj2);
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));

		map->remove(obj);
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));


		/* Test remove by Name */
		map->insert(obj);
		map->insert(obj2);
		map->insert(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()) );

		map->remove("Meow"); // not 'meow'
		map->remove("moo");	// not 'Moo'
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()) );

		map->remove("meow");
		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		map->remove("meow");
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		map->remove("Moo");
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));


		/* Test remove by OID */

		map->insert(obj);
		map->insert(obj2);
		map->insert(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()) );

		map->remove(crc32("Meow")); // not 'meow'
		map->remove(crc32("moo"));	// not 'Moo'
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()) );

		map->remove(obj->getOID());
		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		map->remove(obj->getOID());
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		map->remove(obj2->getOID());
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
		delete obj;
		delete obj2;
			
		FINISH_TEST;
	}

	void testObjMapBasicTake() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );

		TestObj* obj = new TestObj("meow");
		assert( !map->contains(obj->getOID()) );

		map->insert(obj);

		assert( map->take(obj) == obj );

		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(obj->getOID()) );

		map->insert(obj);

		TestObj* obj2 = new TestObj("Moo");

		map->insert(obj2);

		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		assert( map->take(obj) == obj );
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		assert( map->take(obj2) == obj2 );
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->insert(obj2);
		map->insert(obj);

		assert( map->take(obj) == obj );
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		assert( map->take(obj2) == obj2 );
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		assert( !map->take(obj) );
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );

		map->insert(obj2);
		map->insert(obj);

		assert( map->take(obj) == obj );
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		assert( map->take(obj2) == obj2 );
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->insert(obj);
		map->insert(obj);
		map->insert(obj2);
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		assert( map->take(obj2) == obj2 );
		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));

		assert( map->take(obj) == obj );
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));

		assert( map->take(obj) == obj );
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));


		map->insert(obj);
		map->insert(obj2);
		map->insert(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		assert( map->take(obj) == obj );
		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		assert( map->take(obj2) == obj2 );
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));

		assert( map->take(obj) == obj );
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));


		/* Test remove by Name */
		map->insert(obj);
		map->insert(obj2);
		map->insert(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()) );

		assert( !map->take("Meow") ); // not 'meow'
		assert( !map->take("moo") );	// not 'Moo'
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()) );

		assert( map->take("meow") == obj );
		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		assert( map->take("meow") == obj);
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		assert( map->take("Moo") == obj2 );
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));


		/* Test remove by OID */

		map->insert(obj);
		map->insert(obj2);
		map->insert(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()) );

		assert( !map->take(crc32("Meow")) ); // not 'meow'
		assert( !map->take(crc32("moo")) );	// not 'Moo'
		assert( !map->isEmpty() );
		assert( size(map) == 3 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()) );

		assert( map->take(obj->getOID()) == obj );
		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		assert( map->take(obj->getOID()) == obj );
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( !map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( map->at(obj2->getOID()));

		assert( map->take(obj2->getOID()) == obj2 );
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->at(obj->getOID()) );
		assert( !map->at(obj2->getOID()));

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
		delete obj;
		delete obj2;
			
		FINISH_TEST;
	}

	void testObjMapBasicClear() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->getAllocator() && !map->doesOwnAllocator() );

		TestObj* obj = new TestObj("meow");
		assert( !map->contains(obj->getOID()) );

		map->insert(obj);

		map->remove(obj);

		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(obj->getOID()) );

		map->insert(obj);

		TestObj* obj2 = new TestObj("Moo");

		map->insert(obj2);

		assert( !map->isEmpty() );
		assert( size(map) == 2 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		map->clear();
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->insert(obj2);
		map->insert(obj);

		map->remove(obj2);
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->clear();
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->insert(obj2);
		map->insert(obj);

		map->remove(obj2);
		assert( !map->isEmpty() );
		assert( size(map) == 1 ); 
		assert( map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->clear();
		assert( map->isEmpty() );
		assert( size(map) == 0 ); 
		assert( !map->contains(obj->getOID()) );
		assert( !map->contains(obj2->getOID()) );

		map->clear();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );

		map->insert(obj);
		map->insert(obj2);
		map->insert(obj);
		map->insert(obj);
		assert( !map->isEmpty() );
		assert( size(map) == 4 ); 
		assert( map->contains(obj->getOID()) );
		assert( map->contains(obj2->getOID()) );

		map->clear();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );


		delete map;
		delete obj;
		delete obj2;

			
		FINISH_TEST;
	}

	void testObjMapBasicEraseAll() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);
		map->insert(obj4);
		map->insert(obj5);

		assert( map->contains(obj1->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( map->contains(obj5->getOID()) );


		destructorsCalled = 0;
		map->eraseAll();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);

		assert( map->contains(obj1->getOID()) && map->contains("meow") && map->contains(obj1) );
		assert( map->contains(obj2->getOID()) && map->contains("moo") && map->contains(obj2) );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") && map->contains(obj3) );
		assert( !map->contains(obj4->getOID()) && !map->contains("meow0") && !map->contains(obj4) );
		assert( !map->contains(obj5->getOID()) && !map->contains("mooo") && !map->contains(obj5) );

		destructorsCalled = 0;
		map->eraseAll();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 3 );

		assert( !map->contains(obj1->getOID()) && !map->contains("meow") && !map->contains(obj1) );
		assert( !map->contains(obj2->getOID()) && !map->contains("moo") && !map->contains(obj2) );
		assert( !map->contains(obj3->getOID()) && !map->contains("moo2") && !map->contains(obj3) );
		assert( !map->contains(obj4->getOID()) && !map->contains("meow0") && !map->contains(obj4) );
		assert( !map->contains(obj5->getOID()) && !map->contains("mooo") && !map->contains(obj5) );

		map->insert(obj4);
		map->insert(obj5);

		assert( !map->contains(obj1->getOID()) && !map->contains("meow") && !map->contains(obj1) );
		assert( !map->contains(obj2->getOID()) && !map->contains("moo") && !map->contains(obj2) );
		assert( !map->contains(obj3->getOID()) && !map->contains("moo2") && !map->contains(obj3) );
		assert( map->contains(obj4->getOID()) && map->contains("meow0") && map->contains(obj4) );
		assert( map->contains(obj5->getOID()) && map->contains("mooo") && map->contains(obj5) );


		map->eraseAll();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );


		delete map;
			
		FINISH_TEST;
	}

	void testObjMapBasicEraseAllWithObjectAllocator() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );

		PoolMemoryAllocator* alloc = new PoolMemoryAllocator(sizeof(TestObj), 10, TestObj::getMemoryAlignment());
		
		assert( alloc->getBlockSize() == sizeof(TestObj) );
		assert( alloc->getNumberOfBlocks() == 10 );

		TestObj* obj1 = new (*alloc) TestObj("meow");
		TestObj* obj2 = new (*alloc) TestObj("moo");
		TestObj* obj3 = new (*alloc) TestObj("moo2");
		TestObj* obj4 = new (*alloc) TestObj("meow0");
		TestObj* obj5 = new (*alloc) TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);
		map->insert(obj4);
		map->insert(obj5);

		assert( map->contains(obj1->getOID()) && map->at(obj1->getOID()) == obj1 );
		assert( map->contains(obj2->getOID()) && map->at(obj2->getOID()) == obj2 );
		assert( map->contains(obj3->getOID()) && map->at(obj3->getOID()) == obj3 );
		assert( map->contains(obj4->getOID()) && map->at(obj4->getOID()) == obj4 );
		assert( map->contains(obj5->getOID()) && map->at(obj5->getOID()) == obj5 );


		destructorsCalled = 0;
		map->eraseAll(alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );


		obj1 = new (*alloc) TestObj("meow");
		obj2 = new (*alloc) TestObj("moo");
		obj3 = new (*alloc) TestObj("moo2");
		obj4 = new (*alloc) TestObj("meow0");
		obj5 = new (*alloc) TestObj("mooo");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);

		assert( map->contains(obj1->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		destructorsCalled = 0;
		map->eraseAll(alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 3 );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj4);
		map->insert(obj5);

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( map->contains(obj5->getOID()) );

		map->eraseAll(alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
		delete alloc;
			
		FINISH_TEST;
	}

	void testObjMapBasicErase() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);
		map->insert(obj4);
		map->insert(obj5);

		assert( map->contains(obj1->getOID()) && map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains("moo") );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") );
		assert( map->contains(obj4->getOID()) && map->contains("meow0") );
		assert( map->contains(obj5->getOID()) && map->contains("mooo") );


		destructorsCalled = 0;
		map->erase("meow");
		assert( !map->isEmpty() );
		assert( map->getSize() == 4 && size(map) == 4 );
		assert( destructorsCalled == 1 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( map->contains(obj5->getOID()) );

		map->erase("mooo");
		assert( !map->isEmpty() );
		assert( map->getSize() == 3 && size(map) == 3 );
		assert( destructorsCalled == 2 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );

		map->erase("moo2");
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( destructorsCalled == 3 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );

		map->erase("moo");
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( destructorsCalled == 4 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );


		map->erase("meow0");
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );
		
		obj2 = new TestObj("mooAAA");
		map->insert(obj2);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		map->erase("mooAA");
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		map->erase("mooAAA");
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains("mooAAA") );
		assert( destructorsCalled == 6 );


		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);

		assert( map->contains(obj1->getOID()) && map->contains("meow") && map->contains(obj1) );
		assert( map->contains(obj2->getOID()) && map->contains("moo") && map->contains(obj2) );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") && map->contains(obj3) );
		assert( !map->contains(obj4->getOID()) && !map->contains("meow0") && !map->contains(obj4) );
		assert( !map->contains(obj5->getOID()) && !map->contains("mooo") && !map->contains(obj5) );

		destructorsCalled = 0;
		map->erase(obj1);
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( destructorsCalled == 1 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains(obj2) );
		assert( map->contains(obj3->getOID()) && map->contains(obj3) );

		map->erase(obj3->getOID());
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( destructorsCalled == 2 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains(obj2) );
		assert( !map->contains("moo2") );

		map->erase(obj2->getName());
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 3 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );

		map->insert(obj4);

		map->erase(crc32(obj4->getName()));
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 4 );

		map->insert(obj5);
		map->erase(obj5);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		map->erase(crc32("moooo"));
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
			
		FINISH_TEST;
	}

	void testObjMapBasicEraseWithObjectAllocator() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );

		PoolMemoryAllocator* alloc = new PoolMemoryAllocator(sizeof(TestObj), 10, TestObj::getMemoryAlignment());
		
		assert( alloc->getBlockSize() == sizeof(TestObj) );
		assert( alloc->getNumberOfBlocks() == 10 );

		TestObj* obj1 = new (*alloc) TestObj("meow");
		TestObj* obj2 = new (*alloc) TestObj("moo");
		TestObj* obj3 = new (*alloc) TestObj("moo2");
		TestObj* obj4 = new (*alloc) TestObj("meow0");
		TestObj* obj5 = new (*alloc) TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);
		map->insert(obj4);
		map->insert(obj5);

		assert( map->contains(obj1->getOID()) && map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains("moo") );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") );
		assert( map->contains(obj4->getOID()) && map->contains("meow0") );
		assert( map->contains(obj5->getOID()) && map->contains("mooo") );


		destructorsCalled = 0;
		map->erase("meow", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 4 && size(map) == 4 );
		assert( destructorsCalled == 1 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( map->contains(obj5->getOID()) );

		map->erase("mooo", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 3 && size(map) == 3 );
		assert( destructorsCalled == 2 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );

		map->erase("moo2", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( destructorsCalled == 3 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );

		map->erase("moo", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( destructorsCalled == 4 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );


		map->erase("meow0", alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );
		
		obj2 = new (*alloc) TestObj("mooAAA");
		map->insert(obj2);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		map->erase("mooAA", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		map->erase("mooAAA", alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains("mooAAA") );
		assert( destructorsCalled == 6 );


		obj1 = new (*alloc) TestObj("meow");
		obj2 = new (*alloc) TestObj("moo");
		obj3 = new (*alloc) TestObj("moo2");
		obj4 = new (*alloc) TestObj("meow0");
		obj5 = new (*alloc) TestObj("mooo");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);

		assert( map->contains(obj1->getOID()) && map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains("moo") );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") );
		assert( !map->contains(obj4->getOID()) && !map->contains("meow0") );
		assert( !map->contains(obj5->getOID()) && !map->contains("mooo") );

		destructorsCalled = 0;
		map->erase(obj1, alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( destructorsCalled == 1 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );

		map->erase(obj3->getOID(), alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( destructorsCalled == 2 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( !map->contains("moo2") );

		map->erase(obj2->getName(), alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 3 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );

		map->insert(obj4);

		map->erase(crc32(obj4->getName()), alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 4 );

		map->insert(obj5);
		map->erase(obj5, alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		map->erase(crc32("moooo"), alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );


		delete map;
		delete alloc;

					
		FINISH_TEST;
	}



	void testObjMapSetAndCreateAllocator() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		PoolMemoryAllocator* alloc = new PoolMemoryAllocator(sizeof(ObjLink<TestObj>), 10, TestObj::getMemoryAlignment());
		
		assert( alloc->getBlockSize() == sizeof(ObjLink<TestObj>));
		assert( alloc->getNumberOfBlocks() == 10 );

		assert( map->setAllocator(alloc) );
		assert( map->getAllocator() == alloc );
		assert( !map->doesOwnAllocator() );

		PoolMemoryAllocator* alloc2 = new PoolMemoryAllocator(sizeof(ObjLink<TestObj>), 12, TestObj::getMemoryAlignment());
		assert( map->setAllocator(alloc2) );
		assert( map->getAllocator() == alloc2 );
		assert( !map->doesOwnAllocator() );

		TestObj* obj = new TestObj("meow");

		map->insert(obj);
		assert( !map->setAllocator(alloc) );
		assert( map->getAllocator() == alloc2 );
		assert( !map->doesOwnAllocator() );

		map->clear();
		assert( map->setAllocator(alloc) );
		assert( map->getAllocator() == alloc );
		assert( !map->doesOwnAllocator() );

		assert( map->createAllocator(1) );
		assert( map->getAllocator() != alloc2 && map->getAllocator() != alloc );
		assert( map->doesOwnAllocator() );

		map->insert(obj);
		assert( !map->setAllocator(alloc) );
		assert( !map->createAllocator(2) );
		assert( map->getAllocator() != alloc2 && map->getAllocator() != alloc );
		assert( map->doesOwnAllocator() );

		map->clear();
		assert( map->setAllocator(alloc) );
		assert( map->getAllocator() == alloc );
		assert( !map->doesOwnAllocator() );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
		delete alloc;
		delete alloc2;
		delete obj;

		FINISH_TEST;
	}

	void testObjMapInsertWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );


		assert( map->createAllocator(5) );
		assert( map->getAllocator() );
		assert( map->doesOwnAllocator() );
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );


		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(crc32(obj1->getName())) );
		assert( !map->contains(crc32(obj2->getName())) );
		assert( !map->contains(crc32(obj3->getName())) );
		assert( !map->contains(crc32(obj4->getName())) );
		assert( !map->contains(crc32(obj5->getName())) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj4);
		map->insert(obj5);
		map->insert(obj3);

		assert( !map->isEmpty() && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 5 && size(map) == 5 );

		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );

		delete map;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}


	void testObjMapRemoveWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		assert( map->createAllocator(5) );
		assert( map->getAllocator() );
		assert( map->doesOwnAllocator() );
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(crc32(obj1->getName())) );
		assert( !map->contains(crc32(obj2->getName())) );
		assert( !map->contains(crc32(obj3->getName())) );
		assert( !map->contains(crc32(obj4->getName())) );
		assert( !map->contains(crc32(obj5->getName())) );

		map->insert(obj4);
		map->insert(obj3);
		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj5);

		assert( !map->isEmpty() );
		assert( map->getSize() == 5 && size(map) == 5 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );


		map->remove("moo2");
		assert( !map->isEmpty() );
		assert( map->getSize() == 4 && size(map) == 4 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2"));
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );

		map->remove("mooo");
		assert( !map->isEmpty() );
		assert( map->getSize() == 3 && size(map) == 3 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2"));
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->remove("meow0");
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->remove("moo");
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->remove("meow");
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->remove("meow");
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );


		map->insert(obj3);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );
		
		map->remove("moo2");
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->insert(obj3);
		map->insert(obj1);
		map->insert(obj4);
		map->insert(obj5);
		map->insert(obj2);

		assert( !map->isEmpty() );
		assert( map->getSize() == 5 && size(map) == 5 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );


		map->remove("moo2");
		assert( !map->isEmpty() );
		assert( map->getSize() == 4 && size(map) == 4 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2"));
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );

		map->remove(obj5->getOID());
		assert( !map->isEmpty() );
		assert( map->getSize() == 3 && size(map) == 3 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2"));
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->remove(obj4);
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->remove(obj2->getName());
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->remove(crc32(obj1->getName()));
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->remove("meow");
		map->remove(obj1);
		map->remove(obj1->getOID());
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}

	void testObjMapTakeWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );


		assert( map->createAllocator(5) );
		assert( map->getAllocator() );
		assert( map->doesOwnAllocator() );
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );


		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(crc32(obj1->getName())) );
		assert( !map->contains(crc32(obj2->getName())) );
		assert( !map->contains(crc32(obj3->getName())) );
		assert( !map->contains(crc32(obj4->getName())) );
		assert( !map->contains(crc32(obj5->getName())) );

		map->insert(obj4);
		map->insert(obj3);
		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj5);

		assert( !map->isEmpty() );
		assert( map->getSize() == 5 && size(map) == 5 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );


		assert( map->take("moo2") == obj3 );
		assert( !map->isEmpty() );
		assert( map->getSize() == 4 && size(map) == 4 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2"));
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );

		assert( map->take("mooo") == obj5 );
		assert( !map->isEmpty() );
		assert( map->getSize() == 3 && size(map) == 3 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2"));
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( map->take("meow0") == obj4 );
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( map->take("moo") == obj2 );
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( map->take("meow") == obj1 );
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( !map->take("meow") );
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );


		map->insert(obj3);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );
		
		assert( map->take("moo2") == obj3 );
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->insert(obj3);
		map->insert(obj1);
		map->insert(obj4);
		map->insert(obj5);
		map->insert(obj2);

		assert( !map->isEmpty() );
		assert( map->getSize() == 5 && size(map) == 5 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );


		assert( map->take("moo2") == obj3 );
		assert( !map->isEmpty() );
		assert( map->getSize() == 4 && size(map) == 4 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2"));
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );

		assert( map->take(obj5->getOID()) == obj5 );
		assert( !map->isEmpty() );
		assert( map->getSize() == 3 && size(map) == 3 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2"));
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( map->take(obj4) == obj4 );
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( map->take(obj2->getName()) == obj2 );
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( map->take(crc32(obj1->getName())) == obj1 );
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( !map->take("meow") );
		assert( !map->take(obj1) );
		assert( !map->take(obj1->getOID()) );
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}


	void testObjMapClearWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		assert( map->createAllocator(5) );
		assert( map->getAllocator() );
		assert( map->doesOwnAllocator() );
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(crc32(obj1->getName())) );
		assert( !map->contains(crc32(obj2->getName())) );
		assert( !map->contains(crc32(obj3->getName())) );
		assert( !map->contains(crc32(obj4->getName())) );
		assert( !map->contains(crc32(obj5->getName())) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj4);
		map->insert(obj5);
		map->insert(obj3);

		assert( !map->isEmpty() );
		assert( map->getSize() == 5 && size(map) == 5 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );


		map->clear();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		map->clear();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );


		map->insert(obj3);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );
		
		map->clear();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );


		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj4);
		assert( !map->isEmpty() );
		assert( map->getSize() == 3 && size(map) == 3 );
		assert( map->contains(crc32(obj1->getName())) && map->contains("meow") && map->at(crc32(obj1->getName())) == obj1 && map->at("meow") == obj1 );
		assert( map->contains(crc32(obj2->getName())) && map->contains("moo") && map->at(crc32(obj2->getName())) == obj2 && map->at("moo") == obj2 );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( map->contains(crc32(obj4->getName())) && map->contains("meow0") && map->at(crc32(obj4->getName())) == obj4 && map->at("meow0") == obj4 );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );


		map->clear();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );


		map->insert(obj5);
		map->insert(obj3);
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( map->contains(crc32(obj3->getName())) && map->contains("moo2") && map->at(crc32(obj3->getName())) == obj3 && map->at("moo2") == obj3 );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( map->contains(crc32(obj5->getName())) && map->contains("mooo") && map->at(crc32(obj5->getName())) == obj5 && map->at("mooo") == obj5 );


		map->clear();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains(crc32(obj1->getName())) && !map->contains("meow") && !map->at(crc32(obj1->getName())) && !map->at("meow") );
		assert( !map->contains(crc32(obj2->getName())) && !map->contains("moo") && !map->at(crc32(obj2->getName())) && !map->at("moo") );
		assert( !map->contains(crc32(obj3->getName())) && !map->contains("moo2") && !map->at(crc32(obj3->getName())) && !map->at("moo2") );
		assert( !map->contains(crc32(obj4->getName())) && !map->contains("meow0") && !map->at(crc32(obj4->getName())) && !map->at("meow0") );
		assert( !map->contains(crc32(obj5->getName())) && !map->contains("mooo") && !map->at(crc32(obj5->getName())) && !map->at("mooo") );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}

	void testObjMapEraseAllWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );


		assert( map->createAllocator(5) );
		assert( map->getAllocator() );
		assert( map->doesOwnAllocator() );
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);
		map->insert(obj4);
		map->insert(obj5);

		assert( map->contains(obj1->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( map->contains(obj5->getOID()) );

		destructorsCalled = 0;
		map->eraseAll();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);

		assert( map->contains(obj1->getOID()) && map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains("moo") );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") );
		assert( !map->contains(obj4->getOID()) && !map->contains("meow0") );
		assert( !map->contains(obj5->getOID()) && !map->contains("mooo") );

		destructorsCalled = 0;
		map->eraseAll();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 3 );

		assert( !map->contains(obj1->getOID()) && !map->contains("meow") );
		assert( !map->contains(obj2->getOID()) && !map->contains("moo") );
		assert( !map->contains(obj3->getOID()) && !map->contains("moo2") );
		assert( !map->contains(obj4->getOID()) && !map->contains("meow0") );
		assert( !map->contains(obj5->getOID()) && !map->contains("mooo") );

		map->insert(obj4);
		map->insert(obj5);

		assert( !map->contains(obj1->getOID()) && !map->contains("meow") );
		assert( !map->contains(obj2->getOID()) && !map->contains("moo") );
		assert( !map->contains(obj3->getOID()) && !map->contains("moo2") );
		assert( map->contains(obj4->getOID()) && map->contains("meow0") );
		assert( map->contains(obj5->getOID()) && map->contains("mooo") );

		map->eraseAll();
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
			
		FINISH_TEST;
	}

	void testObjMapEraseAllWithNodeAndObjectAllocators() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );


		assert( map->createAllocator(5) );
		assert( map->getAllocator() );
		assert( map->doesOwnAllocator() );
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );

		PoolMemoryAllocator* alloc = new PoolMemoryAllocator(sizeof(TestObj), 5, TestObj::getMemoryAlignment());
		
		assert( alloc->getBlockSize() == sizeof(TestObj) );
		assert( alloc->getNumberOfBlocks() == 5 );

		TestObj* obj1 = new (*alloc) TestObj("meow");
		TestObj* obj2 = new (*alloc) TestObj("moo");
		TestObj* obj3 = new (*alloc) TestObj("moo2");
		TestObj* obj4 = new (*alloc) TestObj("meow0");
		TestObj* obj5 = new (*alloc) TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);
		map->insert(obj4);
		map->insert(obj5);

		assert( map->contains(obj1->getOID()) && map->at(obj1->getOID()) == obj1 );
		assert( map->contains(obj2->getOID()) && map->at(obj2->getOID()) == obj2 );
		assert( map->contains(obj3->getOID()) && map->at(obj3->getOID()) == obj3 );
		assert( map->contains(obj4->getOID()) && map->at(obj4->getOID()) == obj4 );
		assert( map->contains(obj5->getOID()) && map->at(obj5->getOID()) == obj5 );

		destructorsCalled = 0;
		map->eraseAll(alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		obj1 = new (*alloc) TestObj("meow");
		obj2 = new (*alloc) TestObj("moo");
		obj3 = new (*alloc) TestObj("moo2");
		obj4 = new (*alloc) TestObj("meow0");
		obj5 = new (*alloc) TestObj("mooo");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);

		assert( map->contains(obj1->getOID()) );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		destructorsCalled = 0;
		map->eraseAll(alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 3 );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj4);
		map->insert(obj5);

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( map->contains(obj5->getOID()) );

		map->eraseAll(alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
		delete alloc;
			
		FINISH_TEST;
	}

	void testObjMapEraseWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		assert( map->createAllocator(5) );
		assert( map->getAllocator() );
		assert( map->doesOwnAllocator() );
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);
		map->insert(obj4);
		map->insert(obj5);

		assert( map->contains(obj1->getOID()) && map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains("moo") );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") );
		assert( map->contains(obj4->getOID()) && map->contains("meow0") );
		assert( map->contains(obj5->getOID()) && map->contains("mooo") );

		destructorsCalled = 0;
		map->erase("meow");
		assert( !map->isEmpty() );
		assert( map->getSize() == 4 && size(map) == 4 );
		assert( destructorsCalled == 1 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( map->contains(obj5->getOID()) );

		map->erase("mooo");
		assert( !map->isEmpty() );
		assert( map->getSize() == 3 && size(map) == 3 );
		assert( destructorsCalled == 2 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );

		map->erase("moo2");
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( destructorsCalled == 3 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );

		map->erase("moo");
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( destructorsCalled == 4 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );


		map->erase("meow0");
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );
		
		obj2 = new TestObj("mooAAA");
		map->insert(obj2);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		map->erase("mooAA");
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		map->erase("mooAAA");
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains("mooAAA") );
		assert( destructorsCalled == 6 );


		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);

		assert( map->contains(obj1->getOID()) && map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains("moo") );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") );
		assert( !map->contains(obj4->getOID()) && !map->contains("meow0") );
		assert( !map->contains(obj5->getOID()) && !map->contains("mooo") );

		destructorsCalled = 0;
		map->erase(obj1);
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( destructorsCalled == 1 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );

		map->erase(obj3->getOID());
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( destructorsCalled == 2 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( !map->contains("moo2") );

		map->erase(obj2->getName());
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 3 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );

		map->insert(obj4);

		map->erase(crc32(obj4->getName()));
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 4 );

		map->insert(obj5);
		map->erase(obj5);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		map->erase(crc32("moooo"));
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
			
		FINISH_TEST;
	}

	void testObjMapEraseWithNodeAndObjectAllocators() {
		BEGIN_TEST;
		
		ObjMap<TestObj>* map = new ObjMap<TestObj>;
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->getAllocator() );
		assert( !map->doesOwnAllocator() );

		assert( map->createAllocator(5) );
		assert( map->getAllocator() );
		assert( map->doesOwnAllocator() );
		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );
		assert( map->getSize() == 0 && size(map) == 0 );

		PoolMemoryAllocator* alloc = new PoolMemoryAllocator(sizeof(TestObj), 5, TestObj::getMemoryAlignment());
		
		assert( alloc->getBlockSize() == sizeof(TestObj) );
		assert( alloc->getNumberOfBlocks() == 5 );

		TestObj* obj1 = new (*alloc) TestObj("meow");
		TestObj* obj2 = new (*alloc) TestObj("moo");
		TestObj* obj3 = new (*alloc) TestObj("moo2");
		TestObj* obj4 = new (*alloc) TestObj("meow0");
		TestObj* obj5 = new (*alloc) TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !map->contains(obj1->getOID()) );
		assert( !map->contains(obj2->getOID()) );
		assert( !map->contains(obj3->getOID()) );
		assert( !map->contains(obj4->getOID()) );
		assert( !map->contains(obj5->getOID()) );

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);
		map->insert(obj4);
		map->insert(obj5);

		assert( map->contains(obj1->getOID()) && map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains("moo") );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") );
		assert( map->contains(obj4->getOID()) && map->contains("meow0") );
		assert( map->contains(obj5->getOID()) && map->contains("mooo") );

		destructorsCalled = 0;
		map->erase("meow", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 4 && size(map) == 4 );
		assert( destructorsCalled == 1 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( map->contains(obj5->getOID()) );

		map->erase("mooo", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 3 && size(map) == 3 );
		assert( destructorsCalled == 2 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );

		map->erase("moo2", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( destructorsCalled == 3 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );

		map->erase("moo", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( destructorsCalled == 4 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->getOID()) );
		assert( !map->contains("mooo") );


		map->erase("meow0", alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );
		
		obj2 = new (*alloc) TestObj("mooAAA");
		map->insert(obj2);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		map->erase("mooAA", alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( map->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		map->erase("mooAAA", alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( !map->contains("mooAAA") );
		assert( destructorsCalled == 6 );

		obj1 = new (*alloc) TestObj("meow");
		obj2 = new (*alloc) TestObj("moo");
		obj3 = new (*alloc) TestObj("moo2");
		obj4 = new (*alloc) TestObj("meow0");
		obj5 = new (*alloc) TestObj("mooo");

		map->insert(obj1);
		map->insert(obj2);
		map->insert(obj3);

		assert( map->contains(obj1->getOID()) && map->contains("meow") );
		assert( map->contains(obj2->getOID()) && map->contains("moo") );
		assert( map->contains(obj3->getOID()) && map->contains("moo2") );
		assert( !map->contains(obj4->getOID()) && !map->contains("meow0") );
		assert( !map->contains(obj5->getOID()) && !map->contains("mooo") );

		destructorsCalled = 0;
		map->erase(obj1, alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 2 && size(map) == 2 );
		assert( destructorsCalled == 1 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( map->contains(obj3->getOID()) );

		map->erase(obj3->getOID(), alloc);
		assert( !map->isEmpty() );
		assert( map->getSize() == 1 && size(map) == 1 );
		assert( destructorsCalled == 2 );
		assert( !map->contains("meow") );
		assert( map->contains(obj2->getOID()) );
		assert( !map->contains("moo2") );

		map->erase(obj2->getName(), alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 3 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );

		map->insert(obj4);

		map->erase(crc32(obj4->getName()), alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 4 );

		map->insert(obj5);
		map->erase(obj5, alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		map->erase(crc32("moooo"), alloc);
		assert( map->isEmpty() );
		assert( map->getSize() == 0 && size(map) == 0 );
		assert( destructorsCalled == 5 );
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		assert( map->isEmpty() && map->getSize() == 0 && map->getCapacity() == 32 && map->getLoadFactor() == 0.8f && map->getNumBuckets() == 40 );

		delete map;
		delete alloc;

					
		FINISH_TEST;
	}

	void testObjMapEach() {
		BEGIN_TEST;

		ObjMap<TestObj>* map = new ObjMap<TestObj>;

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		map->insert(obj1);
		map->insert(obj3);
		map->insert(obj4);
		map->insert(obj3);
		map->insert(obj1);
		map->insert(obj3);

		assert( !map->isEmpty() );
		TestCallable func = TestCallable(obj1);
		assert( func.getNumEqual() == 0 );
		
		map->each(&func);
		assert( func.getNumEqual() == 2 );
		func.reset();
		assert( func.getNumEqual() == 0 );
		
		func.setTestObj(obj2);
		map->each(&func);
		assert( func.getNumEqual() == 0 );

		func.setTestObj(obj4);
		map->each(&func);
		assert( func.getNumEqual() == 1 );
		func.reset();
		assert( func.getNumEqual() == 0 );

		func.setTestObj(obj3);
		map->each(&func);
		assert( func.getNumEqual() == 3 );

		TestCallable* func2 = new TestCallable(obj5);

		assert( func2->getNumEqual() == 0 );
		map->each(func2);
		assert( func2->getNumEqual() == 0 );
		map->insert(obj5);
		map->each(func2);
		assert( func2->getNumEqual() == 1 );

		assert( func.getNumEqual() == 3 );

		delete func2;
		delete map;
		delete obj2;
		delete obj1;
		delete obj3;
		delete obj4;
		delete obj5;

		FINISH_TEST;
	}
}

int main(int argc, char** argv) {
	cc::testObjMapBasicCreateAndDestroy();
	cc::testObjMapBasicReserveNoObjects();
	cc::testObjMapBasicInsert();
	cc::testObjMapBasicReserveWithObjects();
	cc::testObjMapBasicAutoIncreaseCapacity();
	cc::testObjMapBasicRemove();
	cc::testObjMapBasicTake();
	cc::testObjMapBasicClear();
	cc::testObjMapBasicEraseAll();
	cc::testObjMapBasicEraseAllWithObjectAllocator();
	cc::testObjMapBasicErase();
	cc::testObjMapBasicEraseWithObjectAllocator();
	cc::testObjMapSetAndCreateAllocator();
	cc::testObjMapInsertWithNodeAllocator();
	cc::testObjMapRemoveWithNodeAllocator();
	cc::testObjMapTakeWithNodeAllocator();
	cc::testObjMapClearWithNodeAllocator();
	cc::testObjMapEraseAllWithNodeAllocator();
	cc::testObjMapEraseAllWithNodeAndObjectAllocators();
	cc::testObjMapEraseWithNodeAllocator();
	cc::testObjMapEraseWithNodeAndObjectAllocators();
	cc::testObjMapEach();
	return 0;
}




