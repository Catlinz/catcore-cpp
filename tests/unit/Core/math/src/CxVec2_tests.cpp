#include "core/math/CxVec2.h"
#include "core/CxTestCore.h"

namespace cat {

	/**
	 * @tests CxVec2()
	 * @tests CxVec2(CxZero)
	 * @tests CxVec2(CxReal)
	 * @tests CxVec2(const CxReal *)
	 * @tests CxVec2(CxReal, CxReal)
	 * @tests CxVec2(const CxVec2 &)
	 * @tests operator=(const CxVec2 &)
	 * @tests operator[](CxI32)
	 * @tests operator[](CxI32) const
	 * @tests operator==(const CxVec2 &) const
	 * @tests operator!=(const CxVec2 &) const
	 * @tests setZero()
	 * @tests setXY(CxReal, CxReal)
	 * @tests isZero() const
	 * @tests isUnit() const
	 * @tests magnitudeSqr() const
	 * @tests magnitude() const
	 * @tests max() const
	 * @tests min() const
	 * @tests CxEq(const CxVec2 &, const CxVec2 &, CxReal)
	 * @tests CxDot(const CxVec2 &, const CxVec2 &)
	 * @tests CxIsFinite(const CxVec2 &)
	 */
	void testCxVec2Basics() {
		BEGIN_TEST;

		CxVec2 v0;
		CxVec2 v1(kCxZero);
		ass_true(v1.x == 0 && v1.y == 0);

		CxVec2 v2(3);
		ass_true(v2.x == 3 && v2.y == 3);

		CxReal va[] = {9,-4};
		CxVec2 v3(va);
		ass_true(v3.x == 9 && v3.y == -4);

		CxVec2 v4(-22, 93);
		ass_true(v4.x == -22 && v4.y == 93);


		CxVec2 v5(v3);
		ass_true(v5.x == 9 && v5.y == -4);

		v5 = v1;
		ass_true(v5.x == 0 && v5.y == 0);

		v4 = CxVec2(kCxZero);
		ass_true(v4.x == 0 && v4.y == 0);

		v4 = CxVec2(-22, 93);
		ass_true(v4[0] == -22 && v4[1] == 93);
		const CxVec2 v6 = v4;
		ass_true(v6[0] == -22 && v6[1] == 93);

		ass_true(v1 == v1 && !(v1 != v1));
		ass_true(v5 == v1 && v1 == v5);
		ass_false(v5 != v1 || v1 != v5);
		ass_false(v1 == v4 || v4 == v1);
		ass_true(v1 != v4 && v4 != v1);

		v0 = CxVec2(0.0001f, 1.0001f);
		ass_true(CxEq(CxVec2(0, 1), v0, 0.001f));
		ass_true(CxEq(v0, CxVec2(0, 1), 0.001f));
		ass_false(CxEq(CxVec2(0, 1), v0, 0.0001f));
		ass_false(CxEq(v0, CxVec2(0, 1), 0.0001f));
		ass_false(CxEq(CxVec2(0, 1), v0));
		ass_false(CxEq(v0, CxVec2(0, 1)));

		v0 = CxVec2(kCxZero);
		v1 = CxVec2(0.001f, 0);
		v2 = CxVec2(0, 0.001f);
		v3 = CxVec2(0.001f, -0.001f);
		ass_true(v0.isZero());
		ass_false(v1.isZero() || v2.isZero() || v3.isZero());
		v3.setZero();
		ass_true(v3.isZero());

		v0 = CxVec2(kCxZero);
		v1 = CxVec2(1, 0);
		v2 = CxVec2(0, 1);
		v3 = CxVec2(0, 1.001f);
		v4 = CxVec2(1.001f, 0);
		v5 = CxVec2(0.001f, 1);
		CxVec2 v7 = CxVec2(1, 0.001f);
		
		ass_false(v0.isUnit());
		ass_true(v1.isUnit() && (-v1).isUnit());
		ass_true(v2.isUnit() && (-v2).isUnit());
		ass_false(v3.isUnit() || v4.isUnit() || v5.isUnit() || v7.isUnit());

		v0.setXY(-3, 2);
		ass_true(v0.x == -3 && v0.y == 2);

		v0.setXY(0,0);
		v1.setXY(1,1);
		v2.setXY(1,0);
		v3.setXY(0,-1);
		v4.setXY(-4,-2);
		v5.setXY(-1,-1);

		ass_true(v0.magnitude() == 0 && v0.magnitudeSqr() == 0);
		ass_true(v2.magnitude() == 1 && v2.magnitudeSqr() == 1);
		ass_true(v3.magnitude() == 1 && v3.magnitudeSqr() == 1);
		ass_true(CxEq(v1.magnitude(), kCxSqrtTwo) && v1.magnitudeSqr() == 2);
		ass_true(CxEq(v4.magnitude(), CxSqrt(20.f)) && v4.magnitudeSqr() == 20);
	   ass_true(CxEq(v5.magnitude(), kCxSqrtTwo) && v5.magnitudeSqr() == 2);

		v0.setXY(0,0);
		v1.setXY(1,0);
		v2.setXY(0,1);
		v3.setXY(-1,0);
		v4.setXY(0,-1);
		v5.setXY(-1,1);
		v7.setXY(2, 2.5);

		ass_true(v0.max() == 0 && v0.min() == 0);
		ass_true(v1.max() == 1 && v1.min() == 0);
		ass_true(v2.max() == 1 && v2.min() == 0);
		ass_true(v3.max() == 0 && v3.min() == -1);
		ass_true(v4.max() == 0 && v4.min() == -1);
		ass_true(v5.max() == 1 && v5.min() == -1);
		ass_true(v7.max() == 2.5 && v7.min() == 2);

		v0.setXY(0,0);
		v1.setXY(1,0);
		v2.setXY(0,1);
		v3.setXY(-1,-1);
		v4.setXY(3,4);
		v5.setXY(-5, 30);

		ass_true(CxDot(v0, v0) == 0 && CxDot(v1,v1) == 1 && CxDot(v2,v2) == 1);
		ass_true(CxDot(v3, v3) == 2 && CxDot(v4,v4) == 25 && CxDot(v5,v5) == 925);

		ass_true(CxDot(v0, v1) == 0 && CxDot(v1, v0) == 0);
		ass_true(CxDot(v1, v2) == 0 && CxDot(v2, v1) == 0);
		ass_true(CxDot(v1, v3) == -1 && CxDot(v3, v1) == -1);
		ass_true(CxDot(v2, v3) == -1 && CxDot(v3, v2) == -1);
		ass_true(CxDot(v4, v5) == 105 && CxDot(v5, v4) == 105);


		v0.setXY(0,0);
		v1.setXY(1,1);
		v2.setXY(CX_NAN, 1);
		v3.setXY(1, CX_INF);
		v4.setXY(CX_INF, CX_NAN);

		ass_true(CxIsFinite(v0) && CxIsFinite(v1));
		ass_false(CxIsFinite(v2) || CxIsFinite(v3) || CxIsFinite(v4));
		
		FINISH_TEST;
	}

