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

namespace cat {

	namespace str {

		/**
		 * @brief Method to create a new string.
		 * @param in_len The length of the string (NOT incl. null-terminating char).
		 * @return A pointer to a newly allocated c-string.
		 */
		CX_FORCE_INLINE CxChar * alloc(CxI32 in_len) {
			return (CxChar *)mem::alloc(sizeof(CxChar)*(in_len + 1));
		}

		/**
		 * @brief Method to create a new zeroed string.
		 * @param in_len The length of the string (NOT incl. null-terminating char).
		 * @return A pointer to a newly allocated zeroed c-string.
		 */
		CX_FORCE_INLINE CxChar * allocZero(CxI32 in_len) {
			return (CxChar *)mem::allocZero(sizeof(CxChar), in_len + 1);
		}

		/**
		 * @brief Method to append chars of one string to another string.
		 * @param inout_str The string to append to.
		 * @param in_destSize The size of the destination string.
		 * @param in_end The string to append to the other string.
		 * @param in_len The number of characters to use from the string.
		 * @return A pointer to inout_str is returned.
		 */
		CX_FORCE_INLINE CxChar * append(CxChar *CX_RESTRICT inout_str, CxI32 in_destSize,
												  const CxChar *CX_RESTRICT in_str, CxI32 in_len) {
#if defined(CX_WINDOWS)
			strncat_s(inout_str, in_destSize, in_str, in_len);  return inout_str;
#else
			return strncat(inout_str, in_str, in_len);
#endif
		}

		/**
		 * @brief Method to append one 16 bit string to another.
		 * @param inout_str The string to append to.
		 * @param in_destSize The size of the destination string.
		 * @param in_end The string to append to the other string.
		 * @return A pointer to inout_str is returned.
		 */
		CX_FORCE_INLINE CxChar * append(CxChar *CX_RESTRICT inout_str, CxI32 in_destSize,
												  const CxChar *CX_RESTRICT in_str) {
#if defined(CX_WINDOWS)
		   strcat_s(inout_str, in_destSize, in_str);  return inout_str;
#else
			return strcat(inout_str, in_str);
#endif
		}

		/**
		 * @brief Method to get the length of a string.
		 * @param in_str The string to get the length of.
		 * @return The length of the string, not including null-term char.
		 */
		CX_FORCE_INLINE CxI32 len(const CxChar *CX_RESTRICT in_str) {
			return (in_str != 0) ? strlen(in_str) : 0;
		}

		/**
		 * @brief Method to copy an entire string (incl null term) into another string.
		 * @param inout_str The string to copy into.
		 * @param in_dstSize The amount of room in the destination string.
		 * @param in_str The string to copy.
		 * @return A pointer to out_str is returned.
		 */
		CX_FORCE_INLINE CxChar * copy(CxChar *CX_RESTRICT inout_str, CxI32 in_dstSize,
												const CxChar *CX_RESTRICT in_str) {
#if defined(CX_WINDOWS)
			strcpy_s(inout_str, in_dstSize, in_str);  return inout_str;
#else
			return strcpy(inout_str, in_str);
#endif
		}
		
		/**
		 * @brief Method to copy an entire string (incl null term) into another string.
		 * Tries to copy N bytes (where N is the lesser of in_bytes and the source string length)
		 * into the destination.  If in_bytes >= in_destBytes, then we set inout_dest[0] = 0 and
		 * just return it.  A null-terminating character is always appended at the position 
		 * inout_dest[in_bytes].
		 * @param inout_dest The string to copy into.
		 * @param in_destSize The amount of room in the destination string (incl. null term).
		 * @param in_src The string to copy.
		 * @param in_len The number of characters to copy from the src.
		 * @return A pointer to the destination string is returned.
		 */
		CX_FORCE_INLINE CxChar * copy(CxChar *CX_RESTRICT inout_dest, CxI32 in_destSize,
												const CxChar *CX_RESTRICT in_src, CxI32 in_len) {
			if (in_destSize > in_len) {
#if defined(CX_WINDOWS)
				strncpy_s(inout_dest, in_destSize, in_src, in_len);
#else
				strncpy(inout_dest, in_src, in_len);
				inout_dest[in_len] = 0;
#endif
			} else { inout_dest[0] = 0; }
			return inout_dest;
		}
		
		/**
		 * @brief Method to create a new copy of a string.
		 * If the length is less than the length of the string, a null char 
		 * will still be appended at str[in_len].
		 * @param in_str The string to copy.
2		 * @param in_len The length of the string to copy.
		 * @return A new string that is the copy of the original.
		 */
		CX_FORCE_INLINE CxChar * copy(const CxChar *CX_RESTRICT in_str, CxI32 in_len) {
			return str::copy(str::alloc(in_len), in_len + 1, in_str, in_len);
		}

