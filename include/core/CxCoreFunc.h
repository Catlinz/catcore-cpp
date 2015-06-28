#ifndef CX_CORE_CXCOREFUNC_H
#define CX_CORE_CXCOREFUNC_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxCoreFunc.h
 * @brief Contains core functions that are always included.
 *
 * @author Catlin Zilinski
 * @date May 29, 2015
 */

namespace cat {

	/**
	 * @brief Method to swap two values.
	 * @param inout_v1 The first value to swap.
	 * @param inout_v2 The second value to swap.
	 */
	template <typename T>
	CX_FORCE_INLINE void CxSwap(T &inout_v1, T &inout_v2) {
		T tmp = inout_v1;
		inout_v1 = inout_v2;
		inout_v2 = tmp;
	}
	
} // namespace cat

#endif // CX_CORE_CXCOREFUNC_H
