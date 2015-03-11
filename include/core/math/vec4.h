#ifndef CAT_CORE_MATH_VEC4_H
#define CAT_CORE_MATH_VEC4_H
/**
 * @copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file vec4.h
 * 
 * Contains the definitions of the Vec4 type and math functions to 
 * operate on 4D Vectors.
 *
 * @author Catlin Zilinski
 * @date Dec 3, 2014
 */

#include "core/corelib.h"

namespace Cat {

	/**
	 * @class Vec4 vec4.h "core/math/vec4.h"
	 * @brief A class representing a 4D Vector with Real components.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Dec 4, 2013
	 */
	class Vec4 {
	  public:
		Real x, y, z, w;
		
		/**
		 * @brief Default constructor sets to zero vector.
		 */
		inline Vec4()
			: x(REAL(0.0)), y(REAL(0.0)), z(REAL(0.0)), w(REAL(0.0)) {}

		/**
		 * @brief Sets all the components of the vector from a single scalar.
		 * @param val The scalar value to set the x, y and z components of the vector to.
		 */
		inline explicit Vec4(Real val)
			: x(val), y(val), z(val), w(val) {}
		
		/**
		 * @brief Sets the value of the vector to the specified values.
		 * @param vx The x component of the vector.
		 * @param vy The y component of the vector.
		 * @param vz The z component of the vector.
		 * @param vw The w component of the vector.
		 */
		inline Vec4(Real vx, Real vy, Real vz, Real vw)
			: x(vx), y(vy), z(vz), w(vw) {}

		/**
		 * @brief Sets the vector components from an array of Reals.
		 * @param vec The array of Reals to set the components to.
		 */
		inline explicit Vec4(Real *const vec)
			: x(vec[0]), y(vec[1]), z(vec[2]), w(vec[3]) {}

		/**
		 * @brief Overloaded assignment operator for vector assignment.
		 * @param src The source vector to copy from.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator=(const Vec4& src) { 
			x = src.x; y = src.y; z = src.z; w = src.w;		  
			return *this;			
		}

		/**
		 * @brief Overloaded assignment operator for scalar assignment.
		 * @param scalar The scalar to set all the vector components to.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator=(const Real scalar) {
			x = scalar; y = scalar; z = scalar; w = scalar;			
			return *this;
		}

		/**
		 * @brief Overloaded equality operator for vector equality.
		 * @param rhs The vector we are comparing to.
		 * @return True if the two vectors are equal.
		 */
		inline Boolean operator==(const Vec4& rhs) const {
			return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
		}

		/**
		 * @brief Overloaded non-equality operator for vectors.
		 * @param rhs The vector we are comparing to.
		 * @return True if the two vectors are not equal.
		 */
		inline Boolean operator!=(const Vec4& rhs) const {
			return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w);
		}

		/**
		 * @brief The overloaded negation operator.
		 * @return A negated copy of this vector.
		 */
		inline Vec4 operator-() const {
			return Vec4(-x, -y, -z, -w);
		}		
		
		/**
		 * @brief The overloaded addition operator to add two vectors.
		 * @param rhs The rhs vector.
		 * @return A new vector which is the sum of this + rhs.		 
		 */
		inline Vec4 operator+(const Vec4& rhs) const {
			return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}

		/**
		 * @brief The overloaded addition operator to add a scalar to the vector.
		 * @param scalar The scalar value to add to all the vector's components.
		 * @return A new vector with the scalar added.
		 */
		inline Vec4 operator+(const Real scalar) const {
			return Vec4(x + scalar, y + scalar, z + scalar, w + scalar);
		}
		inline friend Vec4 operator+(const Real scalar, const Vec4& v) { 
			return Vec4(scalar + v.x, scalar + v.y, scalar + v.z, scalar + v.w); 
		}
		
		/**
		 * @brief The overloaded operator to add another vector to this one.
		 * @param rhs The rhs vector to add to this one.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator+=(const Vec4& rhs) { 
			x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;			
			return *this;			
		}

		/**
		 * @brief The overloaded operator to add a scalar to this vector.
		 * @param scalar The scalar value to add to this vector's components.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator+=(const Real scalar) {
			x += scalar; y += scalar; z += scalar; w += scalar;
			return *this;			
		}
		
		/**
		 * @brief The overloaded subtraction operator to subtract two vectors.
		 * @param rhs The rhs vector.
		 * @return A new vector which is the difference of this - rhs.		 
		 */
		inline Vec4 operator-(const Vec4& rhs) const {
			return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}

		/**
		 * @brief The overloaded subtraction operator to subtract a scalar to the vector.
		 * @param scalar The scalar value to subtract from all the vector's components.
		 * @return A new vector with the scalar subtracted.
		 */
		inline Vec4 operator-(const Real scalar) const {
			return Vec4(x - scalar, y - scalar, z - scalar, w - scalar);
		}
		inline friend Vec4 operator-(const Real scalar, const Vec4& v) {
			return Vec4(scalar - v.x, scalar - v.y, scalar - v.z, scalar - v.w);
		}
		
		/**
		 * @brief The overloaded operator to subtract another vector from this one.
		 * @param rhs The rhs vector to subtract from this one.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator-=(const Vec4& rhs) { 
			x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to subtract a scalar from this vector.
		 * @param scalar The scalar value to subtract from this vector's components.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator-=(const Real scalar) {
			x -= scalar; y -= scalar; z -= scalar; w -= scalar;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to get a multiple of this vector.
		 * @param scalar The scalar to multiply the vector by.
		 * @return A copy of the vector multiplied by the scalar.
		 */
		inline Vec4 operator*(const Real scalar) const {
			return Vec4(x*scalar, y*scalar, z*scalar, w*scalar);
		}
		inline friend Vec4 operator*(const Real scalar, const Vec4& v) {
			return Vec4(scalar*v.x, scalar*v.y, scalar*v.z, scalar*v.w);
		}

