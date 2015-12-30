#ifndef CX_CORE_COMMON_CXUTF8_H
#define CX_CORE_COMMON_CXUTF8_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxUtf8.h
 * @brief Contains basic UTF-8 string operations.
 *
 * @author Catlin Zilinski
 * @date Dec 26, 2015
 */

#include "core/common/CxStr.h"


namespace cat {

	namespace utf8 {

		/**
		 * @brief Method to create a new UTF-8 string.
		 * @param in_bytes The number of bytes to allocate (excl. null-term).
		 * @return A pointer to a newly allocated memory.
		 */
		CX_FORCE_INLINE CxU8 * alloc(CxI32 in_bytes) {
			return (CxU8 *)mem::alloc(sizeof(CxU8)*(in_bytes + 1));
		}

		/**
		 * @brief Method to create a new UTF-8 string containing all zeros.
		 * @param in_bytes The number of bytes to allocate (excl. null-term).
		 * @return A pointer to a newly allocated zeroed memory.
		 */
		CX_FORCE_INLINE CxU8 * allocZero(CxI32 in_bytes) {
			return (CxU8 *)mem::allocZero(sizeof(CxU8), in_bytes + 1);
		}

		/**
		 * @brief Method to append bytes of one UTF-8 string to another string.
		 * @param inout_dest The UTF-8 string to append to.
		 * @param in_destBytes The number of bytes in the destination buffer.
		 * @param in_src The UTF-8 string to append to the other string.
		 * @param in_bytes The number of bytes to use from the source string.
		 * @return A pointer to the destination UTF-8 string is returned.
		 */
		CX_FORCE_INLINE CxU8 * append(CxU8 *CX_RESTRICT inout_dest, CxI32 in_destBytes,
												  const CxU8 *CX_RESTRICT in_src, CxI32 in_bytes) {
#if defined(CX_WINDOWS)
			strncat_s((CxChar *)inout_dest, in_destBytes, (const CxChar *)in_src, in_bytes);  return inout_dest;
#else
			return (CxU8 *)strncat((CxChar *)inout_dest, (const CxChar *)in_src, in_bytes);
#endif
		}

		/**
		 * @brief Method to append one UTF-8 string to another.
		 * @param inout_dest The UTF-8 string to append to.
		 * @param in_destBytes The number of bytes in the destination buffer.
		 * @param in_src The UTF-8 string to append to the other string.
		 * @return A pointer to the destination UTF-8 string is returned.
		 */
		CX_FORCE_INLINE CxU8 * append(CxU8 *CX_RESTRICT inout_dest, CxI32 in_destBytes,
												  const CxU8 *CX_RESTRICT in_src) {
#if defined(CX_WINDOWS)
		   strcat_s((CxChar *)inout_dest, in_destBytes, (const CxChar *)in_src);  return inout_dest;
#else
			return (CxU8 *)strcat((CxChar *)inout_dest, (const CxChar *)in_src);
#endif
		}

		/**
		 * @brief Method to get number of bytes of a string.
		 * @param in_str The string to get the size of.
		 * @return The number of bytes excluing null-term.
		 */
		CX_FORCE_INLINE CxI32 bytes(const CxU8 *CX_RESTRICT in_str) {
			return (in_str != 0) ? strlen((const CxChar *)in_str) : 0;
		}

		/**
		 * @brief Method to copy an entire string (incl null term) into another string.
		 * @param inout_dest The string to copy into.
		 * @param in_destBytes The amount of room in the destination string (incl. null term).
		 * @param in_src The string to copy.
		 * @return A pointer to destination string is returned.
		 */
		CX_FORCE_INLINE CxU8 * copy(CxU8 *CX_RESTRICT inout_dest, CxI32 in_destBytes,
												const CxU8 *CX_RESTRICT in_src) {
#if defined(CX_WINDOWS)
			strcpy_s((CxChar *)inout_dest, in_destBytes, (const CxChar *)in_src);  return inout_dest;
#else
			return (CxU8 *)strcpy((CxChar *)inout_dest, (const CxChar *)in_src);
#endif
		}
		
