#ifndef CAT_CORE_MATH_MAT4_H
#define CAT_CORE_MATH_MAT4_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * mat4.h: Contains the Mat4 object which represents a 4x4 Matrix.
 * Also contains math functions to operate on matrices and other objects.
 *
 * Author: Catlin Zilinski
 * Date: Dec 4, 2013
 */

#include "core/math/mathcore.h"
#include "core/math/quaternion.h"
#include "core/math/vec4.h"

namespace Cat {

	/**
	 * @class Mat4 mat4.h "core/math/mat4.h"
	 * @brief A class representing a 4x4 Matrix.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since June 4, 2013
	 */
	class Mat4 {
	  public:
		union MatrixData {
			struct {
				Real   m11, m12, m13, m14,
					m21, m22, m23, m24,
					m31, m32, m33, m34,
					m41, m42, m43, m44;
			};
			Real m[16];
		};

		enum TransformationType {
			kMTranslation = 0x1,
			kMRotation = 0x2,
			kMScale = 0x4,
		};		

		const static Mat4 kIdentity;
		const static Real kEpsilon;
		
		
		/**
		 * @brief Create a new empty matrix.
		 */
		inline Mat4() { }

		/**
		 * @brief Create a new matrix from the specified components.
		 */
		inline Mat4(const Real m11, const Real m12, const Real m13, const Real m14, 
						const Real m21, const Real m22, const Real m23, const Real m24,
						const Real m31, const Real m32, const Real m33, const Real m34,
						const Real m41, const Real m42, const Real m43, const Real m44) {
			m_d.m[0] = m11;
			m_d.m[1] = m12;
			m_d.m[2] = m13;
			m_d.m[3] = m14;
			m_d.m[4] = m21;
			m_d.m[5] = m22;
			m_d.m[6] = m23;
			m_d.m[7] = m24;
			m_d.m[8] = m31;
			m_d.m[9] = m32;
			m_d.m[10] = m33;
			m_d.m[11] = m34;
			m_d.m[12] = m41;
			m_d.m[13] = m42;
			m_d.m[14] = m43;
			m_d.m[15] = m44;
		}

		/**
		 * @brief Create a new matrix from the specified value.
		 * @param value The value to set the components to.
		 */
		inline explicit Mat4(const Real value) {
			m_d.m[0] = value;
			m_d.m[1] = value;
			m_d.m[2] = value;
			m_d.m[3] = value;
			m_d.m[4] = value;
			m_d.m[5] = value;
			m_d.m[6] = value;
			m_d.m[7] = value;
			m_d.m[8] = value;
			m_d.m[9] = value;
			m_d.m[10] = value;
			m_d.m[11] = value;
			m_d.m[12] = value;
			m_d.m[13] = value;
			m_d.m[14] = value;
			m_d.m[15] = value;
		}

		/**
		 * @brief Create a new matrix from an array.
		 *
		 * The array should be of length 16 and contain a list
		 * of column vectors for the columns of the matrix.
		 *
		 * @param columns The array of column vectors for the matrix.
		 */
		inline explicit Mat4(const Real* columns) {
			m_d.m[0] = columns[0];
			m_d.m[1] = columns[1];
			m_d.m[2] = columns[2];
			m_d.m[3] = columns[3];
			m_d.m[4] = columns[4];
			m_d.m[5] = columns[5];
			m_d.m[6] = columns[6];
			m_d.m[7] = columns[7];
			m_d.m[8] = columns[8];
			m_d.m[9] = columns[9];
			m_d.m[10] = columns[10];
			m_d.m[11] = columns[11];
			m_d.m[12] = columns[12];
			m_d.m[13] = columns[13];
			m_d.m[14] = columns[14];
			m_d.m[15] = columns[15];
		}
		

		/**
		 * @brief Creates a new 4x4 matrix with 0 translation from a 3x3 matrix.
		 * @param mat3 The 3x3 matrix to copy from.
		 */
		explicit inline Mat4(const Mat3& mat3) {
			operator=(mat3);

			// Initialize the rest of the matrix
			m_d.m[3] = REAL(0.0);
			m_d.m[7] = REAL(0.0);
			m_d.m[11] = REAL(0.0);			
			m_d.m[12] = m_d.m[13] = m_d.m[14] = REAL(0.0);
			m_d.m[15] = REAL(1.0);
		}

