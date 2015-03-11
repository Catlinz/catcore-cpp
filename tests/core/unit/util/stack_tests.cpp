#include "core/testcore.h"
#include "core/util/stack.h"

namespace cc {

	class TestObject {
		public:
			TestObject() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
			TestObject(F32 x, F32 y, F32 z) : m_x(x), m_y(y), m_z(z) {}

			Boolean test(F32 x, F32 y, F32 z) const {
				return m_x == x && m_y == y && m_z == z;
			}

			F32 m_x, m_y, m_z;

	};

	I32 refTest(Stack<I32>& s, Size idx, I32 add) {
		s.peek(idx) += add;
		return s.peek(idx);
	}

	Stack<I32> refTest2(Stack<I32>& s, I32 value) {
		s.push(value);
		return s;
	}


	void testStackCreateAndDestroy() {
		BEGIN_TEST;

		Stack<I32> is(30);
		ass_true(is.isEmpty());
		assert( is.size() == 0 );
		assert( is.capacity() == 30 );
		
		is = Stack<I32>(32);
		assert( is.size() == 0 );
		assert( is.capacity() == 32 );
		ass_true(is.isEmpty());

		Stack<TestObject> os;
		assert( os.size() == 0 );
		assert( os.capacity() == 0 );
		ass_true(os.isEmpty());
		
		Stack<TestObject*> osp;
		assert( osp.size() == 0 );
		assert( osp.capacity() == 0 );
		ass_true(osp.isEmpty());

		FINISH_TEST;
	}

