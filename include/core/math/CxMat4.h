#ifndef CX_CORE_MATH_CXMAT4_H
#define CX_CORE_MATH_CXMAT4_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxMat4x4.h
 * @brief Contains the definition of a 4x4 column major matrix.
 *
 * @author Catlin Zilinski
 * @date June 2, 2015
 */

#include "core/math/CxMath.h"
#include "core/math/CxVec4.h"
#include "core/math/CxMat3.h"

namespace cat {

	class CxMat4 {
	  public:
		CxVec4 col0, col1, col2, col3;  /**< Columns of the matrix */

		/** @brief Initialise the matrix to garbage */
		CX_FORCE_INLINE CxMat4() {}

		/** @brief Initialise the matrix to all zeros. */
		CX_INLINE CxMat4(CxZero)
			: col0(CxZero), col1(CxZero), col2(CxZero), col3(CxZero) {}

		/** @brief Initialise to the identity matrix. */
		CX_INLINE CxMat4(CxIdentity)
			: col0(1, 0, 0, 0), col1(0, 1, 0, 0),
			  col2(0, 0, 1, 0), col3(0, 0, 0, 1) {}

		/** @brief Initialise the matrix from an array. */
		explicit CX_INLINE CxMat4(const CxReal *in_arr)
			: col0(in_arr[0],  in_arr[1],  in_arr[2],  in_arr[3]),
			  col1(in_arr[4],  in_arr[5],  in_arr[6],  in_arr[7]),
			  col2(in_arr[8],  in_arr[9],  in_arr[10], in_arr[11]),
			  col3(in_arr[12], in_arr[13], in_arr[14], in_arr[15]) {}

		/**
		 * @brief Initialise from a 3x3 rotation matrix (set rest to identity).
		 * @param in_m The 3x3 rotation matrix to set this matrix from.
		 */
		explicit CX_INLINE CxMat4(const CxMat3 &in_r)
			: col0(in_r.col0, 0.0f), col1(in_r.col1, 0.0f), col2(in_r.col2, 0.0f),
			  col3(0.0f, 0.0f, 0.0f, 1.0f) {}

		/** 
		 * @brief Initialise the matrix from a quaternion.
		 * @param in_q The quaternion to initialise the matrix from.
		 */
		CX_FORCE_INLINE explicit CxMat4(const CxQuat &in_q) { setQuat(in_q); }
		
		/**
		 * @brief Initialise from a 3x3 rotation matrix and a position vector.
		 * @param in_r The 3x3 rotation matrix to set this matrix from.
		 * @param in_p The position vector to set the fourth column from.
		 */
		CX_INLINE explicit CxMat4(const CxMat3 &in_r, const CxVec3 &in_p)
			: col0(in_r.col0, 0.0f), col1(in_r.col1, 0.0f), col2(in_r.col2, 0.0f),
			  col3(in_p, 1.0f) {}

		/**
		 * @brief Initialise the columns of the matrix.
		 * @param in_c0 The first column of the matrix.
		 * @param in_c1 The second column of the matrix.
		 * @param in_c2 The third column of the matrix.
		 * @param in_c3 The fourth column of the matrix.
		 */
		CX_INLINE CxMat4(const CxVec4 &in_c0, const CxVec4 &in_c1,
								 const CxVec4 &in_c2, const CxVec4 &in_c3)
			: col0(in_c0), col1(in_c1), col2(in_c2), col3(in_c3) {}

		/**
		 * @brief Initialise from basis vectors and translation vector.
		 * @param in_x The first column (x-axis) of the matrix.
		 * @param in_y The second column (y-axis) of the matrix.
		 * @param in_z The third column (z-axis) of the matrix.
		 * @param in_t The fourth column (translation) of the matrix.
		 */
		CX_INLINE CxMat4(const CxVec3 &in_x, const CxVec3 &in_y,
								 const CxVec3 &in_z, const CxVec3 &in_t)
			: col0(in_x, 0.0f), col1(in_y, 0.0f), col2(in_z, 0.0f), in_c3(in_t, 1.0f) {}

		/** 
		 * @brief Initialise all the components of the matrix. 
		 * Specified in column major format so m00, m01, m02, m03 is the 
		 * first column of the matrix and so on.
		 */
		CX_INLINE CxMat4(CxReal in_m00, CxReal in_m01, CxReal in_m02, CxReal in_m03,
								 CxReal in_m10, CxReal in_m11, CxReal in_m12, CxReal in_m13,
								 CxReal in_m20, CxReal in_m21, CxReal in_m22, CxReal in_m23,
								 CxReal in_m30, CxReal in_m31, CxReal in_m32, CxReal in_m33)
			: col0(in_m00, in_m01, in_m02, in_m03), col1(in_m10, in_m11, in_m12, in_m13),
			  col2(in_m20, in_m21, in_m22, in_m23), col3(in_m30, in_m31, in_m32, in_m33) {}

		/** @brief Copy constructor */
		CX_INLINE CxMat4(const CxMat4 &in_m)
			: col0(in_m.col0), col1(in_m.col1), col2(in_m.col2), col3(in_m.col3) {}

		/** @brief Assignment operator */
		CX_INLINE CxMat4 & operator=(const CxMat4 &in_m) {
			col0 = in_m.col0; col1 = in_m.col1;
			col2 = in_m.col2; col3 = in_m.col3;
			return *this;
		}

