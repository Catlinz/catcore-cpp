#ifndef CX_CORE_MATH_CXMAT4X4_H
#define CX_CORE_MATH_CXMAT4x4_H

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
#include "core/math/CxMat3x3.h"

namespace cat {

	class CxMat4x4 {
	  public:
		CxVec4 col0, col1, col2, col3;  /**< Columns of the matrix */

		const static CxMat4x4 kIdentity;
		
		/** @brief Initialise the matrix to garbage */
		CX_FORCE_INLINE CxMat4x4() {}

		/** @brief Initialise the matrix to all zeros. */
		CX_INLINE CxMat4x4(CxZero)
			: col0(CxZero), col1(CxZero), col2(CxZero), col3(CxZero) {}

		/** @brief Initialise to the identity matrix. */
		CX_INLINE CxMat4x4(CxIdentity)
			: col0(1.0f, 0.0f, 0.0f, 0.0f), col1(0.0f, 1.0f, 0.0f, 0.0f),
			  col2(0.0f, 0.0f, 1.0f, 0.0f), col3(0.0f, 0.0f, 0.0f, 1.0f) {}

		/** @brief Initialise the matrix from an array. */
		explicit CX_INLINE CxMat4x4(const CxReal *in_arr)
			: col0(in_arr[0],  in_arr[1],  in_arr[2],  in_arr[3]),
			  col1(in_arr[4],  in_arr[5],  in_arr[6],  in_arr[7]),
			  col2(in_arr[8],  in_arr[9],  in_arr[10], in_arr[11]),
			  col3(in_arr[12], in_arr[13], in_arr[14], in_arr[15]) {}

		/**
		 * @brief Initialise from a 3x3 rotation matrix (set rest to identity).
		 * @param in_m The 3x3 rotation matrix to set this matrix from.
		 */
		explicit CX_INLINE CxMat4x4(const CxMat3x3 &in_r)
			: col0(in_r.col0, 0.0f), col1(in_r.col1, 0.0f), col2(in_r.col2, 0.0f),
			  col3(0.0f, 0.0f, 0.0f, 1.0f) {}

		/** 
		 * @brief Initialise the matrix from a quaternion.
		 * @param in_q The quaternion to initialise the matrix from.
		 */
		explicit CX_INLINE CxMat4x4(const CxQuat &in_q)
			: col0(CxZero), col1(CxZero), col2(CxZero), col3(0.0f, 0.0f, 0.0f, 1.0f) {
			/* Load the values into the registers. */
			const CxReal x = in_q.x; const CxReal y = in_q.y;
			const CxReal z = in_q.z; const CxReal w = in_q.w;

			const CxReal x2 = x + x; const CxReal y2 = y + y; const CxReal z2 = z + z;
			const CxReal xx = x2*x;  const CxReal yy = y2*y;  const CxReal zz = z2*z;
			const CxReal xy = x2*y;  const CxReal xz = x2*z;  const CxReal x2 = x2*w;
			const CxReal yz = y2*z;  const CxReal yw = y2*w;  const CxReal zw = z2*w;
			
			col0.x = 1.0f - yy - zz; col0.y = xy + zw;        col0.z = xz - yw;
			col1.x = xy - zw;        col1.y = 1.0f - xx - zz; col1.z = yz + xw;
			col2.x = xz + yw;        col2.y = yz - xw;        col2.z = 1.0f - xx - yy;
		}
		
		/**
		 * @brief Initialise from a 3x3 rotation matrix and a position vector.
		 * @param in_r The 3x3 rotation matrix to set this matrix from.
		 * @param in_p The position vector to set the fourth column from.
		 */
		explicit CX_INLINE CxMat4x4(const CxMat3x3 &in_r, const CxVec3 &in_p)
			: col0(in_r.col0, 0.0f), col1(in_r.col1, 0.0f), col2(in_r.col2, 0.0f), col3(in_p, 1.0f) {}

		/**
		 * @brief Initialise the columns of the matrix.
		 * @param in_c0 The first column of the matrix.
		 * @param in_c1 The second column of the matrix.
		 * @param in_c2 The third column of the matrix.
		 * @param in_c3 The fourth column of the matrix.
		 */
		CX_INLINE CxMat4x4(const CxVec4 &in_c0, const CxVec4 &in_c1,
								 const CxVec4 &in_c2, const CxVec4 &in_c3)
			: col0(in_c0), col1(in_c1), col2(in_c2), col3(in_c3) {}

