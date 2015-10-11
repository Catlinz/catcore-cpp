#ifndef CX_CORE_MATH_CXVEC2_H
#define CX_CORE_MATH_CXVEC2_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxVec2.h
 * @brief Contains the definition of the 2d vector class and math functions.
 *
 * @author Catlin Zilinski
 * @date Oct 3, 2015
 */

#include "core/Cx.h"
#include "core/math/CxMath.h"

#  define CX_VEC2_UNIT_EPS 1e-8f
#  define CX_VEC2_NORM_EPS 1e-10f

namespace cat {

	class CxVec2 {
	  public:
		CxReal x, y;

		/** @brief Vector is initialise to garbage. */
		CX_FORCE_INLINE CxVec2() {}
	
		/** @brief Initialise the vector to all zeros. */
		CX_FORCE_INLINE CxVec2(CxZero in_z) : x(0.0f), y(0.0f) {
			CX_UNUSED(in_z);
		}

		/**
		 * @brief Initialise all elements to the same value.
		 * @param in_val The value to initialise the elements to.
		 */
		explicit CX_FORCE_INLINE CxVec2(CxReal in_val)
			: x(in_val), y(in_val) {}

		/**
		 * @brief Initialise the elements from an array.
		 * @param in_arr The array of reals.
		 */
		explicit CX_FORCE_INLINE CxVec2(const CxReal *in_arr)
			: x(in_arr[0]), y(in_arr[1]) {}

		/**
		 * @brief Initialise the elements of the vector.
		 * @param in_x The x-component of the vector.
		 * @param in_y The y-component of the vector.
		 */
		CX_FORCE_INLINE CxVec2(CxReal in_x, CxReal in_y)
			: x(in_x), y(in_y) {}

		/** @brief Copy constructor, simply copies the elements. */
		CX_FORCE_INLINE CxVec2(const CxVec2& in_src)
			: x(in_src.x), y(in_src.y) {}

		/** @brief Overloaded assignment operator */
		CX_FORCE_INLINE CxVec2& operator=(const CxVec2& in_src) {
			x = in_src.x; y = in_src.y;  return *this;
		}

		/** @brief Array style element access. */
		CX_FORCE_INLINE CxReal& operator[](CxI32 in_idx) {
			CXD_IF_ERR((in_idx < 0 || in_idx > 1), "CxVec2 index out of bounds!");
			return reinterpret_cast<CxReal *>(this)[in_idx];
		}

		/** @brief Const array style element access. */
		CX_FORCE_INLINE const CxReal& operator[](CxI32 in_idx) const {
			CXD_IF_ERR((in_idx < 0 || in_idx > 1), "CxVec2 index out of bounds!");
			return reinterpret_cast<const CxReal *>(this)[in_idx];
		}

		/** @return True if the two vectors are strictly equal. */
		CX_FORCE_INLINE CxBool operator==(const CxVec2 &in_v) const {
			return (x == in_v.x) && (y == in_v.y);
		}

		/** @return True if the two vectors are not strictly equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxVec2 &in_v) const {
			return (x != in_v.x) || (y != in_v.y);
		}

		/** @return The negation of the vector. */
		CX_FORCE_INLINE CxVec2 operator-() const {
			return CxVec2(-x, -y);
		}

		/** @return The result of the vector addition this + in_v. */
		CX_FORCE_INLINE CxVec2 operator+(const CxVec2 &in_v) const {
			return CxVec2(x + in_v.x, y + in_v.y);
		}

		/** @return The result of adding a scalar to each element of the vector. */
		CX_FORCE_INLINE CxVec2 operator+(CxReal in_s) const {
			return CxVec2(x + in_s, y + in_s);
		}

		/** @return The result of the vector subtraction this - in_v. */
		CX_FORCE_INLINE CxVec2 operator-(const CxVec2 &in_v) const {
			return CxVec2(x - in_v.x, y - in_v.y);
		}

		/** @return The result of subtracting a scalar from each element of the vector. */
		CX_FORCE_INLINE CxVec2 operator-(CxReal in_s) const {
			return CxVec2(x - in_s, y - in_s);
		}

		/** @return The vector result of element-wise multiplication of this * in_v. */
		CX_FORCE_INLINE CxVec2 operator*(const CxVec2 &in_v) const {
			return CxVec2(x*in_v.x, y*in_v.y);
		}

		/** @return The result of the scalar multiplication this * in_s. */
		CX_FORCE_INLINE CxVec2 operator*(CxReal in_s) const {
			return CxVec2(x*in_s, y*in_s);
		}

