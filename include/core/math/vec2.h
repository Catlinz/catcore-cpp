#ifndef CAT_CORE_MATH_VEC2_H
#define CAT_CORE_MATH_VEC2_H
/**
 * @copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file vec2.h
 * 
 * Contains the definitions of the floating point vec2 class and math functions to 
 * operate on 2D Vectors.
 *
 * @author Catlin Zilinski
 * @date Dec 3, 2014
 */

#include "core/corelib.h"

namespace Cat {

	/**
	 * @class Vec2 vec2.h "core/math/vec2.h"
	 * @brief 2D vector template class.
	 *
	 * Floating point Vec2 class and math functions to operate on 2D Vectors.
	 *
	 * @author Catlin Zilinski
	 * @version 3
	 * @since Apr 30, 2014
	 */
	class Vec2 {
	  public:
		Real x, y;		
		
		/**
		 * @brief Default constructor sets to zero vector.
		 */
		inline Vec2()
			: x(0), y(0) {}

		/**
		 * @brief Sets all the components of the vector from a single scalar.
		 * @param val The scalar value to set the x and y components of the vector to.
		 */
		inline explicit Vec2(Real val)
			: x(val), y(val) {}
		
		/**
		 * @brief Sets the value of the vector to the specified values.
		 * @param vx The x component of the vector.
		 * @param vy The y component of the vector.
		 */
		inline Vec2(Real vx, Real vy)
			: x(vx), y(vy) {}
		
		/**
		 * @brief Sets the vector components from an array of Reals.
		 * @param vec The array of Reals to set the components to.
		 */
		inline explicit Vec2(Real *const vec)
			: x(vec[0]), y(vec[1]) {}

		/**
		 * @brief Copy constructor.
		 * @param p_src The Vec2 to copy from.
		 */
		inline Vec2(const Vec2& p_src) {
			x = p_src.x; y = p_src.y;
		}

		/**
		 * @brief Overloaded assignment operator for vector assignment.
		 * @param src The source vector to copy from.
		 * @return A reference to this vector.
		 */
		inline Vec2& operator=(const Vec2& src) { 
			x = src.x; y = src.y;
			return *this;			
		}

		/**
		 * @brief Overloaded assignment operator for scalar assignment.
		 * @param scalar The scalar to set all the vector components to.
		 * @return A reference to this vector.
		 */
		inline Vec2& operator=(Real scalar) {
			x = scalar; y = scalar;
			return *this;
		}

		/**
		 * @brief Overloaded equality operator for vector equality.
		 * @param rhs The vector we are comparing to.
		 * @return True if the two vectors are equal.
		 */
		inline Boolean operator==(const Vec2& rhs) const {
			return (x == rhs.x && y == rhs.y);
		}

		/**
		 * @brief Overloaded non-equality operator for vectors.
		 * @param rhs The vector we are comparing to.
		 * @return True if the two vectors are not equal.
		 */
		inline Boolean operator!=(const Vec2& rhs) const {
			return (x != rhs.x || y != rhs.y);
		}

		/**
		 * @brief The overloaded negation operator.
		 * @return A negated copy of this vector.
		 */
		inline Vec2 operator-() const {
			return Vec2(-x, -y);
		}		
		
		/**
		 * @brief The overloaded addition operator to add two vectors.
		 * @param rhs The rhs vector.
		 * @return A new vector which is the sum of this + rhs.		 
		 */
		inline Vec2 operator+(const Vec2& rhs) const {
			return Vec2(x + rhs.x, y + rhs.y);
		}

