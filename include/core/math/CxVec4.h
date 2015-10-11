#ifndef CX_CORE_MATH_CXVEC4_H
#define CX_CORE_MATH_CXVEC4_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxVec4.h
 * @brief Contains the definition of the 4d vector class and math functions.
 *
 * @author Catlin Zilinski
 * @date June 2, 2015
 */

#include "core/Cx.h"
#include "core/math/CxMath.h"
#include "core/math/CxVec3.h"

#  define CX_VEC4_UNIT_EPS 1e-6f
#  define CX_VEC4_NORM_EPS 1e-10f

namespace cat {

	class CxVec4 {
	  public:
		CxReal x, y, z, w;

		/** @brief Vector is initialise to garbage. */
		CX_FORCE_INLINE CxVec4() {}
	
		/** @brief Initialise the vector to all zeros. */
		CX_FORCE_INLINE CxVec4(CxZero)
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

		/** @brief Initialise all elements to the same value. */
		explicit CX_FORCE_INLINE CxVec4(CxReal in_val)
			: x(in_val), y(in_val), z(in_val), w(in_val) {}

		/** @brief Initialise the elements from an array. */
		explicit CX_FORCE_INLINE CxVec4(const CxReal *in_arr)
			: x(in_arr[0]), y(in_arr[1]), z(in_arr[2]), w(in_arr[3]) {}

		/**
		 * @brief Initialise the vector from a CxVec3.
		 * @param in_v The CxVec3 to set the x,y,z components from.
		 * @param in_w The w-component of the vector.
		 */
		CX_FORCE_INLINE CxVec4(const CxVec3 &in_v, CxReal in_w)
			: x(in_v.x), y(in_v.y), z(in_v.z), w(in_w) {}

		/** @brief Initialise the elements of the vector. */
		CX_FORCE_INLINE CxVec4(CxReal in_x, CxReal in_y, CxReal in_z, CxReal in_w)
			: x(in_x), y(in_y), z(in_z), w(in_w) {}

		/** @brief Copy constructor, simply copies the elements. */
		CX_FORCE_INLINE CxVec4(const CxVec4& in_src)
			: x(in_src.x), y(in_src.y), z(in_src.z), w(in_src.w) {}

		/** @brief Overloaded assignment operator */
		CX_FORCE_INLINE CxVec4& operator=(const CxVec4& in_src) {
			x = in_src.x; y = in_src.y; z = in_src.z; w = in_src.w;  return *this;
		}

		/** @brief Array style element access. */
		CX_FORCE_INLINE CxReal& operator[](CxI32 in_idx) {
			CXD_IF_ERR((in_idx < 0 || in_idx > 3), "CxVec4 index out of bounds!");
			return reinterpret_cast<CxReal *>(this)[in_idx];
		}

		/** @brief Const array style element access. */
		CX_FORCE_INLINE const CxReal& operator[](CxI32 in_idx) const {
			CXD_IF_ERR((in_idx < 0 || in_idx > 2), "CxVec4 index out of bounds!");
			return reinterpret_cast<const CxReal *>(this)[in_idx];
		}

		/** @return True if the two vectors are strictly equal. */
		CX_FORCE_INLINE CxBool operator==(const CxVec4 &in_v) const {
			return (x == in_v.x) && (y == in_v.y) && (z == in_v.z) && (w == in_v.w);
		}

		/** @return True if the two vectors are not strictly equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxVec4 &in_v) const {
			return (x != in_v.x) || (y != in_v.y) || (z != in_v.z) || (w != in_v.w);
		}

		/** @return The negation of the vector. */
		CX_FORCE_INLINE CxVec4 operator-() const {
			return CxVec4(-x, -y, -z, -w);
		}

		/** @return The result of the vector addition this + in_v. */
		CX_FORCE_INLINE CxVec4 operator+(const CxVec4 &in_v) const {
			return CxVec4(x + in_v.x, y + in_v.y, z + in_v.z, w + in_v.w);
		}

		/** @return The result of adding a scalar to each element of the vector. */
		CX_FORCE_INLINE CxVec4 operator+(CxReal in_s) const {
			return CxVec4(x + in_s, y + in_s, z + in_s, w + in_s);
		}

		/** @return The result of the vector subtraction this - in_v. */
		CX_FORCE_INLINE CxVec4 operator-(const CxVec4 &in_v) const {
			return CxVec4(x - in_v.x, y - in_v.y, z - in_v.z, w - in_v.w);
		}