	/**
	 * @tests operator-() const
	 * @tests operator+(const CxVec2 &) const
	 * @tests operator-(const CxVec2 &) const
	 * @tests operator*(const CxVec2 &) const
	 * @tests operator*(CxReal) const
	 * @tests operator/(const CxVec2 &) const
	 * @tests operator/(CxReal) const
	 * @tests operator+=(const CxVec2 &)
	 * @tests operator+=(CxReal)
	 * @tests opeartor-=(const CxVec2 &)
	 * @tests operator-=(CxReal)
	 * @tests opeartor*=(const CxVec2 &)
	 * @tests operator*=(CxReal)
	 * @tests opeartor/=(const CxVec2 &)
	 * @tests operator/=(CxReal)
	 */
	void testCxVec2Arithmetic() {
		BEGIN_TEST;

		/* operator-() */
		CxVec2 v0(1, -2);
		CxVec2 v1 = -v0;
		ass_true(v0 != v1 && v1 != v0);
		ass_true(v0.x == 1 && v0.y == -2);
		ass_true(v1.x == -1 && v1.y == 2);

		/* operator+(const CxVec2 &) */
		CxVec2 v2 = v0 + v1;
		ass_true(v2.x == 0 && v2.y == 0);
		v2 = v1 + v0;
		ass_true(v2.x == 0 && v2.y == 0);

		v2 = v0 + v0;
		ass_true(v2.x == 2 && v2.y == -4);
		v2 = v1 + v1;
		ass_true(v2.x == -2 && v2.y == 4);

		/* operator-(const CxVec2 &) */
		v2 = v0 - v0;
		ass_true(v2.x == 0 && v2.y == 0);
		v2 = v1 - v1;
		ass_true(v2.x == 0 && v2.y == 0);

		v2 = v0 - v1;
		ass_true(v2.x == 2 && v2.y == -4);
		v2 = v1 - v0;
		ass_true(v2.x == -2 && v2.y == 4);

		/* operator*(const CxVec2 &) */
		v2 = v0 * v0;
		ass_true(v2.x == 1 && v2.y == 4);
		v2 = CxVec2(2,-3) * CxVec2(4,5);
		ass_true(v2.x == 8 && v2.y == -15);

		/* operator*(CxReal) */
		v2 = v0 * 2;
		ass_true(v2.x == 2 && v2.y == -4);
		v2 = CxVec2(8,0) * -3;
		ass_true(v2.x = -24 && v2.y == 0);

		/* operator/(const CxVec2 &) */
		v2 = v0 / v0;
		ass_true(v2.x == 1 && v2.y == 1);
		v2 = CxVec2(8,-15) / CxVec2(4,5);
		ass_true(v2.x == 2 && v2.y == -3);

		/* operator/(CxReal) */
		v2 = v0 / 1;
		ass_true(v2.x == 1 && v2.y == -2);
		v2 = v0 / 2;
		ass_true(CxEq(CxVec2(0.5f, -1), v2));

		/* operator+=(const CxVec2 &) */
		v2 = v0;
		v2 += CxVec2(3, 2);
		ass_true(v2.x == 4 && v2.y == 0);
		v2 += CxVec2(-3, 1);
		ass_true(v2.x == 1 && v2.y == 1);

		/* operator+=(CxReal) */
		v2 = v0;
		v2 += 10;
		ass_true(v2.x == 11 && v2.y == 8);
		v2 += -10;
		ass_true(v2.x == 1 && v2.y == -2);
		
		/* operator-=(const CxVec2 &) */
		v2 = v0;
		v2 -= CxVec2(3, 2);
		ass_true(v2.x == -2 && v2.y == -4);
		v2 -= CxVec2(-1, -3);
		ass_true(v2.x == -1 && v2.y == -1);

		/* operator-=(CxReal) */
		v2 = v0;
		v2 -= 10;
		ass_true(v2.x == -9 && v2.y == -12);
		v2 -= -10;
		ass_true(v2.x == 1 && v2.y == -2);

		/* operator*=(const CxVec2 &) */
		v2 = v0;
		v2 *= CxVec2(1,1);
		ass_true(v2.x == 1 && v2.y == -2);
		v2 = v0;
		v2 *= CxVec2(-2, 10);
		ass_true(CxEq(CxVec2(-2, -20), v2));
		v2 *= CxVec2(0, -1);
		ass_true(v2.x == 0 && v2.y == 20);
		
		/* operator*=(CxReal) */
		v2 = v0;
		v2 *= 1;
		ass_true(v2.x == 1 && v2.y == -2);
		v2 *= 2;
		ass_true(v2.x == 2 && v2.y == -4);
		v2 *= -1;
		ass_true(v2.x == -2 && v2.y == 4);
		v2 *= 0;
		ass_true(v2.x == 0 && v2.y == 0);

		/* operator/=(const CxVec2 &) */
		v2 = v0;
		v2 /= CxVec2(1,1);
		ass_true(v2.x == 1 && v2.y == -2);
		v2 = v0;
		v2 /= CxVec2(-2, 10);
		ass_true(CxEq(CxVec2(-0.5f, -0.2f), v2));
		
		/* operator/=(CxReal) */
		v2 = v0;
		v2 /= 1;
		ass_true(v2.x == 1 && v2.y == -2);
		v2 /= 2;
		ass_true(CxEq(CxVec2(0.5f, -1), v2));
		v2 /= -1;
		ass_true(CxEq(CxVec2(-0.5f, 1), v2));
		
		
		FINISH_TEST;
	}

