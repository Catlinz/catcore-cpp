
CxI32 str::format(CxChar *inout_str, CxI32 in_dstSize, const CxChar *CX_RESTRICT in_format, ...) {
	va_list args;
	va_start(args, in_format);
	CxI32 nm_chars = vsprintf_s(inout_str, in_dstSize, in_format, args);
	va_end(args);
	return nm_chars;
}

	
CxI32 str::format(CxChar16 *inout_str, CxI32 in_dstSize, const CxChar16 *CX_RESTRICT in_format, ...) {
	va_list args;
	va_start(args, in_format);
	CxI32 nm_chars = vswprintf_s(inout_str, in_dstSize, in_format, args);
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


// CxI32 str::scan(const CxChar *CX_RESTRICT in_str,
// 					 const CxChar *CX_RESTRICT in_format, ...) {
// 	va_list args;
// 	va_start(args, in_format);
// 	void *a[16];
// 	for(CxI32 i = 0; i < 16; ++i;) {
// 		a[i] = va_arg(args, void *);
// 	}
// 	CxI32 nm_args = sscanf(in_str, in_format,
// 								  a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9],
// 								  a[10], a[11], a[12], a[13], a[14], a[15]);
// 	va_end(args);
// 	return nm_args;
// }
