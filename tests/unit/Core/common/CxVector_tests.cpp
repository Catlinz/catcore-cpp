#include "core/CxTestCore.h"
#include "core/common/CxVector.h"

namespace cat {

	class TestObject {
	  public:
		TestObject() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
		TestObject(CxF32 x, CxF32 y, CxF32 z) : m_x(x), m_y(y), m_z(z) {}
		inline CxBool operator==(const TestObject &o) const { return (m_x == o.m_x) && (m_y == o.m_y) && (m_z == o.m_z); }
		inline CxBool operator!=(const TestObject &o) const { return (m_x != o.m_x) || (m_y != o.m_y) || (m_z != o.m_z); }
		CxBool test(CxF32 x, CxF32 y, CxF32 z) { return m_x == x && m_y == y && m_z == z; }
		CxF32 m_x, m_y, m_z;
	};

	template <typename T>
	void testAccessAndSet(CxVector<T> &v, CxI32 idx, T& val, T& new_val) {
		ass_eq( v[idx], val );
		ass_eq( v.at(idx), val );
		ass_eq( v.value(idx), val);

		v[idx] = new_val;
	   ass_eq( v[idx], new_val );
		ass_eq( v.at(idx), new_val );
		ass_eq( v.value(idx), new_val);
	}

	template <typename T>
	void testConstAccess(const CxVector<T> &v, CxI32 idx, const T& val) {
		ass_eq( v[idx], val );
		ass_eq( v.at(idx), val );
		ass_eq( v.value(idx), val);
	}
	

	CxI32 refTest(CxVector<CxI32>& v, CxI32 idx, CxI32 add) {
		v[idx] += add;
		return v[idx];
	}

	CxVector<CxI32> refTest2(CxVector<CxI32>& v, CxI32 value) {
		v.append(value);
		return v;
	}

