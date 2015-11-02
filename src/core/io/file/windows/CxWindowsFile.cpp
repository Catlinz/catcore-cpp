#include "core/io/file/CxFile.h"
#include "core/common/CxMem.h"

namespace cat {

	CX_FORCE_INLINE void CxWindowsFile_seekToEnd(HANDLE in_fd) {
		LARGE_INTEGER dp;
		dp.QuadPart = 0;
		SetFilePointerEx(mp_fd, dp, NULL, FILE_END);
	}
	
	CxFile::CxFile(const CxChar *in_filename)
		: CxIODevice(), mp_fd(INVALID_HANDLE_VALUE), m_atEnd(false), m_eof(true) {
		m_filename = CxMallocRef<CxChar>(sys::getPath(in_filename));
	}

	CxFile::CxFile(const CxChar *in_path, CxFile::UsePathFlag)
		: CxIODevice(), mp_fd(INVALID_HANDLE_VALUE), m_atEnd(false), m_eof(true) {
		m_filename = CxMallocRef<CxChar>(in_path);
	}

	CxFile::CxFile(CxFile &&in_src)
		: CxIODevice(), mp_fd(INVALID_HANDLE_VALUE), m_atEnd(false), m_eof(true) {
		m_mode = in_src.m_mode;
		m_filename = static_cast<CxMallocRef<CxChar> &&>(in_src.m_filename);
		mp_fd = in_src.mp_fd;
		m_atEnd = in_src.m_atEnd;
		m_eof = in_src.m_eof;

		in_src.mode = CxIODevice::kNotOpen;
		in_src.mp_fd = INVALID_HANDLE_VALUE;
		in_src.m_atEnd = false;  in_src.m_eof = true;
	}
	


	CxFile & CxFile::operator=(CxFile &&in_src) {
		if (mp_fd) { close(); }
		m_mode = in_src.m_mode;
		m_filename = static_cast<CxMallocRef<CxChar> &&>(in_src.m_filename);
		mp_fd = in_src.mp_fd;
		m_atEnd = in_src.m_atEnd;
		m_eof = in_src.m_eof;

		in_src.mode = CxIODevice::kNotOpen;
		in_src.mp_fd = INVALID_HANDLE_VALUE;
		in_src.m_atEnd = false;  in_src.m_eof = true;
		return *this;
	}
	
	CxBool CxFile::atEnd() const { return m_eof; }

	void CxFile::close() {
		if (mp_fd != INVALID_HANDLE_VALUE) {
			CloseHandle(mp_fd);
			mp_fd = INVALID_HANDLE_VALUE;
		}
		m_mode = CxIODevice::kNotOpen;
	}

	CxFile CxFile::copy(const CxChar *in_filename) {
		if (m_filename == 0) {
			CXD_ERR("Cannot copy null file.");
			m_err = CxErr::kNullError;  return CxFile();
		}
		else if (in_filename == 0) {
			CXD_ERR("Cannot copy TO null file.");
			m_err = CxErr::kNullError;  return CxFile();
		}
		
		CxChar *path = sys::getPath(in_filename);
		if (exists_priv(path) == CxErr::kTrueCode) {
			CXD_WARN("Cannot copy to '%s', file already exists!", path);
			m_err = CxErr::kFileExists;
			mem::free(path);  return CxFile();
		}

		/* Close this file before we copy it */
		if (mp_fd != INVALID_HANDLE_VALUE) { close(); }

		/* Try and copy the file */
		if (CopyFile(filename(), path, true) != 0) {
			return CxFile(path, kUsePath);
		}
		else {
			CXD_ERR("Failed to copy '%s' to '%s'.", filename(), path);
			setErrorFromInternalCode(CxErr::kFileCopyError);
			mem::free(path);
			return CxFile();
		}
	}

	CxBool CxFile::exists() const {
		CxErr::Code ret = exists_priv(filename());
		if (ret == kTrueCode) { return false; }
		else {
			if (ret != kFalseCode) { m_err = ret; }
			return false;
		}
	}

	CxErr::Code CxFile::exists_priv(const CxChar *in_filename) const {
		if (in_filename != 0) {
			HANDLE h = FindFirstFile(in_filename);
			if (h == INVALID_HANDLE_VALUE) {
				if (GetLastError() != ERROR_FILE_NOT_FOUND) {
					CXD_ERR("Failed to check if file '%s' exists.", in_filename);
				   return CxErr::kUnknownError;
				}
				return CxErr::kFalseCode;
			}
			else {
				FindClose(h);
				return CxErr::kTrueCode;
			}
		}
		else {
			CXD_WARN("Null file cannot exist.");
			return CxErr::kNullError;
		}
	}

