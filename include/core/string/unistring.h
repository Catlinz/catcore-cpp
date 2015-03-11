#ifndef CAT_CORE_STRING_UNISTRING_H
#define CAT_CORE_STRING_UNISTRING_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file unistring.h
 * @brief A simple UniString wrapper class.
 *
 * The UniString class is a Unicode string class that can also be used for reference 
 * counted safe pointers to unicode strings.
 *
 * @author Catlin Zilinski
 * @date June 15, 2014
 */

#include <cstring>
#include "core/threading/atomic.h"
#include "core/util/invasivestrongptr.h"

namespace Cat {
	
	/**
	 * @class UniString unistring.h "core/string/unistring.h"
	 * @brief A simple UniString wrapper class.
	 *
	 * The UniString class is a Unicode string class that can also be used for reference 
	 * counted safe pointers to unicode strings.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Apr 1, 2014
	 */
	class UniString {		
	  public:
		/**
		 * @brief Create an empty, null unicode string.
		 */
		inline UniString() :
			m_pString(NIL), m_length(0) {}

		/**
		 * @brief Create a new Unicode String from a copy of a null-terminated c-string.
		 * @param str The null-terminated c-string to initialize the UniString from.
		 */
		explicit UniString(const Char* str);

		/**
		 * @brief Create a new String from a single Character.
		 * @param chr The Character to create the string from.
		 */
		explicit UniString(Char chr) : m_pString(NIL), m_length(1) {
			m_pString = new U16[2];
			m_pString[0] = (U16)chr;
			m_pString[1] = 0;			
		}

		/**
		 * @brief Create a new Unicode String from a copy of a null-terminated unicode string.
		 * @param str The null-terminated unicode-string to initialize the UniString from.
		 */
		explicit UniString(const U16* str);		
		

		/**
		 * @brief Create a new String from a single Unicode Character.
		 * @param chr The Unicode Character to create the string from.
		 */
		explicit UniString(U16 chr) : m_pString(NIL), m_length(1) {
			m_pString = new U16[2];
			m_pString[0] = chr;
			m_pString[1] = 0;			
		}


		/**
		 * @brief Copy constructor, creates a copy of the string.
		 * @param src The String to copy.
		 */
		UniString(const UniString& src) : m_pString(NIL), m_length(0) {
			if (src.m_length > 0) {				
				m_pString = new U16[src.m_length];
				memcpy(m_pString, src.m_pString, sizeof(U16)*(src.m_length));
				m_length = src.m_length;
			}
		}

		/**
		 * @brief Destructor, deletes the associated string data.
		 */
		~UniString() {
			if (m_pString) {
				delete[] m_pString;
				m_pString = NIL;
				m_length = 0;
			}
		}

		/**
		 * @brief Overloaded assignment operator to copy a UniString.
		 * @param src The UniString to copy from.
		 * @return A reference to this string.
		 */
		UniString& operator=(const UniString& src);

		/**
		 * @brief Overloaded assignment operator to copy a unicode c-string.
		 * @param src The unicode c-string to copy from.
		 * @return A reference to this string.
		 */
		UniString& operator=(const U16* str);

		/**
		 * @brief Overloaded assignment operator to copy a c-string.
		 * @param src The c-string to copy from.
		 * @return A reference to this string.
		 */
		UniString& operator=(const Char* str);

		/**
		 * @brief UniString equality operator.
		 * @param other The other UniString to compare this one to.
		 * @return True if the strings are equal.
		 */
		Boolean operator==(const UniString& other) const;		

		/**
		 * @brief UniString character equality operator.
		 * @param other The character to compare this UniString to.
		 * @return True if this UniString is equal to the character.
		 */
		inline Boolean operator==(Char other) const {
			return ( (m_length == 1 && m_pString[0] == (U16)other) ||
						(m_length == 0 && other == '\0') );			
		}

		/**
		 * @brief UniString unicode character equality operator.
		 * @param other The unicode character to compare this UniString to.
		 * @return True if this UniString is equal to the unicode character.
		 */
		inline Boolean operator==(U16 other) const {
			return ( (m_length == 1 && m_pString[0] == other) ||
						(m_length == 0 && other == 0) );			
		}

