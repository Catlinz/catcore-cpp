#ifndef CAT_CORE_STRING_STRING_H
#define CAT_CORE_STRING_STRING_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file string.h
 * @brief A simple String wrapper class desgined to be used as a StringPtr.
 *
 * @author Catlin Zilinski
 * @date Apr 1, 2014
 */

#include <cstring>
#include "core/threading/atomic.h"
#include "core/util/invasivestrongptr.h"

namespace Cat {
	
	/**
	 * @class String string.h "core/string/string.h"
	 * @brief A simple String wrapper class desgined to be used as a StringPtr.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Apr 1, 2014
	 */
	class String {		
	  public:
		/**
		 * @brief Create an empty, null string.
		 */
		inline String() :
			m_pString(NIL), m_length(0) {}

		/**
		 * @brief Create a new String from a copy of a null-terminated c-string.
		 * @param str The null-terminated c-string to initialize the string from.
		 */
		explicit String(const Char* str) : m_pString(NIL), m_length(0) {
			m_length = strlen(str);
			m_pString = new Char[m_length + 1];
			memcpy(m_pString, str, sizeof(Char)*(m_length + 1));
		}

		/**
		 * @brief Create a new String from a single Character.
		 * @param chr The Character to create the string from.
		 */
		explicit String(Char chr) : m_pString(NIL), m_length(1) {
			m_pString = new Char[2];
			m_pString[0] = chr;
			m_pString[1] = '\0';
		}

		/**
		 * @brief Copy constructor, creates a copy of the string.
		 * @param src The String to copy.
		 */
		String(const String& src) : m_pString(NIL), m_length(0) {
			if (src.m_pString) {				
				m_pString = new Char[src.m_length+1];
				memcpy(m_pString, src.m_pString, sizeof(Char)*(src.m_length + 1));
				m_length = src.m_length;
			}
		}

		/**
		 * @brief Destructor, deletes the associated string data.
		 */
		~String() {
			if (m_pString) {
				delete[] m_pString;
				m_pString = NIL;
				m_length = 0;
			}
		}

		/**
		 * @brief Overloaded assignment operator to copy a String.
		 * @param src The String to copy from.
		 * @return A reference to this string.
		 */
		String& operator=(const String& src);

		/**
		 * @brief Overloaded assignment operator to copy a c-string.
		 * @param src The c-string to copy from.
		 * @return A reference to this string.
		 */
		String& operator=(const Char* str);

		/**
		 * @brief String equality operator.
		 * @param other The other String to compare this one to.
		 * @return True if the strings are equal.
		 */
		inline Boolean operator==(const String& other) const {
			return (m_pString == other.m_pString ||
					  (m_pString != NIL && other.m_pString != NIL &&
						strcmp(m_pString, other.m_pString) == 0));
		}

		/**
		 * @brief String equality operator.
		 * @param other The other c-string to compare this String to.
		 * @return True if the strings are equal.
		 */
		inline Boolean operator==(const Char* other) const {
			return ( (m_pString == other) ||
						(m_length == 0 && (other == NIL || other[0] == '\0')) ||
						(m_pString != NIL && other != NIL &&
						 strcmp(m_pString, other) == 0) );
		}

		/**
		 * @brief String character equality operator.
		 * @param other The character to compare this String to.
		 * @return True if this String is equal to the character.
		 */
		inline Boolean operator==(Char other) const {
			return ( (m_length == 1 && m_pString[0] == other) ||
						(m_length == 0 && other == '\0') );			
		}

		/**
		 * @brief String not equals operator.
		 * @param other The other String to compare this one to.
		 * @return True if the strings are not equal.
		 */
		inline Boolean operator!=(const String& other) const {
			return !(operator==(other));			
		}

		/**
		 * @brief String not equals operator.
		 * @param other The c-string to compare this String to.
		 * @return True if the strings are not equal.
		 */
		inline Boolean operator!=(const Char* other) const {
			return !(operator==(other));			
		}

		/**
		 * @brief String character not equals operator.
		 * @param other The character to compare this String to.
		 * @return True if this String is NOT equal to the character.
		 */
		inline Boolean operator!=(Char other) const {
			return ( (m_length > 1) ||
						(m_length == 1 && m_pString[0] != other) ||
						(m_length == 0 && other != '\0') );			
		}

		/**
		 * @brief String array access operator.
		 * @param idx The index of the character to get.
		 * @return The character at the specified index.
		 */
		inline Char operator[](Size idx) const {
			D_CONDERR((idx > m_length), "Index " << idx << " out of range!");
			return m_pString[idx];
		}

		/**
		 * @brief Get the character at the specified index in the String.
		 * @param idx The index of the Character in the String to get.
		 * @return The Char at idx.
		 */
		inline Char at(Size idx) const {
			return (*this)[idx];
		}		

