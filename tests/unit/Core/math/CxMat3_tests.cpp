#include "core/math/CxMat3.h"
#include "core/CxTestCore.h"

namespace cat {

	/**
	 * @tests CxMat3()
	 * @tests CxMat3(CxZero)
	 * @tests CxMat3(CxIdentity)
	 * @tests CxMat3(const CxReal *)
	 * @tests CxMat3(const CxVec3 &, const CxVec3 &, const CxVec3 &)
	 * @tests CxMat3(CxReal, CxReal, CxReal, CxReal, CxReal, CxReal, CxReal, CxReal, CxReal)
	 * @tests CxMat3(const CxMat3 &)
	 * @tests operator=(const CxMat3 &)
	 * @tests operator[](CxI32)
	 * @tests operator[](CxI32) const
	 * @tests operator()(CxI32, CxI32)
	 * @tests operator()(CxI32, CxI32) const
	 * @tests constData() const
	 * @tests data()
	 * @tests data() const
	 * @tests diagonal() const
	 * @tests transpose()
	 * @tests transposed() const
	 * @tests xAxis() const
	 * @tests yAxis() const
	 * @tests zAxis() const
	 */
	void testCxMat3Basics() {
		BEGIN_TEST;

		CxMat3 m0;
		CxMat3 m1(kCxZero);
		ass_true(m1[0] == 0 && m1[1] == 0 && m1[2] == 0 &&
					m1[3] == 0 && m1[4] == 0 && m1[5] == 0 &&
					m1[6] == 0 && m1[7] == 0 && m1[8] == 0);

		CxMat3 m2(kCxIdentity);
		ass_true(m2[0] == 1 && m2[1] == 0 && m2[2] == 0 &&
					m2[3] == 0 && m2[4] == 1 && m2[5] == 0 &&
					m2[6] == 0 && m2[7] == 0 && m2[8] == 1);
		ass_true(m2(0,0) == 1 && m2(0,1) == 0 && m2(0,2) == 0 &&
					m2(1,0) == 0 && m2(1,1) == 1 && m2(1,2) == 0 &&
					m2(2,0) == 0 && m2(2,1) == 0 && m2(2,2) == 1);
		ass_true(m2.col0 == CxVec3(1,0,0));
		ass_true(m2.col1 == CxVec3(0,1,0));
		ass_true(m2.col2 == CxVec3(0,0,1));

		CxMat3 m3(1,2,3,4,5,6,7,8,9);
		ass_true(m3[0] == 1 && m3[1] == 2 && m3[2] == 3 &&
					m3[3] == 4 && m3[4] == 5 && m3[5] == 6 &&
					m3[6] == 7 && m3[7] == 8 && m3[8] == 9);
		ass_true(m3(0,0) == 1 && m3(0,1) == 2 && m3(0,2) == 3 &&
					m3(1,0) == 4 && m3(1,1) == 5 && m3(1,2) == 6 &&
					m3(2,0) == 7 && m3(2,1) == 8 && m3(2,2) == 9);
		ass_true(m3.col0 == CxVec3(1,2,3));
		ass_true(m3.col1 == CxVec3(4,5,6));
		ass_true(m3.col2 == CxVec3(7,8,9));

		const CxReal rd[] = {1,2,3,4,5,6,7,8,9};
		CxMat3 m4(rd);
		ass_true(m4[0] == 1 && m4[1] == 2 && m4[2] == 3 &&
					m4[3] == 4 && m4[4] == 5 && m4[5] == 6 &&
					m4[6] == 7 && m4[7] == 8 && m4[8] == 9);
		ass_true(m4.col0 == CxVec3(1,2,3));
		ass_true(m4.col1 == CxVec3(4,5,6));
		ass_true(m4.col2 == CxVec3(7,8,9));

		CxMat3 m5(CxVec3(1,2,3), CxVec3(4,5,6), CxVec3(7,8,9));
		ass_true(m5[0] == 1 && m5[1] == 2 && m5[2] == 3 &&
					m5[3] == 4 && m5[4] == 5 && m5[5] == 6 &&
					m5[6] == 7 && m5[7] == 8 && m5[8] == 9);
		ass_true(m5.col0 == CxVec3(1,2,3));
		ass_true(m5.col1 == CxVec3(4,5,6));
		ass_true(m5.col2 == CxVec3(7,8,9));

		CxMat3 m6(m5);
		ass_true(m6[0] == 1 && m6[1] == 2 && m6[2] == 3 &&
					m6[3] == 4 && m6[4] == 5 && m6[5] == 6 &&
					m6[6] == 7 && m6[7] == 8 && m6[8] == 9);
		ass_true(m6.col0 == CxVec3(1,2,3));
		ass_true(m6.col1 == CxVec3(4,5,6));
		ass_true(m6.col2 == CxVec3(7,8,9));
		
		m0 = m5;
		ass_true(m0[0] == 1 && m0[1] == 2 && m0[2] == 3 &&
					m0[3] == 4 && m0[4] == 5 && m0[5] == 6 &&
					m0[6] == 7 && m0[7] == 8 && m0[8] == 9);
		ass_true(m0.col0 == CxVec3(1,2,3));
		ass_true(m0.col1 == CxVec3(4,5,6));
		ass_true(m0.col2 == CxVec3(7,8,9));

		const CxMat3 mc0(m5);
		ass_true(mc0[0] == 1 && mc0[1] == 2 && mc0[2] == 3 &&
					mc0[3] == 4 && mc0[4] == 5 && mc0[5] == 6 &&
					mc0[6] == 7 && mc0[7] == 8 && mc0[8] == 9);
		ass_true(mc0(0,0) == 1 && mc0(0,1) == 2 && mc0(0,2) == 3 &&
					mc0(1,0) == 4 && mc0(1,1) == 5 && mc0(1,2) == 6 &&
					mc0(2,0) == 7 && mc0(2,1) == 8 && mc0(2,2) == 9);

		CxReal *d = m6.data();
		ass_true(d[0] == 1 && d[1] == 2 && d[2] == 3 &&
					d[3] == 4 && d[4] == 5 && d[5] == 6 &&
					d[6] == 7 && d[7] == 8 && d[8] == 9);

		const CxReal *dc0 = mc0.data();
		ass_true(dc0[0] == 1 && dc0[1] == 2 && dc0[2] == 3 &&
					dc0[3] == 4 && dc0[4] == 5 && dc0[5] == 6 &&
					dc0[6] == 7 && dc0[7] == 8 && dc0[8] == 9);

		const CxReal *dc1 = mc0.constData();
		ass_true(dc1[0] == 1 && dc1[1] == 2 && dc1[2] == 3 &&
					dc1[3] == 4 && dc1[4] == 5 && dc1[5] == 6 &&
					dc1[6] == 7 && dc1[7] == 8 && dc1[8] == 9);

		const CxReal *dc2 = m6.constData();
		ass_true(dc2[0] == 1 && dc2[1] == 2 && dc2[2] == 3 &&
					dc2[3] == 4 && dc2[4] == 5 && dc2[5] == 6 &&
					dc2[6] == 7 && dc2[7] == 8 && dc2[8] == 9);

		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		m1 = m0.transposed();
		ass_true(m1[0] == 1 && m1[1] == 4 && m1[2] == 7 &&
					m1[3] == 2 && m1[4] == 5 && m1[5] == 8 &&
					m1[6] == 3 && m1[7] == 6 && m1[8] == 9);
		ass_true(m1.col0 == CxVec3(1,4,7));
		ass_true(m1.col1 == CxVec3(2,5,8));
		ass_true(m1.col2 == CxVec3(3,6,9));
		ass_true(m0[0] == 1 && m0[1] == 2 && m0[2] == 3 &&
					m0[3] == 4 && m0[4] == 5 && m0[5] == 6 &&
					m0[6] == 7 && m0[7] == 8 && m0[8] == 9);
		m0.transpose();
		ass_true(m0[0] == 1 && m0[1] == 4 && m0[2] == 7 &&
					m0[3] == 2 && m0[4] == 5 && m0[5] == 8 &&
					m0[6] == 3 && m0[7] == 6 && m0[8] == 9);
		ass_true(m0.col0 == CxVec3(1,4,7));
		ass_true(m0.col1 == CxVec3(2,5,8));
		ass_true(m0.col2 == CxVec3(3,6,9));


		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		m1 = m0.diagonal();
		ass_true(m1[0] == 1 && m1[1] == 0 && m1[2] == 0 &&
					m1[3] == 0 && m1[4] == 5 && m1[5] == 0 &&
					m1[6] == 0 && m1[7] == 0 && m1[8] == 9);
		ass_true(m0[0] == 1 && m0[1] == 2 && m0[2] == 3 &&
					m0[3] == 4 && m0[4] == 5 && m0[5] == 6 &&
					m0[6] == 7 && m0[7] == 8 && m0[8] == 9);

		ass_true(m0.xAxis() == CxVec3(1,2,3));
		ass_true(m0.yAxis() == CxVec3(4,5,6));
		ass_true(m0.zAxis() == CxVec3(7,8,9));
		
		FINISH_TEST;
	}