		/** @return True if the two matrices are strictly equal. */
		CX_INLINE CxBool operator==(const CxMat4 &in_m) const {
			return ((col0 == in_m.col0) && (col1 == in_m.col1) &&
					  (col2 == in_m.col2) && (col3 == in_m.col3));
		}

		/** @return True if the two matrices are strictly not equal. */
		CX_INLINE CxBool operator!=(const CxMat4 &in_m) const {
			return ((col0 != in_m.col0) || (col1 != in_m.col1) ||
					  (col2 != in_m.col2) || (col3 != in_m.col3));
		}

		/** @brief Array access of the columns of the matrix. */
		CX_FORCE_INLINE CxReal & operator[](CxI32 in_idx) { return (&col0)[in_idx]; }

		/** @brief Array access of the columns of the matrix. */
		CX_FORCE_INLINE CxReal operator[](CxI32 in_idx) const { return (&col0)[in_idx]; }

		/**
		 * @brief Access an element of the matrix by it's column and row.
		 * @param in_col The index of the column to access [0-3].
		 * @param in_row The index of the row to access [0-3].
		 * @return The value at m[col][row].
		 */
		CX_FORCE_INLINE CxReal & operator()(CxI32 in_col, CxI32 in_row) {
			return (&col0)[in_col*3 + in_row];
		}
		/** @see operator(CxI32, CxI32) */
		CX_FORCE_INLINE CxReal operator()(CxI32 in_col, CxI32 in_row) const {
			return (&col0)[in_col*3 + in_row];
		}

		/** @return A copy of the matrix with all entries negated. */
		CX_INLINE CxMat4 operator-() const {
			return CxMat4(-col0, -col1, -col2, -col3);
		}

		/** @return The result of the matrix addition of (this + in_m). */
		CX_INLINE CxMat4 operator+(const CxMat4 &in_m) const {
			return CxMat4(col0 + in_m.col0, col1 + in_m.col1,
							  col2 + in_m.col2, col3 + in_m.col3);
		}

		/** @return The result of the matrix subtraction of (this - in_m) */
		CX_INLINE CxMat4 operator-(const CxMat4 &in_m) const {
			return CxMat4(col0 - in_m.col0, col1 - in_m.col1,
							  col2 - in_m.col2, col3 - in_m.col3);
		}

		/** @return The result of multiplying this matrix by a scalar. */
		CX_INLINE CxMat4 operator*(CxReal in_s) const {
			return CxMat4(col0*in_s, col1*in_s, col2*in_s, col3*in_s);
		}

		/** @return The result of the vector v transformed by this matrix (M*v). */
		CxVec4 operator*(const CxVec4 &in_v) const;

		/** @return The result of the vector v transformed by this matrix (M*v). */
		CxVec3 operator*(const CxVec3 &in_v) const;

		/** @return The result of the matrix multiplication (this * in_m). */
		CxMat4 operator*(const CxMat4 &in_m) const;

		/** @return The result of dividing this matrix by a scalar. */
		CX_INLINE CxMat4 operator/(CxReal in_s) const {
			const CxReal r_s = 1.0f/in_s;
			return CxMat4(col0*r_s, col1*r_s, col2*r_s, col3*r_s);
		}

		/** @brief Add another matrix to this matrix. */
		CX_INLINE CxMat4 & operator+=(const CxMat4 &in_m) {
			col0 += in_m.col0; col1 += in_m.col1;
			col2 += in_m.col2; col3 += in_m.col3;
			return *this;
		}

		/** @brief Subtract another matrix from this matrix. */
		CX_INLINE CxMat4 & operator-=(const CxMat4 &in_m) {
			col0 -= in_m.col0; col1 -= in_m.col1;
			col2 -= in_m.col2; col3 -= in_m.col3;
			return *this;
		}

		/** @brief Multiply this matrix by a scalar */
		CX_INLINE CxMat4 & operator*=(CxReal in_s) {
			col0 *= in_s; col1 *= in_s;
			col2 *= in_s; col3 *= in_s;
			return *this;
		}

		/** @brief Multiply this matrix by another matrix. */
		CxMat4 & operator*=(const CxMat4 &in_m);

		/** @brief Divide this matrix by a scalar. */
		CX_INLINE CxMat4 & operator/=(CxReal in_s) {
			const CxReal r_s = 1.0f/in_s;
			col0 *= r_s; col1 *= r_s; col2 *= r_s; col3 *= r_s;
			return *this;
		}

		/** @return The matrix as a column major array stored as col0,col1,col2. */
		CX_FORCE_INLINE const CxReal * constData() const { return &(col0.x); }
		CX_FORCE_INLINE CxReal * data() { return &(col0.x); }
		CX_FORCE_INLINE const CxReal * data() const { return &(col0.x); }

		/** @return The determinant for an invertable matrix. */
		CxReal determinant() const;

		/** @return The determinant for an invertable 3x3 upper left matrix. */
		CxReal determinant3x3() const;

		/** @return The diagonal of this matrix (all off-diagonal entries = 0). */
		CxMat4 diagonal() const;

		/** @return The inverse of the matrix */
		CxMat4 inverse() const;

		/** @brief Invert this matrix. */
		void invert();

		/** @return The upper left 3x3 rotational matrix from this matrix. */
		CxMat3 mat3() const;

		/** @return A quaternion representing the 3x3 rotational component of the matrix. */
		CxQuat quat() const;

