#include "core/common/CxUtf8.h"

namespace cat {

	CxBool utf8::endsWith(const CxU8 *CX_RESTRICT in_str, const CxU8 *CX_RESTRICT in_end) {
		if (in_str == 0 || in_end == 0) { return false; }

		const CxI32 s_bytes = utf8::bytes(in_str);
		const CxI32 e_bytes = utf8::bytes(in_end);

		if (e_bytes <= s_bytes) {
			const CxU8 *CX_RESTRICT s = in_str + s_bytes - 1;
			const CxU8 *CX_RESTRICT e = in_end + e_bytes - 1;
			for (CxI32 i = 0; i < e_bytes; ++i) {
				if (*(s--) != *(e--)) { return false; }
			}
			return true;
		}
		else { return false; }
	}

	CxBool utf8::eq(const CxU8 *CX_RESTRICT in_str1,	const CxU8 *CX_RESTRICT in_str2) {
		/* Check for null strings */
		if (in_str1 == 0 || in_str2 == 0) { return in_str1 == in_str2; }
		while (*(in_str1) != 0) {
			if (*(in_str1++) != *(in_str2++)) { return false; }
		}
		return (*in_str2) == 0;
	}

	CxBool utf8::greater(const CxU8 *CX_RESTRICT in_str1,
							  const CxU8 *CX_RESTRICT in_str2) {
		/* Check for null strings */
		if (in_str1 == 0 || in_str2 == 0) { return in_str1 > in_str2; }
		while (*(in_str2) != 0) {
			const CxU8 c1 = *in_str1++;
			const CxU8 c2 = *in_str2++;
			if (c1 != c2) { return c1 > c2; }
		}
		return *in_str1 > *in_str2;
	}

	CxBool utf8::less(const CxU8 *CX_RESTRICT in_str1,
						  const CxU8 *CX_RESTRICT in_str2) {
		/* Check for null strings */
		if (in_str1 == 0 || in_str2 == 0) { return in_str1 < in_str2; }
		while (*(in_str2) != 0) {
			const CxU8 c1 = *in_str1++;
			const CxU8 c2 = *in_str2++;
			if (c1 != c2) { return c1 < c2; }
		}
		return *in_str1 < *in_str2;
	}

	CxBool utf8::startsWith(const CxU8 *CX_RESTRICT in_str,
								  const CxU8 *CX_RESTRICT in_prefix) {
		if (in_str == 0 || in_prefix == 0) { return false; }
		while (*in_str != 0) {
			const CxU8 c1 = *in_str++;
			const CxU8 c2 = *in_prefix++;
			if (c1 != c2) { return c2 == 0; }
		}
	   return *in_str == *in_prefix;
	}

} // namespace cat

#if defined (CX_WINDOWS)
#  include "windows/CxWindowsUtf8.cpp"
#else
#  include "posix/CxPOSIXUtf8.cpp"
#endif