	/**
	 * @tests operator==(const CxMat3 &) const
	 * @tests operator!=(const CxMat3 &) const
	 * @tests CxEq(const CxMat3 &, const CxMat3 &, CxReal)
	 * @tests CxIsFinite(const CxMat3 &)
	 */
	void testCxMat3Equality() {
		BEGIN_TEST;

		CxMat3 m0(1,2,3,4,5,6,7,8,9);
		CxMat3 m1(1,2,3,4,5,6,7,8,9);

		ass_true(m0 == m1 && m1 == m0);
		ass_false(m0 != m1 || m1 != m0);
		ass_true(CxEq(m0, m1) && CxEq(m1, m0));
		ass_true(CxIsFinite(m0));
		
		m1[0] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[0] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[0] = CX_NAN;  ass_false(CxIsFinite(m1));
		
		m1 = m0;
		ass_true(m0 == m1 && m1 == m0);
		ass_false(m0 != m1 || m1 != m0);
		m1[1] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[1] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[1] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[2] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[2] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[2] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[3] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[3] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[3] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[4] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[4] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[4] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[5] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[5] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[5] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[6] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[6] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[6] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[7] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[7] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[7] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[8] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[8] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[8] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;
		m1[0] = 1.0001f;
		ass_true(CxEq(m1,m0,0.001f) && CxEq(m0,m1,0.001f));
		ass_false(CxEq(m1,m0,0.0001f) || CxEq(m0,m1,0.0001f));

		
		FINISH_TEST;
	}

