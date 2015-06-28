#ifndef CX_CORE_MATH_XPLT_CXXPLTMATH_H
#define CX_CORE_MATH_XPLT_CXXPLTMATH_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxMath.h
 * @brief Contains the fallback implementations of math functions.
 *
 * @author Catlin Zilinski
 * @date June 1, 2015
 */

namespace cat {

	/**
	 * @brief Get the integer log base 2 of a number.
	 * If the input is zero, then the result is undefined.
	 * @param in_x The positive integer value to get the log base 2 of.
	 * @return The integer value of the log base 2 of x.
	 */
	CX_FORCE_INLINE I32 log2i(CxI32 in_x) {
	  CxI32 y = 0;
	  while (x > 1) { x >> 1; y++; }
	  return y;
	}

} // namespace cat

#endif // CX_CORE_MATH_XPLT_CXXPLTMATH_H
