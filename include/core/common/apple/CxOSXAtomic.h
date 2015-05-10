#ifndef CX_CORE_COMMON_APPLE_CXOSXATOMIC_H
#define CX_CORE_COMMON_APPLE_CXOSXATOMIC_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxOSXAtomic.h
 * @brief Contains various atomic operations such as increment and decrement.
 *
 * @author Catlin Zilinski
 * @date May 10, 2015
 */

#include "core/Cx.h"
#include <libkern/OSAtomic.h>

namespace cat {
	namespace atomic {
	   /**
		 * @brief Adds the specified 32 bit value to the addend.
		 * @param inout_addend The 32 bit value to add to.
		 * @param in_value The 32 bit value to add.
		 * @return The resulting 32 bit value.
		 */
		CX_FORCE_INLINE CxI32 add32(CxI32 &inout_addend, CxI32 in_value) {
			return OSAtomicAdd32(in_value, &inout_addend);
		}

		/**
		 * @brief Adds the specified 64 bit value to the addend.
		 * @param inout_addend The 64 bit value to add to.
		 * @param in_value The 64 bit value to add.
		 * @return The resulting 64 bit value.
		 */
		CX_FORCE_INLINE CxI64 add64(CxI64 &inout_addend, CxI64 in_value) {
			return OSAtomicAdd64(in_value, &inout_addend);
		}

		/**
		 * @brief And's a 32 bit value with the 32 bit destination value.
		 * @param inout_dest The value to and with.  Will hold the result.
		 * @param in_mask The value to and.
		 * @return The result of the and.
		 */
		CX_FORCE_INLINE CxI32 and32(CxI32 &inout_dest, CxI32 in_mask) {
			return OSAtomicAnd32(in_mask, &inout_dest);
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
			return OSAtomicCompareAndSwap32(in_old, in_new, &inout_val);
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
			return OSAtomicCompareAndSwap64(in_old, in_new, &inout_val);
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
		   return OSAtomicCompareAndSwapPtr(in_old, in_new, &inout_val);
		}
		
		/**
		 * @brief Decrement the 32 bit value by one.
		 * @param inout_val The value to decrement by one.
		 * @return The decremented value.
		 */
		CX_FORCE_INLINE CxI32 decr32(CxI32 &inout_val) {
			return OSAtomicDecrement32(&inout_val);
		}

		/**
		 * @brief Decrement the 64 bit value by one.
		 * @param inout_val The value to decrement by one.
		 * @return The decremented value.
		 */
		CX_FORCE_INLINE CxI32 decr64(CxI64 &inout_val) {
			return OSAtomicDecrement64(&inout_val);
		}
		
		/**
		 * @brief Increment the 32 bit value by one.
		 * @param inout_val The value to increment by one.
		 * @return The incremented value.
		 */
		CX_FORCE_INLINE CxI32 incr32(CxI32 &inout_val) {
			return OSAtomicIncrement32(&inout_val);
		}
		
		/**
		 * @brief Increment the 64 bit value by one.
		 * @param inout_val The value to increment by one.
		 * @return The incremented value.
		 */
		CX_FORCE_INLINE CxI32 incr64(CxI64 &inout_val) {
			return OSAtomicIncrement64(&inout_val);
		}

		/**
		 * @brief OR's a 32 bit value with the 32 bit destination value.
		 * @param inout_dest The value to OR with.  Will hold the result.
		 * @param in_mask The value to OR.
		 * @return The result of the OR.
		 */
		CX_FORCE_INLINE CxI32 or32(CxI32 &inout_dest, CxI32 in_mask) {
			return OSAtomicOr32(in_mask, &inout_dest);
		}

		/**
		 * @brief XOR's a 32 bit value with the 32 bit destination value.
		 * @param inout_dest The value to XOR with.  Will hold the result.
		 * @param in_mask The value to XOR.
		 * @return The result of the XOR.
		 */
		CX_FORCE_INLINE CxI32 xor32(CxI32 &inout_dest, CxI32 in_mask) {
			return OSAtomicXor32(in_mask, &inout_dest);
		}
	} // namespace atomic
} // namespace cat

#endif // CX_CORE_COMMON_APPLE_CXOSXATOMIC_H
