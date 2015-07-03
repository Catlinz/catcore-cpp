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
		 * @param str The string to hash.
		 * @return A 32 bit unsigned integer represetning the CRC32 hash of the string.
		 */
		CxU32 crc32(const I8 *in_str);
		
	} // namespace hash
	
} // namespace cat

#endif // CX_CORE_COMMON_CXHASH_H
