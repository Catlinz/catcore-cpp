#include "core/testcore.h"
#include "core/util/map.h"

namespace cc {

	Size destructorsCalled = 0;
	
	class TestObj {
	  public:
		TestObj() : m_pName(NIL), m_oid(0) { }
		TestObj(const Char* name) {
			m_pName = copy(name);
			m_oid = crc32(m_pName);
		}

		inline Boolean operator==(const TestObj& rval) const {
			return m_oid == rval.m_oid;
		}
		inline Boolean operator!=(const TestObj& rval) const {
			return m_oid != rval.m_oid;
		}	

		~TestObj() {
			if (m_pName) {
				free(m_pName);
				m_pName = NIL;
			}
			destructorsCalled++;
		}

		inline OID oID() const { return m_oid; }
		inline const Char* name() const { return m_pName; }

	  private:
		Char* m_pName;
		OID	m_oid;
	};

	
	Boolean notIn(TestObj* obj, TestObj** objects, Size num) {
		for (Size i = 0; i < num; i++) {
			if (objects[i] == obj) {
				return false;
			}
		}
		return true;
	}


	void testMapBasicCreateAndDestroy() {
		BEGIN_TEST;

		Map<TestObj*>* map = new Map<TestObj*>;
		ass_eq(map->capacity(), 0);
		ass_eq(map->numBuckets(), 0);
		ass_eq(map->size(), 0);
		ass_eq(map->loadFactor(), 0.0f);
		ass_eq(map->nullValue(), NIL);		
		ass_true(map->isEmpty());
		delete map;
		
		map = new Map<TestObj*>(32, NIL);
		ass_eq(map->capacity(), 32);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->size(), 0);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->nullValue(), NIL);
		ass_true(map->isEmpty());		

		TestObj* obj = new TestObj("meow");
		ass_false(map->contains(obj->oID()));

		delete map;

		map = new Map<TestObj*>(100, 0.5f, NIL);
		ass_eq(map->capacity(), 100);
		ass_eq(map->numBuckets(), 200);
		ass_eq(map->size(), 0);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->nullValue(), NIL);
		ass_true(map->isEmpty());

		delete map;

			
		FINISH_TEST;
	}

	void testMapBasicReserveNoObjects() {
		BEGIN_TEST;
		
		Map<TestObj*>* map = new Map<TestObj*>(32, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);		

		TestObj* obj = new TestObj("meow");
		ass_false(map->contains(obj->oID()));

		map->reserve(20);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		map->reserve(100);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 100);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 125);
		ass_eq(map->nullValue(), NIL);

		delete map;

		map = new Map<TestObj*>(100, 0.5f, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 100);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 200);
		ass_eq(map->nullValue(), NIL);

		map->reserve(99);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 100);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 200);
		ass_eq(map->nullValue(), NIL);

		map->reserve(101);
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 101);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 202);
		ass_eq(map->nullValue(), NIL);

		delete map;

			
		FINISH_TEST;
	}

	void testMapBasicInsert() {
		BEGIN_TEST;
		
		Map<TestObj*>* map = new Map<TestObj*>(32, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		ass_true(obj1->oID() != obj2->oID() && obj1->oID() != obj3->oID() && obj1->oID() != obj4->oID() && obj1->oID() != obj5->oID());
		ass_true(obj2->oID() != obj3->oID() && obj2->oID() != obj4->oID() && obj2->oID() != obj5->oID());
		ass_true(obj3->oID() != obj4->oID() && obj3->oID() != obj5->oID());
		ass_true(obj4->oID() != obj5->oID());

		ass_false(map->contains(obj1) || map->contains(obj1->oID()) || map->contains("meow"));
		ass_false(map->contains(obj2) || map->contains(obj2->oID()) || map->contains("moo"));
		ass_false(map->contains(obj3) || map->contains(obj3->oID()) || map->contains("moo2"));
		ass_false(map->contains(obj4) || map->contains(obj4->oID()) || map->contains("meow0"));
		ass_false(map->contains(obj5) || map->contains(obj5->oID()) || map->contains("mooo"));
		ass_false(map->at(crc32(obj1->name())) || map->at("meow"));
		ass_false(map->at(crc32(obj2->name())) || map->at("moo"));
		ass_false(map->at(crc32(obj3->name())) || map->at("moo2"));
		ass_false(map->at(crc32(obj4->name())) || map->at("meow0"));
		ass_false(map->at(crc32(obj5->name())) || map->at("mooo"));
		ass_false(map->get(crc32(obj1->name())) || map->get("meow"));
		ass_false(map->get(crc32(obj2->name())) || map->get("moo"));
		ass_false(map->get(crc32(obj3->name())) || map->get("moo2"));
		ass_false(map->get(crc32(obj4->name())) || map->get("meow0"));
		ass_false(map->get(crc32(obj5->name())) || map->get("mooo"));

		map->insert(obj1->oID(), obj1);
		map->insert(obj2->name(), obj2);
		map->insert(obj4->oID(), obj4);
		map->insert(obj5->oID(), obj5);
		map->insert(obj3->name(), obj3);

		ass_false(map->isEmpty());
		ass_eq(map->size(), 5);

		ass_true(map->contains(obj1) && map->contains(obj1->oID()) && map->contains("meow"));
		ass_true(map->contains(obj2) && map->contains(obj2->oID()) && map->contains("moo"));
		ass_true(map->contains(obj3) && map->contains(obj3->oID()) && map->contains("moo2"));
		ass_true(map->contains(obj4) && map->contains(obj4->oID()) && map->contains("meow0"));
		ass_true(map->contains(obj5) && map->contains(obj5->oID()) && map->contains("mooo"));
		ass_true(map->at(crc32(obj1->name())) == obj1 && map->at("meow") == obj1);
		ass_true(map->at(crc32(obj2->name())) == obj2 && map->at("moo") == obj2);
		ass_true(map->at(crc32(obj3->name())) == obj3 && map->at("moo2") == obj3);
		ass_true(map->at(crc32(obj4->name())) == obj4 && map->at("meow0") == obj4);
		ass_true(map->at(crc32(obj5->name())) == obj5 && map->at("mooo") == obj5);
		ass_true(map->get(crc32(obj1->name())) == obj1 && map->get("meow") == obj1);
		ass_true(map->get(crc32(obj2->name())) == obj2 && map->get("moo") == obj2);
		ass_true(map->get(crc32(obj3->name())) == obj3 && map->get("moo2") == obj3);
		ass_true(map->get(crc32(obj4->name())) == obj4 && map->get("meow0") == obj4);
		ass_true(map->get(crc32(obj5->name())) == obj5 && map->get("mooo") == obj5);

		ass_false(map->isEmpty());
		ass_eq(map->size(), 5);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);		

		delete map;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
			
		FINISH_TEST;
	}

	void testMapBasicReserveWithObjects() {
		BEGIN_TEST;
		
		Map<TestObj*>* map = new Map<TestObj*>(32, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);		

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("cowAAAAA");

		map->insert(obj1->oID(), obj1);
		map->insert(obj2->oID(), obj2);
		map->insert(obj4->oID(), obj4);
		map->insert(obj5->name(), obj5);
		map->insert(obj3->oID(), obj3);

		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );

		map->reserve(20);
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 5);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );


		map->reserve(100);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 5);
		ass_eq(map->capacity(), 100);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 125);
		ass_eq(map->nullValue(), NIL);

		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );

		delete map;

		map = new Map<TestObj*>(100, 0.5f, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 100);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 200);
		ass_eq(map->nullValue(), NIL);

	   map->insert(obj1->oID(), obj1);
		map->insert(obj2->name(), obj2);
		map->insert(obj4->oID(), obj4);
		map->insert(obj5->name(), obj5);
		map->insert(obj3->oID(), obj3);


		map->reserve(99);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 5);
		ass_eq(map->capacity(), 100);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 200);
		ass_eq(map->nullValue(), NIL);

		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );

		map->reserve(101);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 5);
		ass_eq(map->capacity(), 101);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 202);
		ass_eq(map->nullValue(), NIL);

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

	void testMapBasicAutoIncreaseCapacity() {
		BEGIN_TEST;
		
		Map<TestObj*>* map = new Map<TestObj*>(5, 0.5f, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 5);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 10);
		ass_eq(map->nullValue(), NIL);

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("cowAAAAA");

	   map->insert("meow", obj1);
		map->insert(obj2->name(), obj2);
		map->insert(obj4->oID(), obj4);
		map->insert(obj5->name(), obj5);
		map->insert("moo2", obj3);

		ass_false(map->isEmpty());
		ass_eq(map->size(), 5);
		ass_eq(map->capacity(), 5);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 10);
		ass_eq(map->nullValue(), NIL);
		
		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );

		TestObj* obj6 = new TestObj("Chachacha");

		map->insert(obj6->name(), obj6);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 6);
		ass_eq(map->capacity(), 10);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 20);
		ass_eq(map->nullValue(), NIL);
		
		assert( map->at("meow") == obj1 );
		assert( map->at("moo") == obj2 );
		assert( map->at("moo2") == obj3 );
		assert( map->at("meow0") == obj4 );
		assert( map->at("cowAAAAA") == obj5 );
		assert( map->at("Chachacha") == obj6 );

		map->clear();
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 10);
		ass_eq(map->loadFactor(), 0.5f);
		ass_eq(map->numBuckets(), 20);
		ass_eq(map->nullValue(), NIL);

		delete map;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
		delete obj6;
			
		FINISH_TEST;
	}



	void testMapBasicRemove() {
		BEGIN_TEST;
		
		Map<TestObj*>* map = new Map<TestObj*>(32, NIL);
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);


		TestObj* obj = new TestObj("meow");
		assert( !map->contains(obj->oID()) );

		map->insert("meow", obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		map->remove(obj);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_false(map->contains(obj->oID()));

		map->insert("meow", obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		TestObj* obj2 = new TestObj("Moo");

		map->insert(obj2->oID(), obj2);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		ass_true(map->contains(obj->oID()));
		ass_true(map->contains(obj2->oID()));

		map->remove(obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );

		map->remove(obj2->oID());
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->insert(obj2->oID(), obj2);
		map->insert(obj->oID(), obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		map->remove(obj->name());
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );

		map->remove(obj2);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL); 
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->remove("meow");
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL); 

		map->insert(obj2->name(), obj2);
		map->insert(obj->oID(), obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		map->remove("meow");
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->name()) );
		assert( map->contains(obj2->name()) );

		map->remove(obj2);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->insert("meow", obj);
		map->insert(obj->oID(), obj);
		map->insert(obj2->oID(), obj2);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		map->remove(obj2->oID());
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));

		map->remove(obj->name());
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));

		map->remove("meow");
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));


		map->insert("meow", obj);
		map->insert(obj2->name(), obj2);
		map->insert(obj->name(), obj);
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		map->remove(obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		map->remove(obj2->name());
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));

		map->remove(obj->oID());
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));


		/* Test remove by Name */
		map->insert("meow", obj);
		map->insert(obj2->name(), obj2);
		map->insert(obj->name(), obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()) );

		map->remove("Meow"); // not 'meow'
		map->remove("moo");	// not 'Moo'
		ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()) );

		map->remove("meow");
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		map->remove("meow");
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		map->remove("Moo");
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL); 
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));


		/* Test remove by OID */
		map->insert(obj->oID(), obj);
		map->insert(obj2->oID(), obj2);
		map->insert(obj->oID(), obj);
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()) );

		map->remove(crc32("Meow")); // not 'meow'
		map->remove(crc32("moo"));	// not 'Moo'
		ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()) );

		map->remove(obj->oID());
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		map->remove(obj->oID());
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		map->remove(obj2->oID());
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));

	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		delete map;
		delete obj;
		delete obj2;
			
		FINISH_TEST;
	}

	void testMapBasicTake() {
		BEGIN_TEST;
		
		Map<TestObj*>* map = new Map<TestObj*>(32, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		TestObj* obj = new TestObj("meow");
		assert( !map->contains(obj->oID()) );

		map->insert("meow", obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		assert( map->take(obj) == obj );
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		assert( !map->contains(obj->oID()) );

		map->insert(obj->oID(), obj);
		TestObj* obj2 = new TestObj("Moo");
		map->insert("Moo", obj2);

		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );

		assert( map->take(obj) == obj );
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL); 
		assert( !map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );

		assert( map->take(obj2) == obj2 );
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL); 
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->insert(obj2->name(), obj2);
		map->insert(obj->oID(), obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		assert( map->take(obj) == obj );
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );

		assert( map->take(obj2) == obj2 );
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		assert( !map->take(obj) );
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		map->insert(obj2->name(), obj2);
		map->insert("meow", obj);

		assert( map->take(obj) == obj );
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL); 
		assert( !map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );

		assert( map->take(obj2) == obj2 );
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->insert("meow", obj);
		map->insert(obj->oID(), obj);
		map->insert(obj2->oID(), obj2);
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		assert( map->take(obj2) == obj2 );
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));

		assert( map->take(obj) == obj );
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));

		assert( map->take(obj) == obj );
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));


		map->insert(obj->oID(), obj);
		map->insert(obj2->name(), obj2);
		map->insert("meow", obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		assert( map->take(obj) == obj );
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		assert( map->take(obj2) == obj2 );
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));

		assert( map->take(obj) == obj );
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));


		/* Test remove by Name */
		map->insert("meow", obj);
		map->insert(obj2->name(), obj2);
		map->insert("meow", obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()) );

		assert( !map->take("Meow") ); // not 'meow'
		assert( !map->take("moo") );	// not 'Moo'
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()) );

		assert( map->take("meow") == obj );
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		assert( map->take("meow") == obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		assert( map->take("Moo") == obj2 );
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));


		/* Test remove by OID */
		map->insert("meow", obj);
		map->insert(obj2->oID(), obj2);
		map->insert(obj->oID(), obj);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()) );

		assert( !map->take(crc32("Meow")) ); // not 'meow'
		assert( !map->take(crc32("moo")) );	// not 'Moo'
		ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()) );

		assert( map->take(obj->oID()) == obj );
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		assert( map->take(obj->oID()) == obj );
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( map->at(obj2->oID()));

		assert( map->take(obj2->oID()) == obj2 );
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->at(obj->oID()) );
		assert( !map->at(obj2->oID()));

	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		delete map;
		delete obj;
		delete obj2;
			
		FINISH_TEST;
	}

	void testMapBasicClear() {
		BEGIN_TEST;
		
		Map<TestObj*>* map = new Map<TestObj*>(32, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		TestObj* obj = new TestObj("meow");
		assert( !map->contains(obj->oID()) );

		map->insert("meow", obj);
		map->remove(obj);

	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );

		map->insert("meow", obj);
		TestObj* obj2 = new TestObj("Moo");
		map->insert("Moo", obj2);

		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );

		map->clear();
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->insert(obj2->oID(), obj2);
		map->insert(obj->oID(), obj);

		map->remove(obj2);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->clear();
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->insert(obj2->name(), obj2);
		map->insert(obj->name(), obj);

		map->remove(obj2);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->clear();
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains(obj->oID()) );
		assert( !map->contains(obj2->oID()) );

		map->clear();
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		map->insert("meow", obj);
		map->insert("Moo", obj2);
		map->insert(obj->name(), obj);
		map->insert(obj->oID(), obj);
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 4);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains(obj->oID()) );
		assert( map->contains(obj2->oID()) );

		map->clear();
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		delete map;
		delete obj;
		delete obj2;

			
		FINISH_TEST;
	}

	void testMapBasicEraseAll() {
		BEGIN_TEST;
		
		Map<TestObj*>* map = new Map<TestObj*>(32, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->oID() != obj2->oID() && obj1->oID() != obj3->oID() && obj1->oID() != obj4->oID() && obj1->oID() != obj5->oID() );
		assert( obj2->oID() != obj3->oID() && obj2->oID() != obj4->oID() && obj2->oID() != obj5->oID() );
		assert( obj3->oID() != obj4->oID() && obj3->oID() != obj5->oID() );
		assert( obj4->oID() != obj5->oID() );

		assert( !map->contains(obj1->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->contains(obj3->oID()) );
		assert( !map->contains(obj4->oID()) );
		assert( !map->contains(obj5->oID()) );

		map->insert(obj1->name(), obj1);
		map->insert(obj2->oID(), obj2);
		map->insert(obj3->oID(), obj3);
		map->insert(obj4->name(), obj4);
		map->insert(obj5->oID(), obj5);

		assert( map->contains(obj1->oID()) );
		assert( map->contains(obj2->oID()) );
		assert( map->contains(obj3->oID()) );
		assert( map->contains(obj4->oID()) );
		assert( map->contains(obj5->oID()) );
		ass_false(map->isEmpty());
		ass_eq(map->size(), 5);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		destructorsCalled = 0;
		map->eraseAll();
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 5);

		assert( !map->contains(obj1->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->contains(obj3->oID()) );
		assert( !map->contains(obj4->oID()) );
		assert( !map->contains(obj5->oID()) );

		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		map->insert("meow", obj1);
		map->insert("moo", obj2);
		map->insert("moo2", obj3);

		assert( map->contains(obj1->oID()) && map->contains("meow") && map->contains(obj1) );
		assert( map->contains(obj2->oID()) && map->contains("moo") && map->contains(obj2) );
		assert( map->contains(obj3->oID()) && map->contains("moo2") && map->contains(obj3) );
		assert( !map->contains(obj4->oID()) && !map->contains("meow0") && !map->contains(obj4) );
		assert( !map->contains(obj5->oID()) && !map->contains("mooo") && !map->contains(obj5) );

		destructorsCalled = 0;
		map->eraseAll();
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 3);

		assert( !map->contains(obj1->oID()) && !map->contains("meow") && !map->contains(obj1) );
		assert( !map->contains(obj2->oID()) && !map->contains("moo") && !map->contains(obj2) );
		assert( !map->contains(obj3->oID()) && !map->contains("moo2") && !map->contains(obj3) );
		assert( !map->contains(obj4->oID()) && !map->contains("meow0") && !map->contains(obj4) );
		assert( !map->contains(obj5->oID()) && !map->contains("mooo") && !map->contains(obj5) );

		map->insert(obj4->name(), obj4);
		map->insert(obj5->oID(), obj5);

		assert( !map->contains(obj1->oID()) && !map->contains("meow") && !map->contains(obj1) );
		assert( !map->contains(obj2->oID()) && !map->contains("moo") && !map->contains(obj2) );
		assert( !map->contains(obj3->oID()) && !map->contains("moo2") && !map->contains(obj3) );
		assert( map->contains(obj4->oID()) && map->contains("meow0") && map->contains(obj4) );
		assert( map->contains(obj5->oID()) && map->contains("mooo") && map->contains(obj5) );


		map->eraseAll();
	   ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 5);

		delete map;
			
		FINISH_TEST;
	}

	void testMapBasicErase() {
		BEGIN_TEST;
		
		Map<TestObj*>* map = new Map<TestObj*>(32, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->oID() != obj2->oID() && obj1->oID() != obj3->oID() && obj1->oID() != obj4->oID() && obj1->oID() != obj5->oID() );
		assert( obj2->oID() != obj3->oID() && obj2->oID() != obj4->oID() && obj2->oID() != obj5->oID() );
		assert( obj3->oID() != obj4->oID() && obj3->oID() != obj5->oID() );
		assert( obj4->oID() != obj5->oID() );

		assert( !map->contains(obj1->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->contains(obj3->oID()) );
		assert( !map->contains(obj4->oID()) );
		assert( !map->contains(obj5->oID()) );

		map->insert("meow", obj1);
		map->insert(obj2->name(), obj2);
		map->insert(obj3->oID(), obj3);
		map->insert(obj4->oID(), obj4);
		map->insert(obj5->name(), obj5);

		assert( map->contains(obj1->oID()) && map->contains("meow") );
		assert( map->contains(obj2->oID()) && map->contains("moo") );
		assert( map->contains(obj3->oID()) && map->contains("moo2") );
		assert( map->contains(obj4->oID()) && map->contains("meow0") );
		assert( map->contains(obj5->oID()) && map->contains("mooo") );


		destructorsCalled = 0;
		map->erase("meow");
		ass_false(map->isEmpty());
		ass_eq(map->size(), 4);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		
		ass_eq(destructorsCalled, 1);
		assert( !map->contains("meow") );
		assert( map->contains(obj2->oID()) );
		assert( map->contains(obj3->oID()) );
		assert( map->contains(obj4->oID()) );
		assert( map->contains(obj5->oID()) );

		map->erase("mooo");
		ass_false(map->isEmpty());
		ass_eq(map->size(), 3);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 2);
		assert( !map->contains("meow") );
		assert( map->contains(obj2->oID()) );
		assert( map->contains(obj3->oID()) );
		assert( map->contains(obj4->oID()) );
		assert( !map->contains("mooo") );

		map->erase("moo2");
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 3);
		assert( !map->contains("meow") );
		assert( map->contains(obj2->oID()) );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->oID()) );
		assert( !map->contains("mooo") );

		map->erase("moo");
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 4);
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( map->contains(obj4->oID()) );
		assert( !map->contains("mooo") );


		map->erase("meow0");
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 5);
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );
		
		obj2 = new TestObj("mooAAA");
		map->insert(obj2->name(), obj2);
	   ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains("mooAAA") );
		ass_eq(destructorsCalled, 5);

		map->erase("mooAA");
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( map->contains("mooAAA") );
		ass_eq(destructorsCalled, 5);

		map->erase("mooAAA");
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		assert( !map->contains("mooAAA") );
		ass_eq(destructorsCalled, 6);


		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		map->insert(obj1->name(), obj1);
		map->insert("moo", obj2);
		map->insert(obj3->oID(), obj3);

		assert( map->contains(obj1->oID()) && map->contains("meow") && map->contains(obj1) );
		assert( map->contains(obj2->oID()) && map->contains("moo") && map->contains(obj2) );
		assert( map->contains(obj3->oID()) && map->contains("moo2") && map->contains(obj3) );
		assert( !map->contains(obj4->oID()) && !map->contains("meow0") && !map->contains(obj4) );
		assert( !map->contains(obj5->oID()) && !map->contains("mooo") && !map->contains(obj5) );

		destructorsCalled = 0;
		map->erase(obj1);
		ass_false(map->isEmpty());
		ass_eq(map->size(), 2);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 1);
		assert( !map->contains("meow") );
		assert( map->contains(obj2->oID()) && map->contains(obj2) );
		assert( map->contains(obj3->oID()) && map->contains(obj3) );

		map->erase(obj3->oID());
		ass_false(map->isEmpty());
		ass_eq(map->size(), 1);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 2);
		assert( !map->contains("meow") );
		assert( map->contains(obj2->oID()) && map->contains(obj2) );
		assert( !map->contains("moo2") );

		map->erase(obj2->name());
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 3);
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );

		map->insert(obj4->name(), obj4);

		map->erase(crc32(obj4->name()));
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 4);

		map->insert(obj5->oID(), obj5);
		map->erase(obj5);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 5);
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		map->erase(crc32("moooo"));
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);
		ass_eq(destructorsCalled, 5);
		assert( !map->contains("meow") );
		assert( !map->contains("moo") );
		assert( !map->contains("moo2") );
		assert( !map->contains("meow0") );
		assert( !map->contains("mooo") );

		delete map;
			
		FINISH_TEST;
	}

	void testMapIterator() {
		BEGIN_TEST;

		Map<TestObj*>* map = new Map<TestObj*>(32, NIL);
		ass_true(map->isEmpty());
		ass_eq(map->size(), 0);
		ass_eq(map->capacity(), 32);
		ass_eq(map->loadFactor(), 0.8f);
		ass_eq(map->numBuckets(), 40);
		ass_eq(map->nullValue(), NIL);

		Map<TestObj*>::Iterator itr = map->iterator();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		
		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->oID() != obj2->oID() && obj1->oID() != obj3->oID() && obj1->oID() != obj4->oID() && obj1->oID() != obj5->oID() );
		assert( obj2->oID() != obj3->oID() && obj2->oID() != obj4->oID() && obj2->oID() != obj5->oID() );
		assert( obj3->oID() != obj4->oID() && obj3->oID() != obj5->oID() );
		assert( obj4->oID() != obj5->oID() );

		assert( !map->contains(obj1->oID()) );
		assert( !map->contains(obj2->oID()) );
		assert( !map->contains(obj3->oID()) );
		assert( !map->contains(obj4->oID()) );
		assert( !map->contains(obj5->oID()) );

		map->insert("meow", obj1);
		assert( map->contains(obj1->oID()) && map->contains("meow") );
		itr = map->iterator();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), obj1);
		itr.next();
	   ass_false(itr.isValid());
		ass_false(itr.hasNext());		
		
		map->insert(obj2->name(), obj2);
		assert( map->contains(obj1->oID()) && map->contains("meow") );
		assert( map->contains(obj2->oID()) && map->contains("moo") );
		itr = map->iterator();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val() == obj1 || itr.val() == obj2);
		itr.next();
	   ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val() == obj1 || itr.val() == obj2);
		itr.next();
	   ass_false(itr.isValid());
		ass_false(itr.hasNext());
		
		map->insert(obj3->oID(), obj3);
		map->insert(obj4->oID(), obj4);
		map->insert(obj5->name(), obj5);

		assert( map->contains(obj1->oID()) && map->contains("meow") );
		assert( map->contains(obj2->oID()) && map->contains("moo") );
		assert( map->contains(obj3->oID()) && map->contains("moo2") );
		assert( map->contains(obj4->oID()) && map->contains("meow0") );
		assert( map->contains(obj5->oID()) && map->contains("mooo") );

		TestObj* objs[5];
		for (Size i= 0; i < 5; i++) {
			objs[i] = NIL;
		}		
		itr = map->iterator();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val() == obj1 || itr.val() == obj2 || itr.val() == obj3 || itr.val() == obj4 || itr.val() == obj5);
		ass_true(notIn(itr.val(), objs, 0));		
		objs[0] = itr.val();
		
		itr.next();
	   ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val() == obj1 || itr.val() == obj2 || itr.val() == obj3 || itr.val() == obj4 || itr.val() == obj5);
		ass_true(notIn(itr.val(), objs, 1));
		objs[1] = itr.val();
		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val() == obj1 || itr.val() == obj2 || itr.val() == obj3 || itr.val() == obj4 || itr.val() == obj5);
		ass_true(notIn(itr.val(), objs, 2));
		objs[2] = itr.val();
		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val() == obj1 || itr.val() == obj2 || itr.val() == obj3 || itr.val() == obj4 || itr.val() == obj5);
		ass_true(notIn(itr.val(), objs, 3));
		objs[3] = itr.val();
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val() == obj1 || itr.val() == obj2 || itr.val() == obj3 || itr.val() == obj4 || itr.val() == obj5);
		ass_true(notIn(itr.val(), objs, 4));
		objs[4] = itr.val();
		itr.next();
	   ass_false(itr.isValid());
		ass_false(itr.hasNext());
		

		FINISH_TEST;
	}

	

}

int main(int argc, char** argv) {
	cc::testMapBasicCreateAndDestroy();
	cc::testMapBasicReserveNoObjects();
	cc::testMapBasicInsert();
	cc::testMapBasicReserveWithObjects();
	cc::testMapBasicAutoIncreaseCapacity();
	cc::testMapBasicRemove();
	cc::testMapBasicTake();
	cc::testMapBasicClear();
	cc::testMapBasicEraseAll();
	cc::testMapBasicErase();
	cc::testMapIterator();	
	return 0;
}