	/** 
	 * @tests CxVector()
	 * @tests CxVector(CxI32)
	 * @tests CxVector(CxI32,const T &)
	 * @tests CxVector(T *,CxI32,CxCopy)
	 * @tests CxVector(const CxVector<T> &)
	 * @tests ~CxVector()
	 * @tests operator=(const CxVector<T> &)
	 */
	void testCxVectorCreateAndDestroy() {
		BEGIN_TEST;

		CxVector<CxI32> null_v;
		ass_zero(null_v.size());
		ass_zero(null_v.capacity());
	
		/* Test vector of integers */
		CxVector<CxI32> iv(30);
		ass_eq( iv.size(), 30 );
	   ass_eq( iv.capacity(), 30 );
		iv = CxVector<CxI32>(32);
		ass_eq( iv.size(), 32 );
		ass_eq( iv.capacity(), 32 );
		iv = CxVector<CxI32>();
		ass_zero( iv.size());
		ass_zero( iv.capacity());

		iv = CxVector<CxI32>(3, 99);
		ass_eq( iv.size(), 3 );
		ass_eq( iv.capacity(), 3 );
		ass_eq( iv[0], 99 );
		ass_eq( iv[1], 99 );
		ass_eq( iv[2], 99 );

		/* Test array constructor */
	   CxI32 i_arr[4] = {1, 2, 3, 4 };
		iv = CxVector<CxI32>(i_arr, 4);
		ass_eq( iv.size(), 4 );
		ass_eq( iv.capacity(), 4 );
		ass_eq( iv[0], 1 );
		ass_eq( iv[1], 2 );
		ass_eq( iv[2], 3 );
		ass_eq( iv[3], 4 );
		ass_neq( iv.data(), &(i_arr[0]));
		iv[0] = 44;
		ass_eq( iv[0], 44 );
		ass_eq( i_arr[0], 1 );

		iv = CxVector<CxI32>(i_arr, 4, kCxNoCopy);
		ass_eq( iv.size(), 4 );
		ass_eq( iv.capacity(), 4 );
		ass_eq( iv[0], 1 );
		ass_eq( iv[1], 2 );
		ass_eq( iv[2], 3 );
		ass_eq( iv[3], 4 );
		ass_eq( iv.data(), &(i_arr[0]));
		iv[0] = 44;
		ass_eq( iv[0], 44 );
		ass_eq( i_arr[0], 44 );

		/* Test vector of objects */
		CxVector<TestObject> ov;
		ass_eq( ov.length(), 0 );
		ass_eq( ov.capacity(), 0 );
		ov = CxVector<TestObject>(10);
		ass_eq( ov.size(), 10 );
		ass_eq( ov.capacity(), 10 );

		/* Test Vector of pointers */
		CxVector<TestObject*> ovp;
		ass_eq( ovp.length(), 0 );
		ass_eq( ovp.capacity(), 0 );
		ovp = CxVector<TestObject*>(10);
		ass_eq( ovp.size(), 10 );
		ass_eq( ovp.capacity(), 10 );

		FINISH_TEST;
	}

	
	/** 
	 * @tests operator==(const CxVector<T> &) const
	 * @tests operator!=(const CxVector<T> &) const
	 */
	void testCxVectorEquality() {
		BEGIN_TEST;

		/* Simple integer tests */
		CxVector<CxI32> v0(4, 99);
		ass_true( v0 == v0 );
		ass_false( v0 != v0 );
		
		CxVector<CxI32> v1(4, 99);
		ass_true( v0 == v1 );
		ass_false( v0 != v1 );
		ass_true( v1 == v0 );
		ass_false( v1 != v0 );

		v1 = CxVector<CxI32>(4, -99);
		ass_true( v0 != v1 );
		ass_false( v0 == v1 );
		ass_true( v1 != v0 );
		ass_false( v1 == v0 );

		v1 = CxVector<CxI32>(3, 99);
		ass_true( v0 != v1 );
		ass_false( v0 == v1 );
		ass_true( v1 != v0 );
		ass_false( v1 == v0 );

		v1 = CxVector<CxI32>(5, 99);
		ass_true( v0 != v1 );
		ass_false( v0 == v1 );
		ass_true( v1 != v0 );
		ass_false( v1 == v0 );

		v1 = v0;
		ass_true( v0 == v1 );
		ass_false( v0 != v1 );
		ass_true( v1 == v0 );
		ass_false( v1 != v0 );

		/* Test with object vectors. */
		CxVector<TestObject> v2(4, TestObject(1.0f, 2.0f, -3.0f));
		ass_true( v2 == v2 );
		ass_false( v2 != v2 );
		
		CxVector<TestObject> v3(4, TestObject(1.0f, 2.0f, -3.0f));
		ass_true( v2 == v3 );
		ass_false( v2 != v3 );
		ass_true( v3 == v2 );
		ass_false( v3 != v2 );

		v3 = CxVector<TestObject>(4, TestObject(1.0f, 2.0f, 3.0f));
		ass_true( v2 != v3 );
		ass_false( v2 == v3 );
		ass_true( v3 != v2 );
		ass_false( v3 == v2 );

		v3 = CxVector<TestObject>(3, TestObject(1.0f, 2.0f, -3.0f));
		ass_true( v2 != v3 );
		ass_false( v2 == v3 );
		ass_true( v3 != v2 );
		ass_false( v3 == v2 );

		v3 = CxVector<TestObject>(5, TestObject(1.0f, 2.0f, -3.0f));
		ass_true( v2 != v3 );
		ass_false( v2 == v3 );
		ass_true( v3 != v2 );
		ass_false( v3 == v2 );

		v3 = v2;
		ass_true( v2 == v3 );
		ass_false( v2 != v3 );
		ass_true( v3 == v2 );
		ass_false( v3 != v2 );
		
		FINISH_TEST;
	}