		/**
		 * @brief Initialise from basis vectors and translation vector.
		 * @param in_x The first column (x-axis) of the matrix.
		 * @param in_y The second column (y-axis) of the matrix.
		 * @param in_z The third column (z-axis) of the matrix.
		 * @param in_t The fourth column (translation) of the matrix.
		 */
		CX_INLINE CxMat4x4(const CxVec3 &in_x, const CxVec3 &in_y,
								 const CxVec3 &in_z, const CxVec3 &in_t)
			: col0(in_x, 0.0f), col1(in_y, 0.0f), col2(in_z, 0.0f), in_c3(in_t, 1.0f) {}

		/**
		 * @brief Initialise all the elements of the matrix.
		 */
		CX_INLINE CxMat4x4(CxReal in_m00, CxReal in_m01, CxReal in_m02, CxReal in_m03,
								 CxReal in_m10, CxReal in_m11, CxReal in_m12, CxReal in_m13,
								 CxReal in_m20, CxReal in_m21, CxReal in_m22, CxReal in_m23,
								 CxReal in_m30, CxReal in_m31, CxReal in_m32, CxReal in_m33)
			: col0(in_m00, in_m01, in_m02, in_m03), col1(in_m10, in_m11, in_m12, in_m13),
			  col2(in_m20, in_m21, in_m22, in_m23), col3(in_m30, in_m31, in_m32, in_m33) {}

		/** @brief Copy constructor */
		CX_INLINE CxMat4x4(const CxMat4x4 &in_m)
			: col0(in_m.col0), col1(in_m.col1), col2(in_m.col2), col3(in_m.col3) {}

		/** @brief Assignment operator */
		CX_INLINE CxMat4x4 & operator=(const CxMat4x4 &in_m) {
			col0 = in_m.col0; col1 = in_m.col1;
			col2 = in_m.col2; col3 = in_m.col3;
			return *this;
		}

		/** @return True if the two matrices are strictly equal. */
		CX_INLINE CxBool operator==(const CxMat4x4 &in_m) const {
			return ((col0 == in_m.col0) && (col1 == in_m.col1) &&
					  (col2 == in_m.col2) && (col3 == in_m.col3));
		}

		/** @return True if the two matrices are strictly not equal. */
		CX_INLINE CxBool operator!=(const CxMat4x4 &in_m) const {
			return ((col0 != in_m.col0) || (col1 != in_m.col1) ||
					  (col2 != in_m.col2) || (col3 != in_m.col3));
		}

		/** @brief Array access of the columns of the matrix. */
		CX_FORCE_INLINE CxVec4 & operator[](CxI32 in_idx) { return (&col0)[in_idx]; }

		/** @brief Array access of the columns of the matrix. */
		CX_FORCE_INLINE const CxVec4 & operator[](CxI32 in_idx) const { return (&col0)[in_idx]; }

		/**
		 * @brief Access an element of the matrix by it's column and row.
		 * @param in_col The index of the column to access [0-3].
		 * @param in_row The index of the row to access [0-3].
		 * @return The value at m[col][row].
		 */
		CX_FORCE_INLINE CxReal & operator()(CxI32 in_col, CxI32 in_row) {
			return (*this)[in_col][in_row];
		}
		/** @see operator(CxI32, CxI32) */
		CX_FORCE_INLINE const CxReal & operator()(CxI32 in_col, CxI32 in_row) const {
			return (*this)[in_col][in_row];
		}

		/** @return A copy of the matrix with all entries negated. */
		CX_INLINE CxMat4x4 operator-() const {
			return CxMat4x4(-col0, -col1, -col2, -col3);
		}

		/** @return The result of the matrix addition of (this + in_m). */
		CX_INLINE CxMat4x4 operator+(const CxMat4x4 &in_m) const {
			return CxMat4x4(col0 + in_m.col0, col1 + in_m.col1,
								 col2 + in_m.col2, col3 + in_m.col3);
		}

		/** @return The result of the matrix subtraction of (this - in_m) */
		CX_INLINE CxMat4x4 operator-(const CxMat4x4 &in_m) const {
			return CxMat4x4(col0 - in_m.col0, col1 - in_m.col1,
								 col2 - in_m.col2, col3 - in_m.col3);
		}

		/** @return The result of multiplying this matrix by a scalar. */
		CX_INLINE CxMat4x4 operator*(CxReal in_s) const {
			return CxMat4x4(col0*in_s, col1*in_s, col2*in_s, col3*in_s);
		}

