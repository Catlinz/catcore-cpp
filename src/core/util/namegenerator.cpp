#include <cstring>
#include <cstdio>
#include "core/util/namegenerator.h"
#include "core/string/stringutils.h"

namespace Cat {

	const Char NameGenerator::encodingTable[64] = {
		'+', '/',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
		'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
		'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
		'u', 'v', 'w', 'x', 'y', 'z', 
	};

	NameGenerator::NameGenerator(const NameGenerator& src) {
		m_pPrefix = StringUtils::copy(src.m_pPrefix);
		m_prefixLength = src.m_prefixLength;
		m_maxLength = src.m_maxLength;
		m_count = src.m_count;
	}

	NameGenerator& NameGenerator::operator=(const NameGenerator& src) {
		m_pPrefix = StringUtils::free(m_pPrefix);
		m_pPrefix = StringUtils::copy(src.m_pPrefix);		
		m_prefixLength = src.m_prefixLength;
		m_maxLength = src.m_maxLength;
		m_count = src.m_count;
		return *this;		
	}		

	NameGenerator::~NameGenerator() {
		m_pPrefix = StringUtils::free(m_pPrefix);
	}

	void NameGenerator::initWithPrefix(const Char* prefix) {
		if (!m_pPrefix) {
			m_pPrefix = StringUtils::copy(prefix);
			m_prefixLength = StringUtils::length(prefix);	
			m_maxLength = m_prefixLength + CC_MAX_GENERATED_NAME_NUMBER_LENGTH + 2;
			m_count = 0;
		}
		else {
			DWARN("Cannot initialize NameGenerator already initialized w/ prefix: "
					<< m_pPrefix << ".");
		}
	}

	Char* NameGenerator::generate() {
		Char* str = new Char[m_maxLength];
		memcpy(str, m_pPrefix, sizeof(Char)*m_prefixLength);

		U64 val;		
		m_lock.lock();
		val = ++m_count;
		m_lock.unlock();

		str[m_prefixLength] = '=';		
		str[m_prefixLength+1] = encodingTable[(val & 0x3F)];
		str[m_prefixLength+2] = encodingTable[((val >> 6) & 0x3F)];
		str[m_prefixLength+3] = encodingTable[((val >> 12) & 0x3F)];
		str[m_prefixLength+4] = encodingTable[((val >> 18) & 0x3F)];
		str[m_prefixLength+5] = encodingTable[((val >> 24) & 0x3F)];
		str[m_prefixLength+6] = encodingTable[((val >> 30) & 0x3F)];
		str[m_prefixLength+7] = encodingTable[((val >> 36) & 0x3F)];
		str[m_prefixLength+8] = encodingTable[((val >> 42) & 0x3F)];
		str[m_prefixLength+9] = encodingTable[((val >> 48) & 0x3F)];
		str[m_prefixLength+10] = encodingTable[((val >> 54) & 0x3F)];
		str[m_prefixLength+11] = encodingTable[((val >> 60) & 0x3F)];
		str[m_prefixLength+12] = '\0';

		return str;				
   }
	
	
		
	
		
		

} // namespace Cat
