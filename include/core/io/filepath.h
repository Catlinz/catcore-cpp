#ifndef CAT_CORE_IO_FILEPATH_H
#define CAT_CORE_IO_FILEPATH_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file filepath.h
 * @brief Contains the definition for the FilePath class.
 *
 * @author Catlin Zilinski
 * @date Mar 30, 2014
 */

#include "core/corelib.h"
#if defined(OS_UNIX) || defined(OS_APPLE)
#include <unistd.h>
#endif


#include "core/string/stringutils.h"

namespace Cat {

	/**
	 * @class FilePath filepath.h "core/io/filepath.h"
	 * @brief A class representing a file path on disk.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 30, 2014
	 */
	class FilePath {
	  public:
		/**
		 * @brief Create an empty FilePath.
		 */
		inline FilePath() :
			m_pAbsolutePath(NIL), m_pPath(NIL),
			m_pParentPath(NIL), m_pCanonicalPath(NIL),
			m_nameIdx(-1) {}

		/**
		 * @brief Create a FilePath from the passed in path.
		 *
		 * The path passed in can be either relative or absolute.  If the path 
		 * is relative, then the absolute path is calculated from the current
		 * working directory.
		 *
		 * @param path The relative or absolute path.
		 */
		FilePath(const Char* path) {
			m_pAbsolutePath = FilePath::createAbsolutePath(path, NIL);			
			m_pPath = StringUtils::copy(path);
			m_pParentPath = m_pCanonicalPath = NIL;			
			setNameIndex();
		}

		/**
		 * @brief Create a FilePath from a parent path and relative path.
		 *
		 * The parentPath can be either an absolute or relative path.  
		 * If it is relative, then the absolute path is calculated from 
		 * the current working directory.
		 *
		 * @param parentPath The absolute or relative parent path.
		 * @param relativePath The path relative to the parent path.
		 */
		FilePath(const Char* parentPath, const Char* relativePath) {
			m_pAbsolutePath = FilePath::createAbsolutePath(relativePath, parentPath);
			m_pPath = StringUtils::copy(relativePath);
			m_pParentPath = m_pCanonicalPath = NIL;
			setNameIndex();
		}

		/**
		 * @brief Copy constructor just copies all the string data.
		 * @param src The FilePath to copy from.
		 */
		FilePath(const FilePath& src);

		/**
		 * @brief Destructor destroys all strings.
		 */
		~FilePath();

		/**
		 * @brief Copy Constructor, copies all string data.
		 * @param src The FilePath to copy from.
		 * @return A reference to this FilePath.
		 */
		FilePath& operator=(const FilePath& src);

		/**
		 * @brief The overloaded equality operator, just compares the two paths.
		 * @param other The other path to compare to this one.
		 * @return True if the two paths are equal.
		 */
		inline Boolean operator==(const FilePath& other) const {
			return StringUtils::equals(getCanonicalPath(), other.getCanonicalPath());
		}		

		/**
		 * @brief Get the absolute file path.
		 * @return The absolute file path.
		 */
		inline const Char* absolutePath() const { return m_pAbsolutePath; }

		/**
		 * @brief Get the canonical path.
		 * @return The canonical path.
		 * @todo IMPLEMENT THIS METHOD.
		 */
		inline const Char* getCanonicalPath() const { return absolutePath(); }		
		
		/**
		 * @brief Get the parent portion of the path.
		 * @return The parent portion of the path.
		 */
		const Char* getParentPath();
				
		/**
		 * @brief Get the name portion of the file path.
		 * @return The name portion of the file path.
		 */
		inline const Char* name() const {
			if (m_nameIdx >= 0) {
				return &(m_pAbsolutePath[m_nameIdx]);
			}
			else { return NIL; }
		}

		/**
		 * @brief Get the path that was specified to create this FilePath.
		 * @return The path used to create this FilePath.
		 */
		inline const Char* path() const { return m_pPath; }

		/**
		 * @brief Rename the name of the file / dir referenced by the file path.
		 * @param name The name to rename the file / dir to.
		 * @return True if the name changed, false otherwise.
		 */
		Boolean rename(const Char* name);		

		/**
		 * @brief Set a FilePath from a parent path and relative path.
		 *
		 * The parentPath can be either an absolute or relative path.  
		 * If it is relative, then the absolute path is calculated from 
		 * the current working directory.
		 *
		 * @param parentPath The absolute or relative parent path.
		 * @param relativePath The path relative to the parent path.
		 */
		void set(const Char* parentPath, const Char* relativePath);

		/**
		 * @brief Set the FilePath from the passed in path.
		 *
		 * The path passed in can be either relative or absolute.  If the path 
		 * is relative, then the absolute path is calculated from the current
		 * working directory.
		 *
		 * @param path The relative or absolute path.
		 */
		inline void set(const Char* path) { set(NIL, path); }		
		
		/**
		 * @brief The method to get an absolute path string from a possibly relative path.
		 * @param path The absolute or relative end of the path.
		 * @param parentPath The (optional) absolute or relative parent path.
		 * @return An absolute path string.
		 */
		static Char* createAbsolutePath(const Char* path, const Char* parentPath);

		/**
		 * @brief Get the current working directory.
		 * @return The current working directory path.
		 */
		static inline Char* getCwd() {
			Char* cwd = StringUtils::create(255);
			IF_UNIX_OR_APPLE(getcwd(cwd, 256));
			return cwd;
		}		

		/**
		 * @brief Test to see if a path is absolute or relative.
		 * @param path The path string to test for absoluteness.
		 * @return True if the path is absolute.
		 */
		inline static Boolean isAbsolutePath(const Char* path) {
			return ((path != NIL) &&
					  StringUtils::find(path, FilePath::rootDirName()) == path);	
		}

		/**
		 * @brief Get the separator character for file paths.
		 * @return The separator character for file paths.
		 */
		inline static Char separatorChar() {
			IF_UNIX_OR_APPLE(return '/');
			IF_WINDOWS(return '\\');
		}

		/**
		 * @brief Get the separator character for lists of file paths.
		 * @return The separator character for lists of file paths.
		 */
		inline static Char pathSeparatorChar() {
			IF_UNIX_OR_APPLE(return ':');
			IF_WINDOWS(return ';');
		}

		/**
		 * @brief Get the root directory string.
		 * @return The root directory string.
		 */
		inline static const Char* rootDirName() {
			IF_UNIX_OR_APPLE(return "/");
			IF_WINDOWS(return "C:\\");
		}

		/**
		 * @brief Get the length of the root directory string.
		 * @return The length of the root directory string.
		 */
		inline static I32 rootDirNameLength() {
			IF_UNIX_OR_APPLE(return 1);
			IF_WINDOWS(return 3);
		}
		
	  private:
		/**
		 * @brief Determine the index into the absolute path for the name.
		 */
		void setNameIndex();
		
		Char* m_pAbsolutePath;
		Char* m_pPath;
		Char* m_pParentPath;
		Char* m_pCanonicalPath;
		I32 m_nameIdx;

	};

} // namespace Cat

#endif // CAT_CORE_IO_FILEPATH_H

