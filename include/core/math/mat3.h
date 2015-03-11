#ifndef CAT_CORE_MATH_MAT3_H
#define CAT_CORE_MATH_MAT3_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * mat3.h: Contains the Mat3 object which represents a 3x3 Matrix.
 * Also contains math functions to operate on matrices and other objects.
 *
 * Author: Catlin Zilinski
 * Date: Dec 4th, 2013
 */

#include "core/math/mathcore.h"
#include "core/math/vec3.h"

namespace Cat {

	/**
	 * @class Mat3 mat3.h "core/math/mat3.h"
	 * @brief A class representing a 3x3 Matrix.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Dec 4, 2013
	 */
	class Mat3 {
	  public:
		union MatrixData {
			struct {
				Real  m11, m12, m13,
						m21, m22, m23,
						m31, m32, m33;			
			};
			Real m[9];
		};		

		static const Real kEpsilon;
		static const Mat3 kIdentity;
	
		
		/**
		 * @brief Create a new empty 3x3 matrix.
		 */
		inline Mat3() { }

		/**
		 * @brief Create a new 3x3 matrix filled with the specified value.
		 * @param value The value to fill all the components with.
		 * @todo Possibly optimize with memset?
		 */
		inline explicit Mat3(const Real value) {
			m_d.m[0] = value;
			m_d.m[1] = value;
			m_d.m[2] = value;
			m_d.m[3] = value;
			m_d.m[4] = value;
			m_d.m[5] = value;
			m_d.m[6] = value;
			m_d.m[7] = value;
			m_d.m[8] = value;
		}

		/**
		 * @brief Create a new 3x3 matrix from the values in the array passed in.
		 *
		 * The array passed in as the components for the matrix should be an 
		 * array of length 9 with the column vectors laid out sequentially
		 * (i.e., a list of the column vectors of the matrix).
		 * @param columns The array of column vectors for the matrix.
		 * @todo Possibly optimize with memcpy?
		 */
		inline explicit Mat3(const Real* columns) {
			m_d.m[0] = columns[0];
			m_d.m[1] = columns[1];
			m_d.m[2] = columns[2];
			m_d.m[3] = columns[3];
			m_d.m[4] = columns[4];
			m_d.m[5] = columns[5];
			m_d.m[6] = columns[6];
			m_d.m[7] = columns[7];
			m_d.m[8] = columns[8];
		}

		/**
		 * @brief Create a new 3x3 matrix from the specified components.
		 */
		inline Mat3(const Real m11, const Real m12, const Real m13,
						const Real m21, const Real m22, const Real m23,
						const Real m31, const Real m32, const Real m33) {
			m_d.m[0] = m11;
			m_d.m[1] = m12;
			m_d.m[2] = m13;
			m_d.m[3] = m21;
			m_d.m[4] = m22;
			m_d.m[5] = m23;
			m_d.m[6] = m31;
			m_d.m[7] = m32;
			m_d.m[8] = m33;
		}

		/**
		 * @brief Create a new matrix from three 3D vectors.
		 * @param xaxis The x-axis vector.
		 * @param yaxis The y-axis vector.
		 * @param zaxis The z-axis vector.
		 */
		inline Mat3(const Vec3& xaxis, const Vec3& yaxis, const Vec3& zaxis) {
			m_d.m[0] = xaxis.x; m_d.m[1] = xaxis.y; m_d.m[2] = xaxis.z;
			m_d.m[3] = yaxis.x; m_d.m[4] = yaxis.y; m_d.m[5] = yaxis.z;
			m_d.m[6] = zaxis.x; m_d.m[7] = zaxis.y; m_d.m[8] = zaxis.z;
		}		