		/** @return The result of the vector v transformed by this matrix (M*v). */
		CX_INLINE CxVec4 operator*(const CxVec4 &in_v) const {
			const CxReal x = in_v.x; const CxReal y = in_v.y; const CxReal z = in_v.z; const CxReal w = in_v.w;
			return CxVec4(col0.x*x + col1.x*y + col2.x*z + col3.x*w,
							  col0.y*x + col1.y*y + col2.y*z + col3.y*w,
							  col0.z*x + col1.z*y + col2.z*z + col3.z*w,
							  col0.w*x + col1.w*y + col2.w*z + col3.w*w);
		}

		/** @return The result of the vector v transformed by this matrix (M*v). */
		CX_INLINE CxVec3 operator*(const CxVec3 &in_v) const {
			const CxReal x = in_v.x; const CxReal y = in_v.y; const CxReal z = in_v.z;
			return CxVec3(col0.x*x + col1.x*y + col2.x*z + col3.x,
							  col0.y*x + col1.y*y + col2.y*z + col3.y,
							  col0.z*x + col1.z*y + col2.z*z + col3.z);
		}

		/** @return The result of the matrix multiplication (this * in_m). */
		CX_INLINE CxMat4x4 operator*(const CxMat4x4 &in_m) const {
			Mat4x4 r;
			const CxReal m00 = in_m.col0.x; const CxReal m01 = in_m.col0.y;
			const CxReal m02 = in_m.col0.z; const CxReal m03 = in_m.col0.w;
			const CxReal m10 = in_m.col1.x; const CxReal m11 = in_m.col1.y;
			const CxReal m12 = in_m.col1.z; const CxReal m13 = in_m.col1.w;
			const CxReal m20 = in_m.col2.x; const CxReal m21 = in_m.col2.y;
			const CxReal m22 = in_m.col2.z; const CxReal m23 = in_m.col2.w;
			const CxReal m30 = in_m.col3.x; const CxReal m31 = in_m.col3.y;
			const CxReal m32 = in_m.col3.z; const CxReal m33 = in_m.col3.w;
			
			const CxReal c0x = col0.x; const CxReal c1x = col1.x;
			const CxReal c2x = col2.x; const CxReal c3x = col3.x;
			
			r.c0l0.x = (c0x*m00) + (c1x*m01) + (c2x*m02) + (c3x*m03);
			r.col1.x = (c0x*m10) + (c1x*m11) + (c2x*m12) + (c3x*m13);
			r.col2.x = (c0x*m20) + (c1x*m21) + (c2x*m22) + (c3x*m23);
			r.col3.x = (c0x*m30) + (c1x*m31) + (c2x*m32) + (c3x*m33);

			const CxReal c0y = col0.y; const CxReal c1y = col1.y;
			const CxReal c2y = col2.y; const CxReal c3y = col3.y;
			r.col0.y = (c0y*m00) + (c1y*m01) + (c2y*m02) + (c3y*m03);
			r.col1.y = (c0y*m10) + (c1y*m11) + (c2y*m12) + (c3y*m13);
			r.col2.y = (c0y*m20) + (c1y*m21) + (c2y*m22) + (c3y*m23);
			r.col3.y = (c0y*m30) + (c1y*m31) + (c2y*m32) + (c3y*m33);

			const CxReal c0z = col0.z; const CxReal c1z = col1.z;
			const CxReal c2z = col2.z; const CxReal c3z = col3.z;
			r.col0.z = (c0z*m00) + (c1z*m01) + (c2z*m02) + (c3z*m03);
			r.col1.z = (c0z*m10) + (c1z*m11) + (c2z*m12) + (c3z*m13);
			r.col2.z = (c0z*m20) + (c1z*m21) + (c2z*m22) + (c3z*m23);
			r.col3.z = (c0z*m30) + (c1z*m31) + (c2z*m32) + (c3z*m33);

			const CxReal c0w = col0.w; const CxReal c1w = col1.w;
			const CxReal c2w = col2.w; const CxReal c3w = col3.w;
			r.col0.w = (c0w*m00) + (c1w*m01) + (c2w*m02) + (c3w*m03);
			r.col1.w = (c0w*m10) + (c1w*m11) + (c2w*m12) + (c3w*m13);
			r.col2.w = (c0w*m20) + (c1w*m21) + (c2w*m22) + (c3w*m23);
			r.col3.w = (c0w*m30) + (c1w*m31) + (c2w*m32) + (c3w*m33);

			return r;
		}

