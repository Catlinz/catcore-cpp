#ifndef CX_CORE_MATH_CXMATH_H
#define CX_CORE_MATH_CXMATH_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxMath.h
 * @brief Contains the core math functions.
 *
 * @author Catlin Zilinski
 * @date May 29, 2015
 */

#include <math.h>
#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsMath.h"
#elif defined(CX_GNUC)
#  include "gnuc/CxGNUCMath.h"
#else
#  include "xplt/CxXPltMath.h"
#endif

namespace cat {
	
	/* Some useful math constants */
	const CxReal kCxPi				= (CxReal)3.1415926535897932;
	const CxReal kCxTwoPi			= (CxReal)6.2831853071795865;
	const CxReal kCxHalfPi			= (CxReal)1.5707963267948966;
	const CxReal kCxQuarterPi		= (CxReal)0.7853981633974483;
	const CxReal kCxRecipPi			= (CxReal)0.3183098861837907;
	const CxReal kCxRecipTwoPi		= (CxReal)0.1591549430918953;
		

	const CxReal kCxSqrtTwo			= (CxReal)1.4142135623730950;
	const CxReal kCxTwoSqrtTwo		= (CxReal)2.8284271247461901;
	const CxReal kCxHalfSqrtTwo	= (CxReal)0.7071067811865475;
	const CxReal kCxRecipSqrtTwo	= (CxReal)0.7071067811865475;

	const CxReal kCxDegToRad		= (CxReal)0.0174532925199433;
	const CxReal kCxRadToDeg		= (CxReal)57.295779513082321;

	/* Enum for initialising mathematical objecst to zero */
	enum CxZero { kCxZero = 0 };
	/* Enum for intialiseing mathematical objects to an identity value */
	enum CxIdentity { kCxIdentity = 1 };

	/** @return The absolute value of the argument. */
	CX_FORCE_INLINE CxF32 CxAbs(CxF32 in_val) { return ::fabs(in_val); }
	/** @return The absolute value of the argument. */
	CX_FORCE_INLINE CxF64 CxAbs(CxF64 in_val) { return ::fabs(in_val); }
	/** @return The absolute value of the argument. */
	CX_FORCE_INLINE CxI32 CxAbs(CxI32 in_val) { return ::abs(a); }

	/** @return The angle of the cosine value, in radians, between [0, PI]. */
	CX_FORCE_INLINE CxF32 CxAcos(CxF32 in_cos) { return ::acosf(in_cos); }
	/** @return The angle of the cosine value, in radians, between [0, PI]. */
	CX_FORCE_INLINE CxF64 CxAcos(CxF64 in_cos) { return ::acos(in_cos); }

	/** @return The angle of the sine value, in radians, between [-PI/2, PI/2]. */
	CX_FORCE_INLINE CxF32 CxAsin(CxF32 in_sin) { return ::asinf(in_sin); }
	/** @return The angle of the sine value, in radians, between [-PI/2, PI/2]. */
	CX_FORCE_INLINE CxF64 CxAsin(CxF64 in_sin) { return ::asin(in_sin); }

	/** @return The angle of the tangent value, in radians, between [-PI/2, PI/2]. */
	CX_FORCE_INLINE CxF32 CxAtan(CxF32 in_tan) { return ::atanf(in_tan); }
	/** @return The angle of the tangent value, in radians, between [-PI/2, PI/2]. */
	CX_FORCE_INLINE CxF64 CxAtan(CxF64 in_tan) { return ::atan(in_tan); }

	/** @return The arctangent of (x/y), in radians, between [-PI/2, PI/2]. */
	CX_FORCE_INLINE CxF32 CxAtan2(CxF32 in_x, CxF32 in_y) { return ::atan2f(in_x, in_y); }
	/** @return The arctangent of (x/y), in radians, between [-PI/2, PI/2]. */
	CX_FORCE_INLINE CxF64 CxAtan2(CxF64 in_x, CxF64 in_y) { return ::atan2(in_x, in_y); }

	/** @return The next largest whole number (or the number if already whole). */
	CX_FORCE_INLINE CxF32 CxCeil(CxF32 in_val) { return ::ceilf(in_val); }
	/** @return The next largest whole number (or the number if already whole). */
	CX_FORCE_INLINE CxF64 CxCeil(CxF64 in_val) { return ::ceil(in_val); }

	/**
	 * @brief Makes sure the input value is in the range [min,max].
	 * @param inout_val The value to clamp.
	 * @param in_min The lowest value to clamp to.
	 * @param in_max The highest value to clamp to.
	 * @return A reference to the clamped value.
	 */
	template <typename T>
	CX_FORCE_INLINE T& void CxClamp(T &inout_val, T in_min, T in_max) {
		inout_val = CxMin(in_max, CxMax(inout_val, in_min)); return inout_val;
	}

	/** @return The input value clamped to the range [min,max]. */
	template <typename T>
	CX_FORCE_INLINE T CxClamped(T in_val, T in_min, T in_max) {
		return CxMin(in_max, CxMax(in_val, in_min));
	}

	/** @return The cosine of the angle (specified in radians) */
	CX_FORCE_INLINE CxF32 CxCos(CxF32 in_rad) { return ::cosf(in_rad); }
	/** @return The cosine of the angle (specified in radians) */
	CX_FORCE_INLINE CxF64 CxCos(CxF64 in_rad) { return ::cos(in_rad); }


	/** @return True if a and b are within epsilon of each other (approx. equal) */
	CX_FORCE_INLINE CxBool CxEq(CxF32 in_a, CxF32 in_b, CxF32 in_epsilon = CX_REAL_EPSILON) {
		return CxAbs(in_a - in_b) < in_epsilon;
	}

