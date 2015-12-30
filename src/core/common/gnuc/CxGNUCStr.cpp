
namespace cat {
	CxI32 str::format(CxChar *inout_str, CxI32 in_dstSize, const CxChar *CX_RESTRICT in_format, ...) {
		va_list args;
		va_start(args, in_format);
		CxI32 nm_chars = vsprintf(inout_str, in_format, args);
		va_end(args);
		return nm_chars;
	}

	CxI32 str::scan(const CxChar *CX_RESTRICT in_str,
						 const CxChar *CX_RESTRICT in_format, ...) {
		va_list args;
		va_start(args, in_format);
		CxI32 nm_args = vsscanf(in_str, in_format, args);
		va_end(args);
		return nm_args;
	}
	
} // namespace cat
