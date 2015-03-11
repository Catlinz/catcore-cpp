#include "core/testcore.h"
#include "core/util/array.h"

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


	void testArrayCreateAndDestroy() {
		BEGIN_TEST;

		Array<I32> ia(30);
		ass_len(ia,30);
		ass_false(ia.isNull());
		
		ia = Array<I32>(32);
		ass_len(ia,32);
		ass_false(ia.isNull());

		Array<TestObject> oa;
		ass_len(oa,0);
		ass_true(oa.isNull());

		ia.destroy();
		ass_len(ia, 0);	
		ass_true(ia.isNull());

		/* Test creating from existing data */
		I32* arr = new I32[45];
		arr[4] = 45;
		arr[3] = 33;
		Array<I32> ea(45, arr);
		ass_len(ea, 45);
		ass_false(ea.isNull());		
		ass_eq(ea.at(4), 45);
		ass_eq(ea.at(3), 33);

		ea.destroy();
		ass_len(ea, 0);
		ass_true(ea.isNull());		
		

		FINISH_TEST;
	}

	void testArrayAtAndSet() {
		BEGIN_TEST;
		
		Array<I32> ia(32);
		ass_len(ia, 32);
		ass_false(ia.isNull());
		ia.at(0) = 1;
		ia.set(1, 2);
		ia.set(2, 69);
		ia.at(3) = 30;
		ass_eq(ia.at(0), 1); ass_eq(ia.at(1), 2); ass_eq(ia.at(2), 69); ass_eq(ia.at(3), 30);
		
		/* Test modifying through at() */
		ia.at(0) = 4;
		ass_eq(ia.at(0), 4);
		ia.at(1) = ia.at(1) * 3;
		ass_eq(ia.at(1), 6);
		ia.at(2) = ia.at(1) * ia.at(0);
		ass_eq(ia.at(0), 4); ass_eq(ia.at(1), 6); ass_eq(ia.at(2), 24); ass_eq(ia.at(3), 30);
		ia.at(3)--;
		ass_eq(ia.at(3), 29);
		ass_len(ia, 32);

		const I32& ci = ia.at(3);
		ass_eq(ci, 29);
		ia.at(3)--;
		ass_eq(ci, 28); ass_eq(ia.at(3), 28);

		/* Testing with objects */
		
		Array<TestObject> oa(10);
		ass_len(oa, 10);
		ass_false(oa.isNull());	
		for (Size i = 0; i < oa.length(); i++) {
			ass_true(oa.at(i).test(0.0f, 0.0f, 0.0f));
		}

		oa.set(1, TestObject(1.0f, 2.0f, 3.0f));
		ass_true(oa.at(1).test(1.0f, 2.0f, 3.0f));
		
		oa.set(2, oa.at(1));
		ass_true(oa.at(1).test(1.0f, 2.0f, 3.0f));
		ass_true(oa.at(2).test(1.0f, 2.0f, 3.0f));

		oa.at(1).m_x = 4.0f;
		ass_true(oa.at(1).test(4.0f, 2.0f, 3.0f));
		ass_true(oa.at(2).test(1.0f, 2.0f, 3.0f));
		
		oa.at(3) = oa.at(1);
		ass_true(oa.at(1).test(4.0f, 2.0f, 3.0f));
		ass_true(oa.at(2).test(1.0f, 2.0f, 3.0f));
		ass_true(oa.at(3).test(4.0f, 2.0f, 3.0f));

		oa.at(1).m_x = 6.0f;
		ass_true(oa.at(1).test(6.0f, 2.0f, 3.0f));
		ass_true(oa.at(2).test(1.0f, 2.0f, 3.0f));
		ass_true(oa.at(3).test(4.0f, 2.0f, 3.0f));
		ass_len(oa, 10);
		ass_false(oa.isNull());		

		oa.destroy();
		ass_len(oa, 0);
		ass_true(oa.isNull());					
		

		FINISH_TEST;		
	}

	void testArrayGetCopy() {
		BEGIN_TEST;

		Array<I32> a1(10);
		ass_len(a1, 10);
		ass_false(a1.isNull());
		
		a1.at(1) = 3;
		a1.at(3) = 1;
		a1.at(5) = 392;
		ass_eq(a1.at(1), 3); ass_eq(a1.at(3), 1); ass_eq(a1.at(5), 392);
		
		
		Array<I32> a2 = a1.getCopy();
		ass_len(a2, 10);
		ass_false(a2.isNull());

		ass_eq(a1.at(1), 3); ass_eq(a1.at(3), 1); ass_eq(a1.at(5), 392);
		ass_eq(a2.at(1), 3); ass_eq(a2.at(3), 1); ass_eq(a2.at(5), 392);

		a2.set(1, 1);
		a2.set(5, 40);
		ass_eq(a1.at(1), 3); ass_eq(a1.at(3), 1); ass_eq(a1.at(5), 392);
		ass_eq(a2.at(1), 1); ass_eq(a2.at(3), 1); ass_eq(a2.at(5), 40);

		Array<TestObject> a3(8);
		ass_len(a3, 8);
		ass_false(a3.isNull());
		for (Size i = 0; i < a3.length(); i++) {
			ass_true(a3.at(i).test(0.0f, 0.0f, 0.0f));
		}

		a3.at(0) = TestObject(1.0f, 2.0f, 3.0f);
		ass_true(a3.at(0).test(1.0f, 2.0f, 3.0f));
		a3.set(1, TestObject(2.0f, 3.0f, 4.0f));
		a3.at(2) = TestObject(-2.0f, -3.0f, -4.0f);
		ass_true(a3.at(1).test(2.0f, 3.0f, 4.0f)); ass_true(a3.at(2).test(-2.0f, -3.0f, -4.0f));
		
		Array<TestObject> a4 = a3.getCopy();
		ass_len(a4, 8);
		ass_false(a4.isNull());
		
		ass_true(a3.at(0).test(1.0f, 2.0f, 3.0f)); 
		ass_true(a3.at(1).test(2.0f, 3.0f, 4.0f)); 
		ass_true(a3.at(2).test(-2.0f, -3.0f, -4.0f));
		ass_true(a4.at(0).test(1.0f, 2.0f, 3.0f)); 
		ass_true(a4.at(1).test(2.0f, 3.0f, 4.0f)); 
		ass_true(a4.at(2).test(-2.0f, -3.0f, -4.0f));

		a4.set(1, TestObject(-1.0f, -2.0f, -3.0f));
		a4.at(0) = a3.at(1);	
		ass_true(a3.at(0).test(1.0f, 2.0f, 3.0f)); 
		ass_true(a3.at(1).test(2.0f, 3.0f, 4.0f)); 
		ass_true(a3.at(2).test(-2.0f, -3.0f, -4.0f));
		ass_true(a4.at(0).test(2.0f, 3.0f, 4.0f)); 
		ass_true(a4.at(1).test(-1.0f, -2.0f, -3.0f)); 
		ass_true(a4.at(2).test(-2.0f, -3.0f, -4.0f));
		
		

		FINISH_TEST;
	}
	



} // namespace cc

int main(int argc, char** argv) {
	cc::testArrayCreateAndDestroy();
	cc::testArrayAtAndSet();	
	cc::testArrayGetCopy();	

	return 0;
}

