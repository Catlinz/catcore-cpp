#ifndef CX_CORE_MATH_WINDOWS_CXWINDOWSMATH_H
#define CX_CORE_MATH_WINDOWS_CXWINDOWSMATH_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxMath.h
 * @brief Contains the Windows specific implementations of math functions.
 *
 * @author Catlin Zilinski
 * @date June 1, 2015
 */

#include <intrin.h>

namespace cat {

	/**
	 * @brief Get the integer log base 2 of a number.
	 * If the input is zero, then the result is undefined.
	 * @param in_x The positive integer value to get the log base 2 of.
	 * @return The integer value of the log base 2 of x.
	 */
	CX_FORCE_INLINE CxI32 log2i(CxI32 in_x) {
	  unsigned long y = 0;
	  _BitScanReverse(&y, (unsigned long)in_x);
	  return (CxI32)y;
	}

} // namespace cat

#endif // CX_CORE_MATH_WINDOWS_CXWINDOWSMATH_H
