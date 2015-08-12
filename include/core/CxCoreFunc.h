#ifndef CX_CORE_CXCOREFUNC_H
#define CX_CORE_CXCOREFUNC_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxCoreFunc.h
 * @brief Contains core functions that are always included.
 *
 * @author Catlin Zilinski
 * @date Aug 12, 2015
 */

namespace cat {

	/**
	 * @brief Templated method to compare two objects with the '<' operator in ascending order.
	 * The objects must implement both the == operator and < operator.
	 * @param in_v1 The first value to compare
	 * @param in_v2 The second valud to compare v1 with.
	 * @return -1 if first < second, 0 if first == second and 1 if first > second.
	 */
	template<typename T>
	CX_INLINE CxI32 CxComparePtrAsc(const void *in_v1, const void *in_v2) {
		if (*(T *)in_v1 < *(T *)in_v2) { return -1; }
		else if (*(T *)in_v1 == *(T *)in_v2) { return 0; }
		else { return 1; }
}

	/**
	 * @brief Templated method to compare two objects with the '<' operator in descending order.
	 * The objects must implement both the == operator and < operator.
	 * @param in_v1 The first value to compare
	 * @param in_v2 The second valud to compare v1 with.
	 * @return 1 if first < second, 0 if first == second and -1 if first > second.
	 */
	template<typename T>
	CX_INLINE CxI32 CxComparePtrDesc(const void *in_v1, const void *in_v2) {
		if (*(T *)in_v1 < *(T *)in_v2) { return 1; }
		else if (*(T *)in_v1 == *(T *)in_v2) { return 0; }
		else { return -1; }
	}

	/**
	 * @brief Method to safetly delete a new'd array of objects.
	 * @param inout_array The array to delete.
	 */
	template<typename T>
	CX_FORCE_INLINE void CxDeleteArray(T*& inout_array) {
		if (inout_array != 0) {
			delete[] inout_array;
			inout_array = 0;
		}
	}

	/**
	 * @brief Method to swap two values.
	 * @param inout_v1 The first value to swap.
	 * @param inout_v2 The second value to swap.
	 */
	template<typename T>
	CX_FORCE_INLINE void CxSwap(T &inout_v1, T &inout_v2) {
		T tmp = inout_v1;
		inout_v1 = inout_v2;
		inout_v2 = tmp;
	}

} // namespace cat


#endif // CX_CORE_CXCOREFUNC_H