		/** @return The result of subtracting a scalar from each element of the vector. */
		CX_FORCE_INLINE CxVec4 operator-(CxReal in_s) const {
			return CxVec4(x - in_s, y - in_s, z - in_s, w - in_s);
		}

		/** @return The vector result of element-wise multiplication of this * in_v. */
		CX_FORCE_INLINE CxVec4 operator*(const CxVec4 &in_v) const {
			return CxVec4(x*in_v.x, y*in_v.y, z*in_v.z, w*in_v.w);
		}

		/** @return The result of the scalar multiplication this * in_s. */
		CX_FORCE_INLINE CxVec4 operator*(CxReal in_s) const {
			return CxVec4(x*in_s, y*in_s, z*in_s, w*in_s);
		}

		/** @return The vector result of element-wise division of this / in_v. */
		CX_FORCE_INLINE CxVec4 operator/(const CxVec4 &in_v) const {
			CXD_IF_ERR((in_v.x == 0.0f || in_v.y == 0.0f || in_v.z == 0.0f || in_w.z == 0.0f),
						  "Cannot divide by vector with zero element.");
			return CxVec4(x/in_v.x, y/in_v.y, z/in_v.z, w/in_v.w);
		}

		/** @return The result of the scalar division this / in_s. */
		CX_FORCE_INLINE CxVec4 operator/(CxReal in_s) const {
			in_s = 1.0f / in_s;
			return CxVec4(x*in_s, y*in_s, z*in_s, w*in_s);
		}

		/** @brief Add another vector to this vector */
		CX_FORCE_INLINE CxVec4& operator+=(const CxVec4 &in_v) {
			x += in_v.x; y += in_v.y; z += in_v.z; w += in_v.w;  return *this;
		}

		/** @brief Add a scalar value to each element of the vector */
		CX_FORCE_INLINE CxVec4 & operator+=(CxReal in_s) {
			x += in_s; y += in_s; z += in_s; w += in_s;  return *this;
		}

		/** @brief Subtract another vector from this vector */
		CX_FORCE_INLINE CxVec4& operator-=(const CxVec4 &in_v) {
			x -= in_v.x; y -= in_v.y; z -= in_v.z; w-= in_v.w;  return *this;
		}

		/** @brief Subtract a scalar value from each element of the vector */
		CX_FORCE_INLINE CxVec4 & operator-=(CxReal in_s) {
			x -= in_s; y -= in_s; z -= in_s; w -= in_s;  return *this;
		}

		/** @brief Multiply this vector by another vector, element-wise. */
		CX_FORCE_INLINE CxVec4& operator*=(const CxVec4 &in_v) {
			x *= in_v.x; y *= in_v.y; z *= in_v.z; w *= in_v.w;  return *this;
		}

		/** @brief Multiply this vector by a scalar. */
		CX_FORCE_INLINE CxVec4& operator*=(CxReal in_s) {
			x *= in_s; y *= in_s; z *= in_s; w *= in_s;  return *this;
		}

		/** @brief Divide this vector by another vector, element-wise. */
		CX_FORCE_INLINE CxVec4& operator/=(const CxVec4 &in_v) {
			CXD_IF_ERR(in_v.x == 0.0f || in_v.y == 0.0f || in_v.z == 0.0f || in_v.w == 0.0f,
						  "Cannot divide by a CxVec4 with a zero element.");
			x *= in_v.x; y *= in_v.y; z *= in_v.z; w *= in_v.w;  return *this;
		}

		/** @brief Divide this vector by a scalar. */
		CX_FORCE_INLINE CxVec4& operator/=(CxReal in_s) {
			in_s = 1.0f / in_s;
			x *= in_s; y *= in_s; z *= in_s; w *= in_s;  return *this;
		}
	
		/** @return True if the vector is roughly of unit length. */
		CX_FORCE_INLINE CxBool isUnit() const {
			const CxReal unit_eps = CX_VEC4_UNIT_EPS;
			return CxAbs(magnitudeSquared() - 1) < unit_eps;
		}
	
		/** @return True if all the elements are exactly zero */
		CX_FORCE_INLINE CxBool isZero() const {
			return (x == 0.0f) && (y == 0.0f) && (z == 0.0f) && (w == 0.0f);
		}