		/**
		 * @brief Operator to check for matrix equality.
		 * @param rhs The matrix to check against.
		 * @return True if the matrices are equal.
		 */
		inline Boolean operator==(const Mat3& rhs) const {
			return (
				m_d.m[0] == rhs.m_d.m[0] && m_d.m[1] == rhs.m_d.m[1] && m_d.m[2] == rhs.m_d.m[2] &&
				m_d.m[3] == rhs.m_d.m[3] && m_d.m[4] == rhs.m_d.m[4] && m_d.m[5] == rhs.m_d.m[5] &&
				m_d.m[6] == rhs.m_d.m[6] && m_d.m[7] == rhs.m_d.m[7] && m_d.m[8] == rhs.m_d.m[8]
				);
		}

		/**
		 * @brief Operator to check for matrix inequality.
		 * @param rhs The matrix to check against.
		 * @return True if the matrices are not equal.
		 */
		inline Boolean operator!=(const Mat3& rhs) const {
			return (
				m_d.m[0] != rhs.m_d.m[0] || m_d.m[1] != rhs.m_d.m[1] || m_d.m[2] != rhs.m_d.m[2] ||
				m_d.m[3] != rhs.m_d.m[3] || m_d.m[4] != rhs.m_d.m[4] || m_d.m[5] != rhs.m_d.m[5] ||
				m_d.m[6] != rhs.m_d.m[6] || m_d.m[7] != rhs.m_d.m[7] || m_d.m[8] != rhs.m_d.m[8]
				);
		}
		