	void testCxVec2Normalize() {
		BEGIN_TEST;

		CxVec2 v0(kCxZero);
		CxVec2 v1(0, 1);
		CxVec2 v2(1, 0);
		CxVec2 v3(1, 1);
		CxVec2 v5(5, 2);

		CxVec2 v4 = v0.normalized();
		ass_true(v4.x == 0 && v4.y == 0);
		
		v4 = v1.normalized();
		ass_true(v4.x == 0 && v4.y == 1);
		v4 = v1.normalizedNonZero();
		ass_true(v4.x == 0 && v4.y == 1);

		v4 = v2.normalized();
		ass_true(v4.x == 1 && v4.y == 0);
		v4 = v2.normalizedNonZero();
		ass_true(v4.x == 1 && v4.y == 0);

		v4 = v3.normalized();
		ass_true(CxEq(CxVec2(kCxRecipSqrtTwo, kCxRecipSqrtTwo), v4));
		v4 = v3.normalizedNonZero();
		ass_true(CxEq(CxVec2(kCxRecipSqrtTwo, kCxRecipSqrtTwo), v4));
		ass_true(v3.x == 1 && v3.y == 1);

		v4 = v5.normalized();
		ass_true(CxEq(CxVec2(5.f/CxSqrt(29.f), 2.f/CxSqrt(29.f)), v4));
		v4 = v5.normalizedNonZero();
		ass_true(CxEq(CxVec2(5.f/CxSqrt(29.f), 2.f/CxSqrt(29.f)), v4));

		v0.normalize();
		ass_true(v0.x == 0 && v0.y == 0);
		v1.normalize();
		ass_true(v1.x == 0 && v1.y == 1);
		v2.normalize();
		ass_true(v2.x == 1 && v2.y == 0);

		v4 = v3;
		ass_true(v4.x == 1 && v4.y == 1);
		v3.normalize();
		ass_true(CxEq(CxVec2(kCxRecipSqrtTwo, kCxRecipSqrtTwo), v3));
		v4.normalizeNonZero();
		ass_true(CxEq(CxVec2(kCxRecipSqrtTwo, kCxRecipSqrtTwo), v4));

		v4 = v5;
		ass_true(v4.x == 5 && v4.y == 2);
		v5.normalize();
		ass_true(CxEq(CxVec2(5.f/CxSqrt(29.f), 2.f/CxSqrt(29.f)), v5));
		v4.normalizeNonZero();
		ass_true(CxEq(CxVec2(5.f/CxSqrt(29.f), 2.f/CxSqrt(29.f)), v4));
		
		FINISH_TEST;
	}