		/**
		 * @brief Create a new matrix with 0 translation from a quaternion.
		 * @param q The quaternion to create the matrix from.
		 */
		explicit inline Mat4(const Quaternion& q) {
			Mat3 m3;
			q.toMatrix(m3);
			operator=(m3);

		 	// Initialize the rest of the matrix
		   m_d.m[3] = REAL(0.0);
			m_d.m[7] = REAL(0.0);
			m_d.m[11] = REAL(0.0);			
			m_d.m[12] = m_d.m[13] = m_d.m[14] = REAL(0.0);
			m_d.m[15] = REAL(1.0);
		}
		
	
		/**
		 * @brief Overloaded assignment operator for copying a 3x3 matrix.
		 * @param src The source 3x3 matrix to copy from.
		 * @return A reference to this matrix.
		 */
		inline Mat4& operator=(const Mat3& src) {
			// Set the rotation part of the matrix to the 3x3 matrix.
			m_d.m[0] = src.m_d.m[0]; m_d.m[1] = src.m_d.m[1]; m_d.m[2] = src.m_d.m[2];
			m_d.m[4] = src.m_d.m[3]; m_d.m[5] = src.m_d.m[4]; m_d.m[6] = src.m_d.m[5];
			m_d.m[8] = src.m_d.m[6]; m_d.m[9] = src.m_d.m[7]; m_d.m[10] = src.m_d.m[8];
			return *this;			
		}

		/**
		 * @brief Operator to check for matrix equality.
		 * @param rhs The matrix to check against.
		 * @return True if the matrices are equal.
		 */
		inline Boolean operator==(const Mat4& rhs) const {
			return (m_d.m[0] == rhs.m_d.m[0] && m_d.m[1] == rhs.m_d.m[1] &&
					  m_d.m[2] == rhs.m_d.m[2] && m_d.m[3] == rhs.m_d.m[3] &&

					  m_d.m[4] == rhs.m_d.m[4] && m_d.m[5] == rhs.m_d.m[5] &&
					  m_d.m[6] == rhs.m_d.m[6] && m_d.m[7] == rhs.m_d.m[7] &&

					  m_d.m[8] == rhs.m_d.m[8] && m_d.m[9] == rhs.m_d.m[9] &&
					  m_d.m[10] == rhs.m_d.m[10] && m_d.m[11] == rhs.m_d.m[11] &&

					  m_d.m[12] == rhs.m_d.m[12] && m_d.m[13] == rhs.m_d.m[13] &&
					  m_d.m[14] == rhs.m_d.m[14] && m_d.m[15] == rhs.m_d.m[15]);
		}

		/**
		 * @brief Operator to check for matrix inequality.
		 * @param rhs The matrix to check against.
		 * @return True if the matrices are not equal.
		 */
		inline Boolean operator!=(const Mat4& rhs) const {
			return (m_d.m[0] != rhs.m_d.m[0] || m_d.m[1] != rhs.m_d.m[1] ||
					  m_d.m[2] != rhs.m_d.m[2] || m_d.m[3] != rhs.m_d.m[3] ||

					  m_d.m[4] != rhs.m_d.m[4] || m_d.m[5] != rhs.m_d.m[5] ||
					  m_d.m[6] != rhs.m_d.m[6] || m_d.m[7] != rhs.m_d.m[7] ||

					  m_d.m[8] != rhs.m_d.m[8] || m_d.m[9] != rhs.m_d.m[9] ||
					  m_d.m[10] != rhs.m_d.m[10] || m_d.m[11] != rhs.m_d.m[11] ||

					  m_d.m[12] != rhs.m_d.m[12] || m_d.m[13] != rhs.m_d.m[13] ||
					  m_d.m[14] != rhs.m_d.m[14] || m_d.m[15] != rhs.m_d.m[15]);
		}

