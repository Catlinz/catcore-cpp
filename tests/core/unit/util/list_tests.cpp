#include "core/testcore.h"
#include "core/util/list.h"

namespace cc {

	class TestObject {
		public:
			TestObject() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
			TestObject(F32 x, F32 y, F32 z) : m_x(x), m_y(y), m_z(z) {}

			Boolean test(F32 x, F32 y, F32 z) const {
				return m_x == x && m_y == y && m_z == z;
			}

			bool operator!=(const TestObject& obj) const {
				return (m_x != obj.m_x || m_y != obj.m_y || m_z != obj.m_z);
			}	

			bool operator==(const TestObject& obj) const {
				return (m_x == obj.m_x && m_y == obj.m_y && m_z == obj.m_z);
			}	

			F32 m_x, m_y, m_z;

	};

	I32 refTest(List<I32>& list, Size idx, I32 add) {
		list.at(idx) += add;
		return list.at(idx);
	}

	List<I32> refTest2(List<I32>& list, I32 value) {
		list.append(value);
		return list;
	}


	void testListCreateAndDestroy() {
		BEGIN_TEST;

		List<I32> il(0);
		assert( il.length() == 0 );
		assert( il.getNullValue() == 0 );
		
		il = List<I32>();
		assert( il.length() == 0 );

		List<TestObject> ol;
		assert( ol.length() == 0 );
		
		List<TestObject*> olp(NIL);
		assert( olp.length() == 0 );
		assert( olp.getNullValue() == NIL );

		FINISH_TEST;
	}

