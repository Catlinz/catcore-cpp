#include "core/math/CxQuat.h"
#include "core/CxTestCore.h"

namespace cat {

	/**
	 * @tests CxQuat()
	 * @tests CxQuat(CxZero)
	 * @tests CxQuat(CxIdentity)
	 * @tests CxQuat(const CxReal *)
	 * @tests CxQuat(CxReal, CxReal, CxReal, CxReal)
	 * @tests CxQuat(const CxQuat &)
	 * @tests operator=(const CxQuat &)
	 * @tests operator[](CxI32)
	 * @tests operator[](CxI32) const
	 * @tests operator==(const CxQuat &) const
	 * @tests operator!=(const CxQuat &) const
	 * @tests conjugate() const
	 * @tests setZero()
	 * @tests setXYZW(CxReal, CxReal, CxReal, CxReal)
	 * @tests setXYZ(CxReal, CxReal, CxReal)
	 * @tests xyz() const
	 * @tests isZero() const
	 * @tests isUnit() const
	 * @tests isCloseToUnit() const
	 * @tests magnitudeSqr() const
	 * @tests magnitude() const
	 * @tests CxEq(const CxQuat &, const CxQuat &, CxReal)
	 * @tests CxDot(const CxQuat &, const CxQuat &)
	 * @tests CxIsFinite(const CxQuat &)
	 */
	void testCxQuatBasics() {
		BEGIN_TEST;

		CxQuat v0;
		CxQuat v1(kCxZero);
		ass_true(v1.x == 0 && v1.y == 0 && v1.z == 0 && v1.w == 0);

		CxQuat v2(kCxIdentity);
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0 && v2.w == 1);

		CxReal va[] = {9, -4, 2, 8};
		CxQuat v3(va);
		ass_true(v3.x == 9 && v3.y == -4 && v3.z == 2 && v3.w == 8);

		CxQuat v4(-22, 93, 2, 9);
		ass_true(v4.x == -22 && v4.y == 93 && v4.z == 2 && v4.w == 9);


		CxQuat v5(v3);
		ass_true(v5.x == 9 && v5.y == -4 && v5.z == 2 && v5.w == 8);

		v5 = v1;
		ass_true(v5.x == 0 && v5.y == 0 && v5.z == 0 && v5.w == 0);

		v4 = CxQuat(kCxZero);
		ass_true(v4.x == 0 && v4.y == 0 && v4.z == 0 && v4.w == 0);

		v4 = CxQuat(-22, 93, 2, 9);
		ass_true(v4[0] == -22 && v4[1] == 93 && v4[2] == 2 && v4[3] == 9);
		const CxQuat v6 = v4;
		ass_true(v6[0] == -22 && v6[1] == 93 && v6[2] == 2 && v6[3] == 9);

		ass_true(v1 == v1 && !(v1 != v1));
		ass_true(v5 == v1 && v1 == v5);
		ass_false(v5 != v1 || v1 != v5);
		ass_false(v1 == v4 || v4 == v1);
		ass_true(v1 != v4 && v4 != v1);

		v0 = (CxQuat(1,2,3,4).conjugate());
		ass_true(v0.x == -1 && v0.y == -2 && v0.z == -3 && v0.w == 4);

		v0 = CxQuat(0.0001f, 1.0001f, -0.0001f, -1.0001f);
		ass_true(CxEq(CxQuat(0, 1, 0, -1), v0, 0.001f));
		ass_true(CxEq(v0, CxQuat(0, 1, 0, -1), 0.001f));
		ass_false(CxEq(CxQuat(0, 1, 0, -1), v0, 0.0001f));
		ass_false(CxEq(v0, CxQuat(0, 1, 0, -1), 0.0001f));
		ass_false(CxEq(CxQuat(0, 1, 0, -1), v0));
		ass_false(CxEq(v0, CxQuat(0, 1, 0, -1)));

		v0 = CxQuat(kCxZero);
		v1 = CxQuat(0.001f, 0, -0.001f, 0);
		v2 = CxQuat(0, 0.001f, 0, 0.001f);
		v3 = CxQuat(0.001f, -0.001f, 0.001f, -0.001f);
		ass_true(v0.isZero());
		ass_false(v1.isZero() || v2.isZero() || v3.isZero());
		v3.setZero();
		ass_true(v3.isZero());