		/**
		 * @brief Check for approximate matrix equality.
		 * @param rhs The matrix to check for approximate equality with.
		 * @todo: Test to see if using the sum of the differences would
		 * be faster than comparing each pair of components.
		 * @return True if the matrices are approximately equal.
		 */
		inline Boolean approx(const Mat4& rhs) const {			
			return (fabs(m_d.m[0] - rhs.m_d.m[0]) < Mat4::kEpsilon && 
					  fabs(m_d.m[1] - rhs.m_d.m[1]) < Mat4::kEpsilon && 
					  fabs(m_d.m[2] - rhs.m_d.m[2]) < Mat4::kEpsilon &&
					  fabs(m_d.m[3] - rhs.m_d.m[3]) < Mat4::kEpsilon &&

					  fabs(m_d.m[4] - rhs.m_d.m[4]) < Mat4::kEpsilon &&
					  fabs(m_d.m[5] - rhs.m_d.m[5]) < Mat4::kEpsilon &&
					  fabs(m_d.m[6] - rhs.m_d.m[6]) < Mat4::kEpsilon &&
					  fabs(m_d.m[7] - rhs.m_d.m[7]) < Mat4::kEpsilon &&

					  fabs(m_d.m[8] - rhs.m_d.m[8]) < Mat4::kEpsilon &&
					  fabs(m_d.m[9] - rhs.m_d.m[9]) < Mat4::kEpsilon &&
					  fabs(m_d.m[10] - rhs.m_d.m[10]) < Mat4::kEpsilon &&
					  fabs(m_d.m[11] - rhs.m_d.m[11]) < Mat4::kEpsilon &&
					  fabs(m_d.m[12] - rhs.m_d.m[12]) < Mat4::kEpsilon &&
					  fabs(m_d.m[13] - rhs.m_d.m[13]) < Mat4::kEpsilon &&
					  fabs(m_d.m[14] - rhs.m_d.m[14]) < Mat4::kEpsilon &&
					  fabs(m_d.m[15] - rhs.m_d.m[15]) < Mat4::kEpsilon);
			
		}

		/**
		 * @brief Get a reference to the MatrixData object.
		 * @return A reference to the MatrixData object.
		 */
		inline MatrixData& getMatrixData() { return m_d; }
		
		/**
		 * @brief Get a constant reference to the MatrixData object.
		 * @return A const Reference to the MatrixData obejct.
		 */
		inline const MatrixData& getMatrixDataConst() const { return m_d; }

		/**
		 * @brief Operator for component-wise Matrix addition.
		 * @param rhs The matrix to be added.
		 * @return A new matrix that is the result of this + rhs.
		 */
		inline Mat4 operator+(const Mat4& rhs) const {
			return Mat4( 
				m_d.m[0] + rhs.m_d.m[0], m_d.m[1] + rhs.m_d.m[1], m_d.m[2] + rhs.m_d.m[2], m_d.m[3] + rhs.m_d.m[3],
				m_d.m[4] + rhs.m_d.m[4], m_d.m[5] + rhs.m_d.m[5], m_d.m[6] + rhs.m_d.m[6], m_d.m[7] + rhs.m_d.m[7],
				m_d.m[8] + rhs.m_d.m[8], m_d.m[9] + rhs.m_d.m[9], m_d.m[10] + rhs.m_d.m[10], m_d.m[11] + rhs.m_d.m[11],
				m_d.m[12] + rhs.m_d.m[12], m_d.m[13] + rhs.m_d.m[13], m_d.m[14] + rhs.m_d.m[14], m_d.m[15] + rhs.m_d.m[15]
				);
		}
		
		/**
		 * @brief Operator for component-wise Matrix subtraction.
		 * @param rhs The matrix to be subtracted.
		 * @return A new matrix that is the result of this - rhs.
		 */
		inline Mat4 operator-(const Mat4& rhs) const {
			return Mat4( 
				m_d.m[0] - rhs.m_d.m[0], m_d.m[1] - rhs.m_d.m[1], m_d.m[2] - rhs.m_d.m[2], m_d.m[3] - rhs.m_d.m[3],
				m_d.m[4] - rhs.m_d.m[4], m_d.m[5] - rhs.m_d.m[5], m_d.m[6] - rhs.m_d.m[6], m_d.m[7] - rhs.m_d.m[7],
				m_d.m[8] - rhs.m_d.m[8], m_d.m[9] - rhs.m_d.m[9], m_d.m[10] - rhs.m_d.m[10], m_d.m[11] - rhs.m_d.m[11],
				m_d.m[12] - rhs.m_d.m[12], m_d.m[13] - rhs.m_d.m[13], m_d.m[14] - rhs.m_d.m[14], m_d.m[15] - rhs.m_d.m[15]
				);
		}

