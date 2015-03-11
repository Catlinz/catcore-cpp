#include <assert.h>
#include <ctime>
#include <cstdio>
#ifndef DEBUG
#define DEBUG 1
#endif
#include "core/math/vec3.h"

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)



namespace cc {

	void testBasicVec3CreationAndAssignment() {		
		BEGIN_TEST;
		
		Vec3 empty;
		assert( empty.x == REAL(0.0) && empty.y == REAL(0.0) && empty.z == REAL(0.0) );
		
		Vec3 v1(REAL(1.0), REAL(-0.5), REAL(2.45));
		assert( v1.x == REAL(1.0) && v1.y == REAL(-0.5) && v1.z == REAL(2.45) );

		Vec3 v2(REAL(-1.99));
		assert( v2.x == REAL(-1.99) && v2.y == REAL(-1.99) && v2.z == REAL(-1.99) );
		
		Real arr[3] = { REAL(9.0), REAL(-34.2), REAL(0.0) };
		Vec3 v3(arr);
		assert( v3.x == REAL(9.0) && v3.y == REAL(-34.2) && v3.z == REAL(0.0) );
		
		v3 = v2;
		assert( v3.x == REAL(-1.99) && v3.y == REAL(-1.99) && v3.z == REAL(-1.99) );
		v3 = v1;
		assert( v3.x == REAL(1.0) && v3.y == REAL(-0.5) && v3.z == REAL(2.45) );
		v3 = empty;
		assert( v3.x == REAL(0.0) && v3.y == REAL(0.0) && v3.z == REAL(0.0) );

		v3 = REAL(-45.34);
		assert( v3.x == REAL(-45.34) && v3.y == REAL(-45.34) && v3.z == REAL(-45.34) );


		FINISH_TEST;	  
	}

	void testVec3Equality() {
		BEGIN_TEST;
		
		Vec3 v1;
		Vec3 v2(REAL(0.001));

		assert( v1 == v1 && v2 == v2 );		
		
		assert( v1 != v2 && v2 != v1);
		v1 = v2;
		assert( v1 == v2 && v2 == v1);

		v1 = Vec3(REAL(1.0), REAL(-1.0), REAL(-2.0));
		assert( v1 != v2 && v2 != v1 );
		assert( !(v1 == v2) && !(v2 == v1) );
		v2 = Vec3(REAL(1.0), REAL(-1.0), REAL(-2.0));
		assert( v1 == v2 && v2 == v1 );
		assert( !(v1 != v2) && !(v2 != v1) );				
		
		assert( v1.approx(v1) && v2.approx(v2) );		
		assert( v1.approx(v2) && v2.approx(v1) );
		v2 = v1 + Vec3::EPSILON;
		assert( !v1.approx(v2) && !v2.approx(v1) );
		
		v2 = v1 + (Vec3::EPSILON / 2.0);
		assert( v1.approx(v2) && v2.approx(v1) );

		FINISH_TEST;		
	}

	void testVec3LengthAndUnitLength() {
		BEGIN_TEST;
		
		Vec3 v1(REAL(1.0), REAL(0.0), REAL(0.0));
		Vec3 v2 = v1 + REAL(0.001);
		
		assert( v1.isUnitLength() );
		assert( v1.length() == REAL(1.0) && v1.lengthSquared() == REAL(1.0) );

		assert( !v2.isUnitLength() );
		assert( v2.length() > REAL(1.0) && v2.lengthSquared() > REAL(1.0) );
		assert( v2.lengthSquared() > v2.length() );

		Vec3 v3 = -v2;
		assert( !v3.isUnitLength() );
		assert( v3.length() == v2.length() && v2.lengthSquared() == v2.lengthSquared() );
		
		Vec3 v4 = -v1;
		assert( v4.isUnitLength() );
		assert( v4.length() == REAL(1.0) && v4.lengthSquared() == REAL(1.0) );
		
		v3 = Vec3(REAL(0.0248682), REAL(0.0994729), REAL(0.994729));
		assert( v3.isUnitLength() );

		v4 = Vec3(REAL(1.0), REAL(4.0), REAL(40.0));
		assert( !v4.isUnitLength() );
		assert( v4.lengthSquared() > v4.length() );

		FINISH_TEST;		
	}	