		/** @return The result of dividing this matrix by a scalar. */
		CX_INLINE CxMat4x4 operator/(CxReal in_s) const {
			in_s = 1.0f/in_s;
			return CxMat4x4(col0*in_s, col1*in_s, col2*in_s, col3*in_s);
		}

		/** @brief Add another matrix to this matrix. */
		CX_INLINE CxMat4x4 & operator+=(const CxMat4x4 &in_m) {
			col0 += in_m.col0; col1 += in_m.col1;
			col2 += in_m.col2; col3 += in_m.col3;
			return *this;
		}

		/** @brief Subtract another matrix from this matrix. */
		CX_INLINE CxMat4x4 & operator-=(const CxMat4x4 &in_m) {
			col0 -= in_m.col0; col1 -= in_m.col1;
			col2 -= in_m.col2; col3 -= in_m.col3;
			return *this;
		}

		/** @brief Multiply this matrix by a scalar */
		CX_INLINE CxMat4x4 & operator*=(CxReal in_s) {
			col0 *= in_s; col1 *= in_s;
			col2 *= in_s; col3 *= in_s;
			return *this;
		}

		/** @brief Multiply this matrix by another matrix. */
		CX_INLINE CxMat4x4 & operator*=(const CxMat4x4 &in_m) {
			const CxReal m00 = in_m.col0.x; const CxReal m01 = in_m.col0.y;
			const CxReal m02 = in_m.col0.z; const CxReal m03 = in_m.col0.w;
			const CxReal m10 = in_m.col1.x; const CxReal m11 = in_m.col1.y;
			const CxReal m12 = in_m.col1.z; const CxReal m13 = in_m.col1.w;
			const CxReal m20 = in_m.col2.x; const CxReal m21 = in_m.col2.y;
			const CxReal m22 = in_m.col2.z; const CxReal m23 = in_m.col2.w;
			const CxReal m30 = in_m.col3.x; const CxReal m31 = in_m.col3.y;
			const CxReal m32 = in_m.col3.z; const CxReal m33 = in_m.col3.w;
			
			const CxReal c0x = col0.x; const CxReal c1x = col1.x;
			const CxReal c2x = col2.x; const CxReal c3x = col3.x;
			
		   col0.x = (c0x*m00) + (c1x*m01) + (c2x*m02) + (c3x*m03);
			col1.x = (c0x*m10) + (c1x*m11) + (c2x*m12) + (c3x*m13);
			col2.x = (c0x*m20) + (c1x*m21) + (c2x*m22) + (c3x*m23);
			col3.x = (c0x*m30) + (c1x*m31) + (c2x*m32) + (c3x*m33);

			const CxReal c0y = col0.y; const CxReal c1y = col1.y;
			const CxReal c2y = col2.y; const CxReal c3y = col3.y;
			col0.y = (c0y*m00) + (c1y*m01) + (c2y*m02) + (c3y*m03);
			col1.y = (c0y*m10) + (c1y*m11) + (c2y*m12) + (c3y*m13);
			col2.y = (c0y*m20) + (c1y*m21) + (c2y*m22) + (c3y*m23);
			col3.y = (c0y*m30) + (c1y*m31) + (c2y*m32) + (c3y*m33);

			const CxReal c0z = col0.z; const CxReal c1z = col1.z;
			const CxReal c2z = col2.z; const CxReal c3z = col3.z;
			col0.z = (c0z*m00) + (c1z*m01) + (c2z*m02) + (c3z*m03);
			col1.z = (c0z*m10) + (c1z*m11) + (c2z*m12) + (c3z*m13);
			col2.z = (c0z*m20) + (c1z*m21) + (c2z*m22) + (c3z*m23);
			col3.z = (c0z*m30) + (c1z*m31) + (c2z*m32) + (c3z*m33);

			const CxReal c0w = col0.w; const CxReal c1w = col1.w;
			const CxReal c2w = col2.w; const CxReal c3w = col3.w;
			col0.w = (c0w*m00) + (c1w*m01) + (c2w*m02) + (c3w*m03);
			col1.w = (c0w*m10) + (c1w*m11) + (c2w*m12) + (c3w*m13);
			col2.w = (c0w*m20) + (c1w*m21) + (c2w*m22) + (c3w*m23);
			col3.w = (c0w*m30) + (c1w*m31) + (c2w*m32) + (c3w*m33);

			return *this;
		}

