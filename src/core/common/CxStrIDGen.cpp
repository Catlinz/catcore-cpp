#include "core/common/CxStrIDGen.h"
#include "core/common/CxAtomic.h"

#define CX_MAX_INT_CHAR_LEN 11

namespace cat {

	const CxChar CxStrIDGen::encodingTable[64] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
		'u', 'v', 'w', 'x', 'y', 'z', 
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
		'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
		'U', 'V', 'W', 'X', 'Y', 'Z',
		'+', '/'
	};

	CxStrIDGen::CxStrIDGen(const CxStrIDGen &in_src)
		: mp_prefix(0), m_count(0), m_prefixLn(0), m_maxLn(0) {
	   *this = in_src;
	}

	CxStrIDGen::~CxStrIDGen() {
		str::free(mp_prefix);
		m_prefixLn = m_maxLn = 0;
		m_count = 0;
	}

	CxStrIDGen & CxStrIDGen::operator=(const CxStrIDGen &in_src) {
		str::free(mp_prefix);
		mp_prefix = str::copy(in_src.mp_prefix);
		m_count = in_src.m_count;
		m_prefixLn = in_src.m_prefixLn;
		m_maxLn = in_src.m_maxLn;
		return *this;		
	}

	void CxStrIDGen::initWithPrefix(const CxChar *in_prefix) {
		if (!mp_prefix) {
			mp_prefix = str::copy(in_prefix);
			m_prefixLn = str::len(in_prefix);
			m_maxLn = m_prefixLn + CX_MAX_INT_CHAR_LEN + 1;
			m_count = 0;
		}
		else {
			CXD_WARN("Cannot initialise CxStrIDGen initialised w/ prefix '%s'", mp_prefix);
		}
	}

   CxStrID CxStrIDGen::generate() {
		const CxI32 max_len = m_maxLn;
		const CxI32 p_len = m_prefixLn;
		
		CxChar *str = str::alloc8(m_maxLn);
		mem::copy(str, mp_prefix, sizeof(CxChar)*p_len);

		const CxI64 val = atomic::incr64(m_count);

		CxChar *ptr = str + p_len;
		*ptr = '_';		
		ptr[1] = encodingTable[(val & 0x3F)];
		ptr[2] = encodingTable[((val >> 6) & 0x3F)];
		ptr[3] = encodingTable[((val >> 12) & 0x3F)];
		ptr[4] = encodingTable[((val >> 18) & 0x3F)];
		ptr[5] = encodingTable[((val >> 24) & 0x3F)];
		ptr[6] = encodingTable[((val >> 30) & 0x3F)];
		ptr[7] = encodingTable[((val >> 36) & 0x3F)];
		ptr[8] = encodingTable[((val >> 42) & 0x3F)];
		ptr[9] = encodingTable[((val >> 48) & 0x3F)];
		ptr[10] = encodingTable[((val >> 54) & 0x3F)];
		ptr[11] = encodingTable[((val >> 60) & 0x3F)];
		ptr[12] = 0;

		return CxStrID(str, kCxNoCopy);
   }
	


} // namespace cat