		/**
		 * @brief Operator to add another Matrix to this one, component-wise.
		 * @param rhs The matrix to be added to this one.
		 * @return A reference to this matrix.
		 */
		inline Mat4 operator+=(const Mat4& rhs) {
			m_d.m[0] += rhs.m_d.m[0]; m_d.m[1] += rhs.m_d.m[1]; m_d.m[2] += rhs.m_d.m[2]; m_d.m[3] += rhs.m_d.m[3];
			m_d.m[4] += rhs.m_d.m[4]; m_d.m[5] += rhs.m_d.m[5]; m_d.m[6] += rhs.m_d.m[6]; m_d.m[7] += rhs.m_d.m[7];
			m_d.m[8] += rhs.m_d.m[8]; m_d.m[9] += rhs.m_d.m[9]; m_d.m[10] += rhs.m_d.m[10]; m_d.m[11] += rhs.m_d.m[11];
			m_d.m[12] += rhs.m_d.m[12]; m_d.m[13] += rhs.m_d.m[13]; m_d.m[14] += rhs.m_d.m[14]; m_d.m[15] += rhs.m_d.m[15];
			return *this;
		}
		
		/**
		 * @brief Operator to subtract another Matrix from this one, component-wise.
		 * @param rhs The matrix to be subtracted from this one.
		 * @return A reference to this matrix.
		 */
		inline Mat4 operator-=(const Mat4& rhs) {
			m_d.m[0] -= rhs.m_d.m[0]; m_d.m[1] -= rhs.m_d.m[1]; m_d.m[2] -= rhs.m_d.m[2]; m_d.m[3] -= rhs.m_d.m[3];
			m_d.m[4] -= rhs.m_d.m[4]; m_d.m[5] -= rhs.m_d.m[5]; m_d.m[6] -= rhs.m_d.m[6]; m_d.m[7] -= rhs.m_d.m[7];
			m_d.m[8] -= rhs.m_d.m[8]; m_d.m[9] -= rhs.m_d.m[9]; m_d.m[10] -= rhs.m_d.m[10]; m_d.m[11] -= rhs.m_d.m[11];
			m_d.m[12] -= rhs.m_d.m[12]; m_d.m[13] -= rhs.m_d.m[13]; m_d.m[14] -= rhs.m_d.m[14]; m_d.m[15] -= rhs.m_d.m[15];
			return *this;
		}
		
