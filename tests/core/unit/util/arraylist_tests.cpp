#include "core/testcore.h"
#include "core/util/arraylist.h"

#ifndef DEBUG_REF_POINTERS
#define DEBUG_REF_POINTERS 1
#endif

namespace cc {

	class TestObject {
		public:
			TestObject() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
			TestObject(F32 x, F32 y, F32 z) : m_x(x), m_y(y), m_z(z) {}

			Boolean test(F32 x, F32 y, F32 z) {
				return m_x == x && m_y == y && m_z == z;
			}

			F32 m_x, m_y, m_z;

	};

	I32 refTest(ArrayList<I32>& v, Size idx, I32 add) {
		v.at(idx) += add;
		return v.at(idx);
	}

	ArrayList<I32> refTest2(ArrayList<I32>& v, I32 value) {
		v.append(value);
		return v;
	}


	void testArrayListCreateAndDestroy() {
		BEGIN_TEST;

		ArrayList<I32> iv(30);
		assert( iv.size() == 0 );
		assert( iv.capacity() == 30 );
		
		iv = ArrayList<I32>(32);
		assert( iv.size() == 0 );
		assert( iv.capacity() == 32 );

		ArrayList<TestObject> ov;
		assert( ov.size() == 0 );
		assert( ov.capacity() == 0 );
		
		ArrayList<TestObject*> ovp;
		assert( ovp.size() == 0 );
		assert( ovp.capacity() == 0 );

		FINISH_TEST;
	}

	void testArrayListSet() {
		BEGIN_TEST;

		ArrayList<I32> iv(2);
		assert( iv.size() == 0 && iv.capacity() == 2);
		iv.set(0, 1);
		assert( iv.size() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.set(1, 2);
		assert( iv.size() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.set(2, 69);
		iv.set(3, 30);
		assert( iv.size() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		iv.set(9, 111);
		ass_eq( iv.size(), 10);
		ass_eq( iv.capacity(), 10);		
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 && iv.at(9) == 111);

		ArrayList<TestObject> ov(2);
		TestObject obj;
		assert( ov.size() == 0 && ov.capacity() == 2);
		ov.set(0, obj);
		assert( ov.size() == 1 && ov.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.set(1, obj);
		assert( ov.size() == 2 && ov.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.set(2, obj);
		ov.set(3, obj);
		assert( ov.size() == 4 && ov.capacity() == 4);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );
		obj = TestObject(1.0f, 1.0f, 1.0f);
		ov.set(9, obj);
		ass_eq( ov.size(), 10);
		ass_eq( ov.capacity(), 10);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) && ov.at(9).test(1.0f, 1.0f, 1.0f) );

		
		ArrayList<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.size() == 0 && ovp.capacity() == 2);
		ovp.set(0, ptr1);
		assert( ovp.size() == 1 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.set(1, ptr2);
		assert( ovp.size() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.set(2, ptr3);
		ovp.set(3, ptr3);
		assert( ovp.size() == 4 && ovp.capacity() == 4);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );

		ovp.set(9, ptr1);
		ass_eq( ovp.size(), 10);
		ass_eq( ovp.capacity(), 10);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) && ovp.at(9)->test(0.0f, 0.0f, 0.0f) );

		delete ptr1;
		delete ptr2;
		delete ptr3;

