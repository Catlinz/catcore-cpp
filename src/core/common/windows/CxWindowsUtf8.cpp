#include <Windows.h>

namespace cat {

	CxU8 * utf8::fromWideChars(const wchar_t *CX_RESTRICT in_src) {
		CxU8 *buf = 0;
		CxI32 req_bytes = WideCharToMultiByte(CP_UTF8, 0, in_src, -1, 0, 0, 0, 0);
		if (req_bytes > 0) {
			buf = utf8::alloc(req_bytes);
			WideCharToMultiByte(CP_UTF8, 0, in_src, -1, (LPSTR)buf, req_bytes, 0, 0);
		}
		return buf;
	}


	wchar_t * utf8::toWideChars(const CxU8 *CX_RESTRICT in_src) {
		wchar_t *buf = 0;
		CxI32 req_bytes = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)in_src, -1, 0, 0);
		if (req_bytes > 0) {
			buf = (wchar_t *)mem::alloc(req_bytes*sizeof(wchar_t));
			MultiByteToWideChar(CP_UTF8, 0, (LPCCH)in_src, -1, buf, req_bytes);
		}
		return buf;
	}
} // namespace cat
