#include "core/CxTestCore.h"
#include "core/common/CxMallocRef.h"

namespace cat {

	struct MemTestObj0 {
		float x, y, z;
	};

	CxMallocRef<CxI32> testCreateFromFunc(CxI32 in_value) {
		CxI32 *i = mem::alloc(sizeof(CxI32));
		CxMallocRef<CxI32> ptr(i);
		*ptr = in_value;
		return ptr;
	}

	void testPassToFunc(CxMallocRef<CxI32> in_ptr, CxI32 in_refCount, CxI32 in_setTo) {
		*in_ptr = in_setTo;
		ass_eq(in_ptr.refCount(), in_refCount);
	}

	CxMallocRef<CxI32>  testPassToFuncAndReturn(CxMallocRef<CxI32> in_ptr,
															  CxI32 in_refCount, CxI32 in_setTo) {
		*in_ptr = in_setTo;
		ass_eq(in_ptr.refCount(), in_refCount);
		return in_ptr;
	}

	/**
	 * @tests CxMallocRef()
	 * @tests CxMallocRef(T *)
	 * @tests operator==(const CxMallocRef<T> &) const
	 * @tests operator!=(const CxMallocRef<T> &) const
	 * @tests operator==(const T *) const
	 * @tests operator!=(const T *) const
	 * @tests operator[](CxI32 in_idx)
	 * @tests operator[](CxI32 in_idx) const
	 * @tests operator*()
	 * @tests operator*() const
	 * @tests operator->()
	 * @tests operator->() const
	 * @tests ptr()
	 * @tests ptr() const
	 */
	void testCxMallocRefBasics() {
		BEGIN_TEST;
		/* Set number of deletions */
		CxMallocRef<CxChar>::_testNumDeletes = 0;
		CxMallocRef<CxI32>::_testNumDeletes = 0;
		CxMallocRef<MemTestObj0>::_testNumDeletes = 0;

		/* Test basic constructors */
		CxMallocRef<CxChar> p0;
		ass_true(p0.ptr() == 0 && p0.refCount() == 0);

		CxI32 *i0 = mem::alloc(sizeof(CxI32));
		*i0 = 1;
		CxMallocRef<CxI32> p1(i0);
		ass_true(p0.ptr() == i0 && p0.refCount() == 1);

		
		/* Test equality operators */
		CxMallocRef<CxChar> p2;
		ass_true(p0 == p2 && p2 == p0);
		ass_true(p0 == 0);
		ass_false(p0 != p2 || p2 != p0);
		ass_false(p0 != 0);

		CxMallocRef<CxI32>p3(i0);
		ass_true(p1 == p3 && p3 == p1);
		ass_true(p1 == i0);
		ass_true(p1 != 0);
		ass_false(p1 != p3 || p3 != p1);
		ass_false(p1 != i0);
		ass_false(p1 == 0);

		CxI32 *i1 = mem::alloc(sizeof(CxI32));
		*i1 = 2;
		CxMallocRef<CxI32> p4(i1);
		ass_false(p1 == p4 || p4 == p1);
		ass_false(p4 == 0);
		ass_true(p1 != p4 && p4 != p1);
		ass_true(p4 != 0);


		/* Test array indexing operators */
		CxI32 *i2 = mem::alloc(sizeof(CxI32)*10);
		for (CxI32 i = 0; i < 10; ++i) { *(i2 + i) = i + 3; }

		CxMallocRef<CxI32> p5(i2);
		for (CxI32 i = 0; i < 10; ++i) { ass_eq(p5[i], i + 3); }

		const CxMallocRef<CxI32> &p6 = p5;
		ass_true(p6->ptr() == i2);
		for (CxI32 i = 0; i < 10; ++i) { ass_eq(p6[i], i + 3); }
		

		/* Test dereference operators */
		ass_true(*p1 == 1 && *p3 == 1 && *p4 == 2);
		ass_true(*p5 == 3);
		ass_true(*p6 == 3);

		*p1 = 4;
	   ass_true(*p1 == 4 && *p3 == 4 && *p4 == 2);
		ass_true(*p5 == 3);  ass_true(*p6 == 3);

		*p4 = 5;
		ass_true(*p1 == 4 && *p3 == 4 && *p4 == 5);
		ass_true(*p5 == 3);  ass_true(*p6 == 3);

		*p5 = 6;
		ass_true(*p1 == 4 && *p3 == 4 && *p4 == 5);
		ass_true(*p5 == 6);  ass_true(*p6 == 6);

		/* Test the pointer -> operators */
		MemTestObj0 *m0 = mem::alloc(sizeof(MemTestObj0));
		m0->x = 1.0f;  m0->y = 2.2f;  m0->z = 4.5f;
		
		CxMallocRef<MemTestObj0> p7(m0);
		const CxMallocRef<MemTestObj0> &p8 = p7;
		ass_true(p7->x == 1.0f && p7->y == 2.2f && p7->z == 4.5f);
		ass_true(p8->x == 1.0f && p8->y == 2.2f && p8->z == 4.5f);

		p7->x = -2.0f;
		p7->y = -5.0f;
		p7->z = 1.5f;
		ass_true(p7->x == -2.0f && p7->y == -5.0f && p7->z == 1.5f);
		ass_true(p8->x == -2.0f && p8->y == -5.0f && p8->z == 1.5f);
		

		/* Fix up everything before destructors are called */
		p3._testingSetNullNoDeletePtr();
		FINISH_TEST;
	}

