#include "core/io/CxIODevice.h"

namespace cat {

	CxBool CxIODevice::atEnd() const {
		CXD_ERR("atEnd() not implemented on this CxIODevice!");
		return false;
	}

	CxBool CxIODevice::flush() {
		CXD_ERR("flush() not implemented on this CxIODevice!");
		return false;
	}
	
	CxI64 CxIODevice::peek(void *out_data, CxI64 in_maxBytes) {
		const CxI64 cur_pos = pos();
		const CxI64 bytes_read = read(out_data, in_maxBytes);
		seek(cur_pos);
		return bytes_read;
	}

	CxI64 CxIODevice::pos() {
		CXD_ERR("pos() not implemented on this CxIODevice!");
		return -1;
	}

	CxI64 CxIODevice::read(void *out_data, CxI64 in_maxBytes) {
		if (m_mode.isSet(CxIODevice::kRead)) {
			return read_impl(out_data, in_maxBytes);
		}
		else {
			CXD_WARN("Cannot read from CxIODevice in mode %d.", m_mode);
			m_err = CxErr::kIOReadError;
			return -1;
		}
	}

	CxI64 CxIODevice::readLine(CxChar *out_data, CxI64 in_maxChars) {
		CXD_IF_ERR(in_maxChars < 2, "maxChars should be at least 2.");
		
		if (m_mode.isSet(CxIODevice::kRead)) {
			/* do in_maxBytes - 1 to reserve for '\0' */
			CxI64 chars_read = readLine_impl(out_data, in_maxBytes - 1);
			
			/* replace \r\n with \n if needed and append \0 */
			CxChar *ptr = out_data + chars_read;
			if (chars_read > 1) {
				if (*(ptr - 2) == '\r') { *(ptr - 2) = '\n';  *(ptr - 1) = '\0'; }
				else if (*(ptr - 1) == '\r') { *(ptr -1 ) = '\0'; }
			}
			*ptr = '\0';
			return chars_read*sizeof(CxChar);
		}
		else {
			CXD_WARN("Cannot read from CxIODevice in mode %d.", m_mode);
			m_err = CxErr::kIOReadError;
			return -1;
		}
	}

	CxBool CxIODevice::reset() {
		CXD_ERR("reset() not implemented on this CxIODevice!");
		return false;
	}

	CxBool CxIODevice::seek(CxI64 in_pos) {
		CXD_ERR("seek(CxI64) not implemented on this CxIODevice!");
		return false;
	}

	CxI64 CxIODevice::size() {
		CXD_ERR("size() not implemented on this CxIODevice!");
		return -1;
	}

	CxI64 CxIODevice::write(void *in_data, CxI64 in_bytes) {
		if (m_mode.isSet(CxIODevice::kWrite)) {
			return write_impl(in_data, in_bytes);
		}
		else {
			CXD_WARN("Cannot write to CxIODevice in mode %d.", m_mode);
			m_err = CxErr::kIOWriteError;
			return -1;
		}
	}

} // namespace cat
