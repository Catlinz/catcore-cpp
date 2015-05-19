#include "core/common/CxStr.h"

#if defined(CX_32BIT)
#  define STR_EQ_BITSHIFT 2
#  define STR_EQ_MASK 0x3
#elif defined(CX_64BIT)
#  define STR_EQ_BITSHIFT 3
#  define STR_EQ_MASK 0x7

namespace cat {

	CxChar * str::concat(CxChar *CX_RESTRICT inout_str,
								const CxChar *CX_RESTRICT in_str,
								CxI32 in_len) {
		const int io_str_len = str::len(inout_str);
		memcpy(inout_str + io_str_len, in_str, sizeof(CxChar)*(in_len+1));
		return inout_str;
	}

	CxChar16 * str::concat(CxChar16 *CX_RESTRICT inout_str,
								const CxChar16 *CX_RESTRICT in_str,
								CxI32 in_len) {
		const int io_str_len = str::len(inout_str);
		memcpy(inout_str + io_str_len, in_str, sizeof(CxChar16)*(in_len+1));
		return inout_str;
	}
	
	CxBool str::eq(const CxChar *CX_RESTRICT in_str1,
						const CxChar *CX_RESTRICT in_str2) {
		/* Check for null strings */
		if (in_str1 == 0 || in_str2 == 0) { return in_str1 == in_str2; }
		while (*(in_str1) != 0) {
			if (*(in_str1++) != *(in_str2++)) { return false; }
		}
		return true;
	}

	CxBool str::eq(const CxChar16 *CX_RESTRICT in_str16,
						const CxChar *CX_RESTRICT in_str8) {
		/* Check for null strings */
		if (in_str16 == 0 || in_str8 == 0) { return (CxAddr)in_str16 == (CxAddr)in_str8; }
		while (*(in_str16) != 0) {
			if (*(in_str16++) != (CxChar16)*(in_str8++)) { return false; }
		}
		return true;
	}

	CxBool str::eq(const CxChar16 *CX_RESTRICT in_str1,
						const CxChar16 *CX_RESTRICT in_str2) {
		/* Check for null strings */
		if (in_str1 == 0 || in_str2 == 0) { return in_str1 == in_str2;}
		while (*(in_str1) != 0) {
			if (*(in_str1++) != *(in_str2++)) { return false; }
		}
		return true;
	}

	CxBool str::eq(const CxChar *CX_RESTRICT in_str1,
						const CxChar *CX_RESTRICT in_str2, CxI32 in_len) {
		/* Check for null or empty strings */
		if (in_len == 0) { return true; }

		const CxAddr *CX_RESTRICT ptr1 = reinterpret_cast<const CxAddr *>(in_str1);
		const CxAddr *CX_RESTRICT end1 = reinterpret_cast<const CxAddr *>(in_str1) + ((in_len & (~STR_EQ_MASK)) >> STR_EQ_BITSHIFT);
		const CxAddr *CX_RESTRICT ptr2 = reinterpret_cast<const CxAddr *>(in_str2);

		while(ptr1 != end1) { if (*(ptr1++) != *(ptr2++)) { return false; } }

	   /* Check the end bits of the string */
		for (int i = (in_len & (~STR_EQ_MASK)); i < in_len; ++i) {
			if (in_str1[i] != in_str2[i]) { return false; }
		}
		return true;
	}

} // namespace cat