		FINISH_TEST;
	}

	void testArrayListSetAll() {
		BEGIN_TEST;

		ArrayList<I32> iv(2);
		assert( iv.size() == 0 && iv.capacity() == 2);
		iv.set(0, 1);
		assert( iv.size() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.set(1, 2);
		assert( iv.size() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.set(2, 69);
		iv.set(3, 30);
		assert( iv.size() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		iv.set(9, 111);
		ass_eq( iv.size(), 10);
		ass_eq( iv.capacity(), 10);		
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 && iv.at(9) == 111);
		iv.setAll(69);
		for (Size i = 0; i < iv.size(); i++) {
			ass_eq(iv.at(i), 69);
		}		
		
		ArrayList<TestObject> ov(2);
		TestObject obj;
		assert( ov.size() == 0 && ov.capacity() == 2);
		ov.set(0, obj);
		assert( ov.size() == 1 && ov.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.set(1, obj);
		assert( ov.size() == 2 && ov.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.set(2, obj);
		ov.set(3, obj);
		assert( ov.size() == 4 && ov.capacity() == 4);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );
		obj = TestObject(1.0f, 1.0f, 1.0f);
		ov.set(9, obj);
		ass_eq( ov.size(), 10);
		ass_eq( ov.capacity(), 10);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) && ov.at(9).test(1.0f, 1.0f, 1.0f) );
		ov.setAll(TestObject(-1.0f, -1.0f, -1.0f));
		for (Size i = 0; i < ov.size(); i++) {
			ass_true(ov.at(i).test(-1.0f, -1.0f, -1.0f));
		}	
		
		ArrayList<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.size() == 0 && ovp.capacity() == 2);
		ovp.set(0, ptr1);
		assert( ovp.size() == 1 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.set(1, ptr2);
		assert( ovp.size() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.set(2, ptr3);
		ovp.set(3, ptr3);
		assert( ovp.size() == 4 && ovp.capacity() == 4);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );

		ovp.set(9, ptr1);
		ass_eq( ovp.size(), 10);
		ass_eq( ovp.capacity(), 10);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) && ovp.at(9)->test(0.0f, 0.0f, 0.0f) );
		ovp.setAll(NIL);
		for (Size i = 0; i < ovp.size(); i++) {
			ass_eq(ovp.at(i), NIL);
		}

		delete ptr1;
		delete ptr2;
		delete ptr3;

		FINISH_TEST;
	}

	void testArrayListAppend() {
		BEGIN_TEST;

		ArrayList<I32> iv(2);
		assert( iv.size() == 0 && iv.capacity() == 2);
		iv.append(1);
		assert( iv.size() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.append(2);
		assert( iv.size() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.append(69);
		iv.append(30);
		assert( iv.size() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		
		iv = ArrayList<I32>(4);
		assert( iv.size() == 0 && iv.capacity() == 4);
		iv.append(1);
		ass_eq(iv.at(0), 1);		
		assert( iv.size() == 1 && iv.capacity() == 4);
		iv.append(2);
		assert( iv.size() == 2 && iv.capacity() == 4);
		iv.append(69);
		iv.append(30);
		assert( iv.size() == 4 && iv.capacity() == 4);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );

		ArrayList<TestObject> ov(2);
		TestObject obj;
		assert( ov.size() == 0 && ov.capacity() == 2);
		ov.append(obj);
		assert( ov.size() == 1 && ov.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.append(obj);
		assert( ov.size() == 2 && ov.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.append(obj);
		ov.append(obj);
		assert( ov.size() == 4 && ov.capacity() == 4);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );

		
		ArrayList<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.size() == 0 && ovp.capacity() == 2);
		ovp.append(ptr1);
		assert( ovp.size() == 1 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.append(ptr2);
		assert( ovp.size() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.append(ptr3);
		ovp.append(ptr3);
		assert( ovp.size() == 4 && ovp.capacity() == 4);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );

		delete ptr1;
		delete ptr2;
		delete ptr3;

		FINISH_TEST;
	}

	void testArrayListInsert() {
		BEGIN_TEST;

		ArrayList<I32> iv(2);
		assert( iv.size() == 0 && iv.capacity() == 2);
		iv.append(1);
		assert( iv.size() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.append(2);
		assert( iv.size() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.append(69);
		iv.append(30);
		assert( iv.size() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 &&
				 iv.at(2) == 69 );
		
		iv.insertAt(2, -88);
		ass_eq(iv.size(), 5);
		ass_eq(iv.capacity(), 8);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(2) == -88 &&
				 iv.at(3) == 69 && iv.at(4) == 30 );
			
		iv.insertAt(0, -3);
		ass_eq(iv.size(), 6);
		ass_eq(iv.capacity(), 8);
		assert(iv.at(0) == -3 && iv.at(1) == 1 && iv.at(2) == 2 &&
				 iv.at(3) == -88 && iv.at(4) == 69 && iv.at(5) == 30 );
		
		iv.insertAt(6, 75);
		ass_eq(iv.size(), 7);
		ass_eq(iv.capacity(), 8);
		assert(iv.at(0) == -3 && iv.at(1) == 1 && iv.at(2) == 2 &&
				 iv.at(3) == -88 && iv.at(4) == 69 && iv.at(5) == 30 && iv.at(6) == 75);
		iv.insertAt(6, 71);
		ass_eq(iv.size(), 8);
		ass_eq(iv.capacity(), 8);
		assert(iv.at(0) == -3 && iv.at(1) == 1 && iv.at(2) == 2 &&
				 iv.at(3) == -88 && iv.at(4) == 69 && iv.at(5) == 30 &&
				 iv.at(6) == 71 && iv.at(7) == 75);

		FINISH_TEST;
	}

	void testArrayListRemove() {
		BEGIN_TEST;

		ArrayList<I32> iv(2);
		assert( iv.size() == 0 && iv.capacity() == 2);
		iv.append(1);
		assert( iv.size() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.append(2);
		assert( iv.size() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.append(69);
		iv.append(30);
		assert( iv.size() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 &&
				 iv.at(2) == 69 );

		assert(!iv.removeAt(4));
		assert( iv.size() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 &&
				 iv.at(2) == 69 );
		
		assert(iv.removeAt(0));
		ass_eq(iv.size(), 3);
		ass_eq(iv.capacity(), 4);		
		assert(iv.at(0) == 2 && iv.at(1) == 69 && iv.at(2) == 30 );

		assert(iv.removeAt(1));
		ass_eq(iv.size(), 2);
		ass_eq(iv.capacity(), 4);
		assert(iv.at(0) == 2 && iv.at(1) == 30 );

		assert(iv.removeAt(1));
		ass_eq(iv.size(), 1);
		ass_eq(iv.capacity(), 4);
		assert(iv.at(0) == 2);

		assert(!iv.removeAt(1));
		ass_eq(iv.size(), 1);
		ass_eq(iv.capacity(), 4);
		assert(iv.at(0) == 2);

		assert(iv.removeAt(0));
		ass_eq(iv.size(), 0);
		ass_eq(iv.capacity(), 4);

		ArrayList<TestObject> ov(2);
		TestObject obj;
		assert( ov.size() == 0 && ov.capacity() == 2);
		ov.append(obj);
		assert( ov.size() == 1 && ov.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.append(obj);
		assert( ov.size() == 2 && ov.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.append(obj);
		ov.append(obj);
		assert( ov.size() == 4 && ov.capacity() == 4);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );

		assert(!ov.removeAt(4));
		assert( ov.size() == 4 && ov.capacity() == 4);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );

		assert(ov.removeAt(0));
		ass_eq(ov.size(), 3);
		ass_eq(ov.capacity(), 4);
		assert(ov.at(0).test(1.0f, 2.0f, 3.0f) && ov.at(1).test(20.3f, 69.9f, 69.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) );

		assert(ov.removeAt(1));
		ass_eq(ov.size(), 2);
		ass_eq(ov.capacity(), 4);
	   assert(ov.at(0).test(1.0f, 2.0f, 3.0f) && ov.at(1).test(20.3f, 69.9f, 69.0f) );

		assert(ov.removeAt(1));
		ass_eq(ov.size(), 1);
		ass_eq(ov.capacity(), 4);
		assert(ov.at(0).test(1.0f, 2.0f, 3.0f) );

		assert(!ov.removeAt(1));
		ass_eq(ov.size(), 1);
		ass_eq(ov.capacity(), 4);
		assert(ov.at(0).test(1.0f, 2.0f, 3.0f) );

		assert(ov.removeAt(0));
		ass_eq(ov.size(), 0);
		ass_eq(ov.capacity(), 4);
		
		FINISH_TEST;
	}

	void testArrayListAtAndLast() {
		BEGIN_TEST;

		ArrayList<I32> iv(2);
		assert( iv.size() == 0 && iv.capacity() == 2);
		iv.append(1); iv.append(2); iv.append(69); iv.append(30);
		assert( iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		assert( iv.last() == 30 );
		
		/* Test modifying through at() */
		iv.at(0) = 4;
		assert( iv.at(0) == 4 );
		iv.at(1) = iv.at(1) * 3;
		assert( iv.at(1) == 6 );
		iv.at(2) = iv.at(1) * iv.at(0);
		assert( iv.at(0) == 4 && iv.at(1) == 6 && iv.at(2) == 24 && iv.at(3) == 30);
		iv.at(3)--;
		assert( iv.at(3) == 29 );
		assert( iv.size() == 4 && iv.capacity() == 4);
		assert( iv.last() == 29 );

		const I32 ci = iv.at(3);
		assert( ci == 29 );

		
		ArrayList<TestObject> ov(30);
		TestObject obj;
		assert( ov.size() == 0 && ov.capacity() == 30);
		ov.append(obj);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.append(obj);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.append(obj);
		ov.append(obj);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );
		assert( ov.last().test(20.3f, 69.9f, 69.0f) );

		/* Test modifying through at() */
		ov.at(0).m_x = 22.3f;
		ov.at(0).m_z = -30.11f;
		assert( ov.at(0).test(22.3f, 0.0f, -30.11f) );
		obj = TestObject(-1.0f, 23.0f, -3.1f);
		ov.at(1) = obj;
		assert( ov.at(1).test(-1.0f, 23.0f, -3.1f) );
		ov.at(2) = ov.at(0);
		ov.at(2).m_x = -12.2f;
		assert( ov.at(0).test(22.3f, 0.0f, -30.11f) && ov.at(2).test(-12.2f, 0.0f, -30.11f) );
		assert( ov.at(3).test(20.3f, 69.9f, 69.0f) );
		assert( ov.size() == 4 && ov.capacity() == 30);
		assert( ov.last().test(20.3f, 69.9f, 69.0f) );

		
		ArrayList<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.size() == 0 && ovp.capacity() == 2);
		ovp.append(ptr1);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.append(ptr2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.append(ptr3); ovp.append(ptr3);

		/* Test modifying through at() */
		ovp.at(0)->m_x = 22.3f;
		ovp.at(0)->m_z = -30.11f;
		assert( ovp.at(0)->test(22.3f, 0.0f, -30.11f) );
		TestObject* ptr4 = new TestObject(-1.0f, 23.0f, -3.1f);
		ovp.at(1) = ptr4;
		assert( ovp.at(1)->test(-1.0f, 23.0f, -3.1f) );
		ovp.at(2) = ovp.at(0);
		ovp.at(2)->m_x = -12.2f;
		assert( ovp.at(0)->test(-12.2f, 0.0f, -30.11f) && ovp.at(2)->test(-12.2f, 0.0f, -30.11f) );
		assert( ovp.at(3)->test(20.3f, 69.9f, 69.0f) );
		assert( ovp.size() == 4 && ovp.capacity() == 4);
		assert( ovp.last()->test(20.3f, 69.9f, 69.0f) );
		assert(ptr1->test(-12.2f, 0.0f, -30.11f)  && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) && ptr4->test(-1.0f, 23.0f, -3.1f) );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testArrayListClear() {
		BEGIN_TEST;

		ArrayList<I32> iv(2);
		assert( iv.size() == 0 && iv.capacity() == 2);
		iv.append(1);
		assert( iv.size() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.append(2);
		assert( iv.size() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.append(69);
		iv.append(30);
		assert( iv.size() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		iv.clear();
		assert( iv.size() == 0 && iv.capacity() == 4 );
		iv.append(34);
		iv.append(22);
		assert( iv.size() == 2 && iv.capacity() == 4 && iv.at(0) == 34 && iv.at(1) == 22 );
		iv.clear();
		assert( iv.size() == 0 && iv.capacity() == 4 );
		
		iv = ArrayList<I32>(4);
		assert( iv.size() == 0 && iv.capacity() == 4);
		iv.append(1);
		assert( iv.size() == 1 && iv.capacity() == 4);
		iv.append(2);
		assert( iv.size() == 2 && iv.capacity() == 4);
		iv.append(69);
		iv.append(30);
		assert( iv.size() == 4 && iv.capacity() == 4);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		iv.clear();
		assert( iv.size() == 0 && iv.capacity() == 4 );
		iv.append(32);
		iv.append(45);
		assert( iv.size() == 2 && iv.capacity() == 4 && iv.at(0) == 32 && iv.at(1) == 45 );
		iv.clear();
		assert( iv.size() == 0 && iv.capacity() == 4 );


		ArrayList<TestObject> ov(2);
		TestObject obj;
		assert( ov.size() == 0 && ov.capacity() == 2);
		ov.append(obj);
		assert( ov.size() == 1 && ov.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.append(obj);
		assert( ov.size() == 2 && ov.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.append(obj);
		ov.append(obj);
		assert( ov.size() == 4 && ov.capacity() == 4);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );
		ov.clear();
		assert( ov.size() == 0 && ov.capacity() == 4 );
		obj = TestObject(1.0f, 4.0f, 9.0f);
		ov.append(obj);
		assert( ov.size() == 1 && ov.capacity() == 4 && ov.at(0).test(1.0f, 4.0f, 9.0f) );
		ov.clear();
		assert( ov.size() == 0 && ov.capacity() == 4 );
		
		ArrayList<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.size() == 0 && ovp.capacity() == 2);
		ovp.append(ptr1);
		assert( ovp.size() == 1 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.append(ptr2);
		assert( ovp.size() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.append(ptr3);
		ovp.append(ptr3);
		assert( ovp.size() == 4 && ovp.capacity() == 4);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );
		ovp.clear();
		assert( ovp.size() == 0 && ovp.capacity() == 4 );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		ovp.append(ptr4);
		assert( ovp.size() == 1 && ovp.capacity() == 4 && ovp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		ovp.clear();
		assert( ovp.size() == 0 && ovp.capacity() == 4 );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testArrayListRemoveLast() {
		BEGIN_TEST;

		ArrayList<I32> iv(2);
		assert( iv.size() == 0 && iv.capacity() == 2);
		iv.append(1);
		assert( iv.size() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.append(2);
		assert( iv.size() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.append(69);
		iv.append(30);
		assert( iv.size() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert( iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
	
		iv.removeLast();
		assert( iv.size() == 3 && iv.at(0) == 1 && iv.at(1) == 2 && iv.at(2) == 69 );
		iv.removeLast();
		assert( iv.size() == 2 && iv.at(0) == 1 && iv.at(1) == 2);
		iv.removeLast();
		assert( iv.size() == 1 && iv.at(0) == 1);
		iv.removeLast();
		assert( iv.size() == 0 && iv.capacity() == 4 );
		iv.append(34);
		iv.append(22);
		assert( iv.size() == 2 && iv.capacity() == 4 && iv.at(0) == 34 && iv.at(1) == 22 );
		iv.removeLast();
		iv.removeLast();
		assert( iv.size() == 0 && iv.capacity() == 4 );
		
		iv = ArrayList<I32>(4);
		assert( iv.size() == 0 && iv.capacity() == 4);
		iv.append(1);
		assert( iv.size() == 1 && iv.capacity() == 4);
		iv.append(2);
		assert( iv.size() == 2 && iv.capacity() == 4);
		iv.append(69);
		iv.append(30);
		assert( iv.size() == 4 && iv.capacity() == 4);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		iv.removeLast();
		assert( iv.size() == 3 && iv.at(0) == 1 && iv.at(1) == 2 && iv.at(2) == 69 );
		iv.removeLast();
		assert( iv.size() == 2 && iv.at(0) == 1 && iv.at(1) == 2);
		iv.removeLast();
		assert( iv.size() == 1 && iv.at(0) == 1);
		iv.removeLast();
		assert( iv.size() == 0 && iv.capacity() == 4 );
		iv.append(98);
		iv.append(102);
		assert( iv.size() == 2 && iv.capacity() == 4 && iv.at(0) == 98 && iv.at(1) == 102 );
		iv.removeLast();
		iv.removeLast();
		assert( iv.size() == 0 && iv.capacity() == 4 );
		


		ArrayList<TestObject> ov(2);
		TestObject obj;
		assert( ov.size() == 0 && ov.capacity() == 2);
		ov.append(obj);
		assert( ov.size() == 1 && ov.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.append(obj);
		assert( ov.size() == 2 && ov.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.append(obj);
		ov.append(obj);
		assert( ov.size() == 4 && ov.capacity() == 4);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );

		ov.removeLast();
		assert( ov.size() == 3 && ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) );
		ov.removeLast();
		assert( ov.size() == 2 && ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) );
		ov.removeLast();
		assert( ov.size() == 1 && ov.at(0).test(0.0f, 0.0f, 0.0f) );
		ov.removeLast();
		assert( ov.size() == 0 && ov.capacity() == 4 );
		obj = TestObject(1.0f, 4.0f, 9.0f);
		ov.append(obj);
		assert( ov.size() == 1 && ov.capacity() == 4 && ov.at(0).test(1.0f, 4.0f, 9.0f) );
		ov.removeLast();
		assert( ov.size() == 0 && ov.capacity() == 4 );
		
		ArrayList<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.size() == 0 && ovp.capacity() == 2);
		ovp.append(ptr1);
		assert( ovp.size() == 1 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.append(ptr2);
		assert( ovp.size() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.append(ptr3);
		ovp.append(ptr3);
		assert( ovp.size() == 4 && ovp.capacity() == 4);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );
		
		ovp.removeLast();
		assert( ovp.size() == 3 && ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) );
		ovp.removeLast();
		assert( ovp.size() == 2 && ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) );
		ovp.removeLast();
		assert( ovp.size() == 1 && ovp.at(0)->test(0.0f, 0.0f, 0.0f) );
		ovp.removeLast();		
		assert( ovp.size() == 0 && ovp.capacity() == 4 );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		ovp.append(ptr4);
		assert( ovp.size() == 1 && ovp.capacity() == 4 && ovp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		ovp.removeLast();
		assert( ovp.size() == 0 && ovp.capacity() == 4 );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}


	void testArrayListEraseAll() {
		BEGIN_TEST;

		ArrayList<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.size() == 0 && ovp.capacity() == 2);
		ovp.append(ptr1);
		assert( ovp.size() == 1 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.append(ptr2);
		assert( ovp.size() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		TestObject* ptr4 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.append(ptr3);
		ovp.append(ptr4);
		assert( ovp.size() == 4 && ovp.capacity() == 4);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );
		ovp.eraseAll();
		assert( ovp.size() == 0 && ovp.capacity() == 4 );
		TestObject* ptr5 = new TestObject(-1.0f, 4.0f, -9.0f);
		ovp.append(ptr5);
		assert( ovp.size() == 1 && ovp.capacity() == 4 && ovp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		ovp.eraseAll();
		assert( ovp.size() == 0 && ovp.capacity() == 4 );

		FINISH_TEST;
	}

	void testArrayListEraseLast() {
		BEGIN_TEST;

		ArrayList<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.size() == 0 && ovp.capacity() == 2);
		ovp.append(ptr1);
		assert( ovp.size() == 1 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.append(ptr2);
		assert( ovp.size() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		TestObject* ptr4 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.append(ptr3);
		ovp.append(ptr4);
		assert( ovp.size() == 4 && ovp.capacity() == 4);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );	
		ovp.eraseLast();
		assert( ovp.size() == 3 && ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) );
		ovp.eraseLast();
		assert( ovp.size() == 2 && ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) );
		ovp.eraseLast();
		assert( ovp.size() == 1 && ovp.at(0)->test(0.0f, 0.0f, 0.0f) );
		ovp.eraseLast();		
		assert( ovp.size() == 0 && ovp.capacity() == 4 );
		TestObject* ptr5 = new TestObject(-1.0f, 4.0f, -9.0f);
		ovp.append(ptr5);
		assert( ovp.size() == 1 && ovp.capacity() == 4 && ovp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		ovp.eraseLast();
		assert( ovp.size() == 0 && ovp.capacity() == 4 );

		FINISH_TEST;
	}

	void testArrayListTakeLast() {
		BEGIN_TEST;

		ArrayList<I32> iv(2);
		assert( iv.size() == 0 && iv.capacity() == 2);
		iv.append(1);
		assert( iv.size() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.append(2);
		assert( iv.size() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.append(69);
		iv.append(30);
		assert( iv.size() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert( iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
	
		assert( iv.takeLast() == 30 );
		assert( iv.size() == 3 && iv.at(0) == 1 && iv.at(1) == 2 && iv.at(2) == 69 );
		assert( iv.takeLast() == 69 );
		assert( iv.size() == 2 && iv.at(0) == 1 && iv.at(1) == 2);
		assert( iv.takeLast() == 2 );
		assert( iv.size() == 1 && iv.at(0) == 1);
		assert( iv.takeLast() == 1 );
		assert( iv.size() == 0 && iv.capacity() == 4 );
		iv.append(34);
		iv.append(22);
		assert( iv.size() == 2 && iv.capacity() == 4 && iv.at(0) == 34 && iv.at(1) == 22 );
		assert( iv.takeLast() == 22 );
		assert( iv.takeLast() == 34 );
		assert( iv.size() == 0 && iv.capacity() == 4 );
	
		ArrayList<TestObject> ov(2);
		TestObject obj;
		assert( ov.size() == 0 && ov.capacity() == 2);
		ov.append(obj);
		assert( ov.size() == 1 && ov.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.append(obj);
		assert( ov.size() == 2 && ov.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.append(obj);
		ov.append(obj);
		assert( ov.size() == 4 && ov.capacity() == 4);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );

		TestObject tmp = ov.takeLast();
		assert( tmp.test(20.3f, 69.9f, 69.0f) );
		assert( ov.size() == 3 && ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) );
		tmp = ov.takeLast();
		assert( tmp.test(20.3f, 69.9f, 69.0f) );
		assert( ov.size() == 2 && ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) );
		tmp = ov.takeLast();
		assert( tmp.test(1.0f, 2.0f, 3.0f) );
		assert( ov.size() == 1 && ov.at(0).test(0.0f, 0.0f, 0.0f) );
		tmp = ov.takeLast();
		assert( tmp.test(0.0f, 0.0f, 0.0f) );
		assert( ov.size() == 0 && ov.capacity() == 4 );
		obj = TestObject(1.0f, 4.0f, 9.0f);
		ov.append(obj);
		assert( ov.size() == 1 && ov.capacity() == 4 && ov.at(0).test(1.0f, 4.0f, 9.0f) );
		tmp = ov.takeLast();
		assert( tmp.test(1.0f, 4.0f, 9.0f) );
		assert( ov.size() == 0 && ov.capacity() == 4 );
		
		ArrayList<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.size() == 0 && ovp.capacity() == 2);
		ovp.append(ptr1);
		assert( ovp.size() == 1 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.append(ptr2);
		assert( ovp.size() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.append(ptr3);
		ovp.append(ptr3);
		assert( ovp.size() == 4 && ovp.capacity() == 4);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );
		
		TestObject* tmpptr = ovp.takeLast();
		assert( tmpptr->test(20.3f, 69.9f, 69.0f) );
		assert( ovp.size() == 3 && ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) );
		tmpptr = ovp.takeLast();
		assert( tmpptr->test(20.3f, 69.9f, 69.0f) );
		assert( ovp.size() == 2 && ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) );
		tmpptr = ovp.takeLast();
		assert( tmpptr->test(1.0f, 2.0f, 3.0f) );
		assert( ovp.size() == 1 && ovp.at(0)->test(0.0f, 0.0f, 0.0f) );
		tmpptr = ovp.takeLast();
		assert( tmpptr->test(0.0f, 0.0f, 0.0f) );		
		assert( ovp.size() == 0 && ovp.capacity() == 4 );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		ovp.append(ptr4);
		assert( ovp.size() == 1 && ovp.capacity() == 4 && ovp.at(0)->test(-1.0f, 4.0f, -9.0f) );
		tmpptr = ovp.takeLast();
		assert( tmpptr->test(-1.0f, 4.0f, -9.0f) );
		assert( ovp.size() == 0 && ovp.capacity() == 4 );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testArrayListCopyingBehaviour() {
		BEGIN_TEST;

		ArrayList<I32> vec(15);
		assert( vec.size() == 0 && vec.capacity() == 15 );

		vec.append(12);
		vec.append(13);
		vec.append(15);

		assert (vec.size() == 3 && vec.at(0) == 12 && vec.at(1) == 13 && vec.at(2) == 15 );
		assert( refTest(vec, 0, 1) == 13 );
		assert (vec.size() == 3 && vec.at(0) == 13 && vec.at(1) == 13 && vec.at(2) == 15 );
		assert( refTest(vec, 1, 1) == 14 );
		assert (vec.size() == 3 && vec.at(0) == 13 && vec.at(1) == 14 && vec.at(2) == 15 );
		assert( refTest(vec, 2, 0) == 15 );
		assert (vec.size() == 3 && vec.at(0) == 13 && vec.at(1) == 14 && vec.at(2) == 15 );

		vec = refTest2(vec, 16);
		assert ( vec.size() == 4 && vec.at(0) == 13 && vec.at(1) == 14 && vec.at(2) == 15 && vec.at(3) == 16 );
		vec = refTest2(vec, 17);
		assert ( vec.size() == 5 && vec.at(0) == 13 && vec.at(1) == 14 && vec.at(2) == 15 && vec.at(3) == 16 && vec.at(4) == 17 );
		vec = refTest2(vec, 18);
		assert ( vec.size() == 6 && vec.at(0) == 13 && vec.at(1) == 14 && vec.at(2) == 15 && vec.at(3) == 16 && vec.at(4) == 17 && vec.at(5) == 18 );

		ArrayList<I32> vec2(vec);
		assert ( vec2.size() == 6 && vec2.at(0) == 13 && vec2.at(1) == 14 && vec2.at(2) == 15 && vec2.at(3) == 16 && vec2.at(4) == 17 && vec2.at(5) == 18 );
		vec2.clear();
		vec2.append(100);
		assert ( vec.size() == 6 && vec.at(0) == 13 && vec.at(1) == 14 && vec.at(2) == 15 && vec.at(3) == 16 && vec.at(4) == 17 && vec.at(5) == 18 );
		assert( vec2.size() == 1 && vec2.capacity() == 15 && vec2.at(0) == 100 );
		vec = vec2;
		assert( vec2.size() == 1 && vec2.capacity() == 15 && vec2.at(0) == 100 );
		assert( vec.size() == 1 && vec.capacity() == 15 && vec.at(0) == 100 );
		vec.append(24);
		assert( vec2.size() == 1 && vec2.capacity() == 15 && vec2.at(0) == 100 );
		assert( vec.size() == 2 && vec.capacity() == 15 && vec.at(0) == 100 && vec.at(1) == 24 );

		ArrayList<I32> vec3(1);
		assert( vec3.size() == 0 && vec3.capacity() == 1 );
		vec3 = vec;
		assert( vec3.size() == 2 && vec3.capacity() == 15 && vec3.at(0) == 100 && vec3.at(1) == 24 );


		FINISH_TEST;
	}

	void testArrayListAutoResizing() {
		BEGIN_TEST;

		ArrayList<I32> vec(3);
		assert( vec.size() == 0 && vec.capacity() == 3 );
		vec.append(1);
		assert( vec.size() == 1 && vec.capacity() == 3 && vec.at(0) == 1 );
		vec.append(3);
		assert( vec.size() == 2 && vec.capacity() == 3 && vec.at(0) == 1 && vec.at(1) == 3 );
		vec.append(5);
		assert( vec.size() == 3 && vec.capacity() == 3 && vec.at(0) == 1 && vec.at(1) == 3 && vec.at(2) == 5 );
		/* First resize */
		vec.append(10);
		assert( vec.size() == 4 && vec.capacity() == 6 && vec.at(0) == 1 && vec.at(1) == 3 && vec.at(2) == 5 && vec.at(3) == 10 );
		vec.append(11);
		assert( vec.size() == 5 && vec.capacity() == 6 && vec.at(0) == 1 && vec.at(1) == 3 && vec.at(2) == 5 && vec.at(3) == 10 && vec.at(4) == 11 );
		vec.append(12);
		assert( vec.size() == 6 && vec.capacity() == 6 && vec.at(0) == 1 && vec.at(1) == 3 && vec.at(2) == 5 && vec.at(3) == 10 && vec.at(4) == 11 && vec.at(5) == 12 );
		/* Second Resize */
		vec.append(2);
		assert( vec.size() == 7 && vec.capacity() == 12 && vec.at(0) == 1 && vec.at(1) == 3 && vec.at(2) == 5 && vec.at(3) == 10 && vec.at(4) == 11 && vec.at(5) == 12 && vec.at(6) == 2);

		FINISH_TEST;
	}


	void testArrayListExtendTo() {
		BEGIN_TEST;

		ArrayList<I32> vec(1);
		assert( vec.size() == 0 && vec.capacity() == 1 );
		vec.append(1);
		assert( vec.size() == 1 && vec.capacity() == 1 && vec.at(0) == 1 );

		vec.extendTo(3);
		assert( vec.size() == 3 && vec.capacity() == 3 && vec.at(0) == 1);
		vec.at(1) = 4;
		vec.at(2) = 5;
		assert( vec.size() == 3 && vec.capacity() == 3 &&
				  vec.at(0) == 1 && vec.at(1) == 4 && vec.at(2) == 5);
		vec.append(-9);	
		assert( vec.size() == 4 && vec.capacity() == 6 &&
				  vec.at(0) == 1 && vec.at(1) == 4 && vec.at(2) == 5 &&
				  vec.at(3) == -9);

		vec.extendTo(5);
		assert( vec.size() == 5 && vec.capacity() == 6 &&
				  vec.at(0) == 1 && vec.at(1) == 4 && vec.at(2) == 5 &&
				  vec.at(3) == -9);
		vec.at(4) = -100;
		assert( vec.size() == 5 && vec.capacity() == 6 &&
				  vec.at(0) == 1 && vec.at(1) == 4 && vec.at(2) == 5 &&
				  vec.at(3) == -9 && vec.at(4) == -100);

		vec = ArrayList<I32>();
		assert( vec.size() == 0 && vec.capacity() == 0 );
		vec.initArrayListWithBlockSize(1);
		assert( vec.size() == 0 && vec.capacity() == 1 );
		vec.extendTo(1);
		assert( vec.size() == 1 && vec.capacity() == 1 );
		vec.at(0) = 3414;
		assert( vec.size() == 1 && vec.capacity() == 1 && vec.at(0) == 3414);
				
		FINISH_TEST;
	}

	void testArrayListReserve() {
		BEGIN_TEST;

		ArrayList<I32> vec(1);
		assert( vec.size() == 0 && vec.capacity() == 1 );
		vec.append(1);
		assert( vec.size() == 1 && vec.capacity() == 1 && vec.at(0) == 1 );

		vec.reserve(3);
		assert( vec.size() == 1 && vec.capacity() == 3 && vec.at(0) == 1);
		vec.append(4);
		vec.append(5);
		assert( vec.size() == 3 && vec.capacity() == 3 &&
				  vec.at(0) == 1 && vec.at(1) == 4 && vec.at(2) == 5);
		vec.append(-9);	
		assert( vec.size() == 4 && vec.capacity() == 6 &&
				  vec.at(0) == 1 && vec.at(1) == 4 && vec.at(2) == 5 &&
				  vec.at(3) == -9);

		vec.reserve(5);
		assert( vec.size() == 4 && vec.capacity() == 6 &&
				  vec.at(0) == 1 && vec.at(1) == 4 && vec.at(2) == 5 &&
				  vec.at(3) == -9);
		vec.append(-100);		
		assert( vec.size() == 5 && vec.capacity() == 6 &&
				  vec.at(0) == 1 && vec.at(1) == 4 && vec.at(2) == 5 &&
				  vec.at(3) == -9 && vec.at(4) == -100);

		vec = ArrayList<I32>();
		assert( vec.size() == 0 && vec.capacity() == 0 );
		vec.reserve(1);
		assert( vec.size() == 0 && vec.capacity() == 1 );
		vec.reserve(1);
		assert( vec.size() == 0 && vec.capacity() == 1 );
		vec.append(3414);
		assert( vec.size() == 1 && vec.capacity() == 1 && vec.at(0) == 3414);
		
		
		FINISH_TEST;
	}



} // namespace cc

int main(int argc, char** argv) {
	cc::testArrayListCreateAndDestroy();
	cc::testArrayListSet();
	cc::testArrayListSetAll();	
	cc::testArrayListAppend();
	cc::testArrayListInsert();
	cc::testArrayListRemove();	
	cc::testArrayListAtAndLast();
	cc::testArrayListClear();
	cc::testArrayListRemoveLast();
	cc::testArrayListEraseAll();
	cc::testArrayListEraseLast();
	cc::testArrayListTakeLast();
	cc::testArrayListCopyingBehaviour();
	cc::testArrayListAutoResizing();
	cc::testArrayListExtendTo();
	cc::testArrayListReserve();	
	return 0;
}

