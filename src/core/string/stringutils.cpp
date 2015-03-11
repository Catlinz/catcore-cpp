#include "core/string/stringutils.h"


namespace Cat {

	Char* StringUtils::concat(const Char* p_str1, const Char* p_str2) {
		I32 len1 = StringUtils::length(p_str1);
		I32 len2 = StringUtils::length(p_str2);
		Char* str = NIL;

		str = new Char[len1+len2+1];
		if (len1 > 0) {
			sub(str, p_str1, 0, len1);
		}
		if (len2 > 0) {
			sub(str, p_str2, len1, len2);
		}
		str[len1+len2] = '\0';
		return str;
	}

	U16* StringUtils::concat(const U16* p_str1, const U16* p_str2) {
		I32 len1 = StringUtils::length(p_str1);
		I32 len2 = StringUtils::length(p_str2);
		U16* str = NIL;

		str = new U16[len1+len2+1];
		if (len1 > 0) {
		   memcpy(str, p_str1, sizeof(U16)*len1);
		}
		if (len2 > 0) {
			memcpy(&(str[len1]), p_str1, sizeof(U16)*len2);
		}
		str[len1+len2] = 0;
		return str;
	}

	Char* StringUtils::copy(const Char* str) {
		if (str) {			
			Size stringLength = (strlen(str)+1);
			Char* newStr = new Char[stringLength];		
			memcpy(newStr, str, sizeof(Char)*stringLength);
			return newStr;
		}
		else {
			return NIL;
		}		
	}

	U16* StringUtils::copy(const U16* str) {
		if (str) {
			Size stringLength = StringUtils::length(str);
			U16* newStr = StringUtils::createUnicode(stringLength);			
			memcpy(newStr, str, sizeof(U16)*(stringLength+1));
			return newStr;
		}
		else {
			return NIL;
		}
	}

	Boolean StringUtils::equals(const U16* str1, const U16* str2) {
		if (str1 != NIL && str2 != NIL) {
			while (((*str1) & (*str2)) != 0) {
				if ((*str1) != (*str2)) {
					return false;
				}
				++str1;
				++str2;
			}
			if (str1 == str2) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	Size StringUtils::length(const U16* str) {
	   Size len = 0;
		if (str) {
			while (str[len] != 0) {
				++len;
			}
		}
		return len;
	}

	U16* StringUtils::asciiToUnicode(const Char* str) {
		Size len = StringUtils::length(str);
		U16* uniStr = StringUtils::createUnicode(len);
		for (Size i = 0; i < len; ++i) {
			uniStr[i] = (U16)str[i];
		}
		uniStr[len] = 0;		
		return uniStr;
	}	

} // namespace