		/**
		 * @brief Method to create a new copy of a string.
		 * @param in_str The string to copy.
		 * @return A new string that is the copy of the original, or null if original is null.
		 */
		CX_FORCE_INLINE CxChar * copy(const CxChar *CX_RESTRICT in_str) {
			if (in_str != 0) {
				const CxI32 len = str::len(in_str);
				return str::copy(str::alloc(len), len + 1, in_str, len);
			}
			else { return 0; }
		}

		/**
		 * @brief Test if one string is the end of another string.
		 * If either string is null, returns false.  If equal, returns true.
		 * @param in_str The string to test.
		 * @param in_end The end string to test for.
		 * @return True if in_str starts with in_end.
		 */
		CxBool endsWith(const CxChar *CX_RESTRICT in_str, const CxChar *CX_RESTRICT in_end);

		/**
		 * @brief Test if two strings are equal or not.
		 * @param in_str1 The first string to compare.
		 * @param in_str2 The second string to compare.
		 * @return True if the strings are equal.
		 */
		CxBool eq(const CxChar *CX_RESTRICT in_str1, const CxChar *CX_RESTRICT in_str2);

		/**
		 * @brief Test if the first n characters of two strings are equal or not.
		 * Assumes that the strings are at least 4 or 8 byte aligned.
		 * @param in_str1 The first string to compare.
		 * @param in_str2 The second string to compare.
		 * @param in_len The length of the strings to test.
		 * @return True if the strings are equal.
		 */
		CxBool eq(const CxChar *CX_RESTRICT in_str1, const CxChar *CX_RESTRICT in_str2,
					 CxI32 in_len);

		/**
		 * @brief Method to write a formated string to the specified buffer.
		 * @see sprintf() for details.
		 * @param inout_str The string to store the formatted string into.
		 * @param in_dstSize The length of the buffer to store the string in.
		 * @param in_format The format of the string to store.
		 * @param ... additional arguments to include in the formatted string.
		 * @return The total number of characters written.
		 */
		CxI32 format(CxChar *inout_str, CxI32 in_dstSize, const CxChar *CX_RESTRICT in_format, ...);
		
		/**
		 * @brief Method to free a string and set the pointer to 0.
		 * The method checks for null strings first.
		 * @param inout_str A reference to the pointer to the string.
		 */
		CX_FORCE_INLINE void free(CxChar *&inout_str) {
			if (inout_str != 0) { ::free(inout_str); inout_str = 0; }
		}
		
		/**
		 * @brief Test to see if str1 > str2.
		 * The test returns true if the value of the first nonmatching character 
		 * of str1 is greater than that of str2.
		 * @param in_str1 The first string to compare.
		 * @param in_str2 The second string to compare with.
		 * @return True if str1 is greater than str2.
		 */
		CX_FORCE_INLINE CxBool greater(const CxChar *CX_RESTRICT in_str1,
												 const CxChar *CX_RESTRICT in_str2) {
			if (in_str1 == 0 || in_str2 == 0) { return in_str1 > in_str2;  }
			return ::strcmp(in_str1, in_str2) > 0;
		}

		/**
		 * @brief Test to see if str1 < str2.
		 * The test returns true if the value of the first nonmatching character 
		 * of str1 is less than that of str2.
		 * @param in_str1 The first string to compare.
		 * @param in_str2 The second string to compare with.
		 * @return True if str1 is less than str2.
		 */
		CX_FORCE_INLINE CxBool less(const CxChar *CX_RESTRICT in_str1,
											 const CxChar *CX_RESTRICT in_str2) {
			if (in_str1 == 0 || in_str2 == 0) { return in_str1 < in_str2;  }
			return ::strcmp(in_str1, in_str2) < 0;
		}

		/**
		 * @brief Read an integer from a string and advance the pointer.
		 * This method reads an integer value from a string and advances
		 * the string pointer to the next character after the integer value read.
		 * @param inout_str The string to read the value from and increment.
		 * @param inopt_base The optional base of the integer value (default = autodetect)
		 * @return The integer value read or 0 if cannot convert.
		 */
		CX_FORCE_INLINE CxI32 readI32(CxChar *&inout_str, CxI32 inopt_base = 0) {
			return (inout_str != 0) ? (CxI32)strtol(inout_str, &inout_str, inopt_base) : 0;
		}

