#ifndef CX_CORE_COMMON_CXSTR_H
#define CX_CORE_COMMON_CXSTR_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxStr.h
 * @brief Contains basic C-string creation and operation functions.
 *
 * @author Catlin Zilinski
 * @date May 17, 2015
 */

#include "core/common/CxMem.h"
#include <wchar.h>

namespace cat {

	namespace str {

		/**
		 * @brief Method to create a new string.
		 * @param in_len The length of the string (NOT incl. null-terminating char).
		 * @return A pointer to a newly allocated c-string.
		 */
		CX_FORCE_INLINE CxChar * alloc8(CxI32 in_len) {
			return (CxChar *)mem::alloc(sizeof(CxChar)*(in_len + 1));
		}

		/**
		 * @brief Method to create a new 16 bit unicode string.
		 * @param in_len The length of the string (NOT incl. null-terminating char).
		 * @return A pointer to a newly allocated 16 bit unicode c-string.
		 */
		CX_FORCE_INLINE CxChar16 * alloc16(CxI32 in_len) {
			return (CxChar16 *)mem::alloc(sizeof(CxChar16)*(in_len + 1));
		}

		/**
		 * @brief Method to get the length of a string.
		 * @param in_str The string to get the length of.
		 * @return The length of the string, not including null-term char.
		 */
		CX_FORCE_INLINE CxI32 len(const CxChar *CX_RESTRICT in_str) {
			return strlen(in_str);
		}

		/**
		 * @brief Method to get the length of a 16 bit string.
		 * @param in_str The 16 bit string to get the length of.
		 * @return The length of the string, not including null-term char.
		 */
		CX_FORCE_INLINE CxI32 len(const CxChar16 *CX_RESTRICT in_str) {
			return wcslen((const wchar_t*)in_str);
		}

		/**
		 * @brief Method to append chars of one string to another string.
		 * @param inout_str The string to append to.
		 * @param in_end The string to append to the other string.
		 * @param in_len The number of characters to use from the string.
		 * @return A pointer to inout_str is returned.
		 */
		CxChar * concat(CxChar *CX_RESTRICT inout_str,
							 const CxChar *CX_RESTRICT in_str,
							 CxI32 in_len);

		/**
		 * @brief Method to append chars of one 16 bit string to another.
		 * @param inout_str The 16 bit string to append to.
		 * @param in_end The 16 bit string to append to the other string.
		 * @param in_len The number of characters to use from the string.
		 * @return A pointer to 16 bit inout_str is returned.
		 */
		CxChar16 * concat(CxChar16 *CX_RESTRICT inout_str,
								const CxChar16 *CX_RESTRICT in_str,
								CxI32 in_len);

		/**
		 * @brief Method to append one 16 bit string to another.
		 * @param inout_str The string to append to.
		 * @param in_end The string to append to the other string.
		 * @return A pointer to inout_str is returned.
		 */
		CX_FORCE_INLINE CxChar * concat(CxChar *CX_RESTRICT inout_str,
												  const CxChar *CX_RESTRICT in_str) {
			return str::concat(inout_str, in_str, str::len(in_str));
		}

		/**
		 * @brief Method to append one string to another string.
		 * @param inout_str The string to append to.
		 * @param in_end The string to append to the other string.
		 * @return A pointer to inout_str is returned.
		 */
		CX_FORCE_INLINE CxChar16 * concat(CxChar16 *CX_RESTRICT inout_str,
													 const CxChar16 *CX_RESTRICT in_str) {
			return str::concat(inout_str, in_str, str::len(in_str));
		}
		
		/**
		 * @brief Method to copy an entire string (incl null term) into another string.
		 * @param inout_str The string to copy into.
		 * @param in_str The string to copy.
		 * @param in_len The length of the string to copy.
		 * @return A pointer to out_str is returned.
		 */
		CX_FORCE_INLINE CxChar * copy(CxChar *CX_RESTRICT inout_str,
												const CxChar *CX_RESTRICT in_str,
												CxI32 in_len) {
			return (CxChar *)mem::copy(inout_str, in_str, sizeof(CxChar)*(in_len + 1));
		}

