#include <assert.h>
#include <ctime>
#include <cstdio>
#include <iostream>
#include "core/math/quaternion.h"
#ifndef DEBUG
#define DEBUG 1
#endif

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)


namespace cc {

	void testBasicQuaternionCreationAndAssignment() {
		BEGIN_TEST;
		
		Quaternion q1;
		assert( q1.x == REAL(0.0) && q1.y == REAL(0.0) && q1.z == REAL(0.0) && q1.w == REAL(1.0) );
		Quaternion q2(REAL(1.0), REAL(2.0), REAL(-3.0), REAL(-1.0));
		assert( q2.x == REAL(1.0) && q2.y == REAL(2.0) && q2.z == REAL(-3.0) && q2.w == REAL(-1.0));
		Real arr[4] = {REAL(3.0), REAL(-3.1), REAL(90.3), REAL(-1.2) };
		Quaternion q3(arr);
		assert(q3.x == REAL(3.0) && q3.y == REAL(-3.1) && q3.z == REAL(90.3) && q3.w == REAL(-1.2) );
		// Rotation 45 around x-axis.
		Quaternion q4(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert(q4.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		// Euler angles for 45 Degree rotation around x-axis.
		Quaternion q5(Angle(REAL(0.0)), Angle(REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert(q5.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );

		// Orthonormal basis for 45 degree rotation around x-axis.
		Quaternion q6(
			Vec3(REAL(1.0), REAL(0.0), REAL(0.0)),
			Vec3(REAL(0.0), REAL(0.7071067811865), REAL(0.7071067811865)),
			Vec3(REAL(0.0), REAL(-0.7071067811865), REAL(0.7071067811865))
			);
		
		assert(q6.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		// Matrix for a 45 degree rotation around x-axis.
		Quaternion q7(
			Mat3(
				REAL(1.0), REAL(0.0), REAL(0.0),
				REAL(0.0), REAL(0.7071067811865), REAL(-0.7071067811865),
				REAL(0.0), REAL(0.7071067811865), REAL(0.7071067811865)
				)
			);
		assert(q7.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		assert( q4.approx(q5) && q4.approx(q6) && q4.approx(q7) && 
				  q5.approx(q6) && q5.approx(q7) &&
				  q6.approx(q7) );
		
		q7 = q1;
		assert( q7.x == REAL(0.0) && q7.y == REAL(0.0) && q7.z == REAL(0.0) && q7.w == REAL(1.0) );
		
		FINISH_TEST;
		
	}

	void testQuaternionEquality() {
		BEGIN_TEST;
		
		Quaternion q1;
		Quaternion q2;
		q2.x = REAL(0.001);
		

		assert( q1 == q1 && q2 == q2 );		
		
		assert( q1 != q2 && q2 != q1);
		q1 = q2;
		assert( q1 == q2 && q2 == q1);

		q1 = Quaternion(REAL(1.0), REAL(-1.0), REAL(-2.0), REAL(3.0));
		assert( q1 != q2 && q2 != q1 );
		assert( !(q1 == q2) && !(q2 == q1) );
		q2 = Quaternion(REAL(1.0), REAL(-1.0), REAL(-2.0), REAL(3.0));
		assert( q1 == q2 && q2 == q1 );
		assert( !(q1 != q2) && !(q2 != q1) );				
		
		assert( q1.approx(q1) && q2.approx(q2) );		
		assert( q1.approx(q2) && q2.approx(q1) );
		q2 = q1;
		q2.x += Quaternion::EPSILON;
		assert( !q1.approx(q2) && !q2.approx(q1) );
		q2 = q1;
		q2.x += (Quaternion::EPSILON / REAL(2.0));
		assert( q1.approx(q2) && q2.approx(q1) );

		FINISH_TEST;	
	}
	
	

	void testQuaternionLengthAndUnitLength() {
		BEGIN_TEST;
		
		Quaternion q1(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0));
		Quaternion q2 = Quaternion(REAL(1.001), REAL(0.001), REAL(0.001), REAL(0.001));
		
		assert( q1.isUnitLength() );
		assert( q1.length() == REAL(1.0) && q1.lengthSquared() == REAL(1.0) );

		assert( !q2.isUnitLength() );
		assert( q2.length() > REAL(1.0) && q2.lengthSquared() > REAL(1.0) );
		assert( q2.lengthSquared() > q2.length() );

		Quaternion q3 = -q2;
		assert( !q3.isUnitLength() );
		assert( q3.length() == q2.length() && q2.lengthSquared() == q2.lengthSquared() );
		
		Quaternion q4 = -q1;
		assert( q4.isUnitLength() );
		assert( q4.length() == REAL(1.0) && q4.lengthSquared() == REAL(1.0) );
		
		q3 = Quaternion(REAL(0.182574), REAL(0.365148), REAL(0.547723), REAL(0.730297));
		assert( q3.isUnitLength() );

		q4 = Quaternion(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		assert( !q4.isUnitLength() );
		assert( q4.lengthSquared() > q4.length() );

		FINISH_TEST;
	}	

	void testQuaternionDotProduct() {
		BEGIN_TEST;
		
		Quaternion q1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Quaternion q2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Quaternion q3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));

		assert( q1.dotProduct(q2) == REAL(4.0));
		assert( q2.dotProduct(q1) == REAL(4.0));

		assert( q1.dotProduct(q3) == REAL(-1.0));
		assert( q3.dotProduct(q1) == REAL(-1.0));

		assert( q2.dotProduct(q3) == REAL(-11.0));
		assert( q3.dotProduct(q2) == REAL(-11.0));

		assert( q1.dotProduct(Quaternion()) == REAL(4.0) );
		assert( q1.dotProduct(Quaternion(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0))) == REAL(10.0) );

		q1 = Quaternion(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0));
		q2 = Quaternion(REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0));
		q3 = Quaternion(REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0));
		Quaternion   q4(REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));
		
		assert( q1.dotProduct(q2) == REAL(0.0) );
		assert( q1.dotProduct(q3) == REAL(0.0) );
		assert( q1.dotProduct(q4) == REAL(0.0) );
		assert( q2.dotProduct(q3) == REAL(0.0) );
		assert( q2.dotProduct(q4) == REAL(0.0) );
		assert( q3.dotProduct(q4) == REAL(0.0) );