	void testVec3DistanceTo() {
		BEGIN_TEST;
		
		Vec3 a(REAL(1.0), REAL(0.0), REAL(0.0));
		Vec3 b(REAL(2.0), REAL(0.0), REAL(0.0));
				
		assert( a.distanceTo(b) == REAL(1.0) );
		assert( b.distanceTo(a) == REAL(1.0) );
		assert( a.distanceSquaredTo(b) == REAL(1.0) );
		assert( b.distanceSquaredTo(a) == REAL(1.0) );
		
		b.x = REAL(-1.0);
		assert( a.distanceTo(b) == REAL(2.0) );
		assert( b.distanceTo(a) == REAL(2.0) );
		assert( a.distanceSquaredTo(b) == REAL(4.0) );
		assert( b.distanceSquaredTo(a) == REAL(4.0) );

		b = Vec3(REAL(0.0), REAL(1.0), REAL(0.0));
		assert( approx(a.distanceTo(b), REAL(1.41421)) );
		assert( approx(b.distanceTo(a), REAL(1.41421)) );
		assert( approx(a.distanceSquaredTo(b), REAL(2.0)) );
		assert( approx(b.distanceSquaredTo(a), REAL(2.0)) );

		FINISH_TEST;
	}
	

	void testVec3DotProduct() {
		BEGIN_TEST;
		
		Vec3 v1(REAL(1.0), REAL(2.0), REAL(3.0));
		Vec3 v2(REAL(2.0), REAL(3.0), REAL(0.0));
		Vec3 v3(REAL(0.0), REAL(-3.0), REAL(-1.0));

		assert( v1.dotProduct(v2) == REAL(8.0));
		assert( v2.dotProduct(v1) == REAL(8.0));

		assert( v1.dotProduct(v3) == REAL(-9.0));
		assert( v3.dotProduct(v1) == REAL(-9.0));

		assert( v2.dotProduct(v3) == REAL(-9.0));
		assert( v3.dotProduct(v2) == REAL(-9.0));

		assert( v1.dotProduct(Vec3()) == REAL(0.0) );
		assert( v1.dotProduct(Vec3(REAL(1.0))) == REAL(6.0));

		v1 = Vec3(REAL(1.0), REAL(0.0), REAL(0.0));
		v2 = Vec3(REAL(0.0), REAL(1.0), REAL(0.0));
		v3 = Vec3(REAL(0.0), REAL(0.0), REAL(1.0));
		
		assert( v1.dotProduct(v2) == REAL(0.0) );
		assert( v1.dotProduct(v3) == REAL(0.0) );
		assert( v2.dotProduct(v3) == REAL(0.0) );

		// Dot product > 0 if angle < 90
		v2.x = REAL(0.1);
		assert( v1.dotProduct(v2) > REAL(0.0) );

		// Dot product < 0 if angle > 90
		v2.x = -REAL(0.1);
		assert( v1.dotProduct(v2) < REAL(0.0) );		
		
		FINISH_TEST;
	}