		/**
		 * @brief The overloaded operator to do component-wise vector multiplication.
		 * @param p_rhs The vector to multiply the vector by.
		 * @return A copy of the vector multiplied by the other vector.
		 */
		inline Vec4 operator*(const Vec4& p_rhs) const {
			return Vec4(x*p_rhs.x, y*p_rhs.y, z*p_rhs.z, w*p_rhs.w);
		}
		
		/**
		 * @brief The overloaded operator to multiply this vector by a scalar.
		 * @param scalar The scalar to multiply the vector by.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator*=(const Real scalar) {
			x *= scalar; y *= scalar; z *= scalar; w *= scalar;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to do component-wise vector multiplication.
		 * @param p_rhs The vector to multiply the vector by.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator*=(const Vec4& p_rhs) {
			x *= p_rhs.x; y *= p_rhs.y; z *= p_rhs.z; w *= p_rhs.w;
			return *this;			
		}
		
		/**
		 * @brief The overloaded operator to get a scaled vector.
		 * @param scalar The scalar to divide the vector by.
		 * @return A copy of this vector scaled by the scalar value.
		 */
		inline Vec4 operator/(const Real scalar) const {
			return Vec4(x/scalar, y/scalar, z/scalar, w/scalar);
		}
		inline friend Vec4 operator/(const Real scalar, const Vec4& v) {
			return Vec4(scalar/v.x, scalar/v.y, scalar/v.z, scalar/v.w);
		}

		/**
		 * @brief The overloaded operator to do component-wise vector division.
		 * @param p_rhs The vector to divide this vector by.
		 * @return A copy of this vector multiplied by the other vector.
		 */
		inline Vec4 operator/(const Vec4& p_rhs) const {
			return Vec4(x/p_rhs.x, y/p_rhs.y, z/p_rhs.z, w/p_rhs.w);
		}
		
		/**
		 * @brief The overloaded overator to scale this vector.
		 * @param scalar The scalar to divide the vector by.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator/=(const Real scalar) {
			x /= scalar; y /= scalar; z /= scalar; w /= scalar;
			return *this;
		}

		/**
		 * @brief The overloaded overator to do component-wise vector division.
		 * @param p_rhs The vector to divide this vector by.
		 * @return A reference to this vector.
		 */
		inline Vec4& operator/=(const Vec4& p_rhs) {
			x /= p_rhs.x; y /= p_rhs.y; z /= p_rhs.z; w /= p_rhs.w;
			return *this;
		}

		// ###########
		// ########### Methods for setting various components of the vector.
		// ###########

		/**
		 * @brief Methods to set the components of the vector.
		 * @param p_scalar The value to set the components(s) to.
		 */
		//inline void x(Real p_scalar) { x = p_scalar; }
		//inline void y(Real p_scalar) { y = p_scalar; }
		//inline void z(Real p_scalar) { z = p_scalar; }
		//inline void w(Real p_scalar) { w = p_scalar; }

		/**
		 * @brief Methods to set only two components of the vector.
		 * @param p_c0 The value to set the first component to.
		 * @param p_c1 The value to set the second component to.
		 */
		inline void xy(Real p_c0, Real p_c1) { x = p_c0; y = p_c1; }
		inline void xz(Real p_c0, Real p_c1) { x = p_c0; z = p_c1; }
		inline void xw(Real p_c0, Real p_c1) { x = p_c0; w = p_c1; }
		inline void yz(Real p_c0, Real p_c1) { y = p_c0; z = p_c1; }
		inline void yw(Real p_c0, Real p_c1) { y = p_c0; w = p_c1; }
		inline void zw(Real p_c0, Real p_c1) { z = p_c0; w = p_c1; }

		/**
		 * @brief Methods to set only three components of the vector.
		 * @param p_c0 The value to set the first component to.
		 * @param p_c1 The value to set the second component to.
		 * @param p_c2 The value to set the third component to.
		 */
		inline void xyz(Real p_c0, Real p_c1, Real p_c2) { x = p_c0; y = p_c1; z = p_c2; }
		inline void xyw(Real p_c0, Real p_c1, Real p_c2) { x = p_c0; y = p_c1; w = p_c2; }
		inline void xzw(Real p_c0, Real p_c1, Real p_c2) { x = p_c0; z = p_c1; w = p_c2; }
		inline void yzw(Real p_c0, Real p_c1, Real p_c2) { y = p_c0; z = p_c1; w = p_c2; }

		/**
		 * @brief Method to set all the components of the vector.
		 * @param p_x The value to set the x-component to.
		 * @param p_y The value to set the y-component to.
		 * @param p_z The value to set the z-component to.
		 * @param p_w The value to set the w-component to.
		 */
		inline void xyzw(Real p_x, Real p_y, Real p_z, Real p_w) {
			x = p_x; y = p_y; z = p_z; w = p_w;
		}

#ifdef DEBUG
		friend std::ostream& operator<<(std::ostream& out, const Vec4& v) {
			return out << v.w << " + " << v.x << "i + " << v.y << "j + " << v.z << "k";
		}	
#endif // DEBUG
		
	};


} // namespace Cat


#endif // CAT_CORE_MATH_VEC4_H

