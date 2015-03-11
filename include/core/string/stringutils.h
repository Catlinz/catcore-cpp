#ifndef CAT_CORE_STRING_STRINGUTILS_H
#define CAT_CORE_STRING_STRINGUTILS_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file stringutils.h
 * @brief A static class with string utility methods.
 *
 * @author Catlin Zilinski
 * @date Mar 29, 2014
 */

#include "core/corelib.h"
#include <cstring>

namespace Cat {
	
	/**
	 * @class StringUtils stringutils.h "core/string/stringutils.h"
	 * @brief A static class with string utility methods.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 29, 2014
	 */
	class StringUtils {		
	  public:

		/**
		 * @brief Concatenate two strings together and return the result.
		 * @param p_str1 The first string to concatenate.
		 * @param p_str2 The string to contcatenate with the first.
		 * @param The dynamically allocated result.
		 */
		static Char* concat(const Char* p_str1, const Char* p_str2);

		/**
		 * @brief Concatenate two strings together and return the result.
		 * @param p_str1 The first string to concatenate.
		 * @param p_str2 The string to contcatenate with the first.
		 * @param The dynamically allocated result.
		 */
		static U16* concat(const U16* p_str1, const U16* p_str2);
		
		/**
		 * @brief Copy a null-terminated c-string and return the copy.
		 * @param str The null-terminated c-string to make a copy of.
		 * @return A copy of the string (null-terminated c-string).
		 */
		static Char* copy(const Char* str);

		/**
		 * @brief Copy a null-terminated unicode c-string and return the copy.
		 * @param str The null-terminated unicode c-string to copy.
		 * @return A copy of the string (null-terminated).
		 */
		static U16* copy(const U16* str);		

		/**
		 * @brief Create a new c-string with space for a null-terminating char.
		 * @param length The length of the string to create (w/o null-terminating char).
		 * @return A string with length (length + 1) for null terminating char.
		 */
		inline static Char* create(Size length) {
			return new Char[length + 1];
		}

		/**
		 * @brief Create a new unicode c-string with space for a null-term char.
		 * @param length The length of the string to create (w/o null-term).
		 * @return A unicode string with length (length + 1) for null-term.
		 */
		inline static U16* createUnicode(Size length) {
			return new U16[length + 1];
		}

		/**
		 * @brief Tests to see if two strings are equal or not.
		 * @param str1 The first null-terminated cstring to compare.
		 * @param str2 The second null-terminated cstring to compare.
		 * @return True if the two strings are equal, false if not equal, 
		 * or one or both of the strings are null.
		 */
		static inline Boolean equals(const Char* str1, const Char* str2) {
			return (str1 != NIL && str2 != NIL) && (strcmp(str1, str2) == 0);
		}

		/**
		 * @brief Tests to see if two unicode strings are equal or not.
		 * @param str1 The first null-terminated unicode string to compare.
		 * @param str2 The second null-terminated unicode string to compare.
		 * @return True if the two strings are equal, false if not equal, 
		 * or one or boht of the strings are null.
		 */
		static Boolean equals(const U16* str1, const U16* str2);		

		/**
		 * @brief Find a substring in another string and return a pointer to its location.
		 * @param haystack The string to search inside.
		 * @param needle The substring to search for.
		 * @return A pointer to the first occurance of the substring or NIL if not found.
		 */
		static inline const Char* find(const Char* haystack, const Char* needle) {
			return strstr(haystack, needle);
		}
		static inline Char* find(Char* haystack, const Char* needle) {
			return strstr(haystack, needle);
		}

		/**
		 * @brief Find the first occurance of a character in another string.
		 * @param haystack The string to search inside.
		 * @param needle The character to search for.
		 * @return A pointer to the first occurance of needle in haystack or NIL if none.
		 */
		static inline const Char* find(const Char* haystack, Char needle) {
			return strchr(haystack, needle);
		}
		static inline Char* find(Char* haystack, Char needle) {
			return strchr(haystack, needle);
		}