		v0 = CxQuat(kCxZero);
		v1 = CxQuat(1, 0, 0, 0);
		v2 = CxQuat(0, 1, 0, 0);
		v3 = CxQuat(0, 0, 1, 0);
		CxQuat v3a = CxQuat(0, 0, 0, 1);
		v4 = CxQuat(1.001f, 0, 0, 0);
		v5 = CxQuat(0.001f, 1, 0.001f, 0.001f);
		CxQuat v7 = CxQuat(1, 0.001f, 0, -0.001f);
		
		ass_false(v0.isUnit());
		ass_true(v1.isUnit() && (-v1).isUnit());
		ass_true(v2.isUnit() && (-v2).isUnit());
		ass_true(v3.isUnit() && (-v3).isUnit());
		ass_true(v3a.isUnit() && (-v3a).isUnit());
		ass_false(v4.isUnit() || v5.isUnit() || v7.isUnit());

		ass_false(v0.isCloseToUnit());
		ass_true(v1.isCloseToUnit() && (-v1).isCloseToUnit());
		ass_true(v2.isCloseToUnit() && (-v2).isCloseToUnit());
		ass_true(v3.isCloseToUnit() && (-v3).isCloseToUnit());
		ass_true(v3a.isCloseToUnit() && (-v3a).isCloseToUnit());
		ass_true(v4.isCloseToUnit() || v5.isCloseToUnit() || v7.isCloseToUnit());
		v0 = CxQuat(1, 0.01f, 0, 0);
		ass_false(v0.isCloseToUnit());

		v0.setXYZW(-3, 2, 1, 4);
		ass_true(v0.x == -3 && v0.y == 2 && v0.z == 1 && v0.w == 4);
		ass_true(v0.xyz() == CxVec3(-3,2,1));
		
		v0.setXYZW(0,0,0,0);
		v1.setXYZW(1,1,1,1);
		v2.setXYZW(0,0,0,1);
		v3.setXYZW(0,-1,0,0);
		v4.setXYZW(-4,-2,1,3);
		v5.setXYZW(-1,-1,-1,-1);

		ass_true(v0.magnitude() == 0 && v0.magnitudeSqr() == 0);
		ass_true(v2.magnitude() == 1 && v2.magnitudeSqr() == 1);
		ass_true(v3.magnitude() == 1 && v3.magnitudeSqr() == 1);
		ass_true(CxEq(v1.magnitude(), CxSqrt(4.f)) && v1.magnitudeSqr() == 4);
		ass_true(CxEq(v4.magnitude(), CxSqrt(30.f)) && v4.magnitudeSqr() == 30);
	   ass_true(CxEq(v5.magnitude(), CxSqrt(4.f)) && v5.magnitudeSqr() == 4);

		v0.setXYZW(0,0,0,0);
		v1.setXYZW(1,0,0,1);
		v2.setXYZW(0,1,0,0);
		v3.setXYZW(-1,-1,-1,-1);
		v4.setXYZW(3,4,2,5);
		v5.setXYZW(-5,30,-1,2);

		ass_true(CxDot(v0, v0) == 0 && CxDot(v1,v1) == 2 && CxDot(v2,v2) == 1);
		ass_true(CxDot(v3, v3) == 4 && CxDot(v4,v4) == 54 && CxDot(v5,v5) == 930);

		ass_true(CxDot(v0, v1) == 0 && CxDot(v1, v0) == 0);
		ass_true(CxDot(v1, v2) == 0 && CxDot(v2, v1) == 0);
		ass_true(CxDot(v1, v3) == -2 && CxDot(v3, v1) == -2);
		ass_true(CxDot(v2, v3) == -1 && CxDot(v3, v2) == -1);
		ass_true(CxDot(v4, v5) == 113 && CxDot(v5, v4) == 113);

		v0.setXYZW(0,0,0,0);
		v1.setXYZW(1,1,1,1);
		v2.setXYZW(CX_NAN, 1,0,0);
		v3.setXYZW(1, CX_INF,0,1);
		v4.setXYZW(1, 1,CX_NAN,0);
		v5.setXYZW(1,0,1,CX_INF);
		v7.setXYZW(CX_INF, CX_NAN, CX_INF, CX_NAN);

		ass_true(CxIsFinite(v0) && CxIsFinite(v1));
		ass_false(CxIsFinite(v2) || CxIsFinite(v3) || CxIsFinite(v4) ||
					 CxIsFinite(v5) || CxIsFinite(v7));
		
