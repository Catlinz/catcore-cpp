#include <assert.h>
#include <ctime>
#include <cstdio>
#include <iostream>
#include "core/math/mat3.h"
#ifndef DEBUG
#define DEBUG 1
#endif

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)


namespace cc {

	void testBasicMat3CreationAndAssignment() {
		BEGIN_TEST;

		Mat3 m1;
		m1 = Mat3::IDENTITY;
		
		assert(m1 == Mat3::IDENTITY);
		Mat3::MatrixData& m1d = m1.getMatrixData();
		assert( m1d.m11 == REAL(1.0) && m1d.m12 == REAL(0.0) && m1d.m13 == REAL(0.0) );
		assert( m1d.m21 == REAL(0.0) && m1d.m22 == REAL(1.0) && m1d.m23 == REAL(0.0) );
		assert( m1d.m31 == REAL(0.0) && m1d.m32 == REAL(0.0) && m1d.m33 == REAL(1.0) );
		assert( m1d.m[0] == REAL(1.0) && m1d.m[1] == REAL(0.0) && m1d.m[2] == REAL(0.0) );
		assert( m1d.m[3] == REAL(0.0) && m1d.m[4] == REAL(1.0) && m1d.m[5] == REAL(0.0) );
		assert( m1d.m[6] == REAL(0.0) && m1d.m[7] == REAL(0.0) && m1d.m[8] == REAL(1.0) );

		// Per component constructor
		m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
					 REAL(4.0), REAL(5.0), REAL(6.0),
					 REAL(7.0), REAL(8.0), REAL(9.0));
		m1d = m1.getMatrixData();	
		assert( m1d.m11 == REAL(1.0) && m1d.m12 == REAL(2.0) && m1d.m13 == REAL(3.0) );
		assert( m1d.m21 == REAL(4.0) && m1d.m22 == REAL(5.0) && m1d.m23 == REAL(6.0) );
		assert( m1d.m31 == REAL(7.0) && m1d.m32 == REAL(8.0) && m1d.m33 == REAL(9.0) );
		assert( m1d.m[0] == REAL(1.0) && m1d.m[1] == REAL(2.0) && m1d.m[2] == REAL(3.0) );
		assert( m1d.m[3] == REAL(4.0) && m1d.m[4] == REAL(5.0) && m1d.m[5] == REAL(6.0) );
		assert( m1d.m[6] == REAL(7.0) && m1d.m[7] == REAL(8.0) && m1d.m[8] == REAL(9.0) );

		// Basic vectors constructor
		Mat3 m2(Vec3(REAL(0.8047378541), REAL(0.5058793634), REAL(-0.3106172175)),
				  Vec3(REAL(-0.3106172175), REAL(0.8047378541), REAL(0.5058793634)),
				  Vec3(REAL(0.5058793634), REAL(-0.3106172175), REAL(0.8047378541)));
		Mat3::MatrixData& m2d = m2.getMatrixData();	
		assert( m2d.m11 == REAL(0.8047378541) && m2d.m12 == REAL(-0.3106172175) && m2d.m13 == REAL(0.5058793634) );
		assert( m2d.m21 == REAL(0.5058793634) && m2d.m22 == REAL(0.8047378541) && m2d.m23 == REAL(-0.3106172175) );
		assert( m2d.m31 == REAL(-0.3106172175) && m2d.m32 == REAL(0.5058793634) && m2d.m33 == REAL(0.8047378541) );

		// Copy constructor
		Mat3 m3(m2);
		Mat3::MatrixData& m3d = m3.getMatrixData();	
		assert( m3d.m11 == REAL(0.8047378541) && m3d.m12 == REAL(-0.3106172175) && m3d.m13 == REAL(0.5058793634) );
		assert( m3d.m21 == REAL(0.5058793634) && m3d.m22 == REAL(0.8047378541) && m3d.m23 == REAL(-0.3106172175) );
		assert( m3d.m31 == REAL(-0.3106172175) && m3d.m32 == REAL(0.5058793634) && m3d.m33 == REAL(0.8047378541) );

		// Assignment
		m3 = m1;		
		assert( m3d.m11 == REAL(1.0) && m3d.m12 == REAL(2.0) && m3d.m13 == REAL(3.0) );
		assert( m3d.m21 == REAL(4.0) && m3d.m22 == REAL(5.0) && m3d.m23 == REAL(6.0) );
		assert( m3d.m31 == REAL(7.0) && m3d.m32 == REAL(8.0) && m3d.m33 == REAL(9.0) );

		// Single value constructor
		m1 = Mat3::IDENTITY;
		assert( m1 == Mat3::IDENTITY );
		m1 = Mat3(REAL(3.14));	
		assert( m1d.m11 == REAL(3.14) && m1d.m12 == REAL(3.14) && m1d.m13 == REAL(3.14) );
		assert( m1d.m21 == REAL(3.14) && m1d.m22 == REAL(3.14) && m1d.m23 == REAL(3.14) );
		assert( m1d.m31 == REAL(3.14) && m1d.m32 == REAL(3.14) && m1d.m33 == REAL(3.14) );