		/**
		 * @brief Check for approximate matrix equality.
		 * @param rhs The matrix to check for approximate equality with.
		 * @todo: Test to see if using the sum of the differences would
		 * be faster than comparing each pair of components.
		 * @return True if the matrices are approximately equal.
		 */
		inline Boolean approx(const Mat3& rhs) const {			
			return (fabs(m_d.m[0] - rhs.m_d.m[0]) < Mat3::kEpsilon && 
					  fabs(m_d.m[1] - rhs.m_d.m[1]) < Mat3::kEpsilon && 
					  fabs(m_d.m[2] - rhs.m_d.m[2]) < Mat3::kEpsilon &&

					  fabs(m_d.m[3] - rhs.m_d.m[3]) < Mat3::kEpsilon &&
					  fabs(m_d.m[4] - rhs.m_d.m[4]) < Mat3::kEpsilon &&
					  fabs(m_d.m[5] - rhs.m_d.m[5]) < Mat3::kEpsilon &&

					  fabs(m_d.m[6] - rhs.m_d.m[6]) < Mat3::kEpsilon &&
					  fabs(m_d.m[7] - rhs.m_d.m[7]) < Mat3::kEpsilon &&
					  fabs(m_d.m[8] - rhs.m_d.m[8]) < Mat3::kEpsilon); 
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
		inline Mat3 operator+(const Mat3& rhs) const {
			return Mat3( 
				m_d.m[0] + rhs.m_d.m[0], m_d.m[1] + rhs.m_d.m[1], m_d.m[2] + rhs.m_d.m[2],
				m_d.m[3] + rhs.m_d.m[3], m_d.m[4] + rhs.m_d.m[4], m_d.m[5] + rhs.m_d.m[5],
				m_d.m[6] + rhs.m_d.m[6], m_d.m[7] + rhs.m_d.m[7], m_d.m[8] + rhs.m_d.m[8]
				);
		}
		
		/**
		 * @brief Operator for component-wise Matrix subtraction.
		 * @param rhs The matrix to be subtracted.
		 * @return A new matrix that is the result of this - rhs.
		 */
		inline Mat3 operator-(const Mat3& rhs) const {
			return Mat3( 
				m_d.m[0] - rhs.m_d.m[0], m_d.m[1] - rhs.m_d.m[1], m_d.m[2] - rhs.m_d.m[2],
				m_d.m[3] - rhs.m_d.m[3], m_d.m[4] - rhs.m_d.m[4], m_d.m[5] - rhs.m_d.m[5],
				m_d.m[6] - rhs.m_d.m[6], m_d.m[7] - rhs.m_d.m[7], m_d.m[8] - rhs.m_d.m[8]
				);
		}

		/**
		 * @brief Operator to add another Matrix to this one, component-wise.
		 * @param rhs The matrix to be added to this one.
		 * @return A reference to this matrix.
		 */
		inline Mat3 operator+=(const Mat3& rhs) {
			m_d.m[0] += rhs.m_d.m[0]; m_d.m[1] += rhs.m_d.m[1]; m_d.m[2] += rhs.m_d.m[2];
			m_d.m[3] += rhs.m_d.m[3]; m_d.m[4] += rhs.m_d.m[4]; m_d.m[5] += rhs.m_d.m[5];
			m_d.m[6] += rhs.m_d.m[6]; m_d.m[7] += rhs.m_d.m[7]; m_d.m[8] += rhs.m_d.m[8];
			return *this;
		}
		
		/**
		 * @brief Operator to subtract another Matrix from this one, component-wise.
		 * @param rhs The matrix to be subtracted from this one.
		 * @return A reference to this matrix.
		 */
		inline Mat3 operator-=(const Mat3& rhs) {
			m_d.m[0] -= rhs.m_d.m[0]; m_d.m[1] -= rhs.m_d.m[1]; m_d.m[2] -= rhs.m_d.m[2];
			m_d.m[3] -= rhs.m_d.m[3]; m_d.m[4] -= rhs.m_d.m[4]; m_d.m[5] -= rhs.m_d.m[5];
			m_d.m[6] -= rhs.m_d.m[6]; m_d.m[7] -= rhs.m_d.m[7]; m_d.m[8] -= rhs.m_d.m[8];
			return *this;
		}
		
		/**
		 * @brief Overloaded multiplication operator to multiply two matrices.
		 * @param rhs The matrix to multiply by.
		 * @return A new matrix that is the result of this * rhs.
		 */
		inline Mat3 operator*(const Mat3& rhs) const {
			Mat3 r;			
			r.m_d.m[0] = (m_d.m11*rhs.m_d.m11) + (m_d.m21 * rhs.m_d.m12) + (m_d.m31 * rhs.m_d.m13);
			r.m_d.m[1] = (m_d.m12*rhs.m_d.m11) + (m_d.m22 * rhs.m_d.m12) + (m_d.m32 * rhs.m_d.m13);			
			r.m_d.m[2] = (m_d.m13*rhs.m_d.m11) + (m_d.m23 * rhs.m_d.m12) + (m_d.m33 * rhs.m_d.m13);			
			
			r.m_d.m[3] = (m_d.m11*rhs.m_d.m21) + (m_d.m21 * rhs.m_d.m22) + (m_d.m31 * rhs.m_d.m23);
			r.m_d.m[4] = (m_d.m12*rhs.m_d.m21) + (m_d.m22 * rhs.m_d.m22) + (m_d.m32 * rhs.m_d.m23);		
			r.m_d.m[5] = (m_d.m13*rhs.m_d.m21) + (m_d.m23 * rhs.m_d.m22) + (m_d.m33 * rhs.m_d.m23);	
			
			r.m_d.m[6] = (m_d.m11*rhs.m_d.m31) + (m_d.m21 * rhs.m_d.m32) + (m_d.m31 * rhs.m_d.m33);
			r.m_d.m[7] = (m_d.m12*rhs.m_d.m31) + (m_d.m22 * rhs.m_d.m32) + (m_d.m32 * rhs.m_d.m33);
			r.m_d.m[8] = (m_d.m13*rhs.m_d.m31) + (m_d.m23 * rhs.m_d.m32) + (m_d.m33 * rhs.m_d.m33);
		
			return r;
		}

		/**
		 * @brief Overloaded multiplication operator to multiply by a scalar.
		 * @param scalar The scalar to multiply by.
		 * @return A new matrix multiplied by the scalar.
		 */
		inline Mat3 operator*(const Real scalar) const {
			return Mat3( m_d.m[0] * scalar, m_d.m[1] * scalar, m_d.m[2] * scalar,
							 m_d.m[3] * scalar, m_d.m[4] * scalar, m_d.m[5] * scalar,
							 m_d.m[6] * scalar, m_d.m[7] * scalar, m_d.m[8] * scalar );			
		}
		inline friend Mat3 operator*(const Real scalar, const Mat3& rhs) {
			return rhs * scalar;
		}
		
		/**
		 * @brief Overloaded Multiplication operator to multiply this by a scalar.
		 * @param scalar The scalar to multiply this matrix by.
		 * @return A reference to this matrix.
		 */
		inline Mat3& operator*=(const Real scalar) {			
			m_d.m[0] *= scalar; m_d.m[1] *= scalar; m_d.m[2] *= scalar;
			m_d.m[3] *= scalar; m_d.m[4] *= scalar; m_d.m[5] *= scalar;
		   m_d.m[6] *= scalar; m_d.m[7] *= scalar; m_d.m[8] *= scalar;
			return *this;
		}

		/**
		 * @brief Overloaded division operator to divide by a scalar.
		 * @param scalar The scalar to divide by.
		 * @return A new matrix divided by the scalar.
		 */
		inline Mat3 operator/(const Real scalar) const {
			return Mat3( m_d.m[0] / scalar, m_d.m[1] / scalar, m_d.m[2] / scalar,
							 m_d.m[3] / scalar, m_d.m[4] / scalar, m_d.m[5] / scalar,
							 m_d.m[6] / scalar, m_d.m[7] / scalar, m_d.m[8] / scalar);
		}
		inline friend Mat3 operator/(const Real scalar, const Mat3& rhs) {
			return Mat3(
				scalar / rhs.m_d.m[0], scalar / rhs.m_d.m[1], scalar / rhs.m_d.m[2],
				scalar / rhs.m_d.m[3], scalar / rhs.m_d.m[4], scalar / rhs.m_d.m[5],
				scalar / rhs.m_d.m[6], scalar / rhs.m_d.m[7], scalar / rhs.m_d.m[8]
				);
		}
		
		/**
		 * @brief Overloaded Division operator to divide this by a scalar.
		 * @param scalar The scalar to divide this matrix by.
		 * @return A reference to this matrix.
		 */
		inline Mat3& operator/=(const Real scalar) {			
			m_d.m[0] /= scalar; m_d.m[1] /= scalar; m_d.m[2] /= scalar;
			m_d.m[3] /= scalar; m_d.m[4] /= scalar; m_d.m[5] /= scalar;
		   m_d.m[6] /= scalar; m_d.m[7] /= scalar; m_d.m[8] /= scalar;
			return *this;
		}
		
		/**
		 * @brief Overloaded Multiplication operator to multiply by another matrix.
		 * @param rhs The matrix to multiply this one by.
		 * @return A reference to this matrix.
		 */
		Mat3& operator*=(const Mat3& rhs) {
			Mat3::MatrixData m;

			m.m[0] = (m_d.m11*rhs.m_d.m11) + (m_d.m21 * rhs.m_d.m12) + (m_d.m31 * rhs.m_d.m13);
			m.m[1] = (m_d.m12*rhs.m_d.m11) + (m_d.m22 * rhs.m_d.m12) + (m_d.m32 * rhs.m_d.m13);
			m.m[2] = (m_d.m13*rhs.m_d.m11) + (m_d.m23 * rhs.m_d.m12) + (m_d.m33 * rhs.m_d.m13);

			m.m[3] = (m_d.m11*rhs.m_d.m21) + (m_d.m21 * rhs.m_d.m22) + (m_d.m31 * rhs.m_d.m23);
			m.m[4] = (m_d.m12*rhs.m_d.m21) + (m_d.m22 * rhs.m_d.m22) + (m_d.m32 * rhs.m_d.m23);
			m.m[5] = (m_d.m13*rhs.m_d.m21) + (m_d.m23 * rhs.m_d.m22) + (m_d.m33 * rhs.m_d.m23);

			m.m[6] = (m_d.m11*rhs.m_d.m31) + (m_d.m21 * rhs.m_d.m32) + (m_d.m31 * rhs.m_d.m33);
			m.m[7] = (m_d.m12*rhs.m_d.m31) + (m_d.m22 * rhs.m_d.m32) + (m_d.m32 * rhs.m_d.m33);
			m.m[8] = (m_d.m13*rhs.m_d.m31) + (m_d.m23 * rhs.m_d.m32) + (m_d.m33 * rhs.m_d.m33);
		
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
				(m_d.m11*rhs.x) + (m_d.m21 * rhs.y) + (m_d.m31 * rhs.z),
				(m_d.m12*rhs.x) + (m_d.m22 * rhs.y) + (m_d.m32 * rhs.z),
				(m_d.m13*rhs.x) + (m_d.m23 * rhs.y) + (m_d.m33 * rhs.z)
				);			
		}
		