		/**
		 * @brief Method to copy a 16 bit string into another string.
		 * @param inout_str The 16 bit string to copy into.
		 * @param in_str The 16 bit string to copy.
		 * @param in_len The length of the string to copy.
		 * @return A pointer to out_str is returned.
		 */
		CX_FORCE_INLINE CxChar16 * copy(CxChar16 *CX_RESTRICT inout_str,
												  const CxChar16 *CX_RESTRICT in_str,
												  CxI32 in_len) {
			return (CxChar16 *)mem::copy(inout_str, in_str, sizeof(CxChar16)*(in_len + 1));
		}
		
		/**
		 * @brief Method to create a new copy of a string.
		 * @param in_str The string to copy.
		 * @param in_len The length of the string to copy.
		 * @return A new string that is the copy of the original.
		 */
		CX_FORCE_INLINE CxChar * copy(const CxChar *CX_RESTRICT in_str, CxI32 in_len) {
			return str::copy(str::alloc8(in_len), in_str, in_len);
		}

		/**
		 * @brief Method to create a new copy of a 16 bit string.
		 * @param in_str The 16 bit string to copy.
		 * @param in_len The length of the string to copy.
		 * @return A new 16 bit string that is the copy of the original.
		 */
		CX_FORCE_INLINE CxChar16 * copy(const CxChar16 *CX_RESTRICT in_str, CxI32 in_len) {
			return str::copy(str::alloc16(in_len), in_str, in_len);
		}

		/**
		 * @brief Method to create a new copy of a string.
		 * @param in_str The string to copy.
		 * @return A new string that is the copy of the original, or null if original is null.
		 */
		CX_FORCE_INLINE CxChar * copy(const CxChar *CX_RESTRICT in_str) {
			if (in_str != 0) {
				return str::copy(in_str, str::len(in_str));
			}
			else { return 0; }
		}

		/**
		 * @brief Method to create a new copy of a 16 bit string.
		 * @param in_str The 16 bit string to copy.
		 * @return A new 16 bit string that is the copy of the original, or null if original is null.
		 */
		CX_FORCE_INLINE CxChar16 * copy(const CxChar16 *CX_RESTRICT in_str) {
			if (in_str != 0) {
				return str::copy(in_str, str::len(in_str));
			}
			else { return 0; }
		}

		/**
		 * @brief Test if two strings are equal or not.
		 * @param in_str1 The first string to compare.
		 * @param in_str2 The second string to compare.
		 * @return True if the strings are equal.
		 */
		CxBool eq(const CxChar *CX_RESTRICT in_str1,
					 const CxChar *CX_RESTRICT in_str2);

		/**
		 * @brief Test if a 8 bit string and 16 bit string are equal.
		 * @param in_str1 The 16 bit string to compare.
		 * @param in_str2 The 8 bit string to compare.
		 * @return True if the strings are equal.
		 */
		CxBool eq(const CxChar16 *CX_RESTRICT in_str16,
					 const CxChar *CX_RESTRICT in_str8);

		/**
		 * @brief Test if two 16 bit strings are equal or not.
		 * @param in_str1 The first 16 bit string to compare.
		 * @param in_str2 The second 16 bit string to compare.
		 * @return True if the strings are equal.
		 */
		CxBool eq(const CxChar16 *CX_RESTRICT in_str1,
					 const CxChar16 *CX_RESTRICT in_str2);

		/**
		 * @brief Test if the first n characters of two strings are equal or not.
		 * Assumes that the strings are at least 4 or 8 byte aligned.
		 * @param in_str1 The first string to compare.
		 * @param in_str2 The second string to compare.
		 * @param in_len The length of the strings to test.
		 * @return True if the strings are equal.
		 */
		CxBool eq(const CxChar *CX_RESTRICT in_str1,
					 const CxChar *CX_RESTRICT in_str2, CxI32 in_len);

		/**
		 * @brief Method to free a string and set the pointer to 0.
		 * The method checks for null strings first.
		 * @param inout_str A reference to the pointer to the string.
		 */
		CX_FORCE_INLINE void free(CxChar *&inout_str) {
			if (inout_str != 0) { ::free(inout_str); inout_str = 0; }
		}

		/**
		 * @brief Method to free a 16 bit string and set the pointer to 0.
		 * The method checks for null strings first.
		 * @param inout_str A reference to the pointer to the string.
		 */
		CX_FORCE_INLINE void free(CxChar16 *&inout_str) {
		   if (inout_str != 0) { ::free(inout_str); inout_str = 0; }
		}
		
	} // namespace str
	
} // namespace cat


#endif // CX_CORE_COMMON_CXSTR_H