		/** @return The vector result of element-wise division of this / in_v. */
		CX_FORCE_INLINE CxVec2 operator/(const CxVec2 &in_v) const {
			CXD_IF_ERR((in_v.x == 0.0f || in_v.y == 0.0f),
						  "Cannot divide by Vec2 with zero element.");
			return CxVec2(x/in_v.x, y/in_v.y);
		}

		/** @return The result of the scalar division this / in_s. */
		CX_FORCE_INLINE CxVec2 operator/(CxReal in_s) const {
			in_s = 1.0f / in_s;
			return CxVec2(x*in_s, y*in_s);
		}

		/** @brief Add another vector to this vector */
		CX_FORCE_INLINE CxVec2& operator+=(const CxVec2 &in_v) {
			x += in_v.x; y += in_v.y;  return *this;
		}

		/** @brief Add a scalar value to each element of the vector */
		CX_FORCE_INLINE CxVec2 & operator+=(CxReal in_s) {
			x += in_s; y += in_s;  return *this;
		}

		/** @brief Subtract another vector from this vector */
		CX_FORCE_INLINE CxVec2& operator-=(const CxVec2 &in_v) {
			x -= in_v.x; y -= in_v.y;  return *this;
		}

		/** @brief Subtract a scalar value from each element of the vector */
		CX_FORCE_INLINE CxVec2 & operator-=(CxReal in_s) {
			x -= in_s; y -= in_s;  return *this;
		}

		/** @brief Multiply this vector by another vector, element-wise. */
		CX_FORCE_INLINE CxVec2& operator*=(const CxVec2 &in_v) {
			x *= in_v.x; y *= in_v.y; return *this;
		}

		/** @brief Multiply this vector by a scalar. */
		CX_FORCE_INLINE CxVec2& operator*=(CxReal in_s) {
			x *= in_s; y *= in_s; return *this;
		}

		/** @brief Divide this vector by another vector, element-wise. */
		CX_FORCE_INLINE CxVec2& operator/=(const CxVec2 &in_v) {
			CXD_IF_ERR(in_v.x == 0.0f || in_v.y == 0.0f,
						  "Cannot divide by a Vec2 with a zero element.");
			x /= in_v.x; y /= in_v.y; return *this;
		}

		/** @brief Divide this vector by a scalar. */
		CX_FORCE_INLINE CxVec2& operator/=(CxReal in_s) {
			const CxReal s = 1.0f / in_s;
			x *= s; y *= s; return *this;
		}
	
		/** @return True if the vector is roughly of unit length. */
		CX_FORCE_INLINE CxBool isUnit() const {
			const CxReal unit_eps = CX_VEC2_UNIT_EPS;
			return CxAbs(magnitudeSqr() - 1) < unit_eps;
		}
	
		/** @return True if all the elements are exactly zero */
		CX_FORCE_INLINE CxBool isZero() const {
			return (x == 0.0f) && (y == 0.0f);
		}

		/** @return The square of the vector's magnitude (length). */
		CX_FORCE_INLINE CxReal magnitudeSqr() const {
			return (x*x) + (y*y);
		}

		/** @return The the vector's magnitude (length). */
		CX_FORCE_INLINE CxReal magnitude() const {
			return CxSqrt(magnitudeSqr());
		}

		/** @return The maximum element of the vector. */
		CX_FORCE_INLINE CxReal max() const {
			return CxMax(x, y);
		}

		/** @return The minimum element of the vector. */
		CX_FORCE_INLINE CxReal min() const {
			return CxMin(x, y);
		}

		/** @brief Normalize the vector. */
		CX_FORCE_INLINE void normalize() {
			const CxReal mag_squared = magnitudeSqr();
			if (mag_squared > CX_VEC2_NORM_EPS) {
				(*this) *= CxRecipSqrt(mag_squared);
			}
		}

		/** @brief Normalize the vector, assuming the vector is non-zero. */
		CX_FORCE_INLINE void normalizeNonZero() {
			const CxReal mag_squared = magnitudeSqr();
			CXD_IF_CRASH((mag_squared == 0.0f), "Cannot normalize a zero-vector.");
			(*this) *= CxRecipSqrt(mag_squared);
		}

		/** @return A copy of the vector, normalized. */
		CX_FORCE_INLINE CxVec2 normalized() const {
			const CxReal mag_squared = magnitudeSqr();
			return (mag_squared > CX_VEC2_NORM_EPS) ? (*this) * CxRecipSqrt(mag_squared) : CxVec2(0.0f);
		}