	void testStackPush() {
		BEGIN_TEST;

		Stack<I32> is(2);
		ass_true(is.isEmpty());
		assert( is.size() == 0 && is.capacity() == 2);
		is.push(1);
		ass_false(is.isEmpty());
		ass_eq(is.top(), 1);		
		assert( is.size() == 1 && is.capacity() == 2 && is.peek(0) == 1);
		is.push(2);
		ass_eq(is.top(), 2);	
		assert( is.size() == 2 && is.capacity() == 2 && is.peek(1) == 2);
		is.push(69);
		is.push(30);
		ass_eq(is.top(), 30);	
		assert( is.size() == 4 && is.capacity() == 4 && is.peek(2) == 69 && is.peek(3) == 30);
		assert(is.peek(0) == 1 && is.peek(1) == 2 && is.peek(3) == 30 && is.peek(2) == 69 );
		ass_false(is.isEmpty());
		
		is = Stack<I32>(30);
		ass_true(is.isEmpty());
		assert( is.size() == 0 && is.capacity() == 30);
		is.push(1);
		ass_eq(is.top(), 1);	
		ass_false(is.isEmpty());
		assert( is.size() == 1 && is.capacity() == 30);
		is.push(2);
		ass_eq(is.top(), 2);	
		assert( is.size() == 2 && is.capacity() == 30);
		is.push(69);
		is.push(30);
		ass_eq(is.top(), 30);	
		assert( is.size() == 4 && is.capacity() == 30);
		assert(is.peek(0) == 1 && is.peek_const(1) == 2 && is.peek_const(3) == 30 && is.peek_const(2) == 69 );
		ass_false(is.isEmpty());

		Stack<TestObject> os(2);
		ass_true(os.isEmpty());
		TestObject obj;
		assert( os.size() == 0 && os.capacity() == 2);
		os.push(obj);
		ass_true(os.top().test(0.0f, 0.0f, 0.0f));	
		ass_false(os.isEmpty());
		assert( os.size() == 1 && os.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		os.push(obj);
		ass_true(os.top().test(1.0f, 2.0f, 3.0f));
		assert( os.size() == 2 && os.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		os.push(obj);
		os.push(obj);
		ass_true(os.top().test(20.3f, 69.9f, 69.0f));
		assert( os.size() == 4 && os.capacity() == 4);
		assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek_const(1).test(1.0f, 2.0f, 3.0f) && os.peek(2).test(20.3f, 69.9f, 69.0f) && os.peek_const(3).test(20.3f, 69.9f, 69.0f) );
		ass_false(os.isEmpty());

		
		Stack<TestObject*> osp(2);
		ass_true(osp.isEmpty());
		TestObject* ptr1 = new TestObject();
		assert( osp.size() == 0 && osp.capacity() == 2);
		osp.push(ptr1);
		ass_false(osp.isEmpty());
		assert( osp.size() == 1 && osp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		osp.push(ptr2);
		assert( osp.size() == 2 && osp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		osp.push(ptr3);
		osp.push(ptr3);
		assert( osp.size() == 4 && osp.capacity() == 4);
		assert( osp.peek_const(0)->test(0.0f, 0.0f, 0.0f) && osp.peek_const(1)->test(1.0f, 2.0f, 3.0f) && osp.peek(2)->test(20.3f, 69.9f, 69.0f) && osp.peek(3)->test(20.3f, 69.9f, 69.0f) );
		ass_false(osp.isEmpty());

		delete ptr1;
		delete ptr2;
		delete ptr3;

		FINISH_TEST;
	}

	void testStackRemove() {
		BEGIN_TEST;

		Stack<I32> is(2);
		ass_true(is.isEmpty());
		ass_false(is.remove());
		ass_false(is.remove(-1));
		ass_true(is.isEmpty());
		assert( is.size() == 0 && is.capacity() == 2);
		is.push(1);
		ass_false(is.isEmpty());
		assert( is.size() == 1 && is.capacity() == 2 && is.peek(0) == 1);
		is.push(2);
		assert( is.size() == 2 && is.capacity() == 2 && is.peek(1) == 2);
		is.push(69);
		is.push(30);
		assert( is.size() == 4 && is.capacity() == 4 && is.peek(2) == 69 && is.peek(3) == 30);
		assert(is.peek(0) == 1 && is.peek(1) == 2 && is.peek(3) == 30 &&
				 is.peek(2) == 69 );
		ass_false(is.isEmpty());
		
		assert(is.remove());
		ass_eq(is.size(), 3);
		ass_eq(is.capacity(), 4);		
		assert(is.peek(0) == 1 && is.peek(1) == 2 && is.peek(2) == 69);
		ass_false(is.isEmpty());
		
		assert(is.remove(-1));
		ass_eq(is.size(), 2);
		ass_eq(is.capacity(), 4);
		assert(is.peek(0) == 1 && is.peek(1) == 2);
		ass_false(is.isEmpty());
		
		assert(is.remove());
		ass_eq(is.size(), 1);
		ass_eq(is.capacity(), 4);
		assert(is.peek(0) == 1);
		ass_false(is.isEmpty());

		assert(is.remove());
		ass_eq(is.size(), 0);
		ass_eq(is.capacity(), 4);
		ass_true(is.isEmpty());

		ass_false(is.remove());
		ass_false(is.remove(-1));
		ass_eq(is.size(), 0);
		ass_eq(is.capacity(), 4);
		ass_true(is.isEmpty());

		Stack<TestObject> os(2);
		TestObject obj;
		ass_true(os.isEmpty());
		ass_false(os.remove());
		ass_false(os.remove(TestObject()));
		assert( os.size() == 0 && os.capacity() == 2);
		os.push(obj);
		ass_false(os.isEmpty());
		assert( os.size() == 1 && os.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		os.push(obj);
		assert( os.size() == 2 && os.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		os.push(obj);
		os.push(obj);
		assert( os.size() == 4 && os.capacity() == 4);
		assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek(1).test(1.0f, 2.0f, 3.0f) && os.peek(2).test(20.3f, 69.9f, 69.0f) && os.peek(3).test(20.3f, 69.9f, 69.0f) );
		ass_false(os.isEmpty());

		assert(os.remove());
		ass_false(os.isEmpty());
		ass_eq(os.size(), 3);
		ass_eq(os.capacity(), 4);
		assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek(1).test(1.0f, 2.0f, 3.0f) && os.peek(2).test(20.3f, 69.9f, 69.0f));

		assert(os.remove(TestObject()));
		ass_false(os.isEmpty());
		ass_eq(os.size(), 2);
		ass_eq(os.capacity(), 4);
	   assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek(1).test(1.0f, 2.0f, 3.0f));

		assert(os.remove());
		ass_false(os.isEmpty());
		ass_eq(os.size(), 1);
		ass_eq(os.capacity(), 4);
	   assert( os.peek(0).test(0.0f, 0.0f, 0.0f));

		assert(os.remove(TestObject(-1.0f, -2.0f, -3.0f)));
		ass_true(os.isEmpty());
		ass_eq(os.size(), 0);
		ass_eq(os.capacity(), 4);

		ass_false(os.remove());
		ass_false(os.remove(TestObject(-1.0f, -2.0f, -3.0f)));
		ass_true(os.isEmpty());
		ass_eq(os.size(), 0);
		ass_eq(os.capacity(), 4);
		
		FINISH_TEST;
	}

	void testStackPop() {
		BEGIN_TEST;

		Stack<I32> is(2);
		ass_true(is.isEmpty());
		assert( is.size() == 0 && is.capacity() == 2);
		is.push(1);
		ass_false(is.isEmpty());
		assert( is.size() == 1 && is.capacity() == 2 && is.peek(0) == 1);
		is.push(2);
		assert( is.size() == 2 && is.capacity() == 2 && is.peek(1) == 2);
		is.push(69);
		is.push(30);
		assert( is.size() == 4 && is.capacity() == 4 && is.peek(2) == 69 && is.peek(3) == 30);
		assert(is.peek(0) == 1 && is.peek(1) == 2 && is.peek(3) == 30 &&
				 is.peek(2) == 69 );
		ass_false(is.isEmpty());

		I32 val = is.pop();		
		ass_eq(val, 30);		
		ass_eq(is.size(), 3);
		ass_eq(is.capacity(), 4);		
		assert(is.peek(0) == 1 && is.peek(1) == 2 && is.peek(2) == 69);
		ass_false(is.isEmpty());
		
		val = is.pop(0);		
		ass_eq(val, 69);
		ass_eq(is.size(), 2);
		ass_eq(is.capacity(), 4);
		assert(is.peek(0) == 1 && is.peek(1) == 2);
		ass_false(is.isEmpty());

		is.push(33);
		ass_eq(is.size(), 3);
		ass_eq(is.capacity(), 4);
		assert(is.peek(0) == 1 && is.peek(1) == 2 && is.peek(2) == 33);
		ass_false(is.isEmpty());

		val = is.pop();		
		ass_eq(val, 33);
		ass_eq(is.size(), 2);
		ass_eq(is.capacity(), 4);
		assert(is.peek(0) == 1 && is.peek(1) == 2);
		ass_false(is.isEmpty());
		
		val = is.pop(-1);		
		ass_eq(val, 2);	
		ass_eq(is.size(), 1);
		ass_eq(is.capacity(), 4);
		assert(is.peek(0) == 1);
		ass_false(is.isEmpty());

	   val = is.pop(-2);		
		ass_eq(val, 1);	
		ass_eq(is.size(), 0);
		ass_eq(is.capacity(), 4);
		ass_true(is.isEmpty());

		is.push(-33);
		ass_eq(is.size(), 1);
		ass_eq(is.capacity(), 4);
		assert(is.peek(0) == -33);
		ass_false(is.isEmpty());

		val = is.pop();		
		ass_eq(val, -33);	
		ass_eq(is.size(), 0);
		ass_eq(is.capacity(), 4);
		ass_true(is.isEmpty());

		Stack<TestObject> os(2);
		TestObject obj;
		ass_true(os.isEmpty());
		assert( os.size() == 0 && os.capacity() == 2);
		os.push(obj);
		ass_false(os.isEmpty());
		assert( os.size() == 1 && os.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		os.push(obj);
		assert( os.size() == 2 && os.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		os.push(obj);
		os.push(obj);
		assert( os.size() == 4 && os.capacity() == 4);
		assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek(1).test(1.0f, 2.0f, 3.0f) && os.peek(2).test(20.3f, 69.9f, 69.0f) && os.peek(3).test(20.3f, 69.9f, 69.0f) );
		ass_false(os.isEmpty());

		obj = os.pop(TestObject(-1.0f, -2.0f, -3.0f));		
		ass_true(obj.test(20.3f, 69.9f, 69.0f));		
		ass_false(os.isEmpty());
		ass_eq(os.size(), 3);
		ass_eq(os.capacity(), 4);
		assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek(1).test(1.0f, 2.0f, 3.0f) && os.peek(2).test(20.3f, 69.9f, 69.0f));

		os.push(TestObject(1.0f, 2.0f, 3.0f));
		assert( os.size() == 4 && os.capacity() == 4);
		assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek(1).test(1.0f, 2.0f, 3.0f) && os.peek(2).test(20.3f, 69.9f, 69.0f) && os.peek(3).test(1.0f, 2.0f, 3.0f) );
		ass_false(os.isEmpty());

		obj = os.pop();		
		ass_true(obj.test(1.0f, 2.0f, 3.0f));		
		ass_false(os.isEmpty());
		ass_eq(os.size(), 3);
		ass_eq(os.capacity(), 4);
		assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek(1).test(1.0f, 2.0f, 3.0f) && os.peek(2).test(20.3f, 69.9f, 69.0f));
		
		obj = os.pop();		
		ass_true(obj.test(20.3f, 69.9f, 69.0f));
		ass_false(os.isEmpty());
		ass_eq(os.size(), 2);
		ass_eq(os.capacity(), 4);
	   assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek(1).test(1.0f, 2.0f, 3.0f));

