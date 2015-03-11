#include "core/testcore.h"
#include "core/util/simplequeue.h"

namespace cc {

	I32 destroyed_count = 0;	

	class TestObject {
	  public:
		TestObject() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
		TestObject(F32 x, F32 y, F32 z) : m_x(x), m_y(y), m_z(z) {}

		inline Boolean operator==(const TestObject& rval) const {
			return (m_x == rval.m_x && m_y == rval.m_y && m_z == rval.m_z);
		}

		inline Boolean operator!=(const TestObject& rval) const {
			return !(*this == rval);
		}		

		~TestObject() {
			destroyed_count++;
		}		

		Boolean test(F32 x, F32 y, F32 z) const {
			return m_x == x && m_y == y && m_z == z;
		}

		F32 m_x, m_y, m_z;

		friend std::ostream& operator<<(std::ostream& out, const TestObject& o) {
			return out << "(" << o.m_x << ", " << o.m_y << ", " << o.m_z << ")";
		}	

	};
	
		

	void testCreateAndDestroySimpleQueue() {
		BEGIN_TEST;

		SimpleQueue<I32> s1;
		ass_eq(s1.capacity(), 0);

		s1.initWithCapacity(2, 66);
		ass_eq(s1.capacity(), 2);
		ass_eq(s1.at(0), 66);
		ass_eq(s1.at(1), 66);
		ass_true(s1.isEmpty());
		ass_eq(s1.nullValue(), 66);
		ass_eq(s1.peek(), 66);
		ass_eq(s1.pop(), 66);
		ass_false(s1.isFull());		

		SimpleQueue<I32> s2(2, -1);
		ass_eq(s2.capacity(), 2);
		ass_eq(s2.at(0), -1);
		ass_eq(s2.at(1), -1);
		ass_true(s2.isEmpty());
		ass_eq(s2.nullValue(), -1);
		ass_eq(s2.peek(), -1);
		ass_eq(s2.pop(), -1);
		ass_false(s2.isFull());

		SimpleQueue<TestObject> s3(2, TestObject(-1.0f, -1.0f, -1.0f));
		ass_eq(s3.capacity(), 2);
		ass_true(s3.at(0).test(-1.0f, -1.0f, -1.0f));
		ass_true(s3.at(1).test(-1.0f, -1.0f, -1.0f));
		ass_true(s3.isEmpty());
		ass_true(s3.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s3.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s3.pop().test(-1.0f, -1.0f, -1.0f));
		ass_false(s3.isFull());

		SimpleQueue<TestObject>* s4 = new SimpleQueue<TestObject>(4, TestObject(-1.0f, -1.0f, -1.0f));
		destroyed_count = 0;
		ass_eq(destroyed_count, 0);
		delete s4;
		ass_eq(destroyed_count, 5);
		

		FINISH_TEST;
	}

