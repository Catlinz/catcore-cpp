#include "core/io/file/posix/CxPOSIXFile.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

namespace cat {

	CxFile::CxFile(const CxChar *in_filename)
		: CxIODevice(), mp_fd(0) {
		m_filename = CxMallocRef<CxChar>(str::copy(in_filename));
	}

	CxFile::~CxFile() {
	   close();
	}

	CxBool CxFile::atEnd() const {
		return (!mp_fd || (mp_fd && feof(mp_fd) != 0));
	}

	void CxFile::close() {
		if (mp_fd != 0) {
			fclose(mp_fd);
			mp_fd = 0;
		}
		m_mode = CxIODevice::kNotOpen;
	}

	CxFile CxFile::copy(const CxChar *in_filename) {
		if (m_filename == 0) {
			CXD_ERR("Cannot copy null file.");
			m_err = kNullError;  return CxFile();
		}
		if (exists(in_filename)) {
			CXD_WARN("Cannot copy to '%s', file already exists!", in_filename);
			m_err = kExistsError;  return CxFile();
		}

		CxFile nf(in_filename);
		if (!nf.open(CxIODevice::kWrite)) {
			CXD_ERR("Failed to open file '%s' for writing!", in_filename);			
			m_err = kOpenError;  return CxFile();
		}
		/* Close this file before we start */
		CxIODevice::IOMode m = m_mode;
		CxI64 f_pos = pos();
		if (mp_fd != 0) { close(); }
		
		/* Open for reading */
		if (!open(CxIODevice::kRead)) {
			CXD_ERR("Failed to open file '%s' for writing!", m_filename.ptr());
			m_err = kOpenError;  return CxFile();
		}
		
		
		
		
	}

	CxBool CxFile::exists(const CxChar *in_filename) {
		if (in_filename != 0) {
			stat buffer;
			return (stat(in_filename, &buffer) == 0);
		}
		else { return false; }
	}

	CxBool CxFile::flush() {
		if (mp_fd != 0) {
		   if (fflush(mp_fd) == 0) { return true; }
			else {
				m_err = kFlushError;  return false;
			}
		}
		else {
			m_err = kClosedError;  return false;
		}
	}

	CxI64 CxFile::pos() {
		if (mp_fd != 0) { return ftell(mp_fd); }
		else {
			m_err = kClosedError;  return -1;
		}
	}

	CxBool CxFile::reset() {
		if (mp_fd) { rewind(mp_fd);  return true; }
		else {
			m_err = kClosedError;  return false;
		}
	}

	CxBool CxFile::seek(CxI64 in_pos) {
		if (mp_fd) {
			if (fseek(mp_fd, in_pos, SEEK_SET) == 0) { return true; }
			else { m_err = kSeekError;  return false; }
		}
		else { m_err = kClosedError;  return false; }
	}

	CxI64 CxFile::size() const {
		if (m_filename != 0) {
			stat f_info;
			if  (stat(m_filename.ptr() &f_info) == 0) {
				return f_info.st_size;
			}
			else {
				CxI32 err_no = errno;
				switch (err_no) {
				case EACCES: m_err = kAccessError;  break;
					
				case ELOOP:
				case ENAMETOOLONG:
				case ENOENT:
				case ENOTDIR: m_err = kPathError;  break;
					
				case EBADF:
				case EFAULT:
				case ENOMEM:
				case EOVERFLOW: m_err = kUnknownError;  break;
				default: m_err = kUnknownError;  break;
				}
				return 0;
			}
		}
		else { return 0; }
	}

	CxBool CxFile::open(CxIODevice::IOMode in_mode) {
		
	}
} // namespace cat