	CxBool CxFile::flush() {
		if (mp_fd != 0) {
		   if (FlushFileBuffers(mp_fd) != 0) { return true; }
			else {
				CXD_ERR("Failed to flush file '%s'.", filename());
				setErrorFromInternalCode(CxErr::kIOFlushError);
				return false;
			}
		}
		else {
			m_err = CxErr::kFileClosed;  return false;
		}
	}

	CxBool CxFile::open(CxIODevice::IOMode in_mode) {
		/* Make sure we have file to open */
		if (filename() == 0) {
			CXD_ERR("Cannot open null file.");
			m_err = CxErr::kNullError;  return false;
		}

		if (m_mode != CxIODevice::kNotOpen) {
			if (in_mode == m_mode) { return true; }
			else {
				CXD_ERR("File already open in mode %d.", m_mode);
				m_err = CxErr::kFileOpenError;  return false;
			}
		}

		/* Default to binary mode */
		if (!in_mode.isSet(CxIODevice::kText)) {
			in_mode |= CxIODevice::kBinary;
		}
		DWORD access = 0;
		DWORD creation = 0;
		
		if (in_mode.isSet(CxIODevice::kAppend)) {
			creation |= OPEN_ALWAYS;
		}
		else if (in_mode.isSet(CxIODevice::kTruncate)) {
			creation |= CREATE_ALWAYS;
		}
		
		if (in_mode.isSet(CxIODevice::kReadWrite)) {
		   access = GENERIC_READ | GENERIC_WRITE;
		}
		else if (in_mode.isSet(CxIODevice::kRead)) { access = GENERIC_READ; }
		else if (in_mode.isSet(CxIODevice::kWrite)) { access = GENERIC_WRITE; }
		else {
			CXD_ERR("Tried to open file with invalid mode (%d).", in_mode);
			m_err = CxErr::kIOInvalidMode;  return false;
		}

		mp_fd = CreateFile(filename(), access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
		if (mp_fd != INVALID_HANDLE_VALUE) {
			m_mode = in_mode;
			if (in_mode.isSet(CxIODevice::kAppend)) {
				CxWindowsFile_seekToEnd(mp_fd);
				m_atEnd = true;
			}
			return true;
		}
		else {
			CXD_ERR("Failed to open file '%s'.", filename());
			setErrorFromInternalCode(CxErr::kFileOpenError);
			return false;
		}
	}

	CxI64 CxFile::pos() {
		if (mp_fd != 0) {
			LARGE_INTEGER p;
			if (SetFilePointerEx(mp_fd, 0, &p, FILE_CURRENT) != 0) {
				return (CxI64)p.QuadPart;
			}
			else {
				CXD_ERR("Failed to get the file pointer from file '%s'.", filename());
				setErrorFromInternalCode(CxErr::kIOSeekError);
				return -1;
			}
		}
		else {
			m_err = CxErr::kFileClosed;  return -1;
		}
	}

	CxI64 CxFile::read_impl(void *out_data, CxI64 in_maxBytes) {
		DWORD bytes_read = 0;
		if (ReadFile(mp_fd, out_data, (DWORD)in_maxBytes, &bytes_read, NULL) != 0) {
			if (bytes_read == 0) { m_eof = true; }
		}
		else {
			CXD_ERR("Error reading from file '%s'.", filename());
			setErrorFromInternalCode(CxErr::kIOReadError);
		}
		return (CxI64)bytes_read;
	}

	CxI64 CxFile::readLine_impl(CxChar *out_data, CxI64 in_maxChars) {
		CxChar *ptr = out_data;
		CxI64 mark = pos();
		CxI64 to_read = (in_maxChars > 128) ? 128 : in_maxChars;
		/* Read in max 128 char chunks */
		while (to_read > 0) {
			CxI64 bytes_read = 0;
			if (ReadFile(mp_fd, ptr, sizeof(CxChar)*to_read, &bytes_read, NULL) != 0) {
				total_read += bytes_read;
				in_maxChars -= bytes_read;
				if (bytes_read != 0) {
					const CxChar *end = ptr + bytes_read;
					while (ptr != end) {
						if (*(ptr++) == '\n') {
							/* Put back to just after newline */
							seek(mark + (CxI64)(ptr - out_data));
							in_maxChars = 0;
							break;
						}
					}
				}
				else { /* reached eof */ break; }
			}
			else { /* error reading from file */
				CXD_ERR("Error reading line from file '%s'.", filename());
				setErrorFromInternalCode(CxErr::kFileReadError);
				break;
			}
			to_read = (in_maxChars > 128) ? 128 : in_maxChars;
		}
		return (CxI64)(ptr - out_data);
	}

	CxBool CxFile::reset() { m_err = 0;  seek(0); }

	CxBool CxFile::seek(CxI64 in_pos) {
		if (mp_fd) {
			if (m_mode.isSet(CxIODevice::kAppend) && !m_mode.isSet(CxIODevice::kRead)) {
				CXD_WARN("Cannot seek in Append mode with Write Only file '%s'.", filename());
				m_err = CxErr::kIOSeekError;
				return false;
			}
			LARGE_INTEGER dp;
			dp.QuadPart = in_pos;
			if (SetFilePointerEx(mp_fd, dp, NULL, FILE_CURRENT) != 0) {
				m_atEnd = false;
				return true;
			}
			else {
				CXD_ERR("Failed to seek in file '%s'.", filename());
				setErrorFromInternalCode(CxErr::kIOSeekError);
				return false;
			}
		}
		else { m_err = CxErr::kFileClosed;  return false; }
	}
	
	CxI64 CxFile::size() const {
		if (m_filename != 0) {
			WIN32_FILE_ATTRIBUTE_DATA data;
			if (GetFileAttributesEx(m_filename.ptr(), GetFileExInfoStandard, &data) != 0) {
				LARGE_INTEGER s;
				s.HighPart = data.nFileSizeHigh;  s.LowPart = data.nFileSizeLow;
				return (CxI64)s.QuadPart;
			}
			else {
				setErrorFromInternalCode(CxErr::kUnknownError);
				return -1;
			}
		}
		else { return 0; }
	}

	CxI64 CxFile::write_impl(void *in_data, CxI64 in_bytes) {
		if (m_mode.isSet(CxIODevice::kAppend) && !m_atEnd) {
			CxWindowsFile_seekToEnd(mp_fd);
		}
		DWORD bytes_written = 0;
		if (WriteFile(mp_fd, in_data, (DWORD)in_bytes, &bytes_written, NULL) == 0) {
			CXD_ERR("Error writing to file '%s'.", filename());
			setErrorFromInternalCode(CxErr::kIOWriteError);
		}
		return (CxI64)bytes_written;
	}

	void CxFile::setErrorFromInternalCode(CxErr::Code in_default) {
		DWORD err = GetLastError();
		switch (err) {
		case ERROR_ACCESS_DENIED:
		case ERROR_INVALID_ACCESS:
		case ERROR_WRITE_PROTECT:
		case ERROR_SHARING_VIOLATION:
		case ERROR_LOCK_VIOLATION:
			m_err = CxErr::kAccessError;  break;
					
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			m_err = CxErr::kFileNotFoundError;  break;

		case ERROR_INVALID_NAME:
		case ERROR_BAD_PATHNAME:
		case ERROR_FILENAME_EXCED_RANGE:
		case ERROR_INVALID_DRIVE:
		case ERROR_BUFFER_OVERFLOW:
		case ERROR_PATH_BUSY:
			m_err = CxErr::kFilePathErrror;  break;

		case ERROR_TOO_MANY_OPEN_FILES:
		case ERROR_SHARING_BUFFER_EXCEEDED:
		case ERROR_FILE_TOO_LARGE:
			m_err = CxErr::kInsufficientResources;  break;
			
		case ERROR_NOT_ENOUGH_MEMORY:
		case ERROR_OUTOFMEMORY:
		case ERROR_HANDLE_DISK_FULL:
		case ERROR_DISK_FULL:
			m_err = CxErr::kOutOfMemory;  break;

		case ERROR_INVALID_HANDLE:
		case ERROR_INVALID_TARGET_HANDLE:
		case ERROR_DIRECT_ACCESS_HANDLE:
			m_err = CxErr::kInvalidHandle;  break;

		case ERROR_WRITE_FAULT:
			m_err = CxErr::kIOWriteError;  break;

		case ERROR_READ_FAULT:
			m_err = CxErr::kIOReadError;  break;

		case ERROR_FILE_EXISTS:
		case ERROR_ALREADY_EXISTS:
			m_err = CxErr::kFileExists;  break;

		case ERROR_NEGATIVE_SEEK:
		case ERROR_SEEK_ON_DEVICE:
			m_err = CxErr::kIOSeekError;  break;
			
		default: m_err = in_default;  break;
		}
	}
} // namespace cat