	/** @return True if a and b are within epsilon of each other (approx. equal) */
	CX_FORCE_INLINE CxBool CxEq(CxF64 in_a, CxF64 in_b, CxF64 in_epsilon = CX_REAL_EPSILON) {
		return CxAbs(in_a - in_b) < in_epsilon;
	}

	/** @return The value of e raised to the power of x (e^x). */
	CX_FORCE_INLINE CxF32 CxExp(CxF32 in_x) { return ::expf(in_x); }
	/** @return The value of e raised to the power of x (e^x). */
	CX_FORCE_INLINE CxF64 CxExp(CxF64 in_x) { return ::exp(in_x); }

	/** @return The closest whole number that is equal to or less than val. */
	CX_FORCE_INLINE CxF32 CxFloor(CxF32 in_val) { return ::floorf(in_val); }
	/** @return The closest whole number that is equal to or less than val. */
	CX_FORCE_INLINE CxF64 CxFloor(CxF64 in_val) { return ::floor(in_val); }

	/** @return True if the passed in value is a finite floating point number */
	CX_FORCE_INLINE CxBool CxIsFinite(CxF32 in_val) { return ::isFinite(in_val); }
	/** @return True if the passed in value is a finite floating point number */
	CX_FORCE_INLINE CxBool CxIsFinite(CxF64 in_val) { return ::isFinite(in_val); }

	/** @return The value of log_b(x) (log base b of x). */
	CX_FORCE_INLINE CxF32 CxLog(CxF32 in_b, CxF32 in_x) { return ::log(in_x) / ::log(in_b); }
	/** @return The value of log_b(x) (log base b of x). */
	CX_FORCE_INLINE CxF64 CxLog(CxF64 in_b, CxF64 in_x) { return ::log(in_x) / ::log(in_b); }

	/** @return The natural logarithm (log base e) of x. */
	CX_FORCE_INLINE CxF32 CxLogE(CxF32 in_x) {	return ::log(in_x); }
	/** @return The natural logarithm (log base e) of x. */
	CX_FORCE_INLINE CxF64 CxLogE(CxF64 in_x) {	return ::log(in_x); }

	/** @return The log base 10 of x. */
	CX_FORCE_INLINE CxF32 CxLog10(CxF32 in_x) { return ::log10(in_x); }
	/** @return The log base 10 of x. */
	CX_FORCE_INLINE CxF64 CxLog10(CxF64 in_x) { return ::log10(in_x); }

	/**
	 * @brief Get the integer log base 2 of a number.
	 * @param in_x The number to get the log base 2 of.
	 * @return The integer value of the log base 2 of x.
	 */
	CX_FORCE_INLINE I32 log2i(CxF32 in_x) {
		CxI32 y = 0;
		while (x > 1) { x /= 2; y++; }
		return y;
	}

	/** @return The maximum value of a and b. */
	template <typename T>
	CX_FORCE_INLINE T CxMax(T in_a, T in_b) { return (in_a < in_b) ? in_b : in_a; }

	/** @return The minimum value of a and b. */
	template <typename T>
	CX_FORCE_INLINE T CxMin(T in_a, T in_b) { return (in_a < in_b) ? in_a : in_b; }

	/** @return The value of x^y (x raised to the power of y). */
	CX_FORCE_INLINE CxF32 CxPow(CxF32 in_x, CxF32 in_y) { return ::powf(in_x, in_y); }
	/** @return The value of x^y (x raised to the power of y). */
	CX_FORCE_INLINE CxF64 CxPow(CxF64 in_x, CxF64 in_y) { return ::pow(in_x, in_y); }

	/** @return The reciprocal square root (1/sqrt) of the input value. */
	CX_FORCE_INLINE CxF32 CxRecipSqrt(CxF32 in_val) { return 1.0f / ::sqrtf(in_val); }
	/** @return The reciprocal square root (1/sqrt) of the input value. */
	CX_FORCE_INLINE CxF64 CxRecipSqrt(CxF64 in_val) { return 1.0 / ::sqrt(in_val); }

	/** @return -1.0 if the input is negative, otherwise 1.0. */
	CX_FORCE_INLINE CxF32 CxSign(CxF32 in_val) { return (in_val < 0.0f) ? -1.0f : 1.0f; }
	/** @return -1.0 if the input is negative, otherwise 1.0. */
	CX_FORCE_INLINE CxF32 CxSign(CxF64 in_val) { return (in_val < 0.0) ? -1.0 : 1.0; }

	/** @return -1 if the input is negative, otherwise 1. */
	CX_FORCE_INLINE CxI32 CxSign(CxI32 in_val) { return (in_val < 0) ? -1 : 1; }

	/** @return The sine of the angle (specified in radians) */
	CX_FORCE_INLINE CxF32 CxSin(CxF32 in_rad) { return ::sinf(in_rad); }
	/** @return The sine of the angle (specified in radians) */
	CX_FORCE_INLINE CxF64 CxSin(CxF64 in_rad) { return ::sin(in_rad); }

	/** @return The square root of the input value. */
	CX_FORCE_INLINE CxF32 CxSqrt(CxF32 in_val) { return ::sqrtf(in_val); }
	/** @return The square root of the input value. */
	CX_FORCE_INLINE CxF64 CxSqrt(CxF64 in_val) { return ::sqrt(in_val); }


	/** @return The tangent of the angle (specified in radians) */
	CX_FORCE_INLINE CxF32 CxTan(CxF32 in_rad) { return ::tanf(in_rad); }
	/** @return The tangent of the angle (specified in radians) */
	CX_FORCE_INLINE CxF64 CxTan(CxF64 in_rad) { return ::tan(in_rad); }

} // namespace cat

#endif // CX_CORE_MATH_CXMATH_H
