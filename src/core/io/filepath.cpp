#include "core/io/filepath.h"

namespace Cat {

	FilePath::FilePath(const FilePath& src) {
		m_pAbsolutePath = StringUtils::copy(src.m_pAbsolutePath);
		m_pPath = StringUtils::copy(src.m_pPath);
		m_pParentPath = StringUtils::copy(src.m_pParentPath);
		m_pCanonicalPath = StringUtils::copy(src.m_pCanonicalPath);
		m_nameIdx = src.m_nameIdx;
	}

	FilePath::~FilePath() {
		m_pAbsolutePath = StringUtils::free(m_pAbsolutePath);
		m_pPath = StringUtils::free(m_pPath);
		m_pParentPath = StringUtils::free(m_pParentPath);
		m_pCanonicalPath = StringUtils::free(m_pCanonicalPath);
		m_nameIdx = 0;
	}

	FilePath& FilePath::operator=(const FilePath& src) {
		m_pAbsolutePath = StringUtils::free(m_pAbsolutePath);
		m_pPath = StringUtils::free(m_pPath);
		m_pParentPath = StringUtils::free(m_pParentPath);
		m_pCanonicalPath = StringUtils::free(m_pCanonicalPath);
		
		m_pAbsolutePath = StringUtils::copy(src.m_pAbsolutePath);
		m_pPath = StringUtils::copy(src.m_pPath);
		m_pParentPath = StringUtils::copy(src.m_pParentPath);
		m_pCanonicalPath = StringUtils::copy(src.m_pCanonicalPath);
		m_nameIdx = src.m_nameIdx;

		return *this;		
	}

	const Char* FilePath::getParentPath() {
		if (m_pParentPath) {
			return m_pParentPath;
		}
		else if (m_pAbsolutePath) {
			m_pParentPath = StringUtils::create(m_nameIdx);
			StringUtils::sub(m_pParentPath, m_pAbsolutePath, 0, m_nameIdx);
			m_pParentPath[m_nameIdx] = '\0';
			return m_pParentPath;
		}
		else {
			return NIL;
		}
	}

	Boolean FilePath::rename(const Char* name) {
		// If no file or root directory, return false
		if (!m_pAbsolutePath || m_pAbsolutePath[m_nameIdx] == '\0') {
			return false;
		}
		else {
			/* Strip out leading /'s from the name */		
			while (name[0] == FilePath::separatorChar()) {
				++name;
			}

			Size nameLen = StringUtils::length(name);
			I32 diff = nameLen - StringUtils::length(this->name());
			Char* absPath = StringUtils::create(m_nameIdx + nameLen);
			StringUtils::sub(absPath, m_pAbsolutePath, 0, m_nameIdx);
			StringUtils::sub(absPath, name, m_nameIdx, nameLen+1);
			StringUtils::free(m_pAbsolutePath);
			m_pAbsolutePath = absPath;

			/* Fix the path */
			Size pathLen = StringUtils::length(m_pPath);	
			if (diff > 0) { /* Have to make path bigger */
				Char* path = StringUtils::create(pathLen + diff);
				StringUtils::sub(path, m_pPath, 0, pathLen);
				StringUtils::sub(path, name, (pathLen + diff - nameLen), nameLen + 1);
				StringUtils::free(m_pPath);
				m_pPath = path;				
			} else {
				StringUtils::sub(m_pPath, name, (pathLen - (nameLen-diff)), nameLen + 1);
			}
			m_pCanonicalPath = StringUtils::free(m_pCanonicalPath);			
			return true;			
		}
	}
	

	void FilePath::set(const Char* parentPath, const Char* relativePath) {
		if (m_pAbsolutePath) {
			m_pAbsolutePath = StringUtils::free(m_pAbsolutePath);
			m_pPath = StringUtils::free(m_pPath);
			m_pParentPath = StringUtils::free(m_pParentPath);
			m_pCanonicalPath = StringUtils::free(m_pCanonicalPath);
			m_nameIdx = -1;			
		}		
		m_pAbsolutePath = FilePath::createAbsolutePath(relativePath, parentPath);			
		m_pPath = StringUtils::copy(relativePath);
		m_pParentPath = m_pCanonicalPath = NIL;			
		setNameIndex();
	}

	Char* FilePath::createAbsolutePath(const Char* path, const Char* parentPath) {
		if (!parentPath && FilePath::isAbsolutePath(path)) {
			return StringUtils::copy(path);
		}
		else {			
			Boolean deleteParentPathAfter = false;			
			/* Strip out leading /'s from the path */		
			while (path[0] == FilePath::separatorChar()) {
				++path;
			}
			/* Make the parent path absolute if not already */
			if (parentPath && !FilePath::isAbsolutePath(parentPath)) {				
				deleteParentPathAfter = true;				
				/* Strip out leading /'s from the parent path. */
				while (parentPath[0] == FilePath::separatorChar()) {
					++parentPath;
				}
				parentPath = FilePath::createAbsolutePath(parentPath, NIL);				
			}
			else if (!parentPath) { /* No parent path */
				parentPath = FilePath::getCwd();
			}

			Size parentLen = StringUtils::length(parentPath);
			/* Strip out trailing /'s from the parent path. */
			while (parentPath[parentLen-1] == FilePath::separatorChar()) {
				--parentLen;				
			}
			Size pathLen = StringUtils::length(path);			
			/* Now we can allocate space and start copying */
			Char* absPath = StringUtils::create(pathLen + parentLen + 1);
			StringUtils::sub(absPath, parentPath, 0, parentLen);
			absPath[parentLen] = FilePath::separatorChar();
			StringUtils::sub(absPath, path, parentLen+1, pathLen+1);

			if (deleteParentPathAfter) {
				StringUtils::free(const_cast<Char*>(parentPath));
			}
			return absPath;			
		}
	}	

	void FilePath::setNameIndex() {
		// Get the name pointer from the absolute path.
		if (m_pAbsolutePath) {
			m_nameIdx = StringUtils::lastIndexOf(m_pAbsolutePath,
															 FilePath::separatorChar());						
			if (m_nameIdx >= 0) {
				if (m_pAbsolutePath[m_nameIdx+1] == '\0') { /* Dir name */					
					m_pAbsolutePath[m_nameIdx] = '\0';
					Size idxOf = StringUtils::lastIndexOf(m_pAbsolutePath,
																	  FilePath::separatorChar());
					m_pAbsolutePath[m_nameIdx] = FilePath::separatorChar();
					m_nameIdx = idxOf;
				}									
				++m_nameIdx;
			}
			if (m_nameIdx < FilePath::rootDirNameLength()) {
				m_nameIdx = FilePath::rootDirNameLength();				
			}
		} else {
			m_nameIdx = -1;
		}
	}
	

} // namespace Cat