		// Array value constructor
		Real cols[9] = { 
			REAL(-1.0), REAL(-2.0), REAL(-3.0), 
			REAL(-4.0), REAL(-5.0), REAL(-6.0),
			REAL(-7.0), REAL(-8.0), REAL(-9.0)
		};
		
		m1 = Mat3::IDENTITY;
		assert( m1 == Mat3::IDENTITY );
		m1 = Mat3(cols);
		assert( m1d.m11 == REAL(-1.0) && m1d.m12 == REAL(-2.0) && m1d.m13 == REAL(-3.0) );
		assert( m1d.m21 == REAL(-4.0) && m1d.m22 == REAL(-5.0) && m1d.m23 == REAL(-6.0) );
		assert( m1d.m31 == REAL(-7.0) && m1d.m32 == REAL(-8.0) && m1d.m33 == REAL(-9.0) );

		FINISH_TEST;
		
	}

	void testMat3Equality() {
		BEGIN_TEST;
		
		Mat3 m1(Mat3::IDENTITY);
		assert( m1 == Mat3::IDENTITY && Mat3::IDENTITY == m1 && 
				  !(m1 != Mat3::IDENTITY) && !(Mat3::IDENTITY != m1) );
		assert( m1 == m1 && !(m1 != m1) );
		assert( Mat3::IDENTITY == Mat3::IDENTITY && !(Mat3::IDENTITY != Mat3::IDENTITY) );
		
		
		Mat3 m2 = Mat3::IDENTITY;
		assert( m1 == m2 && !(m1 != m2) );
		
		m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
					 REAL(4.0), REAL(5.0), REAL(6.0),
					 REAL(7.0), REAL(8.0), REAL(9.0));
		assert( m1 != Mat3::IDENTITY && !(m1 == Mat3::IDENTITY) );
		assert( m1 != m2 && !(m1 == m2) );

		m2 = m1;
		assert( m1 == m2 && !(m1 != m2) );
		Mat3::MatrixData& md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(5.0) && md.m23 == REAL(6.0) );
		assert( md.m31 == REAL(7.0) && md.m32 == REAL(8.0) && md.m33 == REAL(9.0) );
		
		// Check to make sure the equality is checking every component
		md.m11 = REAL(-1.0);
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

		md.m21 = REAL(-4.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m21 = REAL(4.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		md.m22 = REAL(-5.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m22 = REAL(5.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		md.m23 = REAL(-6.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m23 = REAL(6.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		md.m31 = REAL(-7.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m31 = REAL(7.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		md.m32 = REAL(-8.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m32 = REAL(8.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		md.m33 = REAL(-9.0);
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m33 = REAL(9.0);
		assert( m1 == m2 && !(m1 != m2) && m1.approx(m2) && m2.approx(m1) );

		FINISH_TEST;	
	}

	void testMat3ApproximateEquality() {
		BEGIN_TEST;
		
		Mat3 m1(Mat3::IDENTITY);
		assert( m1.approx(Mat3::IDENTITY) && !(m1 != Mat3::IDENTITY) );
		assert( m1.approx(m1) && Mat3::IDENTITY.approx(m1) && Mat3::IDENTITY.approx(Mat3::IDENTITY) );
		
		Mat3 m2 = Mat3::IDENTITY;
		assert( m1.approx(m2) && m2.approx(m1) && !(m1 != m2) );
		
		m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
					 REAL(4.0), REAL(5.0), REAL(6.0),
					 REAL(7.0), REAL(8.0), REAL(9.0));
		assert( !m1.approx(Mat3::IDENTITY) && !Mat3::IDENTITY.approx(m1) );
		assert( !m1.approx(m2) && !m2.approx(m1) );

		m2 = m1;
		assert( m1.approx(m2) && m2.approx(m1) && !(m1 != m2) );
		Mat3::MatrixData& md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(5.0) && md.m23 == REAL(6.0) );
		assert( md.m31 == REAL(7.0) && md.m32 == REAL(8.0) && md.m33 == REAL(9.0) );
		
		// Check to make sure the approximate equality is checking every component
		md.m11 += (Mat3::EPSILON / REAL(2.0));		
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m11 += Mat3::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m11 -= Mat3::EPSILON;	  

		md.m12 += (Mat3::EPSILON / REAL(2.0));		
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m12 += Mat3::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m12 -= Mat3::EPSILON;

		md.m13 += (Mat3::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m13 += Mat3::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m13 -= Mat3::EPSILON;

		md.m21 += (Mat3::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m21 += Mat3::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m21 -= Mat3::EPSILON;

		md.m22 += (Mat3::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m22 += Mat3::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m22 -= Mat3::EPSILON;

		md.m23 += (Mat3::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m23 += Mat3::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m23 -= Mat3::EPSILON;

		md.m31 += (Mat3::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m31 += Mat3::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m31 -= Mat3::EPSILON;

		md.m32 += (Mat3::EPSILON / REAL(2.0));	
		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m32 += Mat3::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m32 -= Mat3::EPSILON;

		md.m33 += (Mat3::EPSILON / REAL(2.0));	
 		assert( m1 != m2 && !(m1 == m2) && m1.approx(m2) && m2.approx(m1) );
		md.m33 += Mat3::EPSILON;
		assert( m1 != m2 && !(m1 == m2) && !m1.approx(m2) && !m2.approx(m1) );
		md.m33 -= Mat3::EPSILON;

		FINISH_TEST;	
	}

	void testMatrixTransposition() {
		BEGIN_TEST;
		
		Mat3 m1(Mat3::IDENTITY);
		Mat3 r = m1.transposed();
		assert( r == m1 && r == Mat3::IDENTITY );
		
		m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
					 REAL(4.0), REAL(5.0), REAL(6.0),
					 REAL(7.0), REAL(8.0), REAL(9.0));

		r = m1.transposed();
		Mat3::MatrixData md = r.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(4.0) && md.m13 == REAL(7.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(5.0) && md.m23 == REAL(8.0) );
		assert( md.m31 == REAL(3.0) && md.m32 == REAL(6.0) && md.m33 == REAL(9.0) );
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(5.0) && md.m23 == REAL(6.0) );
		assert( md.m31 == REAL(7.0) && md.m32 == REAL(8.0) && md.m33 == REAL(9.0) );
		m1.transpose();
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(4.0) && md.m13 == REAL(7.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(5.0) && md.m23 == REAL(8.0) );
		assert( md.m31 == REAL(3.0) && md.m32 == REAL(6.0) && md.m33 == REAL(9.0) );
		m1.transpose();
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(5.0) && md.m23 == REAL(6.0) );
		assert( md.m31 == REAL(7.0) && md.m32 == REAL(8.0) && md.m33 == REAL(9.0) );

		FINISH_TEST;
	}

		void testMatrixDeterminant() {
		BEGIN_TEST;
		
		// Determinant test # 1
		Mat3 m1(Mat3::IDENTITY);
		Real det = m1.determinant();
		assert( det == REAL(1.0) );
		
		// Determinant test # 2
	   m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
					 REAL(4.0), REAL(5.0), REAL(6.0),
					 REAL(7.0), REAL(8.0), REAL(9.0));
		det = m1.determinant();
		assert( det == REAL(0.0) );
		m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
					 REAL(2.0), REAL(4.0), REAL(6.0),
					 REAL(3.0), REAL(6.0), REAL(9.0));
		det = m1.determinant();
		assert( det == REAL(0.0) );

		// Determinant test # 3
		m1 = Mat3(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), 
					 REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), 
					 REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790));
		det = m1.determinant();
		assert( det == REAL(1.0) );

		// Determinant test # 4
		m1 = Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
					 REAL(0.0), REAL(-2.0), REAL(-2.0), 
					 REAL(3.0), REAL(0.0), REAL(4.0));
		det = m1.determinant();
		assert( det == REAL(-8.0) );

		FINISH_TEST;
		}
	
	void testMatrixInversion() {
		BEGIN_TEST;
		
		// Inversion test # 1
		Mat3 m(Mat3::IDENTITY);
		Mat3 r = m.inverse();
		assert( m == r );
		
		// Inverse test # 2
		m = Mat3(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), 
					REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), 
					REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790));
	   r = m.inverse();
		assert(r.approx(Mat3(0.7280277254, -0.5251048211, 0.44072730561,
									0.6087885979, 0.7907905580, -0.06345657130, 
									-0.31520164041, 0.31450790171, 0.8953952790)) );
		assert(m == Mat3(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), 
							  REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), 
							  REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790)) );
		m.invert();
		assert(m.approx(Mat3(0.7280277254, -0.5251048211, 0.44072730561,
									0.6087885979, 0.7907905580, -0.06345657130, 
									-0.31520164041, 0.31450790171, 0.8953952790)) );

		r = m * Mat3(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), 
						 REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), 
						 REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790));
		assert( r.approx(Mat3::IDENTITY) );
		m = Mat3(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), 
					REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), 
					REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790));
		r = m * m.inverse();
		assert( r.approx(Mat3::IDENTITY) );
		
		

		// Inverse test # 3
		m = Mat3(REAL(1.0), REAL(0.0), REAL(3.0), 
					REAL(0.0), REAL(-2.0), REAL(0.0), 
					REAL(0.0), REAL(-2.0), REAL(4.0));
		r = m.inverse();
		assert(r.approx(Mat3(1.0, 0.75, -0.75,
									0.0, -0.5, 0.0, 
									0.0, 0.-0.25, 0.25)) );
		assert(m == Mat3(REAL(1.0), REAL(0.0), REAL(3.0), 
							  REAL(0.0), REAL(-2.0), REAL(0.0), 
							  REAL(0.0), REAL(-2.0), REAL(4.0)) );
		m.invert();
		assert(m.approx(Mat3(1.0, 0.75, -0.75,
									0.0, -0.5, 0.0, 
									0.0, 0.-0.25, 0.25)) );

		r = m * Mat3(REAL(1.0), REAL(0.0), REAL(3.0), 
						 REAL(0.0), REAL(-2.0), REAL(0.0), 
						 REAL(0.0), REAL(-2.0), REAL(4.0));
		assert( r.approx(Mat3::IDENTITY) );

		m = Mat3(REAL(1.0), REAL(0.0), REAL(3.0), 
					REAL(0.0), REAL(-2.0), REAL(0.0), 
					REAL(0.0), REAL(-2.0), REAL(4.0));
		r = m * m.inverse();
		assert( r.approx(Mat3::IDENTITY) );
		m *= m.inverse();
		assert( m.approx(Mat3::IDENTITY) );
		

		FINISH_TEST;
	}

	void testMatrixXYZAxes() {
		BEGIN_TEST;
		
		Mat3 m(Mat3::IDENTITY);
		Vec3 x = m.xAxis();
		Vec3 y = m.yAxis();
		Vec3 z = m.zAxis();		
		assert( x == Vec3::X_AXIS );
		assert( y == Vec3::Y_AXIS );
		assert( z == Vec3::Z_AXIS );

		m = Mat3(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), 
					REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), 
					REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790));
		x = m.xAxis();
		y = m.yAxis();
		z = m.zAxis();	
		assert( x.approx(Vec3(0.7280277254, -0.5251048211, 0.44072730561)) );
		assert( y.approx(Vec3(0.6087885979, 0.7907905580, -0.06345657130)) );
		assert( z.approx(Vec3(-0.31520164041, 0.31450790171, 0.8953952790)) );

		FINISH_TEST;		
	}
	
	

	void testMat3Addition() {
		BEGIN_TEST;
		
		// Addition test #1 ////////////////
		Mat3 m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
							REAL(4.0), REAL(5.0), REAL(6.0),
							REAL(7.0), REAL(8.0), REAL(9.0));

		Mat3 m2 = Mat3(REAL(0.5), REAL(1.0), REAL(1.5), 
							REAL(2.0), REAL(2.5), REAL(3.0),
							REAL(3.5), REAL(4.0), REAL(4.5));
		Mat3 r = m1 + m2;
		Mat3::MatrixData md = r.getMatrixData();	
		assert( md.m11 == REAL(1.5) && md.m12 == REAL(3.0) && md.m13 == REAL(4.5) );
		assert( md.m21 == REAL(6.0) && md.m22 == REAL(7.5) && md.m23 == REAL(9.0) );
		assert( md.m31 == REAL(10.5) && md.m32 == REAL(12.0) && md.m33 == REAL(13.5) );
		Mat3 r2 = m2 + m1;
		assert( r == r2 && !(r != r2 ) );
		m2 += m1;
		md = m2.getMatrixData();	
		assert( md.m11 == REAL(1.5) && md.m12 == REAL(3.0) && md.m13 == REAL(4.5) );
		assert( md.m21 == REAL(6.0) && md.m22 == REAL(7.5) && md.m23 == REAL(9.0) );
		assert( md.m31 == REAL(10.5) && md.m32 == REAL(12.0) && md.m33 == REAL(13.5) );
		m2 = Mat3(REAL(0.5), REAL(1.0), REAL(1.5), 
							REAL(2.0), REAL(2.5), REAL(3.0),
							REAL(3.5), REAL(4.0), REAL(4.5));
		m1 += m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(1.5) && md.m12 == REAL(3.0) && md.m13 == REAL(4.5) );
		assert( md.m21 == REAL(6.0) && md.m22 == REAL(7.5) && md.m23 == REAL(9.0) );
		assert( md.m31 == REAL(10.5) && md.m32 == REAL(12.0) && md.m33 == REAL(13.5) );
		
		// Addition test #2 /////////////////
		m1 = Mat3(REAL(-0.5), REAL(0.0), REAL(0.0), 
					 REAL(-2.0), REAL(-1.5), REAL(-10.0),
					 REAL(0.0), REAL(0.0), REAL(0.0));
		r = m1 + m2;
		md = r.getMatrixData();	
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(0.0) && md.m22 == REAL(1.0) && md.m23 == REAL(-7.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );		
		r2 = m2 + m1;
		assert( r2 == r && !(r != r2) );
		m1 += m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(0.0) && md.m22 == REAL(1.0) && md.m23 == REAL(-7.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );	
		m1 = Mat3(REAL(-0.5), REAL(0.0), REAL(0.0), 
					 REAL(-2.0), REAL(-1.5), REAL(-10.0),
					 REAL(0.0), REAL(0.0), REAL(0.0));
		m2 += m1;
		md = m2.getMatrixData();	
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(0.0) && md.m22 == REAL(1.0) && md.m23 == REAL(-7.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );

		// Addition test #3 ////////////////////
		m2 = Mat3(REAL(0.0));
		r = m1 + m2;
		assert( r == m1 );
		r2 = m2 + m1;
		assert( r2 == m1 );
		m1 += m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(-0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) );
		assert( md.m21 == REAL(-2.0) && md.m22 == REAL(-1.5) && md.m23 == REAL(-10.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) );
		m2 += m1;
		assert( m2 == m1 );
		
		// Addition test #4 ///////////////////
		m2 = Mat3(REAL(0.0));
		m2 += Mat3(REAL(-0.5), REAL(0.0), REAL(0.0), 
					 REAL(-2.0), REAL(-1.5), REAL(-10.0),
					 REAL(0.0), REAL(0.0), REAL(0.0));
		assert( m1 == m2 && m2 == m1 );

		FINISH_TEST;
	}
	
	void testMat3Subtraction() {
		BEGIN_TEST;
		
		// Subtraction test #1 ////////////////
		Mat3 m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
							REAL(4.0), REAL(5.0), REAL(6.0),
							REAL(7.0), REAL(8.0), REAL(9.0));

		Mat3 m2 = Mat3(REAL(0.5), REAL(1.0), REAL(1.5), 
							REAL(2.0), REAL(2.5), REAL(3.0),
							REAL(3.5), REAL(4.0), REAL(4.5));
		Mat3 r = m1 - m2;
		Mat3::MatrixData md = r.getMatrixData();	
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(2.5) && md.m23 == REAL(3.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );
		Mat3 r2 = m2 - m1;
		assert( r != r2 && !(r == r2 ) );
		md = r2.getMatrixData();
		assert( md.m11 == REAL(-0.5) && md.m12 == REAL(-1.0) && md.m13 == REAL(-1.5) );
		assert( md.m21 == REAL(-2.0) && md.m22 == REAL(-2.5) && md.m23 == REAL(-3.0) );
		assert( md.m31 == REAL(-3.5) && md.m32 == REAL(-4.0) && md.m33 == REAL(-4.5) );
		m2 -= m1;
		md = m2.getMatrixData();	
		assert( md.m11 == REAL(-0.5) && md.m12 == REAL(-1.0) && md.m13 == REAL(-1.5) );
		assert( md.m21 == REAL(-2.0) && md.m22 == REAL(-2.5) && md.m23 == REAL(-3.0) );
		assert( md.m31 == REAL(-3.5) && md.m32 == REAL(-4.0) && md.m33 == REAL(-4.5) );
		m2 = Mat3(REAL(0.5), REAL(1.0), REAL(1.5), 
							REAL(2.0), REAL(2.5), REAL(3.0),
							REAL(3.5), REAL(4.0), REAL(4.5));
		m1 -= m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(2.5) && md.m23 == REAL(3.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );
		
		// Subtraction test #2 /////////////////
		m1 = Mat3(REAL(-0.5), REAL(0.0), REAL(0.0), 
					 REAL(-2.0), REAL(-1.5), REAL(-10.0),
					 REAL(0.0), REAL(0.0), REAL(0.0));
		r = m1 - m2;
		md = r.getMatrixData();	
		assert( md.m11 == REAL(-1.0) && md.m12 == REAL(-1.0) && md.m13 == REAL(-1.5) );
		assert( md.m21 == REAL(-4.0) && md.m22 == REAL(-4.0) && md.m23 == REAL(-13.0) );
		assert( md.m31 == REAL(-3.5) && md.m32 == REAL(-4.0) && md.m33 == REAL(-4.5) );		
		r2 = m2 - m1;
		assert( r2 != r && !(r == r2) );
		md = r2.getMatrixData();
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(4.0) && md.m23 == REAL(13.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );	
		m1 -= m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(-1.0) && md.m12 == REAL(-1.0) && md.m13 == REAL(-1.5) );
		assert( md.m21 == REAL(-4.0) && md.m22 == REAL(-4.0) && md.m23 == REAL(-13.0) );
		assert( md.m31 == REAL(-3.5) && md.m32 == REAL(-4.0) && md.m33 == REAL(-4.5) );	
		m1 = Mat3(REAL(-0.5), REAL(0.0), REAL(0.0), 
					 REAL(-2.0), REAL(-1.5), REAL(-10.0),
					 REAL(0.0), REAL(0.0), REAL(0.0));
		m2 -= m1;
		md = m2.getMatrixData();	
		assert( r2 != r && !(r == r2) );
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(4.0) && md.m23 == REAL(13.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );	

		// Subtraction test #3 ////////////////////
		m2 = Mat3(REAL(0.0));
		r = m1 - m2;
		assert( r == m1 );
		r2 = m2 - m1;
		assert( r2 != m1 );
		md = r2.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(1.5) && md.m23 == REAL(10.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) );
		m1 -= m2;
		md = m1.getMatrixData();	
		assert( md.m11 == REAL(-0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) );
		assert( md.m21 == REAL(-2.0) && md.m22 == REAL(-1.5) && md.m23 == REAL(-10.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) );
		m2 -= m1;
		assert( m2 != m1 );
		md = m2.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(1.5) && md.m23 == REAL(10.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) );
		
		// Subtraction test #4 ///////////////////
		m2 = Mat3(REAL(0.0));
		m2 -= Mat3(REAL(-0.5), REAL(0.0), REAL(0.0), 
					  REAL(-2.0), REAL(-1.5), REAL(-10.0),
					  REAL(0.0), REAL(0.0), REAL(0.0));
		assert( m1 != m2 && m2 != m1 );
		md = m2.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(1.5) && md.m23 == REAL(10.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) );

		FINISH_TEST;
	}

		
	void testMat3ScalarMultiplication() {
		BEGIN_TEST;
		
		// Scalar Multiplication test #1 ////////////////
		Mat3 m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
							REAL(4.0), REAL(5.0), REAL(6.0),
							REAL(7.0), REAL(8.0), REAL(9.0));

		Mat3 r = m1 * REAL(0.5);
		Mat3::MatrixData md = r.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(2.5) && md.m23 == REAL(3.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );
		r = REAL(0.5) * m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(2.5) && md.m23 == REAL(3.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );
		m1 *= REAL(0.5);
		md = m1.getMatrixData();		
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(2.5) && md.m23 == REAL(3.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );

		// Scalar Multiplication test #2 ////////////////////
		m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
					 REAL(4.0), REAL(5.0), REAL(6.0),
					 REAL(7.0), REAL(8.0), REAL(9.0));
		r = m1 * REAL(1.0);
		assert( r == m1 );
		m1 *= REAL(1.0);
		md = m1.getMatrixData();		
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(5.0) && md.m23 == REAL(6.0) );
		assert( md.m31 == REAL(7.0) && md.m32 == REAL(8.0) && md.m33 == REAL(9.0) );

		// Scalar Multiplication test #3 ////////////////////
		m1 = Mat3(REAL(1.0), REAL(0.0), REAL(3.0), 
					 REAL(4.0), REAL(0.0), REAL(6.0),
					 REAL(1.0), REAL(8.0), REAL(9.0));
		r = m1 * REAL(2.0);
		md = r.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) );
		r = REAL(2.0) * m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) );
		assert( m1 != r );		
		m1 *= REAL(2.0);
		assert( m1 == r );
		md = m1.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) );

		// Scalar mutliplication test #4 /////////////////////
		m1 = Mat3(REAL(1.0), REAL(0.0), REAL(3.0), 
					 REAL(4.0), REAL(0.0), REAL(-6.0),
					 REAL(1.0), REAL(8.0), REAL(9.0));
		r = m1 * 0.0;
		md = r.getMatrixData();
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) );
		assert( md.m21 == REAL(0.0) && md.m22 == REAL(0.0) && md.m23 == REAL(0.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) );
		r = 0.0 * m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) );
		assert( md.m21 == REAL(0.0) && md.m22 == REAL(0.0) && md.m23 == REAL(0.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) );
		md = m1.getMatrixData();
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(0.0) && md.m13 == REAL(3.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(0.0) && md.m23 == REAL(-6.0) );
		assert( md.m31 == REAL(1.0) && md.m32 == REAL(8.0) && md.m33 == REAL(9.0) );
		m1 *= 0.0;
		md = m1.getMatrixData();
		assert( md.m11 == REAL(0.0) && md.m12 == REAL(0.0) && md.m13 == REAL(0.0) );
		assert( md.m21 == REAL(0.0) && md.m22 == REAL(0.0) && md.m23 == REAL(0.0) );
		assert( md.m31 == REAL(0.0) && md.m32 == REAL(0.0) && md.m33 == REAL(0.0) );
	
		FINISH_TEST;
	}
	
	void testMat3ScalarDivision() {
		BEGIN_TEST;
		
		// Scalar Division test #1 ////////////////
		Mat3 m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
							REAL(4.0), REAL(5.0), REAL(6.0),
							REAL(7.0), REAL(8.0), REAL(9.0));

		Mat3 r = m1 / REAL(2.0);
		Mat3::MatrixData md = r.getMatrixData();
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(2.5) && md.m23 == REAL(3.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );
		r = REAL(2.0) / m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(1.0) && approx(md.m13, REAL(0.666667)) );
		assert( md.m21 == REAL(0.5) && md.m22 == REAL(0.4) && approx(md.m23, REAL(0.3333333)) );
		assert( approx(md.m31, REAL(0.2857142)) && md.m32 == REAL(0.25) && approx(md.m33, REAL(0.222222222)) );
		m1 /= REAL(2.0);
		md = m1.getMatrixData();		
		assert( md.m11 == REAL(0.5) && md.m12 == REAL(1.0) && md.m13 == REAL(1.5) );
		assert( md.m21 == REAL(2.0) && md.m22 == REAL(2.5) && md.m23 == REAL(3.0) );
		assert( md.m31 == REAL(3.5) && md.m32 == REAL(4.0) && md.m33 == REAL(4.5) );

		// Scalar Division test #2 ////////////////////
		m1 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
							REAL(4.0), REAL(5.0), REAL(6.0),
							REAL(7.0), REAL(8.0), REAL(9.0));
		r = m1 / REAL(1.0);
		assert( r == m1 );
		m1 /= REAL(1.0);
		md = m1.getMatrixData();		
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(5.0) && md.m23 == REAL(6.0) );
		assert( md.m31 == REAL(7.0) && md.m32 == REAL(8.0) && md.m33 == REAL(9.0) );

		// Scalar Division test #3 ////////////////////
		m1 = Mat3(REAL(1.0), REAL(0.0), REAL(3.0), 
							REAL(4.0), REAL(0.0), REAL(6.0),
							REAL(1.0), REAL(8.0), REAL(9.0));
		r = m1 / REAL(0.5);
		md = r.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) );
		assert( m1 != r );		
		m1 /= REAL(0.5);
		assert( m1 == r );
		md = m1.getMatrixData();
		assert( md.m11 == REAL(2.0) && md.m12 == REAL(0.0) && md.m13 == REAL(6.0) );
		assert( md.m21 == REAL(8.0) && md.m22 == REAL(0.0) && md.m23 == REAL(12.0) );
		assert( md.m31 == REAL(2.0) && md.m32 == REAL(16.0) && md.m33 == REAL(18.0) );
	
		FINISH_TEST;
	}

	void testMat3MatrixMultiplication() {
		BEGIN_TEST;
		
		// Identity multiplication tests ///////////////
		Mat3 m1(Mat3::IDENTITY);
		Mat3 m2(Mat3::IDENTITY);
		Mat3 r = m1 * m2;
		assert( r == Mat3::IDENTITY );
		r = m1 * Mat3::IDENTITY;
		assert( r == Mat3::IDENTITY );
		r = m2 * Mat3::IDENTITY;
		assert( r == Mat3::IDENTITY );
		r = m1 * m1;
		assert( r == Mat3::IDENTITY );	
		m2 = Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
					 REAL(4.0), REAL(5.0), REAL(6.0),
					 REAL(7.0), REAL(8.0), REAL(9.0));
		r = m1 * m2;
		assert( r == m2 );		
		Mat3::MatrixData md = r.getMatrixData();		
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(5.0) && md.m23 == REAL(6.0) );
		assert( md.m31 == REAL(7.0) && md.m32 == REAL(8.0) && md.m33 == REAL(9.0) );
		r = m2 * m1;
		assert( r == m2 );
		md = r.getMatrixData();		
		assert( md.m11 == REAL(1.0) && md.m12 == REAL(2.0) && md.m13 == REAL(3.0) );
		assert( md.m21 == REAL(4.0) && md.m22 == REAL(5.0) && md.m23 == REAL(6.0) );
		assert( md.m31 == REAL(7.0) && md.m32 == REAL(8.0) && md.m33 == REAL(9.0) );
		r = m2 * Mat3::IDENTITY;
		assert( r == m2 );
		r = Mat3::IDENTITY * m2;
		assert( r == m2 );
		assert( m1 == Mat3::IDENTITY );
		m1 *= m2;
		assert( m1 == Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
								 REAL(4.0), REAL(5.0), REAL(6.0),
								 REAL(7.0), REAL(8.0), REAL(9.0)) );
		m1 = Mat3::IDENTITY;
		m2 *= m1;
		assert( m2 == Mat3(REAL(1.0), REAL(2.0), REAL(3.0), 
								 REAL(4.0), REAL(5.0), REAL(6.0),
								 REAL(7.0), REAL(8.0), REAL(9.0)) );
		
		
		// Matrix Multiplication test #2 //////////////////
		m1 = Mat3(REAL(1.0), REAL(4.0), REAL(7.0), 
					 REAL(2.0), REAL(5.0), REAL(8.0), 
					 REAL(3.0), REAL(6.0), REAL(9.0));
		m2 = Mat3(REAL(9.0), REAL(1.0), REAL(9.0),
					 REAL(2.0), REAL(2.0), REAL(2.0),
					 REAL(0.0), REAL(4.0), REAL(5.0));
		
		r = m1 * m2;
		md = r.getMatrixData();
		assert( md.m11 == REAL(38.0) && md.m12 == REAL(95.0) && md.m13 == REAL(152.0) );
		assert( md.m21 == REAL(12.0) && md.m22 == REAL(30.0) && md.m23 == REAL(48.0) );
		assert( md.m31 == REAL(23.0) && md.m32 == REAL(50.0) && md.m33 == REAL(77.0) );
		r = m2 * m1;
		md = r.getMatrixData();
		assert( md.m11 == REAL(17.0) && md.m12 == REAL(37.0) && md.m13 == REAL(52.0) );
		assert( md.m21 == REAL(28.0) && md.m22 == REAL(44.0) && md.m23 == REAL(68.0) );
		assert( md.m31 == REAL(39.0) && md.m32 == REAL(51.0) && md.m33 == REAL(84.0) );
		m1 *= m2;
		md = m1.getMatrixData();
		assert( md.m11 == REAL(38.0) && md.m12 == REAL(95.0) && md.m13 == REAL(152.0) );
		assert( md.m21 == REAL(12.0) && md.m22 == REAL(30.0) && md.m23 == REAL(48.0) );
		assert( md.m31 == REAL(23.0) && md.m32 == REAL(50.0) && md.m33 == REAL(77.0) );
		m1 = Mat3(REAL(1.0), REAL(4.0), REAL(7.0), 
					 REAL(2.0), REAL(5.0), REAL(8.0), 
					 REAL(3.0), REAL(6.0), REAL(9.0));
		m2 *= m1;
		md = m2.getMatrixData();
		assert( md.m11 == REAL(17.0) && md.m12 == REAL(37.0) && md.m13 == REAL(52.0) );
		assert( md.m21 == REAL(28.0) && md.m22 == REAL(44.0) && md.m23 == REAL(68.0) );
		assert( md.m31 == REAL(39.0) && md.m32 == REAL(51.0) && md.m33 == REAL(84.0) );
		

		// Matrix Multiplication test #3 ///////////////
		m1 = Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
					 REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), 
					 REAL(0.0), REAL(-0.70710678118655), REAL(0.70710678118655));
		r = m1 * m1;
		assert(r.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0),
									REAL(0.0), REAL(-1.0), REAL(0.0))) );
		m2 = Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
					 REAL(0.0), REAL(0.70710678118655), REAL(-0.70710678118655), 
					 REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655));

		r = m1 * m2;
		assert(r.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0))) );
		r = m2 * m1;
		assert(r.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0))) );
		m1 *= m2;
		assert(m1.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0),
									 REAL(0.0), REAL(1.0), REAL(0.0),
									 REAL(0.0), REAL(0.0), REAL(1.0))) );
		m1 = Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
					 REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), 
					 REAL(0.0), REAL(-0.70710678118655), REAL(0.70710678118655));
		m2 *= m1;
		assert(m2.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0),
									 REAL(0.0), REAL(1.0), REAL(0.0),
									 REAL(0.0), REAL(0.0), REAL(1.0))) );
		

		FINISH_TEST;
	}

	void testMatrixVectorMultiplication() {
		BEGIN_TEST;
		
	   Mat3 m;		
		Vec3 v(REAL(1.0), REAL(1.0), REAL(1.0));		
		Vec3 vr;
		
		// Rotation 45 degrees around x axis.
		m = Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
					REAL(0.0), REAL(0.70710678118655), REAL(0.70710678118655), 
					REAL(0.0), REAL(-0.70710678118655), REAL(0.70710678118655));	
		vr = m * v;
		assert( vr.approx(Vec3(1.0, 0.0, 1.414214)) );
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around x axis.
		m = m * m;		
		vr = m * v;
		assert(vr.approx(Vec3(1.0, -1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around x axis.
		m *= m;
		m *= m;
		assert(m.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec3(1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );	
		
		// Rotation 45 degrees around y axis.
		m = Mat3(REAL(0.70710678118655), REAL(0.0), REAL(-0.70710678118655), 
					REAL(0.0), REAL(1.0), REAL(0.0), 
					REAL(0.70710678118655), REAL(0.0), REAL(0.70710678118655));	
		vr = m * v;
		assert( vr.approx(Vec3(1.414214, 1.0, 0.0)) );
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around y axis.
		m = m*m;  
		vr = m * v;
		assert(vr.approx(Vec3(1.0, 1.0, -1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around y axis.
		m *= m;
		m *= m;
		assert(m.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec3(1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
	
		// Rotation 45 degrees around z axis.
		m = Mat3(REAL(0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					REAL(-0.70710678118655), REAL(0.70710678118655), REAL(0.0), 
					REAL(0.0), REAL(0.0), REAL(1.0));	
		vr = m * v;
		assert( vr.approx(Vec3(0.0, 1.414214, 1.0)) );
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 90 Degree rotation around z axis.
		m = m*m;
		vr = m * v;
		assert(vr.approx(Vec3(-1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );
		// 360 Degree rotation around z axis.
		m *= m;
		m *= m;
		assert(m.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec3(1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );

		// Rotation 45 degrees around x, y and z axis'. (1i + 2k + 3j)
		m = Mat3(REAL(0.7280277254), REAL(0.6087885979), REAL(-0.31520164041), 
					REAL(-0.5251048211), REAL(0.7907905580), REAL(0.31450790171), 
					REAL(0.44072730561), REAL(-0.06345657130), REAL(0.8953952790));

		vr = m * v;
		assert( vr.approx(Vec3(0.64365, 1.336123, 0.894702)) );
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );		
		// 90 Degree rotation around xyz axis'.
		m = m*m;  	
		vr = m * v;
		assert(vr.approx(Vec3(0.16131, 1.391665, 1.018453)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );	
		// 360 Degree rotation around xyz axis'.
		m *= m;
		m *= m;
		assert(m.approx(Mat3(REAL(1.0), REAL(0.0), REAL(0.0),
									REAL(0.0), REAL(1.0), REAL(0.0),
									REAL(0.0), REAL(0.0), REAL(1.0))) );  // Should have rotated all the way back to 0
		vr = m * v;
		assert(vr.approx(Vec3(1.0, 1.0, 1.0)) );	
		vr = v;
		assert( vr == Vec3(REAL(1.0), REAL(1.0), REAL(1.0)) );	

		FINISH_TEST;
	}	

}

int main(int argc, char** argv) {
	cc::testBasicMat3CreationAndAssignment();
	cc::testMat3Equality();
	cc::testMat3ApproximateEquality();
	cc::testMatrixTransposition();
	cc::testMatrixDeterminant();
	cc::testMatrixInversion();
	cc::testMatrixXYZAxes();
	cc::testMat3Addition();
	cc::testMat3Subtraction();
	cc::testMat3ScalarMultiplication();
	cc::testMat3ScalarDivision();
	cc::testMat3MatrixMultiplication();
	cc::testMatrixVectorMultiplication();
	
	
	
	return 0;
}