		/**
		 * @brief Overloaded multiplication operator to multiply two matrices.
		 * @param rhs The matrix to multiply by.
		 * @return A new matrix that is the result of this * rhs.
		 */
		inline Mat4 operator*(const Mat4& rhs) const {
			Mat4 r;			
			r.m_d.m11 = (m_d.m11*rhs.m_d.m11) + (m_d.m21 * rhs.m_d.m12) + (m_d.m31 * rhs.m_d.m13) + (m_d.m41 * rhs.m_d.m14);
			r.m_d.m21 = (m_d.m11*rhs.m_d.m21) + (m_d.m21 * rhs.m_d.m22) + (m_d.m31 * rhs.m_d.m23) + (m_d.m41 * rhs.m_d.m24);
			r.m_d.m31 = (m_d.m11*rhs.m_d.m31) + (m_d.m21 * rhs.m_d.m32) + (m_d.m31 * rhs.m_d.m33) + (m_d.m41 * rhs.m_d.m34);
			r.m_d.m41 = (m_d.m11*rhs.m_d.m41) + (m_d.m21 * rhs.m_d.m42) + (m_d.m31 * rhs.m_d.m43) + (m_d.m41 * rhs.m_d.m44);
		
			r.m_d.m12 = (m_d.m12*rhs.m_d.m11) + (m_d.m22 * rhs.m_d.m12) + (m_d.m32 * rhs.m_d.m13) + (m_d.m42 * rhs.m_d.m14);
			r.m_d.m22 = (m_d.m12*rhs.m_d.m21) + (m_d.m22 * rhs.m_d.m22) + (m_d.m32 * rhs.m_d.m23) + (m_d.m42 * rhs.m_d.m24);
			r.m_d.m32 = (m_d.m12*rhs.m_d.m31) + (m_d.m22 * rhs.m_d.m32) + (m_d.m32 * rhs.m_d.m33) + (m_d.m42 * rhs.m_d.m34);
			r.m_d.m42 = (m_d.m12*rhs.m_d.m41) + (m_d.m22 * rhs.m_d.m42) + (m_d.m32 * rhs.m_d.m43) + (m_d.m42 * rhs.m_d.m44);

			r.m_d.m13 = (m_d.m13*rhs.m_d.m11) + (m_d.m23 * rhs.m_d.m12) + (m_d.m33 * rhs.m_d.m13) + (m_d.m43 * rhs.m_d.m14);
			r.m_d.m23 = (m_d.m13*rhs.m_d.m21) + (m_d.m23 * rhs.m_d.m22) + (m_d.m33 * rhs.m_d.m23) + (m_d.m43 * rhs.m_d.m24);
			r.m_d.m33 = (m_d.m13*rhs.m_d.m31) + (m_d.m23 * rhs.m_d.m32) + (m_d.m33 * rhs.m_d.m33) + (m_d.m43 * rhs.m_d.m34);
			r.m_d.m43 = (m_d.m13*rhs.m_d.m41) + (m_d.m23 * rhs.m_d.m42) + (m_d.m33 * rhs.m_d.m43) + (m_d.m43 * rhs.m_d.m44);

			r.m_d.m14 = (m_d.m14*rhs.m_d.m11) + (m_d.m24 * rhs.m_d.m12) + (m_d.m34 * rhs.m_d.m13) + (m_d.m44 * rhs.m_d.m14);
			r.m_d.m24 = (m_d.m14*rhs.m_d.m21) + (m_d.m24 * rhs.m_d.m22) + (m_d.m34 * rhs.m_d.m23) + (m_d.m44 * rhs.m_d.m24);
			r.m_d.m34 = (m_d.m14*rhs.m_d.m31) + (m_d.m24 * rhs.m_d.m32) + (m_d.m34 * rhs.m_d.m33) + (m_d.m44 * rhs.m_d.m34);
			r.m_d.m44 = (m_d.m14*rhs.m_d.m41) + (m_d.m24 * rhs.m_d.m42) + (m_d.m34 * rhs.m_d.m43) + (m_d.m44 * rhs.m_d.m44);
		
			return r;
		}

		/**
		 * @brief Overloaded multiplication operator to multiply by a scalar.
		 * @param scalar The scalar to multiply by.
		 * @return A new matrix multiplied by the scalar.
		 */
		inline Mat4 operator*(const Real scalar) const {
			return Mat4( m_d.m[0] * scalar, m_d.m[1] * scalar, m_d.m[2] * scalar, m_d.m[3] * scalar,
							 m_d.m[4] * scalar, m_d.m[5] * scalar, m_d.m[6] * scalar, m_d.m[7] * scalar,
							 m_d.m[8] * scalar, m_d.m[9] * scalar, m_d.m[10] * scalar, m_d.m[11] * scalar,
							 m_d.m[12] * scalar, m_d.m[13] * scalar, m_d.m[14] * scalar, m_d.m[15] * scalar );
		}
		inline friend Mat4 operator*(const Real scalar, const Mat4& rhs) {
			return rhs * scalar;
		}
		
		/**
		 * @brief Overloaded Multiplication operator to multiply this by a scalar.
		 * @param scalar The scalar to multiply this matrix by.
		 * @return A reference to this matrix.
		 */
		inline Mat4& operator*=(const Real scalar) {			
			m_d.m[0] *= scalar; m_d.m[1] *= scalar; m_d.m[2] *= scalar; m_d.m[3] *= scalar;
			m_d.m[4] *= scalar; m_d.m[5] *= scalar; m_d.m[6] *= scalar;	m_d.m[7] *= scalar;
		   m_d.m[8] *= scalar; m_d.m[9] *= scalar; m_d.m[10] *= scalar; m_d.m[11] *= scalar;
		   m_d.m[12] *= scalar; m_d.m[13] *= scalar; m_d.m[14] *= scalar; m_d.m[15] *= scalar; 
			return *this;
		}