		/** @brief Divide this matrix by a scalar. */
		CX_INLINE CxMat4x4 & operator/=(CxReal in_s) {
			in_s = 1.0f/in_s;
			col0 *= in_s; col1 *= in_s; col2 *= in_s; col3 *= in_s;
			return *this;
		}

		/** @return A pointer to the first element of the matrix as an array. */
		CX_FORCE_INLINE CxReal * asArray() {
			return reinterpret_cast<CxReal *>(&col0);
		}

		/** @return A pointer to the first element of the matrix as a constant array. */
		CX_FORCE_INLINE const CxReal * asArray() const {
			return reinterpret_cast<const CxReal *>(&col0);
		}

		/** @return A pointer to the first element of the matrix as a constant array. */
		CX_FORCE_INLINE const CxReal * asConstArray() const {
			return reinterpret_cast<const CxReal *>(&col0);
		}

		/** @return The determinant for an invertable matrix. */
		CxReal determinant() const;

		/** @return The inverse of the matrix */
		CxMat4x4 inverse() const;

		/** @brief Invert this matrix. */
		CxMat4x4 & invert();

		/** @return The upper left 3x3 rotational matrix from this matrix. */
		CX_INLINE CxMat3x3 mat3x3() const {
			return CxMat3x3(col0.x, col0.y, col0.z,
								 col1.x, col1.y, col1.z,
								 col2.x, col2.y, col2.z);
		}

		/** @return A quaternion representing the 3x3 rotational component of the matrix. */
		CX_INLINE CxQuat quaternion() const {
			CX_TODO("Implement this.");
		}
		
		/**
		 * @brief Rotate a vector by the 3x3 rotational portion of the matrix.
		 * @param in_v The vector to rotate.
		 * @return A copy of the vector, rotated by the 3x3 rotational matrix.
		 */
		CX_INLINE CxVec4 & rotate(const CxVec4 &in_v) const {
			return (col0*in_v.x) + (col1*in_v.y) + (col2*in_v.z);
		}

		/**
		 * @brief Rotate a vector by the 3x3 rotational portion of the matrix.
		 * @param in_v The vector to rotate.
		 * @return A copy of the vector, rotated by the 3x3 rotational matrix.
		 */
		CX_INLINE CxVec3 & rotate(const CxVec3 &in_v) const {
			const CxReal x = in_v.x; const CxReal y = in_v.y; const CxReal z = in_v.z;
			return CxVec3(col0.x*x + col1.x*y + col2.x*z,
							  col0.y*x + col1.y*y + col2.y*z,
							  col0.z*x + col1.z*y + col2.z*z);
		}

		/**
		 * @brief Set the matrix to an orthographic projection matrix.
		 * @param in_r The + x-coord of the viewing rectangle.
		 * @param in_l The - x-coord of the viewing rectangle.
		 * @param in_t The + y-coord of the viewing rectangle.
		 * @param in_b The - y-coord of the viewing rectangle.
		 * @param in_n The near clipping plane.
		 * @param in_f The far clipping plane.
		 */
		CX_INLINE void setOrthoProjection(CxReal in_r, CxReal in_l, CxReal in_t, CxReal in_b,
													 CxReal in_n, CxReal in_f) {
			const CxReal x_diff = in_r - in_l;
			const CxReal y_diff = in_t - in_b;
			const CxReal z_diff = in_f - in_b;

			col0.x = 2.0f/x_diff;
			c0l0.y = col0.z = col0.w = col1.x = 0.0f;
			col1.y = 2.0f/y_diff;
			col1.z = col1.w = col2.x = col2.y = 0.0f;
			col2.z = -2.0f/z_diff;
			col2.w = 0.0f;
			col3.x = -((in_r + in_l) / x_diff);
			col3.y = -((in_t + in_b) / y_diff);
			col3.z = -((in_f + in_n) / z_diff);
			col3.w = 1.0f;
		}

		/**
		 * @brief Set the matrix to an orthographic projection matrix centered at (0,0).
		 * @param in_w The width of the viewing rectangle.
		 * @param in_h The height of the viewing rectangle.
		 * @param in_n The near clipping plane.
		 * @param in_f The far clipping plane.
		 */
		CX_INLINE void setOrthoProjection(CxReal in_w, CxReal in_h, CxReal in_n, CxReal in_f) {
			const CxReal z_diff = in_f - in_n;

			col0.x = 2.0f/in_w;
			c0l0.y = col0.z = col0.w = col1.x = 0.0f;
			col1.y = 2.0f/in_h;
			col1.z = col1.w = col2.x = col2.y = 0.0f;
			col2.z = -2.0f/z_diff;
			col2.w = col3.x = col3.y = 0.0f;
			col3.z = -((in_f + in_n) / z_diff);
			col3.w = 1.0f;
		}