	void testVec3CrossProduct() {
		BEGIN_TEST;
		
		Vec3 v1(REAL(1.0), REAL(2.0), REAL(3.0));
		Vec3 v2(REAL(2.0), REAL(3.0), REAL(0.0));
		Vec3 v3(REAL(0.0), REAL(-3.0), REAL(-1.0));


		assert( v1.crossProduct(v2) == Vec3(REAL(-9.0), REAL(6.0), REAL(-1.0)) );
		assert( v2.crossProduct(v1) == Vec3(REAL(9.0), REAL(-6.0), REAL(1.0)) );
		assert( v1.dotProduct(Vec3(REAL(-9.0), REAL(6.0), REAL(-1.0))) == REAL(0.0) );
		assert( v2.dotProduct(Vec3(REAL(-9.0), REAL(6.0), REAL(-1.0))) == REAL(0.0) );
		assert( v1.dotProduct(Vec3(REAL(9.0), REAL(-6.0), REAL(1.0))) == REAL(0.0) );
		assert( v2.dotProduct(Vec3(REAL(9.0), REAL(-6.0), REAL(1.0))) == REAL(0.0) );

		assert( v1.crossProduct(v3) == Vec3(REAL(7.0), REAL(1.0), REAL(-3.0)) );
		assert( v3.crossProduct(v1) == Vec3(REAL(-7.0), REAL(-1.0), REAL(3.0)) );
		assert( v1.dotProduct(Vec3(REAL(7.0), REAL(1.0), REAL(-3.0))) == REAL(0.0) );
		assert( v3.dotProduct(Vec3(REAL(7.0), REAL(1.0), REAL(-3.0))) == REAL(0.0) );
		assert( v1.dotProduct(Vec3(REAL(-7.0), REAL(-1.0), REAL(3.0))) == REAL(0.0) );
		assert( v3.dotProduct(Vec3(REAL(-7.0), REAL(-1.0), REAL(3.0))) == REAL(0.0) );

		assert( v2.crossProduct(v3) == Vec3(REAL(-3.0), REAL(2.0), REAL(-6.0)) );
		assert( v3.crossProduct(v2) == Vec3(REAL(3.0), REAL(-2.0), REAL(6.0)) );
		assert( v2.dotProduct(Vec3(REAL(-3.0), REAL(2.0), REAL(-6.0))) == REAL(0.0) );
		assert( v3.dotProduct(Vec3(REAL(-3.0), REAL(2.0), REAL(-6.0))) == REAL(0.0) );
		assert( v2.dotProduct(Vec3(REAL(3.0), REAL(-2.0), REAL(6.0))) == REAL(0.0) );
		assert( v3.dotProduct(Vec3(REAL(3.0), REAL(-2.0), REAL(6.0))) == REAL(0.0) );

		v1 = Vec3(REAL(1.0), REAL(0.0), REAL(0.0));
		v2 = Vec3(REAL(0.0), REAL(1.0), REAL(0.0));
		v3 = Vec3(REAL(0.0), REAL(0.0), REAL(1.0));

		assert( v1.crossProduct(v2) == v3 );
		assert( v3.crossProduct(v1) == v2 );
		assert( v2.crossProduct(v3) == v1 );

		assert( v1.crossProduct(Vec3(REAL(2.0), REAL(0.0), REAL(0.0))).lengthSquared() == REAL(0.0) );
		
		
		FINISH_TEST;
	}

	void testVec3VectorProjection() {
		BEGIN_TEST;
		
		Vec3 a(REAL(2.0), REAL(0.5), REAL(0.0));
		Vec3 b(REAL(0.5), REAL(2.0), REAL(0.0));
		
		Vec3 pab = a.project(b);
		assert( pab.approx(Vec3(4.0/17.0, 16.0/17.0, 0.0)) );

		a = Vec3(REAL(1.0), REAL(4.0), REAL(9.0));
		b = Vec3(REAL(3.0), REAL(5.0), REAL(2.0));
		
		pab = a.project(b);
		assert( pab.approx(Vec3(123.0/38.0, 205.0/38.0, 82.0/38.0)) );

		b = Vec3(REAL(1.0), REAL(0.0), REAL(0.0));
		a = Vec3(REAL(0.0), REAL(1.0), REAL(0.0));
		
		assert( b.dotProduct(a) == REAL(0.0) && a.dotProduct(b) == REAL(0.0) );
		pab = a.project(b);
		assert( pab.lengthSquared() == REAL(0.0) );
		
		FINISH_TEST;
	}

	void testVec3Normalise() {
		BEGIN_TEST;
		
		Vec3 v1(REAL(1.0), REAL(2.0), REAL(3.0));
		Vec3 v2(REAL(2.0), REAL(3.0), REAL(0.0));
		Vec3 v3(REAL(0.0), REAL(-3.0), REAL(-1.0));


		Vec3 res;

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

		v1 = Vec3(REAL(1.0), REAL(0.0), REAL(0.0));
		assert( v1.isUnitLength() && v1.length() == REAL(1.0) );
		v1.normalise();
		assert( v1.isUnitLength() && v1.length() == REAL(1.0) );

		FINISH_TEST;		
	}

	void testVec3Negation() {
		BEGIN_TEST;
		
		Vec3 v1(REAL(1.0), REAL(2.0), REAL(3.0));
		Vec3 v2(REAL(2.0), REAL(3.0), REAL(0.0));
		Vec3 v3(REAL(0.0), REAL(-3.0), REAL(-1.0));

		Vec3 res;
		
		assert( v1.negated() == Vec3(REAL(-1.0), REAL(-2.0), REAL(-3.0)) );
		assert( v2.negated() == Vec3(REAL(-2.0), REAL(-3.0), REAL(0.0)) );
		assert( v3.negated() == Vec3(REAL(0.0), REAL(3.0), REAL(1.0)) );

		res = v1.negated();		
		assert( res == Vec3(REAL(-1.0), REAL(-2.0), REAL(-3.0)) );
		res.negate();
		assert( res == v1 );

		res = -v1;
		assert( res == Vec3(REAL(-1.0), REAL(-2.0), REAL(-3.0)) );
		res = -res;
		assert( res == v1 );
		
		FINISH_TEST;
		
	}

