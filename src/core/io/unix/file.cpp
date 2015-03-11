#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include "core/io/unix/file.h"

namespace Cat {

	File::~File() {
		// If we have a file and want to destroy it, make it so!
		if (m_deleteFileOnDestroy && m_path.absolutePath()) {
			if (!remove()) {
				DERR("Could not remove file '" << m_path.absolutePath() << " on destroy!");
			}
		}
	}

	Boolean File::canRead() const {
		return ((m_path.absolutePath() != NIL) &&
				  (access(m_path.absolutePath(), R_OK) == 0));	
	}	

	Boolean File::canWrite() const {
		return ((m_path.absolutePath() != NIL) &&
				  (access(m_path.absolutePath(), W_OK) == 0));		
	}

	Boolean File::create(Boolean create_path) {
		if (!m_path.absolutePath()) {
			DWARN("Cannot create file with no filename!");
			return false;
		}
		if (!exists()) {
			if (!create_path) {
				FILE* file = fopen(m_path.absolutePath(), "w+b");
				if (!file) { return false; }
				fclose(file);
				return true;
			} else {
				Char* path = StringUtils::copy(m_path.absolutePath());
				Size len = StringUtils::length(path);
				Size idx = len-1;			
				/* Find the closest created directory */
				for (;idx > (Size)FilePath::rootDirNameLength(); --idx) {
					if (path[idx] == FilePath::separatorChar()) {
						path[idx] = '\0';
						if (access(path, F_OK) == 0) {
							path[idx] = FilePath::separatorChar();
							break;
						}
					}
				}
				/* Create all the needed directories */
				for (; idx < len; ++idx) {
					if (path[idx] == '\0') {
						if (mkdir(path, S_IRWXU) != 0) {
							DERR("Failed to create directory " << path << "!");
							StringUtils::free(path);
							return false;							
						}
						else {
							path[idx] = FilePath::separatorChar();
						}
					}					
				}
				StringUtils::free(path);				
				return create(false);				
			}
		}
		else {
			return false;
		}
	}

	Boolean File::makeDir(Boolean create_path) {
		if (!m_path.absolutePath()) {
			DWARN("Cannot create directory with no filename!");
			return false;
		}
		if (!create_path) {
			return (mkdir(m_path.absolutePath(), S_IRWXU) == 0) ; 			 
		} 
		/** @todo: implement recursive path creation. */
		else {
			return (mkdir(m_path.absolutePath(), S_IRWXU) == 0) ; 	
		}
		return false;

	}

	Boolean File::remove() {
		// If there is no file, just return false.
		if (!m_path.absolutePath()) {
			DWARN("Cannot remove file with no filename!");
			return false;
		}
		
		// Try and remove the file or directory.
		if (isDirectory()) {
			return (rmdir(m_path.absolutePath()) == 0);
		}
		else {
			return (unlink(m_path.absolutePath()) == 0);
		}
	}

	Boolean File::empty() {
		if (m_path.absolutePath()) {
			FILE* file = fopen(m_path.absolutePath(), "wb");
			if (file) {
				fflush(file);
				fclose(file);
				return true;
			} 
			else {
				return false;
			}
		} 
		else {
			return false;
		}
	}
	
	Boolean File::exists() const {
		return ((m_path.absolutePath() != NIL) &&
				  (access(m_path.absolutePath(), F_OK) == 0));		
	}

	Boolean File::isDirectory() const {
		struct stat buf;
		if (!m_path.absolutePath() || stat(m_path.absolutePath(), &buf) != 0) {
			return false;
		}
		return S_ISDIR(buf.st_mode);

	}

	Boolean File::isFile() const {
		struct stat buf;
		if (!m_path.absolutePath() || stat(m_path.absolutePath(), &buf) != 0) {
			return false;
		}
		return S_ISREG(buf.st_mode);
	}

	Boolean File::isHidden() const {
	   // If the m_pPath begins with '.' it is a hidden file.
		if (m_path.name() && m_path.name()[0] == '.') {
			return true;
		}
		else {
			// Otherwise, not a hidden file.
			return false;
		}
	}

	U64 File::getLastModified() const {
		struct stat buf;
		if (!m_path.absolutePath() || stat(m_path.absolutePath(), &buf) != 0) {
			return 0;
		}
		return (Size)buf.st_mtime;

	}

	Size File::getLength() const {
		struct stat buf;
		if (!m_path.absolutePath() || stat(m_path.absolutePath(), &buf) != 0) {
			return 0;
		}
		return (Size)buf.st_size;
	}

	/**
	 * @todo Implement
	 */
	File* File::getFileList() {
		return NIL;
	}

	Boolean File::rename(const Char *filename) {
		// If no file or root directory, return false
		if (!m_path.absolutePath() || !m_path.name()) {
			return false;
		}
		else {
			FilePath path(m_path);
			path.rename(filename);			
			I32 retVal = ::rename(m_path.absolutePath(), path.absolutePath());
			if (retVal == 0) { /* Success */
				m_path = path;
				return true;				
			} else {
				return false;
			}							
		}
	}

} // namespace Cat