		/**
		 * @brief Get the transpose of the matrix.
		 * @return A matrix that is the transpose of this matrix.
		 */
		inline Mat3 transposed() const {
			return Mat3(
				m_d.m11, m_d.m21, m_d.m31,
				m_d.m12, m_d.m22, m_d.m32,
				m_d.m13, m_d.m23, m_d.m33
				);
		}

		/**
		 * @brief Transpose this matrix.
		 * @return A reference to this matrix after it's been transposed.
		 */
		inline Mat3& transpose() {
			Real tmp = m_d.m12;
			m_d.m12 = m_d.m21; m_d.m21 = tmp;
			tmp = m_d.m13; m_d.m13 = m_d.m31; m_d.m31 = tmp;
			tmp = m_d.m23; m_d.m23 = m_d.m32; m_d.m32 = tmp;

			return *this;
		}

		/**
		 * @brief Get the determinent of the matrix.
		 * @return The determinant of the matrix.
		 */
		inline Real determinant() const {
			return 
				m_d.m11*(m_d.m22*m_d.m33 - m_d.m32*m_d.m23) -
				m_d.m21*(m_d.m12*m_d.m33 - m_d.m32*m_d.m13) +
				m_d.m31*(m_d.m12*m_d.m23 - m_d.m22*m_d.m13);
		}		
		
