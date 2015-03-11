#include "core/io/datainputstream.h"
#include "core/string/stringutils.h"

namespace Cat {

	Size DataInputStream::readCStr(CStr string) {
		U32 len = 0;
		Size bytesRead = 0;

		bytesRead += read(&len, sizeof(U32));
		if (bytesRead > 0) {
			bytesRead += read(string, sizeof(Char)*len);
			D_CONDERR((bytesRead - sizeof(U32) < len),
						 "Failed to read all of string!");
			string[bytesRead - sizeof(U32)] = '\0';
		}
		return bytesRead;
	}
	

} // namespace Cat
