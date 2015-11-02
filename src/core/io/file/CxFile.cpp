#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsFile.cpp"
#else // POSIX
#  include "posix/CxPOSIXFile.cpp"
#endif

namespace cat {

	CxFile::~CxFile() { close(); }

	CxFile CxFile::copy(const CxChar *in_srcName, const CxChar *in_dstName) {
		CxFile src(in_srcName);
		return src.copy(in_dstName);
	}

	CxBool CxFile::exists(const CxChar *in_filename) {
		CxChar *path = sys::getPath(in_filename);
		CxErr::Code ret = exists_priv(path);
		mem::free(path);
		return (ret == CxErr::kTrueCode) ? true : false;
	}
	
	void * CxFile::getContents(CxI64 *out_bytes) {
		if (mp_filename != 0) {
			
			CxBool should_close = false;
			if (isOpen()) {
				if (!canRead()) {
					CXD_WARN("Cannot read from write-only file '%s'.", filename());
					m_err = CxErr::kIOReadError;  return 0;
				}
				else { if (!seek(0)) { return 0; } }
			}
			else {
				if (open(CxIODevice::kRead)) { should_close = true; }
				else { return 0; }
			}

			const CxI64 bytes = size();
			void *data = mem::alloc(bytes);
			CxBool sucess = read_impl(data, bytes);
			if (should_close) { close(); }
			if (success) { return data; }
			else { mem::free(data);  return 0; }
		}
		else {
			CXD_WARN("Cannot get file contents of null file.");
			m_err = CxErr::kNullError; return 0;
		}
	}

	
	CxBool CxFile::readAll(void *out_data, CxI64 in_maxBytes) {
		if (m_filename == 0) {
			CXD_ERR("Cannot read from null file");
			m_err = CxErr::kNullError;  return false;
		}

		if (canRead()) {
			const CxI64 f_size = size();
			if (in_maxBytes < f_size) {
				CXD_WARN("Cannot read contents of file, need %lld space, have %lld.",
							f_size, in_maxBytes);
				m_err = CxErr::kIOInvalidMode;
				return false;
			}
			if (seek(0)) {
				read_impl(out_data, in_maxBytes);  return true;
			}
			else {
				CXD_ERR("Cannot reposition to beginning of file %s.", filename());
				return false;
			}
		}
		else {
			CXD_WARN("Cannot read from file '%s'.", filename());
			m_err = CxErr::kIOReadError;  return false;
		}
	}
	
} // namespace cat