		/**
		 * @brief Method to copy an entire string (incl null term) into another string.
		 * Tries to copy N bytes (where N is the lesser of in_bytes and the source string length)
		 * into the destination.  If in_bytes >= in_destBytes, then we set inout_dest[0] = 0 and
		 * just return it.  A null-terminating character is always appended at the position 
		 * inout_dest[in_bytes].
		 * @param inout_dest The string to copy into.
		 * @param in_destBytes The amount of room in the destination string (incl. null term).
		 * @param in_src The string to copy.
		 * @param in_bytes The number of bytes to copy from the src.
		 * @return A pointer to the destination string is returned.
		 */
		CX_FORCE_INLINE CxU8 * copy(CxU8 *CX_RESTRICT inout_dest, CxI32 in_destBytes,
											 const CxU8 *CX_RESTRICT in_src, CxI32 in_bytes) {
			if (in_destBytes > in_bytes) {
#if defined(CX_WINDOWS)
				strncpy_s((CxChar *)inout_dest, in_destBytes, (const CxChar *)in_src, in_bytes);
#else
				strncpy((CxChar *)inout_dest, (const CxChar *)in_src, in_bytes);
				inout_dest[in_bytes] = 0;
#endif
			}
			else { inout_dest[0] = 0; }
			
			return inout_dest;
		}
		
		/**
		 * @brief Method to create a new copy of a string.
		 * If the length is less than the length of the string, a null char 
		 * will still be appended at str[in_len].
		 * @param in_src The string to copy.
		 * @param in_bytes The bytes of the string to copy.
		 * @return A new string that is the copy of the original.
		 */
		CX_FORCE_INLINE CxU8 * copy(const CxU8 *CX_RESTRICT in_src, CxI32 in_bytes) {
			return utf8::copy(utf8::alloc(in_bytes), in_bytes + 1, in_src, in_bytes);
		}

		/**
		 * @brief Method to create a new copy of a UTF-8 string.
		 * @param in_src The UTF-8 string to copy.
		 * @return A new UTF-8 string that is the copy of the original, or null if original is null.
		 */
		CX_FORCE_INLINE CxU8 * copy(const CxU8 *CX_RESTRICT in_src) {
			if (in_src != 0) {
				const CxI32 bytes = utf8::bytes(in_src);
				return utf8::copy(utf8::alloc(bytes), bytes + 1, in_src, bytes);
			}
			else { return 0; }
		}

