#include "core/io/dataoutputstream.h"
#include "core/string/stringutils.h"

namespace Cat {

	Size DataOutputStream::writeCStr(const Char* string) {
		U32 len = StringUtils::length(string);
		Size bytesWritten = 0;		
		if (len > 0) {
			bytesWritten += write(&len, sizeof(U32));
			bytesWritten += write(string, sizeof(Char)*len);

			D_CONDERR((bytesWritten != (len + sizeof(U32))),
						 "Failed to write all of the string!");
		}
		return bytesWritten;
	}
	

} // namespace Cat
