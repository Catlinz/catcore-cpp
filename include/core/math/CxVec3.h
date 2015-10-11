#ifndef CX_CORE_MATH_CXVEC3_H
#define CX_CORE_MATH_CXVEC3_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxVec3.h
 * @brief Contains the definition of the 3d vector class and math functions.
 *
 * @author Catlin Zilinski
 * @date June 1, 2015
 */

#include "core/Cx.h"
#include "core/math/CxMath.h"

#  define CX_VEC3_UNIT_EPS 1e-7f
#  define CX_VEC3_NORM_EPS 1e-10f

namespace cat {

	class CxVec3 {
	  public:
		static const CxVec3 kLeft, kRight;
		static const CxVec3 kUp, kDown;
		static const CxVec3 kForward, kBackward;
		
		CxReal x, y, z;

		/** @brief Vector is initialise to garbage. */
		CX_FORCE_INLINE CxVec3() {}
	
		/** @brief Initialise the vector to all zeros. */
		CX_FORCE_INLINE CxVec3(CxZero) : x(0.0f), y(0.0f), z(0.0f) {}

		/**
		 * @brief Initialise all elements to the same value.
		 * @param in_val The value to initialise the elements to.
		 */
		explicit CX_FORCE_INLINE CxVec3(CxReal in_val)
			: x(in_val), y(in_val), z(in_val) {}

		/**
		 * @brief Initialise the elements from an array.
		 * @param in_arr The array of reals.
		 */
		explicit CX_FORCE_INLINE CxVec3(const CxReal *in_arr)
			: x(in_arr[0]), y(in_arr[1]), z(in_arr[2]) {}

		/**
		 * @brief Initialise the elements of the vector.
		 * @param in_x The x-component of the vector.
		 * @param in_y The y-component of the vector.
		 * @param in_z The z-component of the vector.
		 */
		CX_FORCE_INLINE CxVec3(CxReal in_x, CxReal in_y, CxReal in_z)
			: x(in_x), y(in_y), z(in_z) {}

		/** @brief Copy constructor, simply copies the elements. */
		CX_FORCE_INLINE CxVec3(const CxVec3& in_src)
			: x(in_src.x), y(in_src.y), z(in_src.z) {}

		/** @brief Overloaded assignment operator */
		CX_FORCE_INLINE CxVec3& operator=(const CxVec3& in_src) {
			x = in_src.x; y = in_src.y; z = in_src.z; return *this;
		}

		/** @brief Array style element access. */
		CX_FORCE_INLINE CxReal& operator[](CxI32 in_idx) {
			CXD_IF_ERR((in_idx < 0 || in_idx > 2), "CxVec3 index out of bounds!");
			return reinterpret_cast<CxReal *>(this)[in_idx];
		}

		/** @brief Const array style element access. */
		CX_FORCE_INLINE const CxReal& operator[](CxI32 in_idx) const {
			CXD_IF_ERR((in_idx < 0 || in_idx > 2), "CxVec3 index out of bounds!");
			return reinterpret_cast<const CxReal *>(this)[in_idx];
		}

		/** @return True if the two vectors are strictly equal. */
		CX_FORCE_INLINE CxBool operator==(const CxVec3 &in_v) const {
			return (x == in_v.x) && (y == in_v.y) && (z == in_v.z);
		}

		/** @return True if the two vectors are not strictly equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxVec3 &in_v) const {
			return (x != in_v.x) || (y != in_v.y) || (z != in_v.z);
		}

		/** @return The negation of the vector. */
		CX_FORCE_INLINE CxVec3 operator-() const {
			return CxVec3(-x, -y, -z);
		}

		/** @return The result of the vector addition this + in_v. */
		CX_FORCE_INLINE CxVec3 operator+(const CxVec3 &in_v) const {
			return CxVec3(x + in_v.x, y + in_v.y, z + in_v.z);
		}

