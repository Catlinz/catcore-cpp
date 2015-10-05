#ifndef CX_CORE_MATH_CXMAT3_H
#define CX_CORE_MATH_CXMAT3_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxMat3.h
 * @brief Contains the definition of the 3x3 matrix class.
 *
 * @author Catlin Zilinski
 * @date Oct 5, 2015
 */

#include "core/Cx.h"
#include "core/math/CxMath.h"
#include "core/math/CxVec3.h"
#include "core/math/CxQuat.h"

namespace cat {

	/**
	 * @class CxMat3 CxMat3.h "core/math/CxMat3.h"
	 * @brief A 3x3 matrix class and the operators for it.
	 * 
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Dec 4, 2013
	 */
	class CxMat3 {
	  public:
		CxVec3 col0, col1, col2;

		/** @brief Initialise the matrix to garbage */
		CX_FORCE_INLINE CxMat3() {}

		/** @brief Initialise the matrix to all zeros */
		CX_INLINE CxMat3(CxZero)
			: col0(CxZero), col1(CxZero), col2(CxZero) {}

		/** @brief Initialise to identity matrix. */
		CX_INLINE CxMat3(CxIdentity)
			: col0(1,0,0), col1(0,1,0), col2(0,0,1) {}

		/** 
		 * @brief Initialise the matrix from an array 
		 * The array is laid out in column major format, so the first three
		 * values are the x,y,z of the first column of the matrix, etc.
		 */
		explicit CX_INLINE CxMat3(const CxReal *in_src)
			: col0(in_arr[0], in_arr[1], in_arr[2]),
			  col1(in_arr[3], in_arr[4], in_arr[5]),
			  col2(in_arr[6], in_arr[7], in_arr[8]) {}

		/** @brief Initialise from a unit (rotation) quaternion */
		explicit CxMat3(const CxQuat &in_q);
		
		/** @brief Initialise from three column (basis) vectors. */
		CX_INLINE CxMat3(const CxVec3 &in_col0, const CxVec3& in_col1,
									  const CxVec3 &in_col2)
			: col0(in_col0), col1(in_col1), col2(in_col2) {}

		/** 
		 * @brief Initialise all the components of the matrix. 
		 * Specified in column major format so m00, m01, m02 is the 
		 * first column of the matrix and so on.
		 */
		CX_INLINE CxMat3(CxReal in_m00, CxReal in_m01, CxReal in_m02,
									  CxReal in_m10, CxReal in_m11, CxReal in_m12,
									  CxReal in_m20, CxReal in_m21, CxReal in_m22)
			: col0(in_m00, in_m01, in_m02), col1(in_m10, in_m11, in_m12),
			  col2(in_m20, in_m21, in_m22) {}

		/** @brief Standard copy constructor */
		CX_FORCE_INLINE CxMat3(const CxMat3 &in_src)
			: col0(in_src.col0), col1(in_src.col1), col2(in_src.col2) {}

		/** @brief Standard assignment operator */
		CX_FORCE_INLINE CxMat3 & operator=(const CxMat3 &in_m) {
			col0 = in_m.col0;  col1 = in_m.col1;  col2 = in_m.col2;
			return *this;
		}

		/** @return True if the two matrices are strictly equal */
		CX_INLINE CxBool operator==(const CxMat3 &in_m) const {
			return (col0 == in_m.col0 && col1 == in_m.col1 && col2 == in_m.col2);
		}

		/** @return True if the two matrices are strictly NOT equal */
		CX_INLINE CxBool operator!=(const CxMat3 &in_m) const {
			return (col0 != in_m.col0 || col1 != in_m.col1 || col2 != in_m.col2);
		}

		/** 
		 * @brief Array access to the elements of the matrix. 
		 * The matrix is accessed in column major format so the first three entries are 
		 * the first column of the matrix, etc.
		 */
		CX_FORCE_INLINE CxReal & operator[](CxI32 in_idx) { return (&col0)[in_idx]; }
		CX_FORCE_INLINE const CxReal operator[](CxI32 in_idx) const { return (&col0)[in_idx]; }

		/** @brief Access an element of the matrix by its column and row. */
		CX_FORCE_INLINE CxReal & operator()(CxI32 in_col, CxI32 in_row) {
			return (&col0)[in_col*3 + in_row];
		}
		CX_FORCE_INLINE const CxReal operator()(CxI32 in_col, CxI32 in_row) const {
			return (&col0)[in_col*3 + in_row];
		}

		/** @return A copy of the matrix will all entries negated. */
		CX_INLINE CxMat3 operator-() const { return CxMat3(-col0, -col1, -col2); }

		/** @return The result of the matrix addition this + m */
		CX_INLINE CxMat3 operator+(const CxMat3 &in_m) const {
			return CxMat3(col0 + in_m.col0, col1 + in_m.col1, col2 + in_m.col2);
		}

		/** @return The result of the matrix subtraction this - m */
		CX_INLINE CxMat3 operator-(const CxMat3 &in_m) const {
			return CxMat3(col0 - in_m.col0, col1 - in_m.col1, col2 - in_m.col2);
		}

		/** @return The result of multiplying this matrix by a scalar */
		CX_INLINE CxMat3 operator*(CxReal in_s) const {
			return CxMat3(col0*in_s, col1*in_s, col2*in_s);
		}

		/** @return The result r of the vector v transformed by this matrix (r = M*v) */
		CxVec3 operator*(const CxVec3 &in_v) const;