		/**
		 * @brief Overloaded division operator to divide by a scalar.
		 * @param scalar The scalar to divide by.
		 * @return A new matrix divided by the scalar.
		 */
		inline Mat4 operator/(const Real scalar) const {
			return Mat4( m_d.m[0] / scalar, m_d.m[1] / scalar, m_d.m[2] / scalar, m_d.m[3] / scalar,
							 m_d.m[4] / scalar, m_d.m[5] / scalar, m_d.m[6] / scalar, m_d.m[7] / scalar,
							 m_d.m[8] / scalar, m_d.m[9] / scalar, m_d.m[10] / scalar, m_d.m[11] / scalar,
							 m_d.m[12] / scalar, m_d.m[13] / scalar, m_d.m[14] / scalar, m_d.m[15] / scalar );
		}
		inline friend Mat4 operator/(const Real scalar, const Mat4& rhs) {
			return Mat4(
				scalar / rhs.m_d.m[0], scalar / rhs.m_d.m[1], scalar / rhs.m_d.m[2], scalar / rhs.m_d.m[3], 
				scalar / rhs.m_d.m[4], scalar / rhs.m_d.m[5], scalar / rhs.m_d.m[6], scalar / rhs.m_d.m[7], 
				scalar / rhs.m_d.m[8], scalar / rhs.m_d.m[9], scalar / rhs.m_d.m[10], scalar / rhs.m_d.m[11], 
				scalar / rhs.m_d.m[12], scalar / rhs.m_d.m[13], scalar / rhs.m_d.m[14], scalar / rhs.m_d.m[15]
				);
		}
		
		/**
		 * @brief Overloaded Division operator to divide this by a scalar.
		 * @param scalar The scalar to divide this matrix by.
		 * @return A reference to this matrix.
		 */
		inline Mat4& operator/=(const Real scalar) {			
			m_d.m[0] /= scalar; m_d.m[1] /= scalar; m_d.m[2] /= scalar; m_d.m[3] /= scalar;
			m_d.m[4] /= scalar; m_d.m[5] /= scalar; m_d.m[6] /= scalar;	m_d.m[7] /= scalar;
		   m_d.m[8] /= scalar; m_d.m[9] /= scalar; m_d.m[10] /= scalar; m_d.m[11] /= scalar;
		   m_d.m[12] /= scalar; m_d.m[13] /= scalar; m_d.m[14] /= scalar; m_d.m[15] /= scalar; 
			return *this;
		}
		
		/**
		 * @brief Overloaded Multiplication operator to multiply by another matrix.
		 * @param rhs The matrix to multiply this one by.
		 * @return A reference to this matrix.
		 */
		Mat4& operator*=(const Mat4& rhs) {
			Mat4::MatrixData m;

			m.m11 = (m_d.m11*rhs.m_d.m11) + (m_d.m21 * rhs.m_d.m12) + (m_d.m31 * rhs.m_d.m13) + (m_d.m41 * rhs.m_d.m14);
			m.m21 = (m_d.m11*rhs.m_d.m21) + (m_d.m21 * rhs.m_d.m22) + (m_d.m31 * rhs.m_d.m23) + (m_d.m41 * rhs.m_d.m24);
			m.m31 = (m_d.m11*rhs.m_d.m31) + (m_d.m21 * rhs.m_d.m32) + (m_d.m31 * rhs.m_d.m33) + (m_d.m41 * rhs.m_d.m34);
			m.m41 = (m_d.m11*rhs.m_d.m41) + (m_d.m21 * rhs.m_d.m42) + (m_d.m31 * rhs.m_d.m43) + (m_d.m41 * rhs.m_d.m44);
		
			m.m12 = (m_d.m12*rhs.m_d.m11) + (m_d.m22 * rhs.m_d.m12) + (m_d.m32 * rhs.m_d.m13) + (m_d.m42 * rhs.m_d.m14);
			m.m22 = (m_d.m12*rhs.m_d.m21) + (m_d.m22 * rhs.m_d.m22) + (m_d.m32 * rhs.m_d.m23) + (m_d.m42 * rhs.m_d.m24);
			m.m32 = (m_d.m12*rhs.m_d.m31) + (m_d.m22 * rhs.m_d.m32) + (m_d.m32 * rhs.m_d.m33) + (m_d.m42 * rhs.m_d.m34);
			m.m42 = (m_d.m12*rhs.m_d.m41) + (m_d.m22 * rhs.m_d.m42) + (m_d.m32 * rhs.m_d.m43) + (m_d.m42 * rhs.m_d.m44);

			m.m13 = (m_d.m13*rhs.m_d.m11) + (m_d.m23 * rhs.m_d.m12) + (m_d.m33 * rhs.m_d.m13) + (m_d.m43 * rhs.m_d.m14);
			m.m23 = (m_d.m13*rhs.m_d.m21) + (m_d.m23 * rhs.m_d.m22) + (m_d.m33 * rhs.m_d.m23) + (m_d.m43 * rhs.m_d.m24);
			m.m33 = (m_d.m13*rhs.m_d.m31) + (m_d.m23 * rhs.m_d.m32) + (m_d.m33 * rhs.m_d.m33) + (m_d.m43 * rhs.m_d.m34);
			m.m43 = (m_d.m13*rhs.m_d.m41) + (m_d.m23 * rhs.m_d.m42) + (m_d.m33 * rhs.m_d.m43) + (m_d.m43 * rhs.m_d.m44);

			m.m14 = (m_d.m14*rhs.m_d.m11) + (m_d.m24 * rhs.m_d.m12) + (m_d.m34 * rhs.m_d.m13) + (m_d.m44 * rhs.m_d.m14);
			m.m24 = (m_d.m14*rhs.m_d.m21) + (m_d.m24 * rhs.m_d.m22) + (m_d.m34 * rhs.m_d.m23) + (m_d.m44 * rhs.m_d.m24);
			m.m34 = (m_d.m14*rhs.m_d.m31) + (m_d.m24 * rhs.m_d.m32) + (m_d.m34 * rhs.m_d.m33) + (m_d.m44 * rhs.m_d.m34);
			m.m44 = (m_d.m14*rhs.m_d.m41) + (m_d.m24 * rhs.m_d.m42) + (m_d.m34 * rhs.m_d.m43) + (m_d.m44 * rhs.m_d.m44);
		
			m_d = m;		
			return *this;	
		}
		

