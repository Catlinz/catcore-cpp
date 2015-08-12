#include "core/CxTestCore.h"
#include "core/common/CxPODVector.h"

namespace cat {

	class TestObject {
	  public:
		TestObject() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
		TestObject(CxF32 v) : m_x(v), m_y(v), m_z(v) {}
		TestObject(CxF32 x, CxF32 y, CxF32 z) : m_x(x), m_y(y), m_z(z) {}
		inline CxBool operator==(const TestObject &o) const { return (m_x == o.m_x) && (m_y == o.m_y) && (m_z == o.m_z); }
		inline CxBool operator!=(const TestObject &o) const { return (m_x != o.m_x) || (m_y != o.m_y) || (m_z != o.m_z); }
		CxBool test(CxF32 x, CxF32 y, CxF32 z) const { return m_x == x && m_y == y && m_z == z; }
		CxF32 m_x, m_y, m_z;
	};

	std::ostream& operator<<(std::ostream& out, const TestObject &obj) {
		return out << "TestObject(" << obj.m_x << ", " << obj.m_y << ", " << obj.m_z << ")";
	}

	template<typename T>
	void appendToCxPODVectorPtr(typename CxPODVector<T>::Ptr in_ptr, const T &in_val) {
		(*in_ptr) << in_val;
	}