	/**
	 * @tests operator-() const
	 * @tests operator+(const CxMat3 &) const
	 * @tests operator-(const CxMat3 &) const
	 * @tests operator*(CxReal) const
	 * @tests opeartor/(CxReal) const
	 * @tests operator+=(const CxMat3 &)
	 * @tests opeartor-=(const CxMat3 &)
	 * @tests operator*=(CxReal)
	 * @tests operator/=(CxReal)
	 */
	void testCxMat3Arithmetic() {
		BEGIN_TEST;

		/* test operator-() */
		CxMat3 m0(1,2,3,4,5,6,7,8,9);
		CxMat3 m1 = -m0;
		ass_true(m1[0] == -1 && m1[1] == -2 && m1[2] == -3 &&
					m1[3] == -4 && m1[4] == -5 && m1[5] == -6 &&
					m1[6] == -7 && m1[7] == -8 && m1[8] == -9);
		ass_true(m0[0] == 1 && m0[1] == 2 && m0[2] == 3 &&
					m0[3] == 4 && m0[4] == 5 && m0[5] == 6 &&
					m0[6] == 7 && m0[7] == 8 && m0[8] == 9);
		ass_false(m0 == m1);
		m1 = -m1;
		ass_true(m0 == m1);

		/* test operator+(const CxMat3 &) */
		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		m1 = CxMat3(10,11,12,13,14,15,16,17,18);
		CxMat3 r = m0 + m1;
		ass_true(r[0] == 11 && r[1] == 13 && r[2] == 15 &&
					r[3] == 17 && r[4] == 19 && r[5] == 21 &&
					r[6] == 23 && r[7] == 25 && r[8] == 27);

		m1 = CxMat3(-10,-11,-12,-13,-14,-15,-16,-17,-18);
		r = m0 + m1;
		ass_true(r[0] == -9 && r[1] == -9 && r[2] == -9 &&
					r[3] == -9 && r[4] == -9 && r[5] == -9 &&
					r[6] == -9 && r[7] == -9 && r[8] == -9);

		/* test operator-(const CxMat3 &) */
		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		m1 = CxMat3(10,11,12,13,14,15,16,17,18);
		r = m0 - m1;
		ass_true(r[0] == -9 && r[1] == -9 && r[2] == -9 &&
					r[3] == -9 && r[4] == -9 && r[5] == -9 &&
					r[6] == -9 && r[7] == -9 && r[8] == -9);

		m1 = CxMat3(-10,-11,-12,-13,-14,-15,-16,-17,-18);
		r = m0 - m1;
		ass_true(r[0] == 11 && r[1] == 13 && r[2] == 15 &&
					r[3] == 17 && r[4] == 19 && r[5] == 21 &&
					r[6] == 23 && r[7] == 25 && r[8] == 27);

		/* test operator*(CxReal) */
		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		r = m0*10;
		ass_true(r[0] == 10 && r[1] == 20 && r[2] == 30 &&
					r[3] == 40 && r[4] == 50 && r[5] == 60 &&
					r[6] == 70 && r[7] == 80 && r[8] == 90);

		/* test operator/(CxReal) */
		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		r = m0/10;
		ass_true(CxEq(CxMat3(.1f,.2f,.3f,.4f,.5f,.6f,.7f,.8f,.9f), r, 0.000001f));

		/* test operator+=(const CxMat3 &) */
		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		m1 = CxMat3(10,11,12,13,14,15,16,17,18);
		r = m0;
		r += m1;
		ass_true(r[0] == 11 && r[1] == 13 && r[2] == 15 &&
					r[3] == 17 && r[4] == 19 && r[5] == 21 &&
					r[6] == 23 && r[7] == 25 && r[8] == 27);

		m1 = CxMat3(-10,-11,-12,-13,-14,-15,-16,-17,-18);
		r = m0;
		r += m1;
		ass_true(r[0] == -9 && r[1] == -9 && r[2] == -9 &&
					r[3] == -9 && r[4] == -9 && r[5] == -9 &&
					r[6] == -9 && r[7] == -9 && r[8] == -9);

		/* test operator-=(const CxMat3 &) */
		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		m1 = CxMat3(10,11,12,13,14,15,16,17,18);
		r = m0;
		r -= m1;
		ass_true(r[0] == -9 && r[1] == -9 && r[2] == -9 &&
					r[3] == -9 && r[4] == -9 && r[5] == -9 &&
					r[6] == -9 && r[7] == -9 && r[8] == -9);

		m1 = CxMat3(-10,-11,-12,-13,-14,-15,-16,-17,-18);
		r = m0;
		r -= m1;
		ass_true(r[0] == 11 && r[1] == 13 && r[2] == 15 &&
					r[3] == 17 && r[4] == 19 && r[5] == 21 &&
					r[6] == 23 && r[7] == 25 && r[8] == 27);

		/* test operator*=(CxReal) */
		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		r = m0;
		r *= 10;
		ass_true(r[0] == 10 && r[1] == 20 && r[2] == 30 &&
					r[3] == 40 && r[4] == 50 && r[5] == 60 &&
					r[6] == 70 && r[7] == 80 && r[8] == 90);

		/* test operator/=(CxReal) */
		m0 = CxMat3(1,2,3,4,5,6,7,8,9);
		r = m0;
		r /= 10;
		ass_true(CxEq(CxMat3(.1f,.2f,.3f,.4f,.5f,.6f,.7f,.8f,.9f), r, 0.000001f));
		FINISH_TEST;
	}

