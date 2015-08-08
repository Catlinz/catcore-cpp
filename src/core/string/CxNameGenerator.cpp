#include "core/string/CxNameGenerator.h"
#include "core/common/CxStr.h"
#include "core/common/CxAtomic.h"

namespace cat {

	const CxChar CxNameGenerator::kEncodingTable[64] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
		'u', 'v', 'w', 'x', 'y', 'z', 
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
		'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
		'U', 'V', 'W', 'X', 'Y', 'Z',
		'+', '/',
	};

	CxNameGenerator::CxNameGenerator()
		: mp_prefix(0), m_count(0), m_prefixLength(0), m_maxLength(0) {}

	CxNameGenerator::CxNameGenerator(const CxChar *in_prefix)
		: mp_prefix(0), m_count(0), m_prefixLength(0), m_maxLength(0) {
		initWithPrefix(in_prefix);
	}
	
	CxNameGenerator::CxNameGenerator(const CxNameGenerator &in_src) {
		mp_prefix = str::copy(in_src.mp_prefix);
		m_count = in_src.m_count;
		m_prefixLength = in_src.m_prefixLength;
		m_maxLength = in_src.m_maxLength;
	}

	CxNameGenerator& CxNameGenerator::operator=(const CxNameGenerator &in_src) {
		str::free(mp_prefix);

		mp_prefix = str::copy(in_src.mp_prefix);
		m_count = in_src.m_count;
		m_prefixLength = in_src.m_prefixLength;
		m_maxLength = in_src.m_maxLength;
		return *this;		
	}		

	CxNameGenerator::~CxNameGenerator() {
		str::free(mp_prefix);
	}

	void CxNameGenerator::initWithPrefix(const CxChar *in_prefix) {
		if (mp_prefix != 0) {
			const CxI32 str_len = str::len(in_prefix);
			mp_prefix = str::copy(in_prefix, str_len);
			m_count = 0;
			m_prefixLength = str_len;
			m_maxLength = str_len + CX_NAMEGENERATOR_MAX_NUM_LEN + 1;
		}
		else {
			CXD_WARN("Cannot initialise CxNameGenerator, already initialise with prefix %s!", mp_prefix);
		}
	}

	CxChar * CxNameGenerator::generate() {
		const CxI32 max_len = m_maxLength;
		const CxI32 pre_len = m_prefixLength;
		
		CxChar *str = str::alloc8(max_len);
		str::copy(str, mp_prefix, pre_len);
	  
		const CxI64 val = atomic::incr64(m_count);

		str[m_prefixLength] = '_';		
		str[m_prefixLength+1] = kEncodingTable[(val & 0x3F)];
		str[m_prefixLength+2] = kEncodingTable[((val >> 6) & 0x3F)];
		str[m_prefixLength+3] = kEncodingTable[((val >> 12) & 0x3F)];
		str[m_prefixLength+4] = kEncodingTable[((val >> 18) & 0x3F)];
		str[m_prefixLength+5] = kEncodingTable[((val >> 24) & 0x3F)];
		str[m_prefixLength+6] = kEncodingTable[((val >> 30) & 0x3F)];
		str[m_prefixLength+7] = kEncodingTable[((val >> 36) & 0x3F)];
		str[m_prefixLength+8] = kEncodingTable[((val >> 42) & 0x3F)];
		str[m_prefixLength+9] = kEncodingTable[((val >> 48) & 0x3F)];
		str[m_prefixLength+10] = kEncodingTable[((val >> 54) & 0x3F)];
		str[m_prefixLength+11] = kEncodingTable[((val >> 60) & 0x3F)];
		str[m_prefixLength+12] = '\0';

		return str;				
   }

} // namespace cat