		/** @return The result of adding a scalar to each element of the vector. */
		CX_FORCE_INLINE CxVec3 operator+(CxReal in_s) const {
			return CxVec3(x + in_s, y + in_s, z + in_s);
		}

		/** @return The result of the vector subtraction this - in_v. */
		CX_FORCE_INLINE CxVec3 operator-(const CxVec3 &in_v) const {
			return CxVec3(x - in_v.x, y - in_v.y, z - in_v.z);
		}

		/** @return The result of subtracting a scalar from each element of the vector. */
		CX_FORCE_INLINE CxVec3 operator-(CxReal in_s) const {
			return CxVec3(x - in_s, y - in_s, z - in_s);
		}

		/** @return The vector result of element-wise multiplication of this * in_v. */
		CX_FORCE_INLINE CxVec3 operator*(const CxVec3 &in_v) const {
			return CxVec3(x*in_v.x, y*in_v.y, z*in_v.z);
		}

		/** @return The result of the scalar multiplication this * in_s. */
		CX_FORCE_INLINE CxVec3 operator*(CxReal in_s) const {
			return CxVec3(x*in_s, y*in_s, z*in_s);
		}

		/** @return The vector result of element-wise division of this / in_v. */
		CX_FORCE_INLINE CxVec3 operator/(const CxVec3 &in_v) const {
			CXD_IF_ERR((in_v.x == 0.0f || in_v.y == 0.0f || in_v.z == 0.0f),
						  "Cannot divide by vector with zero element.");
			return CxVec3(x/in_v.x, y/in_v.y, z/in_v.z);
		}

		/** @return The result of the scalar division this / in_s. */
		CX_FORCE_INLINE CxVec3 operator/(CxReal in_s) const {
			const CxReal s = 1.0f / in_s;
			return CxVec3(x*s, y*s, z*s);
		}

		/** @brief Add another vector to this vector */
		CX_FORCE_INLINE CxVec3& operator+=(const CxVec3 &in_v) {
			x += in_v.x; y += in_v.y; z += in_v.z;  return *this;
		}

		/** @brief Add a scalar value to each element of the vector */
		CX_FORCE_INLINE CxVec3 & operator+=(CxReal in_s) {
			x += in_s; y += in_s; z += in_s;  return *this;
		}

		/** @brief Subtract another vector from this vector */
		CX_FORCE_INLINE CxVec3& operator-=(const CxVec3 &in_v) {
			x -= in_v.x; y -= in_v.y; z -= in_v.z;  return *this;
		}

		/** @brief Subtract a scalar value from each element of the vector */
		CX_FORCE_INLINE CxVec3 & operator-=(CxReal in_s) {
			x -= in_s; y -= in_s; z -= in_s;  return *this;
		}

		/** @brief Multiply this vector by another vector, element-wise. */
		CX_FORCE_INLINE CxVec3& operator*=(const CxVec3 &in_v) {
			x *= in_v.x; y *= in_v.y; z *= in_v.z; return *this;
		}

		/** @brief Multiply this vector by a scalar. */
		CX_FORCE_INLINE CxVec3& operator*=(CxReal in_s) {
			x *= in_s; y *= in_s; z *= in_s; return *this;
		}

		/** @brief Divide this vector by another vector, element-wise. */
		CX_FORCE_INLINE CxVec3& operator/=(const CxVec3 &in_v) {
			CXD_IF_ERR(in_v.x == 0.0f || in_v.y == 0.0f || in_v.z == 0.0f,
						  "Cannot divide by a vector with a zero element.");
			x /= in_v.x; y /= in_v.y; z /= in_v.z; return *this;
		}

		/** @brief Divide this vector by a scalar. */
		CX_FORCE_INLINE CxVec3& operator/=(CxReal in_s) {
			const CxReal s = 1.0f / in_s;
			x *= s; y *= s; z *= s; return *this;
		}
	
		/** @return True if the vector is roughly of unit length. */
		CX_FORCE_INLINE CxBool isUnit() const {
			const CxReal unit_eps = CX_VEC3_UNIT_EPS;
			return CxAbs(magnitudeSqr() - 1) < unit_eps;
		}
	