		/**
		 * @brief Set the matrix to an orthographic projection matrix.
		 * @param in_r The + x-coord of the viewing rectangle.
		 * @param in_l The - x-coord of the viewing rectangle.
		 * @param in_t The + y-coord of the viewing rectangle.
		 * @param in_b The - y-coord of the viewing rectangle.
		 * @param in_n The near clipping plane.
		 * @param in_f The far clipping plane.
		 */
		void setOrtho(CxReal in_r, CxReal in_l, CxReal in_t, CxReal in_b,
						  CxReal in_n, CxReal in_f);
		

		/**
		 * @brief Set the matrix to an orthographic projection matrix centered at (0,0).
		 * @param in_w The width of the viewing rectangle.
		 * @param in_h The height of the viewing rectangle.
		 * @param in_n The near clipping plane.
		 * @param in_f The far clipping plane.
		 */
		void setOrtho(CxReal in_w, CxReal in_h, CxReal in_n, CxReal in_f);

		/**
		 * @brief Set the matrix to a perspective projection matrix.
		 * @param in_r The + x-coord of the viewing rectangle.
		 * @param in_l The - x-coord of the viewing rectangle.
		 * @param in_t The + y-coord of the viewing rectangle.
		 * @param in_b The - y-coord of the viewing rectangle.
		 * @param in_n The near clipping plane.
		 * @param in_f The far clipping plane.
		 */
		void setPerspective(CxReal in_r, CxReal in_l, CxReal in_t, CxReal in_b,
								  CxReal in_n, CxReal in_f);
		/**
		 * @brief Set the matrix to a perspective projection matrix.
		 * @param in_fov The field of view for the viewing frustrum.
		 * @param in_aspect The aspect ratio (width/height) of the viewing frustrum.
		 * @param in_n The near clipping plane.
		 * @param in_f The far clipping plane.
		 */
		void setPerspective(CxReal in_fov, CxReal in_aspect, CxReal in_n, CxReal in_f);

		/** @brief Set the matrix to a rotation matrix from a unit quaternion. */
		void setQuat(const CxQuat &in_q);


		/** @brief Set the translation portion of the matrix [c3.x, c3.y, c3.z] */
		CX_FORCE_INLINE void setTranslation(const CxVec3 &in_t) {
			col3.x = in_t.x; col3.y = in_t.y; col3.z = in_t.z;
		}

		/** @return A vector r transformed by the matrix (r = Mv). */
		CxVec4 transform(const CxVec4 &in_v) const;

		/** @return A vector r transformed by the matrix (r = Mv). */
		CxVec3 transform(const CxVec3 &in_v) const;

		/** @return A vector r transformed by the inverse matrix (r = M^-1v). */
		CxVec3 transformInverse(const CxVec3 &in_v) const;
		
		/** @return A vector r transformed by the transposed matrix (r = M^Tv). */
		CxVec3 transformTransposed(const CxVec3 &in_v) const;

		/**
		 * @brief Rotate a vector by the 3x3 rotational portion of the matrix.
		 * @param in_v The vector to rotate.
		 * @return A copy of the vector, rotated by the 3x3 rotational matrix.
		 */
	   CxVec3 transform3x3(const CxVec3 &in_v) const;

		/** @return A vector r transformed by the inverse of the upper 3x3 matrix */
	   CxVec3 transform3x3Inverse(const CxVec3 &in_v) const;

		/** @return A vector r transformed by the transpose of the upper 3x3 matrix */
	   CxVec3 transform3x3Transposed(const CxVec3 &in_v) const;

		/** @return The translation portion of the matrix [c3.x, c3.y, c3.z] */
		CX_FORCE_INLINE CxVec3 translation() const {
			return CxVec3(col3.x, col3.y, col3.z);
		}

		/** @brief Transpose this matrix. */
		void transpose();

		/** @return The transpose of this matrix. */
		CxMat4 transposed() const;

		/** @return The transformed x-axis basis vector */
		CX_FORCE_INLINE CxVec3 xAxis() const { return CxVec3(col0.x, col0.y, col0.z); }

		/** @return The transformed y-axis basis vector */
		CX_FORCE_INLINE CxVec3 yAxis() const { return CxVec3(col1.x, col1.y, col1.z); }

		/** @return The transformed z-axis basis vector */
		CX_FORCE_INLINE CxVec3 zAxis() const { return CxVec3(col2.x, col2.y, col2.z); }
	};

	CX_FORCE_INLINE CxVec4 CxMat4::operator*(const CxVec4 &in_v) const {
		return transform(in_v);
	}

	CX_FORCE_INLINE CxVec4 CxMat4::operator*(const CxVec3 &in_v) const {
		return transform(in_v);
	}

