#include <cstdio>
#include <cstring>

#include "core/io/fileinputstream.h"
#include "core/io/file.h"
#include "core/io/serialisable.h"


namespace Cat {

	FileInputStream::FileInputStream() {
	}

	FileInputStream::FileInputStream(const FilePtr& file) {
		m_fileDescriptor = FileDescriptor(file);
		if (!openFileDescriptor()) {
			if (file.notNull()) {				
				DWARN("Failed to open file '" << file->absolutePath() << "' for input!");
			} else {
				DWARN("Failed to open null file for input.");
			}			
		}
	}

	FileInputStream::FileInputStream(const Char* filename) {
		m_fileDescriptor = FileDescriptor(filename);
		if (!openFileDescriptor()) {
			DWARN("Failed to open file '" << filename << "' for input!");
		}
	}

	FileInputStream::FileInputStream(FileDescriptor* fd) {
		if (fd) {
			m_fileDescriptor = *fd;
			if (!m_fileDescriptor.isOpen()) {
				if (!openFileDescriptor()) {
				DWARN("Failed to open file '" << m_fileDescriptor.getName() << "' for input!");
				}
			} else if (strcmp(m_fileDescriptor.getMode(), "rb") != 0) {
				DWARN("File '" << m_fileDescriptor.getName() << "' open in mode '" << m_fileDescriptor.getMode() << "', input operations may fail!"); 
			}
		} else {
			DERR("Cannot open null FileDescriptor!");
		}
	}

	FileInputStream::FileInputStream(const FileInputStream& src) {
		m_fileDescriptor = src.m_fileDescriptor;
	}	

	FileInputStream& FileInputStream::operator=(const FileInputStream& src) {
		m_fileDescriptor = src.m_fileDescriptor;
		return *this;
	}

	FileInputStream::~FileInputStream() {

	}
	

	Size FileInputStream::read(void* buffer, Size toRead) {
		if (m_fileDescriptor.isOpen()) { 
			return fread(buffer, toRead, 1, m_fileDescriptor.getFileHandle()) * toRead;	
		} else {
			return 0;
		}
	}

	Size FileInputStream::read(void* buffer, Size count, Size size) {
		if (m_fileDescriptor.isOpen()) { 
			return fread(buffer, size, count, m_fileDescriptor.getFileHandle()) * size;	
		} else {
			return 0;
		}
	}

	void FileInputStream::close() {
		m_fileDescriptor.close();
	}

	Size FileInputStream::skip(Size bytes) {
		if (m_fileDescriptor.isOpen()) {
			Size current_pos = ftell(m_fileDescriptor.getFileHandle());
			fseek(m_fileDescriptor.getFileHandle(), bytes, SEEK_CUR);
			Size new_pos = ftell(m_fileDescriptor.getFileHandle());

			// Return the actual amount we skipped over.
			return new_pos - current_pos;
		} else {
			return 0;
		}
	}

	Size FileInputStream::rewind(Size bytes) {
		if (m_fileDescriptor.isOpen()) {
			Size current_pos = ftell(m_fileDescriptor.getFileHandle());

			// Get where we want to go, and make sure its >= 0
			Size wanted_pos = current_pos - bytes;
			if (wanted_pos < 0) {
				wanted_pos = 0;
			}
			fseek(m_fileDescriptor.getFileHandle(), wanted_pos, SEEK_SET);
			
			// return the amount of bytes we rewound over.
			Size new_pos = ftell(m_fileDescriptor.getFileHandle());
			return current_pos - new_pos;
		} else {
			return 0;
		}
	}

	Boolean FileInputStream::isPositionable() const {
		return true;
	}

	StreamDescriptor* FileInputStream::getStreamDescriptor() {
		return (StreamDescriptor*)&m_fileDescriptor;
	}

	Boolean FileInputStream::canRead()  {
		return m_fileDescriptor.isOpen() && m_fileDescriptor.getFile()->canRead() && !feof(m_fileDescriptor.getFileHandle());
	}

	Size FileInputStream::readObject(Serialisable* object) {
		if (m_fileDescriptor.isOpen()) { 
			return object->read(this);
		} else {
			return 0;
		}
	}

	Char* FileInputStream::getContents() {
		if (m_fileDescriptor.isOpen()) {
			Size file_length = m_fileDescriptor.getFile()->getLength();
			Char* contents = new Char[file_length + 1];
			
			Size bytes_read = fread(contents, file_length, 1, m_fileDescriptor.getFileHandle()) * file_length; 

			if (bytes_read == file_length) {
				contents[bytes_read] = '\0';
				return contents;
			} 
			else {
				delete[] contents;
				DERR("Error reading file contents from file '" << m_fileDescriptor.getName() << "'!");
				return NIL;
			}
		} else {
			return NIL;
		}
	}

	UByte* FileInputStream::getData() {
		if (m_fileDescriptor.isOpen()) {
			Size file_length = m_fileDescriptor.getFile()->getLength();
			UByte* contents = new UByte[file_length];
			
			Size bytes_read = fread(contents, file_length, 1, m_fileDescriptor.getFileHandle()) * file_length; 

			if (bytes_read == file_length) {
				return contents;
			} 
			else {
				delete[] contents;
				DERR("Error reading file contents from file '" << m_fileDescriptor.getName() << "'!");
				return NIL;
			}
		} else {
			return NIL;
		}
	}


	Boolean FileInputStream::openFileDescriptor() {
		return m_fileDescriptor.open("rb");
	}

} // namespace Cat