		/**
		 * @brief Find the last occurance of a character in another string.
		 * @param haystack The string to search inside.
		 * @param needle The character to search for.
		 * @return A pointer to the last occurance of needle in haystack or NIL if none.
		 */
		static inline const Char* findLast(const Char* haystack, Char needle) {
			return strrchr(haystack, needle);
		}
		static inline Char* findLast(Char* haystack, Char needle) {
			return strrchr(haystack, needle);
		}

		/**
		 * @brief Null-safe deletion of a c-string.
		 * @param str The string to delete.
		 * @return A null pointer.
		 */
		static inline Char* free(Char* str) {
			if (str) { delete[] str; }
			return NIL;
		}

		/**
		 * @brief Null-safe deletion of a unicode string.
		 * @param str The unicode string to delete.
		 * @return A null pointer.
		 */
		static inline U16* free(U16* str) {
			if (str) { delete[] str; }
			return NIL;
		}

		/**
		 * @brief Get the index of the first occurance of a substring.
		 * @param haystack The string to search in.
		 * @param needle The substring to search for.
		 * @return The index of the first occurance of the substring or -1 if not found.
		 */
		static inline I32 indexOf(const Char* haystack, const Char* needle) {
			const Char* occurance = StringUtils::find(haystack, needle);
			if (occurance) { return (I32)(occurance - haystack); }
			else { return -1; }
		}

		/**
		 * @brief Get the index of the first occurance of a character.
		 * @param haystack The string to search in.
		 * @param needle The character to search for.
		 * @return The index of the first occurance of the character or -1 if not found.
		 */
		static inline I32 indexOf(const Char* haystack, Char needle) {
			const Char* occurance = StringUtils::find(haystack, needle);
			if (occurance) { return (I32)(occurance - haystack); }
			else { return -1; }
		}

		/**
		 * @brief Get the index of the last occurance of a character.
		 * @param haystack The string to search in.
		 * @param needle The character to search for.
		 * @return The index of the last occurance of the character or -1 if not found.
		 */
		static inline I32 lastIndexOf(const Char* haystack, Char needle) {
			const Char* occurance = StringUtils::findLast(haystack, needle);
			if (occurance) { return (I32)(occurance - haystack); }
			else { return -1; }
		}	

		/**
		 * @brief Get the length of a null-terminated c-string.
		 * @param str The null-terminated c-string to get the length of.
		 * @return The length of the null-terminated cstring.
		 */
		static inline Size length(const Char* str) {
			if (str) { return strlen(str); }
			else { return 0; }
		}

		/**
		 * @brief Get the length of a null-terminated unicode c-string.
		 * @param str The null-terminated unicode c-string to get the length.
		 * @return The length of the null-terminated unicode string.
		 */
		static Size length(const U16* str);		

		/**
		 * @brief Copy all of another string into the start of a string.
		 * @param dest The destination string.
		 * @param src The string to copy from.
		 */
		static inline void sub(Char* dest, const Char* src) {
			memcpy(dest, src, sizeof(Char)*StringUtils::length(src));
		}

		/**
		 * @brief Copy part of another string into the start of a string.
		 * @param dest The destination string.
		 * @param src The string to copy from.
		 * @param destStart the index to put the string into.
		 */
		static inline void sub(Char* dest, const Char* src,
									  Size destStart ) {
			memcpy(&(dest[destStart]), src, sizeof(Char)*StringUtils::length(src));
		}		

		/**
		 * @brief Copy part of another string into a string.
		 * @param dest The destination string.
		 * @param src The string to copy from.
		 * @param destStart The index to copy the string into.
		 * @param srcLength The length of the string to copy.
		 */
		static inline void sub(Char* dest, const Char* src,
									  Size destStart, Size srcLength ) {
			memcpy(&(dest[destStart]), src, sizeof(Char)*srcLength);
		}

		/**
		 * @brief Convert an ascii c-style string to a unicode string.
		 * @param str The ascii string to convert to a unicode string.
		 * @return A unicode c-string.
		 */
		static U16* asciiToUnicode(const Char* str);		
	};

} // namespace Cat


#endif // CAT_CORE_STRING_STRINGUTILS_H
