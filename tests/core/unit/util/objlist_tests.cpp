#include <assert.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/util/objlist.h"


#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

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


	void testObjListBasicCreateAndDestroy() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 );

		TestObj* obj = new TestObj("meow");
		assert( !list->contains(obj->getOID()) );

		delete list;
			
		FINISH_TEST;
	}

	void testObjListBasicAppend() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj = new TestObj("meow");
		assert( !list->contains(obj->getOID()) );

		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains("meow") );
		assert( !list->contains("meoww") );

		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );


		delete list;
		delete obj;
			
		FINISH_TEST;
	}

	void testObjListBasicTakeLast() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj = new TestObj("meow");
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		assert( list->takeLast() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		TestObj* obj2 = new TestObj("Moo");

		list->append(obj2);

		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );


		assert( list->takeLast() == obj2 );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		assert( list->takeLast() == obj );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		list->append(obj2);
		list->append(obj);

		assert( list->takeLast() == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );

		assert( list->takeLast() == obj2 );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		assert( !list->takeLast() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		list->append(obj);
		list->append(obj);
		list->append(obj2);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );

		assert( list->takeLast() == obj2 );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		assert( list->takeLast() == obj);
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		assert( list->takeLast() == obj );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );


		list->append(obj);
		list->append(obj2);
		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );

		assert( list->takeLast() == obj);
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );

		assert( list->takeLast() == obj2);
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		assert( list->takeLast() == obj );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );


		delete list;
		delete obj;
		delete obj2;
			
		FINISH_TEST;
	}

	void testObjListBasicTakeFirst() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj = new TestObj("meow");
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		assert( list->takeLast() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		TestObj* obj2 = new TestObj("Moo");

		list->append(obj2);

		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );


		assert( list->takeFirst() == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2);

		assert( list->takeFirst() == obj2 );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		list->append(obj2);
		list->append(obj);

		assert( list->takeFirst() == obj2 );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		assert( list->takeFirst() == obj );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		assert( !list->takeFirst() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		list->append(obj);
		list->append(obj);
		list->append(obj2);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );

		assert( list->takeFirst() == obj);
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );

		assert( list->takeFirst() == obj);
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );

		assert( list->takeFirst() == obj2 );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );


		list->append(obj);
		list->append(obj2);
		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );

		assert( list->takeFirst() == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( list->get(obj2->getOID()) == obj2 );
		assert( list->get("Moo") == obj2 );

		assert( list->takeFirst() == obj2 );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) == obj );
		assert( list->get("meow") == obj );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );

		assert( list->takeFirst() == obj );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get("meow") );
		assert( !list->get(obj2->getOID()) );
		assert( !list->get("Moo") );


		delete list;
		delete obj;
		delete obj2;
			
		FINISH_TEST;
	}


	void testObjListBasicRemove() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj = new TestObj("meow");
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		assert( list->remove(obj) );

		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		TestObj* obj2 = new TestObj("Moo");

		list->append(obj2);

		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		assert( list->remove(obj) );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		assert( list->remove(obj2) );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		list->append(obj2);
		list->append(obj);

		assert( list->remove(obj) );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		assert( list->takeLast() );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		assert( !list->remove(obj) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		list->append(obj2);
		list->append(obj);

		assert( list->takeLast() );
		assert( !list->remove(obj) );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		assert( list->remove(obj2) );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		list->append(obj);
		list->append(obj);
		list->append(obj2);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->remove(obj2) );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));

		assert( list->remove(obj) );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));

		assert( list->remove(obj) );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));


		list->append(obj);
		list->append(obj2);
		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->remove(obj) );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->remove(obj2) );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));

		assert( list->remove(obj) );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));


		/* Test remove by Name */
		list->append(obj);
		list->append(obj2);
		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()) );

		assert( !list->remove("Meow") ); // not 'meow'
		assert( !list->remove("moo") );	// not 'Moo'
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()) );

		assert( list->remove("meow") );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->remove("meow") );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->remove("Moo") );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));


		/* Test remove by OID */

		list->append(obj);
		list->append(obj2);
		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()) );

		assert( !list->remove(crc32("Meow")) ); // not 'meow'
		assert( !list->remove(crc32("moo")) );	// not 'Moo'
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()) );

		assert( list->remove(obj->getOID()) );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->remove(obj->getOID()) );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->remove(obj2->getOID()) );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));

		delete list;
		delete obj;
		delete obj2;
			
		FINISH_TEST;
	}

	void testObjListBasicTake() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj = new TestObj("meow");
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		assert( list->take(obj) == obj );

		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		TestObj* obj2 = new TestObj("Moo");

		list->append(obj2);

		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		assert( list->take(obj) == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		assert( list->take(obj2) == obj2 );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		list->append(obj2);
		list->append(obj);

		assert( list->take(obj) == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		assert( list->takeLast() );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		assert( !list->take(obj) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		list->append(obj2);
		list->append(obj);

		assert( list->takeLast() );
		assert( !list->take(obj) );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		assert( list->take(obj2) == obj2 );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		list->append(obj);
		list->append(obj);
		list->append(obj2);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->take(obj2) == obj2 );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));

		assert( list->take(obj) == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));

		assert( list->take(obj) == obj );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));


		list->append(obj);
		list->append(obj2);
		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->take(obj) == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->take(obj2) == obj2 );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));

		assert( list->take(obj) == obj );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));


		/* Test remove by Name */
		list->append(obj);
		list->append(obj2);
		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()) );

		assert( !list->take("Meow") ); // not 'meow'
		assert( !list->take("moo") );	// not 'Moo'
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()) );

		assert( list->take("meow") == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->take("meow") == obj);
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->take("Moo") == obj2 );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));


		/* Test remove by OID */

		list->append(obj);
		list->append(obj2);
		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()) );

		assert( !list->take(crc32("Meow")) ); // not 'meow'
		assert( !list->take(crc32("moo")) );	// not 'Moo'
		assert( !list->isEmpty() );
		assert( len(list) == 3 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()) );

		assert( list->take(obj->getOID()) == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->take(obj->getOID()) == obj );
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( !list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( list->get(obj2->getOID()));

		assert( list->take(obj2->getOID()) == obj2 );
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->get(obj->getOID()) );
		assert( !list->get(obj2->getOID()));

		delete list;
		delete obj;
		delete obj2;
			
		FINISH_TEST;
	}

	void testObjListBasicClear() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj = new TestObj("meow");
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		list->remove(obj);

		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(obj->getOID()) );

		list->append(obj);

		TestObj* obj2 = new TestObj("Moo");

		list->append(obj2);

		assert( !list->isEmpty() );
		assert( len(list) == 2 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		list->clear();
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		list->append(obj2);
		list->append(obj);

		list->remove(obj2);
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		list->clear();
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		list->append(obj2);
		list->append(obj);

		list->remove(obj2);
		assert( !list->isEmpty() );
		assert( len(list) == 1 ); 
		assert( list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		list->clear();
		assert( list->isEmpty() );
		assert( len(list) == 0 ); 
		assert( !list->contains(obj->getOID()) );
		assert( !list->contains(obj2->getOID()) );

		list->clear();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		list->append(obj);
		list->append(obj2);
		list->append(obj);
		list->append(obj);
		assert( !list->isEmpty() );
		assert( len(list) == 4 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		list->clear();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		delete list;
		delete obj;
		delete obj2;

			
		FINISH_TEST;
	}


	void testObjListIterators() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj = new TestObj("meow");
		assert( !list->contains(obj->getOID()) );
		
		TestObj* obj2 = new TestObj("Moo");

		
		list->append(obj);
		list->append(obj);
		list->append(obj2);
		list->append(obj);
		list->append(obj2);

		assert( !list->isEmpty() );
		assert( len(list) == 5 ); 
		assert( list->contains(obj->getOID()) );
		assert( list->contains(obj2->getOID()) );

		ObjList<TestObj>::Iterator itr = list->getFirst();
		assert( itr.isValid() );
		assert( itr.hasNext() );
		assert( !itr.hasPrev() );
		assert( itr.val() == obj );

		itr.next();
		assert( itr.isValid() && itr.hasNext() && itr.hasPrev() && itr.val() == obj );
		itr.next();
		assert( itr.isValid() && itr.hasNext() && itr.hasPrev() && itr.val() == obj2 );
		itr.next();
		assert( itr.isValid() && itr.hasNext() && itr.hasPrev() && itr.val() == obj );
		itr.next();
		assert( itr.isValid() && !itr.hasNext() && itr.hasPrev() && itr.val() == obj2 );
		itr.next();
		assert( !itr.isValid() && itr.hasNext() && itr.hasPrev() && !itr.val() );

		itr = list->getLast();
		assert( itr.isValid() );
		assert( !itr.hasNext() );
		assert( itr.hasPrev() );
		assert( itr.val() == obj2 );

		itr.prev();
		assert( itr.isValid() && itr.hasNext() && itr.hasPrev() && itr.val() == obj );
		itr.prev();
		assert( itr.isValid() && itr.hasNext() && itr.hasPrev() && itr.val() == obj2 );
		itr.prev();
		assert( itr.isValid() && itr.hasNext() && itr.hasPrev() && itr.val() == obj );
		itr.prev();
		assert( itr.isValid() && itr.hasNext() && !itr.hasPrev() && itr.val() == obj );
		itr.prev();
		assert( !itr.isValid() && itr.hasNext() && itr.hasPrev() && !itr.val() );

		list->clear();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		itr = list->getFirst();
		assert( !itr.isValid() && !itr.hasNext() && !itr.hasPrev() && !itr.val() );
		itr.next();
		assert( !itr.isValid() && !itr.hasNext() && !itr.hasPrev() && !itr.val() );
		itr.prev();
		assert( !itr.isValid() && !itr.hasNext() && !itr.hasPrev() && !itr.val() );

		itr = list->getLast();
		assert( !itr.isValid() && !itr.hasNext() && !itr.hasPrev() && !itr.val() );
		itr.prev();
		assert( !itr.isValid() && !itr.hasNext() && !itr.hasPrev() && !itr.val() );
		itr.next();
		assert( !itr.isValid() && !itr.hasNext() && !itr.hasPrev() && !itr.val() );


		list->append(obj2);
		itr = list->getFirst();
		assert( itr.isValid() && !itr.hasNext() && !itr.hasPrev() && itr.val() );
		itr = list->getLast();
		assert( itr.isValid() && !itr.hasNext() && !itr.hasPrev() && itr.val() );


		list->append(obj);
		itr = list->getFirst();
		assert( itr.isValid() && itr.hasNext() && !itr.hasPrev() && itr.val() );
		itr.next();
		assert( itr.isValid() && !itr.hasNext() && itr.hasPrev() && itr.val() );
		itr.next();
		assert( !itr.isValid() && itr.hasNext() && itr.hasPrev() && !itr.val() );
		itr = list->getLast();
		assert( itr.isValid() && !itr.hasNext() && itr.hasPrev() && itr.val() );
		itr.prev();
		assert( itr.isValid() && itr.hasNext() && !itr.hasPrev() && itr.val() );
		itr.prev();
		assert( !itr.isValid() && itr.hasNext() && itr.hasPrev() && !itr.val() );

		delete list;
		delete obj;
		delete obj2;
			
		FINISH_TEST;
	}

	void testObjListBasicEraseAll() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );



		list->append(obj1);
		list->append(obj2);
		list->append(obj3);
		list->append(obj4);
		list->append(obj5);

		assert( list->contains(obj1->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( list->contains(obj5->getOID()) );


		destructorsCalled = 0;
		list->eraseAll();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );


		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);

		assert( list->contains(obj1->getOID()) && list->contains("meow") && list->contains(obj1) );
		assert( list->contains(obj2->getOID()) && list->contains("moo") && list->contains(obj2) );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") && list->contains(obj3) );
		assert( !list->contains(obj4->getOID()) && !list->contains("meow0") && !list->contains(obj4) );
		assert( !list->contains(obj5->getOID()) && !list->contains("mooo") && !list->contains(obj5) );

		destructorsCalled = 0;
		list->eraseAll();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 3 );

		assert( !list->contains(obj1->getOID()) && !list->contains("meow") && !list->contains(obj1) );
		assert( !list->contains(obj2->getOID()) && !list->contains("moo") && !list->contains(obj2) );
		assert( !list->contains(obj3->getOID()) && !list->contains("moo2") && !list->contains(obj3) );
		assert( !list->contains(obj4->getOID()) && !list->contains("meow0") && !list->contains(obj4) );
		assert( !list->contains(obj5->getOID()) && !list->contains("mooo") && !list->contains(obj5) );

		list->append(obj4);
		list->append(obj5);

		assert( !list->contains(obj1->getOID()) && !list->contains("meow") && !list->contains(obj1) );
		assert( !list->contains(obj2->getOID()) && !list->contains("moo") && !list->contains(obj2) );
		assert( !list->contains(obj3->getOID()) && !list->contains("moo2") && !list->contains(obj3) );
		assert( list->contains(obj4->getOID()) && list->contains("meow0") && list->contains(obj4) );
		assert( list->contains(obj5->getOID()) && list->contains("mooo") && list->contains(obj5) );


		list->eraseAll();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );

		delete list;
			
		FINISH_TEST;
	}

	void testObjListBasicEraseAllWithObjectAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

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

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);
		list->append(obj4);
		list->append(obj5);

		assert( list->contains(obj1->getOID()) && list->get(obj1->getOID()) == obj1 );
		assert( list->contains(obj2->getOID()) && list->get(obj2->getOID()) == obj2 );
		assert( list->contains(obj3->getOID()) && list->get(obj3->getOID()) == obj3 );
		assert( list->contains(obj4->getOID()) && list->get(obj4->getOID()) == obj4 );
		assert( list->contains(obj5->getOID()) && list->get(obj5->getOID()) == obj5 );


		destructorsCalled = 0;
		list->eraseAll(alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );


		obj1 = new (*alloc) TestObj("meow");
		obj2 = new (*alloc) TestObj("moo");
		obj3 = new (*alloc) TestObj("moo2");
		obj4 = new (*alloc) TestObj("meow0");
		obj5 = new (*alloc) TestObj("mooo");

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);

		assert( list->contains(obj1->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );

		destructorsCalled = 0;
		list->eraseAll(alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 3 );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );

		list->append(obj4);
		list->append(obj5);

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( list->contains(obj5->getOID()) );

		list->eraseAll(alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );

		delete list;
		delete alloc;
			
		FINISH_TEST;
	}

	void testObjListBasicErase() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );



		list->append(obj1);
		list->append(obj2);
		list->append(obj3);
		list->append(obj4);
		list->append(obj5);

		assert( list->contains(obj1->getOID()) && list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains("moo") );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") );
		assert( list->contains(obj4->getOID()) && list->contains("meow0") );
		assert( list->contains(obj5->getOID()) && list->contains("mooo") );


		destructorsCalled = 0;
		assert( list->erase("meow") );
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( destructorsCalled == 1 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( list->contains(obj5->getOID()) );

		assert( list->erase("mooo") );
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( destructorsCalled == 2 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );

		assert( list->erase("moo2") );
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( destructorsCalled == 3 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( !list->contains("moo2") );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );

		assert( list->erase("moo") );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( destructorsCalled == 4 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );


		assert( list->erase("meow0") );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );
		
		obj2 = new TestObj("mooAAA");
		list->append(obj2);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		assert( !list->erase("mooAA") );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		assert( list->erase("mooAAA") );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains("mooAAA") );
		assert( destructorsCalled == 6 );


		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);

		assert( list->contains(obj1->getOID()) && list->contains("meow") && list->contains(obj1) );
		assert( list->contains(obj2->getOID()) && list->contains("moo") && list->contains(obj2) );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") && list->contains(obj3) );
		assert( !list->contains(obj4->getOID()) && !list->contains("meow0") && !list->contains(obj4) );
		assert( !list->contains(obj5->getOID()) && !list->contains("mooo") && !list->contains(obj5) );

		destructorsCalled = 0;
		assert( list->erase(obj1) );
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( destructorsCalled == 1 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains(obj2) );
		assert( list->contains(obj3->getOID()) && list->contains(obj3) );

		assert( list->erase(obj3->getOID()) );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( destructorsCalled == 2 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains(obj2) );
		assert( !list->contains("moo2") );

		assert( list->erase(obj2->getName()) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 3 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );

		list->append(obj4);

		assert( list->erase(crc32(obj4->getName())) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 4 );

		list->append(obj5);
		assert( list->erase(obj5) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );

		assert( !list->erase(crc32("moooo")) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );

		delete list;
			
		FINISH_TEST;
	}

	void testObjListBasicEraseWithObjectAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

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

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );



		list->append(obj1);
		list->append(obj2);
		list->append(obj3);
		list->append(obj4);
		list->append(obj5);

		assert( list->contains(obj1->getOID()) && list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains("moo") );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") );
		assert( list->contains(obj4->getOID()) && list->contains("meow0") );
		assert( list->contains(obj5->getOID()) && list->contains("mooo") );


		destructorsCalled = 0;
		assert( list->erase("meow", alloc) );
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( destructorsCalled == 1 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( list->contains(obj5->getOID()) );

		assert( list->erase("mooo", alloc) );
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( destructorsCalled == 2 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );

		assert( list->erase("moo2", alloc) );
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( destructorsCalled == 3 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( !list->contains("moo2") );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );

		assert( list->erase("moo", alloc) );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( destructorsCalled == 4 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );


		assert( list->erase("meow0", alloc) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );
		
		obj2 = new (*alloc) TestObj("mooAAA");
		list->append(obj2);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		assert( !list->erase("mooAA", alloc) );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		assert( list->erase("mooAAA", alloc) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains("mooAAA") );
		assert( destructorsCalled == 6 );


		obj1 = new (*alloc) TestObj("meow");
		obj2 = new (*alloc) TestObj("moo");
		obj3 = new (*alloc) TestObj("moo2");
		obj4 = new (*alloc) TestObj("meow0");
		obj5 = new (*alloc) TestObj("mooo");

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);

		assert( list->contains(obj1->getOID()) && list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains("moo") );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") );
		assert( !list->contains(obj4->getOID()) && !list->contains("meow0") );
		assert( !list->contains(obj5->getOID()) && !list->contains("mooo") );

		destructorsCalled = 0;
		assert( list->erase(obj1, alloc) );
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( destructorsCalled == 1 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );

		assert( list->erase(obj3->getOID(), alloc) );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( destructorsCalled == 2 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( !list->contains("moo2") );

		assert( list->erase(obj2->getName(), alloc) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 3 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );

		list->append(obj4);

		assert( list->erase(crc32(obj4->getName()), alloc) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 4 );

		list->append(obj5);
		assert( list->erase(obj5, alloc) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );

		assert( !list->erase(crc32("moooo"), alloc) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );

		delete list;
		delete alloc;

					
		FINISH_TEST;
	}



	void testObjListSetAndCreateAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;

		PoolMemoryAllocator* alloc = new PoolMemoryAllocator(sizeof(ObjLink<TestObj>), 10, TestObj::getMemoryAlignment());
		
		assert( alloc->getBlockSize() == sizeof(ObjLink<TestObj>));
		assert( alloc->getNumberOfBlocks() == 10 );

		assert( list->setAllocator(alloc) );
		assert( list->getAllocator() == alloc );
		assert( !list->doesOwnAllocator() );

		PoolMemoryAllocator* alloc2 = new PoolMemoryAllocator(sizeof(ObjLink<TestObj>), 12, TestObj::getMemoryAlignment());
		assert( list->setAllocator(alloc2) );
		assert( list->getAllocator() == alloc2 );
		assert( !list->doesOwnAllocator() );

		TestObj* obj = new TestObj("meow");

		list->append(obj);
		assert( !list->setAllocator(alloc) );
		assert( list->getAllocator() == alloc2 );
		assert( !list->doesOwnAllocator() );

		list->clear();
		assert( list->setAllocator(alloc) );
		assert( list->getAllocator() == alloc );
		assert( !list->doesOwnAllocator() );

		assert( list->createAllocator(1) );
		assert( list->getAllocator() != alloc2 && list->getAllocator() != alloc );
		assert( list->doesOwnAllocator() );

		list->append(obj);
		assert( !list->setAllocator(alloc) );
		assert( !list->createAllocator(2) );
		assert( list->getAllocator() != alloc2 && list->getAllocator() != alloc );
		assert( list->doesOwnAllocator() );

		list->clear();
		assert( list->setAllocator(alloc) );
		assert( list->getAllocator() == alloc );
		assert( !list->doesOwnAllocator() );

		delete list;
		delete alloc;
		delete alloc2;
		delete obj;

		FINISH_TEST;
	}

	void testObjListAppendWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );


		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );


		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(crc32(obj1->getName())) );
		assert( !list->contains(crc32(obj2->getName())) );
		assert( !list->contains(crc32(obj3->getName())) );
		assert( !list->contains(crc32(obj4->getName())) );
		assert( !list->contains(crc32(obj5->getName())) );

		list->append(obj1);
		list->append(obj2);
		list->append(obj4);
		list->append(obj5);
		list->append(obj3);

		assert( !list->isEmpty() );
		assert( list->getLength() == 5 && len(list) == 5 );

		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );

		delete list;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}

	void testObjListTakeLastWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );


		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );


		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(crc32(obj1->getName())) );
		assert( !list->contains(crc32(obj2->getName())) );
		assert( !list->contains(crc32(obj3->getName())) );
		assert( !list->contains(crc32(obj4->getName())) );
		assert( !list->contains(crc32(obj5->getName())) );

		list->append(obj1);
		list->append(obj2);
		list->append(obj4);
		list->append(obj5);
		list->append(obj3);

		assert( !list->isEmpty() );
		assert( list->getLength() == 5 && len(list) == 5 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );


		assert( list->takeLast() == obj3 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );

		assert( list->takeLast() == obj5);
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->takeLast() == obj4 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->takeLast() == obj2 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->takeLast() == obj1 );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( !list->takeLast() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		list->append(obj3);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );
		
		assert( list->takeLast() == obj3 );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		delete list;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}

	void testObjListTakeFirstWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );


		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );


		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(crc32(obj1->getName())) );
		assert( !list->contains(crc32(obj2->getName())) );
		assert( !list->contains(crc32(obj3->getName())) );
		assert( !list->contains(crc32(obj4->getName())) );
		assert( !list->contains(crc32(obj5->getName())) );

		list->append(obj1);
		list->append(obj2);
		list->append(obj4);
		list->append(obj5);
		list->append(obj3);

		assert( !list->isEmpty() );
		assert( list->getLength() == 5 && len(list) == 5 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );


		assert( list->takeFirst() == obj1 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );

		assert( list->takeFirst() == obj2);
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );

		assert( list->takeFirst() == obj4 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );

		assert( list->takeFirst() == obj5 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->takeFirst() == obj3 );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		assert( !list->takeLast() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		list->append(obj3);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );
		
		assert( list->takeLast() == obj3 );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		delete list;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}


	void testObjListRemoveWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );


		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );


		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(crc32(obj1->getName())) );
		assert( !list->contains(crc32(obj2->getName())) );
		assert( !list->contains(crc32(obj3->getName())) );
		assert( !list->contains(crc32(obj4->getName())) );
		assert( !list->contains(crc32(obj5->getName())) );

		list->append(obj4);
		list->append(obj3);
		list->append(obj1);
		list->append(obj2);
		list->append(obj5);

		assert( !list->isEmpty() );
		assert( list->getLength() == 5 && len(list) == 5 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );


		list->remove("moo2");
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );

		list->remove("mooo");
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->remove("meow0");
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->remove("moo");
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->remove("meow");
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->remove("meow");
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		list->append(obj3);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );
		
		list->remove("moo2");
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->append(obj3);
		list->append(obj1);
		list->append(obj4);
		list->append(obj5);
		list->append(obj2);

		assert( !list->isEmpty() );
		assert( list->getLength() == 5 && len(list) == 5 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );


		list->remove("moo2");
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );

		list->remove(obj5->getOID());
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->remove(obj4);
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->remove(obj2->getName());
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->remove(crc32(obj1->getName()));
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->remove("meow");
		list->remove(obj1);
		list->remove(obj1->getOID());
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );



		delete list;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}

	void testObjListTakeWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );


		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );


		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(crc32(obj1->getName())) );
		assert( !list->contains(crc32(obj2->getName())) );
		assert( !list->contains(crc32(obj3->getName())) );
		assert( !list->contains(crc32(obj4->getName())) );
		assert( !list->contains(crc32(obj5->getName())) );

		list->append(obj4);
		list->append(obj3);
		list->append(obj1);
		list->append(obj2);
		list->append(obj5);

		assert( !list->isEmpty() );
		assert( list->getLength() == 5 && len(list) == 5 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );


		assert( list->take("moo2") == obj3 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );

		assert( list->take("mooo") == obj5 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->take("meow0") == obj4 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->take("moo") == obj2 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->take("meow") == obj1 );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( !list->take("meow") );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		list->append(obj3);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );
		
		assert( list->take("moo2") == obj3 );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->append(obj3);
		list->append(obj1);
		list->append(obj4);
		list->append(obj5);
		list->append(obj2);

		assert( !list->isEmpty() );
		assert( list->getLength() == 5 && len(list) == 5 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );


		assert( list->take("moo2") == obj3 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );

		assert( list->take(obj5->getOID()) == obj5 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2"));
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->take(obj4) == obj4 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->take(obj2->getName()) == obj2 );
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( list->take(crc32(obj1->getName())) == obj1 );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		assert( !list->take("meow") );
		assert( !list->take(obj1) );
		assert( !list->take(obj1->getOID()) );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );



		delete list;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}


	void testObjListClearWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );


		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );


		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(crc32(obj1->getName())) );
		assert( !list->contains(crc32(obj2->getName())) );
		assert( !list->contains(crc32(obj3->getName())) );
		assert( !list->contains(crc32(obj4->getName())) );
		assert( !list->contains(crc32(obj5->getName())) );

		list->append(obj1);
		list->append(obj2);
		list->append(obj4);
		list->append(obj5);
		list->append(obj3);

		assert( !list->isEmpty() );
		assert( list->getLength() == 5 && len(list) == 5 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );


		list->clear();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );

		list->clear();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		list->append(obj3);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );
		
		list->clear();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		list->append(obj1);
		list->append(obj2);
		list->append(obj4);
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( list->contains(crc32(obj1->getName())) && list->contains("meow") && list->get(crc32(obj1->getName())) == obj1 && list->get("meow") == obj1 );
		assert( list->contains(crc32(obj2->getName())) && list->contains("moo") && list->get(crc32(obj2->getName())) == obj2 && list->get("moo") == obj2 );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( list->contains(crc32(obj4->getName())) && list->contains("meow0") && list->get(crc32(obj4->getName())) == obj4 && list->get("meow0") == obj4 );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		list->clear();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		list->append(obj5);
		list->append(obj3);
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( list->contains(crc32(obj3->getName())) && list->contains("moo2") && list->get(crc32(obj3->getName())) == obj3 && list->get("moo2") == obj3 );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( list->contains(crc32(obj5->getName())) && list->contains("mooo") && list->get(crc32(obj5->getName())) == obj5 && list->get("mooo") == obj5 );


		list->clear();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains(crc32(obj1->getName())) && !list->contains("meow") && !list->get(crc32(obj1->getName())) && !list->get("meow") );
		assert( !list->contains(crc32(obj2->getName())) && !list->contains("moo") && !list->get(crc32(obj2->getName())) && !list->get("moo") );
		assert( !list->contains(crc32(obj3->getName())) && !list->contains("moo2") && !list->get(crc32(obj3->getName())) && !list->get("moo2") );
		assert( !list->contains(crc32(obj4->getName())) && !list->contains("meow0") && !list->get(crc32(obj4->getName())) && !list->get("meow0") );
		assert( !list->contains(crc32(obj5->getName())) && !list->contains("mooo") && !list->get(crc32(obj5->getName())) && !list->get("mooo") );


		delete list;
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
		delete obj5;
			
		FINISH_TEST;
	}

	void testObjListEraseAllWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );


		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);
		list->append(obj4);
		list->append(obj5);

		assert( list->contains(obj1->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( list->contains(obj5->getOID()) );


		destructorsCalled = 0;
		list->eraseAll();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );


		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);

		assert( list->contains(obj1->getOID()) && list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains("moo") );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") );
		assert( !list->contains(obj4->getOID()) && !list->contains("meow0") );
		assert( !list->contains(obj5->getOID()) && !list->contains("mooo") );

		destructorsCalled = 0;
		list->eraseAll();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 3 );

		assert( !list->contains(obj1->getOID()) && !list->contains("meow") );
		assert( !list->contains(obj2->getOID()) && !list->contains("moo") );
		assert( !list->contains(obj3->getOID()) && !list->contains("moo2") );
		assert( !list->contains(obj4->getOID()) && !list->contains("meow0") );
		assert( !list->contains(obj5->getOID()) && !list->contains("mooo") );

		list->append(obj4);
		list->append(obj5);

		assert( !list->contains(obj1->getOID()) && !list->contains("meow") );
		assert( !list->contains(obj2->getOID()) && !list->contains("moo") );
		assert( !list->contains(obj3->getOID()) && !list->contains("moo2") );
		assert( list->contains(obj4->getOID()) && list->contains("meow0") );
		assert( list->contains(obj5->getOID()) && list->contains("mooo") );


		list->eraseAll();
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );

		delete list;
			
		FINISH_TEST;
	}

	void testObjListEraseAllWithNodeAndObjectAllocators() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );


		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

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

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);
		list->append(obj4);
		list->append(obj5);

		assert( list->contains(obj1->getOID()) && list->get(obj1->getOID()) == obj1 );
		assert( list->contains(obj2->getOID()) && list->get(obj2->getOID()) == obj2 );
		assert( list->contains(obj3->getOID()) && list->get(obj3->getOID()) == obj3 );
		assert( list->contains(obj4->getOID()) && list->get(obj4->getOID()) == obj4 );
		assert( list->contains(obj5->getOID()) && list->get(obj5->getOID()) == obj5 );


		destructorsCalled = 0;
		list->eraseAll(alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );


		obj1 = new (*alloc) TestObj("meow");
		obj2 = new (*alloc) TestObj("moo");
		obj3 = new (*alloc) TestObj("moo2");
		obj4 = new (*alloc) TestObj("meow0");
		obj5 = new (*alloc) TestObj("mooo");

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);

		assert( list->contains(obj1->getOID()) );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );

		destructorsCalled = 0;
		list->eraseAll(alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 3 );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );

		list->append(obj4);
		list->append(obj5);

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( list->contains(obj5->getOID()) );

		list->eraseAll(alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );

		delete list;
		delete alloc;
			
		FINISH_TEST;
	}

	void testObjListEraseWithNodeAllocator() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );

		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

		TestObj* obj1 = new TestObj("meow");
		TestObj* obj2 = new TestObj("moo");
		TestObj* obj3 = new TestObj("moo2");
		TestObj* obj4 = new TestObj("meow0");
		TestObj* obj5 = new TestObj("mooo");

		assert( obj1->getOID() != obj2->getOID() && obj1->getOID() != obj3->getOID() && obj1->getOID() != obj4->getOID() && obj1->getOID() != obj5->getOID() );
		assert( obj2->getOID() != obj3->getOID() && obj2->getOID() != obj4->getOID() && obj2->getOID() != obj5->getOID() );
		assert( obj3->getOID() != obj4->getOID() && obj3->getOID() != obj5->getOID() );
		assert( obj4->getOID() != obj5->getOID() );

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );



		list->append(obj1);
		list->append(obj2);
		list->append(obj3);
		list->append(obj4);
		list->append(obj5);

		assert( list->contains(obj1->getOID()) && list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains("moo") );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") );
		assert( list->contains(obj4->getOID()) && list->contains("meow0") );
		assert( list->contains(obj5->getOID()) && list->contains("mooo") );


		destructorsCalled = 0;
		list->erase("meow");
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( destructorsCalled == 1 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( list->contains(obj5->getOID()) );

		list->erase("mooo");
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( destructorsCalled == 2 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );

		list->erase("moo2");
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( destructorsCalled == 3 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( !list->contains("moo2") );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );

		list->erase("moo");
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( destructorsCalled == 4 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );


		list->erase("meow0");
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );
		
		obj2 = new TestObj("mooAAA");
		list->append(obj2);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		list->erase("mooAA");
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		list->erase("mooAAA");
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains("mooAAA") );
		assert( destructorsCalled == 6 );


		obj1 = new TestObj("meow");
		obj2 = new TestObj("moo");
		obj3 = new TestObj("moo2");
		obj4 = new TestObj("meow0");
		obj5 = new TestObj("mooo");

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);

		assert( list->contains(obj1->getOID()) && list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains("moo") );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") );
		assert( !list->contains(obj4->getOID()) && !list->contains("meow0") );
		assert( !list->contains(obj5->getOID()) && !list->contains("mooo") );

		destructorsCalled = 0;
		list->erase(obj1);
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( destructorsCalled == 1 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );

		list->erase(obj3->getOID());
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( destructorsCalled == 2 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( !list->contains("moo2") );

		list->erase(obj2->getName());
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 3 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );

		list->append(obj4);

		list->erase(crc32(obj4->getName()));
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 4 );

		list->append(obj5);
		list->erase(obj5);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );

		list->erase(crc32("moooo"));
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );

		delete list;
			
		FINISH_TEST;
	}

	void testObjListEraseWithNodeAndObjectAllocators() {
		BEGIN_TEST;
		
		ObjList<TestObj>* list = new ObjList<TestObj>;
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->getAllocator() );
		assert( !list->doesOwnAllocator() );

		assert( list->createAllocator(5) );
		assert( list->getAllocator() );
		assert( list->doesOwnAllocator() );
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );

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

		assert( !list->contains(obj1->getOID()) );
		assert( !list->contains(obj2->getOID()) );
		assert( !list->contains(obj3->getOID()) );
		assert( !list->contains(obj4->getOID()) );
		assert( !list->contains(obj5->getOID()) );

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);
		list->append(obj4);
		list->append(obj5);

		assert( list->contains(obj1->getOID()) && list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains("moo") );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") );
		assert( list->contains(obj4->getOID()) && list->contains("meow0") );
		assert( list->contains(obj5->getOID()) && list->contains("mooo") );

		destructorsCalled = 0;
		list->erase("meow", alloc);
		assert( !list->isEmpty() );
		assert( list->getLength() == 4 && len(list) == 4 );
		assert( destructorsCalled == 1 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( list->contains(obj5->getOID()) );

		list->erase("mooo", alloc);
		assert( !list->isEmpty() );
		assert( list->getLength() == 3 && len(list) == 3 );
		assert( destructorsCalled == 2 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );

		list->erase("moo2", alloc);
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( destructorsCalled == 3 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( !list->contains("moo2") );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );

		list->erase("moo", alloc);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( destructorsCalled == 4 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( list->contains(obj4->getOID()) );
		assert( !list->contains("mooo") );


		list->erase("meow0", alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );
		
		obj2 = new (*alloc) TestObj("mooAAA");
		list->append(obj2);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		list->erase("mooAA", alloc);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( list->contains("mooAAA") );
		assert( destructorsCalled == 5 );

		list->erase("mooAAA", alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( !list->contains("mooAAA") );
		assert( destructorsCalled == 6 );

		obj1 = new (*alloc) TestObj("meow");
		obj2 = new (*alloc) TestObj("moo");
		obj3 = new (*alloc) TestObj("moo2");
		obj4 = new (*alloc) TestObj("meow0");
		obj5 = new (*alloc) TestObj("mooo");

		list->append(obj1);
		list->append(obj2);
		list->append(obj3);

		assert( list->contains(obj1->getOID()) && list->contains("meow") );
		assert( list->contains(obj2->getOID()) && list->contains("moo") );
		assert( list->contains(obj3->getOID()) && list->contains("moo2") );
		assert( !list->contains(obj4->getOID()) && !list->contains("meow0") );
		assert( !list->contains(obj5->getOID()) && !list->contains("mooo") );

		destructorsCalled = 0;
		list->erase(obj1, alloc);
		assert( !list->isEmpty() );
		assert( list->getLength() == 2 && len(list) == 2 );
		assert( destructorsCalled == 1 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( list->contains(obj3->getOID()) );

		list->erase(obj3->getOID(), alloc);
		assert( !list->isEmpty() );
		assert( list->getLength() == 1 && len(list) == 1 );
		assert( destructorsCalled == 2 );
		assert( !list->contains("meow") );
		assert( list->contains(obj2->getOID()) );
		assert( !list->contains("moo2") );

		list->erase(obj2->getName(), alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 3 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );

		list->append(obj4);

		list->erase(crc32(obj4->getName()), alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 4 );

		list->append(obj5);
		list->erase(obj5, alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );

		list->erase(crc32("moooo"), alloc);
		assert( list->isEmpty() );
		assert( list->getLength() == 0 && len(list) == 0 );
		assert( destructorsCalled == 5 );
		assert( !list->contains("meow") );
		assert( !list->contains("moo") );
		assert( !list->contains("moo2") );
		assert( !list->contains("meow0") );
		assert( !list->contains("mooo") );

		delete list;
		delete alloc;

					
		FINISH_TEST;
	}
}

int main(int argc, char** argv) {
	cc::testObjListBasicCreateAndDestroy();
	cc::testObjListBasicAppend();
	cc::testObjListBasicTakeLast();
	cc::testObjListBasicTakeFirst();
	cc::testObjListBasicRemove();
	cc::testObjListBasicTake();
	cc::testObjListBasicClear();
	cc::testObjListIterators();
	cc::testObjListBasicEraseAll();
	cc::testObjListBasicEraseAllWithObjectAllocator();
	cc::testObjListBasicErase();
	cc::testObjListBasicEraseWithObjectAllocator();
	cc::testObjListSetAndCreateAllocator();
	cc::testObjListAppendWithNodeAllocator();
	cc::testObjListTakeLastWithNodeAllocator();
	cc::testObjListTakeFirstWithNodeAllocator();
	cc::testObjListRemoveWithNodeAllocator();
	cc::testObjListTakeWithNodeAllocator();
	cc::testObjListClearWithNodeAllocator();
	cc::testObjListEraseAllWithNodeAllocator();
	cc::testObjListEraseAllWithNodeAndObjectAllocators();
	cc::testObjListEraseWithNodeAllocator();
	cc::testObjListEraseWithNodeAndObjectAllocators();
	return 0;
}


