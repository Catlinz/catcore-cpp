#include "core/io/CxIODevice.h"

namespace cat {

	CxI64 CxIODevice::peek(void *out_data, CxI64 in_maxBytes) {
		const CxI64 cur_pos = pos();
		const CxI64 bytes_read = read(out_data, in_maxBytes);
		seek(cur_pos);
		return bytes_read;
	}

} // namespace cat