		/**
		 * @brief UniString not equals operator.
		 * @param other The other UniString to compare this one to.
		 * @return True if the strings are not equal.
		 */
		inline Boolean operator!=(const UniString& other) const {
			return !(operator==(other));			
		}

		/**
		 * @brief UniString character not equals operator.
		 * @param other The character to compare this UniString to.
		 * @return True if this UniString is NOT equal to the character.
		 */
		inline Boolean operator!=(Char other) const {
			return ( (m_length > 1) ||
						(m_length == 1 && m_pString[0] != (U16)other) ||
						(m_length == 0 && other != '\0') );			
		}

		/**
		 * @brief UniString unicode character not equals operator.
		 * @param other The unicode character to compare this UniString to.
		 * @return True if this UniString is NOT equal to the unicode character.
		 */
		inline Boolean operator!=(U16 other) const {
			return ( (m_length > 1) ||
						(m_length == 1 && m_pString[0] != other) ||
						(m_length == 0 && other != 0) );			
		}

		/**
		 * @brief UniString array access operator.
		 * @param idx The index of the character to get.
		 * @return The character at the specified index.
		 */
		inline U16 operator[](Size idx) const {
			D_CONDERR((idx > m_length), "Index " << idx << " out of range!");
			return m_pString[idx];
		}

		/**
		 * @brief Get the unicode character at the specified index in the UniString.
		 * @param idx The index of the unicode character in the UniString to get.
		 * @return The Char at idx.
		 */
		inline U16 at(Size idx) const {
			return (*this)[idx];
		}		

		/**
		 * @brief Copy another UniString.
		 * @param src The UniString to copy from.
		 * @return A reference to this string.
		 */
		inline UniString& copy(const UniString& src) {
			return operator=(src);
		}

		/**
		 * @brief Get a const pointer to the actual string data.
		 * @return A const pointer to the actual string data.
		 */
		inline const U16* str() const { return m_pString; }		

		/**
		 * @brief Method to test to see if another UniString equals this one.
		 * @param other The other UniString to test for equality.
		 * @return True if the two strings are equal.
		 */
		inline Boolean equals(const UniString& other) const {
			return operator==(other);
		}

		/**
		 * @brief Method to test to see if a c-string equals this UniString.
		 * @param other The other C-UniString to test for equality.
		 * @return True if the two strings are equal.
		 */
		Boolean equals(const Char* str) const;		

		/**
		 * @brief Method to test to see if a character equals this UniString.
		 * @param other The character to test for equality.
		 * @return True if this UniString is the character.
		 */
		inline Boolean equals(Char other) const {
			return operator==(other);
		}

		/**
		 * @brief Method to test to see if a unicode character equals this UniString.
		 * @param other The unicode character to test for equality.
		 * @return True if this UniString is the unicode character.
		 */
		inline Boolean equals(U16 other) const {
			return operator==(other);
		}

		/**
		 * @brief Test to see if the UniString is empty or not.
		 * @return True if the UniString is empty.
		 */
		inline Boolean isEmpty() const { return m_length == 0; }		

		/**
		 * @brief Get the length of the UniString.
		 * @return The length of the UniString.
		 */
		inline Size length() const { return m_length; }		

		/**
		 * @brief Increase the retain count by one.
		 */
		inline void retain() { m_retainCount.increment(); }

		/**
		 * @brief Decrement the retainCount by one.
		 * @return True if there are no more references to the UniString.
		 */
		inline Boolean release() {
			return m_retainCount.decrement() <= 0;			
		}

		/**
		 * @brief Get the retain count for the UniString.
		 * @return The Retain count for the UniString.
		 */
		inline I32 retainCount() const { return m_retainCount.val(); }

	  private:
		U16* m_pString;
		Size  m_length;
		AtomicI32 m_retainCount;
	};

	typedef InvasiveStrongPtr<UniString> UniStringPtr;
	
#if defined (DEBUG)
	std::ostream& operator<<(std::ostream& out, const UniString& str);	
	std::ostream& operator<<(std::ostream& out, const UniStringPtr& str);	
#endif /* DEBUG */

} // namespace Cat

#endif // CAT_CORE_STRING_UNISTRING_H