		/** @see readI32(CxChar *&, CxI32) */
		CX_FORCE_INLINE CxI64 readI64(CxChar *&inout_str, CxI32 inopt_base = 0) {
			return (inout_str != 0) ? (CxI64)strtol(inout_str, &inout_str, inopt_base) : 0;
		}

		/** @see readI32(CxChar *&, CxI32) */
		CX_FORCE_INLINE CxU32 readU32(CxChar *&inout_str, CxI32 inopt_base = 0) {
			return (inout_str != 0) ? (CxU32)strtoul(inout_str, &inout_str, inopt_base) : 0;
		}

		/** @see readI32(CxChar *&, CxI32) */
		CX_FORCE_INLINE CxU64 readU64(CxChar *&inout_str, CxI32 inopt_base = 0) {
			return (inout_str != 0) ? (CxU64)strtoul(inout_str, &inout_str, inopt_base) : 0;
		}

		/**
		 * @brief Read a float from a string and advance the pointer.
		 * This method reads a float value from a string and advances
		 * the string pointer to the next character after the float value read.
		 * @param inout_str The string to read the value from and increment.
		 * @return The float value read or 0 if cannot convert.
		 */
		CX_FORCE_INLINE CxF32 readF32(CxChar *&inout_str) {
			return (inout_str != 0) ? (CxF32)strtod(inout_str, &inout_str) : 0;
		}

		/** @see readF32(CxChar *&) */
		CX_FORCE_INLINE CxF64 readF64(CxChar *&inout_str) {
			return (inout_str != 0) ? (CxF64)strtod(inout_str, &inout_str) : 0;
		}

		/**
		 * @brief Read data from a formatted string.
		 * @see scanf().
		 * @param in_str The string to read the data from.
		 * @param in_format The format of the string to read from.
		 * @param ... The arguments to store the data into.
		 * @return The number of arguments in the list successfully filled.
		 */
		CxI32 scan(const CxChar *CX_RESTRICT in_str,
					  const CxChar *CX_RESTRICT in_format, ...);

		/**
		 * @brief Test if one string is the prefix of another string.
		 * If either string is null, returns false.  If equal, returns true.
		 * @param in_str The string to test.
		 * @param in_prefix The prefix string to test for.
		 * @return True if in_str starts with in_prefix.
		 */
		CxBool startsWith(const CxChar *CX_RESTRICT in_str,
								const CxChar *CX_RESTRICT in_prefix);

		/**
		 * @brief Convert a string to an integer.
		 * @param in_str The string to try and convert to an integer value.
		 * @param inopt_base The optional base for the value (default = autodetect)
		 * @return The conversion of the string to integer or zero if cannot convert.
		 */
		CX_FORCE_INLINE CxI32 toI32(const CxChar *CX_RESTRICT in_str, CxI32 inopt_base = 0) {
			return (in_str != 0) ? (CxI32)strtol(in_str, 0, inopt_base) : 0;
		}

		/** @see str::toI32 **/
		CX_FORCE_INLINE CxI64 toI64(const CxChar *CX_RESTRICT in_str, CxI32 inopt_base = 0) {
		   return (in_str != 0) ? (CxI64)strtol(in_str, 0, inopt_base) : 0;
		}

		/** @see str::toI32 **/
		CX_FORCE_INLINE CxU32 toU32(const CxChar *CX_RESTRICT in_str, CxI32 inopt_base = 0) {
			return (in_str != 0) ? (CxU32)strtoul(in_str, 0, inopt_base) : 0;
		}

		/** @see str::toI32 **/
		CX_FORCE_INLINE CxU64 toU64(const CxChar *CX_RESTRICT in_str, CxI32 inopt_base = 0) {
			return (in_str != 0) ? (CxU64)strtoul(in_str, 0, inopt_base) : 0;
		}
		
		/**
		 * @brief Convert a string to a float.
		 * @param in_str The string to try and convert to a float value.
		 * @return The conversion of the string to float or zero if cannot convert.
		 */
		CX_FORCE_INLINE CxF32 toF32(const CxChar *CX_RESTRICT in_str) {
			return (in_str != 0) ? (CxF32)strtod(in_str, 0) : 0;
		}

		/** @see str::toF32 **/
		CX_FORCE_INLINE CxF64 toF64(const CxChar *CX_RESTRICT in_str) {
			return (in_str != 0) ? (CxF64)strtod(in_str, 0) : 0;
		}

	} // namespace str
	
} // namespace cat


#endif // CX_CORE_COMMON_CXSTR_H