	void testCxMat3Mult() {
		BEGIN_TEST;

	   CxMat3 m0(2,3,4,5,6,7,8,9,10);
		CxMat3 m1(11,12,13,14,15,16,17,18,19);
		CxMat3 r = m0*m1;
		ass_true(r[0] == 186 && r[1] == 222 && r[2] == 258 &&
					r[3] == 231 && r[4] == 276 && r[5] == 321 &&
					r[6] == 276 && r[7] == 330 && r[8] == 384);

		r = m1*m0;
		ass_true(r[0] == 132 && r[1] == 141 && r[2] == 150 &&
					r[3] == 258 && r[4] == 276 && r[5] == 294 &&
					r[6] == 384 && r[7] == 411 && r[8] == 438);

		r = m0;
		r *= m1;
		ass_true(r[0] == 186 && r[1] == 222 && r[2] == 258 &&
					r[3] == 231 && r[4] == 276 && r[5] == 321 &&
					r[6] == 276 && r[7] == 330 && r[8] == 384);

		r = m1;
		r *= m0;
		ass_true(r[0] == 132 && r[1] == 141 && r[2] == 150 &&
					r[3] == 258 && r[4] == 276 && r[5] == 294 &&
					r[6] == 384 && r[7] == 411 && r[8] == 438);

		FINISH_TEST;
	}

	
	void testCxMat3DeterminantAndInverse() {
		BEGIN_TEST;

		CxMat3 m0(kCxIdentity);
		ass_true(CxEq(m0.determinant(), 1, 0.0000001f));
		ass_true(CxEq(m0.inverse(), m0, 0.00000001f));
		m0.invert();
		ass_true(CxEq(m0, CxMat3(kCxIdentity), 0.0000001f));
		
		
		m0 = CxMat3(2,3,4,5,6,7,8,9,10);
		ass_true(CxEq(m0.determinant(), 0, 0.0000001f));
		ass_true(CxEq(m0.inverse(), CxMat3(kCxIdentity)));
		m0.invert();
		ass_true(CxEq(m0, CxMat3(2,3,4,5,6,7,8,9,10)));
		
		m0 = CxMat3(0.4f, -12, 1.3f,
						30, -33.3f, -10.1f,
						93, 0.1f, -0.3f);
		ass_true(CxEq(m0.determinant(), 15197.87f, 0.001f));

		CxMat3 m1 = m0.inverse();
		ass_true(CxEq(CxMat3(0.00072378563575f,-0.0002283214687f, 0.010823227f,
									-0.0612125252f, -0.007962958f, 0.0028319758f,
									0.203969372f, -0.073433975f, 0.0228110913f), m1, 0.00001f));
		ass_true(CxEq(m1.determinant(), 0.000065798694f, 0.0000001f));
		ass_true(CxEq(m0.inverse().determinant(), 0.000065798694f, 0.0000001f));
		m1.invert();
		ass_true(CxEq(m1, m0, 0.00001f));
		m0.invert();
		ass_true(CxEq(CxMat3(0.00072378563575f,-0.0002283214687f, 0.010823227f,
									-0.0612125252f, -0.007962958f, 0.0028319758f,
									0.203969372f, -0.073433975f, 0.0228110913f), m0, 0.000001f));
		
		FINISH_TEST;
	}