	void testVec3Addition() {
		BEGIN_TEST;

		Vec3 v1(1.0f, 2.0f, 3.0f);
		Vec3 v2(2.0f, 3.0f, 0.0f);
		Vec3 v3(0.0f, -3.0f, -1.0f);

		Vec3 res;

		res = v1 + (v2 + v3);		
		assert(res.x == 3.0f && res.y == 2.0f && res.z == 2.0f);
		res = v2 + (v3 + v1);
		assert(res.x == 3.0f && res.y == 2.0f && res.z == 2.0f);
		res = (v3 + v1) + v2;
		assert(res.x == 3.0f && res.y == 2.0f && res.z == 2.0f);

		v2 += v3;
		v1 += v2;		
		assert(v1.x == 3.0f && v1.y == 2.0f && v1.z == 2.0f);
	  
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
	   v2 += (v3 += v1);
		assert(v2.x == 3.0f && v2.y == 2.0f && v2.z == 2.0f);

		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v3 += v1;
		v3 += v2;		
		assert(v3.x == 3.0f && v3.y == 2.0f && v3.z == 2.0f);
   
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		res = v1 + 1.0f;
		assert(res.x == 2.0f && res.y == 3.0f && res.z == 4.0f);
		res = v2 + (v3 + 1.0f);
		assert(res.x == 3.0f && res.y == 1.0f && res.z == 0.0f);
		res = (v3 + -1.0f) + v2;
		assert(res.x == 1.0f && res.y == -1.0f && res.z == -2.0f);

		res = REAL(1.0) + v1;

		assert(res.x == 2.0f && res.y == 3.0f && res.z == 4.0f);
		res = v2 + (1.0f + v3);
		assert(res.x == 3.0f && res.y == 1.0f && res.z == 0.0f);
		res = (-1.0f + v3) + v2;
		assert(res.x == 1.0f && res.y == -1.0f && res.z == -2.0f);

		v1 += 1.0f;
		assert(v1.x == 2.0f && v1.y == 3.0f && v1.z == 4.0f);
	   
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v2 += (v3 += 1.0f);		
		assert(v2.x == 3.0f && v2.y == 1.0f && v2.z == 0.0f);

		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v3 += -1.0f;
		v3 += v2;
		assert(v3.x == 1.0f && v3.y == -1.0f && v3.z == -2.0f);

		FINISH_TEST;
	}
	
	void testVec3Subtraction() {
		BEGIN_TEST;
		
		Vec3 v1(1.0f, 2.0f, 3.0f);
		Vec3 v2(2.0f, 3.0f, 0.0f);
		Vec3 v3(0.0f, -3.0f, -1.0f);

		Vec3 res;
		
		res = v1 - (v2 - v3);
		assert(res.x == -1.0f && res.y == -4.0f && res.z == 2.0f);
		res = v2 - (v3 - v1);
		assert(res.x == 3.0f && res.y == 8.0f && res.z == 4.0f);
		res = (v3 - v1) - v2;
		assert(res.x == -3.0f && res.y == -8.0f && res.z == -4.0f);

		v1 -= (v2 -= v3);
		assert(v1.x == -1.0f && v1.y == -4.0f && v1.z == 2.0f);
	   
	   v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v2 -= (v3 -= v1);		
		assert(v2.x == 3.0f && v2.y == 8.0f && v2.z == 4.0f);
	   
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v3 -= v1;
		v3 -= v2;		
		assert(v3.x == -3.0f && v3.y == -8.0f && v3.z == -4.0f);
   
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		res = v1 - 1.0f;
		assert(res.x == 0.0f && res.y == 1.0f && res.z == 2.0f);
		res = v2 - (v3 - 1.0f);
		assert(res.x == 3.0f && res.y == 7.0f && res.z == 2.0f);
		res = (v3 - -1.0f) - v2;
		assert(res.x == -1.0f && res.y == -5.0f && res.z == 0.0f);

		res = 1.0f - v1;
		assert(res.x == 0.0f && res.y == -1.0f && res.z == -2.0f);
		res = v2 - (1.0f - v3);
		assert(res.x == 1.0f && res.y == -1.0f && res.z == -2.0f);
		res = (-1.0f - v3) - v2;
		assert(res.x == -3.0f && res.y == -1.0f && res.z == 0.0f);

		v1 -= 1.0f;
		assert(v1.x == 0.0f && v1.y == 1.0f && v1.z == 2.0f);
	   
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v2 -= (v3 -= 1.0f);
		assert(v2.x == 3.0f && v2.y == 7.0f && v2.z == 2.0f);
		
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v3 -= -1.0f;
		v3 -= v2;
		assert(v3.x == -1.0f && v3.y == -5.0f && v3.z == 0.0f);

		FINISH_TEST;
	}