	CX_INLINE CxMat4 CxMat4::operator*(const CxMat4 &in_m) const {
		const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y;
		const CxReal m02 = in_m.col0.z;  const CxReal m03 = in_m.col0.w;
		const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y;
		const CxReal m12 = in_m.col1.z;  const CxReal m13 = in_m.col1.w;
		const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y;
		const CxReal m22 = in_m.col2.z;  const CxReal m22 = in_m.col2.w;

		const CxReal c0x = col0.x;  const CxReal c1x = col1.x;
		const CxReal c2x = col2.x;  const CxReal c3x = col2.x;
		const CxReal c0y = col0.y;  const CxReal c1y = col1.y;
		const CxReal c2y = col2.y;  const CxReal c3y = col3.y;
		
	   const CxReal n00 = (c0x*m00) + (c1x*m01) + (c2x*m02) + (c3x*m03);
		const CxReal n10 = (c0x*m10) + (c1x*m11) + (c2x*m12) + (c3x*m13);
		const CxReal n20 = (c0x*m20) + (c1x*m21) + (c2x*m22) + (c3x*m23);
		const CxReal n30 = (c0x*m30) + (c1x*m31) + (c2x*m32) + (c3x*m33);

		const CxReal c0z = col0.z;  const CxReal c1z = col1.z;
		const CxReal c2z = col2.z;  const CxReal c3z = col3.z;

	   const CxReal n01 = (c0y*m00) + (c1y*m01) + (c2y*m02) + (c3y*m03);
	   const CxReal n11 = (c0y*m10) + (c1y*m11) + (c2y*m12) + (c3y*m13);
	   const CxReal n21 = (c0y*m20) + (c1y*m21) + (c2y*m22) + (c3y*m23);
		const CxReal n31 = (c0y*m30) + (c1y*m31) + (c2y*m32) + (c3y*m33);

		const CxReal c0w = col0.w;  const CxReal c1w = col1.w;
		const CxReal c2w = col2.w;  const CxReal c3w = col3.w;
		
		const CxReal n02 = (c0z*m00) + (c1z*m01) + (c2z*m02) + (c3z*m03);
		const CxReal n12 = (c0z*m10) + (c1z*m11) + (c2z*m12) + (c3z*m13);
		const CxReal n22 = (c0z*m20) + (c1z*m21) + (c2z*m22) + (c3z*m23);
		const CxReal n23 = (c0z*m30) + (c1z*m31) + (c2z*m32) + (c3z*m33);

		const CxReal n03 = (c0w*m00) + (c1w*m01) + (c2w*m02) + (c3w*m03);
		const CxReal n13 = (c0w*m10) + (c1w*m11) + (c2w*m12) + (c3w*m13);
		const CxReal n23 = (c0w*m20) + (c1w*m21) + (c2w*m22) + (c3w*m23);
		const CxReal n23 = (c0w*m30) + (c1w*m31) + (c2w*m32) + (c3w*m33);

		return CxMat3(n00, n01, n02, n03,
						  n10, n11, n12, n13,
						  n20, n21, n22, n23,
						  n30, n31, n32, n33);
	}

	CX_FORCE_INLINE CxMat4 & CxMat4::operator*=(const CxMat4 &in_m) const {
	   const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y;
		const CxReal m02 = in_m.col0.z;  const CxReal m03 = in_m.col0.w;
		const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y;
		const CxReal m12 = in_m.col1.z;  const CxReal m13 = in_m.col1.w;
		const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y;
		const CxReal m22 = in_m.col2.z;  const CxReal m23 = in_m.col2.w;
		const CxReal m30 = in_m.col3.x;  const CxReal m31 = in_m.col3.y;
		const CxReal m32 = in_m.col3.z;  const CxReal m33 = in_m.col3.w;

		const CxReal c0x = col0.x;  const CxReal c1x = col1.x;
		const CxReal c2x = col2.x;  const CxReal c3x = col2.x;
		const CxReal c0y = col0.y;  const CxReal c1y = col1.y;
		const CxReal c2y = col2.y;  const CxReal c3y = col3.y;
		
	   col0.x = (c0x*m00) + (c1x*m01) + (c2x*m02) + (c3x*m03);
		col1.x = (c0x*m10) + (c1x*m11) + (c2x*m12) + (c3x*m13);
		col2.x = (c0x*m20) + (c1x*m21) + (c2x*m22) + (c3x*m23);
		col3.x = (c0x*m30) + (c1x*m31) + (c2x*m32) + (c3x*m33);

		const CxReal c0z = col0.z;  const CxReal c1z = col1.z;
		const CxReal c2z = col2.z;  const CxReal c3z = col3.z;

	   col0.y = (c0y*m00) + (c1y*m01) + (c2y*m02) + (c3y*m03);
	   col1.y = (c0y*m10) + (c1y*m11) + (c2y*m12) + (c3y*m13);
	   col2.y = (c0y*m20) + (c1y*m21) + (c2y*m22) + (c3y*m23);
		col3.y = (c0y*m30) + (c1y*m31) + (c2y*m32) + (c3y*m33);

		const CxReal c0w = col0.w;  const CxReal c1w = col1.w;
		const CxReal c2w = col2.w;  const CxReal c3w = col3.w;
		
		col0.z = (c0z*m00) + (c1z*m01) + (c2z*m02) + (c3z*m03);
		col1.z = (c0z*m10) + (c1z*m11) + (c2z*m12) + (c3z*m13);
		col2.z = (c0z*m20) + (c1z*m21) + (c2z*m22) + (c3z*m23);
		col3.z = (c0z*m30) + (c1z*m31) + (c2z*m32) + (c3z*m33);

		col0.w = (c0w*m00) + (c1w*m01) + (c2w*m02) + (c3w*m03);
		col1.w = (c0w*m10) + (c1w*m11) + (c2w*m12) + (c3w*m13);
		col2.w = (c0w*m20) + (c1w*m21) + (c2w*m22) + (c3w*m23);
		col3.w = (c0w*m30) + (c1w*m31) + (c2w*m32) + (c3w*m33);
	}