		/**
		 * @brief Get the inverse of the matrix.
		 * @return A matrix that is the inverse of this one.
		 */
		Mat3 inverse() const;
		
		/**
		 * @brief Invert this matrix.
		 * @return A reference to this matrix, inverted.
		 */
		inline Mat3& invert() {
			Mat3 i = inverse();
			m_d = i.m_d;
			return *this;			
		}

		/**
		 * @brief Get the x-axis vector from the matrix.
		 * @return A vector corresponding to the local x-axis.
		 */
		inline Vec3 xAxis() const {
			return Vec3(m_d.m[0], m_d.m[1], m_d.m[2]);
		}

		/**
		 * @brief Get the y-axis vector from the matrix.
		 * @return A vector corresponding to the local y-axis.
		 */
		inline Vec3 yAxis() const {
			return Vec3(m_d.m[3], m_d.m[4], m_d.m[5]);
		}

		/**
		 * @brief Get the z-axis vector from the matrix.
		 * @return A vector corresponding to the local z-axis.
		 */
		inline Vec3 zAxis() const {
			return Vec3(m_d.m[6], m_d.m[7], m_d.m[8]);
		}		

		friend class Mat4;		
 
	  private:
		MatrixData m_d;
		
		
	};
#ifdef DEBUG
	std::ostream& operator<<(std::ostream& out, const Mat3& m);
#endif //DEBUG

} // namespace Cat



#endif //CAT_CORE_MATH_MAT3_H