		/** @return True if all the elements are exactly zero */
		CX_FORCE_INLINE CxBool isZero() const {
			return (x == 0.0f) && (y == 0.0f) && (z == 0.0f);
		}

		/** @return The square of the vector's magnitude (length). */
		CX_FORCE_INLINE CxReal magnitudeSqr() const {
			return (x*x) + (y*y) + (z*z);
		}

		/** @return The the vector's magnitude (length). */
		CX_FORCE_INLINE CxReal magnitude() const {
			return CxSqrt(magnitudeSqr());
		}

		/** @return The maximum element of the vector. */
		CX_FORCE_INLINE CxReal max() const {
			return CxMax(x, CxMax(y, z));
		}

		/** @return The minimum element of the vector. */
		CX_FORCE_INLINE CxReal min() const {
			return CxMin(x, CxMin(y, z));
		}

		/** @brief Normalize the vector. */
		CX_FORCE_INLINE void normalize() {
			const CxReal mag_squared = magnitudeSqr();
			if (mag_squared > CX_VEC3_NORM_EPS) {
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
		CX_FORCE_INLINE CxVec3 normalized() const {
			const CxReal mag_squared = magnitudeSqr();
			return (mag_squared > CX_VEC3_NORM_EPS) ? (*this) * CxRecipSqrt(mag_squared) : CxVec3(0.0f);
		}

		/** @return A copy of the vector, normalized, assuming that the vector is non-zero.*/
		CX_FORCE_INLINE CxVec3 normalizedNonZero() const {
			const CxReal mag_squared = magnitudeSqr();
			CXD_IF_CRASH((mag_squared == 0.0f), "Cannot normalize a zero-vector.");
			return (*this) * CxRecipSqrt(mag_squared);
		}

		/** @brief Set the vector to a zero vector. */
		CX_FORCE_INLINE void setZero() { x = y = z = 0; }
		
		/** @brief Methods to set only two components of the vector. */
		CX_FORCE_INLINE void setXY(CxReal in_x, CxReal in_y) { x = in_x;  y = in_y; }
		CX_FORCE_INLINE void setXZ(CxReal in_x, CxReal in_z) { x = in_x;  z = in_z; }
		CX_FORCE_INLINE void setYZ(CxReal in_y, CxReal in_z) { y = in_y;  z = in_z; }

		/** @brief Method to set all the components of the vector. */
		CX_FORCE_INLINE void setXYZ(CxReal in_x, CxReal in_y, CxReal in_z) {
			x = in_x;  y = in_y;  z = in_z;
		}
	};

	/** @return A vector containing the absolute value of each element. */
	CX_FORCE_INLINE CxVec3 CxAbs(const CxVec3& in_v) {
		return CxVec3(CxAbs(in_v.x), CxAbs(in_v.y), CxAbs(in_v.z));
	}

	/** @return A vector containing the arc cosine value of each element. */
	CX_FORCE_INLINE CxVec3 CxAcos(const CxVec3& in_v) {
		return CxVec3(CxAcos(in_v.x), CxAcos(in_v.y), CxAcos(in_v.z));
	}

	/** @return A vector containing the arc sine value of each element. */
	CX_FORCE_INLINE CxVec3 CxAsin(const CxVec3& in_v) {
		return CxVec3(CxAsin(in_v.x), CxAsin(in_v.y), CxAsin(in_v.z));
	}

	/** @return A vector containing the arc tangent value of each element. */
	CX_FORCE_INLINE CxVec3 CxAtan(const CxVec3& in_v) {
		return CxVec3(CxAtan(in_v.x), CxAtan(in_v.y), CxAtan(in_v.z));
	}

	/** @return A vector containing the ceiling of each element. */
	CX_FORCE_INLINE CxVec3 CxCeil(const CxVec3& in_v) {
		return CxVec3(CxCeil(in_v.x), CxCeil(in_v.y), CxCeil(in_v.z));
	}

	/** 
	 * @brief Clamp each element in the vector to the given range. 
	 * @param in_v The vector to clamp the elements of.
	 * @param in_min The lowest value to clamp to.
	 * @param in_max The highest value to clamp to.
	 * @return A reference to the clamped vector.
	 */
	CX_FORCE_INLINE CxVec3 & CxClamp(CxVec3& in_v, CxReal in_min, CxReal in_max) {
		CxClamp(in_v.x, in_min, in_max);
		CxClamp(in_v.y, in_min, in_max);
		CxClamp(in_v.z, in_min, in_max);
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
	CX_FORCE_INLINE CxVec3 & CxClamp(CxVec3 &in_v, const CxVec3 &in_min, const CxVec3 &in_max) {
		CxClamp(in_v.x, in_min.x, in_max.x);
		CxClamp(in_v.y, in_min.y, in_max.y);
		CxClamp(in_v.z, in_min.z, in_max.z);
		return in_v;
	}

	/** @return A vector with each element clamped to a given range [min,max]. */
	CX_FORCE_INLINE CxVec3 CxClamped(const CxVec3& in_v, CxReal in_min, CxReal in_max) {
		return CxVec3(CxClamped(in_v.x, in_min, in_max),
						  CxClamped(in_v.y, in_min, in_max),
						  CxClamped(in_v.z, in_min, in_max));
	}

	/** @return A vector with each element clamped to a given range [min,max]. */
	template<>
	CX_FORCE_INLINE CxVec3 CxClamped(const CxVec3& in_v, const CxVec3 &in_min, const CxVec3 &in_max) {
		return CxVec3(CxClamped(in_v.x, in_min.x, in_max.x),
						  CxClamped(in_v.y, in_min.y, in_max.y),
						  CxClamped(in_v.z, in_min.z, in_max.z));
	}

	/** @return A vector containing the cosine value of each element. */
	CX_FORCE_INLINE CxVec3 CxCos(const CxVec3& in_v) {
		return CxVec3(CxCos(in_v.x), CxCos(in_v.y), CxCos(in_v.z));
	}

	/** @return The cross product of two vectors. */
	CX_FORCE_INLINE CxVec3 CxCross(const CxVec3 &in_a, const CxVec3 &in_b) {
		return CxVec3(in_a.y*in_b.z - in_a.z*in_b.y,
						  in_a.z*in_b.x - in_a.x*in_b.z,
						  in_a.x*in_b.y - in_a.y*in_b.x);
	}
	

	/** @return The dot product of two vectors. */
	CX_FORCE_INLINE CxReal CxDot(const CxVec3 &in_a, const CxVec3 &in_b) {
		return (in_a.x*in_b.x) + (in_a.y*in_b.y) + (in_a.z*in_b.z);
	}

	/** @return Return true if the two vectors (a and b) are equal within a given error. */
	CX_FORCE_INLINE CxBool CxEq(const CxVec3 &in_a, const CxVec3 &in_b, CxReal in_epsilon = CX_REAL_EPSILON) {
		return (CxEq(in_a.x, in_b.x, in_epsilon) &&
				  CxEq(in_a.y, in_b.y, in_epsilon) &&
				  CxEq(in_a.z, in_b.z, in_epsilon));
	}

	/** @return A vector containing each original element as an exponent of e [e^x, e^y, e^z]. */
	CX_FORCE_INLINE CxVec3 CxExp(const CxVec3& in_v) {
		return CxVec3(CxExp(in_v.x), CxExp(in_v.y), CxExp(in_v.z));
	}

	/** @return A vector containing the floor of each element. */
	CX_FORCE_INLINE CxVec3 CxFloor(const CxVec3& in_v) {
		return CxVec3(CxFloor(in_v.x), CxFloor(in_v.y), CxFloor(in_v.z));
	}

	/** @return True if all elements of the vector are finite values (not NaN or INF). */
	CX_FORCE_INLINE CxBool CxIsFinite(const CxVec3 &in_v) {
		return CxIsFinite(in_v.x) && CxIsFinite(in_v.y) && CxIsFinite(in_v.z);
	}

	/** @return A vector containing the log base b of each element in the vector. */
	CX_FORCE_INLINE CxVec3 CxLog(CxReal in_b, const CxVec3& in_v) {
		return CxVec3(CxLog(in_b, in_v.x), CxLog(in_b, in_v.y), CxLog(in_b, in_v.z));
	}

	/** @return A vector containing the natural logarithm of each element. */
	CX_FORCE_INLINE CxVec3 CxLogE(const CxVec3& in_v) {
		return CxVec3(CxLogE(in_v.x), CxLogE(in_v.y), CxLogE(in_v.z));
	}

	/** @return A vector containing the logarithm base 10 of each element. */
	CX_FORCE_INLINE CxVec3 CxLog10(const CxVec3& in_v) {
		return CxVec3(CxLog10(in_v.x), CxLog10(in_v.y), CxLog10(in_v.z));
	}

	/** @return A vector containin the maximum elements of two vectors. */
	template<>
	CX_FORCE_INLINE CxVec3 CxMax(const CxVec3 &in_a, const CxVec3 &in_b) {
		return CxVec3(CxMax(in_a.x, in_b.x),
						  CxMax(in_a.y, in_b.y),
						  CxMax(in_a.z, in_b.z));
	}

	/** @return A vector containin the minimum elements of two vectors. */
	template<>
	CX_FORCE_INLINE CxVec3 CxMin(const CxVec3 &in_a, const CxVec3 &in_b) {
		return CxVec3(CxMin(in_a.x, in_b.x),
						  CxMin(in_a.y, in_b.y),
						  CxMin(in_a.z, in_b.z));
	}

	/** @return A vector containing the each element raised to the given power, y. */
	CX_FORCE_INLINE CxVec3 CxPow(const CxVec3& in_v, const CxReal in_y) {
		return CxVec3(CxPow(in_v.x, in_y), CxPow(in_v.y, in_y), CxPow(in_v.z, in_y));
	}

	/** @return A vector containing the each element raised to the given power in y. */
	CX_FORCE_INLINE CxVec3 CxPow(const CxVec3& in_v, const CxVec3 &in_y) {
		return CxVec3(CxPow(in_v.x, in_y.x), CxPow(in_v.y, in_y.y), CxPow(in_v.z, in_y.z));
	}

	/** @return A vector containing the one over the sqrt of each element (1/sqrt). */
	CX_FORCE_INLINE CxVec3 CxRecipSqrt(const CxVec3& in_v) {
		return CxVec3(CxRecipSqrt(in_v.x), CxRecipSqrt(in_v.y), CxRecipSqrt(in_v.z));
	}

	/** @return A vector containing the sign of each element (-1.0f if neg, 1.0f if pos). */
	CX_FORCE_INLINE CxVec3 CxSign(const CxVec3 &in_v) {
		return CxVec3(CxSign(in_v.x), CxSign(in_v.y), CxSign(in_v.z));
	}
	
	/** @return A vector containing the sine value of each element. */
	CX_FORCE_INLINE CxVec3 CxSin(const CxVec3& in_v) {
		return CxVec3(CxSin(in_v.x), CxSin(in_v.y), CxSin(in_v.z));
	}

	/** @return A vector containing the square root of each element. */
	CX_FORCE_INLINE CxVec3 CxSqrt(const CxVec3& in_v) {
		return CxVec3(CxSqrt(in_v.x), CxSqrt(in_v.y), CxSqrt(in_v.z));
	}

	/** @return A vector containing the tangent value of each element. */
	CX_FORCE_INLINE CxVec3 CxTan(const CxVec3& in_v) {
		return CxVec3(CxTan(in_v.x), CxTan(in_v.y), CxTan(in_v.z));
	}
} // namespace cat

#endif // CX_CORE_MATH_CXVEC3_H