	CX_INLINE CxReal CxMat4::determinant() const {
		const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y;
		const CxReal m02 = in_m.col0.z;  const CxReal m03 = in_m.col0.w;
		const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y;
		const CxReal m12 = in_m.col1.z;  const CxReal m13 = in_m.col1.w;
		const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y;
		const CxReal m22 = in_m.col2.z;  const CxReal m23 = in_m.col2.w;
		const CxReal m30 = in_m.col3.x;  const CxReal m31 = in_m.col3.y;
		const CxReal m32 = in_m.col3.z;  const CxReal m33 = in_m.col3.w;

		return
			m01*((m22*m30 - m20*m32)*m13 + (m10*m32 - m12*m30)*m23) +
			m00*((m12*m23 - m13*m22)*m31 + (m13*m21 - m11*m23)*m32) + 
			m03*((m12*m21 - m11*m22)*m30 + (m10*m22 - m12*m20)*m31 + (m11*m20 - m10*m21)*m32) +
			m33*((m01*m20 - m00*m21)*m12 + (m00*m11 - m01*m10)*m22) + 
			m02*((m11*m23 - m13*m21)*m30 + (m13*m20 - m10*m23)*m31 + (m10*m21 - m11*m20)*m33);
	}

	CX_INLINE CxReal CxMat4::determinant3x3() const {
		const CxReal m00 = col0.x;  const CxReal m01 = col0.y; const CxReal m02 = col0.z;
		const CxReal m10 = col1.x;  const CxReal m11 = col1.y; const CxReal m12 = col1.z;
		const CxReal m20 = col2.x;  const CxReal m21 = col2.y; const CxReal m22 = col2.z;

		return (m00*(m11*m22 - m21*m12) -
				  m01*(m12*m20 - m10*m22) -
				  m02*(m10*m21 - m11*m20));
	}

	CX_INLINE CxMat4 CxMat4::diagonal() const {
		return CxMat4(col0.x, 0, 0, 0,
						  0, col1.y, 0, 0,
						  0, 0, col2.z, 0,
						  0, 0, 0, col3.w);
	}

	CX_INLINE CxMat4 CxMat4::inverse() const {
		const CxReal det_m = determinant();
		if (det_m != 0) {
			const CxReal dr = 1.0f/det_m;
		   const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y;
			const CxReal m02 = in_m.col0.z;  const CxReal m03 = in_m.col0.w;
			const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y;
			const CxReal m12 = in_m.col1.z;  const CxReal m13 = in_m.col1.w;
			const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y;
			const CxReal m22 = in_m.col2.z;  const CxReal m23 = in_m.col2.w;
			const CxReal m30 = in_m.col3.x;  const CxReal m31 = in_m.col3.y;
			const CxReal m32 = in_m.col3.z;  const CxReal m33 = in_m.col3.w;

			return CxMat4(
				dr*((m12*m23 - m13*m22)*m31 + (m13*m21 - m11*m23)*m32 + (m11*m22 - m12*m21)*m33),
				dr*((m31*m22 - m21*m32)*m03 + (m01*m32 - m31*m02)*m23 + (m21*m02 - m01*m22)*m33),
				dr*((m11*m32 - m31*m12)*m03 + (m31*m02 - m01*m32)*m13 + (m01*m12 - m11*m02)*m33),
				dr*((m21*m12 - m11*m22)*m03 + (m01*m22 - m21*m02)*m13 + (m11*m02 - m01*m12)*m23),
		
				dr*((m30*m22 - m20*m32)*m13 + (m10*m32 - m30*m12)*m23 + (m20*m12 - m10*m22)*m33),
				dr*((m20*m32 - m30*m22)*m03 + (m30*m02 - m00*m32)*m23 + (m00*m22 - m20*m02)*m33),
				dr*((m30*m12 - m10*m32)*m03 + (m00*m32 - m30*m02)*m13 + (m10*m02 - m00*m12)*m33),
				dr*((m10*m22 - m20*m12)*m03 + (m20*m02 - m00*m22)*m13 + (m00*m12 - m10*m02)*m23),

				dr*((m20*m31 - m30*m21)*m13 + (m30*m11 - m10*m31)*m23 + (m10*m21 - m20*m11)*m33),
				dr*((m30*m21 - m20*m31)*m03 + (m00*m31 - m30*m01)*m23 + (m20*m01 - m00*m21)*m33),
				dr*((m10*m31 - m30*m11)*m03 + (m30*m01 - m00*m31)*m13 + (m00*m11 - m10*m01)*m33),
				dr*((m20*m11 - m10*m21)*m03 + (m00*m21 - m20*m01)*m13 + (m10*m01 - m00*m11)*m23),
				
				dr*((m30*m21 - m20*m31)*m12 + (m10*m31 - m30*m11)*m22 + (m20*m11 - m10*m21)*m32),
				dr*((m20*m31 - m30*m21)*m02 + (m30*m01 - m00*m31)*m22 + (m00*m21 - m20*m01)*m32),
				dr*((m30*m11 - m10*m31)*m02 + (m00*m31 - m30*m01)*m12 + (m10*m01 - m00*m11)*m32),
				dr*((m10*m21 - m20*m11)*m02 + (m20*m01 - m00*m21)*m12 + (m00*m11 - m10*m01)*m22)
				);
		}
		else {
			CXD_WARN("Trying to get Inverse of non-invertable CxMat4");
			return CxMat4(CxIdentity);
		}
	}