	/** 
	 * @tests operator[](CxI32)
	 * @tests operator[](const CxI32) const
	 * @tests at(CxI32) const
	 * @tests value(CxI32) const
	 */
	void testCxVectorAccess() {
		BEGIN_TEST;

		/* Test simple integer types */
		CxVector<I32> v0(3, 0);
		testConstAccess(v0, 0, 0);
		testConstAccess(v0, 1, 0);
		testConstAccess(v0, 2, 0);

		testAccessAndSet(v0, 1, 0, 2);
		testConstAccess(v0, 0, 0);
		testConstAccess(v0, 1, 2);
		testConstAccess(v0, 2, 0);

		testAccessAndSet(v0, 0, 0, -2);
		testConstAccess(v0, 0, -2);
		testConstAccess(v0, 1, 2);
		testConstAccess(v0, 2, 0);

		testAccessAndSet(v0, 2, 0, 123);
		testConstAccess(v0, 0, -2);
		testConstAccess(v0, 1, 2);
		testConstAccess(v0, 2, 123);

		/* Test Object vector types */
		CxVector<TestObject> v1(3, TestObject(0, 0, 0));
		testConstAccess(v1, 0, TestObject(0, 0, 0));
		testConstAccess(v1, 1, TestObject(0, 0, 0));
		testConstAccess(v1, 2, TestObject(0, 0, 0));

		testAccessAndSet(v1, 1, TestObject(0, 0, 0), TestObject(2, 1, 3));
		testConstAccess(v1, 0, TestObject(0, 0, 0));
		testConstAccess(v1, 1, TestObject(2, 1, 3));
		testConstAccess(v1, 2, TestObject(0, 0, 0));

		testAccessAndSet(v1, 0, TestObject(0, 0, 0), TestObject(8, -3, -2));
		testConstAccess(v1, 0, TestObject(8, -3, -2));
		testConstAccess(v1, 1, TestObject(2, 1, 3));
		testConstAccess(v1, 2, TestObject(0, 0, 0));

		testAccessAndSet(v1, 2, TestObject(0, 0, 0), TestObject(1, -1, -1));
		testConstAccess(v1, 0, TestObject(8, -3, -2));
		testConstAccess(v1, 1, TestObject(2, 1, 3));
		testConstAccess(v1, 2, TestObject(1, -1, -1));
		
		FINISH_TEST;
	}

	/**
	 * @tests operator+(const CxVector<T> &) const
	 * @tests operator+=(const CxVector<T> &)
	 * @tests operator<<(const CxVector<T> &)
	 * @tests append(const CxVector<T> &)
	 */
	void testCxVectorConcatenate() {
		BEGIN_TEST;

		/* First, test concatentation empty + nonempty. */
		CxVector<I32> v0;
		CxVector<I32> v1(4, 10);
		ass_true( v0.size() == 0 && v0.capacity() == 0 );
		ass_true( v1.size() == 0 && v1.capacity() == 4 );
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );

		CxVector<I32> v2 = v0 + v1;
		ass_true( v0.size() == 0 && v0.capacity() == 0 );
		ass_true( v1.size() == 4 && v1.capacity() == 4 );
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_eq( v2, v1 );

		v2[0] = 100;
		ass_neq( v2, v1 );
		ass_true( v2[0] == 100 && v1[0] == 10 );

		v2 = v0;
		ass_true( v2.size() == 0 && v2.capacity() == 0 );
		v2 += v1;
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_eq( v2, v1 );

		v2 = v0; v2 << v1;
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_eq( v2, v1 );

		v2 = v0; v2.append(v1);
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_eq( v2, v1 );
		
		v2[0] = 100;
		ass_neq( v2, v1 );
		ass_true( v2[0] == 100 && v1[0] == 10 );
		
		/* Second, test concatentation nonempty + empty. */
		v2 = CxVector<I32>();
		ass_true( v2.size() == 0 && v2.capacity() == 0 );

		v2 = v1 + v0;
		ass_true( v0.size() == 0 && v0.capacity() == 0 );
		ass_true( v1.size() == 4 && v1.capacity() == 4 );
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_eq( v2, v1 );

		v2 = v0;
		ass_true( v2.size() 0 && == v2.capacity() == 0 );
		v2 = v1;
		ass_eq( v2, v1 );

		v2 += v0; ass_eq(v2, v1);
		v2 << v0; ass_eq(v2, v1);
		v2.append(v0); ass_eq(v2, v1);

		/* Third, test nonempty + nonempty */
		v0 = CxVector<I32>(2, -1);
		v2 = CxVector<I32>();

		v2 = v0 + v1;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_neq( v2, v1 ); ass_neq( v2, v0 );
		ass_true( v2[0] == -1 && v2[1] == - 1 );
		ass_true( v2[2] == 10 && v2[3] == 10 && v2[4] == 10 && v2[5] == 10 );

		v2 = v1 + v0;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_neq( v2, v1 ); ass_neq( v2, v0 );
		ass_true( v2[0] == 10 && v2[1] == 10 && v2[2] == 10 && v2[3] == 10 );
		ass_true( v2[4] == -1 && v2[5] == - 1 );