		/**
		 * @brief Overloaded multiplication operator to multiply a vector.
		 * @param rhs The vector to multiply with.
		 * @return A new vector result.
		 */
		inline Vec3 operator*(const Vec3& rhs) const {
			return Vec3(
				(m_d.m11*rhs.x) + (m_d.m21 * rhs.y) + (m_d.m31 * rhs.z) + m_d.m41,
				(m_d.m12*rhs.x) + (m_d.m22 * rhs.y) + (m_d.m32 * rhs.z) + m_d.m42,
				(m_d.m13*rhs.x) + (m_d.m23 * rhs.y) + (m_d.m33 * rhs.z) + m_d.m43
				);			
		}

		/**
		 * @brief Overloaded multiplication operator to multiply a vector.
		 * @param rhs The vector to multiply with.
		 * @return A new vector result.
		 */
		inline Vec4 operator*(const Vec4& rhs) const {
			return Vec4(
				(m_d.m11*rhs.x) + (m_d.m21 * rhs.y) + (m_d.m31 * rhs.z) + (m_d.m41 * rhs.w),
				(m_d.m12*rhs.x) + (m_d.m22 * rhs.y) + (m_d.m32 * rhs.z) + (m_d.m42 * rhs.w),
				(m_d.m13*rhs.x) + (m_d.m23 * rhs.y) + (m_d.m33 * rhs.z) + (m_d.m43 * rhs.w),
				(m_d.m14*rhs.x) + (m_d.m24 * rhs.y) + (m_d.m34 * rhs.z) + (m_d.m44 * rhs.w)
				);			
		}
		
		/**
		 * @brief Get the transpose of the matrix.
		 * @return A matrix that is the transpose of this matrix.
		 */
		inline Mat4 transposed() const {
			return Mat4(
				m_d.m11, m_d.m21, m_d.m31, m_d.m41,
				m_d.m12, m_d.m22, m_d.m32, m_d.m42,
				m_d.m13, m_d.m23, m_d.m33, m_d.m43,
				m_d.m14, m_d.m24, m_d.m34, m_d.m44
				);
		}

		/**
		 * @brief Transpose this matrix.
		 * @return A reference to this matrix after it's been transposed.
		 */
		inline Mat4& transpose() {
			Real tmp = m_d.m12;
			m_d.m12 = m_d.m21; m_d.m21 = tmp;
			tmp = m_d.m13; m_d.m13 = m_d.m31; m_d.m31 = tmp;
			tmp = m_d.m14; m_d.m14 = m_d.m41; m_d.m41 = tmp;
			tmp = m_d.m23; m_d.m23 = m_d.m32; m_d.m32 = tmp;
			tmp = m_d.m24; m_d.m24 = m_d.m42; m_d.m42 = tmp;
			tmp = m_d.m34; m_d.m34 = m_d.m43; m_d.m43 = tmp;
			return *this;
		}

