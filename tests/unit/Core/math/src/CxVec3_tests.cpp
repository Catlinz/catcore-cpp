#include "core/math/CxVec3.h"
#include "core/CxTestCore.h"

namespace cat {

	/**
	 * @tests CxVec3()
	 * @tests CxVec3(CxZero)
	 * @tests CxVec3(CxReal)
	 * @tests CxVec3(const CxReal *)
	 * @tests CxVec3(CxReal, CxReal)
	 * @tests CxVec3(const CxVec3 &)
	 * @tests operator=(const CxVec3 &)
	 * @tests operator[](CxI32)
	 * @tests operator[](CxI32) const
	 * @tests operator==(const CxVec3 &) const
	 * @tests operator!=(const CxVec3 &) const
	 * @tests setZero()
	 * @tests setXYZ(CxReal, CxReal, CxReal)
	 * @tests setXY(CxReal, CxReal)
	 * @tests setXZ(CxReal, CxReal)
	 * @tests setYZ(CxReal, CxReal)
	 * @tests isZero() const
	 * @tests isUnit() const
	 * @tests magnitudeSqr() const
	 * @tests magnitude() const
	 * @tests max() const
	 * @tests min() const
	 * @tests CxEq(const CxVec3 &, const CxVec3 &, CxReal)
	 * @tests CxDot(const CxVec3 &, const CxVec3 &)
	 * @tests CxCross(const CxVec3 &, const CxVec3 &)
	 * @tests CxIsFinite(const CxVec3 &)
	 */
	void testCxVec3Basics() {
		BEGIN_TEST;

		CxVec3 v0;
		CxVec3 v1(kCxZero);
		ass_true(v1.x == 0 && v1.y == 0 && v1.z == 0);

		CxVec3 v2(3);
		ass_true(v2.x == 3 && v2.y == 3 && v2.z == 3);

		CxReal va[] = {9, -4, 2};
		CxVec3 v3(va);
		ass_true(v3.x == 9 && v3.y == -4 && v3.z == 2);

		CxVec3 v4(-22, 93, 2);
		ass_true(v4.x == -22 && v4.y == 93 && v4.z == 2);


		CxVec3 v5(v3);
		ass_true(v5.x == 9 && v5.y == -4 && v5.z == 2);

		v5 = v1;
		ass_true(v5.x == 0 && v5.y == 0 && v5.z == 0);

		v4 = CxVec3(kCxZero);
		ass_true(v4.x == 0 && v4.y == 0 && v4.z == 0);

		v4 = CxVec3(-22, 93, 2);
		ass_true(v4[0] == -22 && v4[1] == 93 && v4[2] == 2);
		const CxVec3 v6 = v4;
		ass_true(v6[0] == -22 && v6[1] == 93 && v6[2] == 2);

		ass_true(v1 == v1 && !(v1 != v1));
		ass_true(v5 == v1 && v1 == v5);
		ass_false(v5 != v1 || v1 != v5);
		ass_false(v1 == v4 || v4 == v1);
		ass_true(v1 != v4 && v4 != v1);

		v0 = CxVec3(0.0001f, 1.0001f, -0.0001f);
		ass_true(CxEq(CxVec3(0, 1, 0), v0, 0.001f));
		ass_true(CxEq(v0, CxVec3(0, 1, 0), 0.001f));
		ass_false(CxEq(CxVec3(0, 1, 0), v0, 0.0001f));
		ass_false(CxEq(v0, CxVec3(0, 1, 0), 0.0001f));
		ass_false(CxEq(CxVec3(0, 1, 0), v0));
		ass_false(CxEq(v0, CxVec3(0, 1, 0)));

		v0 = CxVec3(kCxZero);
		v1 = CxVec3(0.001f, 0, -0.001f);
		v2 = CxVec3(0, 0.001f, 0);
		v3 = CxVec3(0.001f, -0.001f, 0.001f);
		ass_true(v0.isZero());
		ass_false(v1.isZero() || v2.isZero() || v3.isZero());
		v3.setZero();
		ass_true(v3.isZero());

		v0 = CxVec3(kCxZero);
		v1 = CxVec3(1, 0, 0);
		v2 = CxVec3(0, 1, 0);
		v3 = CxVec3(0, 0, 1);
		v4 = CxVec3(1.001f, 0, 0);
		v5 = CxVec3(0.001f, 1, 0.001f);
		CxVec3 v7 = CxVec3(1, 0.001f, 0);
		
		ass_false(v0.isUnit());
		ass_true(v1.isUnit() && (-v1).isUnit());
		ass_true(v2.isUnit() && (-v2).isUnit());
		ass_true(v3.isUnit() && (-v3).isUnit());
		ass_false(v4.isUnit() || v5.isUnit() || v7.isUnit());

		v0.setXYZ(-3, 2, 1);
		ass_true(v0.x == -3 && v0.y == 2 && v0.z == 1);
		v0.setXY(7,2);
		ass_true(v0.x == 7 && v0.y == 2 && v0.z == 1);
		v0.setXZ(9,4);
		ass_true(v0.x == 9 && v0.y == 2 && v0.z == 4);
		v0.setYZ(11,34);
		ass_true(v0.x == 9 && v0.y == 11 && v0.z == 34);

		v0.setXYZ(0,0, 0);
		v1.setXYZ(1,1,1);
		v2.setXYZ(1,0,0);
		v3.setXYZ(0,-1,0);
		v4.setXYZ(-4,-2,1);
		v5.setXYZ(-1,-1,-1);

		ass_true(v0.magnitude() == 0 && v0.magnitudeSqr() == 0);
		ass_true(v2.magnitude() == 1 && v2.magnitudeSqr() == 1);
		ass_true(v3.magnitude() == 1 && v3.magnitudeSqr() == 1);
		ass_true(CxEq(v1.magnitude(), CxSqrt(3.f)) && v1.magnitudeSqr() == 3);
		ass_true(CxEq(v4.magnitude(), CxSqrt(21.f)) && v4.magnitudeSqr() == 21);
	   ass_true(CxEq(v5.magnitude(), CxSqrt(3.f)) && v5.magnitudeSqr() == 3);

		v0.setXYZ(0,0,0);
		v1.setXYZ(1,0,0);
		v2.setXYZ(0,1,0);
		v3.setXYZ(-1,0,1);
		v4.setXYZ(0,-1,0);
		v5.setXYZ(0,1,-1);
		v7.setXYZ(2, 2.5,-2);

		ass_true(v0.max() == 0 && v0.min() == 0);
		ass_true(v1.max() == 1 && v1.min() == 0);
		ass_true(v2.max() == 1 && v2.min() == 0);
		ass_true(v3.max() == 1 && v3.min() == -1);
		ass_true(v4.max() == 0 && v4.min() == -1);
		ass_true(v5.max() == 1 && v5.min() == -1);
		ass_true(v7.max() == 2.5 && v7.min() == -2);

		v0.setXYZ(0,0,0);
		v1.setXYZ(1,0,1);
		v2.setXYZ(0,1,0);
		v3.setXYZ(-1,-1,-1);
		v4.setXYZ(3,4,2);
		v5.setXYZ(-5, 30,-1);

		ass_true(CxDot(v0, v0) == 0 && CxDot(v1,v1) == 2 && CxDot(v2,v2) == 1);
		ass_true(CxDot(v3, v3) == 3 && CxDot(v4,v4) == 29 && CxDot(v5,v5) == 926);

		ass_true(CxDot(v0, v1) == 0 && CxDot(v1, v0) == 0);
		ass_true(CxDot(v1, v2) == 0 && CxDot(v2, v1) == 0);
		ass_true(CxDot(v1, v3) == -2 && CxDot(v3, v1) == -2);
		ass_true(CxDot(v2, v3) == -1 && CxDot(v3, v2) == -1);
		ass_true(CxDot(v4, v5) == 103 && CxDot(v5, v4) == 103);

		v0.setXYZ(0,0,1);
		v1.setXYZ(0,1,0);
		v2.setXYZ(1,0,0);
		v3.setXYZ(1,2,3);
		v4.setXYZ(7,6,5);
		

		ass_true(CxEq(-v2, CxCross(v0,v1)));
		ass_true(CxEq(v2, CxCross(v1,v0)));
		ass_true(CxEq(v1, CxCross(v0,v2)));
		ass_true(CxEq(-v1, CxCross(v2,v0)));
		ass_true(CxEq(-v0, CxCross(v1,v2)));
		ass_true(CxEq(v0, CxCross(v2,v1)));
		ass_true(CxEq(CxVec3(-8,16,-8), CxCross(v3,v4)));
		ass_true(CxEq(CxVec3(8,-16,8), CxCross(v4,v3)));

		v0.setXYZ(0,0,0);
		v1.setXYZ(1,1,1);
		v2.setXYZ(CX_NAN, 1,0);
		v3.setXYZ(1, CX_INF,0);
		v4.setXYZ(1, 1,CX_NAN);
		v5.setXYZ(CX_INF, CX_NAN, CX_INF);

		ass_true(CxIsFinite(v0) && CxIsFinite(v1));
		ass_false(CxIsFinite(v2) || CxIsFinite(v3) || CxIsFinite(v4) || CxIsFinite(v5));

		ass_true(CxVec3::kLeft == CxVec3(-1,0,0));
		ass_true(CxVec3::kRight == CxVec3(1,0,0));
		ass_true(CxVec3::kUp == CxVec3(0,1,0));
		ass_true(CxVec3::kDown == CxVec3(0,-1,0));
		ass_true(CxVec3::kForward == CxVec3(0,0,-1));
		ass_true(CxVec3::kBackward == CxVec3(0,0,1));
		
		FINISH_TEST;
	}