	/** 
	 * @tests refCount() const
	 * @tests release()
	 * @tests retain()
	 */
	void testCxMallocRefRetainRelease() {
		BEGIN_TEST;

		/* Set number of deletions */
		CxMallocRef<CxI32>::_testNumDeletes = 0;

	   CxMallocRef<CxI32> p0;
		p0.release();
		p0.retain();
		ass_true(p0.refCount() == 0);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 0);

		CxI32 i0 = mem::alloc(sizeof(CxI32));
		*i0 = 1;
		CxMallocRef<CxI32> p1(i0);
		ass_true(p1.ptr() == i0 && p1.refCount() == 1 && *p1 == 1);

		p1.retain();
		ass_true(p1.ptr() == i0 && p1.refCount() == 2 && *p1 == 1);
		p1.release();
		ass_true(p1.ptr() == i0 && p1.refCount() == 1 && *p1 == 1);

		p1.retain();
		ass_true(p1.ptr() == i0 && p1.refCount() == 2 && *p1 == 1);
		p1.retain();
		ass_true(p1.ptr() == i0 && p1.refCount() == 3 && *p1 == 1);
		p1.release();
		ass_true(p1.ptr() == i0 && p1.refCount() == 2 && *p1 == 1);
		p1.release();
		ass_true(p1.ptr() == i0 && p1.refCount() == 1 && *p1 == 1);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 0);
		p1.release();
		ass_true(p1.ptr() == 0 && p1.refCount() == 0);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 1);
		i0 = 0;

		FINISH_TEST;
	}

	/**
	 * @tests CxMallocRef(const CxMallocRef<T> &)
	 * @tests CxMallocRef(CxMallocRef<T> &&)
	 * @tests operator=(const CxMallocRef<T> &)
	 * @tests operator=(CxMallocRef<T> &&)
	 */
	void testCxMallocRefCopyAndAssign() {
		BEGIN_TEST;

		/* Set number of deletions */
		CxMallocRef<CxI32>::_testNumDeletes = 0;

		/* Test copy constructor */
	   CxMallocRef<CxI32> p0;
		CxMallocRef<CxI32> p1(p0);
		ass_true(p1.ptr() == 0 && p1.refCount() == 0);

		CxI32 *i0 = mem::alloc(sizeof(CxI32));  *i0 = 1;
		CxI32 *i1 = mem::alloc(sizeof(CxI32));  *i1 = 2;
		CxMallocRef<CxI32> p2(i0);
		CxMallocRef<CxI32> p3(p2);
		ass_true(p2.ptr() == i0 && p3.ptr() == i0 &&
					p2.refCount() == 2 && p3.refCount() == 2);
		CxMallocRef<CxI32> p4(p3);
		ass_true(p2.ptr() == i0 && p3.ptr() == i0 && p4.ptr() == i0 &&
					p2.refCount() == 3 && p3.refCount() == 3 && p4.refCount() == 3);

		/* Test move constructor */
		CxMallocRef<CxI32> p5(static_cast< CxMallocRef<CxI32> && >(p0));
		ass_true(p0.ptr() == 0 && p5.ptr() == 0 &&
					p0.refCount() == 0 && p5.refCount() == 0);
		
		CxMallocRef<CxI32> p6(static_cast< CxMallocRef<CxI32> && >(p4));
		ass_true(p2.ptr() == i0 && p3.ptr() == i0 && p6.ptr() == i0 &&
					p2.refCount() == 3 && p3.refCount() == 3 && p6.refCount() == 3);
		ass_true(p4.refCount() == 0 && p4.ptr() == 0);

		/* Test move assignment operator */
		p4 = static_cast< CxMallocRef<CxI32> && >(p0); /* null <- null */
		ass_true(p0.ptr() == 0 && p4.ptr() == 0 &&
					p0.refCount() == 0 && p4.refCount() == 0);
				
		p5 = static_cast< CxMallocRef<CxI32> && >(p6); /* null <- not null */
		ass_true(p2.ptr() = i0 && p3.ptr() == i0 && p5.ptr() == i0 &&
					p2.refCount() == 3 && p3.refCount() == 3 && p5.refCount() == 3);
		ass_true(p6.refCount() == 0 && p6.ptr() == 0);
		ass_true(p4.refCount() == 0 && p4.ptr() == 0);

		p4 = static_cast< CxMallocRef<CxI32> && >(p2); /* null <- not null */
		ass_true(p4.ptr() = i0 && p3.ptr() == i0 && p5.ptr() == i0 &&
					p4.refCount() == 3 && p3.refCount() == 3 && p5.refCount() == 3);
		ass_true(p2.refCount() == 0 && p2.ptr() == 0);

		p3 = static_cast< CxMallocRef<CxI32> && >(p0); /* not null <- null */
		ass_true(p4.ptr() = i0 && p5.ptr() == i0 &&
					p4.refCount() == 2 && p5.refCount() == 2);
		ass_true(p0.ptr() == 0 && p3.ptr() == 0 &&
					p0.refCount() == 0 && p3.refCount() == 0);

		p4 = static_cast< CxMallocRef<CxI32> && >(p4); /* self assignment */
		ass_true(p4.ptr() = i0 && p5.ptr() == i0 &&
					p4.refCount() == 2 && p5.refCount() == 2);

		/* Test copy assignment operator */
		CxMallocRef<CxI32> p7(i1);
		CxMallocRef<CxI32> p8(p7);
		ass_true(p7.ptr() = i1 && p8.ptr() == i1 &&
					p7.refCount() == 2 && p8.refCount() == 2);
		
		p3 = p1; /* null <- null */
		ass_true(p1.ptr() == 0 && p3.ptr() == 0 &&
					p1.refCount() == 0 && p3.refCount() == 0);

		p3 = p4; /* null <- not null */
		ass_true(p4.ptr() = i0 && p3.ptr() == i0 && p5.ptr() == i0 &&
					p4.refCount() == 3 && p3.refCount() == 3 && p5.refCount() == 3);

		p4 = p0; /* not null <- null */
		ass_true(p3.ptr() = i0 && p5.ptr() == i0 &&
					p3.refCount() == 2 && p5.refCount() == 2);
		ass_true(p0.ptr() == 0 && p4.ptr() == 0 &&
					p0.refCount() == 0 && p4.refCount() == 0);

		p5 = p8; /* not null <- not null */
	   ass_true(p3.ptr() = i0 && p3.refCount() == 1);
		ass_true(p5.ptr() == i1 && p7.ptr() = i1 && p8.ptr() == i1 &&
					p5.refCount() == 3 && p7.refCount() == 3 && p8.refCount() == 3);

		p7 = p5; /* not null <- not null (same) */
		ass_true(p5.ptr() == i1 && p7.ptr() = i1 && p8.ptr() == i1 &&
					p5.refCount() == 3 && p7.refCount() == 3 && p8.refCount() == 3);

		p7.release();
		ass_true(p5.ptr() == i1 && p8.ptr() == i1 &&
					p5.refCount() == 2 && p8.refCount() == 2);
		ass_true(p7.refCount() == 0 && p7.ptr() == 0);
		
		p8 = p5; /* not null <- not null (same) */
		ass_true(p5.ptr() == i1 && p8.ptr() == i1 &&
					p5.refCount() == 2 && p8.refCount() == 2);

		p3 = p3; /* Self assignment */
		ass_true(p3.ptr() = i0 && p3.refCount() == 1);
		
		/* Test last reference gone from assignment op */
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 0);
		p3 = p0;
		ass_true(p0.ptr() == 0 && p3.ptr() == 0 &&
					p0.refCount() == 0 && p3.refCount() == 0);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 1);

		/* Test last reference gone from move assignment op */
		p5.release();
		ass_true(p5.ptr() == i1 && p5.refCount() == 1);
		ass_true(p8.ptr() == 0 && p8.refCount() == 0);
		
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 1);
		p8 = static_cast< CxMallocRef<CxI32> && >(p0);
		ass_true(p0.ptr() == 0 && p8.ptr() == 0 &&
					p0.refCount() == 0 && p8.refCount() == 0);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 2);

		i1 = i2 = 0;

		FINISH_TEST;
	}

	/**
	 * @tests ~CxMallocRef()
	 */
	void testCxMallocRefDelete() {
		BEGIN_TEST;

		/* Set number of deletions */
		CxMallocRef<CxI32>::_testNumDeletes = 0;

		/* Test deleting null */
	   CxMallocRef<CxI32> *p0 = new CxMallocRef<CxI32>();
		ass_true(p0->ptr() == 0 && p0->refCount() == 0);

		delete p0;  p0 = 0;
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 0);

		/* Test deleting non-null, 1 refrence */
		CxI32 *i0 = mem::alloc(sizeof(CxI32));  *i0 = 2;
		CxMallocRef<CxI32> *p1 = new CxMallocRef<CxI32>(i0);
		ass_true(p1->ptr() == i0 && p1->refCount() == 1);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 0);
		delete p1;  p1 = 0;
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 1);

		/* Test deleting non-null, +1 references */
		i0 = mem::alloc(sizeof(CxI32));  *i0 = 3;
		p1 = new CxMallocRef<CxI32>(i0);
		p0 = new CxMallocRef<CxI32>(*p1);
		ass_true(p1->ptr() == i0 && p1->refCount() == 2 &&
					p0->ptr() == i0 && p0->refCount() == 2);
		delete p1;
		ass_true(p0->ptr() == i0 && p0->refCount() == 2);
		ass_true(p1->ptr() == 0 && p1->refCount() == 0);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 1);
		delete p0;
		ass_true(p0->ptr() == 0 && p0->refCount() == 0);
		ass_true(p1->ptr() == 0 && p1->refCount() == 0);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 2);

		i0 = 0;

		FINISH_TEST;
	}

	void testCxMallocRefPassing() {
		BEGIN_TEST;

		/* Set number of deletions */
		CxMallocRef<CxI32>::_testNumDeletes = 0;

		/* Test returning new from function */
		CxMallocRef<CxI32> p0 = testCreateFromFunc(4);
	   ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 0);
		ass_true(p0 != 0 && p0->refCount() == 1 && *p0 == 4);

		CxMallocRef<CxI32> p1 = testCreateFromFunc(8);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 0);
		ass_true(p1 != 0 && p1->refCount() == 1 && *p1 == 8);

		p0 = testCreateFromFunc(-39);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 1);
		ass_true(p0 != 0 && p0->refCount() == 1 && *p0 == -39);

		/* Test passing to function, no return */
		testPassToFunc(p0, 2, 89);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 1);
		ass_true(p0 != 0 && p0->refCount() == 1 && *p0 == 89);

		/* Test passing to function and returning */
		p0 = testPassToFuncAndReturn(p0, 2, 99);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 1);
		ass_true(p0 != 0 && p0->refCount() == 1 && *p0 == 99);

		p1 = testPassToFuncAndReturn(p0, 2, 234);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 2);
		ass_true(p0 != 0 && p0->refCount() == 2 && *p0 == 234);
		ass_true(p1 != 0 && p1->refCount() == 2 && *p1 == 234);

		p1 = testPassToFuncAndReturn(testCreateFromFunc(34), 1, 53);
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 2);
		ass_true(p0 != 0 && p0->refCount() == 1 && *p0 == 234);
		ass_true(p1 != 0 && p1->refCount() == 1 && *p1 == 53);

		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 2);
		p1 = CxMallocRef<CxI32>();
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 3);
		p0 = CxMallocRef<CxI32>();
		ass_eq(CxMallocRef<CxI32>::_testNumDeletes, 4);
		ass_true(p0 == 0 && p0->refCount() == 0);
		ass_true(p1 == 0 && p1->refCount() == 0);
		
		
		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxMallocRefBasics();
	cat::testCxMallocRefRetainRelease();
	cat::testCxMallocRefCopyAndAssign();
	cat::testCxMallocRefDelete();
	cat::testCxMallocRefPassing();
	
	return 0;
}