		/** @return The result of the matrix multiplication (this * m) */
		CxMat3 operator*(const CxMat3 &in_m) const;

		/** @return The result of dividing this matrix by a scalar */
		CX_INLINE CxMat3 operator/(CxReal in_s) const {
			const CxReal r_s = 1.0f/in_s;
			return CxMat3(col0*r_s, col1*r_s, col2*r_s);
		}

		/** @brief Add another matrix to this matrix */
		CX_INLINE CxMat3 & operator+=(const CxMat3 &in_m) {
			col0 += in_m.col0;  col1 += in_m.col1;  col2 += in_m.col2;
			return *this;
		}

		/** @brief Subtract another matrix from this matrix */
		CX_INLINE CxMat3 & operator-=(const CxMat3 &in_m) {
			col0 -= in_m.col0;  col1 -= in_m.col1;  col2 -= in_m.col2;
			return *this;
		}

		/** @brief Multiply this matrix by a scalar */
		CX_INLINE CxMat3 & operator*=(CxReal in_s) {
			col0 *= in_s;  col1 *= in_s;  col2 *= in_s;
			return *this;
		}

		/** @brief Multiply this matrix by another matrix */
		CxMat3 & operator*=(const CxMat3 &in_m);

		/** @brief Divide this matrix by a scalar */
		CX_INLINE CxMat3 & operator/=(CxReal in_s) {
			const CxReal r_s = 1.0f/in_s;
			col0 *= r_s;  col1 *= r_s;  col2 *= r_s;
			return *this;
		}
		
		/** @return The matrix as a column major array stored as col0,col1,col2. */
		CX_FORCE_INLINE const CxReal * constData() const { return &(col0.x); }
		CX_FORCE_INLINE CxReal * data() { return &(col0.x); }
		CX_FORCE_INLINE const CxReal * data() const { return &(col0.x); }

		/** @return The determinant for an invertable matrix */
		CxReal determinant() const;

		/** @return The inverse of the matrix */
		CxMat3 inverse() const;
		
	};

	CX_FORCE_INLINE CxVec3 CxMat3::operator*(const CxVec3 &in_v) const {
		const CxReal x = in_v.x;  const CxReal y = in_v.y;  const CxReal z = in_v.z;
		return CxVec3(col0.x*x + col1.x*y + col2.x*z,
						  col0.y*x + col1.y*y + col2.y*z,
						  col0.z*x + col1.z*y + col2.z*z);
	}

	CX_FORCE_INLINE CxMat3 CxMat3::operator*(const CxMat3 &in_m) const {
		const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y; const CxReal m02 = in_m.col0.z;
		const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y; const CxReal m12 = in_m.col1.z;
		const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y; const CxReal m22 = in_m.col2.z;

		const CxReal c0x = col0.x;  const CxReal c1x = col1.x;  const CxReal c2x = col2.x;
		const CxReal c0y = col0.y;  const CxReal c1y = col1.y;  const CxReal c2y = col2.y;
		
	   const CxReal n00 = (c0x*m00) + (c1x*m01) + (c2x*m02);
		const CxReal n10 = (c0x*m10) + (c1x*m11) + (c2x*m12);
		const CxReal n20 = (c0x*m20) + (c1x*m21) + (c2x*m22);

		const CxReal c0z = col0.z;  const CxReal c1z = col1.z;  const CxReal c2z = col2.z;

	   const CxReal n01 = (c0y*m00) + (c1y*m01) + (c2y*m02);
	   const CxReal n11 = (c0y*m10) + (c1y*m11) + (c2y*m12);
	   const CxReal n21 = (c0y*m20) + (c1y*m21) + (c2y*m22);

		const CxReal n02 = (c0z*m00) + (c1z*m01) + (c2z*m02);
		const CxReal n12 = (c0z*m10) + (c1z*m11) + (c2z*m12);
		const CxReal n22 = (c0z*m20) + (c1z*m21) + (c2z*m22);

		return CxMat3(n00, n01, n02, n10, n11, n12, n20, n21, n22);
	}

	CX_FORCE_INLINE CxMat3 & CxMat3::operator*=(const CxMat3 &in_m) {
		const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y; const CxReal m02 = in_m.col0.z;
		const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y; const CxReal m12 = in_m.col1.z;
		const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y; const CxReal m22 = in_m.col2.z;

		const CxReal c0x = col0.x;  const CxReal c1x = col1.x;  const CxReal c2x = col2.x;
		const CxReal c0y = col0.y;  const CxReal c1y = col1.y;  const CxReal c2y = col2.y;
		
	   col0.x = (c0x*m00) + (c1x*m01) + (c2x*m02);
	   col1.x = (c0x*m10) + (c1x*m11) + (c2x*m12);
		col2.x = (c0x*m20) + (c1x*m21) + (c2x*m22);

		const CxReal c0z = col0.z;  const CxReal c1z = col1.z;  const CxReal c2z = col2.z;

	   col0.y = (c0y*m00) + (c1y*m01) + (c2y*m02);
	   col1.y = (c0y*m10) + (c1y*m11) + (c2y*m12);
	   col2.y = (c0y*m20) + (c1y*m21) + (c2y*m22);

		col0.z = (c0z*m00) + (c1z*m01) + (c2z*m02);
		col1.z = (c0z*m10) + (c1z*m11) + (c2z*m12);
		col2.z = (c0z*m20) + (c1z*m21) + (c2z*m22);

		return *this;
	}
	
} // namespace cat

#endif // CX_CORE_MATH_CXMAT3_H