		/**
		 * @brief Set the matrix to a perspective projection matrix.
		 * @param in_r The + x-coord of the viewing rectangle.
		 * @param in_l The - x-coord of the viewing rectangle.
		 * @param in_t The + y-coord of the viewing rectangle.
		 * @param in_b The - y-coord of the viewing rectangle.
		 * @param in_n The near clipping plane.
		 * @param in_f The far clipping plane.
		 */
		CX_INLINE void setPerspectiveProjection(CxReal in_r, CxReal in_l, CxReal in_t, CxReal in_b,
															 CxReal in_n, CxReal in_f) {
			const CxReal x_diff = in_r - in_l;
			const CxReal y_diff = in_t - in_b;
			const CxReal z_diff = in_f - in_b;

			col0.x = (2*in_n) / x_diff;
			col0.y = col0.z = col0.w = col1.x = 0.0f;
			col1.y = (2*in_n) / y_diff;
			col1.z = col1.w = 0.0f;
			col2.x = (in_r + in_l) / x_diff;
			col2.y = (in_t + in_b) / y_diff;
			col2.z = -( (in_f + in_n) / z_diff);
			col2.w = -1.0f;
			col3.x = col3.y = 0.0f;
			col3.z = -((2*in_f*in_r) / z_diff);
			col3.w = 1.0f;
		}

		/**
		 * @brief Set the matrix to a perspective projection matrix.
		 * @param in_fov The field of view for the viewing frustrum.
		 * @param in_aspect The aspect ratio (width/height) of the viewing frustrum.
		 * @param in_n The near clipping plane.
		 * @param in_f The far clipping plane.
		 */
		CX_FORCE_INLINE setPerspectiveProjection(CxReal in_fov, CxReal in_aspect, CxReal in_n, CxReal in_f) {
			const CxReal top = CxTan(CxDegToRad*in_fov*0.5f) * in_n;
			const CxReal right = in_aspect * top;
			setPerspectiveProjection(right, -right, top, -top, in_n, in_f);
		}

		/** @brief Set the translation portion of the matrix [c3.x, c3.y, c3.z] */
		CX_FORCE_INLINE void setTranslation(const CxVec3 &in_t) {
			col3.x = in_t.x; col3.y = in_t.y; col3.z = in_t.z;
		}

		/** @return The translation portion of the matrix [c3.x, c3.y, c3.z] */
		CX_FORCE_INLINE CxVec3 translation() const {
			return CxVec3(col3.x, col3.y, col3.z);
		}

		/** @brief Transpose this matrix. */
		CX_INLINE CxMat4x4 & transpose() {
			const CxReal c0y = col1.x; const CxReal c0z = col2.x; const CxReal c0w = col3.x;
			const CxReal c1x = col0.y; const CxReal c1z = col2.y; const CxReal c1w = col3.y;
			const CxReal c2x = col0.z; const CxReal c2y = col1.z; const CxReal c2w = col2.w;
			const CxReal c3x = col0.w; const CxReal c3y = col1.w; const CxReal c3z = col2.w;

			col0.y = c0y; col0.z = c0z; col0.w = c0w;
			col1.x = c1x; col1.z = c1z; col1.w = c1w;
			col2.x = c2x; col2.y = c2y; col2.w = c2w;
			col3.x = c3x; col3.y = c3y; col3.z = c3z;
			
			return *this;
		}

		/** @return The transpose of this matrix. */
		CX_INLINE CxMat4x4 transposed() const {
			return CxMat4x4(CxVec4(col0.x, col1.x, col2.x, col3.x),
								 CxVec4(col0.y, col1.y, col2.y, col3.y),
								 CxVec4(col0.z, col1.z, col2.z, col3.z),
								 CxVec3(c0l0.w, col1.w, col2.w, col3.w));
		}
	};

	/** @return The result of multiplying this matrix by a scalar. */
	CX_INLINE CxMat4x4 operator*(CxReal in_s, const CxMat4x4 &in_m) const {
		return CxMat4x4(in_s*in_m.col0, in_s*in_m.col1, in_s*in_m.col2, in_s*in_m.col3);
	}
	
} // namespace cat


#endif // CX_CORE_MATH_CXMAT4x4_H