		obj = os.pop();		
		ass_true(obj.test(1.0f, 2.0f, 3.0f));
		ass_false(os.isEmpty());
		ass_eq(os.size(), 1);
		ass_eq(os.capacity(), 4);
	   assert( os.peek(0).test(0.0f, 0.0f, 0.0f));

		obj = os.pop(TestObject(-2.0f, -3.0f, 232.343f));		
		ass_true(obj.test(0.0f, 0.0f, 0.0f));
		ass_true(os.isEmpty());
		ass_eq(os.size(), 0);
		ass_eq(os.capacity(), 4);

		os.push(TestObject(1.0f, 2.0f, 2.0f));
		ass_false(os.isEmpty());
		ass_eq(os.size(), 1);
		ass_eq(os.capacity(), 4);
	   assert( os.peek(0).test(1.0f, 2.0f, 2.0f));

		obj = os.pop(TestObject(1.0f, 2.0f, -2.0f));		
		ass_true(obj.test(1.0f, 2.0f, 2.0f));
		ass_true(os.isEmpty());
		ass_eq(os.size(), 0);
		ass_eq(os.capacity(), 4);
	
		
		FINISH_TEST;
	}

	void testStackClear() {
		BEGIN_TEST;

		Stack<I32> is(2);
		assert( is.size() == 0 && is.capacity() == 2);
		is.push(1);
		assert( is.size() == 1 && is.capacity() == 2 && is.peek(0) == 1);
		is.push(2);
		assert( is.size() == 2 && is.capacity() == 2 && is.peek(1) == 2);
		is.push(69);
		is.push(30);
		assert( is.size() == 4 && is.capacity() == 4 && is.peek(2) == 69 && is.peek(3) == 30);
		assert(is.peek(0) == 1 && is.peek(1) == 2 && is.peek(3) == 30 && is.peek(2) == 69 );
		is.clear();
		assert( is.size() == 0 && is.capacity() == 4 );
		is.push(34);
		is.push(22);
		assert( is.size() == 2 && is.capacity() == 4 && is.peek(0) == 34 && is.peek(1) == 22 );
		is.clear();
		assert( is.size() == 0 && is.capacity() == 4 );
		
		is = Stack<I32>(30);
		assert( is.size() == 0 && is.capacity() == 30);
		is.push(1);
		assert( is.size() == 1 && is.capacity() == 30);
		is.push(2);
		assert( is.size() == 2 && is.capacity() == 30);
		is.push(69);
		is.push(30);
		assert( is.size() == 4 && is.capacity() == 30);
		assert(is.peek(0) == 1 && is.peek(1) == 2 && is.peek(3) == 30 && is.peek(2) == 69 );
		is.clear(-1);
		assert( is.size() == 0 && is.capacity() == 30 );
		is.push(32);
		is.push(45);
		assert( is.size() == 2 && is.capacity() == 30 && is.peek(0) == 32 && is.peek(1) == 45 );
		is.clear();
		assert( is.size() == 0 && is.capacity() == 30 );


		Stack<TestObject> os(2);
		TestObject obj;
		assert( os.size() == 0 && os.capacity() == 2);
		os.push(obj);
		assert( os.size() == 1 && os.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		os.push(obj);
		assert( os.size() == 2 && os.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		os.push(obj);
		os.push(obj);
		assert( os.size() == 4 && os.capacity() == 4);
		assert( os.peek(0).test(0.0f, 0.0f, 0.0f) && os.peek(1).test(1.0f, 2.0f, 3.0f) && os.peek(2).test(20.3f, 69.9f, 69.0f) && os.peek(3).test(20.3f, 69.9f, 69.0f) );
		os.clear(TestObject());
		assert( os.size() == 0 && os.capacity() == 4 );
		obj = TestObject(1.0f, 4.0f, 9.0f);
		os.push(obj);
		assert( os.size() == 1 && os.capacity() == 4 && os.peek(0).test(1.0f, 4.0f, 9.0f) );
		os.clear();
		assert( os.size() == 0 && os.capacity() == 4 );
		
		Stack<TestObject*> osp(2);
		TestObject* ptr1 = new TestObject();
		assert( osp.size() == 0 && osp.capacity() == 2);
		osp.push(ptr1);
		assert( osp.size() == 1 && osp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		osp.push(ptr2);
		assert( osp.size() == 2 && osp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		osp.push(ptr3);
		osp.push(ptr3);
		assert( osp.size() == 4 && osp.capacity() == 4);
		assert( osp.peek(0)->test(0.0f, 0.0f, 0.0f) && osp.peek(1)->test(1.0f, 2.0f, 3.0f) && osp.peek(2)->test(20.3f, 69.9f, 69.0f) && osp.peek(3)->test(20.3f, 69.9f, 69.0f) );
		osp.clear();
		assert( osp.size() == 0 && osp.capacity() == 4 );
		assert( ptr1->test(0.0f, 0.0f, 0.0f) && ptr2->test(1.0f, 2.0f, 3.0f) && ptr3->test(20.3f, 69.9f, 69.0f) );
		TestObject* ptr4 = new TestObject(-1.0f, 4.0f, -9.0f);
		osp.push(ptr4);
		assert( osp.size() == 1 && osp.capacity() == 4 && osp.peek(0)->test(-1.0f, 4.0f, -9.0f) );
		osp.clear(NIL);
		assert( osp.size() == 0 && osp.capacity() == 4 );

		delete ptr1;
		delete ptr2;
		delete ptr3;
		delete ptr4;

		FINISH_TEST;
	}

	void testStackEraseAll() {
		BEGIN_TEST;

		Stack<TestObject*> osp(2);
		TestObject* ptr1 = new TestObject();
		assert( osp.size() == 0 && osp.capacity() == 2);
		osp.push(ptr1);
		assert( osp.size() == 1 && osp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		osp.push(ptr2);
		assert( osp.size() == 2 && osp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		TestObject* ptr4 = new TestObject(20.3f, 69.9f, 69.0f);
		osp.push(ptr3);
		osp.push(ptr4);
		assert( osp.size() == 4 && osp.capacity() == 4);
		assert( osp.peek(0)->test(0.0f, 0.0f, 0.0f) && osp.peek(1)->test(1.0f, 2.0f, 3.0f) && osp.peek(2)->test(20.3f, 69.9f, 69.0f) && osp.peek(3)->test(20.3f, 69.9f, 69.0f) );
		osp.eraseAll();
		assert( osp.size() == 0 && osp.capacity() == 4 );
		TestObject* ptr5 = new TestObject(-1.0f, 4.0f, -9.0f);
		osp.push(ptr5);
		assert( osp.size() == 1 && osp.capacity() == 4 && osp.peek(0)->test(-1.0f, 4.0f, -9.0f) );
		osp.eraseAll();
		assert( osp.size() == 0 && osp.capacity() == 4 );

		FINISH_TEST;
	}

	void testStackErase() {
		BEGIN_TEST;

		Stack<TestObject*> osp(2);
		TestObject* ptr1 = new TestObject();
		ass_false(osp.erase());		
		assert( osp.size() == 0 && osp.capacity() == 2);
		osp.push(ptr1);
		assert( osp.size() == 1 && osp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		osp.push(ptr2);
		assert( osp.size() == 2 && osp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		TestObject* ptr4 = new TestObject(20.3f, 69.9f, 69.0f);
		osp.push(ptr3);
		osp.push(ptr4);
		assert( osp.size() == 4 && osp.capacity() == 4);
		assert( osp.peek(0)->test(0.0f, 0.0f, 0.0f) && osp.peek(1)->test(1.0f, 2.0f, 3.0f) && osp.peek(2)->test(20.3f, 69.9f, 69.0f) && osp.peek(3)->test(20.3f, 69.9f, 69.0f) );	
		assert( osp.erase() );
		assert( osp.size() == 3 && osp.peek(0)->test(0.0f, 0.0f, 0.0f) && osp.peek(1)->test(1.0f, 2.0f, 3.0f) && osp.peek(2)->test(20.3f, 69.9f, 69.0f) );
		assert( osp.erase() );
		assert( osp.size() == 2 && osp.peek(0)->test(0.0f, 0.0f, 0.0f) && osp.peek(1)->test(1.0f, 2.0f, 3.0f) );
		assert( osp.erase() );
		assert( osp.size() == 1 && osp.peek(0)->test(0.0f, 0.0f, 0.0f) );
		assert( osp.erase() );	
		assert( osp.size() == 0 && osp.capacity() == 4 );
		TestObject* ptr5 = new TestObject(-1.0f, 4.0f, -9.0f);
		osp.push(ptr5);
		assert( osp.size() == 1 && osp.capacity() == 4 && osp.peek(0)->test(-1.0f, 4.0f, -9.0f) );
		assert( osp.erase() );
		assert( osp.size() == 0 && osp.capacity() == 4 );
		ass_false(osp.erase());		

		FINISH_TEST;
	}

	void testStackCopyingBehaviour() {
		BEGIN_TEST;

		Stack<I32> stack(15);
		assert( stack.size() == 0 && stack.capacity() == 15 );

		stack.push(12);
		stack.push(13);
		stack.push(15);

		assert (stack.size() == 3 && stack.peek(0) == 12 && stack.peek(1) == 13 && stack.peek(2) == 15 );
		assert( refTest(stack, 0, 1) == 13 );
		assert (stack.size() == 3 && stack.peek(0) == 13 && stack.peek(1) == 13 && stack.peek(2) == 15 );
		assert( refTest(stack, 1, 1) == 14 );
		assert (stack.size() == 3 && stack.peek(0) == 13 && stack.peek(1) == 14 && stack.peek(2) == 15 );
		assert( refTest(stack, 2, 0) == 15 );
		assert (stack.size() == 3 && stack.peek(0) == 13 && stack.peek(1) == 14 && stack.peek(2) == 15 );

		stack = refTest2(stack, 16);
		assert ( stack.size() == 4 && stack.peek(0) == 13 && stack.peek(1) == 14 && stack.peek(2) == 15 && stack.peek(3) == 16 );
		stack = refTest2(stack, 17);
		assert ( stack.size() == 5 && stack.peek(0) == 13 && stack.peek(1) == 14 && stack.peek(2) == 15 && stack.peek(3) == 16 && stack.peek(4) == 17 );
		stack = refTest2(stack, 18);
		assert ( stack.size() == 6 && stack.peek(0) == 13 && stack.peek(1) == 14 && stack.peek(2) == 15 && stack.peek(3) == 16 && stack.peek(4) == 17 && stack.peek(5) == 18 );

		Stack<I32> stack2(stack);
		assert ( stack2.size() == 6 && stack2.peek(0) == 13 && stack2.peek(1) == 14 && stack2.peek(2) == 15 && stack2.peek(3) == 16 && stack2.peek(4) == 17 && stack2.peek(5) == 18 );
		stack2.clear();
		stack2.push(100);
		assert ( stack.size() == 6 && stack.peek(0) == 13 && stack.peek(1) == 14 && stack.peek(2) == 15 && stack.peek(3) == 16 && stack.peek(4) == 17 && stack.peek(5) == 18 );
		assert( stack2.size() == 1 && stack2.capacity() == 15 && stack2.peek(0) == 100 );
		stack = stack2;
		assert( stack2.size() == 1 && stack2.capacity() == 15 && stack2.peek(0) == 100 );
		assert( stack.size() == 1 && stack.capacity() == 15 && stack.peek(0) == 100 );
		stack.push(24);
		assert( stack2.size() == 1 && stack2.capacity() == 15 && stack2.peek(0) == 100 );
		assert( stack.size() == 2 && stack.capacity() == 15 && stack.peek(0) == 100 && stack.peek(1) == 24 );

		Stack<I32> stack3(1);
		assert( stack3.size() == 0 && stack3.capacity() == 1 );
		stack3 = stack;
		assert( stack3.size() == 2 && stack3.capacity() == 15 && stack3.peek(0) == 100 && stack3.peek(1) == 24 );


		FINISH_TEST;
	}

	void testStackAutoResizing() {
		BEGIN_TEST;

		Stack<I32> stack(3);
		assert( stack.size() == 0 && stack.capacity() == 3 );
		stack.push(1);
		assert( stack.size() == 1 && stack.capacity() == 3 && stack.peek(0) == 1 );
		stack.push(3);
		assert( stack.size() == 2 && stack.capacity() == 3 && stack.peek(0) == 1 && stack.peek(1) == 3 );
		stack.push(5);
		assert( stack.size() == 3 && stack.capacity() == 3 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 );
		/* First resize */
		stack.push(10);
		assert( stack.size() == 4 && stack.capacity() == 6 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 && stack.peek(3) == 10 );
		stack.push(11);
		assert( stack.size() == 5 && stack.capacity() == 6 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 && stack.peek(3) == 10 && stack.peek(4) == 11 );
		stack.push(12);
		assert( stack.size() == 6 && stack.capacity() == 6 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 && stack.peek(3) == 10 && stack.peek(4) == 11 && stack.peek(5) == 12 );
		/* Second Resize */
		stack.push(2);
		assert( stack.size() == 7 && stack.capacity() == 12 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 && stack.peek(3) == 10 && stack.peek(4) == 11 && stack.peek(5) == 12 && stack.peek(6) == 2);

		FINISH_TEST;
	}

	void testStackReserve() {
		BEGIN_TEST;

		Stack<I32> stack(2);
		assert( stack.size() == 0 && stack.capacity() == 2 );
		stack.reserve(3);
		assert( stack.size() == 0 && stack.capacity() == 3 );
		stack.reserve(2);
		assert( stack.size() == 0 && stack.capacity() == 3 );
		stack.push(1);
		assert( stack.size() == 1 && stack.capacity() == 3 && stack.peek(0) == 1 );
		stack.reserve(4);		
		stack.push(3);
		assert( stack.size() == 2 && stack.capacity() == 4 && stack.peek(0) == 1 && stack.peek(1) == 3 );
		stack.push(5);
		assert( stack.size() == 3 && stack.capacity() == 4 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 );
		/* First resize */
		stack.push(10);
		assert( stack.size() == 4 && stack.capacity() == 4 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 && stack.peek(3) == 10 );
		stack.reserve(5);		
		stack.push(11);
		assert( stack.size() == 5 && stack.capacity() == 5 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 && stack.peek(3) == 10 && stack.peek(4) == 11 );
		stack.push(12);
		assert( stack.size() == 6 && stack.capacity() == 10 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 && stack.peek(3) == 10 && stack.peek(4) == 11 && stack.peek(5) == 12 );
		/* Second Resize */
		stack.push(2);
		assert( stack.size() == 7 && stack.capacity() == 10 && stack.peek(0) == 1 && stack.peek(1) == 3 && stack.peek(2) == 5 && stack.peek(3) == 10 && stack.peek(4) == 11 && stack.peek(5) == 12 && stack.peek(6) == 2);

		FINISH_TEST;
	}




} // namespace cc

int main(int argc, char** argv) {
	cc::testStackCreateAndDestroy();	
	cc::testStackPush();
	cc::testStackRemove();
	cc::testStackPop();
	cc::testStackClear();
	cc::testStackEraseAll();
	cc::testStackErase();	
	cc::testStackCopyingBehaviour();
	cc::testStackAutoResizing();
	cc::testStackReserve();	
	return 0;
}