		/**
		 * @brief Get the determinent of the matrix.
		 * @return The determinant of the matrix.
		 */
		Real determinant() const;
		
		/**
		 * @brief Get the inverse of the matrix.
		 * @return A matrix that is the inverse of this one.
		 */
		Mat4 inverse() const;
		
		/**
		 * @brief Invert this matrix.
		 * @return A reference to this matrix, inverted.
		 */
		inline Mat4& invert() {
			Mat4 i = inverse();
			m_d = i.m_d;
			return *this;			
		}

		/**
		 * @brief Get the 3x3 rotation matrix from this matrix.
		 * @return The 3x3 portion of the matrix.
		 */
		inline Mat3 get3x3Matrix() const {
			return Mat3(m_d.m11, m_d.m12, m_d.m13,
							m_d.m21, m_d.m22, m_d.m23,
							m_d.m31, m_d.m32, m_d.m33);
		}

		/**
		 * @brief Get the Quaternion representing the rotation matrix.
		 * @return A Quaternion representing the rotation portion of the matrix.
		 */
		inline Quaternion getQuaternion() const {
			return Quaternion(get3x3Matrix());
		}

		/**
		 * @brief Get a vector rotated by the 3x3 rotational portion of the matrix.
		 * @param v The vector to rotate.
		 * @return A copy of v rotated by the 3x3 rotational matrix.
		 */
		inline Vec3 rotate(const Vec3& v) const {
			return Vec3(m_d.m11*v.x + m_d.m21*v.y + m_d.m31*v.z,
							m_d.m12*v.x + m_d.m22*v.y + m_d.m32*v.z,
							m_d.m13*v.x + m_d.m23*v.y + m_d.m33*v.z);
		}

		/**
		 * @brief Get a vector rotated by the inverse 3x3 rotational portion of the matrix.
		 * @param v The vector to rotate.
		 * @return A copy of v rotated by the inverse 3x3 rotational matrix.
		 */
		Vec3 rotateInverse(const Vec3& v) const;

		/**
		 * @brief Set the matrix to an orthographic projection matrix.
		 * @param p_r The right of the viewing rectangle.
		 * @param p_l The left of the viewing rectangle.
		 * @param p_t The top of the viewing rectangle.
		 * @param p_b The bottom of the viewing rectangle.
		 * @param p_n The near clipping plane.
		 * @param p_f The far clipping plane.
		 */
		void setToOrthoProjection(Real p_r, Real p_l, Real p_t, Real p_b,
										  Real p_n, Real p_f);

		/**
		 * @brief Set the matrix to an orthographic projection matrix.
		 * @param p_w The width of the 'screen'.
		 * @param p_h The height of the 'screen'.
		 * @param p_n The near clipping plane.
		 * @param p_f The far clipping plane.
		 */
		void setToOrthoProjection(Real p_w, Real p_h, Real p_n, Real p_f);

		/**
		 * @brief Set the matrix to an perspective projection matrix.
		 * @param p_r The right of the viewing frustrum.
		 * @param p_l The left of the viewing frustrum.
		 * @param p_t The top of the viewing frustrum.
		 * @param p_b The bottom of the viewing frustrum.
		 * @param p_n The near clipping plane.
		 * @param p_f The far clipping plane.
		 */
		void setToPerspectiveProjection(Real p_r, Real p_l, Real p_t, Real p_b,
												  Real p_n, Real p_f);

		/**
		 * @brief Set the matrix to an perspective projection matrix.
		 * @param p_fov The field of view in degrees.
		 * @param p_aspect The aspect ratio for the width and height.
		 * @param p_n The near clipping plane.
		 * @param p_f The far clipping plane.
		 */
		void setToPerspectiveProjection(Real p_fov, Real p_aspect, Real p_n, Real p_f);
 
	  private:
		MatrixData m_d;
		
	};

#ifdef DEBUG
	std::ostream& operator<<(std::ostream& out, const Mat4& m);
#endif //DEBUG

} // namespace Cat



#endif //CAT_CORE_MATH_MAT4_H
