#include "core/common/CxStr.h"
#include <stdarg.h>
#include <stdio.h>

#if defined(CX_32BIT)
#  define STR_EQ_BITSHIFT 2
#  define STR_EQ_MASK 0x3
#elif defined(CX_64BIT)
#  define STR_EQ_BITSHIFT 3
#  define STR_EQ_MASK 0x7
#endif

namespace cat {

	CxBool str::endsWith(const CxChar *CX_RESTRICT in_str,
								const CxChar *CX_RESTRICT in_end) {
		if (in_str == 0 || in_end == 0) { return false; }

		const CxI32 s_len = str::len(in_str);
		const CxI32 e_len = str::len(in_end);

		if (e_len <= s_len) {
			const CxChar *CX_RESTRICT s = in_str + s_len - 1;
			const CxChar *CX_RESTRICT e = in_end + e_len - 1;
			for (CxI32 i = 0; i < e_len; ++i) {
				if (*(s--) != *(e--)) { return false; }
			}
			return true;
		}
		else { return false; }
	}
	
	CxBool str::eq(const CxChar *CX_RESTRICT in_str1,
						const CxChar *CX_RESTRICT in_str2) {
		/* Check for null strings */
		if (in_str1 == 0 || in_str2 == 0) { return in_str1 == in_str2; }
		while (*(in_str1) != 0) {
			if (*(in_str1++) != *(in_str2++)) { return false; }
		}
		return (*in_str2) == 0;
	}

	CxBool str::eq(const CxChar16 *CX_RESTRICT in_str16,
						const CxChar *CX_RESTRICT in_str8) {
		/* Check for null strings */
		if (in_str16 == 0 || in_str8 == 0) { return (CxAddr)in_str16 == (CxAddr)in_str8; }
		while (*(in_str16) != 0) {
			if (*(in_str16++) != (CxChar16)*(in_str8++)) { return false; }
		}
		return (*in_str8) == 0;
	}

	CxBool str::eq(const CxChar16 *CX_RESTRICT in_str1,
						const CxChar16 *CX_RESTRICT in_str2) {
		/* Check for null strings */
		if (in_str1 == 0 || in_str2 == 0) { return in_str1 == in_str2;}
		while (*(in_str1) != 0) {
			if (*(in_str1++) != *(in_str2++)) { return false; }
		}
		return (*in_str2) == 0;
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

	CxBool str::greater(const CxChar16 *CX_RESTRICT in_str1,
						const CxChar16 *CX_RESTRICT in_str2) {
		/* Check for null strings */
		if (in_str1 == 0 || in_str2 == 0) { return in_str1 > in_str2;}
		while (*(in_str1) != 0) {
			if (*in_str1 != *in_str2) { return *in_str1 > *in_str2; }
			++in_str1;  ++in_str2;
		}
		return false;
	}

	CxBool str::less(const CxChar16 *CX_RESTRICT in_str1,
					const CxChar16 *CX_RESTRICT in_str2) {
		/* Check for null strings */
		if (in_str1 == 0 || in_str2 == 0) { return in_str1 < in_str2;}
		while (*(in_str1) != 0) {
			if (*in_str1 != *in_str2) { return *in_str1 < *in_str2; }
			++in_str1;  ++in_str2;
		}
		return (*in_str2) != 0;
	}

	CxBool str::startsWith(const CxChar *CX_RESTRICT in_str,
								  const CxChar *CX_RESTRICT in_prefix) {
		if (in_str == 0 || in_prefix == 0) { return false; }
		while (*in_str != 0) {
			if (*in_str != *in_prefix) { return *in_prefix == '\0'; }
			++in_str;  ++in_prefix;
		}
	   return *in_str == *in_prefix;
	}

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsStr.cpp"
#else
#  include "gnuc/CxGNUCStr.cpp"
#endif
	
} // namespace cat

