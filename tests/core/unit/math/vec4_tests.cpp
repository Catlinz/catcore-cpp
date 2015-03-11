#include <assert.h>
#include <ctime>
#include <cstdio>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/math/vec4.h"

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)



namespace cc {

	void testBasicVec4CreationAndAssignment() {		
		BEGIN_TEST;
		
		Vec4 empty;
		assert( empty.x == REAL(0.0) && empty.y == REAL(0.0) && empty.z == REAL(0.0) && empty.w == REAL(0.0) );
		
		Vec4 v1(REAL(1.0), REAL(-0.5), REAL(2.45), REAL(-99.9));
		assert( v1.x == REAL(1.0) && v1.y == REAL(-0.5) && v1.z == REAL(2.45) && v1.w == REAL(-99.9));

		Vec4 v2(REAL(-1.99));
		assert( v2.x == REAL(-1.99) && v2.y == REAL(-1.99) && v2.z == REAL(-1.99) && v2.w == REAL(-1.99) );
		
		Real arr[4] = { REAL(9.0), REAL(-34.2), REAL(0.0), REAL(-93.2) };
		Vec4 v3(arr);
		assert( v3.x == REAL(9.0) && v3.y == REAL(-34.2) && v3.z == REAL(0.0) && v3.w == REAL(-93.2) );
		
		v3 = v2;
		assert( v3.x == REAL(-1.99) && v3.y == REAL(-1.99) && v3.z == REAL(-1.99) && v3.w == REAL(-1.99) );
		v3 = v1;
		assert( v3.x == REAL(1.0) && v3.y == REAL(-0.5) && v3.z == REAL(2.45) && v3.w == REAL(-99.9) );
		v3 = empty;
		assert( v3.x == REAL(0.0) && v3.y == REAL(0.0) && v3.z == REAL(0.0) && v3.w == REAL(0.0) );

		v3 = REAL(-45.34);
		assert( v3.x == REAL(-45.34) && v3.y == REAL(-45.34) && v3.z == REAL(-45.34) && v3.w == REAL(-45.34));


		FINISH_TEST;	  
	}

	void testVec4Equality() {
		BEGIN_TEST;
		
		Vec4 v1;
		Vec4 v2(REAL(0.001));

		assert( v1 == v1 && v2 == v2 );		
		
		assert( v1 != v2 && v2 != v1);
		v1 = v2;
		assert( v1 == v2 && v2 == v1);

		v1 = Vec4(REAL(1.0), REAL(-1.0), REAL(-2.0), REAL(3.0));
		assert( v1 != v2 && v2 != v1 );
		assert( !(v1 == v2) && !(v2 == v1) );
		v2 = Vec4(REAL(1.0), REAL(-1.0), REAL(-2.0), REAL(3.0));
		assert( v1 == v2 && v2 == v1 );
		assert( !(v1 != v2) && !(v2 != v1) );				
		
		assert( v1.approx(v1) && v2.approx(v2) );		
		assert( v1.approx(v2) && v2.approx(v1) );
		v2 = v1 + Vec4::EPSILON;
		assert( !v1.approx(v2) && !v2.approx(v1) );
		v2 = v1 + (Vec4::EPSILON / REAL(2.0));
		assert( v1.approx(v2) && v2.approx(v1) );

		FINISH_TEST;		
	}

	void testVec4LengthAndUnitLength() {
		BEGIN_TEST;
		
		Vec4 v1(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0));
		Vec4 v2 = v1 + REAL(0.001);
		
		assert( v1.isUnitLength() );
		assert( v1.length() == REAL(1.0) && v1.lengthSquared() == REAL(1.0) );

		assert( !v2.isUnitLength() );
		assert( v2.length() > REAL(1.0) && v2.lengthSquared() > REAL(1.0) );
		assert( v2.lengthSquared() > v2.length() );

		Vec4 v3 = -v2;
		assert( !v3.isUnitLength() );
		assert( v3.length() == v2.length() && v2.lengthSquared() == v2.lengthSquared() );
		
		Vec4 v4 = -v1;
		assert( v4.isUnitLength() );
		assert( v4.length() == REAL(1.0) && v4.lengthSquared() == REAL(1.0) );
		
		v3 = Vec4(REAL(0.182574), REAL(0.365148), REAL(0.547723), REAL(0.730297));
		assert( v3.isUnitLength() );

		v4 = Vec4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		assert( !v4.isUnitLength() );
		assert( v4.lengthSquared() > v4.length() );

