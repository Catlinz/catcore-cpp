#include <wchar.h>

namespace cat {

	CxU8 * utf8::fromWideChars(const wchar_t *CX_RESTRICT in_src) {
		CX_TODO("Check and set LC_CTYPE locale");
		mbstate_t mbs;  mbrlen(0, 0, &mbs);
		CxU8 *buf = 0;
		CxU32 req_bytes = wcsrtombs(0, &in_src, 0, &mbs);
		if (req_bytes > 0) {
			mbrlen(0, 0, &mbs);
			buf = utf8::alloc(req_bytes);
			wcsrtombs((char *)buf, &in_src, req_bytes + 1, &mbs);
		}
		return buf;
	}


	wchar_t * utf8::toWideChars(const CxU8 *CX_RESTRICT in_src) {
		CX_TODO("Check and set LC_CTYPE locale");
		mbstate_t mbs;  mbrlen(0, 0, &mbs);
		wchar_t *buf = 0;
		CxU32 req_bytes = mbsrtowcs(0, (const char **)&in_src, 0, &mbs);
		if (req_bytes > 0) {
			mbrlen(0, 0, &mbs);
			buf = mem::alloc(req_bytes*sizeof(wchar_t));
			mbsrtowcs(buf, (const char **)&in_src, req_bytes + 1, &mbs);
		}
		return buf;
	}
} // namespace cat
