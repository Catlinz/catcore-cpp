#include <assert.h>
#include <ctime>
#include <cstdio>
#include <iostream>
#include "core/math/mat4.h"
#ifndef DEBUG
#define DEBUG 1
#endif

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)


namespace cc {

	void testBasicMat4CreationAndAssignment() {
		BEGIN_TEST;

		Mat4 m1;
		m1 = Mat4::IDENTITY;
		
		assert(m1 == Mat4::IDENTITY);
		Mat4::MatrixData& m1d = m1.getMatrixData();
		assert( m1d.m11 == REAL(1.0) && m1d.m12 == REAL(0.0) && m1d.m13 == REAL(0.0) && m1d.m14 == REAL(0.0) );
		assert( m1d.m21 == REAL(0.0) && m1d.m22 == REAL(1.0) && m1d.m23 == REAL(0.0) && m1d.m24 == REAL(0.0) );
		assert( m1d.m31 == REAL(0.0) && m1d.m32 == REAL(0.0) && m1d.m33 == REAL(1.0) && m1d.m34 == REAL(0.0) );
		assert( m1d.m41 == REAL(0.0) && m1d.m42 == REAL(0.0) && m1d.m43 == REAL(0.0) && m1d.m44 == REAL(1.0) );
		assert( m1d.m[0] == REAL(1.0) && m1d.m[1] == REAL(0.0) && m1d.m[2] == REAL(0.0) && m1d.m[3] == REAL(0.0) );
		assert( m1d.m[4] == REAL(0.0) && m1d.m[5] == REAL(1.0) && m1d.m[6] == REAL(0.0) && m1d.m[7] == REAL(0.0) );
		assert( m1d.m[8] == REAL(0.0) && m1d.m[9] == REAL(0.0) && m1d.m[10] == REAL(1.0) && m1d.m[11] == REAL(0.0) );
		assert( m1d.m[12] == REAL(0.0) && m1d.m[13] == REAL(0.0) && m1d.m[14] == REAL(0.0) && m1d.m[15] == REAL(1.0) );

		// Per component constructor ////////////////////////
		m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
					 REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
					 REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
					 REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));
		m1d = m1.getMatrixData();	
		assert( m1d.m11 == REAL(1.0) && m1d.m12 == REAL(2.0) && m1d.m13 == REAL(3.0) && m1d.m14 == REAL(4.0) );
		assert( m1d.m21 == REAL(5.0) && m1d.m22 == REAL(6.0) && m1d.m23 == REAL(7.0) && m1d.m24 == REAL(8.0) );
		assert( m1d.m31 == REAL(9.0) && m1d.m32 == REAL(10.0) && m1d.m33 == REAL(11.0) && m1d.m34 == REAL(12.0) );
		assert( m1d.m41 == REAL(13.0) && m1d.m42 == REAL(14.0) && m1d.m43 == REAL(15.0) && m1d.m44 == REAL(16.0) );
		assert( m1d.m[0] == REAL(1.0) && m1d.m[1] == REAL(2.0) && m1d.m[2] == REAL(3.0) && m1d.m[3] == REAL(4.0) );
		assert( m1d.m[4] == REAL(5.0) && m1d.m[5] == REAL(6.0) && m1d.m[6] == REAL(7.0) && m1d.m[7] == REAL(8.0) );
		assert( m1d.m[8] == REAL(9.0) && m1d.m[9] == REAL(10.0) && m1d.m[10] == REAL(11.0) && m1d.m[11] == REAL(12.0) );
		assert( m1d.m[12] == REAL(13.0) && m1d.m[13] == REAL(14.0) && m1d.m[14] == REAL(15.0) && m1d.m[15] == REAL(16.0) );

		// Copy constructor ////////////////////////
		Mat4 m3(m1);
		Mat4::MatrixData& m3d = m3.getMatrixData();	
		assert( m3d.m11 == REAL(1.0) && m3d.m12 == REAL(2.0) && m3d.m13 == REAL(3.0) && m3d.m14 == REAL(4.0) );
		assert( m3d.m21 == REAL(5.0) && m3d.m22 == REAL(6.0) && m3d.m23 == REAL(7.0) && m3d.m24 == REAL(8.0) );
		assert( m3d.m31 == REAL(9.0) && m3d.m32 == REAL(10.0) && m3d.m33 == REAL(11.0) && m3d.m34 == REAL(12.0) );
		assert( m3d.m41 == REAL(13.0) && m3d.m42 == REAL(14.0) && m3d.m43 == REAL(15.0) && m3d.m44 == REAL(16.0) );

		// Assignment //////////////////////////////////
		m3 = Mat4::IDENTITY;		
		assert( m3d.m11 == REAL(1.0) && m3d.m12 == REAL(0.0) && m3d.m13 == REAL(0.0) && m3d.m14 == REAL(0.0) );
		assert( m3d.m21 == REAL(0.0) && m3d.m22 == REAL(1.0) && m3d.m23 == REAL(0.0) && m3d.m24 == REAL(0.0) );
		assert( m3d.m31 == REAL(0.0) && m3d.m32 == REAL(0.0) && m3d.m33 == REAL(1.0) && m3d.m34 == REAL(0.0) );
		assert( m3d.m41 == REAL(0.0) && m3d.m42 == REAL(0.0) && m3d.m43 == REAL(0.0) && m3d.m44 == REAL(1.0) );

		// Single value constructor /////////////////////////
		m1 = Mat4::IDENTITY;
		assert( m1 == Mat4::IDENTITY );
		m1 = Mat4(REAL(3.14));	
		assert( m1d.m11 == REAL(3.14) && m1d.m12 == REAL(3.14) && m1d.m13 == REAL(3.14) && m1d.m14 == REAL(3.14) );
		assert( m1d.m21 == REAL(3.14) && m1d.m22 == REAL(3.14) && m1d.m23 == REAL(3.14) && m1d.m24 == REAL(3.14) );
		assert( m1d.m31 == REAL(3.14) && m1d.m32 == REAL(3.14) && m1d.m33 == REAL(3.14) && m1d.m34 == REAL(3.14) );
		assert( m1d.m41 == REAL(3.14) && m1d.m42 == REAL(3.14) && m1d.m43 == REAL(3.14) && m1d.m44 == REAL(3.14) );
		

		// Array value constructor //////////////////////
		Real cols[16] = { 
			REAL(-1.0), REAL(-2.0), REAL(-3.0), REAL(-4.0), 
			REAL(-5.0), REAL(-6.0), REAL(-7.0), REAL(-8.0),
			REAL(-9.0), REAL(-10.0), REAL(-11.0), REAL(-12.0),
			REAL(-13.0), REAL(-14.0), REAL(-15.0), REAL(-16.0),
		};
		
		m1 = Mat4::IDENTITY;
		assert( m1 == Mat4::IDENTITY );
		m1 = Mat4(cols);
		assert( m1d.m11 == REAL(-1.0) && m1d.m12 == REAL(-2.0) && m1d.m13 == REAL(-3.0) && m1d.m14 == REAL(-4.0) );
		assert( m1d.m21 == REAL(-5.0) && m1d.m22 == REAL(-6.0) && m1d.m23 == REAL(-7.0) && m1d.m24 == REAL(-8.0) );
		assert( m1d.m31 == REAL(-9.0) && m1d.m32 == REAL(-10.0) && m1d.m33 == REAL(-11.0) && m1d.m34 == REAL(-12.0) );
		assert( m1d.m41 == REAL(-13.0) && m1d.m42 == REAL(-14.0) && m1d.m43 == REAL(-15.0) && m1d.m44 == REAL(-16.0) );

		// 3x3 Matrix constructor //////////////////////////
		Mat3 m3x3 = Mat3(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), 
							  REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), 
							  REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790));
		m1 = Mat4(m3x3);
		assert( m1d.m11 == REAL(0.7280277254) && m1d.m12 == REAL(0.6087885979) && m1d.m13 == REAL(-0.31520164041) && m1d.m14 == REAL(0.0) );
		assert( m1d.m21 == REAL(-0.5251048211) && m1d.m22 == REAL(0.7907905580) && m1d.m23 == REAL(0.31450790171) && m1d.m24 == REAL(0.0) );
		assert( m1d.m31 == REAL(0.44072730561) && m1d.m32 == REAL(-0.06345657130) && m1d.m33 == REAL(0.8953952790) && m1d.m34 == REAL(0.0) );
		assert( m1d.m41 == REAL(0.0) && m1d.m42 == REAL(0.0) && m1d.m43 == REAL(0.0) && m1d.m44 == REAL(1.0) );

		// 3x3 Matrix assignment //////////////////////////////
		m1 = Mat4(REAL(99.9));
		m1 = m3x3;
		assert( m1d.m11 == REAL(0.7280277254) && m1d.m12 == REAL(0.6087885979) && m1d.m13 == REAL(-0.31520164041) && m1d.m14 == REAL(99.9) );
		assert( m1d.m21 == REAL(-0.5251048211) && m1d.m22 == REAL(0.7907905580) && m1d.m23 == REAL(0.31450790171) && m1d.m24 == REAL(99.9) );
		assert( m1d.m31 == REAL(0.44072730561) && m1d.m32 == REAL(-0.06345657130) && m1d.m33 == REAL(0.8953952790) && m1d.m34 == REAL(99.9) );
		assert( m1d.m41 == REAL(99.9) && m1d.m42 == REAL(99.9) && m1d.m43 == REAL(99.9) && m1d.m44 == REAL(99.9) );
		m1 = Mat4::IDENTITY;
		m1 = m3x3;
		assert( m1d.m11 == REAL(0.7280277254) && m1d.m12 == REAL(0.6087885979) && m1d.m13 == REAL(-0.31520164041) && m1d.m14 == REAL(0.0) );
		assert( m1d.m21 == REAL(-0.5251048211) && m1d.m22 == REAL(0.7907905580) && m1d.m23 == REAL(0.31450790171) && m1d.m24 == REAL(0.0) );
		assert( m1d.m31 == REAL(0.44072730561) && m1d.m32 == REAL(-0.06345657130) && m1d.m33 == REAL(0.8953952790) && m1d.m34 == REAL(0.0) );
		assert( m1d.m41 == REAL(0.0) && m1d.m42 == REAL(0.0) && m1d.m43 == REAL(0.0) && m1d.m44 == REAL(1.0) );

		// Quaternion constructor /////////////////////////////
		// Rotation 45 degrees around x axis.
		Quaternion q = Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287));
		m1 = Mat4(q);
		assert( m1.approx(Mat4(1.0, 0.0, 0.0, 0.0,
									  0.0, 0.70710678118655, -0.70710678118655, 0.0,
									  0.0, 0.70710678118655, 0.70710678118655, 0.0,
									  0.0, 0.0, 0.0, 1.0)) );
		
		// 90 Degree rotation around x axis.
		q = Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655));
		m1 = Mat4(q);
		assert( m1.approx(Mat4(1.0, 0.0, 0.0, 0.0,
									  0.0, 0.0, -1.0, 0.0,
									  0.0, 1.0, 0.0, 0.0,
									  0.0, 0.0, 0.0, 1.0)) );

		// 360 Degree rotation around x axis.
		q = Quaternion(0.0, 0.0, 0.0, -1.0);
		m1 = Mat4(q);
		assert( m1.approx(Mat4(1.0, 0.0, 0.0, 0.0,
									  0.0, 1.0, 0.0, 0.0,
									  0.0, 0.0, 1.0, 0.0,
									  0.0, 0.0, 0.0, 1.0)) );	

		// Rotation 45 degrees around y axis.
		q = Quaternion(REAL(0.0), REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287));
		m1 = Mat4(q);
		assert( m1.approx(Mat4(0.70710678118655, 0.0, 0.70710678118655, 0.0,
									  0.0, 1.0, 0.0, 0.0,
									  -0.70710678118655, 0.0, 0.70710678118655, 0.0,
									  0.0, 0.0, 0.0, 1.0)) );

		// Rotation 45 degrees around z axis.
		q = Quaternion(REAL(0.0), REAL(0.0), REAL(0.38268343236509), REAL(0.923879532511287));
		m1 = Mat4(q);
		assert( m1.approx(Mat4(0.70710678118655, -0.70710678118655, 0.0, 0.0,
									  0.70710678118655, 0.70710678118655, 0.0, 0.0,
									  0.0, 0.0, 1.0, 0.0,
									  0.0, 0.0, 0.0, 1.0)) );

		// Rotation 45 degrees around xyz axis'.
		q = Quaternion(REAL(0.22094238269), REAL(0.22094238269), REAL(0.22094238269), REAL(0.923879532511));
		m1 = Mat4(q);
		assert( m1.approx(Mat4(0.8047378541, -0.3106172175, 0.5058793634, 0.0,
									  0.5058793634, 0.8047378541, -0.3106172175, 0.0,
									  -0.3106172175, 0.5058793634, 0.8047378541, 0.0,
									  0.0, 0.0, 0.0, 1.0)) );
		FINISH_TEST;
		
	}
 
	void testMat4Equality() {
		BEGIN_TEST;
		
		Mat4 m1(Mat4::IDENTITY);
		assert( m1 == Mat4::IDENTITY && Mat4::IDENTITY == m1 && 
				  !(m1 != Mat4::IDENTITY) && !(Mat4::IDENTITY != m1) );
		assert( m1 == m1 && !(m1 != m1) );
		assert( Mat4::IDENTITY == Mat4::IDENTITY && !(Mat4::IDENTITY != Mat4::IDENTITY) );
		
		
		Mat4 m2 = Mat4::IDENTITY;
		assert( m1 == m2 && !(m1 != m2) );
		
		m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
					 REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
					 REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
					 REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));
		assert( m1 != Mat4::IDENTITY && !(m1 == Mat4::IDENTITY) );
		assert( m1 != m2 && !(m1 == m2) );

		m2 = m1;
		assert( m1 == m2 && !(m1 != m2) );
		Mat4::MatrixData& md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) && md.m14 == REAL(4.0) );
		assert( md.m21 == REAL(5.0) && md.m22 == REAL(6.0) && md.m23 == REAL(7.0) && md.m24 == REAL(8.0) );
		assert( md.m31 == REAL(9.0) && md.m32 == REAL(10.0) && md.m33 == REAL(11.0) && md.m34 == REAL(12.0) );
		assert( md.m41 == REAL(13.0) && md.m42 == REAL(14.0) && md.m43 == REAL(15.0) && md.m44 == REAL(16.0) );
		
		// Check to make sure the equality is checking every component
		md.m11 = REAL(-1.0);
		assert( m1 != m2);
		assert( !(m1 == m2) );
		assert( !m1.approx(m2) );
		assert( !m2.approx(m1) );
		
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m11 = REAL(1.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m12 = REAL(-2.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m12 = REAL(2.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m13 = REAL(-3.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m13 = REAL(3.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m14 = REAL(-4.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m14 = REAL(4.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		md.m21 = REAL(-5.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m21 = REAL(5.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m22 = REAL(-6.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m22 = REAL(6.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m23 = REAL(-7.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m23 = REAL(7.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m24 = REAL(-8.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m24 = REAL(8.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		md.m31 = REAL(-9.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m31 = REAL(9.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m32 = REAL(-10.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m32 = REAL(10.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m33 = REAL(-11.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m33 = REAL(11.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m34 = REAL(-12.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m34 = REAL(12.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		md.m41 = REAL(-13.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m41 = REAL(13.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m42 = REAL(-14.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m42 = REAL(14.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m43 = REAL(-15.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m43 = REAL(15.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );
		md.m44 = REAL(-16.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m44 = REAL(16.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		FINISH_TEST;	
	}
 
	void testMat4ApproximateEquality() {
		BEGIN_TEST;
	
		Mat4 m1(Mat4::IDENTITY);
		assert( m1 == Mat4::IDENTITY && Mat4::IDENTITY == m1 && 
				  !(m1 != Mat4::IDENTITY) && !(Mat4::IDENTITY != m1) );
		assert( m1 == m1 && !(m1 != m1) );
		assert( Mat4::IDENTITY == Mat4::IDENTITY && !(Mat4::IDENTITY != Mat4::IDENTITY) );
		
		
		Mat4 m2 = Mat4::IDENTITY;
		assert( m1 == m2 && !(m1 != m2) );
		
		m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
					 REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
					 REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
					 REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));
		assert( m1 != Mat4::IDENTITY && !(m1 == Mat4::IDENTITY) );
		assert( m1 != m2 && !(m1 == m2) );

		m2 = m1;
		assert( m1 == m2 && !(m1 != m2) );
		Mat4::MatrixData& md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) && md.m14 == REAL(4.0) );
		assert( md.m21 == REAL(5.0) && md.m22 == REAL(6.0) && md.m23 == REAL(7.0) && md.m24 == REAL(8.0) );
		assert( md.m31 == REAL(9.0) && md.m32 == REAL(10.0) && md.m33 == REAL(11.0) && md.m34 == REAL(12.0) );
		assert( md.m41 == REAL(13.0) && md.m42 == REAL(14.0) && md.m43 == REAL(15.0) && md.m44 == REAL(16.0) );
		
		// Check to make sure the approximate equality is checking every component
		md.m11 += (Mat4::EPSILON / REAL(2.0));		
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m11 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m11 -= Mat4::EPSILON;	  
		md.m12 += (Mat4::EPSILON / REAL(2.0));		
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m12 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m12 -= Mat4::EPSILON;
		md.m13 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m13 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m13 -= Mat4::EPSILON;
		md.m14 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m14 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m14 -= Mat4::EPSILON;

		md.m21 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m21 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m21 -= Mat4::EPSILON;
		md.m22 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m22 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m22 -= Mat4::EPSILON;
		md.m23 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m23 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m23 -= Mat4::EPSILON;
		md.m24 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m24 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m24 -= Mat4::EPSILON;

		md.m31 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m31 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m31 -= Mat4::EPSILON;
		md.m32 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m32 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m32 -= Mat4::EPSILON;
		md.m33 += (Mat4::EPSILON / REAL(2.0));	
 		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m33 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m33 -= Mat4::EPSILON;
		md.m34 += (Mat4::EPSILON / REAL(2.0));	
 		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m34 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m34 -= Mat4::EPSILON;

		md.m41 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m41 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m41 -= Mat4::EPSILON;
		md.m42 += (Mat4::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m42 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m42 -= Mat4::EPSILON;
		md.m43 += (Mat4::EPSILON / REAL(2.0));	
 		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m43 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m43 -= Mat4::EPSILON;
		md.m44 += (Mat4::EPSILON / REAL(2.0));	
 		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m44 += Mat4::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m44 -= Mat4::EPSILON;

		FINISH_TEST;	
	}

	void testMatrixTransposition() {
		BEGIN_TEST;
		
		Mat4 m1(Mat4::IDENTITY);
		Mat4 r = m1.transposed();
		assert( r == m1 && r == Mat4::IDENTITY );
		
		m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
					 REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
					 REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
					 REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));

		r = m1.transposed();
		Mat4::MatrixData md = r.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(5.0) && md.m13 == REAL(9.0) && md.m14 == REAL(13.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(6.0) && md.m23 == REAL(10.0) && md.m24 == REAL(14.0) );
		assert( md.m31 == REAL(3.0) && md.m32 == REAL(7.0) && md.m33 == REAL(11.0) && md.m34 == REAL(15.0) );
		assert( md.m41 == REAL(4.0) && md.m42 == REAL(8.0) && md.m43 == REAL(12.0) && md.m44 == REAL(16.0) );
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) && md.m14 == REAL(4.0) );
		assert( md.m21 == REAL(5.0) && md.m22 == REAL(6.0) && md.m23 == REAL(7.0) && md.m24 == REAL(8.0) );
		assert( md.m31 == REAL(9.0) && md.m32 == REAL(10.0) && md.m33 == REAL(11.0) && md.m34 == REAL(12.0) );
		assert( md.m41 == REAL(13.0) && md.m42 == REAL(14.0) && md.m43 == REAL(15.0) && md.m44 == REAL(16.0) );
		m1.transpose();
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(5.0) && md.m13 == REAL(9.0) && md.m14 == REAL(13.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(6.0) && md.m23 == REAL(10.0) && md.m24 == REAL(14.0) );
		assert( md.m31 == REAL(3.0) && md.m32 == REAL(7.0) && md.m33 == REAL(11.0) && md.m34 == REAL(15.0) );
		assert( md.m41 == REAL(4.0) && md.m42 == REAL(8.0) && md.m43 == REAL(12.0) && md.m44 == REAL(16.0) );
		m1.transpose();
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) && md.m14 == REAL(4.0) );
		assert( md.m21 == REAL(5.0) && md.m22 == REAL(6.0) && md.m23 == REAL(7.0) && md.m24 == REAL(8.0) );
		assert( md.m31 == REAL(9.0) && md.m32 == REAL(10.0) && md.m33 == REAL(11.0) && md.m34 == REAL(12.0) );
		assert( md.m41 == REAL(13.0) && md.m42 == REAL(14.0) && md.m43 == REAL(15.0) && md.m44 == REAL(16.0) );

		FINISH_TEST;
	}
	
	void testMatrixDeterminant() {
		BEGIN_TEST;
		
		// Determinant test # 1
		Mat4 m1(Mat4::IDENTITY);
		Real det = m1.determinant();
		assert( det == REAL(1.0) );
		
		// Determinant test # 2
	   m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
					 REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
					 REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
					 REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));
		det = m1.determinant();
		assert( det == REAL(0.0) );
	   m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
					 REAL(2.0), REAL(4.0), REAL(6.0), REAL(8.0),
					 REAL(3.0), REAL(6.0), REAL(9.0), REAL(12.0),
					 REAL(4.0), REAL(8.0), REAL(12.0), REAL(16.0));
		det = m1.determinant();
		assert( det == REAL(0.0) );

		// Determinant test # 3
		m1 = Mat4(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), REAL(0.0), 
					 REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), REAL(0.0),
					 REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790), REAL(0.0),
					 REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));
		
		det = m1.determinant();
		assert( det == REAL(1.0) );

		// Determinant test # 4
		m1 = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
					 REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(0.0), 
					 REAL(3.0), REAL(0.0), REAL(4.0), REAL(0.0),
					 REAL(0.0), REAL(5.0), REAL(0.0), REAL(5.0));
		det = m1.determinant();
		assert( det == REAL(2660.0) );

		// Determinant test # 5
		m1 = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
					 REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(0.0), 
					 REAL(3.0), REAL(0.0), REAL(4.0), REAL(0.0),
					 REAL(1.0), REAL(5.0), REAL(7.0), REAL(5.0));
		det = m1.determinant();
		assert( det == REAL(-400.0) );
		
		// Determinant test # 6
		m1 = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
					REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(4.0), 
					REAL(3.0), REAL(0.0), REAL(4.0), REAL(-1.0),
					 REAL(1.0), REAL(5.0), REAL(7.0), REAL(5.0));
		det = m1.determinant();
		assert( det == REAL(-484.0) );

		FINISH_TEST;
		}
 
	void testMatrixInversion() {
		BEGIN_TEST;
		
		// Inversion test # 1
		Mat4 m(Mat4::IDENTITY);
		Mat4 r = m.inverse();
		assert( m == r );
		
		// Inverse test # 2
		m = Mat4(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), REAL(0.0), 
					REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), REAL(0.0),
					REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790), REAL(0.0),
					REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));
	   r = m.inverse();
		assert(r.approx(Mat4(0.7280277254, -0.5251048211, 0.44072730561, 0.0,
									0.6087885979, 0.7907905580, -0.06345657130, 0.0, 
									-0.31520164041, 0.31450790171, 0.8953952790, 0.0,
									0.0, 0.0, 0.0, 1.0)) );
		assert(m == Mat4(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), REAL(0.0), 
							  REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), REAL(0.0), 
							  REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790), REAL(0.0),
							  REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0)) );
		m.invert();
		assert(m.approx(Mat4(0.7280277254, -0.5251048211, 0.44072730561, 0.0,
									0.6087885979, 0.7907905580, -0.06345657130, 0.0, 
									-0.31520164041, 0.31450790171, 0.8953952790, 0.0,
									0.0, 0.0, 0.0, 1.0)) );

		r = m * Mat4(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), REAL(0.0), 
						 REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), REAL(0.0),
						 REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790), REAL(0.0),
						 REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));
		assert( r.approx(Mat4::IDENTITY) );
		m = Mat4(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), REAL(0.0), 
					REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), REAL(0.0),
					REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790), REAL(0.0),
					REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));
		r = m * m.inverse();
		assert( r.approx(Mat4::IDENTITY) );
		
		

		// Inverse test # 3
		m = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
					REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(0.0), 
					REAL(3.0), REAL(0.0), REAL(4.0), REAL(0.0),
					REAL(0.0), REAL(5.0), REAL(0.0), REAL(5.0));
		r = m.inverse();		
		assert(r.approx(Mat4(-0.0150376, 0.676692, 0.338346, 0.270677,
									-0.0112782, 0.0075188, 0.0037594, 0.203008, 
									0.0112782, -0.507519, -0.0037594, -0.203008, 
									0.0112782, -0.0075188, -0.0037594, -0.00300752)) );
		assert(m == Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
							  REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(0.0), 
							  REAL(3.0), REAL(0.0), REAL(4.0), REAL(0.0),
							  REAL(0.0), REAL(5.0), REAL(0.0), REAL(5.0)));
		m.invert();
		assert(m.approx(Mat4(-0.0150376, 0.676692, 0.338346, 0.270677,
									-0.0112782, 0.0075188, 0.0037594, 0.203008, 
									0.0112782, -0.507519, -0.0037594, -0.203008, 
									0.0112782, -0.0075188, -0.0037594, -0.00300752)) );
		r = m * Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
						 REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(0.0), 
						 REAL(3.0), REAL(0.0), REAL(4.0), REAL(0.0),
						 REAL(0.0), REAL(5.0), REAL(0.0), REAL(5.0));
		assert( r.approx(Mat4::IDENTITY) );
		m = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
					REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(0.0), 
					REAL(3.0), REAL(0.0), REAL(4.0), REAL(0.0),
					REAL(0.0), REAL(5.0), REAL(0.0), REAL(5.0));
		r = m * m.inverse();
		assert( r.approx(Mat4::IDENTITY) );
		m *= m.inverse();
		assert( m.approx(Mat4::IDENTITY) );
		

		// Inverse test # 4
		m = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
					REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(4.0), 
					REAL(3.0), REAL(0.0), REAL(4.0), REAL(-1.0),
					REAL(1.0), REAL(5.0), REAL(7.0), REAL(5.0));
		r = m.inverse();
		assert(r.approx(Mat4(0.256198, -3.719008, 0.743802, -1.487603,
									0.206612, -3.216942, 0.293388, -1.086777, 
									-0.190083, 2.799587, -0.309917, 1.119835, 
									0.008264, 0.041322, -0.008264, 0.016529)) );
		assert(m == Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
							  REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(4.0), 
							  REAL(3.0), REAL(0.0), REAL(4.0), REAL(-1.0),
							  REAL(1.0), REAL(5.0), REAL(7.0), REAL(5.0)));
		m.invert();
		assert(m.approx(Mat4(0.256198, -3.719008, 0.743802, -1.487603,
									0.206612, -3.216942, 0.293388, -1.086777, 
									-0.190083, 2.799587, -0.309917, 1.119835, 
									0.008264, 0.041322, -0.008264, 0.016529)) );
		r = m * Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
						 REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(4.0), 
						 REAL(3.0), REAL(0.0), REAL(4.0), REAL(-1.0),
						 REAL(1.0), REAL(5.0), REAL(7.0), REAL(5.0));
		assert( r.approx(Mat4::IDENTITY) );
		m = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(90.0), 
					REAL(0.0), REAL(-2.0), REAL(-2.0), REAL(4.0), 
					REAL(3.0), REAL(0.0), REAL(4.0), REAL(-1.0),
					REAL(1.0), REAL(5.0), REAL(7.0), REAL(5.0));
		r = m * m.inverse();
		assert( r.approx(Mat4::IDENTITY) );
		m *= m.inverse();
		assert( m.approx(Mat4::IDENTITY) );

		FINISH_TEST;
	}
  
	void testMat4Addition() {
		BEGIN_TEST;
		
		// Addition test #1 ////////////////
		Mat4 m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
							REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
							REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
							REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));

		Mat4 m2 = Mat4(REAL(0.5), REAL(1.0), REAL(1.5), REAL(2.0), 
							REAL(2.5), REAL(3.0), REAL(3.5), REAL(4.0),
							REAL(4.5), REAL(5.0), REAL(5.5), REAL(6.0),
							REAL(6.5), REAL(7.0), REAL(7.5), REAL(8.0));
		Mat4 r = m1 + m2;
		Mat4::MatrixData md = r.getMatrixData();	
		assert( md.m11 == REAL(1.5) && md.m12 == REAL(3.0) && md.m13 == REAL(4.5) && md.m14 == REAL(6.0) );
		assert( md.m21 == REAL(7.5) && md.m22 == REAL(9.0) && md.m23 == REAL(10.5) && md.m24 == REAL(12.0) );
		assert( md.m31 == REAL(13.5) && md.m32 == REAL(15.0) && md.m33 == REAL(16.5) && md.m34 == REAL(18.0) );
		assert( md.m41 == REAL(19.5) && md.m42 == REAL(21.0) && md.m43 == REAL(22.5) && md.m44 == REAL(24.0) );
		Mat4 r2 = m2 + m1;
		assert( r == r2 && !(r != r2 ) );
		m2 += m1;
		md = m2.getMatrixData();	
		assert( md.m11 == REAL(1.5) && md.m12 == REAL(3.0) && md.m13 == REAL(4.5) && md.m14 == REAL(6.0) );
		assert( md.m21 == REAL(7.5) && md.m22 == REAL(9.0) && md.m23 == REAL(10.5) && md.m24 == REAL(12.0) );
		assert( md.m31 == REAL(13.5) && md.m32 == REAL(15.0) && md.m33 == REAL(16.5) && md.m34 == REAL(18.0) );
		assert( md.m41 == REAL(19.5) && md.m42 == REAL(21.0) && md.m43 == REAL(22.5) && md.m44 == REAL(24.0) );
		m2 = Mat4(REAL(0.5), REAL(1.0), REAL(1.5), REAL(2.0), 
					 REAL(2.5), REAL(3.0), REAL(3.5), REAL(4.0),
					 REAL(4.5), REAL(5.0), REAL(5.5), REAL(6.0),
					 REAL(6.5), REAL(7.0), REAL(7.5), REAL(8.0));
		m1 += m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.5) && md.m12 == REAL(3.0) && md.m13 == REAL(4.5) && md.m14 == REAL(6.0) );
		assert( md.m21 == REAL(7.5) && md.m22 == REAL(9.0) && md.m23 == REAL(10.5) && md.m24 == REAL(12.0) );
		assert( md.m31 == REAL(13.5) && md.m32 == REAL(15.0) && md.m33 == REAL(16.5) && md.m34 == REAL(18.0) );
		assert( md.m41 == REAL(19.5) && md.m42 == REAL(21.0) && md.m43 == REAL(22.5) && md.m44 == REAL(24.0) );
		
		// Addition test #2 /////////////////
		m1 = Mat4(REAL(-0.5), REAL(0.0), REAL(0.0), REAL(1.0), 
					 REAL(-2.0), REAL(-1.5), REAL(-10.0), REAL(9.3),
					 REAL(0.0), REAL(0.0), REAL(0.0), REAL(0.0),
					 REAL(-1.0), REAL(1.0), REAL(0.5), REAL(-0.5));
		r = m1 + m2;
		md = r.getMatrixData();	
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(3.0) );
		assert( md.m21 == REAL(0.5) && md.m22 == REAL(1.5) && md.m23 == REAL(-6.5) && md.m24 == REAL(13.3) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(5.5) && md.m42 == REAL(8.0) && md.m43 == REAL(8.0) && md.m44 == REAL(7.5) );
		r2 = m2 + m1;
		assert( r2 == r && !(r != r2) );
		m1 += m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(3.0) );
		assert( md.m21 == REAL(0.5) && md.m22 == REAL(1.5) && md.m23 == REAL(-6.5) && md.m24 == REAL(13.3) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(5.5) && md.m42 == REAL(8.0) && md.m43 == REAL(8.0) && md.m44 == REAL(7.5) );
		m1 = Mat4(REAL(-0.5), REAL(0.0), REAL(0.0), REAL(1.0), 
					 REAL(-2.0), REAL(-1.5), REAL(-10.0), REAL(9.3),
					 REAL(0.0), REAL(0.0), REAL(0.0), REAL(0.0),
					 REAL(-1.0), REAL(1.0), REAL(0.5), REAL(-0.5));
		m2 += m1;
		md = m2.getMatrixData();	
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(3.0) );
		assert( md.m21 == REAL(0.5) && md.m22 == REAL(1.5) && md.m23 == REAL(-6.5) && md.m24 == REAL(13.3) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(5.5) && md.m42 == REAL(8.0) && md.m43 == REAL(8.0) && md.m44 == REAL(7.5) );

		// Addition test #3 ////////////////////
		m2 = Mat4(REAL(0.0));
		r = m1 + m2;
		assert( r == m1 );
		r2 = m2 + m1;
		assert( r2 == m1 );
		m1 += m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(-0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) && md.m14 == REAL(1.0) );
		assert( md.m21 == REAL(-2.0) && md.m22 == REAL(-1.5) && md.m23 == REAL(-10.0) && md.m24 == REAL(9.3) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) && md.m34 == REAL(0.0) );
		assert( md.m41 == REAL(-1.0) && md.m42 == REAL(1.0) && md.m43 == REAL(0.5) && md.m44 == REAL(-0.5) );

		m2 += m1;
		assert( m2 == m1 );
		
		// Addition test #4 ///////////////////
		m2 = Mat4(REAL(0.0));
		m2 += Mat4(REAL(-0.5), REAL(0.0), REAL(0.0), REAL(1.0), 
					  REAL(-2.0), REAL(-1.5), REAL(-10.0), REAL(9.3),
					  REAL(0.0), REAL(0.0), REAL(0.0), REAL(0.0),
					  REAL(-1.0), REAL(1.0), REAL(0.5), REAL(-0.5));
		assert( m1 == m2 && m2 == m1 );

		FINISH_TEST;
	}
  
	void testMat4Subtraction() {
		BEGIN_TEST;
		
		// Subtraction test #1 ////////////////
		Mat4 m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
							REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
							REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
							REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));

		Mat4 m2 = Mat4(REAL(0.5), REAL(1.0), REAL(1.5), REAL(2.0), 
							REAL(2.5), REAL(3.0), REAL(3.5), REAL(4.0),
							REAL(4.5), REAL(5.0), REAL(5.5), REAL(6.0),
							REAL(6.5), REAL(7.0), REAL(7.5), REAL(8.0));
		Mat4 r = m1 - m2;
		Mat4::MatrixData md = r.getMatrixData();	
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(2.0) );
		assert( md.m21 == REAL(2.5) && md.m22 == REAL(3.0) && md.m23 == REAL(3.5) && md.m24 == REAL(4.0) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(6.5) && md.m42 == REAL(7.0) && md.m43 == REAL(7.5) && md.m44 == REAL(8.0) );
		Mat4 r2 = m2 - m1;
		assert( r != r2 && !(r == r2 ) );
		md = r2.getMatrixData();
		assert( md.m11 == REAL(-0.5) && md.m12 == REAL(-1.0) && md.m13 == REAL(-1.5) && md.m14 == REAL(-2.0) );
		assert( md.m21 == REAL(-2.5) && md.m22 == REAL(-3.0) && md.m23 == REAL(-3.5) && md.m24 == REAL(-4.0) );
		assert( md.m31 == REAL(-4.5) && md.m32 == REAL(-5.0) && md.m33 == REAL(-5.5) && md.m34 == REAL(-6.0) );
		assert( md.m41 == REAL(-6.5) && md.m42 == REAL(-7.0) && md.m43 == REAL(-7.5) && md.m44 == REAL(-8.0) );
		m2 -= m1;
		md = m2.getMatrixData();	
		assert( md.m11 == REAL(-0.5) && md.m12 == REAL(-1.0) && md.m13 == REAL(-1.5) && md.m14 == REAL(-2.0) );
		assert( md.m21 == REAL(-2.5) && md.m22 == REAL(-3.0) && md.m23 == REAL(-3.5) && md.m24 == REAL(-4.0) );
		assert( md.m31 == REAL(-4.5) && md.m32 == REAL(-5.0) && md.m33 == REAL(-5.5) && md.m34 == REAL(-6.0) );
		assert( md.m41 == REAL(-6.5) && md.m42 == REAL(-7.0) && md.m43 == REAL(-7.5) && md.m44 == REAL(-8.0) );
		m2 = Mat4(REAL(0.5), REAL(1.0), REAL(1.5), REAL(2.0), 
					 REAL(2.5), REAL(3.0), REAL(3.5), REAL(4.0),
					 REAL(4.5), REAL(5.0), REAL(5.5), REAL(6.0),
					 REAL(6.5), REAL(7.0), REAL(7.5), REAL(8.0));
		m1 -= m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(2.0) );
		assert( md.m21 == REAL(2.5) && md.m22 == REAL(3.0) && md.m23 == REAL(3.5) && md.m24 == REAL(4.0) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(6.5) && md.m42 == REAL(7.0) && md.m43 == REAL(7.5) && md.m44 == REAL(8.0) );
		
		// Subtraction test #2 /////////////////
		m1 = Mat4(REAL(-0.5), REAL(0.0), REAL(0.0), REAL(1.0), 
					 REAL(-2.0), REAL(-1.5), REAL(-10.0), REAL(9.3),
					 REAL(0.0), REAL(0.0), REAL(0.0), REAL(0.0),
					 REAL(-1.0), REAL(1.0), REAL(0.5), REAL(-0.5));
		r = m1 - m2;
		md = r.getMatrixData();	
		assert( md.m11 == REAL(-1.0) && md.m12 == REAL(-1.0) && md.m13 == REAL(-1.5) && md.m14 == REAL(-1.0) );
		assert( md.m21 == REAL(-4.5) && md.m22 == REAL(-4.5) && md.m23 == REAL(-13.5) && md.m24 == REAL(5.3) );
		assert( md.m31 == REAL(-4.5) && md.m32 == REAL(-5.0) && md.m33 == REAL(-5.5) && md.m34 == REAL(-6.0) );
		assert( md.m41 == REAL(-7.5) && md.m42 == REAL(-6.0) && md.m43 == REAL(-7.0) && md.m44 == REAL(-8.5) );
		r2 = m2 - m1;
		assert( r2 != r && !(r == r2) );
		md = r2.getMatrixData();
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(1.0) );
		assert( md.m21 == REAL(4.5) && md.m22 == REAL(4.5) && md.m23 == REAL(13.5) && md.m24 == REAL(-5.3) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(7.5) && md.m42 == REAL(6.0) && md.m43 == REAL(7.0) && md.m44 == REAL(8.5) );
		m1 -= m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(-1.0) && md.m12 == REAL(-1.0) && md.m13 == REAL(-1.5) && md.m14 == REAL(-1.0) );
		assert( md.m21 == REAL(-4.5) && md.m22 == REAL(-4.5) && md.m23 == REAL(-13.5) && md.m24 == REAL(5.3) );
		assert( md.m31 == REAL(-4.5) && md.m32 == REAL(-5.0) && md.m33 == REAL(-5.5) && md.m34 == REAL(-6.0) );
		assert( md.m41 == REAL(-7.5) && md.m42 == REAL(-6.0) && md.m43 == REAL(-7.0) && md.m44 == REAL(-8.5) );
		m1 = Mat4(REAL(-0.5), REAL(0.0), REAL(0.0), REAL(1.0), 
					 REAL(-2.0), REAL(-1.5), REAL(-10.0), REAL(9.3),
					 REAL(0.0), REAL(0.0), REAL(0.0), REAL(0.0),
					 REAL(-1.0), REAL(1.0), REAL(0.5), REAL(-0.5));
		m2 -= m1;
		md = m2.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(1.0) );
		assert( md.m21 == REAL(4.5) && md.m22 == REAL(4.5) && md.m23 == REAL(13.5) && md.m24 == REAL(-5.3) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(7.5) && md.m42 == REAL(6.0) && md.m43 == REAL(7.0) && md.m44 == REAL(8.5) );

		// Subtraction test #3 ////////////////////
		m2 = Mat4(REAL(0.0));
		r = m1 - m2;
		assert( r == m1 );
		r2 = m2 - m1;
		assert( r2 != m1 );
		md = r2.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) && md.m14 == REAL(-1.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(1.5) && md.m23 == REAL(10.0) && md.m24 == REAL(-9.3) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) && md.m34 == REAL(0.0) );
		assert( md.m41 == REAL(1.0) && md.m42 == REAL(-1.0) && md.m43 == REAL(-0.5) && md.m44 == REAL(0.5) );

		m1 -= m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(-0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) && md.m14 == REAL(1.0) );
		assert( md.m21 == REAL(-2.0) && md.m22 == REAL(-1.5) && md.m23 == REAL(-10.0) && md.m24 == REAL(9.3) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) && md.m34 == REAL(0.0) );
		assert( md.m41 == REAL(-1.0) && md.m42 == REAL(1.0) && md.m43 == REAL(0.5) && md.m44 == REAL(-0.5) );

		m2 -= m1;
		assert( m2 != m1 );
		md = m2.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) && md.m14 == REAL(-1.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(1.5) && md.m23 == REAL(10.0) && md.m24 == REAL(-9.3) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) && md.m34 == REAL(0.0) );
		assert( md.m41 == REAL(1.0) && md.m42 == REAL(-1.0) && md.m43 == REAL(-0.5) && md.m44 == REAL(0.5) );
		
		// Subtraction test #4 ///////////////////
		m2 = Mat4(REAL(0.0));
		m2 -= Mat4(REAL(-0.5), REAL(0.0), REAL(0.0), REAL(1.0), 
					  REAL(-2.0), REAL(-1.5), REAL(-10.0), REAL(9.3),
					  REAL(0.0), REAL(0.0), REAL(0.0), REAL(0.0),
					  REAL(-1.0), REAL(1.0), REAL(0.5), REAL(-0.5));
		assert( m1 != m2 && m2 != m1 );
		md = m2.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) && md.m14 == REAL(-1.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(1.5) && md.m23 == REAL(10.0) && md.m24 == REAL(-9.3) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) && md.m34 == REAL(0.0) );
		assert( md.m41 == REAL(1.0) && md.m42 == REAL(-1.0) && md.m43 == REAL(-0.5) && md.m44 == REAL(0.5) );

		FINISH_TEST;
	}

	void testMat4ScalarMultiplication() {
		BEGIN_TEST;
		
		// Scalar Multiplication test #1 ////////////////
		Mat4 m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
							REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
							REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
							REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));

		Mat4 r = m1 * REAL(0.5);
		Mat4::MatrixData md = r.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(2.0) );
		assert( md.m21 == REAL(2.5) && md.m22 == REAL(3.0) && md.m23 == REAL(3.5) && md.m24 == REAL(4.0) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(6.5) && md.m42 == REAL(7.0) && md.m43 == REAL(7.5) && md.m44 == REAL(8.0) );
		r = REAL(0.5) * m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(2.0) );
		assert( md.m21 == REAL(2.5) && md.m22 == REAL(3.0) && md.m23 == REAL(3.5) && md.m24 == REAL(4.0) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(6.5) && md.m42 == REAL(7.0) && md.m43 == REAL(7.5) && md.m44 == REAL(8.0) );
		m1 *= REAL(0.5);
		md = m1.getMatrixData();		
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(2.0) );
		assert( md.m21 == REAL(2.5) && md.m22 == REAL(3.0) && md.m23 == REAL(3.5) && md.m24 == REAL(4.0) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(6.5) && md.m42 == REAL(7.0) && md.m43 == REAL(7.5) && md.m44 == REAL(8.0) );

		// Scalar Multiplication test #2 ////////////////////
		m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
					 REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
					 REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
					 REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));
		r = m1 * REAL(1.0);
		assert( r == m1 );
		m1 *= REAL(1.0);
		md = m1.getMatrixData();		
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) && md.m14 == REAL(4.0) );
		assert( md.m21 == REAL(5.0) && md.m22 == REAL(6.0) && md.m23 == REAL(7.0) && md.m24 == REAL(8.0) );
		assert( md.m31 == REAL(9.0) && md.m32 == REAL(10.0) && md.m33 == REAL(11.0) && md.m34 == REAL(12.0) );
		assert( md.m41 == REAL(13.0) && md.m42 == REAL(14.0) && md.m43 == REAL(15.0) && md.m44 == REAL(16.0) );

		// Scalar Multiplication test #3 ////////////////////
		m1 = Mat4(REAL(1.0), REAL(0.0), REAL(3.0), REAL(-4.0), 
					 REAL(4.0), REAL(0.0), REAL(6.0), REAL(0.0),
					 REAL(1.0), REAL(8.0), REAL(9.0), REAL(-1.0),
					 REAL(1.5), REAL(-2.0), REAL(-0.5), REAL(0.5));
		r = m1 * REAL(2.0);
		md = r.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) && md.m14 == REAL(-8.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) && md.m24 == REAL(0.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) && md.m34 == REAL(-2.0) );
		assert( md.m41 == REAL(3.0) && md.m42 == REAL(-4.0) && md.m43 == REAL(-1.0) && md.m44 == REAL(1.0) );
		r = REAL(2.0) * m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) && md.m14 == REAL(-8.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) && md.m24 == REAL(0.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) && md.m34 == REAL(-2.0) );
		assert( md.m41 == REAL(3.0) && md.m42 == REAL(-4.0) && md.m43 == REAL(-1.0) && md.m44 == REAL(1.0) );
		assert( m1 != r );		
		m1 *= REAL(2.0);
		assert( m1 == r );
		md = m1.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) && md.m14 == REAL(-8.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) && md.m24 == REAL(0.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) && md.m34 == REAL(-2.0) );
		assert( md.m41 == REAL(3.0) && md.m42 == REAL(-4.0) && md.m43 == REAL(-1.0) && md.m44 == REAL(1.0) );

		// Scalar mutliplication test #4 /////////////////////
		m1 = Mat4(REAL(1.0), REAL(0.0), REAL(3.0), REAL(-4.0), 
					 REAL(4.0), REAL(0.0), REAL(-6.0), REAL(0.0),
					 REAL(1.0), REAL(8.0), REAL(9.0), REAL(-1.0),
					 REAL(1.5), REAL(-2.0), REAL(-0.5), REAL(0.5));
		r = m1 * 0.0;
		md = r.getMatrixData();
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) && md.m14 == REAL(0.0) );
		assert( md.m21 == REAL(0.0) && md.m22 == REAL(0.0) && md.m23 == REAL(0.0) && md.m24 == REAL(0.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) && md.m34 == REAL(0.0) );
		assert( md.m41 == REAL(0.0) && md.m42 == REAL(0.0) && md.m43 == REAL(0.0) && md.m44 == REAL(0.0) );
		r = 0.0 * m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) && md.m14 == REAL(0.0) );
		assert( md.m21 == REAL(0.0) && md.m22 == REAL(0.0) && md.m23 == REAL(0.0) && md.m24 == REAL(0.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) && md.m34 == REAL(0.0) );
		assert( md.m41 == REAL(0.0) && md.m42 == REAL(0.0) && md.m43 == REAL(0.0) && md.m44 == REAL(0.0) );
		assert( m1 == Mat4(REAL(1.0), REAL(0.0), REAL(3.0), REAL(-4.0), 
								 REAL(4.0), REAL(0.0), REAL(-6.0), REAL(0.0),
								 REAL(1.0), REAL(8.0), REAL(9.0), REAL(-1.0),
								 REAL(1.5), REAL(-2.0), REAL(-0.5), REAL(0.5)) );		
		m1 *= 0.0;
		md = m1.getMatrixData();
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) && md.m14 == REAL(0.0) );
		assert( md.m21 == REAL(0.0) && md.m22 == REAL(0.0) && md.m23 == REAL(0.0) && md.m24 == REAL(0.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) && md.m34 == REAL(0.0) );
		assert( md.m41 == REAL(0.0) && md.m42 == REAL(0.0) && md.m43 == REAL(0.0) && md.m44 == REAL(0.0) );
	
		FINISH_TEST;
	}
	
	void testMat4ScalarDivision() {
		BEGIN_TEST;
		
		// Scalar Division test #1 ////////////////
		Mat4 m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
							REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
							REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
							REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));

		Mat4 r = m1 / REAL(2.0);
		Mat4::MatrixData md = r.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(2.0) );
		assert( md.m21 == REAL(2.5) && md.m22 == REAL(3.0) && md.m23 == REAL(3.5) && md.m24 == REAL(4.0) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(6.5) && md.m42 == REAL(7.0) && md.m43 == REAL(7.5) && md.m44 == REAL(8.0) );
		r = REAL(2.0) / m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(1.0) && approx(md.m13, REAL(0.6666667)) && md.m14 == REAL(0.5) );
		assert( md.m21 == REAL(0.4) && approx(md.m22, REAL(0.333333)) && approx(md.m23, REAL(0.285714)) && md.m24 == REAL(0.25) );
		assert( approx(md.m31, REAL(0.2222222)) && md.m32 == REAL(0.2) && approx(md.m33, REAL(0.18181818)) && approx(md.m34, REAL(0.16666667)) );
		assert( approx(md.m41, REAL(0.153846)) && approx(md.m42, REAL(0.142857)) && approx(md.m43, REAL(0.13333333)) && md.m44 == REAL(0.125) );
		m1 /= REAL(2.0);
		md = m1.getMatrixData();		
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) && md.m14 == REAL(2.0) );
		assert( md.m21 == REAL(2.5) && md.m22 == REAL(3.0) && md.m23 == REAL(3.5) && md.m24 == REAL(4.0) );
		assert( md.m31 == REAL(4.5) && md.m32 == REAL(5.0) && md.m33 == REAL(5.5) && md.m34 == REAL(6.0) );
		assert( md.m41 == REAL(6.5) && md.m42 == REAL(7.0) && md.m43 == REAL(7.5) && md.m44 == REAL(8.0) );

		// Scalar Division test #2 ////////////////////
		m1 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
					 REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
					 REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
					 REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));
		r = m1 / REAL(1.0);
		assert( r == m1 );
		m1 /= REAL(1.0);
		md = m1.getMatrixData();		
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) && md.m14 == REAL(4.0) );
		assert( md.m21 == REAL(5.0) && md.m22 == REAL(6.0) && md.m23 == REAL(7.0) && md.m24 == REAL(8.0) );
		assert( md.m31 == REAL(9.0) && md.m32 == REAL(10.0) && md.m33 == REAL(11.0) && md.m34 == REAL(12.0) );
		assert( md.m41 == REAL(13.0) && md.m42 == REAL(14.0) && md.m43 == REAL(15.0) && md.m44 == REAL(16.0) );

		// Scalar Division test #3 ////////////////////
		m1 = Mat4(REAL(1.0), REAL(0.0), REAL(3.0), REAL(-4.0), 
					 REAL(4.0), REAL(0.0), REAL(6.0), REAL(0.0),
					 REAL(1.0), REAL(8.0), REAL(9.0), REAL(-1.0),
					 REAL(1.5), REAL(-2.0), REAL(-0.5), REAL(0.5));
		r = m1 / REAL(0.5);
		md = r.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) && md.m14 == REAL(-8.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) && md.m24 == REAL(0.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) && md.m34 == REAL(-2.0) );
		assert( md.m41 == REAL(3.0) && md.m42 == REAL(-4.0) && md.m43 == REAL(-1.0) && md.m44 == REAL(1.0) );
		assert( m1 == Mat4(REAL(1.0), REAL(0.0), REAL(3.0), REAL(-4.0), 
								 REAL(4.0), REAL(0.0), REAL(6.0), REAL(0.0),
								 REAL(1.0), REAL(8.0), REAL(9.0), REAL(-1.0),
								 REAL(1.5), REAL(-2.0), REAL(-0.5), REAL(0.5)) );
		m1 /= REAL(0.5);
		assert( m1 == r );
		md = m1.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) && md.m14 == REAL(-8.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) && md.m24 == REAL(0.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) && md.m34 == REAL(-2.0) );
		assert( md.m41 == REAL(3.0) && md.m42 == REAL(-4.0) && md.m43 == REAL(-1.0) && md.m44 == REAL(1.0) );

		
		FINISH_TEST;
	}
	
	void testMat4MatrixMultiplication() {
		BEGIN_TEST;
		
		// Identity multiplication tests ///////////////
		Mat4 m1(Mat4::IDENTITY);
		Mat4 m2(Mat4::IDENTITY);
		Mat4 r = m1 * m2;
		assert( r == Mat4::IDENTITY );
		r = m1 * Mat4::IDENTITY;
		assert( r == Mat4::IDENTITY );
		r = m2 * Mat4::IDENTITY;
		assert( r == Mat4::IDENTITY );
		r = m1 * m1;
		assert( r == Mat4::IDENTITY );	
		m2 = Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
					 REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
					 REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
					 REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0));
		r = m1 * m2;
		assert( r == m2 );			
		assert( r == Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
								REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
								REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
								REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0)) );
		r = m2 * m1;
		assert( r == m2 );
		assert( r == Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
								REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
								REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
								REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0)) );
		r = m2 * Mat4::IDENTITY;
		assert( r == m2 );
		r = Mat4::IDENTITY * m2;
		assert( r == m2 );
		assert( m1 == Mat4::IDENTITY );
		m1 *= m2;
		assert( m1 == Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
								REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
								REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
								REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0)) );
		m1 = Mat4::IDENTITY;
		m2 *= m1;
		assert( m2 == Mat4(REAL(1.0), REAL(2.0), REAL(3.0), REAL(4.0), 
								REAL(5.0), REAL(6.0), REAL(7.0), REAL(8.0),
								REAL(9.0), REAL(10.0), REAL(11.0), REAL(12.0),
								REAL(13.0), REAL(14.0), REAL(15.0), REAL(16.0)) );
		
		// Matrix Multiplication test #2 //////////////////
		m1 = Mat4(REAL(1.0), REAL(5.0), REAL(9.0), REAL(13.0), 
					 REAL(2.0), REAL(6.0), REAL(10.0), REAL(14.0),
					 REAL(3.0), REAL(7.0), REAL(11.0), REAL(15.0), 
					 REAL(4.0), REAL(8.0), REAL(12.0), REAL(16.0));
		m2 = Mat4(REAL(9.0), REAL(1.0), REAL(9.0), REAL(1.0),
					 REAL(2.0), REAL(2.0), REAL(2.0), REAL(-2.0),
					 REAL(0.0), REAL(4.0), REAL(5.0), REAL(-6.0),
					 REAL(-1.0), REAL(8.0), REAL(-7.0), REAL(11.0));
		
		r = m1 * m2;
		Mat4::MatrixData md = r.getMatrixData();
		assert( md.m11 == REAL(42.0) && md.m12 == REAL(122.0) && md.m13 == REAL(202.0) && md.m14 == REAL(282.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(20.0) && md.m23 == REAL(36.0) && md.m24 == REAL(52.0) );
		assert( md.m31 == REAL(-1.0) && md.m32 == REAL(11.0) && md.m33 == REAL(23.0) && md.m34 == REAL(35.0) );
		assert( md.m41 == REAL(38.0) && md.m42 == REAL(82.0) && md.m43 == REAL(126.0) && md.m44 == REAL(170.0) );
		r = m2 * m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(6.0) && md.m12 == REAL(151.0) && md.m13 == REAL(-27.0) && md.m14 == REAL(80.0) );
		assert( md.m21 == REAL(16.0) && md.m22 == REAL(166.0) && md.m23 == REAL(-18.0) && md.m24 == REAL(84.0) );
		assert( md.m31 == REAL(26.0) && md.m32 == REAL(181.0) && md.m33 == REAL(-9.0) && md.m34 == REAL(88.0) );
		assert( md.m41 == REAL(36.0) && md.m42 == REAL(196.0) && md.m43 == REAL(0.0) && md.m44 == REAL(92.0) );
		m1 *= m2;
		md = m1.getMatrixData();
		assert( md.m11 == REAL(42.0) && md.m12 == REAL(122.0) && md.m13 == REAL(202.0) && md.m14 == REAL(282.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(20.0) && md.m23 == REAL(36.0) && md.m24 == REAL(52.0) );
		assert( md.m31 == REAL(-1.0) && md.m32 == REAL(11.0) && md.m33 == REAL(23.0) && md.m34 == REAL(35.0) );
		assert( md.m41 == REAL(38.0) && md.m42 == REAL(82.0) && md.m43 == REAL(126.0) && md.m44 == REAL(170.0) );
		m1 = Mat4(REAL(1.0), REAL(5.0), REAL(9.0), REAL(13.0), 
					 REAL(2.0), REAL(6.0), REAL(10.0), REAL(14.0),
					 REAL(3.0), REAL(7.0), REAL(11.0), REAL(15.0), 
					 REAL(4.0), REAL(8.0), REAL(12.0), REAL(16.0));
		m2 *= m1;
		md = m2.getMatrixData();
		assert( md.m11 == REAL(6.0) && md.m12 == REAL(151.0) && md.m13 == REAL(-27.0) && md.m14 == REAL(80.0) );
		assert( md.m21 == REAL(16.0) && md.m22 == REAL(166.0) && md.m23 == REAL(-18.0) && md.m24 == REAL(84.0) );
		assert( md.m31 == REAL(26.0) && md.m32 == REAL(181.0) && md.m33 == REAL(-9.0) && md.m34 == REAL(88.0) );
		assert( md.m41 == REAL(36.0) && md.m42 == REAL(196.0) && md.m43 == REAL(0.0) && md.m44 == REAL(92.0) );
		

		// Matrix Multiplication test #3 ///////////////
		m1 = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0), 
					 REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					 REAL(0.0), REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0),
					 REAL(1.0), REAL(2.0), REAL(3.0), REAL(1.0));
		r = m1 * m1;
		assert(r.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(-1.0), REAL(0.0), REAL(0.0), 
									REAL(2.0), REAL(1.292893), REAL(6.535534), REAL(1.0))) );
		m2 = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
					 REAL(0.0), REAL(0.70710678118655), REAL(-0.70710678118655), REAL(0.0), 
					 REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0),
					 REAL(-1.0), REAL(-3.53553), REAL(-0.70710678118655), REAL(1.0));

		r = m1 * m2;
		
		assert(r.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0))) );
		r = m2 * m1;
		assert(r.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0))) );
		m1 *= m2;
		assert(m1.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									 REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									 REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									 REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0))) );
		m1 = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0), 
					 REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					 REAL(0.0), REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0),
					 REAL(1.0), REAL(2.0), REAL(3.0), REAL(1.0));
		m2 *= m1;
		assert(m2.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									 REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									 REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									 REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0))) );
		

		FINISH_TEST;
	}

	void testMat4VectorMultiplicationNoTranslation() {
		BEGIN_TEST;
		
	   Mat4 m;		
		Vec4 v(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0));		
		Vec4 vr;
		Vec3 v3(REAL(1.0), REAL(1.0), REAL(1.0));
		Vec3 vr3;
		
		
		// Rotation 45 degrees around x axis.
		m = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0), 
					REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					REAL(0.0), REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));	
		vr = m * v;
		assert( vr.approx(Vec4(1.0, 0.0, 1.414214, 1.0)) );
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.0, 0.0, 1.414214)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around x axis.
		m = m * m;		
		vr = m * v;
		assert(vr.approx(Vec4(1.0, -1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.0, -1.0, 1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around x axis.
		m *= m;
		m *= m;
		assert(m.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec4(1.0, 1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );	
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.0, 1.0, 1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// Rotation 45 degrees around y axis.
		m = Mat4(REAL(0.70710678118655), REAL(0.0), REAL(-0.70710678118655), REAL(0.0), 
					REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
					REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655), REAL(0.0),
					REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));	
		vr = m * v;
		assert( vr.approx(Vec4(1.414214, 1.0, 0.0, 1.0)) );
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.414214, 1.0, 0.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around y axis.
		m = m*m;  
		vr = m * v;
		assert(vr.approx(Vec4(1.0, 1.0, -1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.0, 1.0, -1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around y axis.
		m *= m;
		m *= m;
		assert(m.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec4(1.0, 1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.0, 1.0, 1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// Rotation 45 degrees around z axis.
		m = Mat4(REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), REAL(0.0),
					REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0), REAL(0.0),
					REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
					REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));	
		vr = m * v;
		assert( vr.approx(Vec4(0.0, 1.414214, 1.0, 1.0)) );
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(0.0, 1.414214, 1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around z axis.
		m = m*m;
		vr = m * v;
		assert(vr.approx(Vec4(-1.0, 1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(-1.0, 1.0, 1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around z axis.
		m *= m;
		m *= m;
		assert(m.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec4(1.0, 1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.0, 1.0, 1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// Rotation 45 degrees around x, y and z axis'. (1i + 2k + 3j)
		m = Mat4(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), REAL(0.0), 
					REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), REAL(0.0), 
					REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790), REAL(0.0),
					REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));
		vr = m * v;
		assert( vr.approx(Vec4(0.64365, 1.336123, 0.894702, 1.0)) );
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );		
		vr3 = m * v3;
		assert( vr3.approx(Vec3(0.64365, 1.336123, 0.894702)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around xyz axis'.
		m = m*m;  	
		vr = m * v;
		assert(vr.approx(Vec4(0.16131, 1.391665, 1.018453, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );	
		vr3 = m * v3;
		assert( vr3.approx(Vec3(0.16131, 1.391665, 1.018453)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around xyz axis'.
		m *= m;
		m *= m;
		assert(m.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec4(1.0, 1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );	
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.0, 1.0, 1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		FINISH_TEST;
	}	
	
	void testMat4VectorMultiplicationWithTranslation() {
		BEGIN_TEST;
		
	   Mat4 m;		
		Vec4 v(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0));		
		Vec4 vr;
		Vec3 v3(REAL(1.0), REAL(1.0), REAL(1.0));
		Vec3 vr3;
		
		
		// Rotation 45 degrees around x axis.
		m = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0), 
					REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					REAL(0.0), REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					REAL(1.0), REAL(2.0), REAL(3.0), REAL(1.0));	
		vr = m * v;
		assert( vr.approx(Vec4(2.0, 2.0, 4.414214, 1.0)) );
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(2.0, 2.0, 4.414214)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around x axis.
		m = m * m;		
		vr = m * v;
		assert(vr.approx(Vec4(3.0, 0.2928932, 7.53553390, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(3.0, 0.2928932, 7.53553390)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around x axis.
		m *= m;
		m *= m;
		assert(m.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(8.0), REAL(0.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec4(9.0, 1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );	
		vr3 = m * v3;
		assert( vr3.approx(Vec3(9.0, 1.0, 1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );


		// Rotation 45 degrees around y axis.
		m = Mat4(REAL(0.70710678118655), REAL(0.0), REAL(-0.70710678118655), REAL(0.0), 
					REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
					REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655), REAL(0.0),
					REAL(-1.0), REAL(2.0), REAL(3.0), REAL(1.0));	
		vr = m * v;
		assert( vr.approx(Vec4(0.414214, 3.0, 3.0, 1.0)) );
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(0.414214, 3.0, 3.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around y axis.
		m = m*m;  
		vr = m * v;
		assert(vr.approx(Vec4(1.414214, 5.0, 4.828427, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.414214, 5.0, 4.828427)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around y axis.
		m *= m;
		m *= m;
		assert(m.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(16.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec4(1.0, 17.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.0, 17.0, 1.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );


		// Rotation 45 degrees around z axis.
		m = Mat4(REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), REAL(0.0),
					REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0), REAL(0.0),
					REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
					REAL(-1.0), REAL(-2.0), REAL(3.0), REAL(1.0));	
		vr = m * v;
		assert( vr.approx(Vec4(-1.0, -0.5857864, 4.0, 1.0)) );
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(-1.0, -0.5857864, 4.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around z axis.
		m = m*m;
		vr = m * v;
		assert(vr.approx(Vec4(-1.2928932, -3.121320, 7.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(-1.2928932, -3.121320, 7.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around z axis.
		m *= m;
		m *= m;
		assert(m.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(24.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec4(1.0, 1.0, 25.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );
		vr3 = m * v3;
		assert( vr3.approx(Vec3(1.0, 1.0, 25.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );


		// Rotation 45 degrees around x, y and z axis'. (1i + 2k + 3j)
		m = Mat4(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), REAL(0.0), 
					REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), REAL(0.0), 
					REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790), REAL(0.0),
					REAL(-1.0), REAL(-2.0), REAL(-3.0), REAL(1.0));
		vr = m * v;
		assert( vr.approx(Vec4(-0.35634979, -0.66387742, -2.10529846, 1.0)) );
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );		
		vr3 = m * v3;
		assert( vr3.approx(Vec3(-0.35634979, -0.66387742, -2.10529846)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around xyz axis'.
		m = m*m;  	
		vr = m * v;
		assert(vr.approx(Vec4(-1.8386898, -2.60833466, -4.981547, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );	
		vr3 = m * v3;
		assert( vr3.approx(Vec3(-1.8386898, -2.60833466, -4.981547)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around xyz axis'.
		m *= m;
		m *= m;
		assert(m.approx(Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(-8.0), REAL(-16.0), REAL(-24.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec4(-7.0, -15.0, -23.0, 1.0)) );	
		vr = v;
		assert( vr == Vec4(REAL(1.0), REAL(1.0), REAL(1.0), REAL(1.0)) );	
		vr3 = m * v3;
		assert( vr3.approx(Vec3(-7.0, -15.0, -23.0)) );
		vr3 = v3;
		assert( vr3 == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		FINISH_TEST;
	}	

	void testMat4Get3x3Matrix() {
		BEGIN_TEST;
		
	   Mat4 m;		
		Mat3 m3x3;

		// Rotation 45 degrees around x axis.
		m = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0), 
					REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					REAL(0.0), REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					REAL(1.0), REAL(2.0), REAL(3.0), REAL(1.0));	
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
										 REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), 
										 REAL(0.0), REAL(-0.70710678118655), REAL(0.70710678118655))) );		
		// 90 Degree rotation around x axis.
		m = m * m;		
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
										 REAL(0.0), REAL(0.0), REAL(1.0), 
										 REAL(0.0), REAL(-1.0), REAL(0.0))) );
		// 360 Degree rotation around x axis.
		m *= m;
		m *= m;
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
										 REAL(0.0), REAL(1.0), REAL(0.0), 
										 REAL(0.0), REAL(0.0), REAL(1.0))) );


		// Rotation 45 degrees around y axis.
		m = Mat4(REAL(0.70710678118655), REAL(0.0), REAL(-0.70710678118655), REAL(0.0), 
					REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
					REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655), REAL(0.0),
					REAL(-1.0), REAL(2.0), REAL(3.0), REAL(1.0));	
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(0.70710678118655), REAL(0.0), REAL(-0.70710678118655), 
										 REAL(0.0), REAL(1.0), REAL(0.0), 
										 REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655))) );	
		// 90 Degree rotation around y axis.
		m = m*m;  
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(0.0), REAL(0.0), REAL(-1.0), 
										 REAL(0.0), REAL(1.0), REAL(0.0), 
										 REAL(1.0), REAL(0.0), REAL(0.0))) );
		// 360 Degree rotation around y axis.
		m *= m;
		m *= m;
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
										 REAL(0.0), REAL(1.0), REAL(0.0), 
										 REAL(0.0), REAL(0.0), REAL(1.0))) );


		// Rotation 45 degrees around z axis.
		m = Mat4(REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), REAL(0.0),
					REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0), REAL(0.0),
					REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
					REAL(-1.0), REAL(-2.0), REAL(3.0), REAL(1.0));	
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
										 REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
										 REAL(0.0), REAL(0.0), REAL(1.0))) );	
		// 90 Degree rotation around z axis.
		m = m*m;
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(0.0), REAL(1.0), REAL(0.0), 
										 REAL(-1.0), REAL(0.0), REAL(0.0), 
										 REAL(0.0), REAL(0.0), REAL(1.0))) );
		// 360 Degree rotation around z axis.
		m *= m;
		m *= m;
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
										 REAL(0.0), REAL(1.0), REAL(0.0), 
										 REAL(0.0), REAL(0.0), REAL(1.0))) );


		// Rotation 45 degrees around x, y and z axis'. (1i + 2k + 3j)
		m = Mat4(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), REAL(0.0), 
					REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), REAL(0.0), 
					REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790), REAL(0.0),
					REAL(-1.0), REAL(-2.0), REAL(-3.0), REAL(1.0));
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), 
										 REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), 
										 REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790))) );	
		// 90 Degree rotation around xyz axis'.
		m = m*m;  	
		m3x3 = m.get3x3Matrix();		
		assert( m3x3.approx(Mat3(REAL(0.071428571461), REAL(0.944640868585), REAL(-0.320236769556), 
										 REAL(-0.658926582874), REAL(0.285714285745), REAL(0.695832670486), 
										 REAL(0.748808198115), REAL(0.161310186648), REAL(0.642857142864))) );	
		// 360 Degree rotation around xyz axis'.
		m *= m;
		m *= m;
		m3x3 = m.get3x3Matrix();
		assert( m3x3.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
										 REAL(0.0), REAL(1.0), REAL(0.0), 
										 REAL(0.0), REAL(0.0), REAL(1.0))) );
		FINISH_TEST;
	}	

	void testMat4GetQuaternion() {
		BEGIN_TEST;
		
		// Rotation 45 degrees around x axis.
		Mat4 m(1.0, 0.0, 0.0, 0.0,
				 0.0, 0.70710678118655, -0.70710678118655, 0.0,
				 0.0, 0.70710678118655, 0.70710678118655, 0.0,
				 1.0, 2.0, 3.0, 1.0);
		Quaternion q = m.getQuaternion();
	   assert(q.approx(Quaternion(REAL(0.38268343236509), REAL(0.0), REAL(0.0), REAL(0.923879532511287))) );	
		// 90 Degree rotation around x axis.
		m = Mat4(1.0, 0.0, 0.0, 0.0,
					0.0, 0.0, -1.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					-1.0, 0.0, 2.0, 1.0);
		q = m.getQuaternion();
		assert( q.approx(Quaternion(REAL(0.70710678118655), REAL(0.0), REAL(0.0), REAL(0.70710678118655))) );
		// 360 Degree rotation around x axis.	
		m = Mat4(1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, -2.0, 3.0, 1.0);	
		q = m.getQuaternion();
		assert( q.approx(Quaternion(0.0, 0.0, 0.0, 1.0)) );

		// Rotation 45 degrees around y axis.
	   m = Mat4(0.70710678118655, 0.0, 0.70710678118655, 0.0,
					0.0, 1.0, 0.0, 0.0,
					-0.70710678118655, 0.0, 0.70710678118655, 0.0,
					0.0, 0.0, -5.0, 1.0);
		q = m.getQuaternion();
		assert(q.approx(Quaternion(REAL(0.0), REAL(0.38268343236509), REAL(0.0), REAL(0.923879532511287))) );

		// Rotation 45 degrees around z axis.
		m = Mat4(0.70710678118655, -0.70710678118655, 0.0, 0.0,
					0.70710678118655, 0.70710678118655, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					1.0, -4.0, 0.0, 1.0);
		q = m.getQuaternion();
		assert(q.approx(Quaternion(REAL(0.0), REAL(0.0), REAL(0.38268343236509), REAL(0.923879532511287))) );

		// Rotation 45 degrees around xyz axis'.
	   m = Mat4(0.8047378541, -0.3106172175, 0.5058793634, 0.0,
					0.5058793634, 0.8047378541, -0.3106172175, 0.0,
					-0.3106172175, 0.5058793634, 0.8047378541, 0.0,
					0.0, 0.0, 0.0, 1.0);
		q = m.getQuaternion();
		assert(q.approx(Quaternion(REAL(0.22094238269), REAL(0.22094238269), REAL(0.22094238269), REAL(0.923879532511))) );
		
		FINISH_TEST;
	}
	
}

int main(int argc, char** argv) {
	cc::testBasicMat4CreationAndAssignment();
	cc::testMat4Equality();
	cc::testMat4ApproximateEquality();
	cc::testMatrixTransposition();
	cc::testMatrixDeterminant();
	cc::testMatrixInversion();
	cc::testMat4Addition();
	cc::testMat4Subtraction();
	cc::testMat4ScalarMultiplication();
	cc::testMat4ScalarDivision();
	cc::testMat4MatrixMultiplication();
	cc::testMat4VectorMultiplicationNoTranslation();
	cc::testMat4VectorMultiplicationWithTranslation();
	cc::testMat4Get3x3Matrix();
	cc::testMat4GetQuaternion();
	
	return 0;
}