		FINISH_TEST;		
	}		

	void testVec4DistanceTo() {
		BEGIN_TEST;
		
		Vec4 a(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0));
		Vec4 b(REAL(2.0), REAL(0.0), REAL(0.0), REAL(0.0));
				
		assert( a.distanceTo(b) == REAL(1.0) );
		assert( b.distanceTo(a) == REAL(1.0) );
		assert( a.distanceSquaredTo(b) == REAL(1.0) );
		assert( b.distanceSquaredTo(a) == REAL(1.0) );
		
		b.x = REAL(-1.0);
		assert( a.distanceTo(b) == REAL(2.0) );
		assert( b.distanceTo(a) == REAL(2.0) );
		assert( a.distanceSquaredTo(b) == REAL(4.0) );
		assert( b.distanceSquaredTo(a) == REAL(4.0) );

		b = Vec4(REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0));
		assert( approx(a.distanceTo(b), REAL(1.41421)) );
		assert( approx(b.distanceTo(a), REAL(1.41421)) );
		assert( approx(a.distanceSquaredTo(b), REAL(2.0)) );
		assert( approx(b.distanceSquaredTo(a), REAL(2.0)) );

		FINISH_TEST;
	}
	

	void testVec4DotProduct() {
		BEGIN_TEST;
		
		Vec4 v1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Vec4 v2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Vec4 v3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));

		assert( v1.dotProduct(v2) == REAL(4.0));
		assert( v2.dotProduct(v1) == REAL(4.0));

		assert( v1.dotProduct(v3) == REAL(-1.0));
		assert( v3.dotProduct(v1) == REAL(-1.0));

		assert( v2.dotProduct(v3) == REAL(-11.0));
		assert( v3.dotProduct(v2) == REAL(-11.0));

		assert( v1.dotProduct(Vec4()) == REAL(0.0) );
		assert( v1.dotProduct(Vec4(REAL(1.0))) == REAL(10.0));

		v1 = Vec4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0));
		v2 = Vec4(REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0));
		v3 = Vec4(REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0));
		Vec4   v4(REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));
		
		assert( v1.dotProduct(v2) == REAL(0.0) );
		assert( v1.dotProduct(v3) == REAL(0.0) );
		assert( v1.dotProduct(v4) == REAL(0.0) );
		assert( v2.dotProduct(v3) == REAL(0.0) );
		assert( v2.dotProduct(v4) == REAL(0.0) );
		assert( v3.dotProduct(v4) == REAL(0.0) );

		// Dot product > 0 if angle < 90
		v2.x = REAL(0.1);
		assert( v1.dotProduct(v2) > REAL(0.0) );

		// Dot product < 0 if angle > 90
		v2.x = -REAL(0.1);
		assert( v1.dotProduct(v2) < REAL(0.0) );		
		
		FINISH_TEST;
	}


	void testVec4Normalise() {
		BEGIN_TEST;
		
		Vec4 v1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Vec4 v2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Vec4 v3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));


		Vec4 res;

		res = v1.normalised();
		assert( res.isUnitLength());
		res = v2.normalised();		
		assert( res.isUnitLength() );
		res = v3.normalised();		
		assert( res.isUnitLength() );

		assert( !v1.isUnitLength() );		
		v1.normalise();		
		assert( v1.isUnitLength() );
		assert( !v2.isUnitLength() );		
		v2.normalise();		
		assert( v2.isUnitLength() );
		assert( !v3.isUnitLength() );		
		v3.normalise();		
		assert( v3.isUnitLength() );

		v1 = Vec4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0));
		assert( v1.isUnitLength() && v1.length() == REAL(1.0) );
		v1.normalise();
		assert( v1.isUnitLength() && v1.length() == REAL(1.0) );

		FINISH_TEST;		
	}

	void testVec4Negation() {
		BEGIN_TEST;
		
		Vec4 v1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Vec4 v2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Vec4 v3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));

		Vec4 res;
		
		assert( v1.negated() == Vec4(REAL(-1.0), REAL(-2.0), REAL(-3.0), REAL(-4.0)) );
		assert( v2.negated() == Vec4(REAL(-2.0), REAL(-3.0), REAL(0.0), REAL(1.0)) );
		assert( v3.negated() == Vec4(REAL(0.0), REAL(3.0), REAL(1.0), REAL(-2.0)) );

		res = v1.negated();		
		assert( res == Vec4(REAL(-1.0), REAL(-2.0), REAL(-3.0), REAL(-4.0)) );
		res.negate();
		assert( res == v1 );

		res = -v1;
		assert( res == Vec4(REAL(-1.0), REAL(-2.0), REAL(-3.0), REAL(-4.0)) );
		res = -res;
		assert( res == v1 );
		
		FINISH_TEST;
		
	}

	void testVec4Addition() {
		BEGIN_TEST;

		Vec4 v1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Vec4 v2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Vec4 v3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));

		Vec4 res;

		res = v1 + (v2 + v3);		
		assert(res.x == 3.0f && res.y == 2.0f && res.z == 2.0f && res.w == 5.0f);
		res = v2 + (v3 + v1);
		assert(res.x == 3.0f && res.y == 2.0f && res.z == 2.0f && res.w == 5.0f);
		res = (v3 + v1) + v2;
		assert(res.x == 3.0f && res.y == 2.0f && res.z == 2.0f && res.w == 5.0f);

		v2 += v3;
		v1 += v2;		
		assert(v1.x == 3.0f && v1.y == 2.0f && v1.z == 2.0f && res.w == 5.0f);
	  
		v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
	   v2 += (v3 += v1);
		assert(v2.x == 3.0f && v2.y == 2.0f && v2.z == 2.0f && v2.w == 5.0f);

		v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v3 += v1;
		v3 += v2;		
		assert(v3.x == 3.0f && v3.y == 2.0f && v3.z == 2.0f && v3.w == 5.0f);
   
		v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		res = v1 + 1.0f;
		assert(res.x == 2.0f && res.y == 3.0f && res.z == 4.0f && res.w == 5.0f);
		res = v2 + (v3 + 1.0f);
		assert(res.x == 3.0f && res.y == 1.0f && res.z == 0.0f && res.w == 2.0f);
		res = (v3 + -1.0f) + v2;
		assert(res.x == 1.0f && res.y == -1.0f && res.z == -2.0f && res.w == 0.0f);

		res = REAL(1.0) + v1;

		assert(res.x == 2.0f && res.y == 3.0f && res.z == 4.0f && res.w == 5.0f);
		res = v2 + (1.0f + v3);
		assert(res.x == 3.0f && res.y == 1.0f && res.z == 0.0f && res.w == 2.0f);
		res = (-1.0f + v3) + v2;
		assert(res.x == 1.0f && res.y == -1.0f && res.z == -2.0f && res.w == 0.0f);

		v1 += 1.0f;
		assert(v1.x == 2.0f && v1.y == 3.0f && v1.z == 4.0f && v1.w == 5.0f);
	   
	   v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v2 += (v3 += 1.0f);		
		assert(v2.x == 3.0f && v2.y == 1.0f && v2.z == 0.0f && v2.w == 2.0f);

		v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v3 += -1.0f;
		v3 += v2;
		assert(v3.x == 1.0f && v3.y == -1.0f && v3.z == -2.0f && v3.w == 0.0f);

		FINISH_TEST;
	}
	
	void testVec4Subtraction() {
		BEGIN_TEST;
		
		Vec4 v1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Vec4 v2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Vec4 v3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));

		Vec4 res;
		
		res = v1 - (v2 - v3);
		assert(res.x == -1.0f && res.y == -4.0f && res.z == 2.0f && res.w == 7.0f);
		res = v2 - (v3 - v1);
		assert(res.x == 3.0f && res.y == 8.0f && res.z == 4.0f && res.w == 1.0f);
		res = (v3 - v1) - v2;
		assert(res.x == -3.0f && res.y == -8.0f && res.z == -4.0f && res.w == -1.0f);

		v1 -= (v2 -= v3);
		assert(v1.x == -1.0f && v1.y == -4.0f && v1.z == 2.0f && v1.w == 7.0f);
	   
	   v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v2 -= (v3 -= v1);		
		assert(v2.x == 3.0f && v2.y == 8.0f && v2.z == 4.0f && v2.w == 1.0f);
	   
		v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v3 -= v1;
		v3 -= v2;		
		assert(v3.x == -3.0f && v3.y == -8.0f && v3.z == -4.0f && v3.w == -1.0f);
   
	   v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		res = v1 - 1.0f;
		assert(res.x == 0.0f && res.y == 1.0f && res.z == 2.0f && res.w == 3.0f);
		res = v2 - (v3 - 1.0f);
		assert(res.x == 3.0f && res.y == 7.0f && res.z == 2.0f && res.w == -2.0f);
		res = (v3 - -1.0f) - v2;
		assert(res.x == -1.0f && res.y == -5.0f && res.z == 0.0f && res.w == 4.0f);

		res = 1.0f - v1;
		assert(res.x == 0.0f && res.y == -1.0f && res.z == -2.0f && res.w == -3.0f);
		res = v2 - (1.0f - v3);
		assert(res.x == 1.0f && res.y == -1.0f && res.z == -2.0f && res.w == 0.0f);
		res = (-1.0f - v3) - v2;
		assert(res.x == -3.0f && res.y == -1.0f && res.z == 0.0f && res.w == -2.0f);

		v1 -= 1.0f;
		assert(v1.x == 0.0f && v1.y == 1.0f && v1.z == 2.0f && v1.w == 3.0f);
	   
		v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v2 -= (v3 -= 1.0f);
		assert(v2.x == 3.0f && v2.y == 7.0f && v2.z == 2.0f && v2.w == -2.0f);
		
	   v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v3 -= -1.0f;
		v3 -= v2;
		assert(v3.x == -1.0f && v3.y == -5.0f && v3.z == 0.0f && v3.w == 4.0f);

		FINISH_TEST;
	}

	void testVec4Multiplication() {
		BEGIN_TEST;
		
		Vec4 v1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Vec4 v2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Vec4 v3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));

		Vec4 res;

		res = v1 * 1.0f;
		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f && res.w == 4.0f);
		res = 1.0f * v1;		
		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f && res.w == 4.0f);
		
		res = (v3 * -1.0f) * -1.0f;
		assert(res.x == 0.0f && res.y == -3.0f && res.z == -1.0f && res.w == 2.0f);
		res = (v3 * -1.0f) * 1.5f;
		assert(res.x == 0.0f && res.y == 4.5f && res.z == 1.5f && res.w == -3.0f);

		res = -1.0f * (-1.0f * v3);	  
		assert(res.x == 0.0f && res.y == -3.0f && res.z == -1.0f && res.w == 2.0f);
		res = 1.5f * (-1.0f * v3);		
		assert(res.x == 0.0f && res.y == 4.5f && res.z == 1.5f && res.w == -3.0f);

		v1 *= 1.0f;
		assert(v1.x == 1.0f && v1.y == 2.0f && v1.z == 3.0f && v1.w == 4.0f);
		
		v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v3 *= -1.0f;
		v3 *= -1.0f;
		assert(v3.x == 0.0f && v3.y == -3.0f && v3.z == -1.0f && v3.w == 2.0f);

		v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v3 *= -1.0f;
		v3 *= 1.5f;
		assert(v3.x == 0.0f && v3.y == 4.5f && v3.z == 1.5f && v3.w == -3.0f);

		FINISH_TEST;
	}
	
	void testVec4Division() {
		BEGIN_TEST;

		Vec4 v1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Vec4 v2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Vec4 v3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));
		Vec4 res;

		res = v1 / 1.0f;
		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f && res.w == 4.0f);
		res = (v3 / -1.0f) / -1.0f;
		assert(res.x == 0.0f && res.y == -3.0f && res.z == -1.0f && res.w == 2.0f);
		res = (v3 / -1.0f) / 8.0f;
		assert(res.x == 0.0f && res.y == 0.375f && res.z == 0.125f && res.w == -0.25f);

		res = 1.0f / v1;
		assert(res.x == 1.0f && res.y == 0.5f && res.w == 0.25f && res.approx(Vec4(1.0f, 0.5f, 1.0/3.0, 0.25f)) );
		res = -1.0f / (-1.0f / v1);

		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f && res.w == 4.0f);
		res = 8.0f / (-1.0f / v1);		
		assert(res.x == -8.0f && res.y == -16.0f && res.z == -24.0f && res.w == -32.0f);

		v1 /= 1.0f;		
		assert(v1.x == 1.0f && v1.y == 2.0f && v1.z == 3.0f && v1.w == 4.0f);
		
	   v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v3 /= -1.0f;
		v3 /= -1.0f;
		assert(v3.x == 0.0f && v3.y == -3.0f && v3.z == -1.0f && v3.w == 2.0f);
	
		v1 = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
		v2 = Vec4(2.0f, 3.0f, 0.0f, -1.0f);
		v3 = Vec4(0.0f, -3.0f, -1.0f, 2.0f);
		v3 /= -1.0f;
		v3 /= 8.0f;
		assert(v3.x == 0.0f && v3.y == 0.375f && v3.z == 0.125f && v3.w == -0.25f);

		FINISH_TEST;	
	}
	


}

int main(int argc, char** argv) {
	cc::testBasicVec4CreationAndAssignment();	
	cc::testVec4Equality();	
	cc::testVec4LengthAndUnitLength();
	cc::testVec4DistanceTo();
	cc::testVec4DotProduct();
	cc::testVec4Normalise();	
	cc::testVec4Negation();
	cc::testVec4Addition();	
	cc::testVec4Subtraction();	
	cc::testVec4Multiplication();	
	cc::testVec4Division();	
	return 0;
}