	CX_INLINE void CxMat4::invert() {
		const CxReal det_m = determinant();
		if (det_m != 0) {
			const CxReal dr = 1.0f/det_m;
		   const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y;
			const CxReal m02 = in_m.col0.z;  const CxReal m03 = in_m.col0.w;
			const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y;
			const CxReal m12 = in_m.col1.z;  const CxReal m13 = in_m.col1.w;
			const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y;
			const CxReal m22 = in_m.col2.z;  const CxReal m23 = in_m.col2.w;
			const CxReal m30 = in_m.col3.x;  const CxReal m31 = in_m.col3.y;
			const CxReal m32 = in_m.col3.z;  const CxReal m33 = in_m.col3.w;
			
			col0.x = dr*((m12*m23 - m13*m22)*m31 + (m13*m21 - m11*m23)*m32 + (m11*m22 - m12*m21)*m33);
			col0.y = dr*((m31*m22 - m21*m32)*m03 + (m01*m32 - m31*m02)*m23 + (m21*m02 - m01*m22)*m33);
			col0.z = dr*((m11*m32 - m31*m12)*m03 + (m31*m02 - m01*m32)*m13 + (m01*m12 - m11*m02)*m33);
			col0.w = dr*((m21*m12 - m11*m22)*m03 + (m01*m22 - m21*m02)*m13 + (m11*m02 - m01*m12)*m23);
		
			col1.x = dr*((m30*m22 - m20*m32)*m13 + (m10*m32 - m30*m12)*m23 + (m20*m12 - m10*m22)*m33);
			col1.y = dr*((m20*m32 - m30*m22)*m03 + (m30*m02 - m00*m32)*m23 + (m00*m22 - m20*m02)*m33);
			col1.z = dr*((m30*m12 - m10*m32)*m03 + (m00*m32 - m30*m02)*m13 + (m10*m02 - m00*m12)*m33);
			col1.w = dr*((m10*m22 - m20*m12)*m03 + (m20*m02 - m00*m22)*m13 + (m00*m12 - m10*m02)*m23);

			col2.x = dr*((m20*m31 - m30*m21)*m13 + (m30*m11 - m10*m31)*m23 + (m10*m21 - m20*m11)*m33);
			col2.y = dr*((m30*m21 - m20*m31)*m03 + (m00*m31 - m30*m01)*m23 + (m20*m01 - m00*m21)*m33);
			col2.z = dr*((m10*m31 - m30*m11)*m03 + (m30*m01 - m00*m31)*m13 + (m00*m11 - m10*m01)*m33);
			col2.w = dr*((m20*m11 - m10*m21)*m03 + (m00*m21 - m20*m01)*m13 + (m10*m01 - m00*m11)*m23);
				
			col3.x = dr*((m30*m21 - m20*m31)*m12 + (m10*m31 - m30*m11)*m22 + (m20*m11 - m10*m21)*m32);
			col3.y = dr*((m20*m31 - m30*m21)*m02 + (m30*m01 - m00*m31)*m22 + (m00*m21 - m20*m01)*m32);
			col3.z = dr*((m30*m11 - m10*m31)*m02 + (m00*m31 - m30*m01)*m12 + (m10*m01 - m00*m11)*m32);
			col3.w = dr*((m10*m21 - m20*m11)*m02 + (m20*m01 - m00*m21)*m12 + (m00*m11 - m10*m01)*m22);
		}
#if defined(CX_DEBUG)
		else { CXD_WARN("Trying to Invert non-invertable CxMat4"); }
#endif // CX_DEBUG
	}

	CX_FORCE_INLINE CxMat3 CxMat4::mat3() const {
		return CxMat3(col0.x, col0.y, col0.z,
						  col1.x, col1.y, col1.z,
						  col2.x, col2.y, col2.z);
	}

	CX_INLINE CxQuat CxMat4::quat() const {
	   CxMat3 m = mat3();  return m.quat();
	}

	CX_INLINE void CxMat4::setOrtho(CxReal in_r, CxReal in_l, CxReal in_t, CxReal in_b,
											  CxReal in_n, CxReal in_f) {
		const CxReal x_diff = 1.0f/(in_r - in_l);
		const CxReal y_diff = 1.0f/(in_t - in_b);
		const CxReal z_diff = 1.0f/(in_f - in_n);

		col0.x = 2.0f*x_diff;
		col0.y = col0.z = col0.w = col1.x = 0.0f;
		col0.z = col0.w = col1.x = 0.0f;
		col1.y = 2.0f*y_diff;
		col1.z = col1.w = col2.x = col2.y = 0.0f;
		col1.w = col2.x = col2.y = 0.0f;
		col2.z = -2.0f*z_diff;
		col2.w = 0.0f;
		col3.x = -((in_r + in_l) * x_diff);
		col3.y = -((in_t + in_b) * y_diff);
		col3.z = -((in_f + in_n) * z_diff);
		col3.w = 1.0f;
	}

	CX_INLINE void CxMat4::setOrtho(CxReal in_w, CxReal in_h, CxReal in_n, CxReal in_f) {
		const CxReal z_diff = 1.0f/(in_f - in_n);

		col0.x = 2.0f/in_w;
		c0l0.y = col0.z = col0.w = col1.x = 0.0f;
		col1.y = 2.0f/in_h;
		col1.z = col1.w = col2.x = col2.y = 0.0f;
		col2.z = -2.0f*z_diff;
		col2.w = col3.x = col3.y = 0.0f;
		col3.z = -((in_f + in_n)*z_diff);
		col3.w = 1.0f;
	}

