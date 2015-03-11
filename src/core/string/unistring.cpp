#include "core/string/unistring.h"
#include "core/string/stringutils.h"

namespace Cat {

	UniString::UniString(const Char* str)
		: m_pString(NIL), m_length(0) {
		m_length = StringUtils::length(str);
		m_pString = new U16[m_length+1];
		for (Size i = 0; i <= m_length; ++i) {
			m_pString[i] = (U16)str[i];
		}		
	}

	UniString::UniString(const U16* str)
		: m_pString(NIL), m_length(0) {
		m_length = StringUtils::length(str);
		m_pString = new U16[m_length+1];
		memcpy(m_pString, str, sizeof(U16)*(m_length+1));	
	}

	UniString& UniString::operator=(const UniString& src) {
		if (src.m_length > 0) {
			U16* newData = new U16[src.m_length+1];
		   memcpy(newData, src.m_pString, sizeof(U16)*(src.m_length+1));
			U16* tmp = m_pString;
			m_pString = newData;
			m_length = src.m_length;			
			if (tmp) {
				delete[] tmp;
			}
		}
		else {
			m_length = 0;			
			if (m_pString) {
				delete[] m_pString;
				m_pString = NIL;				
			}
		}
		return *this;		
	}

	UniString& UniString::operator=(const Char* str) {
		if (str) {
			Size len = StringUtils::length(str);			
			U16* newData = new U16[len+1];
			for (Size i = 0; i <= m_length; ++i) {
				m_pString[i] = (U16)str[i];
			}	
			U16* tmp = m_pString;
			m_pString = newData;
			m_length = len;			
			if (tmp) {
				delete[] tmp;
			}
		}
		else {
			m_length = 0;			
			if (m_pString) {
				delete[] m_pString;
				m_pString = NIL;				
			}
		}
		return *this;		
	}

	UniString& UniString::operator=(const U16* str) {
		if (str) {
			Size len = StringUtils::length(str);			
			U16* newData = new U16[len+1];
			memcpy(newData, str, sizeof(U16)*(len+1));
			U16* tmp = m_pString;
			m_pString = newData;
			m_length = len;			
			if (tmp) {
				delete[] tmp;
			}
		}
		else {
			m_length = 0;			
			if (m_pString) {
				delete[] m_pString;
				m_pString = NIL;				
			}
		}
		return *this;		
	}

	Boolean UniString::operator==(const UniString& other) const {
		return (m_pString == other.m_pString ||
				  (m_pString != NIL && other.m_pString != NIL &&
					StringUtils::equals(m_pString, other.m_pString)));		
	}

	Boolean UniString::equals(const Char* str) const {
		if (m_length == 0 && (!str || str[0] == '\0')) {
			return true;
		}
		else if (!str || m_length == 0) {
			return false;
		}
		for (Size i = 0; i <= m_length; ++i) {
			if (m_pString[i] != (U16)str[i]) {
				return false;
			}			
		}
		return true;
	}

#if defined (DEBUG)
	std::ostream& operator<<(std::ostream& out, const UniString& str) {
		if (str.isEmpty()) {
			return out << "";
		}
		else {
			for (Size i = 0; i < str.length(); ++i) {
				if (str[i] <= 127) {
					out << (Char)str[i];
				}
				else {
					out << std::setbase(16) << str[i];
				}				
			}			
			return out << std::setbase(10);			
		}		
	}

	std::ostream& operator<<(std::ostream& out, const UniStringPtr& str) {
	   if (str.isNull()) {
			return out << "";
		}
		else {
			return out << (*str);
		}		
	}
#endif /* DEBUG */
	
	

} // namespace Cat