	void testCopySimpleQueue() {
		BEGIN_TEST;
	
		SimpleQueue<TestObject> s1(4, TestObject(-1.0f, -1.0f, -1.0f));
		ass_eq(s1.capacity(), 4);
		ass_true(s1.at(0).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.at(1).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.at(2).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s1.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.pop().test(-1.0f, -1.0f, -1.0f));

		SimpleQueue<TestObject> s2(s1);
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.at(1).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.at(2).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		destroyed_count = 0;
		
		s2 = s1;
		ass_eq(destroyed_count, 0);		
		ass_eq(s1.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.at(1).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.at(2).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		ass_eq(s1.capacity(), 4);
		ass_true(s1.at(0).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.at(1).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.at(2).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s1.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.pop().test(-1.0f, -1.0f, -1.0f));

		s2 = SimpleQueue<TestObject>(5, TestObject(0.0f, 0.0f, 0.0f));
		ass_eq(s2.capacity(), 5);
		ass_true(s2.at(0).test(0.0f, 0.0f, 0.0f));
		ass_true(s2.at(1).test(0.0f, 0.0f, 0.0f));
		ass_true(s2.at(2).test(0.0f, 0.0f, 0.0f));
		ass_true(s2.at(3).test(0.0f, 0.0f, 0.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(0.0f, 0.0f, 0.0f) );
		ass_true(s2.peek().test(0.0f, 0.0f, 0.0f));
		ass_true(s2.pop().test(0.0f, 0.0f, 0.0f));
		
		destroyed_count = 0;
		s2 = s1;
		ass_eq(destroyed_count, 5);
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.at(1).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.at(2).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		destroyed_count = 0;
		

		s1.push(TestObject(1.0f, 2.0f, 3.0f));
		s1.push(TestObject(2.0f, 3.0f, 4.0f));
		s1.push(TestObject(3.0f, 4.0f, 5.0f));
		s1.push(TestObject(4.0f, 5.0f, 6.0f));
		ass_true(s1.at(0).test(1.0f, 2.0f, 3.0f) && s1.at(1).test(2.0f, 3.0f, 4.0f) &&
					s1.at(2).test(3.0f, 4.0f, 5.0f) && s1.at(3).test(4.0f, 5.0f, 6.0f));

		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));

		s2 = s1;
		ass_true(s2.at(0).test(1.0f, 2.0f, 3.0f) && s2.at(1).test(2.0f, 3.0f, 4.0f) &&
					s2.at(2).test(3.0f, 4.0f, 5.0f) && s2.at(3).test(4.0f, 5.0f, 6.0f));

		s1.clear();
		ass_true(s1.at(0).test(-1.0f, -1.0f, -1.0f) && s1.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s1.at(2).test(-1.0f, -1.0f, -1.0f) && s1.at(3).test(-1.0f, -1.0f, -1.0f));

		ass_true(s2.at(0).test(1.0f, 2.0f, 3.0f) && s2.at(1).test(2.0f, 3.0f, 4.0f) &&
					s2.at(2).test(3.0f, 4.0f, 5.0f) && s2.at(3).test(4.0f, 5.0f, 6.0f));
		
		FINISH_TEST;
	}

	void testPushAndPopAndAtForSimpleQueueInteger() {
		BEGIN_TEST;	

		SimpleQueue<I32> s1(4, -1);
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);