	CX_INLINE void CxMat4::setPerspective(CxReal in_r, CxReal in_l, CxReal in_t, CxReal in_b,
													  CxReal in_n, CxReal in_f) {
		const CxReal x_diff = 1.0f/(in_r - in_l);
		const CxReal y_diff = 1.0f/(in_t - in_b);
		const CxReal z_diff = 1.0f/(in_f - in_n);

		col0.x = (2*in_n)*x_diff;
		col0.y = col0.z = col0.w = col1.x = 0.0f;
		col1.y = (2*in_n)*y_diff;
		col1.z = col1.w = 0.0f;
		col2.x = (in_r + in_l)*x_diff;
		col2.y = (in_t + in_b)*y_diff;
		col2.z = -( (in_f + in_n)*z_diff);
		col2.w = -1.0f;
		col3.x = col3.y = 0.0f;
		col3.z = -((2*in_f*in_r)*z_diff);
		col3.w = 1.0f;
	}

	CX_INLINE void CxMat4::setPerspective(CxReal in_fov, CxReal in_aspect,
													  CxReal in_n, CxReal in_f) {
		const CxReal right = in_aspect * top;
		const CxReal top = CxTan(CxDegToRad*in_fov*0.5f) * in_n;
		
		const CxReal z_diff = 1.0f/(in_f - in_n);
		const CxReal x_diff = 1.0f/(right + right);
		const CxReal y_diff = 1.0f/(top + top);

		col0.x = (2*in_n)*x_diff;
		col0.y = col0.z = col0.w = col1.x = 0.0f;
		col1.y = (2*in_n)*y_diff;
		col1.z = col1.w = col2.x = col2.y = 0.0f;
		col2.z = -((in_f + in_n)*z_diff);
		col2.w = -1.0f;
		col3.x = col3.y = 0.0f;
		col3.z = -((2*in_f*in_r)*z_diff);
		col3.w = 1.0f;
	}

	CX_FORCE_INLINE void CxMat4::setQuat(const CxQuat &in_q) {
		const CxReal x = in_q.x;  const CxReal y = in_q.y;
		const CxReal z = in_q.z;  const CxReal w = in_q.w;

		const CxReal x2 = x + x;  const CxReal y2 = y + y;  const CxReal z2 = z+z;
		const CxReal xx2 = x*x2;  const CxReal yy2 = y*y2;  const CxReal zz2 = z*z2;
		const CxReal xy2 = x2*y;  const CxReal xz2 = x2*z;  const CxReal xw2 = x2*w;
		const CxReal yz2 = y2*z;  const CxReal yw2 = y2*w;  const CxReal zw2 = z2*w;

		col0.x = 1.0f - yy2 - zz2;  col0.y = xy2 + zw2;         col0.z = xz2 - yw2; col0.w = 0;
		col1.x = xy2 - zw2;         col1.y = 1.0f - xx2 - zz2;  col1.z = yz2 + xw2; col1.w = 0;
		col2.x = xz2 + yw2;         col2.y = yz2 - xw2;         col2.z = 1.0f - xx2 - yy2;  col2.w = 0;
		col3.setXYZW(0,0,0,1);
	}

	CX_FORCE_INLINE CxVec4 CxMat4::transform(CxVec4 &in_v) const {
	   const CxReal x = in_v.x;  const CxReal y = in_v.y;
		const CxReal z = in_v.z;  const CxReal w = in_v.w;
		return CxVec4(col0.x*x + col1.x*y + col2.x*z + col3.x*w,
						  col0.y*x + col1.y*y + col2.y*z + col3.y*w,
						  col0.z*x + col1.z*y + col2.z*z + col3.z*w,
						  col0.w*x + col1.w*y + col2.w*z + col3.w*w);
	}
	
	CX_FORCE_INLINE CxVec3 CxMat4::transform(CxVec3 &in_v) const {
	   const CxReal x = in_v.x;  const CxReal y = in_v.y;  const CxReal z = in_v.z;
		return CxVec3(col0.x*x + col1.x*y + col2.x*z + col3.x,
						  col0.y*x + col1.y*y + col2.y*z + col3.y
						  col0.z*x + col1.z*y + col2.z*z + col3.z);
	}

	CX_INLINE CxVec3 CxMat4::transformInverse(CxVec3 &in_v) const {
		const CxReal det_m = determinant();
		const CxReal x = in_v.x;  const CxReal y = in_v.y;  const CxReal z = in_v.z;
		if (det_m != 0) {
			const CxReal dr = 1.0f/det_m;
			const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y;
			const CxReal m02 = in_m.col0.z;  const CxReal m03 = in_m.col0.w;
			const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y;
			const CxReal m12 = in_m.col1.z;  const CxReal m13 = in_m.col1.w;
			const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y;
			const CxReal m22 = in_m.col2.z;  const CxReal m23 = in_m.col2.w;
			const CxReal m30 = in_m.col3.x;  const CxReal m31 = in_m.col3.y;
			const CxReal m32 = in_m.col3.z;  const CxReal m33 = in_m.col3.w;

			return CxVec3(
				dr*((m03*m12 - m02*m13)*m21 + (m01*m13 - m03*m11)*m22 + (m02*m11 - m01*m12)*m23 +
					 ((m12*m23 - m13*m22)*m31 + (m13*m21 - m11*m23)*m32 + (m11*m22 - m12*m21)*m33)*x +
					 ((m03*m22 - m02*m23)*m31 + (m01*m23 - m03*m21)*m32 + (m02*m21 - m01*m22)*m33)*y +
					 ((m02*m13 - m03*m12)*m31 + (m03*m11 - m01*m13)*m32 + (m01*m12 - m02*m11)*m33)*z),

			   dr*((m02*m13 - m03*m12)*m20 + (m03*m10 - m00*m13)*m22 + (m00*m12 - m02*m10)*m23 +
					 ((m13*m22 - m12*m23)*m30 + (m10*m23 - m13*m20)*m32 + (m12*m20 - m10*m22)*m33)*x +
					 ((m02*m23 - m03*m22)*m30 + (m03*m20 - m00*m23)*m32 + (m00*m22 - m02*m20)*m33)*y +
					 ((m03*m12 - m02*m13)*m30 + (m00*m13 - m03*m10)*m32 + (m02*m10 - m00*m12)*m33)*z),
				
				dr*((m03*m11 - m01*m13)*m20 + (m00*m13 - m03*m10)*m21 + (m01*m10 - m00*m11)*m23 +
					 ((m11*m23 - m13*m21)*m30 + (m13*m20 - m10*m23)*m31 + (m10*m21 - m11*m20)*m33)*x +
					 ((m03*m21 - m01*m23)*m30 + (m00*m23 - m03*m20)*m31 + (m01*m20 - m00*m21)*m33)*y +
					 ((m01*m13 - m03*m11)*m30 + (m03*m10 - m00*m13)*m31 + (m00*m11 - m01*m10)*m33)*z)
				);
		}
		else {
			CXD_WARN("Trying to transform by inverse of non-invertable CxMat4");
			return in_v;
		}
	}