	void testCxMat3Transform() {
		BEGIN_TEST;

		CxMat3 m0(kCxIdentity);
		CxVec3 v0(11,12,13);
		CxVec3 r = m0.transform(v0);
		ass_true(CxEq(v0, r));
		r = m0*v0;
		ass_true(CxEq(v0, r));
		r = m0.transformInv(v0);
		ass_true(CxEq(v0, r));
		r = m0.transformTrans(v0);
		ass_true(CxEq(v0,r));
		
		m0 = CxMat3(2,3,4,5,6,7,8,9,10);
		v0 = CxVec3(11,12,13);
		r = m0.transform(v0);
		ass_true(CxEq(CxVec3(186,222,258), r));
		r = m0*v0;
		ass_true(CxEq(CxVec3(186,222,258), r));
		r = m0.transformTrans(v0);
		ass_true(CxEq(CxVec3(110,218,326), r));
		
		m0 = CxMat3(0.4f, -12, 1.3f,
						30, -33.3f, -10.1f,
						93, 0.1f, -0.3f);
		r = m0.transform(v0);
		ass_true(CxEq(CxVec3(1573.4f,-530.3f,-110.8f), r, 0.00001f));
		r = m0*v0;
		ass_true(CxEq(CxVec3(1573.4f,-530.3f,-110.8f), r, 0.00001f));
		CxMat3 m1 = m0.inverse();
		r = m1.transform(r);
		ass_true(CxEq(CxVec3(11,12,13), r, 0.00001f));
		r = m0*v0;
		r = m0.transformInv(r);
	   ass_true(CxEq(CxVec3(11,12,13), r, 0.00001f));
		r = m0.transformInv(v0);
		ass_true(CxEq(CxVec3(1.9250131762f, -1.052708702f, 0.449583396f), r, 0.00001f));
		r = m0.transformTrans(v0);
		ass_true(CxEq(CxVec3(-122.7f, -200.9f, 1020.3f), r, 0.0001f));
		
		FINISH_TEST;
	}

