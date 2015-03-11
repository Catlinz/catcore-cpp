#include "core/io/filedescriptor.h"

namespace Cat {

	FDFileHandle::~FDFileHandle() {
		DMSG("File handle closed.");		
		if (fd) {
			fclose(fd);
			fd = NIL;
		}
	}

	FileDescriptor::~FileDescriptor() {
		if (isOpen()) {
			flush();
		}
		m_pHandle.setNull();
		m_pFile.setNull();
	}

	Boolean FileDescriptor::open(const Char* mode) {
		if (!isValid()) {
			DWARN("Cannot open null or invalid file!");
			return false;
		}
		if (m_pHandle.notNull() && m_pHandle->fd) { 
			if ( StringUtils::equals(m_pHandle->mode, mode) ) {
				DWARN("Cannot open file '" << m_pFile->absolutePath()
						<< "' in mode '" << mode
						<< "' already open in mode '"
						<< m_pHandle->mode << "'!");
			}
			else if (!StringUtils::equals(m_pHandle->mode, "rb")) {
				DWARN("Cannot open file '" << m_pFile->absolutePath()
						<< "' in mode 'rw' already open in mode '"
						<< m_pHandle->mode << "'!");
			}
			else { 
				DWARN("Cannot open file '" << m_pFile->absolutePath()
						<< "' that is already open!");
			}
			return false;
		}
		
		FILE* fd = NIL;
		if (mode) {
			fd = fopen(m_pFile->absolutePath(), mode);
			if (!fd) {
				DWARN("Failed to open file '" << m_pFile->absolutePath()
						<< "' with mode: '" << mode << "'!");
				return false;
			}
			if (m_pHandle.isNull()) {				
				m_pHandle = FDFileHandlePtr(new FDFileHandle(fd, mode));
			} else {
				m_pHandle->fd = fd;
				StringUtils::sub(m_pHandle->mode, mode, 0, StringUtils::length(mode)+1);
			}			
		} else {
			fd = fopen(m_pFile->absolutePath(), "rb");
			if (!fd) {
				DWARN("Failed to open file '" << m_pFile->absolutePath()
						<< "' with DEFAULT mode: 'rb'!");
				return false;
			}
			if (m_pHandle.isNull()) {				
				m_pHandle = FDFileHandlePtr(new FDFileHandle(fd, "rb"));
			} else {
				m_pHandle->fd = fd;
				StringUtils::sub(m_pHandle->mode, "rb", 0, 3);
			}	
		}
		return true;
	}

	void FileDescriptor::flush() {
		if (isOpen()) {
			fflush(m_pHandle->fd);
		} else {
			if (isValid()) {
				DWARN("Attempted to flush closed file '" << m_pFile->absolutePath() << "'!");
			} else {
				DWARN("Attempted to flush null or invalid file!");
			}
		}
	}

	void FileDescriptor::close() {
		if (isOpen()) {
			flush();
			fclose(m_pHandle->fd);
			m_pHandle->fd = NIL;			
			//m_pHandle.setNull();			
		} else {
			if (isValid()) {
				DWARN("Attempted to close closed file '" << m_pFile->absolutePath() << "'!");
			} else {
				DWARN("Attempted to close null or invalid file!");
			}
		}
	}

} // namespace Cat