	void testListAppend() {
		BEGIN_TEST;

		List<I32> il;
		assert( il.length() == 0 && il.isEmpty());
		il.append(1);
		assert( !il.isEmpty() );		
		assert( il.length() == 1  && il.at(0) == 1 && il.get(1) == 1 && il.first() == 1 && il.last() == 1);
		il.append(2);
		assert( il.length() == 2  && il.at(1) == 2 && il.get(2) == 2 && il.first() == 1 && il.last() == 2);
		il.append(69);
		il.append(30);
		assert( il.length() == 4  && il.at(2) == 69 && il.at(3) == 30 && il.first() == 1 && il.last() == 30);
		assert(il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
		
		il = List<I32>(0);
		assert( il.getNullValue() == 0 && il.isEmpty());
		assert( il.length() == 0 );
		assert( il.first() == 0 && il.last() == 0 && il.get(2) == 0 && il.get(0) == 0 );
		il.append(1);
		assert( !il.isEmpty() );		
		assert( il.length() == 1 && il.first() == 1 && il.last() == 1 && il.get(1) == 1);
		il.append(2);
		assert( il.length() == 2 && il.first() == 1 && il.last() == 2 && il.get(2) == 2);
		il.append(69);
		il.append(30);
		assert( il.length() == 4 && il.get(-1) == 0 );
		assert(il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );

		List<TestObject> ol;
		TestObject obj;
		assert( ol.length() == 0 && ol.isEmpty() );
		ol.append(obj);
		assert( ol.length() == 1 && !ol.isEmpty() );
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		assert( ol.length() == 2 );
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.length() == 4 );
		assert( ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );

		
		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL && olp.get(NIL) == NIL && olp.isEmpty() );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 && olp.get(ptr1) == ptr1 && !olp.isEmpty() );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 && olp.get(ptr2) == ptr2 && olp.get(ptr1) == ptr1);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr3);
		assert( olp.length() == 4 && olp.get(ptr3) == ptr3);
		assert( olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );

		delete ptr1;
		delete ptr2;
		delete ptr3;

		FINISH_TEST;
	}

	void testListPrepend() {
		BEGIN_TEST;

		List<I32> il;
		assert( il.length() == 0 );
		il.prepend(1);
		assert( il.length() == 1  && il.at(0) == 1 && il.first() == 1 && il.last() == 1);
		il.prepend(2);
		assert( il.length() == 2  && il.at(0) == 2 && il.first() == 2 && il.last() == 1);
		il.prepend(69);
		il.prepend(30);
		assert( il.length() == 4  && il.at(1) == 69 && il.at(0) == 30 && il.first() == 30 && il.last() == 1);
		assert(il.at(3) == 1 && il.at(2) == 2 && il.at(0) == 30 && il.at(1) == 69 );
		
		il = List<I32>(0);
		assert( il.getNullValue() == 0 );
		assert( il.length() == 0 );
		assert( il.first() == 0 && il.last() == 0 );		
		il.prepend(1);
		assert( il.length() == 1 && il.first() == 1 && il.last() == 1);
		il.prepend(2);
		assert( il.length() == 2 && il.first() == 2 && il.last() == 1);
		il.prepend(69);
		il.prepend(30);
		assert( il.length() == 4 && il.first() == 30 && il.last() == 1);
		assert(il.at(3) == 1 && il.at(2) == 2 && il.at(0) == 30 && il.at(1) == 69 );

		List<TestObject> ol;
		TestObject obj;
		assert( ol.length() == 0 );
		ol.prepend(obj);
		assert( ol.length() == 1 );
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.prepend(obj);
		assert( ol.length() == 2 );
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.prepend(obj);
		ol.prepend(obj);
		assert( ol.length() == 4 );
		assert( ol.at(3).test(0.0f, 0.0f, 0.0f) && ol.at(2).test(1.0f, 2.0f, 3.0f) && ol.at(1).test(20.3f, 69.9f, 69.0f) && ol.at(0).test(20.3f, 69.9f, 69.0f) );

		
		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.prepend(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.prepend(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.prepend(ptr3);
		olp.prepend(ptr3);
		assert( olp.length() == 4 );
		assert( olp.at(3)->test(0.0f, 0.0f, 0.0f) && olp.at(2)->test(1.0f, 2.0f, 3.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) && olp.at(0)->test(20.3f, 69.9f, 69.0f) );

		delete ptr1;
		delete ptr2;
		delete ptr3;

		FINISH_TEST;
	}


	void testListAtAndLast() {
		BEGIN_TEST;

		List<I32> il;
		assert( il.length() == 0 );
		il.append(1); il.append(2); il.append(69); il.append(30);
		assert( il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
		assert( il.last() == 30 );
		
		/* Test modifying through at() */
		il.at(0) = 4;
		assert( il.at(0) == 4 );
		il.at(1) = il.at(1) * 3;
		assert( il.at(1) == 6 );
		il.at(2) = il.at(1) * il.at(0);
		assert( il.at(0) == 4 && il.at(1) == 6 && il.at(2) == 24 && il.at(3) == 30);
		il.at(3)--;
		assert( il.at(3) == 29 );
		assert( il.length() == 4 );
		assert( il.last() == 29 );

		const I32 ci = il.at(3);
		assert( ci == 29 );

		
		List<TestObject> ol;
		TestObject obj;
		assert( ol.length() == 0 );
		ol.append(obj);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );
		assert( ol.last().test(20.3f, 69.9f, 69.0f) );

		/* Test modifying through at() */
		ol.at(0).m_x = 22.3f;
		ol.at(0).m_z = -30.11f;
		assert( ol.at(0).test(22.3f, 0.0f, -30.11f) );
		obj = TestObject(-1.0f, 23.0f, -3.1f);
		ol.at(1) = obj;
		assert( ol.at(1).test(-1.0f, 23.0f, -3.1f) );
		ol.at(2) = ol.at(0);
		ol.at(2).m_x = -12.2f;
		assert( ol.at(0).test(22.3f, 0.0f, -30.11f) && ol.at(2).test(-12.2f, 0.0f, -30.11f) );
		assert( ol.at(3).test(20.3f, 69.9f, 69.0f) );
		assert( ol.length() == 4 );
		assert( ol.last().test(20.3f, 69.9f, 69.0f) );

		
		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3); olp.append(ptr3);

		/* Test modifying through at() */
		olp.at(0)->m_x = 22.3f;
		olp.at(0)->m_z = -30.11f;
		assert( olp.at(0)->test(22.3f, 0.0f, -30.11f) );
		TestObject* ptr4 = new TestObject(-1.0f, 23.0f, -3.1f);
		olp.at(1) = ptr4;
		assert( olp.at(1)->test(-1.0f, 23.0f, -3.1f) );
		olp.at(2) = olp.at(0);
		olp.at(2)->m_x = -12.2f;
		assert( olp.at(0)->test(-12.2f, 0.0f, -30.11f) && olp.at(2)->test(-12.2f, 0.0f, -30.11f) );
		assert( olp.at(3)->test(20.3f, 69.9f, 69.0f) );
		assert( olp.length() == 4 );
		assert( olp.last()->test(20.3f, 69.9f, 69.0f) );
		assert(ptr1->test(-12.2f, 0.0f, -30.11f)  && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) && ptr4->test(-1.0f, 23.0f, -3.1f) );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testListClear() {
		BEGIN_TEST;

		List<I32> il(9999);
		assert( il.length() == 0 && il.isEmpty() );
		assert( il.last() == 9999 && il.first() == 9999);		
		il.append(1);
		assert( il.length() == 1 && !il.isEmpty() && il.at(0) == 1 && il.first() == 1 && il.last() == 1);
		il.append(2);
		assert( il.length() == 2  && il.at(1) == 2 && il.first() == 1 && il.last() == 2);
		il.append(69);
		il.append(30);
		assert( il.length() == 4  && il.at(2) == 69 && il.at(3) == 30 && il.first() == 1 && il.last() == 30);
		assert(il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
		il.clear();
		assert( il.length() == 0 && il.isEmpty() );
		assert( il.last() == 9999 && il.first() == 9999 );		
		il.append(34);
		il.append(22);
		assert( il.length() == 2 && !il.isEmpty() && il.at(0) == 34 && il.at(1) == 22 && il.first() == 34 && il.last() == 22);
		il.clear();
		assert( il.length() == 0 && il.first() == 9999 && il.last() == 9999 );
		
		il = List<I32>(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1 );
		il.append(2);
		assert( il.length() == 2 );
		il.append(69);
		il.append(30);
		assert( il.length() == 4 );
		assert(il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
		il.clear();
		assert( il.length() == 0  );
		il.append(32);
		il.append(45);
		assert( il.length() == 2  && il.at(0) == 32 && il.at(1) == 45 );
		il.clear();
		assert( il.length() == 0  );


		TestObject obj = TestObject(9999.0f, 9999.0f, 9999.0f);
		List<TestObject> ol(obj);
		assert( ol.getNullValue().test(9999.0f, 9999.0f, 9999.0f) );
		obj = TestObject(0.0f, 0.0f, 0.0f);
		assert( ol.length() == 0 );
		ol.append(obj);
		assert( ol.length() == 1 );
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		assert( ol.length() == 2 );
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.length() == 4 );
		assert( ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );
		ol.clear();
		assert( ol.length() == 0  );
		obj = TestObject(1.0f, 4.0f, 9.0f);
		ol.append(obj);
		assert( ol.length() == 1  && ol.at(0).test(1.0f, 4.0f, 9.0f) );
		ol.clear();
		assert( ol.length() == 0  );
		
		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr3);
		assert( olp.length() == 4 );
		assert( olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );
		olp.clear();
		assert( olp.length() == 0  );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr4);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		olp.clear();
		assert( olp.length() == 0  );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testListContains() {
		BEGIN_TEST;

		List<I32> il(9999);
		assert( il.length() == 0 );
		il.append(1); il.append(2); il.append(69); il.append(30);
		assert( il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
		assert( il.contains(1) && il.contains(2) && il.contains(30) && il.contains(69) && !il.contains(68) ); 
		
		List<TestObject> ol(TestObject(9999.0f, 9999.0f, 9999.0f));
		TestObject obj;
		assert( ol.length() == 0 );
		ol.append(obj);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );
		assert( ol.contains(TestObject(0.0f, 0.0f, 0.0f)) );
		assert( ol.contains(TestObject(1.0f, 2.0f, 3.0f)) );
		assert( ol.contains(TestObject(20.3f, 69.9f, 69.0f)) );
		assert( !ol.contains(TestObject(9.0f, 2.0f, 3.0f)) );
		
		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3); olp.append(ptr3);

		assert( olp.contains(ptr1) && olp.contains(ptr2) && olp.contains(ptr3) && !olp.contains(0) && !olp.contains((TestObject*)2323) );

		delete ptr1;
		delete ptr2;
		delete ptr3;

		FINISH_TEST;
	}

	void testListRemove() {
		BEGIN_TEST;

		List<I32> il(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1  && il.at(0) == 1);
		il.append(2);
		assert( il.length() == 2  && il.at(1) == 2);
		il.append(69);
		il.append(30);
		assert( il.length() == 4  && il.at(2) == 69 && il.at(3) == 30);
		assert( il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
	
		assert( il.remove(2) );
		assert( il.length() == 3 && il.at(0) == 1 && il.at(2) == 30 && il.at(1) == 69 );
		il.remove(30);
		assert( il.length() == 2 && il.at(0) == 1 && il.at(1) == 69 );
		il.remove(1);
		assert( il.length() == 1 && il.at(0) == 69 );
		il.remove(69);
		assert( il.length() == 0 );
		il.append(34);
		il.append(22);
		assert( il.length() == 2  && il.at(0) == 34 && il.at(1) == 22 );
		il.remove(34);
		il.remove(22);
		assert( il.length() == 0  );
		assert( !il.remove(22) );
		assert( !il.remove(9999) );
		
		il = List<I32>(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1 );
		il.append(2);
		assert( il.length() == 2 );
		il.append(69);
		il.append(30);
		assert(il.length() == 4 && il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
		il.remove(1);
		assert(il.length() == 3 && il.at(0) == 2 && il.at(2) == 30 && il.at(1) == 69 );
		il.removeLast();
		assert(il.length() == 2 && il.at(0) == 2 && il.at(1) == 69 );
		il.remove(69);
		assert(il.length() == 1 && il.at(0) == 2 );
		assert( !il.remove(69) && !il.remove(1) && !il.remove(-2) );
		assert(il.length() == 1 && il.at(0) == 2 );
		il.remove(2);
		assert( il.length() == 0  );
		il.append(98);
		il.append(102);
		assert( il.length() == 2  && il.at(0) == 98 && il.at(1) == 102 );
		assert( il.remove(il.at(0)) );
		assert( il.length() == 1 && il.at(0) == 102 );
		assert( il.remove(il.get(102)) );
		assert( il.length() == 0  );
		

		TestObject obj = TestObject(9999.0f, 9999.0f, 9999.0f);
		List<TestObject> ol(obj);
		assert( ol.getNullValue().test(9999.0f, 9999.0f, 9999.0f) );
		obj = TestObject(0.0f, 0.0f, 0.0f);
		assert( ol.length() == 0 );
		ol.append(obj);
		assert( ol.length() == 1 );
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		assert( ol.length() == 2 );
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.length() == 4 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );
		assert( ol.remove(TestObject(20.3f, 69.9f, 69.0f)) );
		assert( ol.length() == 3 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) );
		ol.remove(TestObject(0.0f, 0.0f, 0.0f));
		assert( ol.length() == 2 && ol.at(0).test(1.0f, 2.0f, 3.0f) && ol.at(1).test(20.3f, 69.9f, 69.0f) );
		ol.remove(TestObject(20.3f, 69.9f, 69.0f));
		assert( ol.length() == 1 && ol.at(0).test(1.0f, 2.0f, 3.0f) );
		assert( !ol.remove(TestObject(20.3f, 69.9f, 69.0f)) && !ol.remove(TestObject(0.0f, 0.0f, 0.0f)) && !ol.remove(TestObject(9999.0f, 9999.0f, 9999.0f)) );
		ol.remove(TestObject(1.0f, 2.0f, 3.0f));
		assert( ol.length() == 0  );
		//obj = TestObject(1.0f, 4.0f, 9.0f);
		ol.append(TestObject(1.0f, 4.0f, 9.0f));
		ol.append(TestObject(2.0f, 4.0f, 9.0f));
		assert( ol.length() == 2 && ol.at(0).test(1.0f, 4.0f, 9.0f) && ol.at(1).test(2.0f, 4.0f, 9.0f) );
		assert( ol.remove(ol.at(1)) );
		assert( ol.length() == 1 && ol.at(0).test(1.0f, 4.0f, 9.0f) );
		assert( ol.remove(ol.at(0)) );
		assert( ol.length() == 0  );
		
		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr3);
		assert( olp.length() == 4 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );	
		assert( olp.remove(ptr2) );
		assert( olp.length() == 3 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) );	
		olp.remove(ptr1);
		assert( olp.length() == 2 && olp.at(0)->test(20.3f, 69.9f, 69.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) );
		olp.remove(ptr3);
		assert( olp.length() == 1 && olp.at(0)->test(20.3f, 69.9f, 69.0f) );
		assert( !olp.remove(ptr1) && !olp.remove(ptr2) && !olp.remove(NIL) );
		assert( olp.length() == 1 && olp.at(0)->test(20.3f, 69.9f, 69.0f) );
		olp.remove(ptr3);		
		assert( olp.length() == 0  );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr4);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		assert( olp.remove(olp.at(0)) );
		assert( olp.length() == 0  );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testListRemoveFirst() {
		BEGIN_TEST;

		List<I32> il(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1  && il.at(0) == 1);
		il.append(2);
		assert( il.length() == 2  && il.at(1) == 2);
		il.append(69);
		il.append(30);
		assert( il.length() == 4  && il.at(2) == 69 && il.at(3) == 30);
		assert( il.length() == 4 && il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
	
		il.removeFirst();
		assert( il.length() == 3 && il.at(0) == 2 && il.at(2) == 30 && il.at(1) == 69 );
		il.removeFirst();
		assert( il.length() == 2 && il.at(1) == 30 && il.at(0) == 69 );
		il.removeFirst();
		assert( il.length() == 1 && il.at(0) == 30 );
		il.removeFirst();
		assert( il.length() == 0  );
		il.append(34);
		il.append(22);
		assert( il.length() == 2  && il.at(0) == 34 && il.at(1) == 22 );
		il.removeFirst();
		il.removeFirst();
		assert( il.length() == 0  );
		
		il = List<I32>(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1 );
		il.append(2);
		assert( il.length() == 2 );
		il.append(69);
		il.append(30);
		assert( il.length() == 4 && il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
		il.removeFirst();
		assert( il.length() == 3 && il.at(0) == 2 && il.at(2) == 30 && il.at(1) == 69 );
		il.removeFirst();
		assert( il.length() == 2 && il.at(1) == 30 && il.at(0) == 69 );
		il.removeFirst();
		assert( il.length() == 1 && il.at(0) == 30 );
		il.removeFirst();
		assert( il.length() == 0  );
		il.append(98);
		il.append(102);
		assert( il.length() == 2  && il.at(0) == 98 && il.at(1) == 102 );
		il.removeFirst();
		il.removeFirst();
		assert( il.length() == 0  );
		
		TestObject obj = TestObject(9999.0f, 9999.0f, 9999.0f);
		List<TestObject> ol(obj);
		assert( ol.getNullValue().test(9999.0f, 9999.0f, 9999.0f) );
		obj = TestObject(0.0f, 0.0f, 0.0f);
		assert( ol.length() == 0 );
		ol.append(obj);
		assert( ol.length() == 1 );
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		assert( ol.length() == 2 );
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.length() == 4 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );

		ol.removeFirst();
		assert( ol.length() == 3 && ol.at(0).test(1.0f, 2.0f, 3.0f) && ol.at(1).test(20.3f, 69.9f, 69.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) );
		ol.removeFirst();
		assert( ol.length() == 2 && ol.at(0).test(20.3f, 69.9f, 69.0f) && ol.at(1).test(20.3f, 69.9f, 69.0f) );
		ol.removeFirst();
		assert( ol.length() == 1 && ol.at(0).test(20.3f, 69.9f, 69.0f) );
		ol.removeFirst();
		assert( ol.length() == 0  );
		obj = TestObject(1.0f, 4.0f, 9.0f);
		ol.append(obj);
		assert( ol.length() == 1 && ol.at(0).test(1.0f, 4.0f, 9.0f) );
		ol.removeFirst();
		assert( ol.length() == 0  );
		
		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr3);
		assert( olp.length() == 4 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );
		
		olp.removeFirst();
		assert( olp.length() == 3 && olp.at(0)->test(1.0f, 2.0f, 3.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) );
		olp.removeFirst();
		assert( olp.length() == 2 && olp.at(0)->test(20.3f, 69.9f, 69.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) );
		olp.removeFirst();
		assert( olp.length() == 1 && olp.at(0)->test(20.3f, 69.9f, 69.0f) );
		olp.removeFirst();		
		assert( olp.length() == 0  );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr4);
		assert( olp.length() == 1 && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		olp.removeFirst();
		assert( olp.length() == 0  );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}


	void testListRemoveLast() {
		BEGIN_TEST;

		List<I32> il(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1  && il.at(0) == 1);
		il.append(2);
		assert( il.length() == 2  && il.at(1) == 2);
		il.append(69);
		il.append(30);
		assert( il.length() == 4  && il.at(2) == 69 && il.at(3) == 30);
		assert( il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
	
		il.removeLast();
		assert( il.length() == 3 && il.at(0) == 1 && il.at(1) == 2 && il.at(2) == 69 );
		il.removeLast();
		assert( il.length() == 2 && il.at(0) == 1 && il.at(1) == 2);
		il.removeLast();
		assert( il.length() == 1 && il.at(0) == 1);
		il.removeLast();
		assert( il.length() == 0  );
		il.append(34);
		il.append(22);
		assert( il.length() == 2  && il.at(0) == 34 && il.at(1) == 22 );
		il.removeLast();
		il.removeLast();
		assert( il.length() == 0  );
		
		il = List<I32>(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1 );
		il.append(2);
		assert( il.length() == 2 );
		il.append(69);
		il.append(30);
		assert( il.length() == 4 );
		assert(il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
		il.removeLast();
		assert( il.length() == 3 && il.at(0) == 1 && il.at(1) == 2 && il.at(2) == 69 );
		il.removeLast();
		assert( il.length() == 2 && il.at(0) == 1 && il.at(1) == 2);
		il.removeLast();
		assert( il.length() == 1 && il.at(0) == 1);
		il.removeLast();
		assert( il.length() == 0  );
		il.append(98);
		il.append(102);
		assert( il.length() == 2  && il.at(0) == 98 && il.at(1) == 102 );
		il.removeLast();
		il.removeLast();
		assert( il.length() == 0  );
		


		TestObject obj = TestObject(9999.0f, 9999.0f, 9999.0f);
		List<TestObject> ol(obj);
		assert( ol.getNullValue().test(9999.0f, 9999.0f, 9999.0f) );
		obj = TestObject(0.0f, 0.0f, 0.0f);
		assert( ol.length() == 0 );
		ol.append(obj);
		assert( ol.length() == 1 );
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		assert( ol.length() == 2 );
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.length() == 4 );
		assert( ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );

		ol.removeLast();
		assert( ol.length() == 3 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) );
		ol.removeLast();
		assert( ol.length() == 2 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) );
		ol.removeLast();
		assert( ol.length() == 1 && ol.at(0).test(0.0f, 0.0f, 0.0f) );
		ol.removeLast();
		assert( ol.length() == 0  );
		obj = TestObject(1.0f, 4.0f, 9.0f);
		ol.append(obj);
		assert( ol.length() == 1  && ol.at(0).test(1.0f, 4.0f, 9.0f) );
		ol.removeLast();
		assert( ol.length() == 0  );
		
		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr3);
		assert( olp.length() == 4 );
		assert( olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );
		
		olp.removeLast();
		assert( olp.length() == 3 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) );
		olp.removeLast();
		assert( olp.length() == 2 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) );
		olp.removeLast();
		assert( olp.length() == 1 && olp.at(0)->test(0.0f, 0.0f, 0.0f) );
		olp.removeLast();		
		assert( olp.length() == 0  );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr4);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		olp.removeLast();
		assert( olp.length() == 0  );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}


	void testListEraseAll() {
		BEGIN_TEST;

		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		TestObject* ptr4 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr4);
		assert( olp.length() == 4 );
		assert( olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );
		olp.eraseAll();
		assert( olp.length() == 0  );
		TestObject* ptr5 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr5);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		olp.eraseAll();
		assert( olp.length() == 0  );

		FINISH_TEST;
	}

	void testListErase() {
		BEGIN_TEST;

		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		TestObject* ptr4 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr4);
		assert( olp.length() == 4 );
		assert( olp.length() == 4 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );	
		assert( olp.erase(ptr3) );
		assert( olp.length() == 3 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) );
		olp.erase(ptr1);
		assert( olp.length() == 2 && olp.at(0)->test(1.0f, 2.0f, 3.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) );
		olp.erase(ptr4);
		assert( olp.length() == 1 && olp.at(0)->test(1.0f, 2.0f, 3.0f) );
		assert( !olp.erase(ptr4) && !olp.erase(ptr1) && !olp.erase(ptr3));
		assert( olp.length() == 1 && olp.at(0)->test(1.0f, 2.0f, 3.0f) );
		assert( olp.erase(ptr2) );
		assert( olp.length() == 0  );
		TestObject* ptr5 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr5);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		olp.erase(ptr5);
		assert( olp.length() == 0  );

		FINISH_TEST;
	}

	void testListEraseFirst() {
		BEGIN_TEST;

		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		TestObject* ptr4 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr4);
		assert( olp.length() == 4 );
		assert( olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );	
		olp.eraseFirst();
		assert( olp.length() == 3 && olp.at(0)->test(1.0f, 2.0f, 3.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) );	
		olp.eraseFirst();
		assert( olp.length() == 2 && olp.at(0)->test(20.3f, 69.9f, 69.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) );	
		olp.eraseFirst();
		assert( olp.length() == 1 &&  olp.at(0)->test(20.3f, 69.9f, 69.0f) );	
		olp.eraseFirst();		
		assert( olp.length() == 0  );
		TestObject* ptr5 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr5);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		olp.eraseFirst();
		assert( olp.length() == 0  );

		FINISH_TEST;
	}

	void testListEraseLast() {
		BEGIN_TEST;

		List<TestObject*> olp(NIL);
		assert( olp.getNullValue() == NIL );
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		TestObject* ptr4 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr4);
		assert( olp.length() == 4 );
		assert( olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );	
		olp.eraseLast();
		assert( olp.length() == 3 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) );
		olp.eraseLast();
		assert( olp.length() == 2 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) );
		olp.eraseLast();
		assert( olp.length() == 1 && olp.at(0)->test(0.0f, 0.0f, 0.0f) );
		olp.eraseLast();		
		assert( olp.length() == 0  );
		TestObject* ptr5 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr5);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		olp.eraseLast();
		assert( olp.length() == 0  );

		FINISH_TEST;
	}

	void testListTake() {
		BEGIN_TEST;

		List<I32> il(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1  && il.at(0) == 1);
		il.append(2);
		assert( il.length() == 2  && il.at(1) == 2);
		il.append(69);
		il.append(30);
		assert( il.length() == 4  && il.at(2) == 69 && il.at(3) == 30);
		assert( il.length() == 4 && il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
	
		assert( il.take(200) == 9999 && il.take(9999) == 9999 );
		assert( il.length() == 4 && il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );

		assert( il.take(69) == 69 );
		assert( il.length() == 3 && il.at(0) == 1 && il.at(1) == 2 && il.at(2) == 30 );
		assert( il.take(30) == 30 );
		assert( il.length() == 2 && il.at(0) == 1 && il.at(1) == 2 );
		assert( il.take(1) == 1 );
		assert( il.length() == 1 && il.at(0) == 2 );
		assert( il.take(1) == 9999 );		
		assert( il.take(2) == 2 );
		assert( il.length() == 0  );
		il.append(34);
		il.append(22);
		assert( il.length() == 2  && il.at(0) == 34 && il.at(1) == 22 );
		assert( il.take(34) == 34 );
		assert( il.length() == 1 && il.at(0) == 22 );
		assert( il.take(22) == 22 );
		assert( il.length() == 0  );
	
		TestObject obj = TestObject(9999.0f, 9999.0f, 9999.0f);
		List<TestObject> ol(obj);
		assert( ol.getNullValue().test(9999.0f, 9999.0f, 9999.0f) );
		obj = TestObject(0.0f, 0.0f, 0.0f);
		assert( ol.length() == 0 );
		ol.append(obj);
		assert( ol.length() == 1 );
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		assert( ol.length() == 2 );
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.length() == 4 );
		assert( ol.length() == 4 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );

		TestObject tmp = ol.take( TestObject(20.3f, 69.9f, 69.0f) );
		assert( tmp.test(20.3f, 69.9f, 69.0f) );
		assert( ol.length() == 3 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) );
		tmp = ol.take( TestObject(0.0f, 0.0f, 0.0f) );
		assert( tmp.test(0.0f, 0.0f, 0.0f) );
		assert( ol.length() == 2 && ol.at(0).test(1.0f, 2.0f, 3.0f) && ol.at(1).test(20.3f, 69.9f, 69.0f) );
		tmp = ol.take( TestObject(20.3f, 69.9f, 69.0f) );
		assert( tmp.test(20.3f, 69.9f, 69.0f) );
		assert( ol.length() == 1 && ol.at(0).test(1.0f, 2.0f, 3.0f) );
		tmp = ol.take( TestObject(0.0f, 0.0f, 0.0f) );
		assert( tmp.test(9999.0f, 9999.0f, 9999.0f) );
		tmp = ol.take( TestObject(9999.9f, 9999.0f, 9999.0f) );
		assert( tmp.test(9999.0f, 9999.0f, 9999.0f) );
		tmp = ol.take( TestObject(1.0f, 2.0f, 3.0f) );		
		assert( tmp.test(1.0f, 2.0f, 3.0f) );
		assert( ol.length() == 0  );
		ol.append(TestObject(1.0f, 4.0f, 9.0f));
		assert( ol.length() == 1  && ol.at(0).test(1.0f, 4.0f, 9.0f) );
		tmp = ol.take( TestObject(1.0f, 4.0f, 9.0f ) );
		assert( tmp.test(1.0f, 4.0f, 9.0f) );
		assert( ol.length() == 0  );
		
		List<TestObject*> olp(NIL);
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr3);
		assert( olp.length() == 4 );
		assert( olp.length() == 4 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );
		
		TestObject* tmpptr = olp.take(ptr2);
		assert( tmpptr->test(1.0f, 2.0f, 3.0f) );
		assert( olp.length() == 3 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) );
		tmpptr = olp.take(ptr1);
		assert( tmpptr->test(0.0f, 0.0f, 0.0f) );
		assert( olp.length() == 2 && olp.at(0)->test(20.3f, 69.9f, 69.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) );
		tmpptr = olp.take(ptr3);
		assert( tmpptr->test(20.3f, 69.9f, 69.0f) );
		assert( olp.length() == 1 && olp.at(0)->test(20.3f, 69.9f, 69.0f) );
		assert( olp.take(ptr1) == NIL && olp.take(ptr2) == NIL && olp.take(NIL) == NIL );		
		tmpptr = olp.take(ptr3);
		assert( tmpptr->test(20.3f, 69.9f, 69.0f) );		
		assert( olp.length() == 0  );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr4);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		tmpptr = olp.take(ptr4);
		assert( tmpptr->test(-1.0f, 4.0f, -9.0f) );
		assert( olp.length() == 0  && olp.take(ptr1) == NIL && olp.take(ptr2) == NIL && olp.take(ptr3) == NIL && olp.take(ptr4) == NIL && olp.take(NIL) == NIL);

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testListTakeFirst() {
		BEGIN_TEST;

		List<I32> il(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1  && il.at(0) == 1);
		il.append(2);
		assert( il.length() == 2  && il.at(1) == 2);
		il.append(69);
		il.append(30);
		assert( il.length() == 4  && il.at(2) == 69 && il.at(3) == 30);
		assert( il.length() == 4 && il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
	
		assert( il.takeFirst() == 1 );
		assert( il.length() == 3 && il.at(0) == 2 && il.at(2) == 30 && il.at(1) == 69 );
		assert( il.takeFirst() == 2 );
		assert( il.length() == 2 && il.at(1) == 30 && il.at(0) == 69 );
		assert( il.takeFirst() == 69 );
		assert( il.length() == 1 && il.at(0) == 30 );
		assert( il.takeFirst() == 30 );
		assert( il.length() == 0  );
		il.append(34);
		il.append(22);
		assert( il.length() == 2  && il.at(0) == 34 && il.at(1) == 22 );
		assert( il.takeFirst() == 34 );
		assert( il.takeFirst() == 22 );
		assert( il.length() == 0  );
	
		TestObject obj = TestObject(9999.0f, 9999.0f, 9999.0f);
		List<TestObject> ol(obj);
		assert( ol.getNullValue().test(9999.0f, 9999.0f, 9999.0f) );
		obj = TestObject(0.0f, 0.0f, 0.0f);
		assert( ol.length() == 0 );
		ol.append(obj);
		assert( ol.length() == 1 );
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		assert( ol.length() == 2 );
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.length() == 4 );
		assert( ol.length() == 4 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );

		TestObject tmp = ol.takeFirst();
		assert( tmp.test(0.0f, 0.0f, 0.0f) );
		assert( ol.length() == 3 && ol.at(0).test(1.0f, 2.0f, 3.0f) && ol.at(1).test(20.3f, 69.9f, 69.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) );
		tmp = ol.takeFirst();
		assert( tmp.test(1.0f, 2.0f, 3.0f) );
		assert( ol.length() == 2 && ol.at(0).test(20.3f, 69.9f, 69.0f) && ol.at(1).test(20.3f, 69.9f, 69.0f) );
		tmp = ol.takeFirst();
		assert( tmp.test(20.3f, 69.9f, 69.0f) );
		assert( ol.length() == 1 && ol.at(0).test(20.3f, 69.9f, 69.0f) );
		tmp = ol.takeFirst();
		assert( tmp.test(20.3f, 69.9f, 69.0f) );
		assert( ol.length() == 0  );
		obj = TestObject(1.0f, 4.0f, 9.0f);
		ol.append(obj);
		assert( ol.length() == 1  && ol.at(0).test(1.0f, 4.0f, 9.0f) );
		tmp = ol.takeFirst();
		assert( tmp.test(1.0f, 4.0f, 9.0f) );
		assert( ol.length() == 0  );
		
		List<TestObject*> olp(NIL);
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr3);
		assert( olp.length() == 4 );
		assert( olp.length() == 4 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );
		
		TestObject* tmpptr = olp.takeFirst();
		assert( tmpptr->test(0.0f, 0.0f, 0.0f) );
		assert( olp.length() == 3 && olp.at(0)->test(1.0f, 2.0f, 3.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) );
		tmpptr = olp.takeFirst();
		assert( tmpptr->test(1.0f, 2.0f, 3.0f) );
		assert( olp.length() == 2 && olp.at(0)->test(20.3f, 69.9f, 69.0f) && olp.at(1)->test(20.3f, 69.9f, 69.0f) );
		tmpptr = olp.takeFirst();
		assert( tmpptr->test(20.3f, 69.9f, 69.0f) );
		assert( olp.length() == 1 && olp.at(0)->test(20.3f, 69.9f, 69.0f) );
		tmpptr = olp.takeFirst();
		assert( tmpptr->test(20.3f, 69.9f, 69.0f) );		
		assert( olp.length() == 0  );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr4);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		tmpptr = olp.takeFirst();
		assert( tmpptr->test(-1.0f, 4.0f, -9.0f) );
		assert( olp.length() == 0  );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testListTakeLast() {
		BEGIN_TEST;

		List<I32> il(9999);
		assert( il.length() == 0 );
		il.append(1);
		assert( il.length() == 1  && il.at(0) == 1);
		il.append(2);
		assert( il.length() == 2  && il.at(1) == 2);
		il.append(69);
		il.append(30);
		assert( il.length() == 4  && il.at(2) == 69 && il.at(3) == 30);
		assert( il.at(0) == 1 && il.at(1) == 2 && il.at(3) == 30 && il.at(2) == 69 );
	
		assert( il.takeLast() == 30 );
		assert( il.length() == 3 && il.at(0) == 1 && il.at(1) == 2 && il.at(2) == 69 );
		assert( il.takeLast() == 69 );
		assert( il.length() == 2 && il.at(0) == 1 && il.at(1) == 2);
		assert( il.takeLast() == 2 );
		assert( il.length() == 1 && il.at(0) == 1);
		assert( il.takeLast() == 1 );
		assert( il.length() == 0  );
		il.append(34);
		il.append(22);
		assert( il.length() == 2  && il.at(0) == 34 && il.at(1) == 22 );
		assert( il.takeLast() == 22 );
		assert( il.takeLast() == 34 );
		assert( il.length() == 0  );
	
		TestObject obj = TestObject(9999.0f, 9999.0f, 9999.0f);
		List<TestObject> ol(obj);
		assert( ol.getNullValue().test(9999.0f, 9999.0f, 9999.0f) );
		obj = TestObject(0.0f, 0.0f, 0.0f);
		assert( ol.length() == 0 );
		ol.append(obj);
		assert( ol.length() == 1 );
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ol.append(obj);
		assert( ol.length() == 2 );
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ol.append(obj);
		ol.append(obj);
		assert( ol.length() == 4 );
		assert( ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) && ol.at(3).test(20.3f, 69.9f, 69.0f) );

		TestObject tmp = ol.takeLast();
		assert( tmp.test(20.3f, 69.9f, 69.0f) );
		assert( ol.length() == 3 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) && ol.at(2).test(20.3f, 69.9f, 69.0f) );
		tmp = ol.takeLast();
		assert( tmp.test(20.3f, 69.9f, 69.0f) );
		assert( ol.length() == 2 && ol.at(0).test(0.0f, 0.0f, 0.0f) && ol.at(1).test(1.0f, 2.0f, 3.0f) );
		tmp = ol.takeLast();
		assert( tmp.test(1.0f, 2.0f, 3.0f) );
		assert( ol.length() == 1 && ol.at(0).test(0.0f, 0.0f, 0.0f) );
		tmp = ol.takeLast();
		assert( tmp.test(0.0f, 0.0f, 0.0f) );
		assert( ol.length() == 0  );
		obj = TestObject(1.0f, 4.0f, 9.0f);
		ol.append(obj);
		assert( ol.length() == 1  && ol.at(0).test(1.0f, 4.0f, 9.0f) );
		tmp = ol.takeLast();
		assert( tmp.test(1.0f, 4.0f, 9.0f) );
		assert( ol.length() == 0  );
		
		List<TestObject*> olp(NIL);
		TestObject* ptr1 = new TestObject();
		assert( olp.length() == 0 );
		olp.append(ptr1);
		assert( olp.length() == 1 );
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		olp.append(ptr2);
		assert( olp.length() == 2 );
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		olp.append(ptr3);
		olp.append(ptr3);
		assert( olp.length() == 4 );
		assert( olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) && olp.at(3)->test(20.3f, 69.9f, 69.0f) );
		
		TestObject* tmpptr = olp.takeLast();
		assert( tmpptr->test(20.3f, 69.9f, 69.0f) );
		assert( olp.length() == 3 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) && olp.at(2)->test(20.3f, 69.9f, 69.0f) );
		tmpptr = olp.takeLast();
		assert( tmpptr->test(20.3f, 69.9f, 69.0f) );
		assert( olp.length() == 2 && olp.at(0)->test(0.0f, 0.0f, 0.0f) && olp.at(1)->test(1.0f, 2.0f, 3.0f) );
		tmpptr = olp.takeLast();
		assert( tmpptr->test(1.0f, 2.0f, 3.0f) );
		assert( olp.length() == 1 && olp.at(0)->test(0.0f, 0.0f, 0.0f) );
		tmpptr = olp.takeLast();
		assert( tmpptr->test(0.0f, 0.0f, 0.0f) );		
		assert( olp.length() == 0  );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		olp.append(ptr4);
		assert( olp.length() == 1  && olp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		tmpptr = olp.takeLast();
		assert( tmpptr->test(-1.0f, 4.0f, -9.0f) );
		assert( olp.length() == 0  );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testListCopyingBehaviour() {
		BEGIN_TEST;

		List<I32> list(9999);
		assert( list.length() == 0);

		list.append(12);
		list.append(13);
		list.append(15);

		assert (list.length() == 3 && list.at(0) == 12 && list.at(1) == 13 && list.at(2) == 15 );
		assert( refTest(list, 0, 1) == 13 );
		assert (list.length() == 3 && list.at(0) == 13 && list.at(1) == 13 && list.at(2) == 15 );
		assert( refTest(list, 1, 1) == 14 );
		assert (list.length() == 3 && list.at(0) == 13 && list.at(1) == 14 && list.at(2) == 15 );
		assert( refTest(list, 2, 0) == 15 );
		assert (list.length() == 3 && list.at(0) == 13 && list.at(1) == 14 && list.at(2) == 15 );

		list = refTest2(list, 16);
		assert ( list.length() == 4 && list.at(0) == 13 && list.at(1) == 14 && list.at(2) == 15 && list.at(3) == 16 );
		list = refTest2(list, 17);
		assert ( list.length() == 5 && list.at(0) == 13 && list.at(1) == 14 && list.at(2) == 15 && list.at(3) == 16 && list.at(4) == 17 );
		list = refTest2(list, 18);
		assert ( list.length() == 6 && list.at(0) == 13 && list.at(1) == 14 && list.at(2) == 15 && list.at(3) == 16 && list.at(4) == 17 && list.at(5) == 18 );

		List<I32> list2(list);
		assert ( list2.length() == 6 && list2.at(0) == 13 && list2.at(1) == 14 && list2.at(2) == 15 && list2.at(3) == 16 && list2.at(4) == 17 && list2.at(5) == 18 );
		list2.clear();
		list2.append(100);
		assert ( list.length() == 6 && list.at(0) == 13 && list.at(1) == 14 && list.at(2) == 15 && list.at(3) == 16 && list.at(4) == 17 && list.at(5) == 18 );
		assert( list2.length() == 1 && list2.at(0) == 100 );
		list = list2;
		assert( list2.length() == 1 && list2.at(0) == 100 );
		assert( list.length() == 1 && list.at(0) == 100 );
		list.append(24);
		assert( list2.length() == 1 && list2.at(0) == 100 );
		assert( list.length() == 2 && list.at(0) == 100 && list.at(1) == 24 );

		List<I32> list3(9997);
		assert( list3.getNullValue() == 9997 );
		assert( list3.length() == 0 );
		list3 = list;
		assert( list3.getNullValue() != 9997 && list3.getNullValue() == list.getNullValue() );
		assert( list3.length() == 2 && list3.at(0) == 100 && list3.at(1) == 24 );


		FINISH_TEST;
	}

	void testListIterator() {
		BEGIN_TEST;

		List<I32> l(0);

		List<I32>::Iterator itr = l.begin();

		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_false(itr.hasPrev());

		itr = l.end();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_false(itr.hasPrev());

		l.append(3);
		itr = l.begin();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_false(itr.hasPrev());
		ass_eq(itr.val(), 3);

		itr = l.end();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_false(itr.hasPrev());
		ass_eq(itr.val(), 3);
		
		l.append(99);
		itr = l.begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_false(itr.hasPrev());
		ass_eq(itr.val(), 3);

		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.hasPrev());
		ass_eq(itr.val(), 99);

		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_false(itr.hasPrev());

		itr = l.end();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.hasPrev());
		ass_eq(itr.val(), 99);

		itr.prev();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_false(itr.hasPrev());
		ass_eq(itr.val(), 3);

		itr.prev();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_false(itr.hasPrev());
		
		l.append(23);
		l.append(-99);
   
		itr = l.begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_false(itr.hasPrev());
		ass_eq(itr.val(), 3);

		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.hasPrev());
		ass_eq(itr.val(), 99);

		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.hasPrev());
		ass_eq(itr.val(), 23);

		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.hasPrev());
		ass_eq(itr.val(), -99);

		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_false(itr.hasPrev());


		itr = l.end();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.hasPrev());
		ass_eq(itr.val(), -99);

		itr.prev();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.hasPrev());
		ass_eq(itr.val(), 23);

		itr.prev();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.hasPrev());
		ass_eq(itr.val(), 99);

		itr.prev();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_false(itr.hasPrev());
		ass_eq(itr.val(), 3);

		itr.prev();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_false(itr.hasPrev());
		FINISH_TEST;
	}
	

} // namespace cc

int main(int argc, char** argv) {
	cc::testListCreateAndDestroy();
	cc::testListAppend();
	cc::testListPrepend();
	cc::testListAtAndLast();
	cc::testListClear();
	cc::testListContains();
	cc::testListRemove();
	cc::testListRemoveFirst();
	cc::testListRemoveLast();
	cc::testListErase();
	cc::testListEraseFirst();
  	cc::testListEraseAll();
	cc::testListEraseLast();
	cc::testListTake();
	cc::testListTakeFirst();	
	cc::testListTakeLast();
	cc::testListCopyingBehaviour();
	cc::testListIterator();	
	return 0;
}