	/**
	 * @tests operator-() const
	 * @tests operator+(const CxVec3 &) const
	 * @tests operator-(const CxVec3 &) const
	 * @tests operator*(const CxVec3 &) const
	 * @tests operator*(CxReal) const
	 * @tests operator/(const CxVec3 &) const
	 * @tests operator/(CxReal) const
	 * @tests operator+=(const CxVec3 &)
	 * @tests operator+=(CxReal)
	 * @tests opeartor-=(const CxVec3 &)
	 * @tests operator-=(CxReal)
	 * @tests opeartor*=(const CxVec3 &)
	 * @tests operator*=(CxReal)
	 * @tests opeartor/=(const CxVec3 &)
	 * @tests operator/=(CxReal)
	 */
	void testCxVec3Arithmetic() {
		BEGIN_TEST;

		/* operator-() */
		CxVec3 v0(1, -2, 2);
		CxVec3 v1 = -v0;
		ass_true(v0 != v1 && v1 != v0);
		ass_true(v0.x == 1 && v0.y == -2 && v0.z == 2);
		ass_true(v1.x == -1 && v1.y == 2 && v1.z == -2);

		/* operator+(const CxVec3 &) */
		CxVec3 v2 = v0 + v1;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0);
		v2 = v1 + v0;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0);

		v2 = v0 + v0;
		ass_true(v2.x == 2 && v2.y == -4 && v2.z == 4);
		v2 = v1 + v1;
		ass_true(v2.x == -2 && v2.y == 4 && v2.z == -4);

		/* operator-(const CxVec3 &) */
		v2 = v0 - v0;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0);
		v2 = v1 - v1;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0);

		v2 = v0 - v1;
		ass_true(v2.x == 2 && v2.y == -4 && v2.z == 4);
		v2 = v1 - v0;
		ass_true(v2.x == -2 && v2.y == 4 && v2.z == -4);

		/* operator*(const CxVec3 &) */
		v2 = v0 * v0;
		ass_true(v2.x == 1 && v2.y == 4 && v2.z == 4);
		v2 = CxVec3(2,-3,1) * CxVec3(4,5,3);
		ass_true(v2.x == 8 && v2.y == -15 && v2.z == 3);

		/* operator*(CxReal) */
		v2 = v0 * 2;
		ass_true(v2.x == 2 && v2.y == -4 && v2.z == 4);
		v2 = CxVec3(8,0, 2) * -3;
		ass_true(v2.x = -24 && v2.y == 0 && v2.z == -6);

		/* operator/(const CxVec3 &) */
		v2 = v0 / v0;
		ass_true(v2.x == 1 && v2.y == 1 && v2.z == 1);
		v2 = CxVec3(8,-15,6) / CxVec3(4,5,2);
		ass_true(v2.x == 2 && v2.y == -3 && v2.z == 3);

		/* operator/(CxReal) */
		v2 = v0 / 1;
		ass_true(v2.x == 1 && v2.y == -2 && v2.z == 2);
		v2 = v0 / 2;
		ass_true(CxEq(CxVec3(0.5f, -1, 1), v2));

		/* operator+=(const CxVec3 &) */
		v2 = v0;
		v2 += CxVec3(3, 2, 4);
		ass_true(v2.x == 4 && v2.y == 0 && v2.z == 6);
		v2 += CxVec3(-3, 1, -5);
		ass_true(v2.x == 1 && v2.y == 1 && v2.z == 1);

		/* operator+=(CxReal) */
		v2 = v0;
		v2 += 10;
		ass_true(v2.x == 11 && v2.y == 8 && v2.z == 12);
		v2 += -10;
		ass_true(v2.x == 1 && v2.y == -2 && v2.z == 2);
		
		/* operator-=(const CxVec3 &) */
		v2 = v0;
		v2 -= CxVec3(3, 2, 4);
		ass_true(v2.x == -2 && v2.y == -4 && v2.z == -2);
		v2 -= CxVec3(-1, -3, -1);
		ass_true(v2.x == -1 && v2.y == -1 && v2.z == -1);

		/* operator-=(CxReal) */
		v2 = v0;
		v2 -= 10;
		ass_true(v2.x == -9 && v2.y == -12 && v2.z == -8);
		v2 -= -10;
		ass_true(v2.x == 1 && v2.y == -2 && v2.z == 2);

		/* operator*=(const CxVec3 &) */
		v2 = v0;
		v2 *= CxVec3(1,1,1);
		ass_true(v2.x == 1 && v2.y == -2 && v2.z == 2);
		v2 = v0;
		v2 *= CxVec3(-2, 10, 3);
		ass_true(CxEq(CxVec3(-2, -20, 6), v2));
		v2 *= CxVec3(0, -1, -2);
		ass_true(v2.x == 0 && v2.y == 20 && v2.z == -12);
		
		/* operator*=(CxReal) */
		v2 = v0;
		v2 *= 1;
		ass_true(v2.x == 1 && v2.y == -2 && v2.z == 2);
		v2 *= 2;
		ass_true(v2.x == 2 && v2.y == -4 && v2.z == 4);
		v2 *= -1;
		ass_true(v2.x == -2 && v2.y == 4 && v2.z == -4);
		v2 *= 0;
		ass_true(v2.x == 0 && v2.y == 0 && v2.z == 0);

		/* operator/=(const CxVec3 &) */
		v2 = v0;
		v2 /= CxVec3(1,1,1);
		ass_true(v2.x == 1 && v2.y == -2 && v2.z == 2);
		v2 = v0;
		v2 /= CxVec3(-2, 10, 8);
		ass_true(CxEq(CxVec3(-0.5f, -0.2f, 0.25f), v2));
		
		/* operator/=(CxReal) */
		v2 = v0;
		v2 /= 1;
		ass_true(v2.x == 1 && v2.y == -2 && v2.z == 2);
		v2 /= 2;
		ass_true(CxEq(CxVec3(0.5f, -1, 1), v2));
		v2 /= -1;
		ass_true(CxEq(CxVec3(-0.5f, 1, -1), v2));
		
		
		FINISH_TEST;
	}

	void testCxVec3Normalize() {
		BEGIN_TEST;

		CxVec3 v0(kCxZero);
		CxVec3 v1(0, 1, 0);
		CxVec3 v2(1, 0, 0);
		CxVec3 v3(0, 0, 1);
		CxVec3 v4(1, 1, 1);
		CxVec3 v5(5, 2, 3);

		CxVec3 v6 = v0.normalized();
		ass_true(v6.x == 0 && v6.y == 0 && v6.z == 0);
		
		v6 = v1.normalized();
		ass_true(v6.x == 0 && v6.y == 1 && v6.z == 0);
		v6 = v1.normalizedNonZero();
		ass_true(v6.x == 0 && v6.y == 1 && v6.z == 0);

		v6 = v2.normalized();
		ass_true(v6.x == 1 && v6.y == 0 && v6.z == 0);
		v6 = v2.normalizedNonZero();
		ass_true(v6.x == 1 && v6.y == 0 && v6.z == 0);

		v6 = v3.normalized();
		ass_true(v6.x == 0 && v6.y == 0 && v6.z == 1);
		v6 = v3.normalizedNonZero();
		ass_true(v6.x == 0 && v6.y == 0 && v6.z == 1);
		
		v6 = v4.normalized();
		ass_true(CxEq(CxVec3(1.f/CxSqrt(3.f), 1.f/CxSqrt(3.f), 1.f/CxSqrt(3.f)), v6));
		v6 = v4.normalizedNonZero();
		ass_true(CxEq(CxVec3(1.f/CxSqrt(3.f), 1.f/CxSqrt(3.f), 1.f/CxSqrt(3.f)), v6));
		ass_true(v4.x == 1 && v4.y == 1 && v4.z == 1);

		v6 = v5.normalized();
		ass_true(CxEq(CxVec3(5.f/CxSqrt(38.f), 2.f/CxSqrt(38.f), 3.f/CxSqrt(38.f)), v6, 0.000001f));
		v6 = v5.normalizedNonZero();
		ass_true(CxEq(CxVec3(5.f/CxSqrt(38.f), 2.f/CxSqrt(38.f), 3.f/CxSqrt(38.f)), v6, 0.000001f));

		v0.normalize();
		ass_true(v0.x == 0 && v0.y == 0 && v0.z == 0);
		v1.normalize();
		ass_true(v1.x == 0 && v1.y == 1 && v1.z == 0);
		v2.normalize();
		ass_true(v2.x == 1 && v2.y == 0 && v2.z == 0);
		v3.normalize();
		ass_true(v3.x == 0 && v3.y == 0 && v3.z == 1);

		v6 = v4;
		ass_true(v6.x == 1 && v6.y == 1 && v6.z == 1);
		v4.normalize();
		ass_true(CxEq(CxVec3(1.f/CxSqrt(3.f), 1.f/CxSqrt(3.f), 1.f/CxSqrt(3.f)), v4));
		v6.normalizeNonZero();
		ass_true(CxEq(CxVec3(1.f/CxSqrt(3.f), 1.f/CxSqrt(3.f), 1.f/CxSqrt(3.f)), v6));

		v6 = v5;
		ass_true(v6.x == 5 && v6.y == 2 && v6.z == 3);
		v5.normalize();
		ass_true(CxEq(CxVec3(5.f/CxSqrt(38.f), 2.f/CxSqrt(38.f), 3.f/CxSqrt(38.f)), v5, 0.000001f));
		v6.normalizeNonZero();
		ass_true(CxEq(CxVec3(5.f/CxSqrt(38.f), 2.f/CxSqrt(38.f), 3.f/CxSqrt(38.f)), v6, 0.000001f));
		
		FINISH_TEST;
	}

	void testCxVec3MathOps() {
		BEGIN_TEST;

		/* CxAbs */
		CxVec3 v0(0,0,0);
		CxVec3 v1(1,-1,1);
		CxVec3 v2(-1,1,-1);
		CxVec3 v3(-1,-1,-1);
		CxVec3 v4(23,123,3);

		CxVec3 r;
		r = CxAbs(v0);  ass_true(r.x == 0 && r.y == 0 && r.z == 0);
		r = CxAbs(v1);  ass_true(r.x == 1 && r.y == 1 && r.z == 1);
		r = CxAbs(v2);  ass_true(r.x == 1 && r.y == 1 && r.z == 1);
		r = CxAbs(v3);  ass_true(r.x == 1 && r.y == 1 && r.z == 1);
		r = CxAbs(v4);  ass_true(r.x == 23 && r.y == 123 && r.z == 3);

		/* Trig functions */
		v0.setXYZ(0.5f, 1, -0.3f);

		r = CxAcos(v0);
		ass_true(CxEq(CxVec3(1.04719755f, 0, 1.87548898f), r, 0.0001f));
		r = CxAsin(v0);
		ass_true(CxEq(CxVec3(0.523598776f, 1.57079633f, -0.304692654f), r, 0.0001f));
		r = CxAtan(v0);
		ass_true(CxEq(CxVec3(0.46364761f, 0.7853981634f, -0.291456794f), r, 0.0001f));

		v0.setXYZ(1.04719755f, 1.57079633f, -0.304692654f);
		r = CxCos(v0);
		ass_true(CxEq(CxVec3(0.5f, 0.0f, 0.953939201422f), r, 0.0001f));
		r =  CxSin(v0);
		ass_true(CxEq(CxVec3(0.866025403f, 1, -0.3f), r, 0.0001f));
		v0.y = 0.5f;
		r = CxTan(v0);
		ass_true(CxEq(CxVec3(1.7320508028f, 0.5463024898f, -0.314485451f), r, 0.0001f));

		/* Ceil and floor*/
	   r = CxCeil(CxVec3(0.4f, -0.4f, -1));
		ass_true(CxEq(CxVec3(1, 0, -1), r));

		r = CxFloor(CxVec3(-0.4f, 0.6f, -1));
		ass_true(CxEq(CxVec3(-1, 0, -1), r));

		/* Clamp */
		r = CxClamp(CxVec3(0.9f, -0.9f, 0.9f), -1.0f, 1.0f);
		ass_true(r.x == 0.9f && r.y == -0.9f && r.z == 0.9f);
		r = CxClamp(CxVec3(1.1f, -1.1f, 1.2f), -1.0f, 1.0f);
		ass_true(r.x == 1.0f && r.y == -1.0f && r.z == 1.0f);

		v0 = CxVec3(-1, -2, -3);
		v1 = CxVec3(1, 2, 3);
		r = CxClamp(CxVec3(0.9f, -1.9f, -2.9f), v0, v1);
		ass_true(r.x == 0.9f && r.y == -1.9f && r.z == -2.9f);
		r = CxClamp(CxVec3(1.1f, -2.1f, -3.1f), v0, v1);
		ass_true(r.x == 1.0f && r.y == -2.0f && r.z == -3.0f);
		r = CxClamp(CxVec3(-1.1f, 2.1f, 3.1f), v0, v1);
		ass_true(r.x == -1.0f && r.y == 2.0f && r.z == 3.0f);

		r = CxVec3(-1.1f, -2.1f, -3.1f);
		r = CxClamped(r, v0, v1);
		ass_true(r.x == -1.0f && r.y == -2.0f && r.z == -3.0f);
		r = CxVec3(1.1f, 2.1f, 3.1f);
		r = CxClamped(r, v0, v1);
		ass_true(r.x == 1.0f && r.y == 2.0f && r.z == 3.0f);

		r = CxVec3(-1.1f, 1.1f, -1.1f);
		r = CxClamped(r, -1.0f, 1.0f);
		ass_true(r.x == -1.0f && r.y == 1.0f && r.z == -1.0f);

		/* Exponential, pow and Log */
		v0 = CxVec3(1, 2, 3);
		r = CxExp(v0);
		ass_true(CxEq(CxVec3(kCxE, kCxE*kCxE, kCxE*kCxE*kCxE), r, 0.00001f));

		r = CxLog(kCxE, v0);
		ass_true(CxEq(CxVec3(0, 0.693147181f, 1.098612288668f), r, 0.00001f));
		r = CxLog(10, v0);
		ass_true(CxEq(CxVec3(0, 0.30102999566f, 0.47712125472f), r, 0.0001f));
		
		r = CxLogE(v0);
		ass_true(CxEq(CxVec3(0, 0.693147181f, 1.098612288668f), r, 0.0001f));

		r = CxLog10(v0);
		ass_true(CxEq(CxVec3(0, 0.30102999566f, 0.47712125472f), r, 0.0001f));

		r = CxPow(v0, 2);
		ass_true(CxEq(CxVec3(1, 4, 9), r));
		r = CxPow(v0, CxVec3(3, 4, 5));
		ass_true(CxEq(CxVec3(1, 16, 243), r));

		/* max and min */
		v0 = CxVec3(1, -1, 0);
		v1 = CxVec3(-1, 1, 2);

		r = CxMax(v0, v1);
		ass_true(r.x == 1 && r.y == 1 && r.z == 2);
		r = CxMin(v0, v1);
		ass_true(r.x == -1 && r.y == -1 && r.z == 0);

		/* sqrt */
		v0 = CxVec3(9, 81, 64);
		r = CxSqrt(v0);
		ass_true(CxEq(CxVec3(3, 9, 8), r));
		r = CxRecipSqrt(v0);
		ass_true(CxEq(CxVec3(1.f/3.f, 1.f/9.f, 1.f/8.f), r));

		/* Sign */
		v0 = CxVec3(1, 1,1);
		v1 = CxVec3(-1, 1, -1);
		v2 = CxVec3(1, -1, -1);
		v3 = CxVec3(-1,-1, 1);

		r = CxSign(v0);
		ass_true(r.x == 1 && r.y == 1 && r.z == 1);
		r = CxSign(v1);
		ass_true(r.x == -1 && r.y == 1 && r.z == -1);
		r = CxSign(v2);
		ass_true(r.x == 1 && r.y == -1 && r.z == -1);
		r = CxSign(v3);
		ass_true(r.x == -1 && r.y == -1 && r.z == 1);

		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxVec3Basics();
	cat::testCxVec3Arithmetic();
	cat::testCxVec3Normalize();
	cat::testCxVec3MathOps();
	return 0;
}
