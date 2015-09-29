#ifndef CX_CORE_COMMON_CXHASH_H
#define CX_CORE_COMMON_CXHASH_H

/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxHash.h
 * @brief Some simple hashing methods.
 *
 * @author Catlin Zilinski
 * @date May 9, 2015
 */

#include "core/Cx.h"

namespace cat {
	
	namespace hash {
		
		/**
		 * @brief Calculates the CRC32 hash of a string and returns the hash as a 32 bit integer.
		 * @todo Look into using intrinsics for the CRC32-C hashing.
		 * @param str The string to hash.
		 * @return A 32 bit unsigned integer represetning the CRC32 hash of the string.
		 */
		CxU32 crc32(const CxChar *in_str);
		
	} // namespace hash

	/**
	 * @brief Method to convert a string to a hash.
	 * @param in_str The string to hash.
	 * @return The crc32 hash of the string.
	 */
	CX_FORCE_INLINE CxU32 CxHash(const CxChar *in_str) {
		return hash::crc32(in_str);
	}

	/** @brief Methods to hash integer values.  Just converst to CxU32 */
	CX_FORCE_INLINE CxU32 CxHash(CxI32 in_val) { return (CxU32)in_val; }
	CX_FORCE_INLINE CxU32 CxHash(CxU32 in_val) { return in_val; }

	
} // namespace cat

#endif // CX_CORE_COMMON_CXHASH_H