		/** @return The square of the vector's magnitude (length). */
		CX_FORCE_INLINE CxReal magnitudeSquared() const {
			return (x*x) + (y*y) + (z*z) + (w*w);
		}

		/** @return The the vector's magnitude (length). */
		CX_FORCE_INLINE CxReal magnitude() const {
			return CxSqrt(magnitudeSquared());
		}

		/** @return The maximum element of the vector. */
		CX_FORCE_INLINE CxReal max() const {
			return CxMax(CxMax(x, y), CxMax(z, w));
		}

		/** @return The minimum element of the vector. */
		CX_FORCE_INLINE CxReal min() const {
			return CxMin(CxMin(x, y), CxMin(z, w));
		}

		/** @brief Normalize the vector. */
		CX_FORCE_INLINE void normalize() {
			const CxReal mag_squared = magnitudeSquared();
			if (mag_squared > CX_VEC4_NORM_EPS) {
				(*this) *= CxRecipSqrt(mag_squared);
			}
		}

		/** @brief Normalize the vector, assuming the vector is non-zero. */
		CX_FORCE_INLINE void normalizeNonZero() {
			const CxReal mag_squared = magnitudeSquared();
			CXD_IF_CRASH((mag_squared == 0.0f), "Cannot normalize a zero-vector.");
			(*this) *= CxRecipSqrt(mag_squared);
		}

		/** @return A copy of the vector, normalized. */
		CX_FORCE_INLINE CxVec4 normalized() const {
			const CxReal mag_squared = magnitudeSquared();
			return (mag_squared > CX_VEC4_NORM_EPS) ? (*this) * CxRecipSqrt(mag_squared) : CxVec4(0.0f);
		}

		/** @return A copy of the vector, normalized, assuming that the vector is non-zero.*/
		CX_FORCE_INLINE CxVec4 normalizedNonZero() const {
			const CxReal mag_squared = magnitudeSquared();
			CXD_IF_CRASH((mag_squared == 0.0f), "Cannot normalize a zero-vector.");
			return (*this) * CxRecipSqrt(mag_squared);
		}

		/** @brief Set the vector to a zero vector. */
		CX_FORCE_INLINE void setZero() { x = y = z = w = 0.0f; }

		/** @return A CxVec3 from the x, y and z components of this vector. */
		CX_FORCE_INLINE CxVec3 xyz() const { return CxVec3(x, y, z); }

		/** @brief Set the first three components of the vector */
		CX_FORCE_INLINE void setXYZ(CxReal in_x, CxReal in_y, CxReal in_z) {
			x = in_x;  y = in_y;  z = in_z;
		}

		/** @brief Method to set all the components of the vector. */
		CX_FORCE_INLINE void setXYZW(CxReal in_x, CxReal in_y, CxReal in_z CxReal in_w) {
			x = in_x;  y = in_y;  z = in_z;  w = in_w;
		}
	};

	/**
	 * @brief Overloaded method to multiply a scalar by a vector (in_s * in_v).
	 * @return The vector result of in_s * in_v.
	 */
	CX_FORCE_INLINE CxVec4 operator*(CxReal in_s, const CxVec4 &in_v) {
		return CxVec4(in_s*in_v.x, in_s*in_v.y, in_s*in_v.z, in_s*in_v.w);
	}

	/** @return A vector containing the absolute value of each element. */
	CX_FORCE_INLINE CxVec4 CxAbs(const CxVec4& in_v) {
		return CxVec4(CxAbs(in_v.x), CxAbs(in_v.y), CxAbs(in_v.z), CxAbs(in_v.w));
	}

	/** @return A vector containing the arc cosine value of each element. */
	CX_FORCE_INLINE CxVec4 CxAcos(const CxVec4& in_v) {
		return CxVec4(CxAcos(in_v.x), CxAcos(in_v.y), CxAcos(in_v.z), CxAcos(in_v.w));
	}

	/** @return A vector containing the arc sine value of each element. */
	CX_FORCE_INLINE CxVec4 CxAsin(const CxVec4& in_v) {
		return CxVec4(CxAsin(in_v.x), CxAsin(in_v.y), CxAsin(in_v.z), CxAsin(in_v.w));
	}

	/** @return A vector containing the arc tangent value of each element. */
	CX_FORCE_INLINE CxVec4 CxAtan(const CxVec4& in_v) {
		return CxVec4(CxAtan(in_v.x), CxAtan(in_v.y), CxAtan(in_v.z), CxAtan(in_v.w));
	}

