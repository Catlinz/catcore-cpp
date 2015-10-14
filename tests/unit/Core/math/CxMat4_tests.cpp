#include "core/math/CxMat4.h"
#include "core/CxTestCore.h"

namespace cat {

	/**
	 * @tests CxMat4()
	 * @tests CxMat4(CxZero)
	 * @tests CxMat4(CxIdentity)
	 * @tests CxMat4(const CxReal *)
	 * @tests CxMat4(const CxMat3 &)
	 * @tests CxMat4(const CxMat3 &, const CxVec3 &)
	 * @tests CxMat4(const CxVec4 &, const CxVec4 &, const CxVec4 &, const CxVec4 &)
	 * @tests CxMat4(const CxVec3 &, const CxVec3 &, const CxVec3 &, const CxVec3 &)
	 * @tests CxMat4(CxReal, CxReal, CxReal, CxReal, CxReal, CxReal, CxReal, CxReal, 
	 *               CxReal, CxReal, CxReal, CxReal, CxReal, CxReal, CxReal, CxReal)
	 * @tests CxMat4(const CxMat4 &)
	 * @tests operator=(const CxMat4 &)
	 * @tests operator[](CxI32)
	 * @tests operator[](CxI32) const
	 * @tests operator()(CxI32, CxI32)
	 * @tests operator()(CxI32, CxI32) const
	 * @tests constData() const
	 * @tests data()
	 * @tests data() const
	 * @tests diagonal() const
	 * @tests mat3() const
	 * @tests setTranslation(const CxVec3 &)
	 * @tests translation() const
	 * @tests transpose()
	 * @tests transposed() const
	 * @tests xAxis() const
	 * @tests yAxis() const
	 * @tests zAxis() const
	 */
	void testCxMat4Basics() {
		BEGIN_TEST;

		CxMat4 m0;
		CxMat4 m1(kCxZero);
		ass_true(m1[0] == 0 && m1[1] == 0 && m1[2] == 0 && m1[3] == 0 &&
					m1[4] == 0 && m1[5] == 0 && m1[6] == 0 && m1[7] == 0 &&
					m1[8] == 0 && m1[9] == 0 && m1[10] == 0 && m1[11] == 0 &&
					m1[12] == 0 && m1[13] == 0 && m1[14] == 0 && m1[15] == 0);

		CxMat4 m2(kCxIdentity);
		ass_true(m2[0] == 1 && m2[1] == 0 && m2[2] == 0 && m2[3] == 0 &&
					m2[4] == 0 && m2[5] == 1 && m2[6] == 0 && m2[7] == 0 &&
					m2[8] == 0 && m2[9] == 0 && m2[10] == 1 && m2[11] == 0 &&
					m2[12] == 0 && m2[13] == 0 && m2[14] == 0 && m2[15] == 1);
		ass_true(m2(0,0) == 1 && m2(0,1) == 0 && m2(0,2) == 0 && m2(0,3) == 0 &&
					m2(1,0) == 0 && m2(1,1) == 1 && m2(1,2) == 0 && m2(1,3) == 0 &&
					m2(2,0) == 0 && m2(2,1) == 0 && m2(2,2) == 1 && m2(2,3) == 0 &&
					m2(3,0) == 0 && m2(3,1) == 0 && m2(3,2) == 0 && m2(3,3) == 1);
		ass_true(m2.col0 == CxVec4(1,0,0,0));
		ass_true(m2.col1 == CxVec4(0,1,0,0));
		ass_true(m2.col2 == CxVec4(0,0,1,0));
		ass_true(m2.col3 == CxVec4(0,0,0,1));

		CxMat4 m3(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		ass_true(m3[0] == 1 && m3[1] == 2 && m3[2] == 3 && m3[3] == 4 &&
					m3[4] == 5 && m3[5] == 6 && m3[6] == 7 && m3[7] == 8 &&
					m3[8] == 9 && m3[9] == 10 && m3[10] == 11 && m3[11] == 12 &&
					m3[12] == 13 && m3[13] == 14 && m3[14] == 15 && m3[15] == 16);
		ass_true(m3(0,0) == 1 && m3(0,1) == 2 && m3(0,2) == 3 && m3(0,3) == 4 &&
					m3(1,0) == 5 && m3(1,1) == 6 && m3(1,2) == 7 && m3(1,3) == 8 &&
					m3(2,0) == 9 && m3(2,1) == 10 && m3(2,2) == 11 && m3(2,3) == 12 &&
					m3(3,0) == 13 && m3(3,1) == 14 && m3(3,2) == 15 && m3(3,3) == 16);
		ass_true(m3.col0 == CxVec4(1,2,3,4));
		ass_true(m3.col1 == CxVec4(5,6,7,8));
		ass_true(m3.col2 == CxVec4(9,10,11,12));
		ass_true(m3.col3 == CxVec4(13,14,15,16));

		const CxReal rd[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
		CxMat4 m4(rd);
		ass_true(m4[0] == 1 && m4[1] == 2 && m4[2] == 3 && m4[3] == 4 &&
					m4[4] == 5 && m4[5] == 6 && m4[6] == 7 && m4[7] == 8 &&
					m4[8] == 9 && m4[9] == 10 && m4[10] == 11 && m4[11] == 12 &&
					m4[12] == 13 && m4[13] == 14 && m4[14] == 15 && m4[15] == 16);
		ass_true(m4.col0 == CxVec4(1,2,3,4));
		ass_true(m4.col1 == CxVec4(5,6,7,8));
		ass_true(m4.col2 == CxVec4(9,10,11,12));
		ass_true(m4.col3 == CxVec4(13,14,15,16));

		CxMat3 m30(1,2,3,4,5,6,7,8,9);
		CxMat4 m4a(m30);
		ass_true(m4a[0] == 1 && m4a[1] == 2 && m4a[2] == 3 && m4a[3] == 0 &&
					m4a[4] == 4 && m4a[5] == 5 && m4a[6] == 6 && m4a[7] == 0 &&
					m4a[8] == 7 && m4a[9] == 8 && m4a[10] == 9 && m4a[11] == 0 &&
					m4a[12] == 0 && m4a[13] == 0 && m4a[14] == 0 && m4a[15] == 1);

		CxMat4 m4b(m30, CxVec3(20,21,22));
		ass_true(m4b[0] == 1 && m4b[1] == 2 && m4b[2] == 3 && m4b[3] == 0 &&
					m4b[4] == 4 && m4b[5] == 5 && m4b[6] == 6 && m4b[7] == 0 &&
					m4b[8] == 7 && m4b[9] == 8 && m4b[10] == 9 && m4b[11] == 0 &&
					m4b[12] == 20 && m4b[13] == 21 && m4b[14] == 22 && m4b[15] == 1);

		CxMat4 m5(CxVec4(1,2,3,4), CxVec4(5,6,7,8),
					 CxVec4(9,10,11,12), CxVec4(13,14,15,16));
		ass_true(m5[0] == 1 && m5[1] == 2 && m5[2] == 3 && m5[3] == 4 &&
					m5[4] == 5 && m5[5] == 6 && m5[6] == 7 && m5[7] == 8 &&
					m5[8] == 9 && m5[9] == 10 && m5[10] == 11 && m5[11] == 12 &&
					m5[12] == 13 && m5[13] == 14 && m5[14] == 15 && m5[15] == 16);

		CxMat4 m5a(CxVec3(1,2,3), CxVec3(4,5,6), CxVec3(7,8,9), CxVec3(10,11,12));
		ass_true(m5a[0] == 1 && m5a[1] == 2 && m5a[2] == 3 && m5a[3] == 0 &&
					m5a[4] == 4 && m5a[5] == 5 && m5a[6] == 6 && m5a[7] == 0 &&
					m5a[8] == 7 && m5a[9] == 8 && m5a[10] == 9 && m5a[11] == 0 &&
					m5a[12] == 10 && m5a[13] == 11 && m5a[14] == 12 && m5a[15] == 1);

		CxMat4 m6(m5);
		ass_true(m6[0] == 1 && m6[1] == 2 && m6[2] == 3 && m6[3] == 4 &&
					m6[4] == 5 && m6[5] == 6 && m6[6] == 7 && m6[7] == 8 &&
					m6[8] == 9 && m6[9] == 10 && m6[10] == 11 && m6[11] == 12 &&
					m6[12] == 13 && m6[13] == 14 && m6[14] == 15 && m6[15] == 16);
		
		m0 = m5;
	   ass_true(m0[0] == 1 && m0[1] == 2 && m0[2] == 3 && m0[3] == 4 &&
					m0[4] == 5 && m0[5] == 6 && m0[6] == 7 && m0[7] == 8 &&
					m0[8] == 9 && m0[9] == 10 && m0[10] == 11 && m0[11] == 12 &&
					m0[12] == 13 && m0[13] == 14 && m0[14] == 15 && m0[15] == 16);

		const CxMat4 mc0(m5);
		ass_true(mc0[0] == 1 && mc0[1] == 2 && mc0[2] == 3 && mc0[3] == 4 &&
					mc0[4] == 5 && mc0[5] == 6 && mc0[6] == 7 && mc0[7] == 8 &&
					mc0[8] == 9 && mc0[9] == 10 && mc0[10] == 11 && mc0[11] == 12 &&
					mc0[12] == 13 && mc0[13] == 14 && mc0[14] == 15 && mc0[15] == 16);
		ass_true(mc0(0,0) == 1 && mc0(0,1) == 2 && mc0(0,2) == 3 && mc0(0,3) == 4 &&
					mc0(1,0) == 5 && mc0(1,1) == 6 && mc0(1,2) == 7 && mc0(1,3) == 8 &&
					mc0(2,0) == 9 && mc0(2,1) == 10 && mc0(2,2) == 11 && mc0(2,3) == 12 &&
					mc0(3,0) == 13 && mc0(3,1) == 14 && mc0(3,2) == 15 && mc0(3,3) == 16);

		CxReal *d = m6.data();
		ass_true(d[0] == 1 && d[1] == 2 && d[2] == 3 && d[3] == 4 &&
					d[4] == 5 && d[5] == 6 && d[6] == 7 && d[7] == 8 &&
					d[8] == 9 && d[9] == 10 && d[10] == 11 && d[11] == 12 &&
					d[12] == 13 && d[13] == 14 && d[14] == 15 && d[15] == 16);

		const CxReal *dc0 = mc0.data();
	   ass_true(dc0[0] == 1 && dc0[1] == 2 && dc0[2] == 3 && dc0[3] == 4 &&
					dc0[4] == 5 && dc0[5] == 6 && dc0[6] == 7 && dc0[7] == 8 &&
					dc0[8] == 9 && dc0[9] == 10 && dc0[10] == 11 && dc0[11] == 12 &&
					dc0[12] == 13 && dc0[13] == 14 && dc0[14] == 15 && dc0[15] == 16);

		const CxReal *dc1 = mc0.constData();
		ass_true(dc1[0] == 1 && dc1[1] == 2 && dc1[2] == 3 && dc1[3] == 4 &&
					dc1[4] == 5 && dc1[5] == 6 && dc1[6] == 7 && dc1[7] == 8 &&
					dc1[8] == 9 && dc1[9] == 10 && dc1[10] == 11 && dc1[11] == 12 &&
					dc1[12] == 13 && dc1[13] == 14 && dc1[14] == 15 && dc1[15] == 16);

		const CxReal *dc2 = m6.constData();
	   ass_true(dc2[0] == 1 && dc2[1] == 2 && dc2[2] == 3 && dc2[3] == 4 &&
					dc2[4] == 5 && dc2[5] == 6 && dc2[6] == 7 && dc2[7] == 8 &&
					dc2[8] == 9 && dc2[9] == 10 && dc2[10] == 11 && dc2[11] == 12 &&
					dc2[12] == 13 && dc2[13] == 14 && dc2[14] == 15 && dc2[15] == 16);

		m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		m1 = m0.transposed();
		ass_true(m1[0] == 1 && m1[1] == 5 && m1[2] == 9 && m1[3] == 13 &&
					m1[4] == 2 && m1[5] == 6 && m1[6] == 10 && m1[7] == 14 &&
					m1[8] == 3 && m1[9] == 7 && m1[10] == 11 && m1[11] == 15 &&
					m1[12] == 4 && m1[13] == 8 && m1[14] == 12 && m1[15] == 16);
		ass_true(m1.col0 == CxVec4(1,5,9,13));
		ass_true(m1.col1 == CxVec4(2,6,10,14));
		ass_true(m1.col2 == CxVec4(3,7,11,15));
		ass_true(m1.col3 == CxVec4(4,8,12,16));
		ass_true(m0[0] == 1 && m0[1] == 2 && m0[2] == 3 && m0[3] == 4 &&
					m0[4] == 5 && m0[5] == 6 && m0[6] == 7 && m0[7] == 8 &&
					m0[8] == 9 && m0[9] == 10 && m0[10] == 11 && m0[11] == 12 &&
					m0[12] == 13 && m0[13] == 14 && m0[14] == 15 && m0[15] == 16);
		m0.transpose();
	   ass_true(m0[0] == 1 && m0[1] == 5 && m0[2] == 9 && m0[3] == 13 &&
					m0[4] == 2 && m0[5] == 6 && m0[6] == 10 && m0[7] == 14 &&
					m0[8] == 3 && m0[9] == 7 && m0[10] == 11 && m0[11] == 15 &&
					m0[12] == 4 && m0[13] == 8 && m0[14] == 12 && m0[15] == 16);
		ass_true(m0.col0 == CxVec4(1,5,9,13));
		ass_true(m0.col1 == CxVec4(2,6,10,14));
		ass_true(m0.col2 == CxVec4(3,7,11,15));
		ass_true(m0.col3 == CxVec4(4,8,12,16));

		m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		m1 = m0.diagonal();
	   ass_true(m1[0] == 1 && m1[1] == 0 && m1[2] == 0 && m1[3] == 0 &&
					m1[4] == 0 && m1[5] == 6 && m1[6] == 0 && m1[7] == 0 &&
					m1[8] == 0 && m1[9] == 0 && m1[10] == 11 && m1[11] == 0 &&
					m1[12] == 0 && m1[13] == 0 && m1[14] == 0 && m1[15] == 16);
	   ass_true(m0[0] == 1 && m0[1] == 2 && m0[2] == 3 && m0[3] == 4 &&
					m0[4] == 5 && m0[5] == 6 && m0[6] == 7 && m0[7] == 8 &&
					m0[8] == 9 && m0[9] == 10 && m0[10] == 11 && m0[11] == 12 &&
					m0[12] == 13 && m0[13] == 14 && m0[14] == 15 && m0[15] == 16);

		m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		CxMat3 m31 = m0.mat3();
		ass_true(m31[0] == 1 && m31[1] == 2 && m31[2] == 3 &&
					m31[3] == 5 && m31[4] == 6 && m31[5] == 7 &&
					m31[6] == 9 && m31[7] == 10 && m31[8] == 11);

		m0.setTranslation(CxVec3(20,21,22));
		ass_true(m0[0] == 1 && m0[1] == 2 && m0[2] == 3 && m0[3] == 4 &&
					m0[4] == 5 && m0[5] == 6 && m0[6] == 7 && m0[7] == 8 &&
					m0[8] == 9 && m0[9] == 10 && m0[10] == 11 && m0[11] == 12 &&
					m0[12] == 20 && m0[13] == 21 && m0[14] == 22 && m0[15] == 16);

		CxVec3 tr = m0.translation();
		ass_true(tr.x == 20 && tr.y == 21 && tr.z == 22);

		m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		ass_true(m0.xAxis() == CxVec3(1,2,3));
		ass_true(m0.yAxis() == CxVec3(5,6,7));
		ass_true(m0.zAxis() == CxVec3(9,10,11));
		
		FINISH_TEST;
	}

	/**
	 * @tests operator==(const CxMat4 &) const
	 * @tests operator!=(const CxMat4 &) const
	 * @tests CxEq(const CxMat4 &, const CxMat4 &, CxReal)
	 * @tests CxIsFinite(const CxMat4 &)
	 */
	void testCxMat4Equality() {
		BEGIN_TEST;

		CxMat4 m0(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		CxMat4 m1(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);

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

		m1 = m0;  m1[9] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[9] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[9] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[10] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[10] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[10] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[11] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[11] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[11] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[12] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[12] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[12] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[13] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[13] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[13] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[14] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[14] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[14] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;  m1[15] = 0;
		ass_false(m0 == m1 || m1 == m0);
		ass_true(m0 != m1 && m1 != m0);
		ass_false(CxEq(m0, m1) || CxEq(m1, m0));
		m1[15] = CX_INF;  ass_false(CxIsFinite(m1));
		m1[15] = CX_NAN;  ass_false(CxIsFinite(m1));

		m1 = m0;
		m1[0] = 1.0001f;
		ass_true(CxEq(m1,m0,0.001f) && CxEq(m0,m1,0.001f));
		ass_false(CxEq(m1,m0,0.0001f) || CxEq(m0,m1,0.0001f));

		
		FINISH_TEST;
	}

	/**
	 * @tests operator-() const
	 * @tests operator+(const CxMat4 &) const
	 * @tests operator-(const CxMat4 &) const
	 * @tests operator*(CxReal) const
	 * @tests opeartor/(CxReal) const
	 * @tests operator+=(const CxMat4 &)
	 * @tests opeartor-=(const CxMat4 &)
	 * @tests operator*=(CxReal)
	 * @tests operator/=(CxReal)
	 */
	void testCxMat4Arithmetic() {
		BEGIN_TEST;

		/* test operator-() */
		CxMat4 m0(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		CxMat4 m1 = -m0;
		ass_true(m1[0] == -1 && m1[1] == -2 && m1[2] == -3 && m1[3] == -4 &&
					m1[4] == -5 && m1[5] == -6 && m1[6] == -7 && m1[7] == -8 &&
					m1[8] == -9 && m1[9] == -10 && m1[10] == -11 && m1[11] == -12 &&
					m1[12] == -13 && m1[13] == -14 && m1[14] == -15 && m1[15] == -16);
		ass_true(m0[0] == 1 && m0[1] == 2 && m0[2] == 3 && m0[3] == 4 &&
					m0[4] == 5 && m0[5] == 6 && m0[6] == 7 && m0[7] == 8 &&
					m0[8] == 9 && m0[9] == 10 && m0[10] == 11 && m0[11] == 12 &&
					m0[12] == 13 && m0[13] == 14 && m0[14] == 15 && m0[15] == 16);
		ass_false(m0 == m1);
		m1 = -m1;
		ass_true(m0 == m1);

		/* test operator+(const CxMat4 &) */
		m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		m1 = CxMat4(17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32);
		CxMat4 r = m0 + m1;
		ass_true(r[0] == 18 && r[1] == 20 && r[2] == 22 && r[3] == 24 &&
					r[4] == 26 && r[5] == 28 && r[6] == 30 && r[7] == 32 &&
					r[8] == 34 && r[9] == 36 && r[10] == 38 && r[11] == 40 &&
					r[12] == 42 && r[13] == 44 && r[14] == 46 && r[15] == 48);

		m1 = CxMat4(-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32);
		r = m0 + m1;
		ass_true(r[0] == -16 && r[1] == -16 && r[2] == -16 && r[3] == -16 &&
					r[4] == -16 && r[5] == -16 && r[6] == -16 && r[7] == -16 &&
					r[8] == -16 && r[9] == -16 && r[10] == -16 && r[11] == -16 &&
					r[12] == -16 && r[13] == -16 && r[14] == -16 && r[15] == -16);

		/* test operator-(const CxMat4 &) */
		m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		m1 = CxMat4(17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32);
		
		r = m0 - m1;
		ass_true(r[0] == -16 && r[1] == -16 && r[2] == -16 && r[3] == -16 &&
					r[4] == -16 && r[5] == -16 && r[6] == -16 && r[7] == -16 &&
					r[8] == -16 && r[9] == -16 && r[10] == -16 && r[11] == -16 &&
					r[12] == -16 && r[13] == -16 && r[14] == -16 && r[15] == -16);

		m1 = CxMat4(-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32);
		r = m0 - m1;
		ass_true(r[0] == 18 && r[1] == 20 && r[2] == 22 && r[3] == 24 &&
					r[4] == 26 && r[5] == 28 && r[6] == 30 && r[7] == 32 &&
					r[8] == 34 && r[9] == 36 && r[10] == 38 && r[11] == 40 &&
					r[12] == 42 && r[13] == 44 && r[14] == 46 && r[15] == 48);

		/* test operator*(CxReal) */
	   m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		r = m0*10;
		ass_true(r[0] == 10 && r[1] == 20 && r[2] == 30 && r[3] == 40 &&
					r[4] == 50 && r[5] == 60 && r[6] == 70 && r[7] == 80 &&
					r[8] == 90 && r[9] == 100 && r[10] == 110 && r[11] == 120 &&
					r[12] == 130 && r[13] == 140 && r[14] == 150 && r[15] == 160);

		/* test operator/(CxReal) */
	   m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		r = m0/10;
		ass_true(CxEq(CxMat4(.1f,.2f,.3f,.4f,.5f,.6f,.7f,.8f,
									.9f,1.0f,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f), r, 0.000001f));

		/* test operator+=(const CxMat4 &) */
		m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		m1 = CxMat4(17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32);
		r = m0;
		r += m1;
		ass_true(r[0] == 18 && r[1] == 20 && r[2] == 22 && r[3] == 24 &&
					r[4] == 26 && r[5] == 28 && r[6] == 30 && r[7] == 32 &&
					r[8] == 34 && r[9] == 36 && r[10] == 38 && r[11] == 40 &&
					r[12] == 42 && r[13] == 44 && r[14] == 46 && r[15] == 48);

		m1 = CxMat4(-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32);
		r = m0;
		r += m1;
		ass_true(r[0] == -16 && r[1] == -16 && r[2] == -16 && r[3] == -16 &&
					r[4] == -16 && r[5] == -16 && r[6] == -16 && r[7] == -16 &&
					r[8] == -16 && r[9] == -16 && r[10] == -16 && r[11] == -16 &&
					r[12] == -16 && r[13] == -16 && r[14] == -16 && r[15] == -16);

		/* test operator-=(const CxMat4 &) */
	   m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		m1 = CxMat4(17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32);
		r = m0;
		r -= m1;
		ass_true(r[0] == -16 && r[1] == -16 && r[2] == -16 && r[3] == -16 &&
					r[4] == -16 && r[5] == -16 && r[6] == -16 && r[7] == -16 &&
					r[8] == -16 && r[9] == -16 && r[10] == -16 && r[11] == -16 &&
					r[12] == -16 && r[13] == -16 && r[14] == -16 && r[15] == -16);

		m1 = CxMat4(-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32);
		r = m0;
		r -= m1;
		ass_true(r[0] == 18 && r[1] == 20 && r[2] == 22 && r[3] == 24 &&
					r[4] == 26 && r[5] == 28 && r[6] == 30 && r[7] == 32 &&
					r[8] == 34 && r[9] == 36 && r[10] == 38 && r[11] == 40 &&
					r[12] == 42 && r[13] == 44 && r[14] == 46 && r[15] == 48);

		/* test operator*=(CxReal) */
	   m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		r = m0;
		r *= 10;
		ass_true(r[0] == 10 && r[1] == 20 && r[2] == 30 && r[3] == 40 &&
					r[4] == 50 && r[5] == 60 && r[6] == 70 && r[7] == 80 &&
					r[8] == 90 && r[9] == 100 && r[10] == 110 && r[11] == 120 &&
					r[12] == 130 && r[13] == 140 && r[14] == 150 && r[15] == 160);

		/* test operator/=(CxReal) */
		m0 = CxMat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
		r = m0;
		r /= 10;
		ass_true(CxEq(CxMat4(.1f,.2f,.3f,.4f,.5f,.6f,.7f,.8f,
									.9f,1.0f,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f), r, 0.000001f));
		FINISH_TEST;
	}

	/**
	 * @tests operator*(const CxMat4 &) const
	 * @tests operator*=(const CxMat4 &)
	 * @tests multTx(const CxMat4 &) const
	 * @tests multByTx(const CxMat4 &)
	 */
	void testCxMat4Mult() {
		BEGIN_TEST;

	   CxMat4 m0(2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17);
		CxMat4 m1(18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33);
		CxMat4 r = m0*m1;
		ass_true(CxEq(CxMat4(644,722,800,878,
									772,866,960,1054,
									900,1010,1120,1230,
									1028,1154,1280,1406), r, 0.00001f));

		r = m1*m0;
		ass_true(r[0] == 356 && r[1] == 370 && r[2] == 384 && r[3] == 398 &&
					r[4] == 740 && r[5] == 770 && r[6] == 800 && r[7] == 830 &&
					r[8] == 1124 && r[9] == 1170 && r[10] == 1216 && r[11] == 1262 &&
					r[12] == 1508 && r[13] == 1570 && r[14] == 1632 && r[15] == 1694);

		r = m0;
		r *= m1;
		ass_true(r[0] == 644 && r[1] == 722 && r[2] == 800 && r[3] == 878 &&
					r[4] == 772 && r[5] == 866 && r[6] == 960 && r[7] == 1054 &&
					r[8] == 900 && r[9] == 1010 && r[10] == 1120 && r[11] == 1230 &&
					r[12] == 1028 && r[13] == 1154 && r[14] == 1280 && r[15] == 1406);

		r = m1;
		r *= m0;
		ass_true(r[0] == 356 && r[1] == 370 && r[2] == 384 && r[3] == 398 &&
					r[4] == 740 && r[5] == 770 && r[6] == 800 && r[7] == 830 &&
					r[8] == 1124 && r[9] == 1170 && r[10] == 1216 && r[11] == 1262 &&
					r[12] == 1508 && r[13] == 1570 && r[14] == 1632 && r[15] == 1694);

		r = m0.multTx(m1);
		ass_true(r[0] == 350 && r[1] == 407 && r[2] == 464 && r[3] == 0 &&
					r[4] == 422 && r[5] == 491 && r[6] == 560 && r[7] == 0 &&
					r[8] == 494 && r[9] == 575 && r[10] == 656 && r[11] == 0 &&
					r[12] == 580 && r[13] == 674 && r[14] == 768 && r[15] == 1);

		r = m1.multTx(m0);
		ass_true(r[0] == 206 && r[1] == 215 && r[2] == 224 && r[3] == 0 &&
					r[4] == 470 && r[5] == 491 && r[6] == 512 && r[7] == 0 &&
					r[8] == 734 && r[9] == 767 && r[10] == 800 && r[11] == 0 &&
					r[12] == 1028 && r[13] == 1074 && r[14] == 1120 && r[15] == 1);

		r = m0;
		r.multByTx(m1);
		ass_true(r[0] == 350 && r[1] == 407 && r[2] == 464 && r[3] == 5 &&
					r[4] == 422 && r[5] == 491 && r[6] == 560 && r[7] == 9 &&
					r[8] == 494 && r[9] == 575 && r[10] == 656 && r[11] == 13 &&
					r[12] == 580 && r[13] == 674 && r[14] == 768 && r[15] == 17);

		r = m1;
		r.multByTx(m0);
		ass_true(r[0] == 206 && r[1] == 215 && r[2] == 224 && r[3] == 21 &&
					r[4] == 470 && r[5] == 491 && r[6] == 512 && r[7] == 25 &&
					r[8] == 734 && r[9] == 767 && r[10] == 800 && r[11] == 29 &&
					r[12] == 1028 && r[13] == 1074 && r[14] == 1120 && r[15] == 33);

		r = m0;
		r(0,3) = r(1,3) = r(2,3) = 0;
		r(3,3) = 1;
		r.multByTx(m1);
		ass_true(r[0] == 350 && r[1] == 407 && r[2] == 464 && r[3] == 0 &&
					r[4] == 422 && r[5] == 491 && r[6] == 560 && r[7] == 0 &&
					r[8] == 494 && r[9] == 575 && r[10] == 656 && r[11] == 0 &&
					r[12] == 580 && r[13] == 674 && r[14] == 768 && r[15] == 1);

		r = m1;
		r(0,3) = r(1,3) = r(2,3) = 0;
		r(3,3) = 1;
		r.multByTx(m0);
		ass_true(r[0] == 206 && r[1] == 215 && r[2] == 224 && r[3] == 0 &&
					r[4] == 470 && r[5] == 491 && r[6] == 512 && r[7] == 0 &&
					r[8] == 734 && r[9] == 767 && r[10] == 800 && r[11] == 0 &&
					r[12] == 1028 && r[13] == 1074 && r[14] == 1120 && r[15] == 1);

		FINISH_TEST;
	}

	/**
	 * @tests determinant() const
	 * @tests determinant3x3() const
	 * @tests inverse() const
	 * @tests invert()
	 */
	void testCxMat4DeterminantAndInverse() {
		BEGIN_TEST;

		CxMat4 m0(kCxIdentity);
		ass_true(CxEq(m0.determinant(), 1, 0.0000001f));
		ass_true(CxEq(m0.determinant3x3(), 1, 0.0000001f));
		ass_true(CxEq(m0.inverse(), m0, 0.00000001f));
		m0.invert();
		ass_true(CxEq(m0, CxMat4(kCxIdentity), 0.0000001f));
		
		m0 = CxMat4(2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17);
		ass_true(CxEq(m0.determinant(), 0, 0.0000001f));
		ass_true(CxEq(m0.determinant3x3(), 0, 0.0000001f));
		ass_true(CxEq(m0.inverse(), CxMat4(kCxIdentity)));
		m0.invert();
		ass_true(CxEq(m0, CxMat4(2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17)));
		
		m0 = CxMat4(0.4f, -2.1f, 1.3f, -0.8f,
						30, -3.3f, -10.1f, 0.23f,
						7.4f, 0.1f, -0.3f, -2.4f,
						8.2f, 4.3f, 20.2f, -0.1f);
		ass_true(CxEq(m0.determinant(), 4231.85134f, 0.001f));
		ass_true(CxEq(m0.determinant3x3(), 174.5f, 0.00001f));
		CxMat4 m1 = m0.inverse();
		ass_true(CxEq(CxMat4(-0.0130417f, 0.0278653f, 0.0063982f, 0.0148670f,
									-0.3973533f, -0.0299562f, 0.1290590f, 0.0125109f,
									0.0895428f, -0.0045129f, -0.0319889f, 0.0410108f,
									-0.0679613f, 0.0852341f, -0.3875628f, 0.0412349f), m1, 0.00001f));
		ass_true(CxEq(m1.determinant(), 0.0002363031968f, 0.0000001f));
		ass_true(CxEq(m1.determinant3x3(), -0.00002363031968f, 0.000000001f));
		ass_true(CxEq(m0.inverse().determinant(), 0.0002363031968f, 0.00000001f));
		m1.invert();
		ass_true(CxEq(m1, m0, 0.00001f));
		m0.invert();
		ass_true(CxEq(CxMat4(-0.0130417f, 0.0278653f, 0.0063982f, 0.0148670f,
									-0.3973533f, -0.0299562f, 0.1290590f, 0.0125109f,
									0.0895428f, -0.0045129f, -0.0319889f, 0.0410108f,
									-0.0679613f, 0.0852341f, -0.3875628f, 0.0412349f), m0, 0.00001f));

		FINISH_TEST;
	}

	/**
	 * @tests setOrtho(CxReal, CxReal, CxReal, CxReal, CxReal, CxReal)
	 * @tests setOrtho(CxReal, CxReal, CxReal, CxReal)
	 * @tests setPerspective(CxReal, CxReal, CxReal, CxReal, CxReal, CxReal)
	 * @tests setPerspective(CxReal, CxReal, CxReal, CxReal)
	 */
	void testCxMat4Projection() {
		BEGIN_TEST;

		/* Test setOrtho(6) */
		CxMat4 m0;
		m0.setOrtho(1,-1,1,-1,1,100);
		ass_true(CxEq(CxMat4(1,0,0,0,
									0,1,0,0,
									0,0,-0.02020202020f,0,
									0,0,-1.0202020202020f,1),
						  m0, 0.000001f));

		m0.setOrtho(512, -512, 512, -512, 1, 1000);
		ass_true(CxEq(CxMat4(0.001953125f,0,0,0,
									0,0.001953125f,0,0,
									0,0,-0.002002002002f,0,
									0,0,-1.002002002002f,1),
						  m0, 0.0000001f));

		m0.setOrtho(512, -512, 200, -200, 1, 1000);
		ass_true(CxEq(CxMat4(0.001953125f,0,0,0,
									0,0.005f,0,0,
									0,0,-0.002002002002f,0,
									0,0,-1.002002002002f,1),
						  m0, 0.0000001f));

		m0.setOrtho(512, -200, 400, 100, 1, 1000);
		ass_true(CxEq(CxMat4(0.002808988764f,0,0,0,
									0,0.0066666666667f,0,0,
									0,0,-0.002002002002f,0,
									-0.438202247191f,-1.66666666667f,-1.002002002002f,1),
						  m0, 0.000001f));

		m0.setOrtho(512, -200, 400, 100, 5, 100);
		ass_true(CxEq(CxMat4(0.002808988764f,0,0,0,
									0,0.0066666666667f,0,0,
									0,0,-0.021052631578947f,0,
									-0.438202247191f,-1.66666666667f,-1.105263157894737f,1),
						  m0, 0.000001f));

		/* test setOrtho(4) */
		m0.setOrtho(2,2,1,100);
		ass_true(CxEq(CxMat4(1,0,0,0,
									0,1,0,0,
									0,0,-0.02020202020f,0,
									0,0,-1.0202020202020f,1),
						  m0, 0.000001f));

		m0.setOrtho(1024, 1024, 1, 1000);
		ass_true(CxEq(CxMat4(0.001953125f,0,0,0,
									0,0.001953125f,0,0,
									0,0,-0.002002002002f,0,
									0,0,-1.002002002002f,1),
						  m0, 0.0000001f));

		m0.setOrtho(1024, 400, 1, 1000);
		ass_true(CxEq(CxMat4(0.001953125f,0,0,0,
									0,0.005f,0,0,
									0,0,-0.002002002002f,0,
									0,0,-1.002002002002f,1),
						  m0, 0.0000001f));

		m0.setOrtho(1024, 400, 5, 100);
		ass_true(CxEq(CxMat4(0.001953125f,0,0,0,
									0,0.005f,0,0,
									0,0,-0.021052631578947f,0,
									0,0,-1.105263157894737f,1),
						  m0, 0.000001f));
		
		/* test setPerspective(6) */
		m0.setPerspective(1,-1,1,-1,1,100);
		ass_true(CxEq(CxMat4(1,0,0,0,
									0,1,0,0,
									0,0,-1.020202020202f,-1,
									0,0,-2.020202020202f,0),
						  m0, 0.00000001f));

		m0.setPerspective(512,-512,512,-512,1,1000);
		ass_true(CxEq(CxMat4(0.001953125f,0,0,0,
									0,0.001953125f,0,0,
									0,0,-1.002002002002f,-1,
									0,0,-2.002002002002f,0),
						  m0, 0.00000001f));

		m0.setPerspective(512,-512,200,-200,1,1000);
		ass_true(CxEq(CxMat4(0.001953125f,0,0,0,
									0,0.005f,0,0,
									0,0,-1.002002002002f,-1,
									0,0,-2.002002002002f,0),
						  m0, 0.00000001f));

		m0.setPerspective(512,-200,400,100,1,1000);
		ass_true(CxEq(CxMat4(0.002808988764f,0,0,0,
									0,0.0066666666667f,0,0,
									0.438202247191f,1.66666666667f,-1.002002002002f,-1,
									0,0,-2.002002002002f,0),
						  m0, 0.000001f));

		CxReal t = 2.07106781186548f;
		CxReal r = 3.10660171779821f;
		m0.setPerspective(r,-r,t,-t,5,100);
		ass_true(CxEq(CxMat4(1.60947571f,0,0,0,
									0,2.41421356f,0,0,
									0,0,-1.105263158f,-1,
									0,0,-10.52631579f,0),
						  m0, 0.000001f));

		/* test setPerspective(4) */
		m0.setPerspective(45.0f,1.5f,1,1000);
		ass_true(CxEq(CxMat4(1.60947570825f,0,0,0,
									0,2.41421356237f,0,0,
									0,0,-1.002002002002f,-1,
									0,0,-2.002002002002f,0),
						  m0, 0.000001f));

		m0.setPerspective(45.0f,1.5f,5,100);
		ass_true(CxEq(CxMat4(1.60947571f,0,0,0,
									0,2.41421356f,0,0,
									0,0,-1.105263158f,-1,
									0,0,-10.52631579f,0),
						  m0, 0.000001f));

		FINISH_TEST;
	}

	/**
	 * @tests operator*(const CxVec3 &) const
	 * @tests operator*(const CxVec4 &) const
	 * @tests transform(const CxVec4 &) const
	 * @tests transform(const CxVec3 &) const
	 * @tests transformInv(const CxVec3 &) const
	 * @tests transformTrans(const CxVec3 &) const
	 * @tests transform3x3(const CxVec3 &) const
	 * @tests transform3x3Inv(const CxVec3 &) const
	 * @tests transform3x3Trans(const CxVec3 &) const
	 */
	void testCxMat4Transform() {
		BEGIN_TEST;

		CxMat4 m0(kCxIdentity);
		CxVec4 v4(4,5,6,7);
		CxVec3 v3(4,5,6);
		CxVec4 r4 = m0.transform(v4);
		ass_true(CxEq(v4, r4));
		r4 = m0*v4;  ass_true(CxEq(v4, r4));
		CxVec3 r3 = m0.transform(v3);
		ass_true(CxEq(v3, r3));
		r3 = m0*v3;  ass_true(CxEq(v3, r3));
		r3 = m0.transformInv(v3);  ass_true(CxEq(v3, r3));
		r3 = m0.transformTrans(v3);  ass_true(CxEq(v3,r3));
		r3 = m0.transform3x3(v3);  ass_true(CxEq(v3,r3));
		r3 = m0.transform3x3Trans(v3);  ass_true(CxEq(v3,r3));
		r3 = m0.transform3x3Inv(v3);  ass_true(CxEq(v3,r3));

		
		m0 = CxMat4(.2f,.3f,.4f,.5f,.6f,.7f,.8f,.9f,1.0f,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f);
		r4 = m0.transform(v4);
		ass_true(CxEq(CxVec4(19.6f,21.8f,24,26.2f), r4, 0.0000001f));
		r4 = m0*v4;
		ass_true(CxEq(CxVec4(19.6f,21.8f,24,26.2f), r4, 0.000001f));
		r3 = m0.transform(v3);
		ass_true(CxEq(CxVec3(11.2f, 12.8f, 14.4f), r3, 0.000001f));
		r3 = m0*v3;
		ass_true(CxEq(CxVec3(11.2f, 12.8f, 14.4f), r3, 0.000001f));
		r3 = m0.transformTrans(v3);
		ass_true(CxEq(CxVec3(5.2f, 11.6f, 18), r3, 0.0000001f));
		r3 = m0.transform3x3(v3);
		ass_true(CxEq(CxVec3(9.8f, 11.3f, 12.8f), r3, 0.0000001f));
		r3 = m0.transform3x3Trans(v3);
		ass_true(CxEq(CxVec3(4.7f, 10.7f, 16.7f), r3, 0.000001f));
		
	   m0 = CxMat4(0.4f, -2.1f, 1.3f, -0.8f,
						30, -3.3f, -10.1f, 0.23f,
						7.4f, 0.1f, -0.3f, -2.4f,
						8.2f, 4.3f, 20.2f, -0.1f);
		r4 = m0.transform(v4);
		ass_true(CxEq(CxVec4(253.4f,5.8f,94.3f,-17.15f), r4, 0.00001f));
		r4 = m0*v4;
		ass_true(CxEq(CxVec4(253.4f,5.8f,94.3f,-17.15f), r4, 0.00001f));
		r3 = m0.transform(v3);
		ass_true(CxEq(CxVec3(204.2f, -20, -26.9f), r3, 0.00001f));
		r3 = m0*v3;
		ass_true(CxEq(CxVec3(204.2f, -20, -26.9f), r3, 0.00001f));
		r3 = m0.transformTrans(v3);
		ass_true(CxEq(CxVec3(-1.9f, 43.13f, 25.9f), r3, 0.00001f));
		r3 = m0.transform3x3(v3);
		ass_true(CxEq(CxVec3(196, -24.3f, -47.1f), r3, 0.0000001f));
		r3 = m0.transform3x3Trans(v3);
		ass_true(CxEq(CxVec3(-1.1f, 42.9f, 28.3f), r3, 0.00001f));
		
		CxMat4 m1 = m0.inverse();
		r4 = m1.transform(v4);
		ass_true(CxEq(CxVec4(-1.97740542559f, 0.531241724f, -2.233984665f, 0.65673133972f),
						  r4, 0.00001f));
		r3 = m1.transform(v3);
		ass_true(CxEq(CxVec3(-1.569637794f, 0.01983718076f, 0.091391915483f),
						  r3, 0.000001f));
		r3 = m1.transform3x3(v3);
		ass_true(CxEq(CxVec3(-1.50167652156f, -0.0653969097f, 0.47895467897f), r3, 0.000001f));
		r3 = m1.transform3x3Trans(v3);
		ass_true(CxEq(CxVec3(0.12554907f, -0.964839942f, 0.14367347791f), r3, 0.000001f));
		
		r3 = m0.transformInv(v3);
		ass_true(CxEq(CxVec3(-1.569637794f, 0.01983718076f, 0.091391915483f),
						  r3, 0.000001f));
		r3 = m1.transformInv(v3);
		ass_true(CxEq(CxVec3(204.2f, -20, -26.9f),
						  r3, 0.0001f));
		r3 = m1.transformTrans(v3);
		ass_true(CxEq(CxVec3(0.1404160856f, -0.95232905795f, 0.184684228534f),
						  r3, 0.000001f));
		r3 = m0.transform3x3Inv(v3);
		ass_true(CxEq(CxVec3(-0.89501433f, -0.826246418f, 3.938567335244f),
						  r3, 0.000001f));
		r3 = m1.transform3x3Inv(v3);
		ass_true(CxEq(CxVec3(-1152.9f,-731.65f,-3370), r3, 0.01f));

		
		FINISH_TEST;
	}

	/**
	 * @tests CxMat4(const CxQuat &)
	 * @tests setQuat(const CxQuat &)
	 * @tests quat() const
	 */
	void testCxMat4FromQuaternion() {
		BEGIN_TEST;
	   CxQuat q0(0.0691723f, 0.1383446f, 0.207516898f, 0.96592583f);
		CxMat4 m0(q0);
		ass_true(CxEq(m0, CxMat4(0.875595017799836f,
										 0.4200310908994311f,
										 -0.23855239986623264f,
										 0,
										 -0.3817526348378421f,
										 0.9043038598460277f,
										 0.1910483050485956f,
										 0,
										 0.29597008395861607f,
										 -0.07621293686382874f,
										 0.9521519299230139f,
										 0,0,0,0,1), 0.0000001f));
		ass_true(CxEq(m0.quat(), q0, 0.00001f));
		m0 = CxMat4(kCxIdentity);
		m0.setQuat(q0);
		ass_true(CxEq(m0, CxMat4(0.875595017799836f,
										 0.4200310908994311f,
										 -0.23855239986623264f,
										 0,
										 -0.3817526348378421f,
										 0.9043038598460277f,
										 0.1910483050485956f,
										 0,
										 0.29597008395861607f,
										 -0.07621293686382874f,
										 0.9521519299230139f,
										 0,0,0,0,1), 0.0000001f));
		ass_true(CxEq(m0.quat(), q0, 0.000001f));
		
		q0 = CxQuat(0.24691719f, 0.49383438247f, 0.74075157371f, 0.3826834324f);
		m0 = CxMat4(q0);
		ass_true(CxEq(m0, CxMat4(-0.5851705825303655f,
										 0.810819106826205f,
										 -0.012155877040681329f,
										 0,
										 -0.32307431220147687f,
										 -0.21936198656181954f,
										 0.9205994284417055f,
										 0,
										 0.7437730689777732f,
										 0.5426349554324782f,
										 0.39031900671909026f,
										 0,0,0,0,1), 0.000001f));
		ass_true(CxEq(m0.quat(), q0, 0.000001f));
		
		m0 = CxMat4(kCxIdentity);
		m0.setQuat(q0);
		ass_true(CxEq(m0, CxMat4(-0.5851705825303655f,
										 0.810819106826205f,
										 -0.012155877040681329f,
										 0,
										 -0.32307431220147687f,
										 -0.21936198656181954f,
										 0.9205994284417055f,
										 0,
										 0.7437730689777732f,
										 0.5426349554324782f,
										 0.39031900671909026f,
										 0,0,0,0,1), 0.000001f));
		ass_true(CxEq(m0.quat(), q0, 0.000001f));
		
	   q0 = CxQuat(0.9238795325f,0,0,0.3826834324f);
		m0 = CxMat4(q0);
		ass_true(CxEq(m0, CxMat4(1,0,0,0,
										 0, -0.7071067811865475f, 0.7071067811865476f, 0,
										 0, -0.7071067811865476f, -0.7071067811865475f, 0,
										 0,0,0,1), 0.000001f));
		ass_true(CxEq(m0.quat(), q0, 0.000001f));
		
		m0 = CxMat4(kCxIdentity);
		m0.setQuat(q0);
		ass_true(CxEq(m0, CxMat4(1,0,0,0,
										 0, -0.7071067811865475f, 0.7071067811865476f, 0,
										 0, -0.7071067811865476f, -0.7071067811865475f, 0,
										 0,0,0,1), 0.000001f));
		ass_true(CxEq(m0.quat(), q0, 0.000001f));

		FINISH_TEST;
	}

} // namespace cat


int main(int argc, char **argv) {
	cat::testCxMat4Basics();
	cat::testCxMat4Equality();
	cat::testCxMat4Arithmetic();
	cat::testCxMat4Mult();
	cat::testCxMat4DeterminantAndInverse();
	cat::testCxMat4Projection();
	cat::testCxMat4Transform();
	cat::testCxMat4FromQuaternion();
	
	return 0;
}
