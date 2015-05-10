#ifndef CX_CORE_COMMON_WINDOWS_CXWINDOWSATOMIC_H
#define CX_CORE_COMMON_WINDOWS_CXWINDOWSATOMIC_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxWindowsAtomic.h
 * @brief Contains various atomic operations such as increment and decrement.
 *
 * @author Catlin Zilinski
 * @date May 10, 2015
 */

#include "core/Cx.h"
#include <Windows.h>

namespace cat {
	namespace atomic {
		/**
		 * @brief Adds the specified 32 bit value to the addend.
		 * @param inout_addend The 32 bit value to add to.
		 * @param in_value The 32 bit value to add.
		 * @return The resulting 32 bit value.
		 */
		CX_FORCE_INLINE CxI32 add32(CxI32 &inout_addend, CxI32 in_value) {
			return InterlockedAdd(&inout_addend, in_value);
		}

		/**
		 * @brief Adds the specified 64 bit value to the addend.
		 * @param inout_addend The 64 bit value to add to.
		 * @param in_value The 64 bit value to add.
		 * @return The resulting 64 bit value.
		 */
		CX_FORCE_INLINE CxI64 add64(CxI64 &inout_addend, CxI64 in_value) {
			return InterlockedAdd64(&inout_addend, in_value);
		}

		/**
		 * @brief And's a 32 bit value with the 32 bit destination value.
		 * @param inout_dest The value to and with.  Will hold the result.
		 * @param in_mask The value to and.
		 * @return The result of the and.
		 */
		CX_FORCE_INLINE CxI32 and32(CxI32 &inout_dest, CxI32 in_mask) {
			return InterlockedAnd(&inout_dest, in_mask);
		}

		/**
		 * @brief Sets to a new value if equal to an old value (32 bit).
		 * This method compares the value to the old value, and if they are 
		 * equal, sets the value to the new value.
		 * @param inout_val The value to compare and set.
		 * @param in_old The value to compare to.
		 * @param in_new The new value to set to if equal to old value.
		 * @return True if the value was changed (i.e., value == old value).
		 */
		CX_FORCE_INLINE CxBool compareAndSet32(CxI32 &inout_val, CxI32 in_old, CxI32 in_new) {
			return InterlockedCompareExchange(&inout_val, in_new, in_old) == in_old;
		}

		/**
		 * @brief Sets to a new value if equal to an old value (64 bit).
		 * @see compareAndSet32()
		 * @param inout_val The value to compare and set.
		 * @param in_old The value to compare to.
		 * @param in_new The new value to set to if equal to old value.
		 * @return True if the value was changed (i.e., value == old value).
		 */
		CX_FORCE_INLINE CxBool compareAndSet64(CxI64 &inout_val, CxI64 in_old, CxI64 in_new) {
			return InterlockedCompareExchange64(&inout_val, in_new, in_old) == in_old;
		}

		/**
		 * @brief Sets to a new value if equal to an old value (Pointer).
		 * @see compareAndSet32()
		 * @param inout_val The value to compare and set.
		 * @param in_old The value to compare to.
		 * @param in_new The new value to set to if equal to old value.
		 * @return True if the value was changed (i.e., value == old value).
		 */
		CX_FORCE_INLINE CxBool compareAndSetPtr(void *&inout_val, void *in_old, void *in_new) {
			return InterlockedCompareExchangePointer(&inout_val, in_new, in_old) == in_old;
		}
		
		/**
		 * @brief Decrement the 32 bit value by one.
		 * @param inout_val The value to decrement by one.
		 * @return The decremented value.
		 */
		CX_FORCE_INLINE CxI32 decr32(CxI32 &inout_val) {
			return InterlockedDecrement(&inout_val);
		}

		/**
		 * @brief Decrement the 64 bit value by one.
		 * @param inout_val The value to decrement by one.
		 * @return The decremented value.
		 */
		CX_FORCE_INLINE CxI32 decr64(CxI64 &inout_val) {
			return InterlockedDecrement64(&inout_val);
		}
		
		/**
		 * @brief Increment the 32 bit value by one.
		 * @param inout_val The value to increment by one.
		 * @return The incremented value.
		 */
		CX_FORCE_INLINE CxI32 incr32(CxI32 &inout_val) {
			return InterlockedIncrement(&inout_val);
		}

		/**
		 * @brief Increment the 64 bit value by one.
		 * @param inout_val The value to increment by one.
		 * @return The incremented value.
		 */
		CX_FORCE_INLINE CxI32 incr64(CxI64 &inout_val) {
			return InterlockedIncrement64(&inout_val);
		}

		/**
		 * @brief OR's a 32 bit value with the 32 bit destination value.
		 * @param inout_dest The value to OR with.  Will hold the result.
		 * @param in_mask The value to OR.
		 * @return The result of the OR.
		 */
		CX_FORCE_INLINE CxI32 or32(CxI32 &inout_dest, CxI32 in_mask) {
			return InterlockedOr(&inout_dest, in_mask);
		}

		/**
		 * @brief XOR's a 32 bit value with the 32 bit destination value.
		 * @param inout_dest The value to XOR with.  Will hold the result.
		 * @param in_mask The value to XOR.
		 * @return The result of the XOR.
		 */
		CX_FORCE_INLINE CxI32 xor32(CxI32 &inout_dest, CxI32 in_mask) {
			return InterlockedXor(&inout_dest, in_mask);
		}
		
	} // namespace atomic
} // namespace cat

#endif // CX_CORE_COMMON_WINDOWS_CXWINDOWSATOMIC_H
