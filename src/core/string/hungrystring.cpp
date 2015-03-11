#include <cstring>
#include "core/string/hungrystring.h"


namespace Cat {

	HungryString::HungryString(const Char* string, Size maxLength) {
		m_pString = copy(string);
		m_maxLength = maxLength;
		m_pBuiltString = new Char[m_maxLength];
		m_currentIdx = 0;
		m_currentBuildIdx = 0;
		m_remainingFeeds = 0;
		m_finished = false;		
		findAndSetNumberOfParamsInString();		
		findNextParamAndCopy();		
	}

	HungryString::HungryString(const HungryString& src) {
		m_pString = NIL;
		m_pBuiltString = NIL;		
		if (src.m_pString) {			
			m_pString = copy(src.m_pString);
		}
		if (src.m_pBuiltString) {
			m_pBuiltString = new Char[src.m_maxLength];			
		   memcpy(m_pBuiltString, src.m_pBuiltString, sizeof(Char)*src.m_maxLength);			
		}
		m_maxLength = src.m_maxLength;
		m_remainingFeeds = src.m_remainingFeeds;
		m_currentIdx = src.m_currentIdx;
		m_currentBuildIdx = src.m_currentBuildIdx;
		m_finished = src.m_finished;
	}				  

	HungryString::~HungryString() {
		if (m_pString) {
			delete[] m_pString;
			m_pString = NIL;
		}

		if (m_pBuiltString) {
			delete[] m_pBuiltString;
		}
		m_maxLength = m_remainingFeeds = 0;		
	}

	HungryString& HungryString::operator=(const HungryString& src) {
		if (m_pString) {
			delete[] m_pString;			
			m_pString = NIL;
		}
		if (m_pBuiltString) {
			delete[] m_pBuiltString;			
			m_pBuiltString = NIL;
		}		
		if (src.m_pString) {			
			m_pString = copy(src.m_pString);
		}
		if (src.m_pBuiltString) {
			m_pBuiltString = new Char[src.m_maxLength];			
		   memcpy(m_pBuiltString, src.m_pBuiltString, sizeof(Char)*src.m_maxLength);	
		}
		m_maxLength = src.m_maxLength;
		m_remainingFeeds = src.m_remainingFeeds;
		m_currentIdx = src.m_currentIdx;
		m_currentBuildIdx = src.m_currentBuildIdx;
		m_finished = src.m_finished;

		return *this;
	}	

	HungryString& HungryString::feed(const Char* str) {
		if (!m_finished) {
			Size len = strlen(str);			
			memcpy(&(m_pBuiltString[m_currentBuildIdx]), str, sizeof(Char)*len);
			m_currentBuildIdx += len;
			--m_remainingFeeds;
			findNextParamAndCopy();			
		}
		return *this;		
	}
	
	void HungryString::findAndSetNumberOfParamsInString() {
		Size idx = 0;
		while (m_pString[idx] != '\0') {
			if (m_pString[idx] == '%') {
				/* Treat %% as literal % */
				if (m_pString[idx+1] == '%') {
					idx += 2;
				}
				else {
					++m_remainingFeeds;
					++idx;
				}
			} else {
				++idx;
			}						
		}	
	}	
			
	void HungryString::findNextParamAndCopy() {
		if (m_remainingFeeds > 0) {
			Size idx = m_currentIdx;
			while (m_pString[idx] != '\0') {
				if (m_pString[idx] == '%') {
					/* If we see %%, then treat as literal % character and copy */
					if (m_pString[idx+1] == '%') {
						memcpy(&(m_pBuiltString[m_currentBuildIdx]), &(m_pString[m_currentIdx]),
								 sizeof(Char)*((idx - m_currentIdx) + 1));
						m_currentBuildIdx += ((idx - m_currentIdx) + 1);					
						idx += 2;
						m_currentIdx = idx;
					}				
					else {
						if (m_currentIdx != idx) {							
							memcpy(&(m_pBuiltString[m_currentBuildIdx]),
									 &(m_pString[m_currentIdx]),
									 sizeof(Char)*(idx - m_currentIdx));							
							m_currentBuildIdx += (idx - m_currentIdx);
						}
						
						m_currentIdx = idx + 1; /* The next char after the % */
						break;					
					}
				} else {	
					idx++;
				}				
			}
		}	
		/* If no more parameters to feed, copy rest of string */
		else if(!m_finished) {
			memcpy(&(m_pBuiltString[m_currentBuildIdx]), &(m_pString[m_currentIdx]),
					 sizeof(Char)*(strlen(m_pString) - m_currentIdx + 1));
			m_finished = true;
		}
	}
	
					 

} // namespace Cat
