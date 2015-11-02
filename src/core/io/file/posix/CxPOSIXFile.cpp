#include "core/io/file/CxFile.h"
#include "core/common/CxMem.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

namespace cat {

	CxFile::CxFile(const CxChar *in_filename)
		: CxIODevice(), mp_fd(0) {
		m_filename = CxMallocRef<CxChar>(sys::getPath(in_filename));
	}

	CxFile::CxFile(const CxChar *in_path, CxFile::UsePathFlag)
		: CxIODevice(), mp_fd(0) {
		m_filename = CxMallocRef<CxChar>(in_path);
	}

	CxFile::CxFile(CxFile &&in_src) : CxIODevice(), mp_fd(0) {
		m_mode = in_src.m_mode;
		m_filename = static_cast<CxMallocRef<CxChar> &&>(in_src.m_filename);
		mp_fd = in_src.mp_fd;

		in_src.mode = CxIODevice::kNotOpen;
		in_src.mp_fd = 0;
	}

	CxFile & CxFile::operator=(CxFile &&in_src) {
		if (mp_fd) { close(); }
		m_mode = in_src.m_mode;
		m_filename = static_cast<CxMallocRef<CxChar> &&>(in_src.m_filename);
		mp_fd = in_src.mp_fd;

		in_src.mode = CxIODevice::kNotOpen;
		in_src.mp_fd = 0;
		return *this;
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

		/* Try and open the new file for writing */
		CxFile nf(path, kUsePath);
		if (!nf.open(CxIODevice::kWrite)) {
			CXD_ERR("Failed to open file '%s' for writing!", path);			
			m_err = nf.error();  return CxFile();
		}
		
		/* Close this file before we start */
		if (mp_fd != 0) { close(); }
		
		/* Open this file for reading */
		if (!open(CxIODevice::kRead)) {
			CXD_ERR("Failed to open file '%s' for reading!", filename());
		   return CxFile();
		}

		/* Read the file and then write to the new file */
		CxChar buf[4096];
		while (!atEnd()) {
			CxI64 to_write = read(buf, 4096);
			if (nf.write(buf, 4096) != to_write) {
				CXD_ERR("Failed to write all the bytes from '%s' to '%s'.",
						  filename(), nf.filename());
				m_err = CxErr::kFileCopyError;
				nf.m_err = CxErr::kFileCopyError;
			}
		}
		nf.close();
		close();
		return nf;
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
			stat buffer;
			if (stat(in_filename, &buffer) == 0) { return CxErr::kTrueCode; }
			else { return CxErr::kFalseCode; }
		}
		else {
			CXD_WARN("Null file cannot exist.");
			return CxErr::kNullError;
		}
	}

	CxBool CxFile::flush() {
		if (mp_fd != 0) {
		   if (fflush(mp_fd) == 0) { return true; }
			else {
				CXD_ERR("Failed to flush file '%s'.", filename());
				setErrorFromInternalCode(CxErr::kIOFlushError);
				clearerr(mp_fd);
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

		CxChar f_mode[4] = {0,0,0,0};
		if (in_mode.isSet(CxIODevice::kAppend)) { f_mode[0] = 'a'; }
		else if (in_mode.isSet(CxIODevice::kTruncate)) { f_mode[0] = 'w'; }
		
		if (in_mode.isSet(CxIODevice::kReadWrite)) {
			if (f_mode[0] == 0) { f_mode[0] = 'r'; }
			f_mode[1] = '+';
		}
		else if (in_mode.isSet(CxIODevice::kRead)) { f_mode[0] = 'r'; }
		else if (in_mode.isSet(CxIODevice::kWrite)) {
			if (f_mode[0] == 0) { f_mode[0] = 'a'; }
		}
		else {
			CXD_ERR("Tried to open file with invalid mode (%d).", in_mode);
			m_err = CxErr::kIOInvalidMode;  return false;
		}

		/* Check for binary or text */
		if (in_mode.isSet(CxIODevice::kBinary)) {
			if (f_mode[1] == 0) { f_mode[1] = 'b'; }
			else { f_mode[2] = 'b'; }
		}

		mp_fd = fopen(filename(), f_mode);
		if (mp_fd) {
			m_mode = in_mode;  return true;
		}
		else {
			CXD_ERR("Failed to open file '%s'.", filename());
			setErrorFromInternalCode(CxErr::kFileOpenError);
			return false;
		}
	}

	CxI64 CxFile::pos() {
		if (mp_fd != 0) {
			CxI64 p = ftell(mp_fd);
			if (p  == -1) { setErrorFromInternalCode(CxErr::kIOSeekError); }
			return p;
		}
		else {
			m_err = CxErr::kFileClosed;  return -1;
		}
	}

	CxI64 CxFile::read_impl(void *out_data, CxI64 in_maxBytes) {
		CxI64 bytes_read = fread(out_data, 1, in_maxBytes, mp_fd);
		/* Check for a read error */
		if (bytes_read != in_maxBytes) {
			if (ferror(mp_fd) != 0) {
				CXD_ERR("Error reading from file '%s'.", filename());
				m_err = CxErr::kIOReadError;
				clearerr(mp_fd);
			}
		}
		return bytes_read;
	}

	CxI64 CxFile::readLine_impl(CxChar *out_data, CxI64 in_maxChars) {
		if (m_mode.isSet(CxIODevice::kBinary)) {
			return readLineBinary_priv(out_data, in_maxChars);
		}
		else if (m_mode.isSet(CxIODevice::kText)) {
			return readLineText_priv(out_data, in_maxChars);
		}
		else {
			CXD_WARN("Unknown mode %d for file %f.", m_mode, filename());
			m_err = CxErr::kIOInvalidMode;
			return -1;
		}
	}

	CxBool CxFile::reset() {
		m_err = 0;
		if (mp_fd) { rewind(mp_fd);  return true; }
		else {
			m_err = CxErr::kFileClosed;  return false;
		}
	}

	CxBool CxFile::seek(CxI64 in_pos) {
		if (mp_fd) {
			if (fseek(mp_fd, in_pos, SEEK_SET) == 0) { return true; }
			else {
				CXD_ERR("Failed to seek in file '%s'.", filename());
				setErrorFromInternalCode(CxErr::kIOSeekError);
				clearerr(mp_fd);  return false;
			}
		}
		else { m_err = CxErr::kFileClosed;  return false; }
	}

	CxI64 CxFile::size() const {
		if (m_filename != 0) {
			stat f_info;
			if  (stat(m_filename.ptr() &f_info) == 0) {
				return f_info.st_size;
			}
			else {
				setErrorFromInternalCode(CxErr::kUnknownError);
				return -1;
			}
		}
		else { return 0; }
	}

	CxI64 CxFile::write_impl(void *in_data, CxI64 in_bytes) {
		CxI64 bytes_written = fwrite(out_data, 1, in_bytes, mp_fd);
		/* Check for a write error */
		if (bytes_read != in_maxBytes) {
			CXD_ERR("Error write to file '%s'.", filename());
			m_err = CxErr::kIOWriteError;
			clearerr(mp_fd);
		}
		return bytes_written;
	}

	CxI64 CxFile::readLineBinary_priv(CxChar *out_data, CxI64 in_maxChars) {
		CxChar *ptr = out_data;
		CxI64 mark = pos();
		CxI64 to_read = (in_maxChars > 128) ? 128 : in_maxBytes;

		/* Read in max 128 char chunks */
		while (to_read > 0) {
			CxI64 chars_read = fread(ptr, sizeof(CxChar), to_read, mp_fd);
			in_maxChars -= chars_read;
			if (chars_read != 0) {
				const CxChar *end = ptr + chars_read;
				while (ptr != end) {
					if (*(ptr++) == '\n') {
						/* Put back to just after newline */
						seek(mark + (CxI64)(ptr - out_data));
						in_maxChars = 0;
						break;
					}
				}
			}
			else { /* if (chars_read == 0) */
				if (ferror(mp_fd) != 0) {
					CXD_ERR("Error reading line from file '%s'.", filename());
					m_err = CxErr::kFileReadError;
					clearerr(mp_fd);
				}
				break;
			}
			to_read = (in_maxChars > 128) ? 128 : in_maxBytes;
		}
		return (CxI64)(ptr - out_data);
	}

	CxI64 CxFile::readLineText_priv(CxChar *out_data, CxI64 in_maxChars) {
		/* Horribly inefficient right now */
		CxI64 chars_read = 0;
		CxChar *c = out_data;
		for (CxI32 i = 0; i < in_maxChars; ++i) {
			if (fread(c, sizeof(CxChar), 1, mp_fd) == sizeof(CxChar)) {
				++chars_read;
				if (*c == '\n') { break; }
			}
			else {
				if (ferror(mp_fd) != 0) {
					CXD_ERR("Error reading line from file '%s'.", filename());
					m_err = CxErr::kFileReadError;
					clearerr(mp_fd);
				}
				break;
			}
		}
		return chars_read;
	}

	void CxFile::setErrorFromInternalCode(CxErr::Code in_default) {
		CxI32 err_no = errno;
		switch (err_no) {
		case EACCES:
		case EAGAIN:
		case EISDIR:
		case EPERM:
			m_err = CxErr::kAccessError;  break;
					
		case ELOOP:
		case ENAMETOOLONG:
		case ENOTDIR:
		case EOVERFLOW:
			m_err = CxErr::kFilePathError;  break;

		case ENOENT:
			m_err = CxErr::kFileNotFoundError;  break;

		case EBADF:
			m_err = CxErr::kInvalidHandle;  break;
			
		case ENOMEM:
		case EDQUOT:
			m_err = CxErr::kOutOfMemory;  break;
			
		case EEXIST:
			m_err = CxErr::kFileExists;  break;

		case EROFS:
		case ETXTBSY:
			m_err = CxErr::kIOWriteError;  break;
			
		case EFAULT:
		case EFBIG:
		case EMFILE:
		case ENFILE:
			m_err = CxErr::kInsufficientResources;  break;
		default: m_err = in_default;  break;
		}
	}
} // namespace cat
