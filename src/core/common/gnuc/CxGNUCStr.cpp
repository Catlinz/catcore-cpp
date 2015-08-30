CxI32 str::format(CxChar *inout_str, CxI32 in_dstSize, const CxChar *CX_RESTRICT in_format, ...) {
	va_list args;
	va_start(args, in_format);
	CxI32 nm_chars = vsprintf(inout_str, in_format, args);
	va_end(args);
	return nm_chars;
}

	
CxI32 str::format(CxChar16 *inout_str, CxI32 in_dstSize, const CxChar16 *CX_RESTRICT in_format, ...) {
	va_list args;
	va_start(args, in_format);
	CxI32 nm_chars = vswprintf(inout_str, in_format, args);
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

CxI32 str::scan(const CxChar16 *CX_RESTRICT in_str,
					 const CxChar16 *CX_RESTRICT in_format, ...) {
	va_list args;
	va_start(args, in_format);
	CxI32 nm_args = vswscanf(in_str, in_format, args);
	va_end(args);
	return nm_args;
}
