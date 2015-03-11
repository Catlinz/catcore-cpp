#ifndef CAT_CORE_MATH_VEC3_H
#define CAT_CORE_MATH_VEC3_H
/**
 * @copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file vec3.h
 * 
 * Contains the definitions of the Vec3 type and math functions to 
 * operate on 3D Vectors.
 *
 * @author Catlin Zilinski
 * @date Dec 3, 2013
 */

#include "core/corelib.h"

namespace Cat {

	/**
	 * @class Vec3 vec3.h "core/math/vec3.h"
	 * @brief A class representing a 3D Vector with floating point components.
	 *
	 * @author Catlin Zilinski
	 * @version 3
	 * @since May 30, 2013
	 */
	class Vec3 {
	  public:
		Real x, y, z;
		
		/**
		 * @brief Default constructor sets to zero vector.
		 */
		inline Vec3()
			: x(REAL(0.0)), y(REAL(0.0)), z(REAL(0.0)) {}

		/**
		 * @brief Sets all the components of the vector from a single scalar.
		 * @param val The scalar value to set the x, y and z components of the vector to.
		 */
		inline explicit Vec3(Real val)
			: x(val), y(val), z(val) {}
		
		/**
		 * @brief Sets the value of the vector to the specified values.
		 * @param vx The x component of the vector.
		 * @param vy The y component of the vector.
		 * @param vz The z component of the vector.
		 */
		inline Vec3(Real vx, Real vy, Real vz)
			: x(vx), y(vy), z(vz) {}

		/**
		 * @brief Sets the vector components from an array of Reals.
		 * @param vec The array of Reals to set the components to.
		 */
		inline explicit Vec3(Real *const vec)
			: x(vec[0]), y(vec[1]), z(vec[2]) {}

		/**
		 * @brief Copy constructor for the Vec3.
		 * @param p_src The source vector to copy from.
		 */
		inline Vec3(const Vec3& p_src)
			: x(p_src.x), y(p_src.y), z(p_src.z) {}

		/**
		 * @brief Overloaded assignment operator for vector assignment.
		 * @param src The source vector to copy from.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator=(const Vec3& src) { 
			x = src.x; y = src.y; z = src.z;
			return *this;			
		}

		/**
		 * @brief Overloaded assignment operator for scalar assignment.
		 * @param scalar The scalar to set all the vector components to.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator=(const Real scalar) {
			x = scalar; y = scalar;	z = scalar;
			return *this;
		}

		/**
		 * @brief Overloaded equality operator for vector equality.
		 * @param rhs The vector we are comparing to.
		 * @return True if the two vectors are equal.
		 */
		inline Boolean operator==(const Vec3& rhs) const {
			return (x == rhs.x && y == rhs.y && z == rhs.z);
		}

		/**
		 * @brief Overloaded non-equality operator for vectors.
		 * @param rhs The vector we are comparing to.
		 * @return True if the two vectors are not equal.
		 */
		inline Boolean operator!=(const Vec3& rhs) const {
			return (x != rhs.x || y != rhs.y || z != rhs.z);
		}
		
		/**
		 * @brief The overloaded negation operator.
		 * @return A negated copy of this vector.
		 */
		inline Vec3 operator-() const {
			return Vec3(-x, -y, -z);
		}		
		
		/**
		 * @brief The overloaded addition operator to add two vectors.
		 * @param rhs The rhs vector.
		 * @return A new vector which is the sum of this + rhs.		 
		 */
		inline Vec3 operator+(const Vec3& rhs) const {
			return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
		}

		/**
		 * @brief The overloaded addition operator to add a scalar to the vector.
		 * @param scalar The scalar value to add to all the vector's components.
		 * @return A new vector with the scalar added.
		 */
		inline Vec3 operator+(const Real scalar) const {
			return Vec3(x + scalar, y + scalar, z + scalar);
		}
		inline friend Vec3 operator+(const Real scalar, const Vec3& v) { 
			return Vec3(scalar + v.x, scalar + v.y, scalar + v.z); 
		}
		
		/**
		 * @brief The overloaded operator to add another vector to this one.
		 * @param rhs The rhs vector to add to this one.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator+=(const Vec3& rhs) { 
			x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to add a scalar to this vector.
		 * @param scalar The scalar value to add to this vector's components.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator+=(const Real scalar) {
			x += scalar; y += scalar; z += scalar;
			return *this;			
		}
		
		/**
		 * @brief The overloaded subtraction operator to subtract two vectors.
		 * @param rhs The rhs vector.
		 * @return A new vector which is the difference of this - rhs.		 
		 */
		inline Vec3 operator-(const Vec3& rhs) const {
			return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
		}