		// Dot product > 0 if angle < 90
		q2.x = REAL(0.1);
		assert( q1.dotProduct(q2) > REAL(0.0) );

		// Dot product < 0 if angle > 90
		q2.x = -REAL(0.1);
		assert( q1.dotProduct(q2) < REAL(0.0) );		
		
		FINISH_TEST;
	}

	void testQuaternionNormalise() {
		BEGIN_TEST;
		
		Quaternion q1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Quaternion q2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Quaternion q3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));


		Quaternion res;

		res = q1.normalised();
		assert( res.isUnitLength());
		res = q2.normalised();		
		assert( res.isUnitLength() );
		res = q3.normalised();		
		assert( res.isUnitLength() );

		assert( !q1.isUnitLength() );		
		q1.normalise();		
		assert( q1.isUnitLength() );
		assert( !q2.isUnitLength() );		
		q2.normalise();		
		assert( q2.isUnitLength() );
		assert( !q3.isUnitLength() );		
		q3.normalise();		
		assert( q3.isUnitLength() );

		q1 = Quaternion(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0));
		assert( q1.isUnitLength() && q1.length() == REAL(1.0) );
		q1.normalise();
		assert( q1.isUnitLength() && q1.length() == REAL(1.0) );

		FINISH_TEST;	
	}	

	void testQuaternionInversion() {
		BEGIN_TEST;
		
		Quaternion q1(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0));
		Quaternion q2(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		
		Quaternion res;
		
		res = q1.inverse();
		assert( res.isUnitLength() );
		assert( res.x == REAL(-1.0) && res.y == REAL(0.0) && res.z == REAL(0.0) && res.w == REAL(0.0) );

		res = q1.unitInverse();
		assert( res.length() == REAL(1.0) && res.lengthSquared() == REAL(1.0));
		assert( res.x == REAL(-1.0) && res.y == REAL(0.0) && res.z == REAL(0.0) && res.w == REAL(0.0) );

		q1.invert();
		assert( q1.length() == REAL(1.0) && q1.lengthSquared() == REAL(1.0));
		assert( q1.x == REAL(-1.0) && q1.y == REAL(0.0) && q1.z == REAL(0.0) && q1.w == REAL(0.0) );
		
		res = q2.inverse();
		assert( res.x < 0.0 && res.y < 0.0 && res.z < 0.0 && res.w > 0.0 );
		
		assert( res.approx(Quaternion(-0.0333333, -0.066666, -0.1, 0.133333)) );
		
		res = res.unitInverse();
		assert( res.approx(Quaternion(0.0333333, 0.066666, 0.1, 0.133333)) );
		
		res = -res;
		assert( res.approx(Quaternion(-0.0333333, -0.066666, -0.1, 0.133333)) );
		
		assert(q2 == Quaternion(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0)) );		
		q2.invert();
		assert( q2.x < 0.0 && q2.y < 0.0 && q2.z < 0.0 && q2.w > 0.0 );
		assert( q2.approx(Quaternion(-0.0333333, -0.066666, -0.1, 0.133333)) );
		assert( !q2.approx(Quaternion(0.0333333, 0.066666, 0.1, 0.133333)) );

		q2.invertUnit();
		assert( q2.approx(Quaternion(0.0333333, 0.066666, 0.1, 0.133333)) );		
		
		FINISH_TEST;
	}

   	void testQuaternionAddition() {
		BEGIN_TEST;

		Quaternion q1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Quaternion q2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Quaternion q3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));

		Quaternion res;

		res = q1 + (q2 + q3);		
		assert(res.x == 3.0f && res.y == 2.0f && res.z == 2.0f && res.w == 5.0f);
		res = q2 + (q3 + q1);
		assert(res.x == 3.0f && res.y == 2.0f && res.z == 2.0f && res.w == 5.0f);
		res = (q3 + q1) + q2;
		assert(res.x == 3.0f && res.y == 2.0f && res.z == 2.0f && res.w == 5.0f);

		q2 += q3;
		q1 += q2;		
		assert(q1.x == 3.0f && q1.y == 2.0f && q1.z == 2.0f && res.w == 5.0f);
	  
		q1 = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
		q2 = Quaternion(2.0f, 3.0f, 0.0f, -1.0f);
		q3 = Quaternion(0.0f, -3.0f, -1.0f, 2.0f);
	   q2 += (q3 += q1);
		assert(q2.x == 3.0f && q2.y == 2.0f && q2.z == 2.0f && q2.w == 5.0f);

		q1 = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
		q2 = Quaternion(2.0f, 3.0f, 0.0f, -1.0f);
		q3 = Quaternion(0.0f, -3.0f, -1.0f, 2.0f);
		q3 += q1;
		q3 += q2;		
		assert(q3.x == 3.0f && q3.y == 2.0f && q3.z == 2.0f && q3.w == 5.0f);
  
		FINISH_TEST;
	}
	
	void testQuaternionSubtraction() {
		BEGIN_TEST;
		
		Quaternion q1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Quaternion q2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Quaternion q3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));

		Quaternion res;
		
		res = q1 - (q2 - q3);
		assert(res.x == -1.0f && res.y == -4.0f && res.z == 2.0f && res.w == 7.0f);
		res = q2 - (q3 - q1);
		assert(res.x == 3.0f && res.y == 8.0f && res.z == 4.0f && res.w == 1.0f);
		res = (q3 - q1) - q2;
		assert(res.x == -3.0f && res.y == -8.0f && res.z == -4.0f && res.w == -1.0f);

		q1 -= (q2 -= q3);
		assert(q1.x == -1.0f && q1.y == -4.0f && q1.z == 2.0f && q1.w == 7.0f);
	   
	   q1 = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
		q2 = Quaternion(2.0f, 3.0f, 0.0f, -1.0f);
		q3 = Quaternion(0.0f, -3.0f, -1.0f, 2.0f);
		q2 -= (q3 -= q1);		
		assert(q2.x == 3.0f && q2.y == 8.0f && q2.z == 4.0f && q2.w == 1.0f);
	   
		q1 = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
		q2 = Quaternion(2.0f, 3.0f, 0.0f, -1.0f);
		q3 = Quaternion(0.0f, -3.0f, -1.0f, 2.0f);
		q3 -= q1;
		q3 -= q2;		
		assert(q3.x == -3.0f && q3.y == -8.0f && q3.z == -4.0f && q3.w == -1.0f);
   	
		FINISH_TEST;
	}

	void testQuaternionMultiplication() {
		BEGIN_TEST;
		
		Quaternion q01(1.0f, 2.0f, 3.0f, 1.0f);
		Quaternion q02(2.0f, 3.0f, 0.0f, -3.0f);
		Quaternion q03(0.0, -3.0, -1.0f, 0.0f);

		Quaternion res = q01*q02;
		assert (res.x == -10.0f && res.y == 3.0f && res.z == -10.0f && res.w == -11.0f);
		res = q02*q01;
		assert (res.x == 8.0f && res.y == -9.0f && res.z == -8.0f && res.w == -11.0f);
		res = q03 * q02;
		assert (res.x == 3.0f && res.y == 7.0f && res.z == 9.0f && res.w == 9.0f);

		q01 *= q02;
		assert (q01.x == -10.0f && q01.y == 3.0f && q01.z == -10.0f && q01.w == -11.0f);
		q03 *= q02;
		assert (q03.x == 3.0f && q03.y == 7.0f && q03.z == 9.0f && q03.w == 9.0f);

		Quaternion q4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		res = q4 * Quaternion(REAL(4.0), REAL(-7.0), REAL(1.0), REAL(8.0));

		assert( res.x == REAL(47.0) && res.y == REAL(-1.0) && res.z == REAL(13.0) && res.w == REAL(39.0) );
	   
		q4 *= Quaternion(REAL(4.0), REAL(-7.0), REAL(1.0), REAL(8.0));
		assert( q4.x == REAL(47.0) && q4.y == REAL(-1.0) && q4.z == REAL(13.0) && q4.w == REAL(39.0) );

		// Rotation 45 degrees around x axis.
		q4 = Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert(q4.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		res = q4 * q4;  // 90 Degree rotation around x axis.
		assert( res.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		res *= res;
		res *= res;
		assert( res.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around y axis.
		q4 = Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert(q4.approx(Quaternion(REAL(0.0), REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );
		res = q4 * q4;  // 90 Degree rotation around x axis.
		assert( res.approx(Quaternion(REAL(0.0), REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655))) );
		res *= res;
		res *= res;
		assert( res.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around z axis.
		q4 = Quaternion(Vec3(REAL(0.0), REAL(0.0), REAL(1.0)), Angle::fromDegrees(REAL(45.0)));
		assert(q4.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.38268343236509), REAL(0.923879532511287))) );
		res = q4 * q4;  // 90 Degree rotation around z axis.
		assert( res.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655))) );
		res *= res;
		res *= res;
		assert( res.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0

		// Scalar multiplication tests
		Quaternion q1(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0));
		Quaternion q2(REAL(2.0), REAL(3.0), REAL(0.0), REAL(-1.0));
		Quaternion q3(REAL(0.0), REAL(-3.0), REAL(-1.0), REAL(2.0));

		res = q1 * 1.0f;
		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f && res.w == 4.0f);
		res = 1.0f * q1;		
		assert(res.x == 1.0f && res.y == 2.0f && res.z == 3.0f && res.w == 4.0f);
		
		res = (q3 * -1.0f) * -1.0f;
		assert(res.x == 0.0f && res.y == -3.0f && res.z == -1.0f && res.w == 2.0f);
		res = (q3 * -1.0f) * 1.5f;
		assert(res.x == 0.0f && res.y == 4.5f && res.z == 1.5f && res.w == -3.0f);

		res = -1.0f * (-1.0f * q3);	  
		assert(res.x == 0.0f && res.y == -3.0f && res.z == -1.0f && res.w == 2.0f);
		res = 1.5f * (-1.0f * q3);		
		assert(res.x == 0.0f && res.y == 4.5f && res.z == 1.5f && res.w == -3.0f);

		q1 *= 1.0f;
		assert(q1.x == 1.0f && q1.y == 2.0f && q1.z == 3.0f && q1.w == 4.0f);
		
		q1 = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
		q2 = Quaternion(2.0f, 3.0f, 0.0f, -1.0f);
		q3 = Quaternion(0.0f, -3.0f, -1.0f, 2.0f);
		q3 *= -1.0f;
		q3 *= -1.0f;
		assert(q3.x == 0.0f && q3.y == -3.0f && q3.z == -1.0f && q3.w == 2.0f);

		q1 = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
		q2 = Quaternion(2.0f, 3.0f, 0.0f, -1.0f);
		q3 = Quaternion(0.0f, -3.0f, -1.0f, 2.0f);
		q3 *= -1.0f;
		q3 *= 1.5f;
		assert(q3.x == 0.0f && q3.y == 4.5f && q3.z == 1.5f && q3.w == -3.0f);

		FINISH_TEST;
	}

	void testQuaternionVectorMultiplicationAndRotation() {
		BEGIN_TEST;

		Quaternion q;		
		Vec3 v(REAL(1.0), REAL(1.0), REAL(1.0));		
		Vec3 vr;
		
		
		// Rotation 45 degrees around x axis.
		q = Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		vr = q * v;
		assert( vr.approx(Vec3(1.0, 0.0, 1.414214)) );
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert( vr.approx(Vec3(1.0, 0.0, 1.414214)) );
		// 90 Degree rotation around x axis.
		q = q*q;  
		assert( q.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		vr = q * v;
		assert(vr.approx(Vec3(1.0, -1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert( vr.approx(Vec3(1.0, -1.0, 1.0)) );
		// 360 Degree rotation around x axis.
		q *= q;
		q *= q;
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		vr = q * v;
		assert(vr.approx(Vec3(1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert( vr.approx(Vec3(1.0, 1.0, 1.0)) );
		// Rotate 45 again (Even though axis is inverted, should be same?)
		q *= Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		vr = q* v;		
		assert( vr.approx(Vec3(1.0, 0.0, 1.414214)) );
		
		// Rotation 45 degrees around y axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );
		vr = q * v;
		assert( vr.approx(Vec3(1.414214, 1.0, 0.0)) );
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert( vr.approx(Vec3(1.414214, 1.0, 0.0)) );
		// 90 Degree rotation around y axis.
		q = q*q;  
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655))) );
		vr = q * v;
		assert(vr.approx(Vec3(1.0, 1.0, -1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert( vr.approx(Vec3(1.0, 1.0, -1.0)) );
		// 360 Degree rotation around y axis.
		q *= q;
		q *= q;
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		vr = q * v;
		assert(vr.approx(Vec3(1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert( vr.approx(Vec3(1.0, 1.0, 1.0)) );
		// Rotate 45 again (Even though axis is inverted, should be same?)
		q *= Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		vr = q* v;		
	   assert( vr.approx(Vec3(1.414214, 1.0, 0.0)) );
	
		// Rotation 45 degrees around z axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(0.0), REAL(1.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.38268343236509), REAL(0.923879532511287))) );
		vr = q * v;
		assert( vr.approx(Vec3(0.0, 1.414214, 1.0)) );
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
	   assert( vr.approx(Vec3(0.0, 1.414214, 1.0)) );
		// 90 Degree rotation around z axis.
		q = q*q;  
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655))) );
		vr = q * v;
		assert(vr.approx(Vec3(-1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert( vr.approx(Vec3(-1.0, 1.0, 1.0)) );
		// 360 Degree rotation around z axis.
		q *= q;
		q *= q;
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		vr = q * v;
		assert(vr.approx(Vec3(1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert( vr.approx(Vec3(1.0, 1.0, 1.0)) );
		// Rotate 45 again (Even though axis is inverted, should be same?)
		q *= Quaternion(Vec3(REAL(0.0), REAL(0.0), REAL(1.0)), Angle::fromDegrees(REAL(45.0)));
		vr = q* v;		
	   assert( vr.approx(Vec3(0.0, 1.414214, 1.0)) );
	

		// Rotation 45 degrees around x, y and z axis'. (1i + 2k + 3j)
		q = Quaternion(Vec3(REAL(1.0), REAL(2.0), REAL(3.0)), 
							Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.102276449393), REAL(0.204552898786), REAL(0.30682934818), REAL(0.923879532511287))) );
		vr = q * v;
		assert( vr.approx(Vec3(0.64365, 1.336123, 0.894702)) );
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
	   assert( vr.approx(Vec3(0.64365, 1.336123, 0.894702)) );
		// 90 Degree rotation around xyz axis'.
		q = q*q;  
		assert( q.approx(Quaternion(REAL(0.18898223650), REAL(0.37796447301), REAL(0.56694670951), REAL(0.70710678118655))) );
		vr = q * v;
		assert(vr.approx(Vec3(0.16131, 1.391665, 1.018453)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert(vr.approx(Vec3(0.16131, 1.391665, 1.018453)) );
		// 360 Degree rotation around xyz axis'.
		q *= q;
		q *= q;
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		vr = q * v;
		assert(vr.approx(Vec3(1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		q.rotate(vr);
		assert( vr.approx(Vec3(1.0, 1.0, 1.0)) );
		// Rotate 45 again (Even though axis is inverted, should be same?)
		q *= Quaternion(Vec3(REAL(1.0), REAL(2.0), REAL(3.0)), Angle::fromDegrees(REAL(45.0)));
		vr = q* v;		
	   assert( vr.approx(Vec3(0.64365, 1.336123, 0.894702)) );

		FINISH_TEST;
	}

	void testQuaternionFromAxisAngle() {
		BEGIN_TEST;
		
		Quaternion q;				
		
		// Rotation 45 degrees around x axis.
		q = Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		// 90 Degree rotation around x axis.
	   q = Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		// 360 Degree rotation around x axis.
	   q = Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		
		// Rotation 45 degrees around y axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );
		// 90 Degree rotation around y axis.
	   q = Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655))) );
		// 360 Degree rotation around y axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around z axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(0.0), REAL(1.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.38268343236509), REAL(0.923879532511287))) );	
		// 90 Degree rotation around z axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(0.0), REAL(1.0)), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655))) );		
		// 360 Degree rotation around z axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(0.0), REAL(1.0)), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around x, y and z axis'. (1i + 2k + 3j)
		q = Quaternion(Vec3(REAL(1.0), REAL(2.0), REAL(3.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.102276449393), REAL(0.204552898786), REAL(0.30682934818), REAL(0.923879532511287))) );	
		// 90 Degree rotation around xyz axis'.
		q = Quaternion(Vec3(REAL(1.0), REAL(2.0), REAL(3.0)), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.18898223650), REAL(0.37796447301), REAL(0.56694670951), REAL(0.70710678118655))) );	
		// 360 Degree rotation around xyz axis'.
	   q = Quaternion(Vec3(REAL(1.0), REAL(2.0), REAL(3.0)), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		
		FINISH_TEST;
	}

	void testQuaternionToAxisAngle() {
		BEGIN_TEST;
		
		Quaternion q;
		Vec3 axis;
		Angle angle;
		
		
		// Rotation 45 degrees around x axis.
		q = Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(1.0, 0.0, 0.0)) );
		assert( approx(angle.degrees(), 45.0) );		
		axis = Vec3();
		angle = Angle();	
	
		// 90 Degree rotation around x axis.
	   q = Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(1.0, 0.0, 0.0)) );
		assert( approx(angle.degrees(), 90.0) );
		axis = Vec3();
		angle = Angle();	

		// 360 Degree rotation around x axis.
	   q = Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(-1.0, 0.0, 0.0)) );
		assert( approx(angle.degrees(), 360.0) );
		axis = Vec3();
		angle = Angle();	

		// Rotation 45 degrees around y axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(0.0, 1.0, 0.0)) );
		assert( approx(angle.degrees(), 45.0) );
		axis = Vec3();
		angle = Angle();	

		// 90 Degree rotation around y axis.
	   q = Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655))) );
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(0.0, 1.0, 0.0)) );
		assert( approx(angle.degrees(), 90.0) );
		axis = Vec3();
		angle = Angle();	

		// 360 Degree rotation around y axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(0.0, -1.0, 0.0)) );
		assert( approx(angle.degrees(), 360.0) );
		axis = Vec3();
		angle = Angle();	

		// Rotation 45 degrees around z axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(0.0), REAL(1.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.38268343236509), REAL(0.923879532511287))) );
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(0.0, 0.0, 1.0)) );
		assert( approx(angle.degrees(), 45.0) );
		axis = Vec3();
		angle = Angle();	
	
		// 90 Degree rotation around z axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(0.0), REAL(1.0)), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655))) );
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(0.0, 0.0, 1.0)) );
		assert( approx(angle.degrees(), 90.0) );	
		axis = Vec3();
		angle = Angle();	
	
		// 360 Degree rotation around z axis.
		q = Quaternion(Vec3(REAL(0.0), REAL(0.0), REAL(1.0)), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(0.0, 0.0, -1.0)) );
		assert( approx(angle.degrees(), 360.0) );
		axis = Vec3();
		angle = Angle();	

		// Rotation 45 degrees around x, y and z axis'. (1i + 2k + 3j)
		q = Quaternion(Vec3(REAL(1.0), REAL(2.0), REAL(3.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.102276449393), REAL(0.204552898786), REAL(0.30682934818), REAL(0.923879532511287))) );	
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(0.267261241913, 0.534522483825, 0.801783725738)) );
		assert( approx(angle.degrees(), 45.0) );
		axis = Vec3();
		angle = Angle();	

		// 90 Degree rotation around xyz axis'.
		q = Quaternion(Vec3(REAL(1.0), REAL(2.0), REAL(3.0)), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.18898223650), REAL(0.37796447301), REAL(0.56694670951), REAL(0.70710678118655))) );	
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(0.267261241913, 0.534522483825, 0.801783725738)) );
		assert( approx(angle.degrees(), 90.0) );
		axis = Vec3();
		angle = Angle();	

		// 360 Degree rotation around xyz axis'.
	   q = Quaternion(Vec3(REAL(1.0), REAL(2.0), REAL(3.0)), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		q.toAxisAngle(axis, angle);
		assert( axis.approx(Vec3(-0.267261241913, -0.534522483825, -0.801783725738)) );
		assert( approx(angle.degrees(), 360.0) );
		axis = Vec3();
		angle = Angle();	
		

		FINISH_TEST;	
	}

	void testQuaternionFromEulerAngles() {
		BEGIN_TEST;
		
		Quaternion q;
		
		// Rotation 45 degrees around x axis.
		q = Quaternion(Angle(0.0), Angle(0.0), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		// 90 Degree rotation around x axis.
		q = Quaternion(Angle(0.0), Angle(0.0), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		// 360 Degree rotation around x axis.
		q = Quaternion(Angle(0.0), Angle(0.0), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		
		// Rotation 45 degrees around y axis.
		q = Quaternion(Angle::fromDegrees(REAL(45.0)), Angle(0.0), Angle(0.0));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );
		// 90 Degree rotation around y axis.
	   q = Quaternion(Angle::fromDegrees(REAL(90.0)), Angle(0.0), Angle(0.0));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655))) );
		// 360 Degree rotation around y axis.
		q = Quaternion(Angle::fromDegrees(REAL(360.0)), Angle(0.0), Angle(0.0));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around z axis.
		q = Quaternion(Angle(0.0), Angle::fromDegrees(REAL(45.0)), Angle(0.0));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.38268343236509), REAL(0.923879532511287))) );	
		// 90 Degree rotation around z axis.
		q = Quaternion(Angle(0.0), Angle::fromDegrees(REAL(90.0)), Angle(0.0));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655))) );		
		// 360 Degree rotation around z axis.
		q = Quaternion(Angle(0.0), Angle::fromDegrees(REAL(360.0)), Angle(0.0));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around x, y and z axis'. (1i + 2k + 3j)
	   q = Quaternion(Angle::fromDegrees(REAL(45.0)), Angle::fromDegrees(REAL(45.0)), Angle::fromDegrees(REAL(45.0)));
		assert( q.approx(Quaternion(REAL(0.4619397662556), REAL(0.1913417161825), REAL(0.4619397662556), REAL(0.7325378163287))) );	
		// 90 Degree rotation around xyz axis'.
	   q = Quaternion(Angle::fromDegrees(REAL(90.0)), Angle::fromDegrees(REAL(90.0)), Angle::fromDegrees(REAL(90.0)));
		assert( q.approx(Quaternion(REAL(0.7071067811866), REAL(0.0), REAL(0.7071067811866), REAL(0.0))) );	
		// 360 Degree rotation around xyz axis'.
	   q = Quaternion(Angle::fromDegrees(REAL(360.0)), Angle::fromDegrees(REAL(360.0)), Angle::fromDegrees(REAL(360.0)));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, -1.0)) ); // Should have rotated all the way back to 0
		
		
		FINISH_TEST;
	}

	void testQuaternionFromLocalAxes() {
		BEGIN_TEST;
		
		Quaternion q;				
		
		// Rotation 45 degrees around x axis.
		q = Quaternion(Vec3(1, 0, 0), Vec3(0, 0.7071067811865, 0.7071067811865), Vec3(0, -0.7071067811865, 0.7071067811865));
		assert( q.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		// 90 Degree rotation around x axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 0, 1), Vec3(0, -1, 0));
		assert( q.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		// 360 Degree rotation around x axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		
		// Rotation 45 degrees around y axis.
		q = Quaternion(Vec3(0.7071067811865, 0, -0.7071067811865), Vec3(0, 1, 0), Vec3(0.7071067811865, 0, 0.7071067811865));
		assert( q.approx(Quaternion(REAL(0.0),REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );
		// 90 Degree rotation around y axis.
	   q = Quaternion(Vec3(0, 0, -1), Vec3(0, 1, 0), Vec3(1, 0, 0));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.70710678118655),REAL(0.0), REAL(0.70710678118655))) );
		// 360 Degree rotation around y axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around z axis.
			q = Quaternion(Vec3(0.7071067811865, 0.7071067811865, 0), Vec3(-0.7071067811865, 0.7071067811865, 0), Vec3(-0, 0, 1));
		assert( q.approx(Quaternion( REAL(0.0), REAL(0.0), REAL(0.38268343236509),REAL(0.923879532511287))) );
		// 90 Degree rotation around z axis.
	   q = Quaternion(Vec3(0, 1, 0), Vec3(-1, 0, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion( REAL(0.0), REAL(0.0), REAL(0.70710678118655),REAL(0.70710678118655))) );
		// 360 Degree rotation around z axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around xyz axis'.
		q = Quaternion(Vec3(0.8047378541, 0.5058793634, -0.3106172175),
							Vec3(-0.3106172175, 0.8047378541, 0.5058793634),
							Vec3(0.5058793634, -0.3106172175, 0.8047378541));

		assert( q.approx(Quaternion(REAL(0.22094238269), REAL(0.22094238269), REAL(0.22094238269), REAL(0.923879532511))) );
		// 90 Degree rotation around xyz axis'.
		q = Quaternion(Vec3(0.33333333333, 0.91068360252, -0.24401693586),
							Vec3(-0.24401693586, 0.33333333333, 0.91068360252),
							Vec3(0.91068360252, -0.24401693586, 0.33333333333));
		assert( q.approx(Quaternion(REAL(0.408248290464), REAL(0.408248290464), REAL(0.408248290464), REAL(0.707106781187))) );
		// 360 Degree rotation around xyz axis'.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0

		FINISH_TEST;
	}
	

	void testQuaternionToLocalAxes() {
		BEGIN_TEST;
		
		Quaternion q;
		Vec3 xaxis;
		Vec3 yaxis;
		Vec3 zaxis;
		
		
		// Rotation 45 degrees around x axis.
		q = Quaternion(Vec3(1, 0, 0), Vec3(0, 0.7071067811865, 0.7071067811865), Vec3(0, -0.7071067811865, 0.7071067811865));
		assert( q.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(1, 0, 0)) );
		assert( yaxis.approx(Vec3(0, 0.7071067811865, 0.7071067811865)) );
		assert( zaxis.approx(Vec3(0, -0.7071067811865, 0.7071067811865)) );
		
		
		// 90 Degree rotation around x axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 0, 1), Vec3(0, -1, 0));
		assert( q.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(1, 0, 0)) );
		assert( yaxis.approx(Vec3(0, 0, 1)) );
		assert( zaxis.approx(Vec3(0, -1, 0)) );

		// 360 Degree rotation around x axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(1, 0, 0)) );
		assert( yaxis.approx(Vec3(0, 1, 0)) );
		assert( zaxis.approx(Vec3(0, 0, 1)) );
		
		// Rotation 45 degrees around y axis.
		q = Quaternion(Vec3(0.7071067811865, 0, -0.7071067811865), Vec3(0, 1, 0), Vec3(0.7071067811865, 0, 0.7071067811865));
		assert( q.approx(Quaternion(REAL(0.0),REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(0.7071067811865, 0, -0.7071067811865)) );
		assert( yaxis.approx(Vec3(0, 1, 0)) );
		assert( zaxis.approx(Vec3(0.7071067811865, 0, 0.7071067811865)) );

		// 90 Degree rotation around y axis.
	   q = Quaternion(Vec3(0, 0, -1), Vec3(0, 1, 0), Vec3(1, 0, 0));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.70710678118655),REAL(0.0), REAL(0.70710678118655))) );
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(0, 0, -1)) );
		assert( yaxis.approx(Vec3(0, 1, 0)) );
		assert( zaxis.approx(Vec3(1, 0, 0)) );

		// 360 Degree rotation around y axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(1, 0, 0)) );
		assert( yaxis.approx(Vec3(0, 1, 0)) );
		assert( zaxis.approx(Vec3(0, 0, 1)) );

		// Rotation 45 degrees around z axis.
			q = Quaternion(Vec3(0.7071067811865, 0.7071067811865, 0), Vec3(-0.7071067811865, 0.7071067811865, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion( REAL(0.0), REAL(0.0), REAL(0.38268343236509),REAL(0.923879532511287))) );
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(0.7071067811865, 0.7071067811865, 0)) );
		assert( yaxis.approx(Vec3(-0.7071067811865, 0.7071067811865, 0)) );
		assert( zaxis.approx(Vec3(0, 0, 1)) );

		// 90 Degree rotation around z axis.
	   q = Quaternion(Vec3(0, 1, 0), Vec3(-1, 0, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion( REAL(0.0), REAL(0.0), REAL(0.70710678118655),REAL(0.70710678118655))) );
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(0, 1, 0)) );
		assert( yaxis.approx(Vec3(-1, 0, 0)) );
		assert( zaxis.approx(Vec3(0, 0, 1)) );

		// 360 Degree rotation around z axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(1, 0, 0)) );
		assert( yaxis.approx(Vec3(0, 1, 0)) );
		assert( zaxis.approx(Vec3(0, 0, 1)) );

		// Rotation 45 degrees around xyz axis'.
		q = Quaternion(Vec3(0.8047378541, 0.5058793634, -0.3106172175),
							Vec3(-0.3106172175, 0.8047378541, 0.5058793634),
							Vec3(0.5058793634, -0.3106172175, 0.8047378541));

		assert( q.approx(Quaternion(REAL(0.22094238269), REAL(0.22094238269), REAL(0.22094238269), REAL(0.923879532511))) );
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(0.8047378541, 0.5058793634, -0.3106172175)) );
		assert( yaxis.approx(Vec3(-0.3106172175, 0.8047378541, 0.5058793634)) );
		assert( zaxis.approx(Vec3(0.5058793634, -0.3106172175, 0.8047378541)) );

		// 90 Degree rotation around xyz axis'.
		q = Quaternion(Vec3(0.33333333333, 0.91068360252, -0.24401693586),
							Vec3(-0.24401693586, 0.33333333333, 0.91068360252),
							Vec3(0.91068360252, -0.24401693586, 0.33333333333));
		assert( q.approx(Quaternion(REAL(0.408248290464), REAL(0.408248290464), REAL(0.408248290464), REAL(0.707106781187))) );
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(0.33333333333, 0.91068360252, -0.24401693586)) );
		assert( yaxis.approx(Vec3(-0.24401693586, 0.33333333333, 0.91068360252)) );
		assert( zaxis.approx(Vec3(0.91068360252, -0.24401693586, 0.33333333333)) );

		// 360 Degree rotation around xyz axis'.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		q.toLocalAxes(xaxis, yaxis, zaxis);
		assert( xaxis.approx(Vec3(1, 0, 0)) );
		assert( yaxis.approx(Vec3(0, 1, 0)) );
		assert( zaxis.approx(Vec3(0, 0, 1)) );

		FINISH_TEST;
	}
	
	void testQuaternionFromMatrix() {
		BEGIN_TEST;
		
		Quaternion q;				
		
		// Rotation 45 degrees around x axis.
		q = Quaternion(
			Mat3(
				1.0, 0.0, 0.0,
				0.0, 0.70710678118655, -0.70710678118655,
				0.0, 0.70710678118655, 0.70710678118655
				));			
		assert( q.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		// 90 Degree rotation around x axis.
	  q = Quaternion(
			Mat3(
				1.0, 0.0, 0.0,
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0
				));	
		assert( q.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		// 360 Degree rotation around x axis.
	   q = Quaternion(
			Mat3(
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0
				));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		
		// Rotation 45 degrees around y axis.
	   q = Quaternion(
			Mat3(
				0.70710678118655, 0.0, 0.70710678118655,
				0.0, 1.0, 0.0,
				-0.70710678118655, 0.0, 0.70710678118655
				));	
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );
		// 90 Degree rotation around y axis.
	   q = Quaternion(
			Mat3(
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				-1.0, 0.0, 0.0
				));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655))) );
		// 360 Degree rotation around y axis.
	    q = Quaternion(
			Mat3(
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0
				));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around z axis.
	   q = Quaternion(
			Mat3(
				0.70710678118655, -0.70710678118655, 0.0,
				0.70710678118655, 0.70710678118655, 0.0,
				0.0, 0.0, 1.0
				));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.38268343236509), REAL(0.923879532511287))) );	
		// 90 Degree rotation around z axis.
		 q = Quaternion(
			Mat3(
				0.0, -1.0, 0.0,
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0
				));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655))) );		
		// 360 Degree rotation around z axis.
		q = Quaternion(
			Mat3(
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0
				));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0

		// Rotation 45 degrees around xyz axis'.
		q = Quaternion(
			Mat3 (
				0.8047378541, -0.3106172175, 0.5058793634, 
				0.5058793634, 0.8047378541, -0.3106172175, 
				-0.3106172175, 0.5058793634, 0.8047378541
				));
		
		assert( q.approx(Quaternion(REAL(0.22094238269), REAL(0.22094238269), REAL(0.22094238269), REAL(0.923879532511))) );
		// 90 Degree rotation around xyz axis'
		q = Quaternion(
			Mat3 (
				0.33333333333, -0.24401693586, 0.91068360252, 
				0.91068360252, 0.33333333333, -0.24401693586, 
				-0.24401693586, 0.91068360252, 0.33333333333
				));
		assert( q.approx(Quaternion(REAL(0.408248290464), REAL(0.408248290464), REAL(0.408248290464), REAL(0.707106781187))) );
		// 360 Degree rotation around xyz axis'.
	   q = Quaternion(
			Mat3(
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0
				));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		
		FINISH_TEST;
	}

	void testQuaternionToMatrix() {
		BEGIN_TEST;
		
		Quaternion q;	
		Mat3 m;
		
		
		// Rotation 45 degrees around x axis.
		q = Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287));
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  1.0, 0.0, 0.0,
								  0.0, 0.70710678118655, -0.70710678118655,
								  0.0, 0.70710678118655, 0.70710678118655
								  )) );	
		
		// 90 Degree rotation around x axis.
		q = Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655));
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  1.0, 0.0, 0.0,
								  0.0, 0.0, -1.0,
								  0.0, 1.0, 0.0
								  )) );			
		
		// 360 Degree rotation around x axis.
		q = Quaternion(0.0, 0.0, 0.0, -1.0);
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  1.0, 0.0, 0.0,
								  0.0, 1.0, 0.0,
								  0.0, 0.0, 1.0
								  )) );			

		// Rotation 45 degrees around y axis.
		q = Quaternion(REAL(0.0), REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287));
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  0.70710678118655, 0.0, 0.70710678118655,
								  0.0, 1.0, 0.0,
								  -0.70710678118655, 0.0, 0.70710678118655
								  )) );	

		// 90 Degree rotation around y axis.
		q = Quaternion(REAL(0.0), REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655));
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  0.0, 0.0, 1.0,
								  0.0, 1.0, 0.0,
								  -1.0, 0.0, 0.0
								  )) );	
 
		// 360 Degree rotation around y axis.
		q = Quaternion(0.0, 0.0, 0.0, -1.0);
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  1.0, 0.0, 0.0,
								  0.0, 1.0, 0.0,
								  0.0, 0.0, 1.0
								  )) );		

		// Rotation 45 degrees around z axis.
		q = Quaternion(REAL(0.0), REAL(0.0), REAL(0.38268343236509), REAL(0.923879532511287));
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  0.70710678118655, -0.70710678118655, 0.0,
								  0.70710678118655, 0.70710678118655, 0.0,
								  0.0, 0.0, 1.0
								  )) );	
	 	
		// 90 Degree rotation around z axis.
		q = Quaternion(REAL(0.0), REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655));
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  0.0, -1.0, 0.0,
								  1.0, 0.0, 0.0,
								  0.0, 0.0, 1.0
								  )) );	
			
		// 360 Degree rotation around z axis.
		q = Quaternion(0.0, 0.0, 0.0, -1.0);
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  1.0, 0.0, 0.0,
								  0.0, 1.0, 0.0,
								  0.0, 0.0, 1.0
								  )) );	

		// Rotation 45 degrees around xyz axis'.
		q = Quaternion(REAL(0.22094238269), REAL(0.22094238269), REAL(0.22094238269), REAL(0.923879532511));
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  0.8047378541, -0.3106172175, 0.5058793634, 
								  0.5058793634, 0.8047378541, -0.3106172175, 
								  -0.3106172175, 0.5058793634, 0.8047378541
								  )) );	
		
		// 90 Degree rotation around xyz axis'
		q = Quaternion(REAL(0.408248290464), REAL(0.408248290464), REAL(0.408248290464), REAL(0.707106781187));
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  0.33333333333, -0.24401693586, 0.91068360252, 
								  0.91068360252, 0.33333333333, -0.24401693586, 
								  -0.24401693586, 0.91068360252, 0.33333333333
								  )) );	
  
		// 360 Degree rotation around xyz axis'.
		q = Quaternion(0.0, 0.0, 0.0, -1.0);
		q.toMatrix(m);
		assert( m.approx(Mat3(
								  1.0, 0.0, 0.0,
								  0.0, 1.0, 0.0,
								  0.0, 0.0, 1.0
								  )) );	
		
		FINISH_TEST;
	}

	void testQuaternionXYZAxes() {
		BEGIN_TEST;
		
		Quaternion q;		
		
		// Rotation 45 degrees around x axis.
		q = Quaternion(Vec3(1, 0, 0), Vec3(0, 0.7071067811865, 0.7071067811865), Vec3(0, -0.7071067811865, 0.7071067811865));
		assert( q.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );
		assert( q.xAxis().approx(Vec3(1, 0, 0)) );
		assert( q.yAxis().approx(Vec3(0, 0.7071067811865, 0.7071067811865)) );
		assert( q.zAxis().approx(Vec3(0, -0.7071067811865, 0.7071067811865)) );
		
		
		// 90 Degree rotation around x axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 0, 1), Vec3(0, -1, 0));
		assert( q.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		assert( q.xAxis().approx(Vec3(1, 0, 0)) );
		assert( q.yAxis().approx(Vec3(0, 0, 1)) );
		assert( q.zAxis().approx(Vec3(0, -1, 0)) );

		// 360 Degree rotation around x axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		assert( q.xAxis().approx(Vec3(1, 0, 0)) );
		assert( q.yAxis().approx(Vec3(0, 1, 0)) );
		assert( q.zAxis().approx(Vec3(0, 0, 1)) );
		
		// Rotation 45 degrees around y axis.
		q = Quaternion(Vec3(0.7071067811865, 0, -0.7071067811865), Vec3(0, 1, 0), Vec3(0.7071067811865, 0, 0.7071067811865));
		assert( q.approx(Quaternion(REAL(0.0),REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );
		assert( q.xAxis().approx(Vec3(0.7071067811865, 0, -0.7071067811865)) );
		assert( q.yAxis().approx(Vec3(0, 1, 0)) );
		assert( q.zAxis().approx(Vec3(0.7071067811865, 0, 0.7071067811865)) );

		// 90 Degree rotation around y axis.
	   q = Quaternion(Vec3(0, 0, -1), Vec3(0, 1, 0), Vec3(1, 0, 0));
		assert( q.approx(Quaternion(REAL(0.0), REAL(0.70710678118655),REAL(0.0), REAL(0.70710678118655))) );
		assert( q.xAxis().approx(Vec3(0, 0, -1)) );
		assert( q.yAxis().approx(Vec3(0, 1, 0)) );
		assert( q.zAxis().approx(Vec3(1, 0, 0)) );

		// 360 Degree rotation around y axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		assert( q.xAxis().approx(Vec3(1, 0, 0)) );
		assert( q.yAxis().approx(Vec3(0, 1, 0)) );
		assert( q.zAxis().approx(Vec3(0, 0, 1)) );

		// Rotation 45 degrees around z axis.
			q = Quaternion(Vec3(0.7071067811865, 0.7071067811865, 0), Vec3(-0.7071067811865, 0.7071067811865, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion( REAL(0.0), REAL(0.0), REAL(0.38268343236509),REAL(0.923879532511287))) );
		assert( q.xAxis().approx(Vec3(0.7071067811865, 0.7071067811865, 0)) );
		assert( q.yAxis().approx(Vec3(-0.7071067811865, 0.7071067811865, 0)) );
		assert( q.zAxis().approx(Vec3(0, 0, 1)) );

		// 90 Degree rotation around z axis.
	   q = Quaternion(Vec3(0, 1, 0), Vec3(-1, 0, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion( REAL(0.0), REAL(0.0), REAL(0.70710678118655),REAL(0.70710678118655))) );
		assert( q.xAxis().approx(Vec3(0, 1, 0)) );
		assert( q.yAxis().approx(Vec3(-1, 0, 0)) );
		assert( q.zAxis().approx(Vec3(0, 0, 1)) );

		// 360 Degree rotation around z axis.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		assert( q.xAxis().approx(Vec3(1, 0, 0)) );
		assert( q.yAxis().approx(Vec3(0, 1, 0)) );
		assert( q.zAxis().approx(Vec3(0, 0, 1)) );

		// Rotation 45 degrees around xyz axis'.
		q = Quaternion(Vec3(0.8047378541, 0.5058793634, -0.3106172175),
							Vec3(-0.3106172175, 0.8047378541, 0.5058793634),
							Vec3(0.5058793634, -0.3106172175, 0.8047378541));

		assert( q.approx(Quaternion(REAL(0.22094238269), REAL(0.22094238269), REAL(0.22094238269), REAL(0.923879532511))) );
		assert( q.xAxis().approx(Vec3(0.8047378541, 0.5058793634, -0.3106172175)) );
		assert( q.yAxis().approx(Vec3(-0.3106172175, 0.8047378541, 0.5058793634)) );
		assert( q.zAxis().approx(Vec3(0.5058793634, -0.3106172175, 0.8047378541)) );

		// 90 Degree rotation around xyz axis'.
		q = Quaternion(Vec3(0.33333333333, 0.91068360252, -0.24401693586),
							Vec3(-0.24401693586, 0.33333333333, 0.91068360252),
							Vec3(0.91068360252, -0.24401693586, 0.33333333333));
		assert( q.approx(Quaternion(REAL(0.408248290464), REAL(0.408248290464), REAL(0.408248290464), REAL(0.707106781187))) );
		assert( q.xAxis().approx(Vec3(0.33333333333, 0.91068360252, -0.24401693586)) );
		assert( q.yAxis().approx(Vec3(-0.24401693586, 0.33333333333, 0.91068360252)) );
		assert( q.zAxis().approx(Vec3(0.91068360252, -0.24401693586, 0.33333333333)) );

		// 360 Degree rotation around xyz axis'.
	   q = Quaternion(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) ); // Should have rotated all the way back to 0
		assert( q.xAxis().approx(Vec3(1, 0, 0)) );
		assert( q.yAxis().approx(Vec3(0, 1, 0)) );
		assert( q.zAxis().approx(Vec3(0, 0, 1)) );

		FINISH_TEST;
	}

}

int main(int argc, char** argv) {
	cc::testBasicQuaternionCreationAndAssignment();	
	cc::testQuaternionEquality();
	cc::testQuaternionLengthAndUnitLength();
	cc::testQuaternionDotProduct();	
	cc::testQuaternionNormalise();
	cc::testQuaternionInversion();
	cc::testQuaternionAddition();
	cc::testQuaternionSubtraction();
	cc::testQuaternionMultiplication();
	cc::testQuaternionVectorMultiplicationAndRotation();
	cc::testQuaternionFromAxisAngle();
	cc::testQuaternionToAxisAngle();
	cc::testQuaternionFromEulerAngles();
	cc::testQuaternionFromLocalAxes();
	cc::testQuaternionToLocalAxes();
	cc::testQuaternionFromMatrix();
	cc::testQuaternionToMatrix();
	cc::testQuaternionXYZAxes();	
	
	return 0;
}