		FINISH_TEST;
	}

	/**
	 * @tests operator-() const
	 * @tests operator+(const CxQuat &) const
	 * @tests operator-(const CxQuat &) const
	 * @tests operator*(CxReal) const
	 * @tests operator+=(const CxQuat &)
	 * @tests opeartor-=(const CxQuat &)
	 * @tests operator*=(CxReal)
	 */
	void testCxQuatArithmetic() {
		BEGIN_TEST;

		/* operator-() */
		CxQuat v0(1, -2, 2,-1);
		CxQuat v1 = -v0;
		ass_true(v0 != v1 && v1 != v0);
		ass_true(v0.x == 1 && v0.y == -2 && v0.z == 2 && v0.w == -1);
		ass_true(v1.x == -1 && v1.y == 2 && v1.z == -2 && v1.w == 1);

		/* operator+(const CxQuat &) */
		CxQuat v2 = v0 + v1;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0 && v2.w == 0);
		v2 = v1 + v0;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0 && v2.w == 0);

		v2 = v0 + v0;
		ass_true(v2.x == 2 && v2.y == -4 && v2.z == 4 && v2.w == -2);
		v2 = v1 + v1;
		ass_true(v2.x == -2 && v2.y == 4 && v2.z == -4 && v2.w == 2);

		/* operator-(const CxQuat &) */
		v2 = v0 - v0;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0 && v2.w == 0);
		v2 = v1 - v1;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0 && v2.w == 0);

		v2 = v0 - v1;
		ass_true(v2.x == 2 && v2.y == -4 && v2.z == 4 && v2.w == -2);
		v2 = v1 - v0;
		ass_true(v2.x == -2 && v2.y == 4 && v2.z == -4 && v2.w == 2);

		/* operator*(CxReal) */
		v2 = v0 * 2;
		ass_true(v2.x == 2 && v2.y == -4 && v2.z == 4 && v2.w == -2);
		v2 = CxQuat(8,0, 2,3) * -3;
		ass_true(v2.x = -24 && v2.y == 0 && v2.z == -6 && v2.w == -9);

		/* operator+=(const CxQuat &) */
		v2 = v0;
		v2 += CxQuat(3, 2, 4, 10);
		ass_true(v2.x == 4 && v2.y == 0 && v2.z == 6 && v2.w == 9);
		v2 += CxQuat(-3, 1, -5, -8);
		ass_true(v2.x == 1 && v2.y == 1 && v2.z == 1 && v2.w == 1);
		
		/* operator-=(const CxQuat &) */
		v2 = v0;
		v2 -= CxQuat(3, 2, 4, 5);
		ass_true(v2.x == -2 && v2.y == -4 && v2.z == -2 && v2.w == -6);
		v2 -= CxQuat(-1, -3, -1, -5);
		ass_true(v2.x == -1 && v2.y == -1 && v2.z == -1 && v2.w == -1);
		
		/* operator*=(CxReal) */
		v2 = v0;
		v2 *= 1;
		ass_true(v2.x == 1 && v2.y == -2 && v2.z == 2 && v2.w == -1);
		v2 *= 2;
		ass_true(v2.x == 2 && v2.y == -4 && v2.z == 4 && v2.w == -2);
		v2 *= -1;
		ass_true(v2.x == -2 && v2.y == 4 && v2.z == -4 && v2.w == 2);
		v2 *= 0;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0 && v2.w == 0);

		
		FINISH_TEST;
	}

	void testCxQuatNormalize() {
		BEGIN_TEST;

		CxQuat v0(kCxZero);
		CxQuat v1(0, 0, 1, 0);
		CxQuat v2(0, 0, 0, 1);
		CxQuat v3(0, 1, 0, 0);
		CxQuat v4(1, 1, 1, 1);
		CxQuat v5(5, 2, 3, 1);

		CxQuat v6 = v0.normalized();
		ass_true(v6.x == 0 && v6.y == 0 && v6.z == 0 && v6.w == 0);
		
		v6 = v1.normalized();
		ass_true(v6.x == 0 && v6.y == 0 && v6.z == 1 && v6.w == 0);

		v6 = v2.normalized();
		ass_true(v6.x == 0 && v6.y == 0 && v6.z == 0 && v6.w == 1);

		v6 = v3.normalized();
		ass_true(v6.x == 0 && v6.y == 1 && v6.z == 0 && v6.w == 0);
		
		v6 = v4.normalized();
		ass_true(CxEq(CxQuat(0.5f, 0.5f, 0.5f, 0.5f), v6));
		ass_true(v4.x == 1 && v4.y == 1 && v4.z == 1 && v4.w == 1);

		v6 = v5.normalized();
		ass_true(CxEq(CxQuat(5.f/CxSqrt(39.f), 2.f/CxSqrt(39.f),
									3.f/CxSqrt(39.f), 1.f/CxSqrt(39.f)), v6, 0.000001f));

		v0.normalize();
		ass_true(v0.x == 0 && v0.y == 0 && v0.z == 0 && v0.w == 0);
		v1.normalize();
		ass_true(v1.x == 0 && v1.y == 0 && v1.z == 1 && v1.w == 0);
		v2.normalize();
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0 && v2.w == 1);
		v3.normalize();
		ass_true(v3.x == 0 && v3.y == 1 && v3.z == 0 && v3.w == 0);

		v6 = v4;
		ass_true(v6.x == 1 && v6.y == 1 && v6.z == 1 && v6.w == 1);
		v4.normalize();
		ass_true(CxEq(CxQuat(0.5f, 0.5f, 0.5f, 0.5f), v4));

		v6 = v5;
		ass_true(v6.x == 5 && v6.y == 2 && v6.z == 3 && v6.w == 1);
		v5.normalize();
		ass_true(CxEq(CxQuat(5.f/CxSqrt(39.f), 2.f/CxSqrt(39.f),
									3.f/CxSqrt(39.f), 1.f/CxSqrt(39.f)), v5, 0.000001f));
		
		FINISH_TEST;
	}

	/**
	 * @tests CxQuat(const CxVec3 &, CxReal)
	 * @tests axis() const
	 * @tests angle() const
	 * @tests axisAngle(CxVec3 &, CxReal &)
	 * @tests xAxis() const
	 * @tests yAxis() const
	 * @tests zAxis() const
	 */
	void testCxQuatFromAxisAngle() {
		BEGIN_TEST;

		CxVec3 axis;
		CxReal angle;
		
		/* Test +x @ 0 degrees*/
		CxQuat q0(CxVec3::kRight, kCxDegToRad*0.f);
		ass_true(q0.isUnit());
		ass_true(CxEq(CxQuat(0,0,0,1), q0, 0.000001f));
		ass_true(CxEq(q0.angle(), 0) &&
					CxEq(CxVec3::kRight, q0.axis(), 0.000001f));
		q0.axisAngle(axis, angle);
		ass_true(CxEq(angle, 0) &&
					CxEq(CxVec3::kRight, axis, 0.000001f));
		ass_true(q0.xAxis() == CxVec3::kRight);
		ass_true(q0.yAxis() == CxVec3::kUp);
		ass_true(q0.zAxis() == CxVec3::kBackward);

		/* Test +x @ 30 degrees*/
	   CxQuat q1(CxVec3::kRight, kCxDegToRad*30.f);
		ass_true(q1.isUnit());
		ass_true(CxEq(CxQuat(0.25881905f, 0, 0, 0.96592582f), q1, 0.000001f));
		ass_true(CxEq(q1.angle(), kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kRight, q1.axis(), 0.000001f));
		q1.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kRight, axis, 0.000001f));
		ass_true(q1.xAxis() == CxVec3::kRight);
		ass_true(CxEq(q1.yAxis(), CxVec3(0, 0.86602540f, 0.5f), 0.000001f));
		ass_true(CxEq(q1.zAxis(), CxVec3(0, -0.5f, 0.86602540f), 0.000001f));

		/* Test +x @ 90 degrees*/
		CxQuat q2(CxVec3::kRight, kCxDegToRad*90.f);
		ass_true(q2.isUnit());
		ass_true(CxEq(CxQuat(0.707106781f, 0, 0, 0.707106781f), q2, 0.000001f));
		ass_true(CxEq(q2.angle(), kCxDegToRad*90.f, 0.00001f) &&
					CxEq(CxVec3::kRight, q2.axis(), 0.000001f));
		q2.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*90.f, 0.00001f) &&
					CxEq(CxVec3::kRight, axis, 0.000001f));
		ass_true(q2.xAxis() == CxVec3::kRight);
		ass_true(CxEq(q2.yAxis(), CxVec3(0,0,1), 0.000001f));
		ass_true(CxEq(q2.zAxis(), CxVec3(0,-1,0), 0.000001f));

		/* Test +x @ 135 degrees*/
		CxQuat q3(CxVec3::kRight, kCxDegToRad*135.f);
		ass_true(q3.isUnit());
		ass_true(CxEq(CxQuat(0.923879533f, 0, 0, 0.382683432f), q3, 0.000001f));
		ass_true(CxEq(q3.angle(), kCxDegToRad*135.f, 0.00001f) &&
					CxEq(CxVec3::kRight, q3.axis(), 0.000001f));
		q3.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*135.f, 0.00001f) &&
					CxEq(CxVec3::kRight, axis, 0.000001f));
		ass_true(q3.xAxis() == CxVec3::kRight);
		ass_true(CxEq(q3.yAxis(), CxVec3(0, -0.707106781f, 0.707106781f), 0.000001f));
		ass_true(CxEq(q3.zAxis(), CxVec3(0, -0.707106781f, -0.707106781f), 0.000001f));
		
		/* Test +x @ 180 degrees*/
		CxQuat q4(CxVec3::kRight, kCxDegToRad*180.f);
		ass_true(q4.isUnit());
		ass_true(CxEq(CxQuat(1, 0, 0, 0), q4, 0.000001f));
		ass_true(CxEq(q4.angle(), kCxDegToRad*180.f, 0.00001f) &&
					CxEq(CxVec3::kRight, q4.axis(), 0.000001f));
		q4.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*180.f, 0.00001f) &&
					CxEq(CxVec3::kRight, axis, 0.000001f));
		ass_true(q4.xAxis() == CxVec3::kRight);
		ass_true(CxEq(q4.yAxis(), CxVec3(0,-1,0), 0.000001f));
		ass_true(CxEq(q4.zAxis(), CxVec3(0,0,-1), 0.000001f));

		/* Test +x @ 270 degrees*/
		CxQuat q5(CxVec3::kRight, kCxDegToRad*270.f);
		ass_true(q5.isUnit());
		ass_true(CxEq(CxQuat(0.707106781f, 0, 0, -0.707106781f), q5, 0.000001f));
		ass_true(CxEq(q5.angle(), kCxDegToRad*270.f, 0.00001f) &&
					CxEq(CxVec3::kRight, q5.axis(), 0.000001f));
		q5.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*270.f, 0.00001f) &&
					CxEq(CxVec3::kRight, axis, 0.000001f));
		ass_true(q5.xAxis() == CxVec3::kRight);
		ass_true(CxEq(q5.yAxis(), CxVec3(0,0,-1), 0.000001f));
		ass_true(CxEq(q5.zAxis(), CxVec3(0,1,0), 0.000001f));
		
		/* Test +x @ 360 degrees*/
		CxQuat q6(CxVec3::kRight, kCxDegToRad*360.f);
		ass_true(q6.isUnit());
		ass_true(CxEq(CxQuat(0, 0, 0, -1), q6, 0.000001f));
		ass_true(CxEq(q6.xAxis(), CxVec3::kRight, 0.00001f));
		ass_true(CxEq(q6.yAxis(), CxVec3::kUp, 0.00001f));
		ass_true(CxEq(q6.zAxis(), CxVec3::kBackward, 0.00001f));


		/* Test -x @ 30 degrees*/
	   q1 = CxQuat(CxVec3::kLeft, kCxDegToRad*30.f);
		ass_true(q1.isUnit());
		ass_true(CxEq(CxQuat(-0.25881905f, 0, 0, 0.96592582f), q1, 0.000001f));
		ass_true(CxEq(q1.angle(), kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kLeft, q1.axis(), 0.000001f));
		q1.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kLeft, axis, 0.000001f));
		ass_true(q1.xAxis() == CxVec3::kRight);
		ass_true(CxEq(q1.yAxis(), CxVec3(0, 0.86602540f, -0.5f), 0.000001f));
		ass_true(CxEq(q1.zAxis(), CxVec3(0, 0.5f, 0.86602540f), 0.000001f));
		
		/* Test +y @ 30 degrees*/
	   q1 = CxQuat(CxVec3::kUp, kCxDegToRad*30.f);
		ass_true(q1.isUnit());
		ass_true(CxEq(CxQuat(0, 0.25881905f, 0, 0.96592582f), q1, 0.000001f));
		ass_true(CxEq(q1.angle(), kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kUp, q1.axis(), 0.000001f));
		q1.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kUp, axis, 0.000001f));
		ass_true(CxEq(q1.xAxis(), CxVec3(0.86602540f, 0, -0.5f), 0.000001f));
		ass_true(CxEq(q1.yAxis(), CxVec3(0,1,0), 0.000001f));
		ass_true(CxEq(q1.zAxis(), CxVec3(0.5f, 0, 0.86602540f), 0.000001f));

		/* Test -y @ 30 degrees*/
	   q1 = CxQuat(CxVec3::kDown, kCxDegToRad*30.f);
		ass_true(q1.isUnit());
		ass_true(CxEq(CxQuat(0, -0.25881905f, 0, 0.96592582f), q1, 0.000001f));
		ass_true(CxEq(q1.angle(), kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kDown, q1.axis(), 0.000001f));
		q1.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kDown, axis, 0.000001f));
		ass_true(CxEq(q1.xAxis(), CxVec3(0.86602540f, 0, 0.5f), 0.000001f));
		ass_true(CxEq(q1.yAxis(), CxVec3(0,1,0), 0.000001f));
		ass_true(CxEq(q1.zAxis(), CxVec3(-0.5f, 0, 0.86602540f), 0.000001f));

		/* Test -z @ 30 degrees*/
	   q1 = CxQuat(CxVec3::kForward, kCxDegToRad*30.f);
		ass_true(q1.isUnit());
		ass_true(CxEq(CxQuat(0, 0, -0.25881905f, 0.96592582f), q1, 0.000001f));
		ass_true(CxEq(q1.angle(), kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kForward, q1.axis(), 0.000001f));
		q1.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kForward, axis, 0.000001f));
		ass_true(CxEq(q1.xAxis(), CxVec3(0.86602540f, -0.5f, 0), 0.000001f));
		ass_true(CxEq(q1.yAxis(), CxVec3(0.5f, 0.86602540f, 0), 0.000001f));
		ass_true(CxEq(q1.zAxis(), CxVec3(0,0,1), 0.000001f));
		
		/* Test +z @ 30 degrees*/
	   q1 = CxQuat(CxVec3::kBackward, kCxDegToRad*30.f);
		ass_true(q1.isUnit());
		ass_true(CxEq(CxQuat(0, 0, 0.25881905f, 0.96592582f), q1, 0.000001f));
		ass_true(CxEq(q1.angle(), kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kBackward, q1.axis(), 0.000001f));
		q1.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3::kBackward, axis, 0.000001f));
		ass_true(CxEq(q1.xAxis(), CxVec3(0.86602540f, 0.5f, 0), 0.000001f));
		ass_true(CxEq(q1.yAxis(), CxVec3(-0.5f, 0.86602540f, 0), 0.000001f));
		ass_true(CxEq(q1.zAxis(), CxVec3(0,0,1), 0.000001f));

		
		q1 = CxQuat(CxVec3(1,2,3).normalized(), kCxDegToRad*30.f);
		ass_true(q1.isUnit());
		ass_true(CxEq(CxQuat(0.0691723f, 0.1383446f, 0.207516898f, 0.96592583f), q1, 0.000001f));
	   ass_true(CxEq(q1.angle(), kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3(1,2,3).normalized(), q1.axis(), 0.000001f));
		q1.axisAngle(axis, angle);
		ass_true(CxEq(angle, kCxDegToRad*30.f, 0.00001f) &&
					CxEq(CxVec3(1,2,3).normalized(), axis, 0.000001f));
		ass_true(CxEq(q1.xAxis(), CxVec3(0.8755950178f, 0.420031091f, -0.2385524f), 0.000001f));
		ass_true(CxEq(q1.yAxis(), CxVec3(-0.38175263f, 0.90430386f, 0.191048305f), 0.000001f));
	   ass_true(CxEq(q1.zAxis(), CxVec3(0.295970084f, -0.076212937f, 0.95215193f), 0.000001f));
		
		FINISH_TEST;
	}

	void testCxQuatFromMat3() {
		BEGIN_TEST;

		assert(false);

		FINISH_TEST;
	}

	void testCxQuatFromStartEnd() {
		BEGIN_TEST;

		assert(false);

		FINISH_TEST;
	}

	void testCxQuatMult() {
		BEGIN_TEST;

		assert(false);

		FINISH_TEST;
	}

	void testCxQuatRotate() {
		BEGIN_TEST;

		assert(false);

		FINISH_TEST;
	}

	void testCxQuatTranslation() {
		BEGIN_TEST;

		assert(false);

		FINISH_TEST;
	}
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxQuatBasics();
	cat::testCxQuatArithmetic();
	cat::testCxQuatNormalize();
	cat::testCxQuatFromAxisAngle();
	cat::testCxQuatFromMat3();
	cat::testCxQuatFromStartEnd();
	cat::testCxQuatMult();
	cat::testCxQuatRotate();
	cat::testCxQuatTranslation();
	return 0;
}