		/** @return A copy of the vector, normalized, assuming that the vector is non-zero.*/
		CX_FORCE_INLINE CxVec2 normalizedNonZero() const {
			const CxReal mag_squared = magnitudeSqr();
			CXD_IF_CRASH((mag_squared == 0.0f), "Cannot normalize a zero-vector.");
			return (*this) * CxRecipSqrt(mag_squared);
		}

		/** @brief Set the vector to a zero vector. */
		CX_FORCE_INLINE void setZero() { x = y = 0; }

		/** @brief Method to set all the components of the vector. */
		CX_FORCE_INLINE void setXY(CxReal in_x, CxReal in_y) {
			x = in_x;  y = in_y;
		}
	};

	/** @return A vector containing the absolute value of each element. */
	CX_FORCE_INLINE CxVec2 CxAbs(const CxVec2& in_v) {
		return CxVec2(CxAbs(in_v.x), CxAbs(in_v.y));
	}

	/** @return A vector containing the arc cosine value of each element. */
	CX_FORCE_INLINE CxVec2 CxAcos(const CxVec2& in_v) {
		return CxVec2(CxAcos(in_v.x), CxAcos(in_v.y));
	}

	/** @return A vector containing the arc sine value of each element. */
	CX_FORCE_INLINE CxVec2 CxAsin(const CxVec2& in_v) {
		return CxVec2(CxAsin(in_v.x), CxAsin(in_v.y));
	}

	/** @return A vector containing the arc tangent value of each element. */
	CX_FORCE_INLINE CxVec2 CxAtan(const CxVec2& in_v) {
		return CxVec2(CxAtan(in_v.x), CxAtan(in_v.y));
	}

	/** @return A vector containing the ceiling of each element. */
	CX_FORCE_INLINE CxVec2 CxCeil(const CxVec2& in_v) {
		return CxVec2(CxCeil(in_v.x), CxCeil(in_v.y));
	}

	/** 
	 * @brief Clamp each element in the vector to the given range. 
	 * @param in_v The vector to clamp the elements of.
	 * @param in_min The lowest value to clamp to.
	 * @param in_max The highest value to clamp to.
	 * @return A reference to the clamped vector.
	 */
	CX_FORCE_INLINE CxVec2 & CxClamp(CxVec2& in_v, CxReal in_min, CxReal in_max) {
		CxClamp(in_v.x, in_min, in_max);
		CxClamp(in_v.y, in_min, in_max);
		return in_v;
	}

	/** 
	 * @brief Clamp each element in the vector to the given range. 
	 * @param in_v The vector to clamp the elements of.
	 * @param in_min The vector of lowest values to clamp to.
	 * @param in_max The vector highest values to clamp to.
	 * @return A reference to the clamped vector.
	 */
	template<>
	CX_FORCE_INLINE CxVec2 & CxClamp(CxVec2 &in_v, const CxVec2 &in_min, const CxVec2 &in_max) {
		CxClamp(in_v.x, in_min.x, in_max.x);
		CxClamp(in_v.y, in_min.y, in_max.y);
		return in_v;
	}

	/** @return A vector with each element clamped to a given range [min,max]. */
	CX_FORCE_INLINE CxVec2 CxClamped(const CxVec2& in_v, CxReal in_min, CxReal in_max) {
		return CxVec2(CxClamped(in_v.x, in_min, in_max),
						  CxClamped(in_v.y, in_min, in_max));
	}

	/** @return A vector with each element clamped to a given range [min,max]. */
	template<>
	CX_FORCE_INLINE CxVec2 CxClamped(const CxVec2& in_v, const CxVec2 &in_min, const CxVec2 &in_max) {
		return CxVec2(CxClamped(in_v.x, in_min.x, in_max.x),
						  CxClamped(in_v.y, in_min.y, in_max.y));
	}

	/** @return A vector containing the cosine value of each element. */
	CX_FORCE_INLINE CxVec2 CxCos(const CxVec2& in_v) {
		return CxVec2(CxCos(in_v.x), CxCos(in_v.y));
	}

	/** @return The dot product of two vectors. */
	CX_FORCE_INLINE CxReal CxDot(const CxVec2 &in_a, const CxVec2 &in_b) {
		return (in_a.x*in_b.x) + (in_a.y*in_b.y);
	}