		Boolean tf = s1.push(1);		
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.peek(), 1);
		ass_eq(s1.at(0), 1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);

		tf = s1.push(2);
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.peek(), 1);
		ass_eq(s1.at(0), 1); ass_eq(s1.at(1), 2); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);

		tf = s1.push(4);
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isFull());
		ass_false(s1.isEmpty());
		ass_eq(s1.peek(), 1);
		ass_eq(s1.at(0), 1); ass_eq(s1.at(1), 2); ass_eq(s1.at(2), 4); ass_eq(s1.at(3), -1);

		tf = s1.push(3);
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_true(s1.isFull());
		ass_eq(s1.peek(), 1);
		ass_eq(s1.at(0), 1); ass_eq(s1.at(1), 2); ass_eq(s1.at(2), 4); ass_eq(s1.at(3), 3);

		tf = s1.push(5);
		ass_false(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_true(s1.isFull());
		ass_eq(s1.peek(), 1);
		ass_eq(s1.at(0), 1); ass_eq(s1.at(1), 2); ass_eq(s1.at(2), 4); ass_eq(s1.at(3), 3);

		I32 ival = s1.pop();		
		ass_eq(ival, 1);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.peek(), 2);
		ass_eq(s1.at(0), 2); ass_eq(s1.at(1), 4); ass_eq(s1.at(2), 3); ass_eq(s1.at(3), -1);

		tf = s1.push(5);		
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_true(s1.isFull());
		ass_eq(s1.peek(), 2);
		ass_eq(s1.at(0), 2); ass_eq(s1.at(1), 4); ass_eq(s1.at(2), 3); ass_eq(s1.at(3), 5);

		tf = s1.push(6);		
		ass_false(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_true(s1.isFull());
		ass_eq(s1.peek(), 2);
		ass_eq(s1.at(0), 2); ass_eq(s1.at(1), 4); ass_eq(s1.at(2), 3); ass_eq(s1.at(3), 5);

		ival = s1.pop();	
		ass_eq(ival, 2);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.peek(), 4);
		ass_eq(s1.at(0), 4); ass_eq(s1.at(1), 3); ass_eq(s1.at(2), 5); ass_eq(s1.at(3), -1);

		ival = s1.pop();	
		ass_eq(ival, 4);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.peek(), 3);
		ass_eq(s1.at(0), 3); ass_eq(s1.at(1), 5); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);

		ival = s1.pop();	
		ass_eq(ival, 3);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.peek(), 5);
		ass_eq(s1.at(0), 5); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);		

		ival = s1.pop();	
		ass_eq(ival, 5);
		ass_eq(s1.capacity(), 4);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.peek(), -1);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);		

		tf = (s1.push(4) && s1.push(2) && s1.push(-3) && s1.push(9));		
		ass_true(tf);
		ass_true(!s1.isEmpty());
		ass_true(s1.isFull());
		ass_eq(s1.at(0), 4); ass_eq(s1.at(1), 2); ass_eq(s1.at(2), -3); ass_eq(s1.at(3), 9);
		ival = s1.pop();
		ass_eq(ival, 4);
		ass_true(!s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.at(0), 2); ass_eq(s1.at(1), -3); ass_eq(s1.at(2), 9); ass_eq(s1.at(3), -1);
		ival = s1.pop();
		ass_eq(ival, 2);
		ass_true(!s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.at(0), -3); ass_eq(s1.at(1), 9); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ival = s1.pop();
		ass_eq(ival, -3);
		ass_true(!s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.at(0), 9); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ival = s1.pop();
		ass_eq(ival, 9);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ival = s1.pop();
		ass_eq(ival, -1);
		ass_false(s1.isFull());

		FINISH_TEST;
	}

	void testPushAndPopAndAtForSimpleQueueObject() {
		BEGIN_TEST;	

		
		SimpleQueue<TestObject> s1(4, TestObject(-1.0f, -1.0f, -1.0f));
		ass_eq(s1.capacity(), 4);
		ass_true(s1.at(0).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.at(1).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.at(2).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s1.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s1.pop().test(-1.0f, -1.0f, -1.0f));		

		destroyed_count = 0;	
		Boolean tf = s1.push(TestObject(1.0f, 0.0f, 0.0f));		
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.peek().test(1.0f, 0.0f, 0.0f));
		ass_eq(destroyed_count, 1);
		ass_true(s1.at(0).test(1.0f, 0.0f, 0.0f) && s1.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s1.at(2).test(-1.0f, -1.0f, -1.0f) && s1.at(3).test(-1.0f, -1.0f, -1.0f));

		tf = s1.push(TestObject(2.0f, 2.0f, 0.0f));
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.peek().test(1.0f, 0.0f, 0.0f));
		ass_eq(destroyed_count, 2);
		ass_true(s1.at(0).test(1.0f, 0.0f, 0.0f) && s1.at(1).test(2.0f, 2.0f, 0.0f) &&
					s1.at(2).test(-1.0f, -1.0f, -1.0f) && s1.at(3).test(-1.0f, -1.0f, -1.0f));

		tf = s1.push(TestObject(4.0f, 4.0f, 4.0f));
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.peek().test(1.0f, 0.0f, 0.0f));
		ass_eq(destroyed_count, 3);
		ass_true(s1.at(0).test(1.0f, 0.0f, 0.0f) && s1.at(1).test(2.0f, 2.0f, 0.0f) &&
					s1.at(2).test(4.0f, 4.0f, 4.0f) && s1.at(3).test(-1.0f, -1.0f, -1.0f));

		tf = s1.push(TestObject(3.0, 0.0, 3.0));
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_true(s1.isFull());
		ass_true(s1.peek().test(1.0f, 0.0f, 0.0f));
		ass_eq(destroyed_count, 4);
		ass_true(s1.at(0).test(1.0f, 0.0f, 0.0f) && s1.at(1).test(2.0f, 2.0f, 0.0f) &&
					s1.at(2).test(4.0f, 4.0f, 4.0f) && s1.at(3).test(3.0, 0.0, 3.0));

		tf = s1.push(TestObject(0.0, 5.0, 0.0));
		ass_false(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_true(s1.isFull());
	   ass_true(s1.peek().test(1.0f, 0.0f, 0.0f));
		ass_eq(destroyed_count, 5);
		ass_true(s1.at(0).test(1.0f, 0.0f, 0.0f) && s1.at(1).test(2.0f, 2.0f, 0.0f) &&
					s1.at(2).test(4.0f, 4.0f, 4.0f) && s1.at(3).test(3.0, 0.0, 3.0));

		TestObject tval = s1.pop();
		ass_true(tval.test(1.0f, 0.0f, 0.0f));
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.peek().test(2.0f, 2.0f, 0.0f));
		ass_eq(destroyed_count, 5);
		ass_true(s1.at(0).test(2.0f, 2.0f, 0.0f) && s1.at(1).test(4.0f, 4.0f, 4.0f) &&
					s1.at(2).test(3.0, 0.0, 3.0) && s1.at(3).test(-1.0, -1.0, -1.0));

		tf = s1.push(TestObject(0.0, 5.0, 0.0));		
		ass_true(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_true(s1.isFull());
		ass_true(s1.peek().test(2.0f, 2.0f, 0.0f));
		ass_eq(destroyed_count, 6);
		ass_true(s1.at(0).test(2.0f, 2.0f, 0.0f) && s1.at(1).test(4.0f, 4.0f, 4.0f) &&
					s1.at(2).test(3.0, 0.0, 3.0) && s1.at(3).test(0.0, 5.0, 0.0));

		tf = s1.push(TestObject(0.0, 0.0, 6.0));		
		ass_false(tf);
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_true(s1.isFull());
		ass_true(s1.peek().test(2.0f, 2.0f, 0.0f));
		ass_eq(destroyed_count, 7);
		ass_true(s1.at(0).test(2.0f, 2.0f, 0.0f) && s1.at(1).test(4.0f, 4.0f, 4.0f) &&
					s1.at(2).test(3.0, 0.0, 3.0) && s1.at(3).test(0.0, 5.0, 0.0));

		tval = s1.pop();	
		ass_true(tval.test(2.0f, 2.0f, 0.0f));
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_true(s1.peek().test(4.0f, 4.0f, 4.0f));
		ass_eq(destroyed_count, 8);
		ass_true(s1.at(0).test(4.0f, 4.0f, 4.0f) && s1.at(1).test(3.0, 0.0, 3.0) &&
					s1.at(2).test(0.0, 5.0, 0.0) && s1.at(3).test(-1.0, -1.0, -1.0));

		tval = s1.pop();	
		ass_true(tval.test(4.0f, 4.0f, 4.0f));
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.peek().test(3.0f, 0.0f, 3.0f));
		ass_eq(destroyed_count, 9);
		ass_true(s1.at(0).test(3.0, 0.0, 3.0) && s1.at(1).test(0.0, 5.0, 0.0) &&
					s1.at(2).test(-1.0, -1.0, -1.0) && s1.at(3).test(-1.0, -1.0, -1.0));

		tval = s1.pop();	
		ass_true(tval.test(3.0f, 0.0f, 3.0f));
		ass_eq(s1.capacity(), 4);
		ass_false(s1.isEmpty());
		ass_false(s1.isFull());
	   ass_true(s1.peek().test(0.0f, 5.0f, 0.0f));
		ass_eq(destroyed_count, 10);
		ass_true(s1.at(0).test(0.0, 5.0, 0.0) && s1.at(1).test(-1.0, -1.0, -1.0) &&
					s1.at(2).test(-1.0, -1.0, -1.0) && s1.at(3).test(-1.0, -1.0, -1.0));

		tval = s1.pop();	
		ass_true(tval.test(0.0f, 5.0f, 0.0f));
		ass_eq(s1.capacity(), 4);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.peek().test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 11);
		ass_true(s1.at(0).test(-1.0, -1.0, -1.0) && s1.at(1).test(-1.0, -1.0, -1.0) &&
					s1.at(2).test(-1.0, -1.0, -1.0) && s1.at(3).test(-1.0, -1.0, -1.0));

		tf = (s1.push(TestObject(4.0f, 3.0f, 2.0f)) &&
				s1.push(TestObject(2.0f, 1.0f, 0.0f)) &&
				s1.push(TestObject(-3.0f, -4.0f, -5.0f)) &&
				s1.push(TestObject(-9.0f, -10.0f, -11.0f)));
		ass_true(s1.isFull());
		ass_eq(destroyed_count, 15);
		ass_true(s1.at(0).test(4.0f, 3.0f, 2.0f) && s1.at(1).test(2.0f, 1.0f, 0.0f) &&
					s1.at(2).test(-3.0f, -4.0f, -5.0f) && s1.at(3).test(-9.0f, -10.0f, -11.0f));
		ass_true(tf);
		ass_true(!s1.isEmpty());
		tval = s1.pop();
		ass_true(tval.test(4.0f, 3.0f, 2.0f));
		ass_true(!s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.at(0).test(2.0f, 1.0f, 0.0f) && s1.at(1).test(-3.0f, -4.0f, -5.0f) &&
					s1.at(2).test(-9.0f, -10.0f, -11.0f) && s1.at(3).test(-1.0, -1.0, -1.0));
		tval = s1.pop();
		ass_true(tval.test(2.0f, 1.0f, 0.0f));
		ass_true(!s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.at(0).test(-3.0f, -4.0f, -5.0f) && s1.at(1).test(-9.0f, -10.0f, -11.0f) &&
					s1.at(2).test(-1.0, -1.0, -1.0) && s1.at(3).test(-1.0, -1.0, -1.0));
		tval = s1.pop();
		ass_true(tval.test(-3.0f, -4.0f, -5.0f));
		ass_true(!s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.at(0).test(-9.0f, -10.0f, -11.0f) && s1.at(1).test(-1.0, -1.0, -1.0) &&
					s1.at(2).test(-1.0, -1.0, -1.0) && s1.at(3).test(-1.0, -1.0, -1.0));
		tval = s1.pop();
		ass_true(tval.test(-9.0f, -10.0f, -11.0f));
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_true(s1.at(0).test(-1.0, -1.0, -1.0) && s1.at(1).test(-1.0, -1.0, -1.0) &&
					s1.at(2).test(-1.0, -1.0, -1.0) && s1.at(3).test(-1.0, -1.0, -1.0));
		tval = s1.pop();
		ass_false(s1.isFull());
		ass_true(tval.test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 20);
		ass_true(s1.at(0).test(-1.0, -1.0, -1.0) && s1.at(1).test(-1.0, -1.0, -1.0) &&
					s1.at(2).test(-1.0, -1.0, -1.0) && s1.at(3).test(-1.0, -1.0, -1.0));

		FINISH_TEST;
	}

	void testClearSimpleQueue() {
		BEGIN_TEST;

		SimpleQueue<I32> s1(4, -1);
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);

		s1.clear();
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);
		
		s1.push(1);
		s1.push(2);
		s1.push(3);
		s1.push(3);
		ass_eq(s1.at(0), 1); ass_eq(s1.at(1), 2); ass_eq(s1.at(2), 3); ass_eq(s1.at(3), 3);
		ass_true(s1.isFull());
		
		s1.clear();
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);
		
		s1.push(5);
		s1.push(2);
		s1.push(-8);
		ass_false(s1.isFull());
		
		ass_eq(s1.at(0), 5); ass_eq(s1.at(1), 2); ass_eq(s1.at(2), -8); ass_eq(s1.at(3), -1);
		s1.clear();
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);

		s1.push(-99);
		ass_eq(s1.at(0), -99); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_false(s1.isFull());
		s1.clear();
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);


		// TEST OBJECTS

		SimpleQueue<TestObject> s2(4, TestObject(-1.0f, -1.0f, -1.0f));
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		s2.clear();
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));

		destroyed_count = 0;
		
		s2.push(TestObject(1.0f, 2.0f, 3.0f));
		s2.push(TestObject(2.0f, 4.0f, 6.0f));
		s2.push(TestObject(3.0f, 6.0f, 9.0f));
		s2.push(TestObject(3.0f, 6.0f, 9.0f));
		ass_true(s2.at(0).test(1.0f, 2.0f, 3.0f) && s2.at(1).test(2.0f, 4.0f, 6.0f) &&
					s2.at(2).test(3.0f, 6.0f, 9.0f) && s2.at(3).test(3.0f, 6.0f, 9.0f));
		ass_eq(destroyed_count, 4);
		ass_true(s2.isFull());

		s2.clear();
		ass_eq(destroyed_count, 4);	
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));		
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 6);	
			
		
		s2.push(TestObject(5.0f, 10.0f, 15.0f));
		s2.push(TestObject(2.0f, 4.0f, 6.0f));
		s2.push(TestObject(-8.0f, -16.0f, -24.0f));
		ass_true(s2.at(0).test(5.0f, 10.0f, 15.0f) && s2.at(1).test(2.0f, 4.0f, 6.0f) &&
					s2.at(2).test(-8.0f, -16.0f, -24.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_false(s2.isFull());
		ass_eq(destroyed_count, 9);
		s2.clear();
		ass_eq(destroyed_count, 9);
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 11);

		s2.push(TestObject(-99.0f, -98.0f, 1230.0f));
		ass_true(s2.at(0).test(-99.0f, -98.0f, 1230.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_false(s2.isFull());
		ass_eq(destroyed_count, 12);
		s2.clear();
		ass_eq(destroyed_count, 12);
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 14);
		FINISH_TEST;
	}

	void testSimpleQueueRemove() {
		BEGIN_TEST;

		SimpleQueue<I32> s1(4, -1);
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);

		s1.remove();
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);
		
		s1.push(1);
		s1.push(2);
		s1.push(3);
		s1.push(3);
		ass_true(s1.isFull());
		ass_eq(s1.at(0), 1); ass_eq(s1.at(1), 2); ass_eq(s1.at(2), 3); ass_eq(s1.at(3), 3);
		
		s1.remove();
		ass_eq(s1.at(0), 2); ass_eq(s1.at(1), 3); ass_eq(s1.at(2), 3); ass_eq(s1.at(3), -1);
		ass_false(s1.isFull());
		s1.remove();
		ass_eq(s1.at(0), 3); ass_eq(s1.at(1), 3); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_false(s1.isFull());
		s1.remove();
		ass_eq(s1.at(0), 3); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_false(s1.isFull());
		s1.remove();
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);
		
		s1.push(5);
		s1.push(2);
		s1.push(-8);
		ass_false(s1.isFull());
		ass_eq(s1.at(0), 5); ass_eq(s1.at(1), 2); ass_eq(s1.at(2), -8); ass_eq(s1.at(3), -1);
		
		s1.remove();
		ass_eq(s1.at(0), 2); ass_eq(s1.at(1), -8); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_false(s1.isFull());
		s1.remove();
		ass_eq(s1.at(0), -8); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_false(s1.isFull());
		s1.remove();
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);

		s1.push(-99);
		ass_eq(s1.at(0), -99); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_false(s1.isFull());
		s1.remove();
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);

		s1.remove();
		ass_eq(s1.capacity(), 4);
		ass_eq(s1.at(0), -1); ass_eq(s1.at(1), -1); ass_eq(s1.at(2), -1); ass_eq(s1.at(3), -1);
		ass_true(s1.isEmpty());
		ass_false(s1.isFull());
		ass_eq(s1.nullValue(), -1);
		ass_eq(s1.peek(), -1);
		ass_eq(s1.pop(), -1);


		// TEST OBJECTS

		SimpleQueue<TestObject> s2(4, TestObject(-1.0f, -1.0f, -1.0f));
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));

		s2.remove();
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));

		destroyed_count = 0;
		
		s2.push(TestObject(1.0f, 2.0f, 3.0f));
		s2.push(TestObject(2.0f, 4.0f, 6.0f));
		s2.push(TestObject(3.0f, 6.0f, 9.0f));
		s2.push(TestObject(3.0f, 6.0f, 9.0f));
		ass_true(s2.at(0).test(1.0f, 2.0f, 3.0f) && s2.at(1).test(2.0f, 4.0f, 6.0f) &&
					s2.at(2).test(3.0f, 6.0f, 9.0f) && s2.at(3).test(3.0f, 6.0f, 9.0f));
		ass_eq(destroyed_count, 4);
		ass_true(s2.isFull());
		s2.remove();
		ass_true(s2.at(0).test(2.0f, 4.0f, 6.0f) && s2.at(1).test(3.0f, 6.0f, 9.0f) &&
					s2.at(2).test(3.0f, 6.0f, 9.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_false(s2.isFull());
		s2.remove();
		ass_true(s2.at(0).test(3.0f, 6.0f, 9.0f) && s2.at(1).test(3.0f, 6.0f, 9.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_false(s2.isFull());
		s2.remove();
		ass_true(s2.at(0).test(3.0f, 6.0f, 9.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_false(s2.isFull());
		s2.remove();
		ass_eq(destroyed_count, 4);	
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));		
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 6);	
			
		
		s2.push(TestObject(5.0f, 10.0f, 15.0f));
		s2.push(TestObject(2.0f, 4.0f, 6.0f));
		s2.push(TestObject(-8.0f, -16.0f, -24.0f));
		ass_true(s2.at(0).test(5.0f, 10.0f, 15.0f) && s2.at(1).test(2.0f, 4.0f, 6.0f) &&
					s2.at(2).test(-8.0f, -16.0f, -24.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 9);
		ass_false(s2.isFull());
		s2.remove();
		ass_true(s2.at(0).test(2.0f, 4.0f, 6.0f) && s2.at(1).test(-8.0f, -16.0f, -24.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_false(s2.isFull());
		s2.remove();
		ass_true(s2.at(0).test(-8.0f, -16.0f, -24.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_false(s2.isFull());
		s2.remove();
		ass_eq(destroyed_count, 9);
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 11);

		s2.push(TestObject(-99.0f, -98.0f, 1230.0f));
		ass_true(s2.at(0).test(-99.0f, -98.0f, 1230.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 12);
		ass_false(s2.isFull());
		s2.remove();
		ass_eq(destroyed_count, 12);
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 14);

		s2.remove();
		ass_eq(destroyed_count, 14);
		ass_eq(s2.capacity(), 4);
		ass_true(s2.at(0).test(-1.0f, -1.0f, -1.0f) && s2.at(1).test(-1.0f, -1.0f, -1.0f) &&
					s2.at(2).test(-1.0f, -1.0f, -1.0f) && s2.at(3).test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.isEmpty());
		ass_false(s2.isFull());
		ass_true(s2.nullValue().test(-1.0f, -1.0f, -1.0f) );
		ass_true(s2.peek().test(-1.0f, -1.0f, -1.0f));
		ass_true(s2.pop().test(-1.0f, -1.0f, -1.0f));
		ass_eq(destroyed_count, 16);
		FINISH_TEST;
	}

	void testSimpleQueueIteratorInteger() {
		BEGIN_TEST;

		SimpleQueue<I32> sq(4, -1);

		SimpleQueue<I32>::Iterator itr = sq.begin();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

	   sq.push(2);
		itr = sq.begin();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 2);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

		sq.push(3);
		itr = sq.begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val(), 2);
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 3);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

		sq.push(4);
		sq.push(100);
		sq.push(33); /* Fails */
		
		itr = sq.begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val(), 2);
		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val(), 3);
		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val(), 4);
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 100);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 2);

		sq.remove();
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val(), 3);
		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val(), 4);
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 100);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

		sq.remove();
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val(), 4);
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 100);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

		sq.remove();
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 100);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

		sq.remove();
		itr = sq.begin();	
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

		sq.push(43);
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 43);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

		sq.remove();
		itr = sq.begin();	
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);
		
		sq.push(223);
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 223);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

		sq.push(0);
		itr = sq.begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val(), 223);
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), 0);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);

		sq.clear();
		itr = sq.begin();	
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), -1);
		

		FINISH_TEST;
	}


		void testSimpleQueueIteratorObject() {
		BEGIN_TEST;

		SimpleQueue<TestObject> sq(4, TestObject(-1.0f, -1.0f, -1.0f));

		SimpleQueue<TestObject>::Iterator itr = sq.begin();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

	   sq.push(TestObject(2.0f, 4.0f, 6.0f));
		itr = sq.begin();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(2.0f, 4.0f, 6.0f));
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

		sq.push(TestObject(3.0f, 6.0f, -9.0f));
		itr = sq.begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val().test(2.0f, 4.0f, 6.0f));
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(3.0f, 6.0f, -9.0f));
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

		sq.push(TestObject(4.0f, -8.0f, 16.0f));
		sq.push(TestObject(100.0f, 1000.0f, 10000.0f));
		sq.push(TestObject(33.0f, 34.0f, -223.0f)); /* Fails */
		
		itr = sq.begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val().test(2.0f, 4.0f, 6.0f));
		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val().test(3.0f, 6.0f, -9.0f));
		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val().test(4.0f, -8.0f, 16.0f));
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(100.0f, 1000.0f, 10000.0f));
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val(), TestObject(2.0f, 4.0f, 6.0f));

		sq.remove();
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val().test(3.0f, 6.0f, -9.0f));
		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val().test(4.0f, -8.0f, 16.0f));
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(100.0f, 1000.0f, 10000.0f));
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

		sq.remove();
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val().test(4.0f, -8.0f, 16.0f));
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(100.0f, 1000.0f, 10000.0f));
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

		sq.remove();
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(100.0f, 1000.0f, 10000.0f));
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

		sq.remove();
		itr = sq.begin();	
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

		sq.push(TestObject(43.3f, 42.0f, 292.03f));
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(43.3f, 42.0f, 292.03f));
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

		sq.remove();
		itr = sq.begin();	
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));
		
		sq.push(TestObject(223.0f, 3430.0f, -123.34f));
		itr = sq.begin();	
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(223.0f, 3430.0f, -123.34f));
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

		sq.push(TestObject());
		itr = sq.begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_true(itr.val().test(223.0f, 3430.0f, -123.34f));
		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(0.0f, 0.0f, 0.0f));
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));

		sq.clear();
		itr = sq.begin();	
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		ass_true(itr.val().test(-1.0f, -1.0f, -1.0f));
		

		FINISH_TEST;
	}
	
	
	

} // namespace cc

int main(int argc, char** argv) {
	cc::testCreateAndDestroySimpleQueue();
	cc::testCopySimpleQueue();	
	cc::testPushAndPopAndAtForSimpleQueueInteger();
	cc::testPushAndPopAndAtForSimpleQueueObject();
	cc::testClearSimpleQueue();
	cc::testSimpleQueueRemove();
	cc::testSimpleQueueIteratorInteger();
	cc::testSimpleQueueIteratorObject();	
	
	return 0;
}