	/** 
	 * @tests CxPODVector()
	 * @tests CxPODVector(CxI32)
	 * @tests CxPODVector(CxI32,const T &)
	 * @tests CxPODVector(T *,CxI32,CxCopy)
	 * @tests CxPODVector(const CxPODVector<T> &)
	 * @tests ~CxPODVector()
	 * @tests operator=(const CxPODVector<T> &)
	 */
	void testCxPODVectorCreateAndDestroy() {
		BEGIN_TEST;

		CxPODVector<CxI32> null_v;
		ass_zero(null_v.size());
		ass_zero(null_v.capacity());
	
		/* Test vector of integers */
		CxPODVector<CxI32> iv(30);
		ass_eq( iv.size(), 30 );
	   ass_eq( iv.capacity(), 30 );
		iv = CxPODVector<CxI32>(32);
		ass_eq( iv.size(), 32 );
		ass_eq( iv.capacity(), 32 );
		iv = CxPODVector<CxI32>();
		ass_zero( iv.size());
		ass_zero( iv.capacity());

		iv = CxPODVector<CxI32>(3, 99);
		ass_eq( iv.size(), 3 );
		ass_eq( iv.capacity(), 3 );
		ass_eq( iv[0], 99 );
		ass_eq( iv[1], 99 );
		ass_eq( iv[2], 99 );

		/* Test array constructor */
		CxI32 *i_arr = new CxI32[4];
		i_arr[0] = 1;  i_arr[1] = 2;  i_arr[2] = 3;  i_arr[3] = 4;
		iv = CxPODVector<CxI32>(i_arr, 4);
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

		CxPODVector<CxI32> ivn(i_arr, 4, kCxNoCopy);
		ass_eq( ivn.size(), 4 );
		ass_eq( ivn.capacity(), 4 );
		ass_eq( ivn[0], 1 );
		ass_eq( ivn[1], 2 );
		ass_eq( ivn[2], 3 );
		ass_eq( ivn[3], 4 );
		ass_eq( ivn.data(), &(i_arr[0]));
		ivn[0] = 44;
		ass_eq( ivn[0], 44 );
		ass_eq( i_arr[0], 44 );

		iv = ivn;
		ass_eq(iv.size(), 4);
		ass_eq(iv.capacity(), 4);
		ass_eq(iv[0], 44);
		ass_eq(iv[1], 2);
		ass_eq(iv[2], 3);
		ass_eq(iv[3], 4);
		ass_neq(iv.data(), &(i_arr[0]));
		iv[0] = 45;
		ass_eq(iv[0], 45);
		ass_eq(i_arr[0], 44);

		/* Test vector of objects */
		CxPODVector<TestObject> ov;
		ass_eq( ov.length(), 0 );
		ass_eq( ov.capacity(), 0 );
		ov = CxPODVector<TestObject>(10);
		ass_eq( ov.size(), 10 );
		ass_eq( ov.capacity(), 10 );

		/* Test Vector of pointers */
		CxPODVector<TestObject*> ovp;
		ass_eq( ovp.length(), 0 );
		ass_eq( ovp.capacity(), 0 );
		ovp = CxPODVector<TestObject*>(10);
		ass_eq( ovp.size(), 10 );
		ass_eq( ovp.capacity(), 10 );

		FINISH_TEST;
	}

	
	/** 
	 * @tests operator==(const CxPODVector<T> &) const
	 * @tests operator!=(const CxPODVector<T> &) const
	 */
	void testCxPODVectorEquality() {
		BEGIN_TEST;

		/* Simple integer tests */
		CxPODVector<CxI32> v0(4, 99);
		ass_true( v0 == v0 );
		ass_false( v0 != v0 );
		
		CxPODVector<CxI32> v1(4, 99);
		ass_true( v0 == v1 );
		ass_false( v0 != v1 );
		ass_true( v1 == v0 );
		ass_false( v1 != v0 );

		v1 = CxPODVector<CxI32>(4, -99);
		ass_true( v0 != v1 );
		ass_false( v0 == v1 );
		ass_true( v1 != v0 );
		ass_false( v1 == v0 );

		v1 = CxPODVector<CxI32>(3, 99);
		ass_true( v0 != v1 );
		ass_false( v0 == v1 );
		ass_true( v1 != v0 );
		ass_false( v1 == v0 );

		v1 = CxPODVector<CxI32>(5, 99);
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
		CxPODVector<TestObject> v2(4, TestObject(1.0f, 2.0f, -3.0f));
		ass_true( v2 == v2 );
		ass_false( v2 != v2 );
		
		CxPODVector<TestObject> v3(4, TestObject(1.0f, 2.0f, -3.0f));
		ass_true( v2 == v3 );
		ass_false( v2 != v3 );
		ass_true( v3 == v2 );
		ass_false( v3 != v2 );

		v3 = CxPODVector<TestObject>(4, TestObject(1.0f, 2.0f, 3.0f));
		ass_true( v2 != v3 );
		ass_false( v2 == v3 );
		ass_true( v3 != v2 );
		ass_false( v3 == v2 );

		v3 = CxPODVector<TestObject>(3, TestObject(1.0f, 2.0f, -3.0f));
		ass_true( v2 != v3 );
		ass_false( v2 == v3 );
		ass_true( v3 != v2 );
		ass_false( v3 == v2 );

		v3 = CxPODVector<TestObject>(5, TestObject(1.0f, 2.0f, -3.0f));
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
	 * @tests operator+(const CxPODVector<T> &) const
	 * @tests operator+=(const CxPODVector<T> &)
	 * @tests operator<<(const CxPODVector<T> &)
	 * @tests append(const CxPODVector<T> &)
	 */
	void testCxPODVectorConcatenate() {
		BEGIN_TEST;

		/* First, test concatentation empty + nonempty. */
		CxPODVector<CxI32> v0;
		CxPODVector<CxI32> v1(4, 10);
		ass_true( v0.size() == 0 && v0.capacity() == 0 );
		ass_true( v1.size() == 4 && v1.capacity() == 4 );
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );

		CxPODVector<CxI32> v2 = v0 + v1;
		ass_true( v0.size() == 0 && v0.capacity() == 0 );
		ass_true( v1.size() == 4 && v1.capacity() == 4 );
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_true( v2 == v1 );

		v2[0] = 100;
		ass_true( v2 != v1 );
		ass_true( v2[0] == 100 && v1[0] == 10 );

		v2 = v0;
		ass_true( v2.size() == 0 && v2.capacity() == 0 );
		v2 += v1;
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_true( v2 == v1 );

		v2 = v0; v2 << v1;
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_true( v2 == v1 );

		v2 = v0; v2.append(v1);
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_true( v2 == v1 );
		
		v2[0] = 100;
		ass_true( v2 != v1 );
		ass_true( v2[0] == 100 && v1[0] == 10 );
		
		/* Second, test concatentation nonempty + empty. */
		v2 = CxPODVector<CxI32>();
		ass_true( v2.size() == 0 && v2.capacity() == 0 );

		v2 = v1 + v0;
		ass_true( v0.size() == 0 && v0.capacity() == 0 );
		ass_true( v1.size() == 4 && v1.capacity() == 4 );
		ass_true( v1[0] == 10 && v1[1] == 10 && v1[2] == 10 && v1[3] == 10 );
		ass_true( v2 == v1 );

		v2 = v0;
		ass_true( v2.size() == 0 && v2.capacity() == 0 );
		v2 = v1;
		ass_true( v2 == v1 );

		v2 += v0; ass_true(v2 == v1);
		v2 << v0; ass_true(v2 == v1);
		v2.append(v0); ass_true(v2 == v1);

		/* Third, test nonempty + nonempty */
		v0 = CxPODVector<CxI32>(2, -1);
		v2 = CxPODVector<CxI32>();

		v2 = v0 + v1;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_true( v2 != v1 ); ass_true( v2 != v0 );
		ass_true( v2[0] == -1 && v2[1] == - 1 );
		ass_true( v2[2] == 10 && v2[3] == 10 && v2[4] == 10 && v2[5] == 10 );

		v2 = v1 + v0;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_true( v2 != v1 ); ass_true( v2 != v0 );
		ass_true( v2[0] == 10 && v2[1] == 10 && v2[2] == 10 && v2[3] == 10 );
		ass_true( v2[4] == -1 && v2[5] == - 1 );

		v2 = v1; ass_true( v2 == v1 );
		v2 += v0;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_true( v2 != v1 ); ass_true( v2 != v0 );
		ass_true( v2[0] == 10 && v2[1] == 10 && v2[2] == 10 && v2[3] == 10 );
		ass_true( v2[4] == -1 && v2[5] == - 1 );

		v2 = v1; ass_true( v2 == v1 );
		v2 << v0;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_true( v2 != v1 ); ass_true( v2 != v0 );
		ass_true( v2[0] == 10 && v2[1] == 10 && v2[2] == 10 && v2[3] == 10 );
		ass_true( v2[4] == -1 && v2[5] == - 1 );

		v2 = v1; ass_true( v2 == v1 );
		v2.append(v0);
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_true( v2 != v1 ); ass_true( v2 != v0 );
		ass_true( v2[0] == 10 && v2[1] == 10 && v2[2] == 10 && v2[3] == 10 );
		ass_true( v2[4] == -1 && v2[5] == - 1 );

		v2 = v0; ass_true( v2 == v0 );
		v2 += v1;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_true( v2 != v1 ); ass_true( v2 != v0 );
		ass_true( v2[0] == -1 && v2[1] == - 1 );
		ass_true( v2[2] == 10 && v2[3] == 10 && v2[4] == 10 && v2[5] == 10 );

		v2 = v0; ass_true( v2 == v0 );
		v2 << v1;
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_true( v2 != v1 ); ass_true( v2 != v0 );
		ass_true( v2[0] == -1 && v2[1] == - 1 );
		ass_true( v2[2] == 10 && v2[3] == 10 && v2[4] == 10 && v2[5] == 10 );

		v2 = v0; ass_true( v2 == v0 );
		v2.append(v1);
		ass_eq( v2.size(), v0.size() + v1.size() );
		ass_true( v2 != v1 ); ass_true( v2 != v0 );
		ass_true( v2[0] == -1 && v2[1] == - 1 );
		ass_true( v2[2] == 10 && v2[3] == 10 && v2[4] == 10 && v2[5] == 10 );


		/* Test appending two of the same vectors */
		v1 = CxPODVector<CxI32>(2, -1);
		v1[0] = 2;
		CxPODVector<CxI32> v3 = v1 + v1;
		ass_true( v3 != v1 );
		ass_eq( v3.size(), v1.size() * 2 );
		ass_true( v3[0] == 2 && v3[1] == -1 && v3[2] == 2 && v3[3] == -1 );

		v3 = v1; ass_true( v3 == v1 );
		v3 += v3; ass_true( v3 != v1 );
		ass_eq( v3.size(), v1.size() * 2 );
		ass_true( v3[0] == 2 && v3[1] == - 1 && v3[2] == 2 && v3[3] == - 1 );

		v3 = v1; ass_true( v3 == v1 );
		v3 << v3; ass_true( v3 != v1 );
		ass_eq( v3.size(), v1.size() * 2 );
		ass_true( v3[0] == 2 && v3[1] == - 1 && v3[2] == 2 && v3[3] == - 1 );

		v3 = v1; ass_true( v3 == v1 );
		v3.append(v3); ass_true( v3 != v1 );
		ass_eq( v3.size(), v1.size() * 2 );
		ass_true( v3[0] == 2 && v3[1] == - 1 && v3[2] == 2 && v3[3] == - 1 );

		/* Try concating empty vectors together */
		v1 = CxPODVector<CxI32>();
		v2 = CxPODVector<CxI32>();

		v3 = v1 + v2;
		ass_true( v3 == v1 ); ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 = v1 + v1;
		ass_true( v3 == v1 );	ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 += v3;
		ass_true( v3 == v1 ); ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 << v3;
		ass_true( v3 == v1 ); ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3.append(v3);
		ass_true( v3 == v1 ); ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		/* Test concating empty vectors with reserved space */
		v1.reserve(20);
		v2.reserve(10);

		ass_true( v1.size() == 0 && v1.capacity() == 20 );
		ass_true( v2.size() == 0 && v2.capacity() == 10 );

		v3 = v1 + v2;
		ass_true( v3 == v1 ); ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 = v1 + v1;
		ass_true( v3 == v1 );	ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 += v1;
		ass_true( v3 == v1 );	ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3 << v1;
		ass_true( v3 == v1 );	ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );

