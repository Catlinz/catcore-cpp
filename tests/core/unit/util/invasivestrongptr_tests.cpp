#include "core/testcore.h"
#include "core/util/invasivestrongptr.h"


#define ass_ref(ref, expected_count)															\
	if ((ref).count() != (expected_count)) {													\
		DERR("ASSERT FAILED: [ " #ref".count() == " #expected_count " ] --->> [ " << (ref).count() << " == " << (expected_count) << " ] is FALSE!"); \
	}																							\
	assert((ref).count() == (expected_count));											


cc::I32 to_destroyed = 0;




namespace cc {

	//typedef InvasiveStrongPtr<TestVirtual> VirtualPtr;

	class TestVirtual {
	  public:

		TestVirtual() : m_retainCount(0) {}
		

		virtual ~TestVirtual() {
			to_destroyed++;
			std::cout << "Destroyed count: " << to_destroyed << std::endl;	
		}

		virtual void meow() = 0;		

		inline void retain() {
			m_retainCount += 1;			
		}
		inline Boolean release() {
			m_retainCount -= 1;		
			if (m_retainCount <= 0) {
				return true;
			}
			return false;
		}

		inline I32 retainCount() const { return m_retainCount; }

	  private:
		I32 m_retainCount;		

	};

	class TestVirtualImpl : public TestVirtual {
	  public:
		TestVirtualImpl() :
			TestVirtual(), m_num(0) {}

		TestVirtualImpl(I32 mm)
			: TestVirtual(), m_num(mm) {}

		void meow() { std::cout << " Meow #" << m_num << std::endl; }

	  private:
		I32 m_num;
	};
	

	typedef InvasiveStrongPtr<TestVirtual> VirtualPtr;
	
	class TestObject {
	  public:
		TestObject() 
			: m_retainCount(0), m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
	  
		TestObject(F32 dx, F32 dy, F32 dz)
			: m_retainCount(0), m_x(dx), m_y(dy), m_z(dz) { }

		~TestObject() {
			to_destroyed++;
			std::cout << "Destroyed count: " << to_destroyed << std::endl;			
		}
				
		Boolean test(F32 dx, F32 dy, F32 dz) {
			return (dx == m_x && dy == m_y && dz == m_z);
		}

		inline F32 x() const { return m_x; }
		inline F32 y() const { return m_y; }
		inline F32 z() const { return m_z; }

		inline void retain() {
			m_retainCount += 1;			
		}
		inline Boolean release() {
			m_retainCount -= 1;		
			if (m_retainCount <= 0) {
				return true;
			}
			return false;
		}

		inline I32 retainCount() const { return m_retainCount; }

		static inline InvasiveStrongPtr<TestObject> create() {
			return InvasiveStrongPtr<TestObject>(new TestObject());
		}

		static inline InvasiveStrongPtr<TestObject> create(F32 x, F32 y, F32 z) {
			return InvasiveStrongPtr<TestObject>(new TestObject(x, y, z));
		}
		
	  private:
		I32 m_retainCount;		
		F32 m_x, m_y, m_z;
		
	};

	typedef InvasiveStrongPtr<TestObject> TestPtr;

	
	I32 testRetainCount(TestPtr ptr) {
		I32 references = ptr.count();
		return references;
	}


	void testInvasiveStrongPtrCreateAndDestroy() {
		BEGIN_TEST;
		to_destroyed = 0;
		
		TestPtr nptr;
		ass_ref(nptr, 0);		
		ass_eq(nptr.ptr(), NIL);
		ass_true(nptr.isNull());
		ass_false(nptr.notNull());				
		
		TestPtr ptr = TestObject::create(1.0f, 9.0f, 100.0f);
		ass_ref(ptr, 1);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_true(ptr->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr).test(1.0f, 9.0f, 100.0f));
		ass_false(ptr == nptr); ass_false(nptr == ptr);
		ass_true(ptr != nptr); ass_true(nptr != ptr);
		

		TestPtr ptr2 = TestPtr(ptr);
		ass_ref(ptr, 2);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_true(ptr->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr).test(1.0f, 9.0f, 100.0f));
		ass_ref(ptr2, 2);
		ass_neq(ptr2.ptr(), NIL);
		ass_false(ptr2.isNull());
		ass_true(ptr2.notNull());
		ass_true(ptr2->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr2).test(1.0f, 9.0f, 100.0f));
		ass_true(ptr == ptr2); ass_true(ptr2 == ptr);
		ass_false(ptr != ptr2); ass_false(ptr2 != ptr);

		ptr2 = TestObject::create();
		ass_ref(ptr, 1);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_true(ptr->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr).test(1.0f, 9.0f, 100.0f));
		ass_ref(ptr2, 1);
		ass_neq(ptr2.ptr(), NIL);
		ass_false(ptr2.isNull());
		ass_true(ptr2.notNull());
		ass_true(ptr2->test(0.0f, 0.0f, 0.0f));
		ass_true((*ptr2).test(0.0f, 0.0f, 0.0f));
		ass_eq(to_destroyed, 0);
		ass_false(ptr == ptr2); ass_false(ptr2 == ptr);
		ass_true(ptr != ptr2); ass_true(ptr2 != ptr);
		
		ptr = ptr2;
		ass_ref(ptr, 2);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_true(ptr->test(0.0f, 0.0f, 0.0f));
		ass_true((*ptr).test(0.0f, 0.0f, 0.0f));
		ass_ref(ptr2, 2);
		ass_neq(ptr2.ptr(), NIL);
		ass_false(ptr2.isNull());
		ass_true(ptr2.notNull());
		ass_true(ptr2->test(0.0f, 0.0f, 0.0f));
		ass_true((*ptr2).test(0.0f, 0.0f, 0.0f));
		ass_eq(to_destroyed, 1);
		ass_true(ptr == ptr2); ass_true(ptr2 == ptr);
		ass_false(ptr != ptr2); ass_false(ptr2 != ptr);
		

		FINISH_TEST;
	}

	void testInvasiveStrongPtrCreateAndDestroyVirtual() {
		BEGIN_TEST;
		to_destroyed = 0;
		
		VirtualPtr nptr;
		ass_ref(nptr, 0);		
		ass_eq(nptr.ptr(), NIL);
		ass_true(nptr.isNull());
		ass_false(nptr.notNull());				
		
		VirtualPtr ptr = VirtualPtr(new TestVirtualImpl());		
		ass_ref(ptr, 1);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ptr->meow();		
		ass_false(ptr == nptr); ass_false(nptr == ptr);
		ass_true(ptr != nptr); ass_true(nptr != ptr);
		

		VirtualPtr ptr2 = VirtualPtr(ptr);		
		ass_ref(ptr, 2);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_ref(ptr2, 2);
		ass_neq(ptr2.ptr(), NIL);
		ass_false(ptr2.isNull());
		ass_true(ptr2.notNull());
		ass_true(ptr == ptr2); ass_true(ptr2 == ptr);
		ass_false(ptr != ptr2); ass_false(ptr2 != ptr);

		ptr2 = VirtualPtr(new TestVirtualImpl(45));		
		ass_ref(ptr, 1);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_ref(ptr2, 1);
		ass_neq(ptr2.ptr(), NIL);
		ass_false(ptr2.isNull());
		ass_true(ptr2.notNull());
		ass_eq(to_destroyed, 0);
		ass_false(ptr == ptr2); ass_false(ptr2 == ptr);
		ass_true(ptr != ptr2); ass_true(ptr2 != ptr);
		ptr2->meow();
		ptr->meow();		
		
		ptr = ptr2;
		ass_ref(ptr, 2);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_ref(ptr2, 2);
		ass_neq(ptr2.ptr(), NIL);
		ass_false(ptr2.isNull());
		ass_true(ptr2.notNull());
		ass_eq(to_destroyed, 1);
		ass_true(ptr == ptr2); ass_true(ptr2 == ptr);
		ass_false(ptr != ptr2); ass_false(ptr2 != ptr);
		

		FINISH_TEST;
	}	

	void testInvasiveStrongPtrMethods() {
		BEGIN_TEST;

		to_destroyed = 0;
		ass_eq( to_destroyed, 0 );		

		TestPtr nptr;
		ass_ref(nptr, 0);		
		ass_eq(nptr.ptr(), NIL);
		ass_true(nptr.isNull());
		ass_false(nptr.notNull());				
		
		TestPtr ptr = TestObject::create(1.0f, 9.0f, 100.0f);
		ass_ref(ptr, 1);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_true(ptr->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr).test(1.0f, 9.0f, 100.0f));
		ass_false(ptr == nptr); ass_false(nptr == ptr);
		ass_true(ptr != nptr); ass_true(nptr != ptr);
		ass_eq(to_destroyed, 0);		
		
		ptr.setNull();
		ass_ref(ptr, 0);		
		ass_eq(ptr.ptr(), NIL);
		ass_true(ptr.isNull());
		ass_false(ptr.notNull());	
		ass_true(ptr == nptr); ass_true(nptr == ptr);
		ass_false(ptr != nptr); ass_false(nptr != ptr);
		ass_eq(to_destroyed, 1);		

		TestPtr ptr2 = TestPtr(ptr);
		ass_ref(ptr2, 0);		
		ass_eq(ptr2.ptr(), NIL);
		ass_true(ptr2.isNull());
		ass_false(ptr2.notNull());	
		ass_true(ptr2 == nptr); ass_true(nptr == ptr2);
		ass_false(ptr2 != nptr); ass_false(nptr != ptr2);

		ptr = TestObject::create(1.0f, 9.0f, 100.0f);
		ptr2 = ptr;		
		ass_ref(ptr, 2);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_true(ptr->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr).test(1.0f, 9.0f, 100.0f));
		ass_ref(ptr2, 2);
		ass_neq(ptr2.ptr(), NIL);
		ass_false(ptr2.isNull());
		ass_true(ptr2.notNull());
		ass_true(ptr2->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr2).test(1.0f, 9.0f, 100.0f));
		ass_true(ptr == ptr2); ass_true(ptr2 == ptr);
		ass_false(ptr != ptr2); ass_false(ptr2 != ptr);
		ass_eq(to_destroyed, 1);		
		
		ptr.setNull();
		ass_ref(ptr, 0);		
		ass_eq(ptr.ptr(), NIL);
		ass_true(ptr.isNull());
		ass_false(ptr.notNull());	
		ass_true(ptr == nptr); ass_true(nptr == ptr);
		ass_false(ptr != nptr); ass_false(nptr != ptr);
		ass_ref(ptr2, 1);
		ass_neq(ptr2.ptr(), NIL);
		ass_false(ptr2.isNull());
		ass_true(ptr2.notNull());
		ass_true(ptr2->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr2).test(1.0f, 9.0f, 100.0f));
		ass_false(ptr == ptr2); ass_false(ptr2 == ptr);
		ass_true(ptr != ptr2); ass_true(ptr2 != ptr);
		ass_eq(to_destroyed, 1);		
		
		ptr2.setNull();
		ass_ref(ptr2, 0);		
		ass_eq(ptr2.ptr(), NIL);
		ass_true(ptr2.isNull());
		ass_false(ptr2.notNull());	
		ass_true(ptr2 == nptr); ass_true(nptr == ptr2);
		ass_false(ptr2 != nptr); ass_false(nptr != ptr2);
		ass_eq(to_destroyed, 2);		
		
		FINISH_TEST;		
		
	}

	void testInvasiveStrongPtrCreateAndDestroyParameterPassing() {
		BEGIN_TEST;

		to_destroyed = 0;
		
		TestPtr nptr;
		ass_ref(nptr, 0);		
		ass_eq(nptr.ptr(), NIL);
		ass_true(nptr.isNull());
		ass_false(nptr.notNull());

		ass_eq(testRetainCount(nptr), 0);
		ass_eq(to_destroyed, 0);
		
		TestPtr ptr = TestObject::create(1.0f, 9.0f, 100.0f);
		ass_ref(ptr, 1);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_true(ptr->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr).test(1.0f, 9.0f, 100.0f));
		ass_false(ptr == nptr); ass_false(nptr == ptr);
		ass_true(ptr != nptr); ass_true(nptr != ptr);

		ass_eq(testRetainCount(ptr), 2);
		ass_ref(ptr, 1);
		ass_eq(to_destroyed, 0);
		
		TestPtr ptr2 = TestPtr(ptr);
		ass_ref(ptr, 2);
		ass_neq(ptr.ptr(), NIL);
		ass_false(ptr.isNull());
		ass_true(ptr.notNull());
		ass_true(ptr->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr).test(1.0f, 9.0f, 100.0f));
		ass_ref(ptr2, 2);
		ass_neq(ptr2.ptr(), NIL);
		ass_false(ptr2.isNull());
		ass_true(ptr2.notNull());
		ass_true(ptr2->test(1.0f, 9.0f, 100.0f));
		ass_true((*ptr2).test(1.0f, 9.0f, 100.0f));
		ass_true(ptr == ptr2); ass_true(ptr2 == ptr);
		ass_false(ptr != ptr2); ass_false(ptr2 != ptr);

		ass_eq(testRetainCount(ptr), 3);
		ass_ref(ptr, 2);
		ass_eq(to_destroyed, 0);

		ptr2.setNull();
		ass_eq(testRetainCount(ptr), 2);
		ass_ref(ptr, 1);		

		ass_eq(to_destroyed, 0);		
		I32 count = testRetainCount(TestObject::create(1.0f, 9.0f, 100.0f));
		ass_eq(count, 1);				
		ass_eq(to_destroyed, 1);
		count = testRetainCount(TestObject::create());
		ass_eq(count, 1);
		ass_eq(to_destroyed, 2);

		ptr = TestPtr::nullPtr();
		ass_eq(to_destroyed, 3);
		ass_ref(ptr, 0);		
		ass_eq(ptr.ptr(), NIL);
		ass_true(ptr.isNull());
		ass_false(ptr.notNull());

		FINISH_TEST;
	}	



} // namespace cc

int main(int argc, char** argv) {
	cc::testInvasiveStrongPtrCreateAndDestroy();
	cc::testInvasiveStrongPtrCreateAndDestroyVirtual();
	cc::testInvasiveStrongPtrMethods();	
	cc::testInvasiveStrongPtrCreateAndDestroyParameterPassing();
	return 0;
}