		v2 = v1; ass_eq( v2, v1 );
		v2 += v0;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_neq( v2, v1 ); ass_neq( v2, v0 );
		ass_true( v2[0] == 10 && v2[1] == 10 && v2[2] == 10 && v2[3] == 10 );
		ass_true( v2[4] == -1 && v2[5] == - 1 );

		v2 = v1; ass_eq( v2, v1 );
		v2 << v0;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_neq( v2, v1 ); ass_neq( v2, v0 );
		ass_true( v2[0] == 10 && v2[1] == 10 && v2[2] == 10 && v2[3] == 10 );
		ass_true( v2[4] == -1 && v2[5] == - 1 );

		v2 = v1; ass_eq( v2, v1 );
		v2.append(v0);
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_neq( v2, v1 ); ass_neq( v2, v0 );
		ass_true( v2[0] == 10 && v2[1] == 10 && v2[2] == 10 && v2[3] == 10 );
		ass_true( v2[4] == -1 && v2[5] == - 1 );

		v2 = v0; ass_eq( v2, v0 );
		v2 += v1;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_neq( v2, v1 ); ass_neq( v2, v0 );
		ass_true( v2[0] == -1 && v2[1] == - 1 );
		ass_true( v2[2] == 10 && v2[3] == 10 && v2[4] == 10 && v2[5] == 10 );

		v2 = v0; ass_eq( v2, v0 );
		v2 << v1;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_neq( v2, v1 ); ass_neq( v2, v0 );
		ass_true( v2[0] == -1 && v2[1] == - 1 );
		ass_true( v2[2] == 10 && v2[3] == 10 && v2[4] == 10 && v2[5] == 10 );

		v2 = v0; ass_eq( v2, v0 );
		v2.append(v1);
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_neq( v2, v1 ); ass_neq( v2, v0 );
		ass_true( v2[0] == -1 && v2[1] == - 1 );
		ass_true( v2[2] == 10 && v2[3] == 10 && v2[4] == 10 && v2[5] == 10 );


		/* Test appending two of the same vectors */
		v1[0] = 2;
		CxVector<I32> v3 = v1 + v1;
		ass_neq( v3, v1 );
		ass_eq( v3.size(), v2.size() * 2 );
		ass_true( v3[0] == 2 && v3[1] == - 1 && v3[2] == 2 && v3[3] == - 1 );

		v3 = v1; ass_eq( v3, v1 );
		v3 += v3; ass_neq( v3, v1 );
		ass_eq( v3.size(), v2.size() * 2 );
		ass_true( v3[0] == 2 && v3[1] == - 1 && v3[2] == 2 && v3[3] == - 1 );

		v3 = v1; ass_eq( v3, v1 );
		v3 << v3; ass_neq( v3, v1 );
		ass_eq( v3.size(), v2.size() * 2 );
		ass_true( v3[0] == 2 && v3[1] == - 1 && v3[2] == 2 && v3[3] == - 1 );

		v3 = v1; ass_eq( v3, v1 );
		v3.append(v3); ass_neq( v3, v1 );
		ass_eq( v3.size(), v2.size() * 2 );
		ass_true( v3[0] == 2 && v3[1] == - 1 && v3[2] == 2 && v3[3] == - 1 );

		/* Try concating empty vectors together */
		v1 = CxVector<I32>();
		v2 = CxVector<I32>();

		v3 = v1 + v2;
		ass_eq( v3, v1 ); ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 = v1 + v1;
		ass_eq( v3, v1 );	ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 += v3;
		ass_eq( v3, v1 ); ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 << v3;
		ass_eq( v3, v1 ); ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3.append(v3);
		ass_eq( v3, v1 ); ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		/* Test concating empty vectors with reserved space */
		v1.reserve(20);
		v2.reserve(10);

		ass_true( v1.size() == 0 && v1.capacity() == 20 );
		ass_true( v2.size() == 0 && v2.capacity() == 10 );

		v3 = v1 + v2;
		ass_eq( v3, v1 ); ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 = v1 + v1;
		ass_eq( v3, v1 );	ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 += v1;
		ass_eq( v3, v1 );	ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 << v1;
		ass_eq( v3, v1 );	ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3.append(v1);
		ass_eq( v3, v1 );	ass_eq( v3, v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );
		