		/**
		 * @brief The overloaded subtraction operator to subtract a scalar to the vector.
		 * @param scalar The scalar value to subtract from all the vector's components.
		 * @return A new vector with the scalar subtracted.
		 */
		inline Vec3 operator-(const Real scalar) const {
			return Vec3(x - scalar, y - scalar, z - scalar);
		}
		inline friend Vec3 operator-(const Real scalar, const Vec3& v) {
			return Vec3(scalar - v.x, scalar - v.y, scalar - v.z);
		}
		
		/**
		 * @brief The overloaded operator to subtract another vector from this one.
		 * @param rhs The rhs vector to subtract from this one.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator-=(const Vec3& rhs) { 
			x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to subtract a scalar from this vector.
		 * @param scalar The scalar value to subtract from this vector's components.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator-=(const Real scalar) {
			x -= scalar; y -= scalar; z -= scalar;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to get a multiple of this vector.
		 * @param scalar The scalar to multiply the vector by.
		 * @return A copy of the vector multiplied by the scalar.
		 */
		inline Vec3 operator*(Real scalar) const {
			return Vec3(x*scalar, y*scalar, z*scalar);
		}
		inline friend Vec3 operator*(const Real scalar, const Vec3& v) {
			return Vec3(scalar*v.x, scalar*v.y, scalar*v.z);
		}

		/**
		 * @brief The overloaded operator to do component-wise vector multiplication.
		 * @param p_rhs The vector to multiply the vector by.
		 * @return A copy of the vector multiplied by the other vector.
		 */
		inline Vec3 operator*(const Vec3& p_rhs) const {
			return Vec3(x*p_rhs.x, y*p_rhs.y, z*p_rhs.z);
		}
		
		/**
		 * @brief The overloaded operator to multiply this vector by a scalar.
		 * @param scalar The scalar to multiply the vector by.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator*=(const Real scalar) {
			x *= scalar; y *= scalar; z *= scalar;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to do component-wise vector multiplication.
		 * @param p_rhs The scalar to multiply the vector by.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator*=(const Vec3& p_rhs) {
			x *= p_rhs.x; y *= p_rhs.y; z *= p_rhs.z;
			return *this;			
		}
		
		/**
		 * @brief The overloaded operator to get a scaled vector.
		 * @param scalar The scalar to divide the vector by.
		 * @return A copy of this vector scaled by the scalar value.
		 */
		inline Vec3 operator/(const Real scalar) const {
			return Vec3(x/scalar, y/scalar, z/scalar);
		}
		inline friend Vec3 operator/(const Real scalar, const Vec3& v) {
			return Vec3(scalar/v.x, scalar/v.y, scalar/v.z);
		}

		/**
		 * @brief The overloaded operator to do component-wise vector division.
		 * @param p_rhs The vector to divide this vector by.
		 * @return A copy of this vector divided by the other vector value.
		 */
		inline Vec3 operator/(const Vec3& p_rhs) const {
			return Vec3(x/p_rhs.x, y/p_rhs.y, z/p_rhs.z);
		}
		
		/**
		 * @brief The overloaded overator to scale this vector.
		 * @param scalar The scalar to divide the vector by.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator/=(const Real scalar) {
			x /= scalar; y /= scalar; z /= scalar;
			return *this;
		}

		/**
		 * @brief The overloaded overator to do component-wise vector division.
		 * @param p_rhs The vector to divide the vector by.
		 * @return A reference to this vector.
		 */
		inline Vec3& operator/=(const Vec3& p_rhs) {
			x /= p_rhs.x; y /= p_rhs.y; z /= p_rhs.z;
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

		/**
		 * @brief Methods to set only two components of the vector.
		 * @param p_c0 The value to set the first component to.
		 * @param p_c1 The value to set the second component to.
		 */
		inline void xy(Real p_c0, Real p_c1) { x = p_c0; y = p_c1; }
		inline void xz(Real p_c0, Real p_c1) { x = p_c0; z = p_c1; }
		inline void yz(Real p_c0, Real p_c1) { y = p_c0; z = p_c1; }

		/**
		 * @brief Method to set all the components of the vector.
		 * @param p_x The value to set the x-component to.
		 * @param p_y The value to set the y-component to.
		 * @param p_z The value to set the z-component to.
		 */
		inline void xyz(Real p_x, Real p_y, Real p_z) {
			x = p_x; y = p_y; z = p_z;
		}

#ifdef DEBUG
		friend std::ostream& operator<<(std::ostream& out, const Vec3& v) {
			return out << v.x << "i + " << v.y << "j + " << v.z << "k";
		}	
#endif // DEBUG
		
		static const Vec3 kXAxis;
		static const Vec3 kYAxis;
		static const Vec3 kZAzis;
		static const Vec3 kZero;		
		
		
	};


} // namespace Cat


#endif // CAT_CORE_MATH_VEC3_H