		/**
		 * @brief Create and return a copy of this String.
		 * @return A copy of this String as a InvasiveStrongPtr<String>.
		 */
		inline InvasiveStrongPtr<String> clone() const {
			return InvasiveStrongPtr<String>(new String(*this));
		}

		/**
		 * @brief Copy another String.
		 * @param src The String to copy from.
		 * @return A reference to this string.
		 */
		inline String& copy(const String& src) {
			return operator=(src);
		}

		/**
		 * @brief Get a const pointer to the actual string data.
		 * @return A const pointer to the actual string data.
		 */
		inline const Char* cStr() const { return m_pString; }		

		/**
		 * @brief Method to test to see if another String equals this one.
		 * @param other The other String to test for equality.
		 * @return True if the two strings are equal.
		 */
		inline Boolean equals(const String& other) const {
			return operator==(other);
		}

		/**
		 * @brief Method to test to see if a c-string equals this String.
		 * @param other The other C-String to test for equality.
		 * @return True if the two strings are equal.
		 */
		inline Boolean equals(const Char* other) const {
			return operator==(other);
		}

		/**
		 * @brief Method to test to see if a character equals this String.
		 * @param other The character to test for equality.
		 * @return True if this String is the character.
		 */
		inline Boolean equals(Char other) const {
			return operator==(other);
		}

		/**
		 * @brief Case insensitive equality test.
		 * @param other The other String to test for equality (ignoring case).
		 * @return True if the two Strings are equals (ignoring case).
		 */
		Boolean equalsIgnoreCase(const String& other) const;

		/**
		 * @brief Case insensitive equality test.
		 * @param other The c-string to test for equality (ignoring case).
		 * @return True if the two Strings are equals (ignoring case).
		 */
		Boolean equalsIgnoreCase(const Char* other) const;

		/**
		 * @brief Case insensitive character equality test.
		 * @param other The character to test for equality (ignoring case).
		 * @return True if this String is the character (ignoring case).
		 */
		inline Boolean equalsIgnoreCase(Char other) const {
			return ( (m_length == 1 && tolower(m_pString[0]) == tolower(other)) ||
						(m_length == 0 && other == '\0') );
		}		

		/**
		 * @brief Test to see if the String is empty or not.
		 * @return True if the String is empty.
		 */
		inline Boolean isEmpty() const { return m_length == 0; }		

		/**
		 * @brief Get the length of the String.
		 * @return The length of the String.
		 */
		inline Size length() const { return m_length; }		

		/**
		 * @brief Increase the retain count by one.
		 */
		inline void retain() { m_retainCount.increment(); }

		/**
		 * @brief Decrement the retainCount by one.
		 * @return True if there are no more references to the String.
		 */
		inline Boolean release() {
			return m_retainCount.decrement() <= 0;
		}

		/**
		 * @brief Get the retain count for the String.
		 * @return The Retain count for the String.
		 */
		inline I32 retainCount() const { return m_retainCount.val(); }

		/**
		 * @brief Method to store a c-string into this string.
		 * This method will NOT copy the string, simply uses the pointer,
		 * so the pointer needs to be dynamically allocated, and not deleted
		 * afterwards.  The method will calculate the length of the string if 
		 * not input.  This method will also DELETE the current string stored.
		 * @param str The c-string to store in the string.
		 * @param len The optional length of the string.
		 */
		void store(Char* str, I32 length = -1);			

		/**
		 * @brief Static method to create a StringPtr from an empty String.
		 * @return The StringPtr containing the null string.
		 */
		static inline InvasiveStrongPtr<String> create()  {
			return InvasiveStrongPtr<String>(new String());
		}

		/**
		 * @brief Static method to create a StringPtr from a c-string.
		 * @param str The c-string to create the String from.
		 * @return The StringPtr containing the String.
		 */
		static inline InvasiveStrongPtr<String> create(const Char* str)  {
			return InvasiveStrongPtr<String>(new String(str));
		}

		/**
		 * @brief Static method to create a StringPtr from a character.
		 * @param chr The character to create the String from.
		 * @return The StringPtr containing the Character String.
		 */
		static inline InvasiveStrongPtr<String> create(Char chr)  {
			return InvasiveStrongPtr<String>(new String(chr));
		}			

	  private:
		Char* m_pString;
		Size  m_length;
		AtomicI32 m_retainCount;
	};

	typedef InvasiveStrongPtr<String> StringPtr;
	
#if defined (DEBUG)
	inline std::ostream& operator<<(std::ostream& out, const String& str) {
		if (str.isEmpty()) { return out << ""; }
		else { return out << str.cStr(); }		
	}

	inline std::ostream& operator<<(std::ostream& out, const StringPtr& str) {
		if (str.isNull() || str->isEmpty()) { return out << ""; }
		else { return out << str->cStr(); }		
	}
#endif /* DEBUG */

} // namespace Cat

#endif // CAT_CORE_SYS_SYSTEM_H
