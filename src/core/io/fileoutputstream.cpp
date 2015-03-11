#include <cstdio>
#include "core/io/fileoutputstream.h"
#include "core/io/serialisable.h"


namespace Cat {

	FileOutputStream::FileOutputStream() {
	}

	FileOutputStream::FileOutputStream(const FilePtr& file) {
		m_fileDescriptor = FileDescriptor(file);
		if (!openFileDescriptor()) {
		   if (file.notNull()) {				
				DWARN("Failed to open file '" << file->absolutePath() << "' for output!");
			} else {
				DWARN("Failed to open null file for output.");
			}	
		}
	}

	FileOutputStream::FileOutputStream(const Char* filename) {
		m_fileDescriptor = FileDescriptor(filename);
		if (!openFileDescriptor()) {
			DWARN("Failed to open file '" << filename << "' for output!");
		}
	}

	FileOutputStream::FileOutputStream(FileDescriptor* fd) {
		if (fd) {
			m_fileDescriptor = *fd;
			if (!m_fileDescriptor.isOpen()) {
				if (!openFileDescriptor()) {
				DWARN("Failed to open file '" << m_fileDescriptor.getName() << "' for output!");
				}
			} else if (strcmp(m_fileDescriptor.getMode(), "rb") == 0) {
				DWARN("File '" << m_fileDescriptor.getName() << "' already open in read only mode, all output operations will fail!"); 
			}
		} else {
			DERR("Cannot open null FileDescriptor!");
			m_fileDescriptor = FileDescriptor();
		}
	}

	FileOutputStream::FileOutputStream(const FileOutputStream& src) {
		m_fileDescriptor = src.m_fileDescriptor;
	}

	FileOutputStream& FileOutputStream::operator=(const FileOutputStream& src) {
		m_fileDescriptor = src.m_fileDescriptor;
		return *this;
	}

	FileOutputStream::~FileOutputStream() {
		
	}
	

	Size FileOutputStream::write(const void* buffer, Size toWrite) {
		if (m_fileDescriptor.isOpen()) { 
			return fwrite(buffer, toWrite, 1, m_fileDescriptor.getFileHandle()) * toWrite;	
		} else {
			return 0;
		}
	}

	Size FileOutputStream::write(const void* buffer, Size count, Size size) {
		if (m_fileDescriptor.isOpen()) { 
			return fwrite(buffer, size, count, m_fileDescriptor.getFileHandle()) * size;	
		} else {
			return 0;
		}
	}

	void FileOutputStream::close() {
		m_fileDescriptor.close();
	}

	void FileOutputStream::flush() {
		m_fileDescriptor.flush();
	}

	StreamDescriptor* FileOutputStream::getStreamDescriptor() {
		return (StreamDescriptor*)&m_fileDescriptor;
	}

	Boolean FileOutputStream::canWrite() {
		return m_fileDescriptor.isOpen() && m_fileDescriptor.getFile()->canWrite();
	}

	Size FileOutputStream::writeObject(Serialisable* object) {
		if (m_fileDescriptor.isOpen()) { 
			return object->write(this);
		} else {
			return 0;
		}
	}

	Boolean FileOutputStream::openFileDescriptor() {
		return m_fileDescriptor.open("ab");
	}

} // namespace Cat