		FINISH_TEST;
	}

	void testCxVectorAppend() {
		BEGIN_TEST;

		/* tests: []:X, [1]:X, [...]:X */
		/* First, test with appending an element to an empty list */
		CxVector<I32> v0(3, 0);
		v0.clear();
		assert( v0.size() == 0 && v0.capacity() == 3 );
		
		v0 << 2;
		assert( v0.size() == 1 );
		assert( v0[0] == 2 );

		v0[0] = 0; v0.clear();
		v0 += 2;
		assert( v0.size() == 1 );
		assert( v0[0] == 2 );

		v0[0] = 0; v0.clear();
		v0.append(2);
		assert( v0.size() == 1 );
		assert( v0[0] == 2 );

		/* Test appending to single element list */
		CxVector<I32> v1(1, 0);
		

		FINISH_TEST;
	}

	/**
	 * @tests capacity() const
	 * @tests size() const
	 * @tests length() const
	 */
	void testCxVectorCapacity() {
		BEGIN_TEST;

		CxVector<I32> v0;
		ass_true( v0.size() == 0 && v0.length() == 0 );
		ass_zero( v0.capacity() );

		v0 = CxVector<I32>(1);
		ass_true( v0.size() == 0 && v0.length() == 1 );
		ass_eq( v0.capacity(), 1);

		v0.reserve(4);
		ass_true( v0.size() == 0 && v0.length() == 1 );
		ass_eq( v0.capacity(), 4);

		v0.reserve(2);
		ass_true( v0.size() == 0 && v0.length() == 1 );
		ass_eq( v0.capacity(), 4);

		v0.resize(4);
		ass_true( v0.size() == 0 && v0.length() == 4 );
		ass_eq( v0.capacity(), 4);

		v0.resize(10);
		ass_true( v0.size() == 0 && v0.length() == 10 );
		ass_eq( v0.capacity(), 10);
		
		FINISH_TEST;
	}
	

	void testCxVectorSet() {
		BEGIN_TEST;

		CxVector<CxI32> iv(2);
		assert( iv.length() == 0 && iv.capacity() == 2);
		iv.set(0, 1);
		assert( iv.length() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.set(1, 2);
		assert( iv.length() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.set(2, 69);
		iv.set(3, 30);
		assert( iv.length() == 4 && iv.capacity() == 4 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		iv.set(9, 111);
		ass_eq( iv.length(), 10);
		ass_eq( iv.capacity(), 18);		
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 && iv.at(9) == 111);

		CxVector<TestObject> ov(2);
		TestObject obj;
		assert( ov.length() == 0 && ov.capacity() == 2);
		ov.set(0, obj);
		assert( ov.length() == 1 && ov.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.set(1, obj);
		assert( ov.length() == 2 && ov.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.set(2, obj);
		ov.set(3, obj);
		assert( ov.length() == 4 && ov.capacity() == 4);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );
		obj = TestObject(1.0f, 1.0f, 1.0f);
		ov.set(9, obj);
		ass_eq( ov.length(), 10);
		ass_eq( ov.capacity(), 18);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) && ov.at(9).test(1.0f, 1.0f, 1.0f) );

		
		CxVector<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.length() == 0 && ovp.capacity() == 2);
		ovp.set(0, ptr1);
		assert( ovp.length() == 1 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.set(1, ptr2);
		assert( ovp.length() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.set(2, ptr3);
		ovp.set(3, ptr3);
		assert( ovp.length() == 4 && ovp.capacity() == 4);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );

		ovp.set(9, ptr1);
		ass_eq( ovp.length(), 10);
		ass_eq( ovp.capacity(), 18);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) && ovp.at(9)->test(0.0f, 0.0f, 0.0f) );

		delete ptr1;
		delete ptr2;
		delete ptr3;

		FINISH_TEST;
	}


} // namespace cat

int main(int argc, char **argv) {
	cat::testCxVectorCreateAndDestroy();
	cat::testCxVectorEquality();
	cat::testCxVectorAccess();
	cat::testCxVectorConcatenate();
	cat::testCxVectorCapacity();
	cat::testCxVectorSet();
	return 0;
}