		v3.append(v1);
		ass_true( v3 == v1 );	ass_true( v3 == v2 );
		ass_true( v3.size() == 0 && v3.capacity() == 0 );
		
		FINISH_TEST;
	}

	/**
	 * @tests append(const T &)
	 * @tests operator<<(const T &)
	 * @tests operator+=(const T &)
	 */
	void testCxPODVectorAppend() {
		BEGIN_TEST;

		/* tests: []:X, [1]:X, [...]:X */
		/* First, test with appending an element to an empty list */
		CxPODVector<CxI32> v0;
		ass_true( v0.size() == 0 && v0.capacity() == 0 );
		
		v0 << 2;
		ass_true( v0.size() == 1 && v0.capacity() == 2);
		ass_eq( v0[0], 2 );

		v0[0] = 0; v0.clear();
		v0 += 2;
		ass_eq( v0.size(), 1 );
		ass_eq( v0[0], 2 );

		v0 = CxPODVector<CxI32>(3, 0);
		v0.clear();
		v0.append(2);
		ass_eq( v0.size(), 1 );
		ass_eq( v0[0], 2 );

		/* Test appending to single element list */
		CxPODVector<CxI32> v1(1, 0);
		ass_true( v1.size() == 1);
		ass_eq(v1[0], 0);

		v1 << 2;
		ass_true(v1.size() == 2 && v1.capacity() == 4);
		ass_eq(v1[0], 0);  ass_eq(v1[1], 2);

		v1 = CxPODVector<CxI32>(1, 0);
		ass_true( v1.size() == 1);
		ass_eq(v1[0], 0);

		v1 += 3;
		ass_true(v1.size() == 2 && v1.capacity() == 4);
		ass_eq(v1[0], 0);  ass_eq(v1[1], 3);
		
		v1 = CxPODVector<CxI32>(3, 0);
		v1.clear();
		v1 << 0;
		ass_true( v1.size() == 1);
		ass_eq(v1[0], 0);

		v1.append(4);
		ass_true(v1.size() == 2);
		ass_eq(v1[0], 0);  ass_eq(v1[1], 4);

		/* Test appending to multiple element list */
		CxPODVector<CxI32> v2;
		v2 << 1;  v2 << 2;  v2 << 3;
		ass_eq(v2.size(), 3);
		ass_eq(v2[0], 1);  ass_eq(v2[1], 2);  ass_eq(v2[2], 3);

		v2 << 2;
		ass_eq(v2.size(), 4);
		ass_eq(v2[3], 2);

		v2 = CxPODVector<CxI32>();
		v2 << 1;  v2 << 2;  v2 << 3;
		ass_eq(v2.size(), 3);
		ass_eq(v2[0], 1);  ass_eq(v2[1], 2);  ass_eq(v2[2], 3);

		v2 += 3;
		ass_eq(v2.size(), 4);
		ass_eq(v2[3], 3);

		v2 = CxPODVector<CxI32>();
		v2 << 1;  v2 << 2;  v2 << 3;
		
		v2.append(4);
		ass_eq(v2.size(), 4);
		ass_eq(v2[3], 4);
		
		FINISH_TEST;
	}

	/**
	 * @tests set(CxI32, const T &)
	 * @tests setAll(const T &)
	 */
	void testCxPODVectorSet() {
		BEGIN_TEST;

		CxPODVector<CxI32> iv(2);
		assert( iv.length() == 2 && iv.capacity() == 2);
		iv.set(0, 1);
		assert( iv.length() == 2 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.set(1, 2);
		assert( iv.length() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.set(2, 69);
		iv.set(3, 30);
		assert( iv.length() == 4 && iv.capacity() == 6 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		iv.set(9, 111);
		ass_eq( iv.length(), 10);
		ass_eq( iv.capacity(), 20);		
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 && iv.at(9) == 111);

		iv.setAll(0);
		for (CxI32 i = 0; i < iv.size(); ++i) { assert(iv[i] == 0); }

	   iv = CxPODVector<CxI32>();
		assert( iv.length() == 0 && iv.capacity() == 0);
		iv.set(0, 1);
		assert( iv.length() == 1 && iv.capacity() == 2 && iv.at(0) == 1);
		iv.set(1, 2);
		assert( iv.length() == 2 && iv.capacity() == 2 && iv.at(1) == 2);
		iv.set(2, 69);
		iv.set(3, 30);
		assert( iv.length() == 4 && iv.capacity() == 6 && iv.at(2) == 69 && iv.at(3) == 30);
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 );
		iv.set(9, 111);
		ass_eq( iv.length(), 10);
		ass_eq( iv.capacity(), 20);		
		assert(iv.at(0) == 1 && iv.at(1) == 2 && iv.at(3) == 30 && iv.at(2) == 69 && iv.at(9) == 111);

		iv.setAll(0);
		for (CxI32 i = 0; i < iv.size(); ++i) { ass_eq(iv[i], 0); }

		CxPODVector<TestObject> ov(2);
		TestObject obj;
		assert( ov.length() == 2 && ov.capacity() == 2);
		ov.set(0, obj);
		assert( ov.length() == 2 && ov.capacity() == 2);
		obj = TestObject(1.0f, 2.0f, 3.0f);
		ov.set(1, obj);
		assert( ov.length() == 2 && ov.capacity() == 2);
		obj = TestObject(20.3f, 69.9f, 69.0f);
		ov.set(2, obj);
		ov.set(3, obj);
		assert( ov.length() == 4 && ov.capacity() == 6);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) );
		obj = TestObject(1.0f, 1.0f, 1.0f);
		ov.set(9, obj);
		ass_eq( ov.length(), 10);
		ass_eq( ov.capacity(), 20);
		assert( ov.at(0).test(0.0f, 0.0f, 0.0f) && ov.at(1).test(1.0f, 2.0f, 3.0f) && ov.at(2).test(20.3f, 69.9f, 69.0f) && ov.at(3).test(20.3f, 69.9f, 69.0f) && ov.at(9).test(1.0f, 1.0f, 1.0f) );

		ov.setAll(TestObject(0, 0, 0));
		for (CxI32 i = 0; i < ov.size(); ++i) {
			ass_true(ov[i].test(0, 0, 0));
		}
		
		CxPODVector<TestObject*> ovp(2);
		TestObject* ptr1 = new TestObject();
		assert( ovp.length() == 2 && ovp.capacity() == 2);
		ovp.set(0, ptr1);
		assert( ovp.length() == 2 && ovp.capacity() == 2);
		TestObject* ptr2 = new TestObject(1.0f, 2.0f, 3.0f);
		ovp.set(1, ptr2);
		assert( ovp.length() == 2 && ovp.capacity() == 2);
		TestObject* ptr3 = new TestObject(20.3f, 69.9f, 69.0f);
		ovp.set(2, ptr3);
		ovp.set(3, ptr3);
		assert( ovp.length() == 4 && ovp.capacity() == 6);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) );

		ovp.set(9, ptr1);
		ass_eq( ovp.length(), 10);
		ass_eq( ovp.capacity(), 20);
		assert( ovp.at(0)->test(0.0f, 0.0f, 0.0f) && ovp.at(1)->test(1.0f, 2.0f, 3.0f) && ovp.at(2)->test(20.3f, 69.9f, 69.0f) && ovp.at(3)->test(20.3f, 69.9f, 69.0f) && ovp.at(9)->test(0.0f, 0.0f, 0.0f) );

		ovp.setAll(0);
		for (CxI32 i = 0; i < ov.size(); ++i) { ass_eq(ovp[i], 0); }

		delete ptr1;
		delete ptr2;
		delete ptr3;

		FINISH_TEST;
	}

	/**
	 * @tests compact()
	 */
	void testCxPODVectorCompact() {
		BEGIN_TEST;

		CxPODVector<CxI32> v0;
		ass_true(v0.size() == 0 && v0.capacity() == 0);
		v0.compact();
		ass_true(v0.size() == 0 && v0.capacity() == 0);

		v0 = CxPODVector<CxI32>(10, 1);
		ass_true(v0.size() == 10 && v0.capacity() == 10);
		v0.compact();
		ass_true(v0.size() == 10 && v0.capacity() == 10);

		v0.clear();
		ass_eq(v0.size(), 0);
		ass_eq(v0.capacity(), 10);
		ass_true(v0.size() == 0 && v0.capacity() == 10);
		v0.compact();
		ass_true(v0.size() == 0 && v0.capacity() == 0);

		v0 << 1 << 2 << 3 << 4;
		ass_true(v0.size() == 4 && v0.capacity() == 6);
		v0.compact();
		ass_true(v0.size() == 4 && v0.capacity() == 4);

		v0.clear();
		ass_true(v0.size() == 0 && v0.capacity() == 4);
		v0.compact();
		ass_true(v0.size() == 0 && v0.capacity() == 0);

		FINISH_TEST;
	}

	void testCxPODVectorDealloc() {
		BEGIN_TEST;

		CxPODVector<CxI32> v0;
		v0.dealloc();
		ass_zero(v0.capacity());
		ass_zero(v0.data());

		v0 << 1 << 2 << 3 << 4;
		ass_eq(v0.size(), 4);
		ass_eq(v0.capacity(), 6);
		ass_neq(v0.data(), 0);
		v0.dealloc();
		ass_zero(v0.capacity());
		ass_zero(v0.data());

		v0 << 1 << 2 << 3 << 4;
	  	ass_eq(v0.size(), 4);
		ass_eq(v0.capacity(), 6);
		ass_neq(v0.data(), 0);
		v0.clear();
		ass_true(v0.capacity() == 6 && v0.data() != 0);
		v0.dealloc();
		ass_zero(v0.capacity());
		ass_zero(v0.data());

		CxPODVector<TestObject*> v1;
		v1.deallocPtr();
		ass_zero(v1.capacity());
		ass_zero(v1.data());

		v1 << new TestObject(1, 1, 1) << new TestObject(-1, -1, -1);
		ass_eq(v1.size(), 2);
		ass_eq(v1.capacity(), 2);
		ass_neq(v1.data(), 0);
		v1.clear();
		ass_true(v1.capacity() == 2 && v1.data() != 0);
		v1.deallocPtr();
		ass_zero(v1.capacity());
		ass_zero(v1.data());
		
		FINISH_TEST;
	}

	void testCxPODVectorFill() {
		BEGIN_TEST;

		CxPODVector<CxI32> v;
		v.fill(2, 10);
		ass_eq(v.size(), 10);
		for (CxI32 i = 0; i < 10; ++i) { ass_eq(v[i], 2); }
		
		v.fill(-3);
		ass_eq(v.size(), 10);
		for (CxI32 i = 0; i < 10; ++i) { ass_eq(v[i], -3); }

		CxPODVector<TestObject> v2;
		v2.fill(TestObject(1,2,3), 20);
		ass_eq(v2.size(), 20);
		for (CxI32 i = 0; i < 20; ++i) { ass_eq(v2[i], TestObject(1,2,3)); }

		v2.fill(TestObject(-1,-2,-3));
		ass_eq(v2.size(), 20);
		for (CxI32 i = 0; i < 20; ++i) { ass_eq(v2[i], TestObject(-1, -2, -3)); }

		CxPODVector<CxI32> v3;
		v3.fill(-33);
		ass_eq(v3.size(), 0);
		
		FINISH_TEST;
	}

	void testCxPODVectorInsert() {
		BEGIN_TEST;

		/* Test inserting at the beginning */
		CxPODVector<CxI32> v;
		v.insert(0, 1);
		ass_eq(v.size(), 1);
		ass_eq(v[0], 1);

		v.insert(0, 2);
		ass_eq(v.size(), 2);
		ass_true(v[0] == 2 && v[1] == 1);

		v.insert(0, 3);
		ass_eq(v.size(), 3);
		ass_true(v[0] == 3 && v[1] == 2 && v[2] == 1);

		/* Test inserting at the end */
		CxPODVector<CxI32> v1;
		v1.insert(0, 1);
		ass_eq(v1.size(), 1);
		ass_eq(v1[0], 1);

		v1.insert(1, 2);
		ass_eq(v1.size(), 2);
		ass_true(v1[0] == 1 && v1[1] == 2);

		v1.insert(2, 3);
		ass_eq(v1.size(), 3);
		ass_true(v1[0] == 1 && v1[1] == 2 && v1[2] == 3);

		/* Test inserting in the middle */
		CxPODVector<CxI32> v2;
		v2 << 1 << 2 << 3;
		v2.insert(1, -1);
		ass_eq(v2.size(), 4);
		ass_true(v2[0] == 1 && v2[1] == -1 && v2[2] == 2 && v2[3] == 3);

		v2.insert(2, -2);
		ass_eq(v2.size(), 5);
		ass_true(v2[0] == 1 && v2[1] == -1 && v2[2] == -2 && v2[3] == 2 && v2[4] == 3);

		/* Test multiple insert at beginning */
		v.dealloc();
		v.insert(0, 1, 2);
		ass_eq(v.size(), 2);
		ass_true(v[0] == 1 && v[1] == 1);

		v.insert(0, 2, 4);
		ass_eq(v.size(), 6);
		ass_true(v[0] == 2 && v[1] == 2 && v[2] == 2 && v[3] == 2 &&
					v[4] == 1 && v[5] == 1);

		v.insert(0, 3, 1);
		ass_eq(v.size(), 7);
		ass_true(v[0] == 3 &&
					v[1] == 2 && v[2] == 2 && v[3] == 2 && v[4] == 2 &&
					v[5] == 1 && v[6] == 1);

		/* Test multiple insert at end */
		v1.dealloc();
		v1.insert(0, 1, 2);
		ass_eq(v1.size(), 2);
		ass_true(v1[0] == 1 && v1[1] == 1);

		v1.insert(2, 2, 4);
		ass_eq(v1.size(), 6);
		ass_true(v1[0] == 1 && v1[1] == 1 &&
					v1[2] == 2 && v1[3] == 2 && v1[4] == 2 && v1[5] == 2);

		v1.insert(6, 3, 1);
		ass_eq(v1.size(), 7);
		ass_true(v1[0] == 1 && v1[1] == 1 &&
					v1[2] == 2 && v1[3] == 2 && v1[4] == 2 && v1[5] == 2 &&
					v1[6] == 3);

		/* Test multiple insert at middle */
	   v2.dealloc();
		v2 << 1 << 2 << 3;
		v2.insert(1, -1, 2);
		ass_eq(v2.size(), 5);
		ass_true(v2[0] == 1 &&
					v2[1] == -1 && v2[2] == -1 &&
					v2[3] == 2 && v2[4] == 3);

		v2.insert(3, -2, 1);
	   ass_eq(v2.size(), 6);
		ass_true(v2[0] == 1 &&
					v2[1] == -1 && v2[2] == -1 &&
					v2[3] == -2 && 
					v2[4] == 2 && v2[5] == 3);
		
		FINISH_TEST;
	}

	/**
	 * @tests remove(const T &)
	 */
	void testCxPODVectorRemove() {
		BEGIN_TEST;

		/* Test remove(const T &) */
		CxPODVector<CxI32> v0;
		assert(!v0.remove(0));
		assert(!v0.remove(-1));
		assert(!v0.remove(1));

		/* Test remove single item */
		v0 << 3;
		ass_eq(v0[0], 3);
		assert(!v0.remove(0));
		assert(v0.remove(3));
		ass_eq(v0.size(), 0);

		v0 << 3 << 3 << 3;
		ass_eq(v0.size(), 3);
		assert(v0.remove(3));  ass_eq(v0.size(), 2);
		assert(v0.remove(3));  ass_eq(v0.size(), 1);
		assert(v0.remove(3));  ass_eq(v0.size(), 0);

		/* Test remove first item from list */
		v0 << 2 << 3;
		ass_eq(v0.size(), 2);
		assert(!v0.remove(-2));
		assert(v0.remove(2));
		ass_eq(v0.size(), 1);  ass_eq(v0[0], 3);
		assert(!v0.remove(2));
		assert(v0.remove(3));
		ass_eq(v0.size(), 0);

		v0 << 2 << 2 << 3;
		assert(v0.remove(2));  ass_eq(v0.size(), 2);
		assert(v0.remove(2));  ass_eq(v0.size(), 1);
		assert(v0.remove(3));  ass_eq(v0.size(), 0);

		/* Test remove last item from list */
		v0 << 2 << 3;
		ass_eq(v0.size(), 2);
		assert(v0.remove(3));
		ass_eq(v0.size(), 1);  ass_eq(v0[0], 2);
		assert(!v0.remove(3));
		assert(v0.remove(2));
		ass_eq(v0.size(), 0);

		v0 << 2 << 3 << 3;
		assert(v0.remove(3));  ass_eq(v0.size(), 2);
		assert(v0.remove(3));  ass_eq(v0.size(), 1);
		assert(v0.remove(2));  ass_eq(v0.size(), 0);
		
		/* Test remove middle item from list */
		v0 << 1 << 2 << 3 << 4;
		ass_eq(v0.size(), 4);
		assert(v0[0] == 1 && v0[1] == 2 && v0[2] == 3 && v0[3] == 4);
		assert(!v0.remove(0));
		assert(v0.remove(2));
		ass_eq(v0.size(), 3);
		assert(v0[0] == 1 && v0[1] == 3 && v0[2] == 4);
		assert(v0.remove(3));
		assert(v0[0] == 1 && v0[1] == 4);
		assert(v0.remove(1));  ass_true(v0[0] == 4);
		assert(v0.remove(4));  ass_eq(v0.size(), 0);

		v0 << 1 << 1 << 2 << 2 << 3 << 3;
		ass_eq(v0.size(), 6);
		assert(v0[0] == 1 && v0[1] == 1 && v0[2] == 2 && v0[3] == 2 &&
				 v0[4] == 3 && v0[5] == 3);
		assert(v0.remove(2));
		assert(v0[0] == 1 && v0[1] == 1 && v0[2] == 2 && v0[3] == 3 && v0[4] == 3);
		assert(v0.remove(3));
		assert(v0[0] == 1 && v0[1] == 1 && v0[2] == 2 && v0[3] == 3);
		assert(v0.remove(2));
		assert(v0[0] == 1 && v0[1] == 1 && v0[2] == 3);
		assert(v0.remove(1));
		assert(v0[0] == 1 && v0[1] == 3);
		assert(v0.remove(1));  ass_true(v0[0] == 3);
		assert(v0.remove(3));  ass_eq(v0.size(), 0);


		CxPODVector<TestObject> v1;
		assert(!v1.remove(TestObject(1,1,1)));
		v1 << TestObject(1,2,3) << TestObject(1,1,1) << TestObject(1,2,3) << TestObject(1,1,1);
		assert(!v1.remove(TestObject(1,1,3)));
		assert(v1.remove(TestObject(1,1,1)));
		assert(v1[0].test(1,2,3) && v1[1].test(1,2,3) && v1[2].test(1,1,1));
		assert(v1.remove(TestObject(1,1,1)));
		assert(v1[0].test(1,2,3) && v1[1].test(1,2,3));
		assert(v1.remove(TestObject(1,2,3)));
		ass_eq(v1.size(), 1);
		assert(v1.remove(TestObject(1,2,3)));
		ass_eq(v1.size(), 0);

		
		FINISH_TEST;
	}

	/**
	 * @tests removeAll(const T &)
	 */
	void testCxPODVectorRemoveAll() {
		BEGIN_TEST;

		CxPODVector<CxI32> v0;
		assert(!v0.removeAll(0));

		/* Test remove all from front */
		v0 << 1;
		ass_eq(v0[0], 1);
		assert(!v0.removeAll(0));
		assert(v0.removeAll(1));
		ass_eq(v0.size(), 0);

		v0 << 1 << 1 << 1;
		ass_eq(v0.size(), 3);
		assert(!v0.removeAll(0));
		assert(v0.removeAll(1));
		ass_eq(v0.size(), 0);

		v0 << 1 << 2 << 3;
		ass_eq(v0.size(), 3);
		assert(v0[0] == 1 && v0[1] == 2 && v0[2] == 3);
		assert(v0.removeAll(1));
		ass_eq(v0.size(), 2);  assert(v0[0] == 2 && v0[1] == 3);
		assert(v0.removeAll(2));
		ass_eq(v0.size(), 1);  assert(v0[0] == 3);
		assert(v0.removeAll(3));
		ass_eq(v0.size(), 0);

		v0 << 1 << 1 << 1 << 2 << 3;
		ass_eq(v0.size(), 5);
		assert(v0[0] == 1 && v0[1] == 1 && v0[2] == 1 && v0[3] == 2 && v0[4] == 3);
		assert(v0.removeAll(1));
		ass_eq(v0.size(), 2);  assert(v0[0] == 2 && v0[1] == 3);
		assert(v0.removeAll(2));
		ass_eq(v0.size(), 1);  assert(v0[0] == 3);
		assert(v0.removeAll(3));
		ass_eq(v0.size(), 0);

		/* Test remove all from end */
		v0 << 2 << 3 << 1;
		ass_eq(v0.size(), 3);
		assert(v0[0] == 2 && v0[1] == 3 && v0[2] == 1);
		assert(v0.removeAll(1));
		ass_eq(v0.size(), 2);  assert(v0[0] == 2 && v0[1] == 3);
		assert(v0.removeAll(2));
		ass_eq(v0.size(), 1);  assert(v0[0] == 3);
		assert(v0.removeAll(3));
		ass_eq(v0.size(), 0);

		v0 << 2 << 3 << 1 << 1 << 1;
		ass_eq(v0.size(), 5);
		assert(v0[0] == 2 && v0[1] == 3 && v0[2] == 1 && v0[3] == 1 && v0[4] == 1);
		assert(v0.removeAll(1));
		ass_eq(v0.size(), 2);  assert(v0[0] == 2 && v0[1] == 3);
		assert(v0.removeAll(2));
		ass_eq(v0.size(), 1);  assert(v0[0] == 3);
		assert(v0.removeAll(3));
		ass_eq(v0.size(), 0);

		/* Test remove all from middle */
		v0 << 1 << 2 << 3 << 4;
		ass_eq(v0.size(), 4);
		ass_true(v0[0] == 1 && v0[1] == 2 && v0[2] == 3 && v0[3] == 4);
		assert(v0.removeAll(2));
		ass_eq(v0.size(), 3);
		ass_true(v0[0] == 1 && v0[1] == 3 && v0[2] == 4);
		assert(v0.removeAll(3));
		ass_true(v0[0] == 1 && v0[1] == 4);
		assert(v0.removeAll(1));
		ass_eq(v0[0], 4);
		assert(!v0.removeAll(1));
		assert(v0.removeAll(4));
		ass_eq(v0.size(), 0);

		v0 << 1 << 2 << 2 << 2 << 3;
		ass_eq(v0.size(), 5);
		ass_true(v0[0] == 1 && v0[1] == 2 && v0[2] == 2 && v0[3] == 2 && v0[4] == 3);
		assert(v0.removeAll(2));
		ass_eq(v0.size(), 2);
		ass_true(v0[0] == 1 && v0[1] == 3);
		v0.clear();
		assert(!v0.removeAll(1));

		/* Test remove from random */
		v0 << 1 << 2 << 1 << 1 << 2 << 2 << 3;
		ass_eq(v0.size(), 7);
		ass_true(v0[0] == 1 && v0[1] == 2 && v0[2] == 1 && v0[3] == 1 &&
					v0[4] == 2 && v0[5] == 2 && v0[6] == 3);
		assert(v0.removeAll(1));
		ass_eq(v0.size(), 4);
		ass_true(v0[0] == 2 && v0[1] == 2 && v0[2] == 2 && v0[3] == 3);
		assert(!v0.removeAll(1));
		assert(v0.removeAll(2));
		ass_eq(v0.size(), 1);
		assert(v0.removeAll(3));
		ass_eq(v0.size(), 0);


		CxPODVector<TestObject> v1;
		assert(!v1.removeAll(TestObject(1,1,1)));

		v1 << TestObject(1,1,1) << TestObject(2,2,2) << TestObject(1,1,1) << TestObject(2,2,2);
		ass_eq(v1.size(), 4);
		assert(!v1.removeAll(TestObject(0,0,0)));
		assert(v1.removeAll(TestObject(1,1,1)));
		ass_eq(v1.size(), 2);
		ass_true(v1[0].test(2,2,2) && v1[1].test(2,2,2));
		assert(!v1.removeAll(TestObject(1,1,1)));
		assert(v1.removeAll(TestObject(2,2,2)));
		ass_eq(v1.size(), 0);
		
		FINISH_TEST;
	}

	/** 
	 * @tests removeAt(CxI32)
	 */
	void testCxPODVectorRemoveAt() {
		BEGIN_TEST;

		CxPODVector<CxI32> v0;
		assert(!v0.removeAt(1));

		/* Test remove single element */
		v0 << 1;
		ass_eq(v0[0], 1);
		assert(!v0.removeAt(1));
		assert(v0.removeAt(0));
		ass_eq(v0.size(), 0);

		/* Test remove from front */
		v0 << 1 << 2;
		ass_true(v0[0] == 1 && v0[1] == 2);
		assert(v0.removeAt(0));
		ass_true(v0.size() == 1 && v0[0] == 2);
		assert(v0.removeAt(0));
		ass_eq(v0.size(), 0);

		v0 << 1 << 2 << 3;
		ass_true(v0[0] == 1 && v0[1] == 2 && v0[2] == 3);
		assert(v0.removeAt(0));
		ass_true(v0[0] == 2 && v0[1] == 3 && v0.size() == 2);
		assert(v0.removeAt(0));
		ass_true(v0.size() == 1 && v0[0] == 3);
		assert(v0.removeAt(0));
		ass_eq(v0.size(), 0);

		/* Test remove from last */
		v0 << 1 << 2;
		ass_true(v0[0] == 1 && v0[1] == 2);
		assert(v0.removeAt(1));
		ass_true(v0.size() == 1 && v0[0] == 1);
		v0.clear();
		assert(!v0.removeAt(0));

		v0 << 1 << 2 << 3;
		ass_true(v0[0] == 1 && v0[1] == 2 && v0[2] == 3);
		assert(v0.removeAt(2));
		ass_true(v0[0] == 1 && v0[1] == 2);
		assert(v0.removeAt(1));
		ass_true(v0[0] == 1 && v0.size() == 1);
		v0.clear();

		/* Test remove from middle */
		v0 << 1 << 2 << 3 << 4 << 5;
		assert(v0.removeAt(2));
		ass_true(v0[0] == 1 && v0[1] == 2 && v0[2] == 4 && v0[3] == 5);
		assert(v0.removeAt(1));
		ass_true(v0[0] == 1 && v0[1] == 4 && v0[2] == 5);
		assert(v0.removeAt(2));
		ass_true(v0[0] == 1 && v0[1] == 4);
		assert(v0.removeAt(0));
		ass_true(v0[0] == 4);
		assert(v0.removeAt(0));
		ass_eq(v0.size(), 0);


		CxPODVector<TestObject> v1;
		assert(!v1.removeAt(0));

		v1 << TestObject(1,1,1) << TestObject(2,2,2) << TestObject(3,3,3) << TestObject(4,4,4);
		assert(v1.removeAt(1));
		ass_true(v1[0].test(1,1,1) && v1[1].test(3,3,3) && v1[2].test(4,4,4));
		assert(v1.removeAt(2));
		ass_true(v1[0].test(1,1,1) && v1[1].test(3,3,3));
		assert(v1.removeAt(0));
		ass_true(v1[0].test(3,3,3));
		assert(v1.removeAt(0));
		ass_eq(v1.size(), 0);

		FINISH_TEST;
	}

	/**
	 * @tests removeFirst()
	 * @tests removeLast()
	 */
	void testCxPODVectorRemoveFirstLast() {
		BEGIN_TEST;

		/* First, test removing from single element vector */
		CxPODVector<CxI32> v0;
		v0 << 1;
		ass_eq(v0[0], 1);
		v0.removeFirst();
		ass_zero(v0.size());

		v0 << 2;
		ass_eq(v0[0], 2);
		v0.removeLast();
		ass_zero(v0.size());

		/* Test remove from double element vector */
		v0 << 1 << 2;
		ass_true(v0[0] == 1 && v0[1] == 2);
		v0.removeFirst();
		ass_true(v0.size() == 1 && v0[0] == 2);
		v0.removeFirst();
		ass_zero(v0.size());

		v0 << 3 << 4;
		ass_true(v0[0] == 3 && v0[1] == 4);
		v0.removeLast();
		ass_true(v0.size() == 1 && v0[0] == 3);
		v0.removeLast();
		ass_zero(v0.size());

		/* Test remove from multiple element vector */
		v0 << 1 << 2 << 3 << 4 << 5;
		ass_true(v0[0] == 1 && v0[1] == 2 && v0[2] == 3 &&
					v0[3] == 4 && v0[4] == 5);
		v0.removeFirst();
		ass_true(v0[0] == 2 && v0[1] == 3 && v0[2] == 4 &&
					v0[3] == 5 && v0.size() == 4);
		v0.removeLast();
		ass_true(v0[0] == 2 && v0[1] == 3 && v0[2] == 4 &&
					v0.size() == 3);
		v0.removeLast();
		ass_true(v0[0] == 2 && v0[1] == 3 && v0.size() == 2);
		v0.removeFirst();
		ass_true(v0[0] == 3 && v0.size() == 1);
		v0.removeFirst();
		ass_zero(v0.size());


		/* Test with objects */
		CxPODVector<TestObject> v1;
		v1 << TestObject(1,1,1) << TestObject(2,2,2) << TestObject(3,3,3)
			<< TestObject(4,4,4) << TestObject(5,5,5);
		ass_true(v1[0].test(1,1,1) && v1[1].test(2,2,2) && v1[2].test(3,3,3) &&
					v1[3].test(4,4,4) && v1[4].test(5,5,5) && v1.size() == 5);
		v1.removeFirst();
		ass_true(v1[0].test(2,2,2) && v1[1].test(3,3,3) &&
					v1[2].test(4,4,4) && v1[3].test(5,5,5) && v1.size() == 4);
		v1.removeLast();
		ass_true(v1[0].test(2,2,2) && v1[1].test(3,3,3) &&
					v1[2].test(4,4,4) && v1.size() == 3);
		v1.removeLast();
		ass_true(v1[0].test(2,2,2) && v1[1].test(3,3,3) && v1.size() == 2);
		v1.removeFirst();
		ass_true(v1[0].test(3,3,3) && v1.size() == 1);
		v1.removeFirst();
		ass_zero(v1.size());
		
		FINISH_TEST;
	}

	/**
	 * @tests reserve(CxI32)
	 * @tests resizeToCapacity(CxI32)
	 */
	void testCxPODVectorReserve() {
		BEGIN_TEST;

		CxPODVector<CxI32> v0;
		ass_zero(v0.capacity());
		ass_zero(v0.size());

		v0.reserve(1);
		ass_eq(v0.capacity(), 1);
		ass_zero(v0.size());

		v0.reserve(10);
		ass_eq(v0.capacity(), 10);
		ass_zero(v0.size());

		v0.dealloc();
		ass_zero(v0.capacity());
		ass_zero(v0.size());

		v0.reserve(10);
		ass_eq(v0.capacity(), 10);
		ass_zero(v0.size());

		v0.reserve(2);
		ass_eq(v0.capacity(), 10);
		ass_zero(v0.size());


		CxPODVector<TestObject> v1;
		ass_zero(v1.capacity());
		ass_zero(v1.size());

		v1.reserve(1);
		ass_eq(v1.capacity(), 1);
		ass_zero(v1.size());

		v1.reserve(10);
		ass_eq(v1.capacity(), 10);
		ass_zero(v1.size());

		v1.dealloc();
		ass_zero(v1.capacity());
		ass_zero(v1.size());

		v1.reserve(10);
		ass_eq(v1.capacity(), 10);
		ass_zero(v1.size());

		v1.reserve(2);
		ass_eq(v1.capacity(), 10);
		ass_zero(v1.size());
		
		FINISH_TEST;
	}

	/**
	 * @tests resize(CxI32)
	 * @tests resizeToCapacity(CxI32)
	 */
	void testCxPODVectorResize() {
		BEGIN_TEST;

		CxPODVector<CxI32> v0;
		ass_zero(v0.capacity());
		ass_zero(v0.size());

		v0.resize(1);
		ass_eq(v0.capacity(), 1);
		ass_eq(v0.size(), 1);

		v0.resize(10);
		ass_eq(v0.capacity(), 10);
		ass_eq(v0.size(), 10);

		v0.dealloc();
		ass_zero(v0.capacity());
		ass_zero(v0.size());

		v0.resize(10);
		ass_eq(v0.capacity(), 10);
		ass_eq(v0.size(), 10);

		v0.resize(4);
		ass_eq(v0.capacity(), 10);
		ass_eq(v0.size(), 4);

		v0.resize(1);
		ass_eq(v0.capacity(), 10);
		ass_eq(v0.size(), 1);

		v0.resize(0);
		ass_eq(v0.capacity(), 10);
		ass_eq(v0.size(), 0);

		/* Test with obejcst */
		CxPODVector<TestObject> v1;
		ass_zero(v1.capacity());
		ass_zero(v1.size());

		v1.resize(1);
		ass_eq(v1.capacity(), 1);
		ass_eq(v1.size(), 1);

		v1.resize(10);
		ass_eq(v1.capacity(), 10);
		ass_eq(v1.size(), 10);

		v1.dealloc();
		ass_zero(v1.capacity());
		ass_zero(v1.size());

		v1.resize(10);
		ass_eq(v1.capacity(), 10);
		ass_eq(v1.size(), 10);

		v1.resize(4);
		ass_eq(v1.capacity(), 10);
		ass_eq(v1.size(), 4);

		v1.resize(1);
		ass_eq(v1.capacity(), 10);
		ass_eq(v1.size(), 1);

		v1.resize(0);
		ass_eq(v1.capacity(), 10);
		ass_eq(v1.size(), 0);
		

		FINISH_TEST;
	}


	void testCxPODVectorTakeAt() {
		BEGIN_TEST;

		CxPODVector<CxI32> v0;

		/* Test with single element vector */
		v0 << 0;
		assert(v0.takeAt(0) == 0);
		ass_zero(v0.size());

		v0 << -1;
		assert(v0.takeAt(0) == -1);
		ass_zero(v0.size());

		/* Test with multiple element vector */
		v0 << 1 << 2 << 3 << 4 << 5;
		assert(v0.takeAt(0) == 1);
		ass_eq(v0.size(), 4);

		assert(v0.takeAt(1) == 3);
		ass_eq(v0.size(), 3);

		assert(v0.takeAt(2) == 5);
		ass_eq(v0.size(), 2);
		
		assert(v0.takeAt(1) == 4);
		ass_eq(v0.size(), 1);

		assert(v0.takeAt(0) == 2);
		ass_zero(v0.size());


		/* Test with object vector */
		CxPODVector<TestObject> v1;

		v1 << TestObject(0);
		assert(v1.takeAt(0) == TestObject(0));
		ass_zero(v1.size());

		v1 << TestObject(1,2,3);
		assert(v1.takeAt(0) == TestObject(1,2,3));
		ass_zero(v1.size());

		/* Test with multiple elements */
		v1 << TestObject(1) << TestObject(1,2,3) << TestObject(3)
			<< TestObject(4) << TestObject(5,5,1);

		assert(v1.takeAt(0) == TestObject(1,1,1));
		ass_eq(v1.size(), 4);

		assert(v1.takeAt(1) == TestObject(3));
		ass_eq(v1.size(), 3);

		assert(v1.takeAt(2) == TestObject(5,5,1));
		ass_eq(v1.size(), 2);

		assert(v1.takeAt(1) == TestObject(4));
		ass_eq(v1.size(), 1);

		assert(v1.takeAt(0) == TestObject(1,2,3));
		ass_zero(v1.size());
		
		FINISH_TEST;
	}

	void testCxPODVectorTakeFirstLast() {
		BEGIN_TEST;

		CxPODVector<CxI32> v0;

		/* Test Take first / last from single element vector */
		v0 << 0;
		assert(v0.takeFirst() == 0);
		ass_zero(v0.size());

		v0 << 0;
		assert(v0.takeLast() == 0);
		ass_zero(v0.size());

		v0 << 1;
		assert(v0.takeFirst() == 1);
		ass_zero(v0.size());

		v0 << 1;
		assert(v0.takeLast() == 1);
		ass_zero(v0.size());

		/* Test take first / last from multiple element vector */
		v0 << 1 << 2 << 3 << 4 << 5;

		assert(v0.takeFirst() == 1);
		ass_eq(v0.size(), 4);

		assert(v0.takeLast() == 5);
		ass_eq(v0.size(), 3);

		assert(v0.takeLast() == 4);
		ass_eq(v0.size(), 2);

		assert(v0.takeFirst() == 2);
		ass_eq(v0.size(), 1);

		assert(v0.takeLast() == 3);
		ass_zero(v0.size());


		/* Test with objects */
		CxPODVector<TestObject> v1;

		/* Test on single element vectors */
		v1 << TestObject(0);
		assert(v1.takeFirst() == TestObject(0));
		ass_zero(v1.size());

		v1 << TestObject(1,2,3);
		assert(v1.takeLast() == TestObject(1,2,3));
		ass_zero(v1.size());

		/* Test on multiple element vectors */
		v1 << TestObject(1) << TestObject(1,2,3) << TestObject(3)
			<< TestObject(4) << TestObject(5,5,1);

		assert(v1.takeFirst() == TestObject(1));
		ass_eq(v1.size(), 4);

		assert(v1.takeLast() == TestObject(5,5,1));
		ass_eq(v1.size(), 3);

		assert(v1.takeLast() == TestObject(4,4,4));
		ass_eq(v1.size(), 2);

		assert(v1.takeFirst() == TestObject(1,2,3));
		ass_eq(v1.size(), 1);

		assert(v1.takeFirst() == TestObject(3));
		ass_zero(v1.size());

		FINISH_TEST;
	}

	void testCxPODVectorISPTR() {
		BEGIN_TEST;

		CxPODVector<CxI32> *v0 = new CxPODVector<CxI32>();
		ass_eq(v0->retainCount(), 1);

		CxDeleteISPtr(v0);
		ass_zero(v0);

		v0 = new CxPODVector<CxI32>();
		(*v0) << 1 << 2 << 3;
		ass_eq(v0->size(), 3);

		CxPODVector<CxI32> *v01 = v0;
		v0->retain();

		CxDeleteISPtr(v01);
		ass_zero(v01);
		ass_eq(v0->retainCount(), 1);

		/* Test the actual ISPointers */
		CxPODVector<CxI32>::Ptr v1(v0);
		ass_eq(v0->retainCount(), 2);

		appendToCxPODVectorPtr(v1, 2);
		ass_eq(v0->at(3), 2);

		v1.setNull();
		ass_eq(v0->retainCount(), 1);

		v1 = v0;
		ass_eq(v0->retainCount(), 2);

		CxDeleteISPtr(v0);
		ass_zero(v0);

		v1.setNull();

		FINISH_TEST;
	}
	
} // namespace cat

int main(int argc, char **argv) {
	cat::testCxPODVectorCreateAndDestroy();
	cat::testCxPODVectorEquality();
	cat::testCxPODVectorConcatenate();
	cat::testCxPODVectorAppend();
	cat::testCxPODVectorSet();
	cat::testCxPODVectorCompact();
	cat::testCxPODVectorDealloc();
	cat::testCxPODVectorFill();
	cat::testCxPODVectorInsert();
	cat::testCxPODVectorRemove();
	cat::testCxPODVectorRemoveAll();
	cat::testCxPODVectorRemoveAt();
	cat::testCxPODVectorRemoveFirstLast();
	cat::testCxPODVectorReserve();
	cat::testCxPODVectorResize();
	cat::testCxPODVectorTakeAt();
	cat::testCxPODVectorTakeFirstLast();
	cat::testCxPODVectorISPTR();
	return 0;
}