		/**
		 * @brief The overloaded addition operator to add a scalar to the vector.
		 * @param scalar The scalar value to add to all the vector's components.
		 * @return A new vector with the scalar added.
		 */
		inline Vec2 operator+(const Real scalar) const {
			return Vec2(x + scalar, y + scalar);
		}
		
		
		/**
		 * @brief The overloaded operator to add another vector to this one.
		 * @param rhs The rhs vector to add to this one.
		 * @return A reference to this vector.
		 */
		inline Vec2& operator+=(const Vec2& rhs) { 
			x += rhs.x;	y += rhs.y;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to add a scalar to this vector.
		 * @param scalar The scalar value to add to this vector's components.
		 * @return A reference to this vector.
		 */
		inline Vec2& operator+=(const Real scalar) {
			x += scalar; y += scalar;
			return *this;			
		}
		
		/**
		 * @brief The overloaded subtraction operator to subtract two vectors.
		 * @param rhs The rhs vector.
		 * @return A new vector which is the difference of this - rhs.		 
		 */
		inline Vec2 operator-(const Vec2& rhs) const {
			return Vec2(x - rhs.x, y - rhs.y);
		}

		/**
		 * @brief The overloaded subtraction operator to subtract a scalar to the vector.
		 * @param scalar The scalar value to subtract from all the vector's components.
		 * @return A new vector with the scalar subtracted.
		 */
		inline Vec2 operator-(const Real scalar) const {
			return Vec2(x - scalar, y - scalar);
		}
		
		/**
		 * @brief The overloaded operator to subtract another vector from this one.
		 * @param rhs The rhs vector to subtract from this one.
		 * @return A reference to this vector.
		 */
		inline Vec2& operator-=(const Vec2& rhs) { 
			x -= rhs.x; y -= rhs.y;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to subtract a scalar from this vector.
		 * @param scalar The scalar value to subtract from this vector's components.
		 * @return A reference to this vector.
		 */
		inline Vec2& operator-=(const Real scalar) {
			x -= scalar; y -= scalar;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to get a multiple of this vector.
		 * @param scalar The scalar to multiply the vector by.
		 * @return A copy of the vector multiplied by the scalar.
		 */
		inline Vec2 operator*(const Real scalar) const {
			return Vec2(x*scalar, y*scalar);
		}

		/**
		 * @brief The overloaded operator to perform component-wise multiplication.
		 * @param p_rhs The vector to multiply this vector by, component-wise.
		 * @return The result of this*p_rhs, component-wise.
		 */
		inline Vec2 operator*(const Vec2& p_rhs) const {
			return Vec2(x*p_rhs.x, y*p_rhs.y);
		}
		
		/**
		 * @brief The overloaded operator to multiply this vector by a scalar.
		 * @param scalar The scalar to multiply the vector by.
		 * @return A reference to this vector.
		 */
		inline Vec2& operator*=(const Real scalar) {
			x *= scalar; y *= scalar;
			return *this;			
		}

		/**
		 * @brief The overloaded operator to perform component-wise multiplication.
		 * @param p_rhs The vector to multiply this vector by, component-wise.
		 * @return A reference to this vector, multiplied by p_rhs.
		 */
		inline Vec2& operator*=(const Vec2& p_rhs) {
			x *= p_rhs.x; y *= p_rhs.y;
			return *this;
		}
		
		/**
		 * @brief The overloaded operator to get a scaled vector.
		 * @param scalar The scalar to divide the vector by.
		 * @return A copy of this vector scaled by the scalar value.
		 */
		inline Vec2 operator/(const Real scalar) const {
			return Vec2(x/scalar, y/scalar);
		}
		/**
		 * @brief The overloaded operator to perform component-wise division.
		 * @param p_rhs The vector to divide this vector by, component-wise.
		 * @return The result of this/p_rhs, component-wise.
		 */
		inline Vec2 operator/(const Vec2& p_rhs) const {
			return Vec2(x/p_rhs.x, y/p_rhs.y);
		}
		
		/**
		 * @brief The overloaded overator to scale this vector.
		 * @param scalar The scalar to divide the vector by.
		 * @return A reference to this vector.
		 */
		inline Vec2& operator/=(const Real scalar) {
			x /= scalar; y /= scalar;
			return *this;
		}
		
		/**
		 * @brief The overloaded operator to perform component-wise division.
		 * @param p_rhs The vector to divide this vector by, component-wise.
		 * @return A reference to this vector, divided by p_rhs.
		 */
		inline Vec2& operator/=(const Vec2& p_rhs) {
			x /= p_rhs.x; y /= p_rhs.y;
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

		/**
		 * @brief Methods to set the components of the vector.
		 * @param p_x The value to set the x component to.
		 * @param p_y The value to set the y component to.
		 */
		inline void xy(Real p_x, Real p_y) { x = p_x; y = p_y; }



#ifdef DEBUG
		friend std::ostream& operator<<(std::ostream& out, const Vec2& v) {
			return out << v.x << "i + " << v.y << "j";
		}	
#endif // DEBUG
		
		static const Vec2 kXAxis;
		static const Vec2 kYAxis;
		static const Vec2 kZero;		
	};

	template <typename T>
	inline Vec2 operator/(const Real scalar, const Vec2& v) {
		return Vec2(scalar/v.x, scalar/v.y);
	}

	template <typename T>
	inline Vec2 operator*(const Real scalar, const Vec2& v) {
		return Vec2(scalar*v.x, scalar*v.y);
	}

	template <typename T>
	inline Vec2 operator-(const Real scalar, const Vec2& v) {
		return Vec2(scalar - v.x, scalar - v.y);
	}

	template <typename T>
	inline Vec2 operator+(const Real scalar, const Vec2& v) { 
		return Vec2(scalar + v.x, scalar + v.y); 
	}
} // namespace Cat


#endif // CAT_CORE_MATH_VEC2_H

