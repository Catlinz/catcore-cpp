#include "core/string/string.h"
#include "core/string/stringutils.h"

namespace Cat {

	String& String::operator=(const String& src) {
		if (m_pString) {
			delete[] m_pString;
			m_pString = NIL;				
			m_length = 0;				
		}			
		if (src.m_pString) {				
			m_pString = new Char[src.m_length+1];
			memcpy(m_pString, src.m_pString, sizeof(Char)*(src.m_length + 1));
			m_length = src.m_length;
		}
		return *this;
	}

	String& String::operator=(const Char* src) {
		if (m_pString) {
				delete[] m_pString;
				m_pString = NIL;				
				m_length = 0;				
		}
		if (src != NIL) {
			m_length = StringUtils::length(src);
			m_pString = new Char[m_length+1];
			memcpy(m_pString, src, sizeof(Char)*(m_length + 1));			
		}
		return *this;
	}
		

	Boolean String::equalsIgnoreCase(const String& other) const {
		if (m_pString == other.m_pString) {
			return true;
		}
		else if (m_length == other.m_length) {
			for (Size idx = 0; idx < m_length; idx++) {
				if (tolower(m_pString[idx]) != tolower(other.m_pString[idx])) {
					return false;
				}
			}
			return true;			
		}
		else {
			return false;
		}
	}

	Boolean String::equalsIgnoreCase(const Char* other) const {
		if (other == NIL && m_length > 0) {
			return false;
		}
		else if (m_pString == other) {
			return true;
		}		
		Size len = StringUtils::length(other);
	   if (m_length == len) {
			for (Size idx = 0; idx < m_length; idx++) {
				if (tolower(m_pString[idx]) != tolower(other[idx])) {
					return false;
				}
			}
			return true;			
		}
		else {
			return false;
		}
	}	

	void String::store(Char* str, I32 length) {
		if (m_pString) {
			delete[] m_pString;
		}
		m_pString = str;
		if (length > 0) {
			m_length = (Size)length;
		}
		else {
			m_length = StringUtils::length(m_pString);
		}
	}

} // namespace Cat