	CX_FORCE_INLINE CxVec3 CxMat4::transformTransposed(CxVec3 &in_v) const {
		const CxReal x = in_v.x;  const CxReal y = in_v.y;  const CxReal z = in_v.z;
		/* (dot(col0, v), dot(col1, v), dot(col2, v)) */
		return CxVec3(col0.x*x + col0.y*y + col0.z*z + col0.w,
						  col1.x*x + col1.y*y + col1.z*z + col1.w,
						  col2.x*x + col2.y*y + col2.z*z + col2.w);
	}

	CX_FORCE_INLINE CxVec3 CxMat4::transform3x3(CxVec3 &in_v) const {
		const CxReal x = in_v.x;  const CxReal y = in_v.y;  const CxReal z = in_v.z;
		return CxVec3(col0.x*x + col1.x*y + col2.x*z,
						  col0.y*x + col1.y*y + col2.y*z,
						  col0.z*x + col1.z*y + col2.z*z);
	}

	CX_FORCE_INLINE CxVec3 CxMat4::transform3x3Inverse(CxVec3 &in_v) const {
		const CxReal det_m = determinant3x3();
		if (det_m != 0) {
			const CxReal dr = 1.0f/det_m;
			const CxReal m10 = col1.x;    const CxReal m11 = col1.y;    const CxReal m12 = col1.z;
			const CxReal m20 = col2.x;    const CxReal m21 = col2.y;    const CxReal m22 = col2.z;
			const CxReal dx = in_v.x*dr;  const CxReal dy = in_v.y*dr;  const CxReal dz = in_v.z*dr;
			const CxReal m00 = col0.x;    const CxReal m01 = col0.y;    const CxReal m02 = col0.z;

			return CxVec3(dx*(m11*m22 - m12*m21) + dy*(m12*m20 - m10*m22) + dz*(m10*m21 - m11*m20),
							  dx*(m02*m21 - m01*m22) + dy*(m00*m22 - m02*m20) + dz*(m00*m21 - m01*m20),
							  dx*(m01*m12 - m02*m11) + dy*(m02*m10 - m00*m12) + dz*(m00*m11 - m01*m10));
		}
		else {
			CXD_WARN("Trying to transform by inverse of non-invertable 3x3 portion of CxMat4");
			return in_v;
		}
	}

	CX_FORCE_INLINE CxVec3 CxMat4::transform3x3Transposed(CxVec3 &in_v) const {
		const CxReal x = in_v.x;  const CxReal y = in_v.y;  const CxReal z = in_v.z;
		/* (dot(col0, v), dot(col1, v), dot(col2, v)) */
		return CxVec3(col0.x*x + col0.y*y + col0.z*z,
						  col1.x*x + col1.y*y + col1.z*z,
						  col2.x*x + col2.y*y + col2.z*z);
	}

	CX_INLINE void CxMat4::transpose() {
	   const CxReal m01 = in_m.col0.y;  const CxReal m02 = in_m.col0.z;
		const CxReal m03 = in_m.col0.w;  const CxReal m10 = in_m.col1.x;
		const CxReal m12 = in_m.col1.z;  const CxReal m13 = in_m.col1.w;
		const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y;
		const CxReal m23 = in_m.col2.w;  const CxReal m30 = in_m.col3.x;
		const CxReal m31 = in_m.col3.y;  const CxReal m32 = in_m.col3.z;

		col0.y = m10;  col0.z = m20;  col0.w = m30;
		col1.x = m01;  col1.z = m21;  col1.w = m31;
		col2.x = m02;  col2.y = m12;  col2.w = m32;
		col3.x = m03;  col3.y = m13;  col3.z = m23;
	}

	CX_INLINE CxMat4 CxMat4::transposed() const {
	   return CxMat4(col0.x, col1.x, col2.x, col3.x,
						  col0.y, col1.y, col2.y, col3.y,
						  col0.z, col1.z, col2.z, col3.z,
						  col0.w, col1.w, col2.w, col3.w);
	}
} // namespace cat


#endif // CX_CORE_MATH_CXMAT4_H