	void testCxVec2MathOps() {
		BEGIN_TEST;

		/* CxAbs */
		CxVec2 v0(0,0);
		CxVec2 v1(1,-1);
		CxVec2 v2(-1,1);
		CxVec2 v3(-1,-1);
		CxVec2 v4(23, 123);

		CxVec2 r;
		r = CxAbs(v0);  ass_true(r.x == 0 && r.y == 0);
		r = CxAbs(v1);  ass_true(r.x == 1 && r.y == 1);
		r = CxAbs(v2);  ass_true(r.x == 1 && r.y == 1);
		r = CxAbs(v3);  ass_true(r.x == 1 && r.y == 1);
		r = CxAbs(v4);  ass_true(r.x == 23 && r.y == 123);

		/* Trig functions */
		v0.setXY(0.5, 1);

		r = CxAcos(v0);
		ass_true(CxEq(CxVec2(1.04719755f, 0), r, 0.0001f));
		r = CxAsin(v0);
		ass_true(CxEq(CxVec2(0.523598776f, 1.57079633f), r, 0.0001f));
		r = CxAtan(v0);
		ass_true(CxEq(CxVec2(0.46364761f, 0.7853981634f), r, 0.0001f));

		v0.setXY(1.04719755f, 1.57079633f);
		r = CxCos(v0);
		ass_true(CxEq(CxVec2(0.5f, 0.0f), r, 0.0001f));
		r =  CxSin(v0);
		ass_true(CxEq(CxVec2(0.866025403f, 1), r, 0.0001f));
		v0.y = 0.5f;
		r = CxTan(v0);
		ass_true(CxEq(CxVec2(1.7320508028f, 0.5463024898f), r, 0.0001f));

		/* Ceil and floor*/
	   r = CxCeil(CxVec2(0.4f, -0.4f));
		ass_true(CxEq(CxVec2(1, 0), r));

		r = CxFloor(CxVec2(-0.4f, 0.6f));
		ass_true(CxEq(CxVec2(-1, 0), r));

		/* Clamp */
		r = CxClamp(CxVec2(0.9f, -0.9f), -1.0f, 1.0f);
		ass_true(r.x == 0.9f && r.y == -0.9f);
		r = CxClamp(CxVec2(1.1f, -1.1f), -1.0f, 1.0f);
		ass_true(r.x == 1.0f && r.y == -1.0f);

		v0 = CxVec2(-1, -2);
		v1 = CxVec2(1, 2);
		r = CxClamp(CxVec2(0.9f, -1.9f), v0, v1);
		ass_true(r.x == 0.9f && r.y == -1.9f);
		r = CxClamp(CxVec2(1.1f, -2.1f), v0, v1);
		ass_true(r.x == 1.0f && r.y == -2.0f);
		r = CxClamp(CxVec2(-1.1f, 2.0f), v0, v1);
		ass_true(r.x == -1.0f && r.y == 2.0f);

		r = CxVec2(-1.1f, -2.1f);
		r = CxClamped(r, v0, v1);
		ass_true(r.x == -1.0f && r.y == -2.0f);
		r = CxVec2(1.1f, 2.1f);
		r = CxClamped(r, v0, v1);
		ass_true(r.x == 1.0f && r.y == 2.0f);

		r = CxVec2(-1.1f, 1.1f);
		r = CxClamped(r, -1.0f, 1.0f);
		ass_true(r.x == -1.0f && r.y == 1.0f);

		/* Exponential, pow and Log */
		v0 = CxVec2(1, 2);
		r = CxExp(v0);
		ass_true(CxEq(CxVec2(kCxE, kCxE*kCxE), r, 0.000001f));

		r = CxLog(kCxE, v0);
		ass_true(CxEq(CxVec2(0, 0.693147181f), r, 0.00001f));
		r = CxLog(10, v0);
		ass_true(CxEq(CxVec2(0, 0.30102999566f), r, 0.0001f));
		
		r = CxLogE(v0);
		ass_true(CxEq(CxVec2(0, 0.693147181f), r, 0.0001f));

		r = CxLog10(v0);
		ass_true(CxEq(CxVec2(0, 0.30102999566f), r, 0.0001f));

		r = CxPow(v0, 2);
		ass_true(CxEq(CxVec2(1, 4), r));
		r = CxPow(v0, CxVec2(3, 4));
		ass_true(CxEq(CxVec2(1, 16), r));

		/* max and min */
		v0 = CxVec2(1, -1);
		v1 = CxVec2(-1, 1);

		r = CxMax(v0, v1);
		ass_true(r.x == 1 && r.y == 1);
		r = CxMin(v0, v1);
		ass_true(r.x == -1 && r.y == -1);

		/* sqrt */
		v0 = CxVec2(9, 81);
		r = CxSqrt(v0);
		ass_true(CxEq(CxVec2(3, 9), r));
		r = CxRecipSqrt(v0);
		ass_true(CxEq(CxVec2(1.f/3.f, 1.f/9.f), r));

		/* Sign */
		v0 = CxVec2(1, 1);
		v1 = CxVec2(-1, 1);
		v2 = CxVec2(1, -1);
		v3 = CxVec2(-1,-1);

		r = CxSign(v0);
		ass_true(r.x == 1 && r.y == 1);
		r = CxSign(v1);
		ass_true(r.x == -1 && r.y == 1);
		r = CxSign(v2);
		ass_true(r.x == 1 && r.y == -1);
		r = CxSign(v3);
		ass_true(r.x == -1 && r.y == -1);

		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxVec2Basics();
	cat::testCxVec2Arithmetic();
	cat::testCxVec2Normalize();
	cat::testCxVec2MathOps();
	return 0;
}