		/**
		 * @brief Method to create a new copy of a c-string string.
		 * @param in_src The c-string string to copy.
		 * @return A new UTF-8 string that is the copy of the original, or null if original is null.
		 */
		CX_FORCE_INLINE CxU8 * copy(const CxChar *CX_RESTRICT in_src) {
			if (in_src != 0) {
				const CxI32 bytes = str::len(in_src);
				return utf8::copy(utf8::alloc(bytes), bytes + 1, (const CxU8 *)in_src, bytes);
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
		CxBool endsWith(const CxU8 *CX_RESTRICT in_str, const CxU8 *CX_RESTRICT in_end);

		/**
		 * @brief Test if one string is the end of another string.
		 * If either string is null, returns false.  If equal, returns true.
		 * @param in_str The string to test.
		 * @param in_end The end string to test for.
		 * @return True if in_str starts with in_end.
		 */
		CX_FORCE_INLINE CxBool endsWith(const CxU8 *CX_RESTRICT in_str,
												  const CxChar *CX_RESTRICT in_end) {
			return utf8::endsWith(in_str, (const CxU8 *)in_end);
		}

		/**
		 * @brief Test if two UTF-8 strings are equal or not.
		 * @param in_str1 The first UTF-8 string to compare.
		 * @param in_str2 The second UTF-8 string to compare.
		 * @return True if the UTF-8 strings are equal.
		 */
		CxBool eq(const CxU8 *CX_RESTRICT in_str1, const CxU8 *CX_RESTRICT in_str2);

		/**
		 * @brief Test if a UTF-8 string and C-String are equal or not.
		 * @param in_str1 The UTF-8 string to compare.
		 * @param in_str2 The C-String string to compare.
		 * @return True if the two strings are equal.
		 */
		CX_FORCE_INLINE CxBool eq(const CxU8 *CX_RESTRICT in_utf8,
										  const CxChar *CX_RESTRICT in_cstr) {
			return utf8::eq(in_utf8, (const CxU8 *)in_cstr);
		}
		

		/**
		 * @brief Test if the first n characters of two strings are equal or not.
		 * @param in_str1 The first string to compare.
		 * @param in_str2 The second string to compare.
		 * @param in_bytes The number of bytes of the strings to test.
		 * @return True if the strings are equal.
		 */
		CX_FORCE_INLINE CxBool eq(const CxU8 *CX_RESTRICT in_str1,
										  const CxU8 *CX_RESTRICT in_str2, CxI32 in_bytes) {
			return ::memcmp(in_str1, in_str2, in_bytes) == 0;
		}
		
		/**
		 * @brief Method to free a UTF-8 string and set the pointer to 0.
		 * The method checks for null strings first.
		 * @param inout_str A reference to the pointer to the string.
		 */
		CX_FORCE_INLINE void free(CxU8 *&inout_str) {
			if (inout_str != 0) { ::free(inout_str); inout_str = 0; }
		}

		/**
		 * @brief Convert a wchar_t string to a UTF-8 string.
		 * @param in_src The wchar_t string to convert to a UTF-8 string.
		 * @return A pointer to a newly allocated UTF-8 string or null if null input.
		 */
		CxU8 * fromWideChars(const wchar_t *CX_RESTRICT in_src);
		
		/**
		 * @brief Test to see if str1 > str2.
		 * The test returns true if the value of the first nonmatching byte 
		 * of str1 is greater than that of str2.
		 * @param in_str1 The first string to compare.
		 * @param in_str2 The second string to compare with.
		 * @return True if str1 is greater than str2.
		 */
		CxBool greater(const CxU8 *CX_RESTRICT in_str1, const CxU8 *CX_RESTRICT in_str2);
		
		/**
		 * @brief Test to see if str1 < str2.
		 * The test returns true if the value of the first nonmatching character 
		 * of str1 is less than that of str2.
		 * @param in_str1 The first string to compare.
		 * @param in_str2 The second string to compare with.
		 * @return True if str1 is less than str2.
		 */
		CxBool less(const CxU8 *CX_RESTRICT in_str1, const CxU8 *CX_RESTRICT in_str2);

		/**
		 * @brief Test if one string is the prefix of another string.
		 * If either string is null, returns false.  If equal, returns true.
		 * @param in_str The string to test.
		 * @param in_prefix The prefix string to test for.
		 * @return True if in_str starts with in_prefix.
		 */
		CxBool startsWith(const CxU8 *CX_RESTRICT in_str, const CxU8 *CX_RESTRICT in_prefix);

		/**
		 * @brief Test if one string is the prefix of another string.
		 * If either string is null, returns false.  If equal, returns true.
		 * @param in_str The string to test.
		 * @param in_prefix the prefix string to test for.
		 * @return True if in_str starts with in_prefix.
		 */
		CX_FORCE_INLINE CxBool startsWith(const CxU8 *CX_RESTRICT in_str,
													 const CxChar *CX_RESTRICT in_prefix) {
			return utf8::startsWith(in_str, (const CxU8 *)in_prefix);
		}

		/**
		 * @brief Convert a UTF-8 string to a wchar_t string.
		 * @param in_src The UTF-8 string to convert to a wchar_t string.
		 * @return A pointer to a newly allocated wchar_t string or null if null input.
		 */
		wchar_t * toWideChars(const CxU8 *CX_RESTRICT in_src);
		
	} // namespace utf8
	
} // namespace cat


#endif // CX_CORE_COMMON_CXUTF8_H