	void testVec3Multiplication() {
		BEGIN_TEST;
		
		Vec3 v1(1.0f, 2.0f, 3.0f);
		Vec3 v2(2.0f, 3.0f, 0.0f);
		Vec3 v3(0.0f, -3.0f, -1.0f);
		Vec3 res;

		res = v1 * 1.0f;
		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f);
		res = 1.0f * v1;		
		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f);
		
		res = (v3 * -1.0f) * -1.0f;
		assert(res.x == 0.0f && res.y == -3.0f && res.z == -1.0f);
		res = (v3 * -1.0f) * 1.5f;
		assert(res.x == 0.0f && res.y == 4.5f && res.z == 1.5f);

		res = -1.0f * (-1.0f * v3);	  
		assert(res.x == 0.0f && res.y == -3.0f && res.z == -1.0f);
		res = 1.5f * (-1.0f * v3);		
		assert(res.x == 0.0f && res.y == 4.5f && res.z == 1.5f);

		v1 *= 1.0f;
		assert(v1.x == 1.0f && v1.y == 2.0f && v1.z == 3.0f);
		
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v3 *= -1.0f;
		v3 *= -1.0f;
		assert(v3.x == 0.0f && v3.y == -3.0f && v3.z == -1.0f);

		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v3 *= -1.0f;
		v3 *= 1.5f;
		assert(v3.x == 0.0f && v3.y == 4.5f && v3.z == 1.5f);

		FINISH_TEST;
	}
	
	void testVec3Division() {
		BEGIN_TEST;

		Vec3 v1(1.0f, 2.0f, 3.0f);
		Vec3 v2(2.0f, 3.0f, 0.0f);
		Vec3 v3(0.0f, -3.0f, -1.0f);
		Vec3 res;

		res = v1 / 1.0f;
		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f);
		res = (v3 / -1.0f) / -1.0f;
		assert(res.x == 0.0f && res.y == -3.0f && res.z == -1.0f);
		res = (v3 / -1.0f) / 8.0f;
		assert(res.x == 0.0f && res.y == 0.375f && res.z == 0.125f);

		res = 1.0f / v1;
		assert(res.x == 1.0f && res.y == 0.5f && res.approx(Vec3(1.0f, 0.5f, 1.0/3.0)) );
		res = -1.0f / (-1.0f / v1);

		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f);
		res = 8.0f / (-1.0f / v1);		
		assert(res.x == -8.0f && res.y == -16.0f && res.z == -24.0f);

		v1 /= 1.0f;		
		assert(v1.x == 1.0f && v1.y == 2.0f && v1.z == 3.0f);
		
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v3 /= -1.0f;
		v3 /= -1.0f;
		assert(v3.x == 0.0f && v3.y == -3.0f && v3.z == -1.0f);
	
		v1 = Vec3(1.0f, 2.0f, 3.0f);
		v2 = Vec3(2.0f, 3.0f, 0.0f);
		v3 = Vec3(0.0f, -3.0f, -1.0f);
		v3 /= -1.0f;
		v3 /= 8.0f;
		assert(v3.x == 0.0f && v3.y == 0.375f && v3.z == 0.125f);

		FINISH_TEST;	
	}
	


}

int main(int argc, char** argv) {
	cc::testBasicVec3CreationAndAssignment();	
	cc::testVec3Equality();	
	cc::testVec3LengthAndUnitLength();
	cc::testVec3DistanceTo();	
	cc::testVec3DotProduct();
	cc::testVec3CrossProduct();	
	cc::testVec3VectorProjection();	
	cc::testVec3Normalise();	
	cc::testVec3Negation();
	cc::testVec3Addition();	
	cc::testVec3Subtraction();	
	cc::testVec3Multiplication();	
	cc::testVec3Division();	
	return 0;
}