	/** @return Return true if the two vectors (a and b) are equal within a given error. */
	CX_FORCE_INLINE CxBool CxEq(const CxVec2 &in_a, const CxVec2 &in_b, CxReal in_epsilon = CX_REAL_EPSILON) {
		return CxEq(in_a.x, in_b.x, in_epsilon) && CxEq(in_a.y, in_b.y, in_epsilon);
	}

	/** @return A vector containing each original element as an exponent of e [e^x, e^y, e^z]. */
	CX_FORCE_INLINE CxVec2 CxExp(const CxVec2& in_v) {
		return CxVec2(CxExp(in_v.x), CxExp(in_v.y));
	}

	/** @return A vector containing the floor of each element. */
	CX_FORCE_INLINE CxVec2 CxFloor(const CxVec2& in_v) {
		return CxVec2(CxFloor(in_v.x), CxFloor(in_v.y));
	}

	/** @return True if all elements of the vector are finite values (not NaN or INF). */
	CX_FORCE_INLINE CxBool CxIsFinite(const CxVec2 &in_v) {
		return CxIsFinite(in_v.x) && CxIsFinite(in_v.y);
	}

	/** @return A vector containing the log base b of each element in the vector. */
	CX_FORCE_INLINE CxVec2 CxLog(CxReal in_b, const CxVec2& in_v) {
		return CxVec2(CxLog(in_b, in_v.x), CxLog(in_b, in_v.y));
	}

	/** @return A vector containing the natural logarithm of each element. */
	CX_FORCE_INLINE CxVec2 CxLogE(const CxVec2& in_v) {
		return CxVec2(CxLogE(in_v.x), CxLogE(in_v.y));
	}

	/** @return A vector containing the logarithm base 10 of each element. */
	CX_FORCE_INLINE CxVec2 CxLog10(const CxVec2& in_v) {
		return CxVec2(CxLog10(in_v.x), CxLog10(in_v.y));
	}

	/** @return A vector containin the maximum elements of two vectors. */
	template<>
	CX_FORCE_INLINE CxVec2 CxMax(const CxVec2 &in_a, const CxVec2 &in_b) {
		return CxVec2(CxMax(in_a.x, in_b.x),
						  CxMax(in_a.y, in_b.y));
	}

	/** @return A vector containin the minimum elements of two vectors. */
	template<>
	CX_FORCE_INLINE CxVec2 CxMin(const CxVec2 &in_a, const CxVec2 &in_b) {
		return CxVec2(CxMin(in_a.x, in_b.x),
						  CxMin(in_a.y, in_b.y));
	}

	/** @return A vector containing the each element raised to the given power, y. */
	CX_FORCE_INLINE CxVec2 CxPow(const CxVec2& in_v, const CxReal in_y) {
		return CxVec2(CxPow(in_v.x, in_y), CxPow(in_v.y, in_y));
	}

	/** @return A vector containing the each element raised to the given power in y. */
	CX_FORCE_INLINE CxVec2 CxPow(const CxVec2& in_v, const CxVec2 &in_y) {
		return CxVec2(CxPow(in_v.x, in_y.x), CxPow(in_v.y, in_y.y));
	}

	/** @return A vector containing the one over the sqrt of each element (1/sqrt). */
	CX_FORCE_INLINE CxVec2 CxRecipSqrt(const CxVec2& in_v) {
		return CxVec2(CxRecipSqrt(in_v.x), CxRecipSqrt(in_v.y));
	}

	/** @return A vector containing the sign of each element (-1.0f if neg, 1.0f if pos). */
	CX_FORCE_INLINE CxVec2 CxSign(const CxVec2 &in_v) {
		return CxVec2(CxSign(in_v.x), CxSign(in_v.y));
	}
	
	/** @return A vector containing the sine value of each element. */
	CX_FORCE_INLINE CxVec2 CxSin(const CxVec2& in_v) {
		return CxVec2(CxSin(in_v.x), CxSin(in_v.y));
	}

	/** @return A vector containing the square root of each element. */
	CX_FORCE_INLINE CxVec2 CxSqrt(const CxVec2& in_v) {
		return CxVec2(CxSqrt(in_v.x), CxSqrt(in_v.y));
	}

	/** @return A vector containing the tangent value of each element. */
	CX_FORCE_INLINE CxVec2 CxTan(const CxVec2& in_v) {
		return CxVec2(CxTan(in_v.x), CxTan(in_v.y));
	}
} // namespace cat

#endif // CX_CORE_MATH_CXVEC2_H