	void testCxMat3FromQuaternion() {
		BEGIN_TEST;
	   CxQuat q0(0.0691723f, 0.1383446f, 0.207516898f, 0.96592583f);
		CxMat3 m0(q0);
		ass_true(CxEq(m0, CxMat3(0.875595017799836f,
										 0.4200310908994311f,
										 -0.23855239986623264f,
										 -0.3817526348378421f,
										 0.9043038598460277f,
										 0.1910483050485956f,
										 0.29597008395861607f,
										 -0.07621293686382874f,
										 0.9521519299230139f),
						  0.0000001f));

		m0 = CxMat3(kCxIdentity);
		m0.setQuat(q0);
		ass_true(CxEq(m0, CxMat3(0.875595017799836f,
										 0.4200310908994311f,
										 -0.23855239986623264f,
										 -0.3817526348378421f,
										 0.9043038598460277f,
										 0.1910483050485956f,
										 0.29597008395861607f,
										 -0.07621293686382874f,
										 0.9521519299230139f),
						  0.0000001f));

		q0 = CxQuat(0.24691719f, 0.49383438247f, 0.74075157371f, 0.3826834324f);
		m0 = CxMat3(q0);
		ass_true(CxEq(m0, CxMat3(-0.5851705825303655f,
										 0.810819106826205f,
										 -0.012155877040681329f,
										 -0.32307431220147687f,
										 -0.21936198656181954f,
										 0.9205994284417055f,
										 0.7437730689777732f,
										 0.5426349554324782f,
										 0.39031900671909026f),
						  0.000001f));
		
		m0 = CxMat3(kCxIdentity);
		m0.setQuat(q0);
		ass_true(CxEq(m0, CxMat3(-0.5851705825303655f,
										 0.810819106826205f,
										 -0.012155877040681329f,
										 -0.32307431220147687f,
										 -0.21936198656181954f,
										 0.9205994284417055f,
										 0.7437730689777732f,
										 0.5426349554324782f,
										 0.39031900671909026f),
						  0.000001f));
		
	   q0 = CxQuat(0.9238795325f,0,0,0.3826834324f);
		m0 = CxMat3(q0);
		ass_true(CxEq(m0, CxMat3(1,0,0,
										 0, -0.7071067811865475f, 0.7071067811865476f,
										 0, -0.7071067811865476f, -0.7071067811865475f),
						  0.000001f));
		m0 = CxMat3(kCxIdentity);
		m0.setQuat(q0);
		ass_true(CxEq(m0, CxMat3(1,0,0,
										 0, -0.7071067811865475f, 0.7071067811865476f,
										 0, -0.7071067811865476f, -0.7071067811865475f),
						  0.000001f));

		FINISH_TEST;
	}

} // namespace cat


int main(int argc, char **argv) {
	cat::testCxMat3Basics();
	cat::testCxMat3Equality();
	cat::testCxMat3Arithmetic();
	cat::testCxMat3Mult();
	cat::testCxMat3DeterminantAndInverse();
	cat::testCxMat3Transform();
	cat::testCxMat3FromQuaternion();
	
	return 0;
}