	/** @return A vector containing the ceiling of each element. */
	CX_FORCE_INLINE CxVec4 CxCeil(const CxVec4& in_v) {
		return CxVec4(CxCeil(in_v.x), CxCeil(in_v.y), CxCeil(in_v.z), CxCeil(in_v.w));
	}

	/** 
	 * @brief Clamp each element in the vector to the given range. 
	 * @param in_v The vector to clamp the elements of.
	 * @param in_min The lowest value to clamp to.
	 * @param in_max The highest value to clamp to.
	 * @return A reference to the clamped vector.
	 */
	template <>
	CX_FORCE_INLINE CxVec4 & CxClamp(CxVec4& in_v, CxReal in_min, CxReal in_max) {
		CxClamp(in_v.x, in_min, in_max);
		CxClamp(in_v.y, in_min, in_max);
		CxClamp(in_v.z, in_min, in_max);
		CxClamp(in_v.w, in_min, in_max);
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
	CX_FORCE_INLINE CxVec4 & CxClamp(CxVec4 &in_v, const CxVec4 &in_min, const CxVec4 &in_max) {
		CxClamp(in_v.x, in_min.x, in_max.x);
		CxClamp(in_v.y, in_min.y, in_max.y);
		CxClamp(in_v.z, in_min.z, in_max.z);
		CxClamp(in_v.w, in_min.w, in_max.w);
		return in_v;
	}

	/** @return A vector with each element clamped to a given range [min,max]. */
	template<>
	CX_FORCE_INLINE CxVec4 CxClamped(const CxVec4& in_v, CxReal in_min, CxReal in_max) {
		return CxVec4(CxClamped(in_v.x, in_min, in_max),
						  CxClamped(in_v.y, in_min, in_max),
						  CxClamped(in_v.z, in_min, in_max),
						  CxClamped(in_v.w, in_min, in_max));
	}

	/** @return A vector with each element clamped to a given range [min,max]. */
	template<>
	CX_FORCE_INLINE CxVec4 CxClamped(const CxVec4& in_v, const CxVec4 &in_min, const CxVec4 &in_maxx) {
		return CxVec4(CxClamped(in_v.x, in_min.x, in_max.x),
						  CxClamped(in_v.y, in_min.y, in_max.y),
						  CxClamped(in_v.z, in_min.z, in_max.z),
						  CxClamped(in_v.w, in_min.w, in_max.w));
	}

	/** @return A vector containing the cosine value of each element. */
	CX_FORCE_INLINE CxVec4 CxCos(const CxVec4& in_v) {
		return CxVec4(CxCos(in_v.x), CxCos(in_v.y), CxCos(in_v.z), CxCos(in_v.w));
	}

	/** @return The dot product of two vectors. */
	CX_FORCE_INLINE CxReal CxDot(const CxVec4 &in_a, const CxVec4 &in_b) {
		return (in_a.x*in_b.x) + (in_a.y*in_b.y) + (in_a.z*in_b.z) + (in_a.w*in_b.w);
	}

	/** @return Return true if the two vectors (a and b) are equal within a given error. */
	CX_FORCE_INLINE CxVec4 CxEq(const CxVec4 &in_a, const CxVec4 &in_b, CxReal in_epsilon = CX_REAL_EPSILON) {
		return CxEq(in_a.x, in_b.x) && CxEq(in_a.y, in_b.y) && CxEq(in_a.z, in_b.z) && CxEq(in_a.w, in_b.w);
	}

	/** @return A vector containing each original element as an exponent of e [e^x, e^y, e^z]. */
	CX_FORCE_INLINE CxVec4 CxExp(const CxVec4& in_v) {
		return CxVec4(CxExp(in_v.x), CxExp(in_v.y), CxExp(in_v.z), CxExp(in_v.w));
	}

	/** @return A vector containing the floor of each element. */
	CX_FORCE_INLINE CxVec4 CxFloor(const CxVec4& in_v) {
		return CxVec4(CxFloor(in_v.x), CxFloor(in_v.y), CxFloor(in_v.z), CxFloor(in_v.w));
	}

	/** @return True if all elements of the vector are finite values (not NaN or INF). */
	CX_FORCE_INLINE CxBool CxIsFinite(const CxVec4 &in_v) {
		return isFinite(in_v.x) && isFinite(in_v.y) && isFinite(in_v.z) && isFinite(in_v.w);
	}

	/** @return A vector containing the log base b of each element in the vector. */
	CX_FORCE_INLINE CxVec4 CxLog(CxReal in_b, const CxVec4& in_v) {
		return CxVec4(CxLog(in_b, in_v.x), CxLog(in_b, in_v.y), CxLog(in_b, in_v.z), CxLog(in_b, in_v.z));
	}

	/** @return A vector containing the natural logarithm of each element. */
	CX_FORCE_INLINE CxVec4 CxLogE(const CxVec4& in_v) {
		return CxVec4(CxLogE(in_v.x), CxLogE(in_v.y), CxLogE(in_v.z), CxLogE(in_v.w));
	}

	/** @return A vector containing the logarithm base 10 of each element. */
	CX_FORCE_INLINE CxVec4 CxLog10(const CxVec4& in_v) {
		return CxVec4(CxLog10(in_v.x), CxLog10(in_v.y), CxLog10(in_v.z), CxLog10(in_v.w));
	}

	/** @return A vector containin the maximum elements of two vectors. */
	template<>
	CX_FORCE_INLINE CxVec4 CxMax(const CxVec4 &in_a, const CxVec4 &in_b) {
		return CxVec4(CxMax(in_a.x, in_b.x),
						  CxMax(in_a.y, in_b.y),
						  CxMax(in_a.z, in_b.z),
						  CxMax(in_a.w, in_b.w));
	}

	/** @return A vector containin the minimum elements of two vectors. */
	template<>
	CX_FORCE_INLINE CxVec4 CxMin(const CxVec4 &in_a, const CxVec4 &in_b) {
		return CxVec4(CxMin(in_a.x, in_b.x),
						  CxMin(in_a.y, in_b.y),
						  CxMin(in_a.z, in_b.z),
						  CxMin(in_a.w, in_b.w));
	}

	/** @return A vector containing the each element raised to the given power, y. */
	CX_FORCE_INLINE CxVec4 CxPow(const CxVec4& in_v, const CxReal in_y) {
		return CxVec4(CxPow(in_v.x, in_y), CxPow(in_v.y, in_y), CxPow(in_v.z, in_y), CxPow(in_v.w, in_y));
	}

	/** @return A vector containing the each element raised to the given power in y. */
	CX_FORCE_INLINE CxVec4 CxPow(const CxVec4& in_v, const CxVec4 &in_y) {
		return CxVec4(CxPow(in_v.x, in_y.x), CxPow(in_v.y, in_y.y), CxPow(in_v.z, in_y.z), CxPow(in_v.w, in_y.w));
	}

	/** @return A vector containing the one over the sqrt of each element (1/sqrt). */
	CX_FORCE_INLINE CxVec4 CxRecipSqrt(const CxVec4& in_v) {
		return CxVec4(CxRecipSqrt(in_v.x), CxRecipSqrt(in_v.y), CxRecipSqrt(in_v.z), CxRecipSqrt(in_v.w));
	}

	/** @return A vector containing the sign of each element (-1.0f if neg, 1.0f if pos). */
	CX_FORCE_INLINE CxVec4 CxSign(const CxVec4 &in_v) {
		return CxVec4(CxSign(in_v.x), CxSign(in_v.y), CxSign(in_v.z), CxSign(in_v.w));
	}
	
	/** @return A vector containing the sine value of each element. */
	CX_FORCE_INLINE CxVec4 CxSin(const CxVec4& in_v) {
		return CxVec4(CxSin(in_v.x), CxSin(in_v.y), CxSin(in_v.z), CxSin(in_v.w));
	}

	/** @return A vector containing the square root of each element. */
	CX_FORCE_INLINE CxVec4 CxSqrt(const CxVec4& in_v) {
		return CxVec4(CxSqrt(in_v.x), CxSqrt(in_v.y), CxSqrt(in_v.z), CxSqrt(in_v.w));
	}

	/** @return A vector containing the tangent value of each element. */
	CX_FORCE_INLINE CxVec4 CxTan(const CxVec4& in_v) {
		return CxVec4(CxTan(in_v.x), CxTan(in_v.y), CxTan(in_v.z), CxTan(in_v.w));
	}
} // namespace cat

#endif // CX_CORE_MATH_CXVEC4_H
