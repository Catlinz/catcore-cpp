#ifndef CAT_CORE_IO_UNIX_FILE_H
#define CAT_CORE_IO_UNIX_FILE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file file.h
 * @brief Contains the definition of the File class. 
 *
 * The File class represents a file and contains methods for file management.
 *
 * @author Catlin Zilinski
 * @date Mar 30, 2014
 */

#include "core/threading/atomic.h"
#include "core/util/invasivestrongptr.h"
#include "core/io/filepath.h"

namespace Cat {

	/**
	 * @class File file.h "core/io/file.h"
	 * @brief A class representing a file in the file system.
	 *
	 * The File class represents a file in the file system and contains methods 
	 * for file managment.
	 *
	 * @author Catlin Zilinski
	 * @version 3
	 * @since Sept 30, 2013
	 */
	class File {
	  public:
		/**
		 * @brief Create an empty file with no filename associated with it.
		 */
		inline File()
			: m_deleteFileOnDestroy(false) {}

		/**
		 * @brief Create a file corresponding to the specified filename.
		 * @param filename The filename of the file to associate with.
		 */
		inline File(const Char* filename)
			: m_deleteFileOnDestroy(false) {
		   m_path.set(filename);			
		}

		/**
		 * @brief Create a file corresponding to the specified filename inside the parent path.
		 * @param parentPath The parent path to prepend to the filename. (Must be absolute path)
		 * @param filename The filename relative to the parent path.
		 */
		File(const Char* parentPath, const Char* filename)
			: m_deleteFileOnDestroy(false) {
			m_path.set(parentPath, filename);
		}

		/**
		 * @brief Create a File from a FilePath.
		 * @param path The FilePath to create the File from.
		 */
		File(const FilePath& path)
			: m_deleteFileOnDestroy(false), m_path(path) {}		

		/**
		 * @brief Copy constructor, makes sure to copy filename strings.
		 * @param file The source filei to copy from.
		 */
		inline File(const File& file) {
			m_deleteFileOnDestroy = file.m_deleteFileOnDestroy;
			m_path = file.m_path;
		}		

		/**
		 * @brief Assignment operator, does same as copy constructor.
		 * @param file The source File to copy from.
		 */
		inline File& operator=(const File& file) {
			m_deleteFileOnDestroy = file.m_deleteFileOnDestroy;
			m_path = file.m_path;
			return *this;			
		}

		/**
		 * @brief Free's any memory used by the File object.
		 */
		~File();

		/**
		 * @brief Test whether or not the file can be read from.
		 * @return True if the file can be read from.
		 */
		Boolean canRead() const;

		/**
		 * @brief Test whether or not the file can be written to.
		 * @return True if the file can be written to.
		 */
		Boolean canWrite() const;

		/**
		 * @brief Creates the file associated with this File object if it doesn't exist.
		 * @param create_path Whether or not to also create the file path if it doesn't exist.
		 * @return True if the file was created successfully, false if it failed to be created or already exists.
		 */
		Boolean create(Boolean create_path = false);

		/**
		 * @brief Creates the directory associated with this File object if it doesn't exist.
		 * @param create_path Whether or not to also create the directory path if it doesn't exist.
		 * @return True if the directory was created successfully, false if it failed to be created or already exists.
		 */
		Boolean makeDir(Boolean create_path = false);

		/**
		 * @brief Deletes the current File.
		 * @return True if the file was deleted successfully.
		 */
		Boolean remove();

		/**
		 * @brief Sets whether or not to remove the file when the File object is destroyed.
		 * @param removeFileOnDestroy Whether or not to remove the file when the File object is destroyed.
		 */
		inline void setRemoveFileOnDestroy(Boolean removeFileOnDestroy) {
			m_deleteFileOnDestroy = removeFileOnDestroy;
		}	  

		/**
		 * @brief Empties the contents of the file if it exists.
		 * @return true if the file was successfully emptied.
		 */
		Boolean empty();

		/**
		 * @brief Tests whether two File object are equal or not. (reference the same filename)
		 * @param other The file to test equality against.
		 * @return True if the two File objects are equal, false otherwise.
		 */
		inline Boolean equals(const File *other) const {
			return m_path == other->m_path;			
		}
		inline Boolean equals(const File& other) const {
			return m_path == other.m_path;			
		}		

		/**
		 * @brief Tests to see if the file referenced by the File object exists.
		 * @return True if the file exists.
		 */
		Boolean exists() const;

		/**
		 * @brief Gets the absolute file path (including the filename and extension).
		 * @return The absolute file path of the File.
		 */
		inline const Char* absolutePath() const {
			return m_path.absolutePath();			
		}		

		/**
		 * @brief Get the Canonical file path for the current File object.
		 * @return The Canonical file path.
		 * @todo IMPLEMENT THIS FUNCTION
		 */
		inline const Char* getCanonicalPath() {
			return m_path.getCanonicalPath();			
		}		

		/**
		 * @brief Gets the filename of the file (i.e., no file path).
		 * @return The filename of the File.
		 */
		inline const Char* name() const { return m_path.name(); }		

		/**
		 * @brief Gets the file path of the parent directory if there is one.
		 * @return The file path of the parent directory.
		 */
		inline const Char* getParentPath() {
			return m_path.getParentPath();
		}		

		/**
		 * @brief Gets the File object representing the parent directory of the current File object.
		 * @return The parent File.
		 */
		inline InvasiveStrongPtr<File> getParentFile() {
			if (getParentPath()) {
				return InvasiveStrongPtr<File>(new File(getParentPath()));
			}
			else {
				return InvasiveStrongPtr<File>::nullPtr();
			}
		}		

		/**
		 * @brief Get the path of the current File (not necessarily absolute).
		 * @return The path of the current File.
		 */
		inline const Char* path() const {
			return m_path.path();			
		}

		/**
		 * @brief Get a reference to the FilePath object.
		 * @return A reference to the FilePath object.
		 */
		inline FilePath& filePath() { return m_path; }		

		/**
		 * @brief Tests whether or not the File represents a absolute file path.
		 * @return True if the filename was specified as an absolute file path.
		 */
		inline Boolean isAbsolute() const {
			return FilePath::isAbsolutePath(m_path.path());
		}		

		/**
		 * @brief Tests whether or not the File represents a directory.
		 * @return True if the File represents a directory.
		 */
		Boolean isDirectory() const;

		/**
		 * @brief Tests whether or not the File represents a regular file.
		 * @return True if the File represents a regular file.
		 */
		Boolean isFile() const;

		/**
		 * @brief Tests whether or not the File represents a hidden file.
		 * @return True if the File represents a hidden file.
		 */
		Boolean isHidden() const;

		/**
		 * @brief Gets the last modified timestamp of the File.
		 * @return The last modified timestamp of the File.
		 */
		U64 getLastModified() const;

		/**
		 * @brief Gets the length of the file in bytes.
		 * @return The length of the file in bytes.
		 */
		Size getLength() const;

		/**
		 * @brief Get a list of File objects representing the list of files in a directory.
		 * Gets a list of File objects representing the list of files in a directory.  
		 * Only works if this File object actually represents a directory, otherwise, returns NIL.
		 * @return A list of Files in the directory or NIL if this File is not a directory.
		 */
		File* getFileList();

		/**
		 * @brief Trys to rename the current File to the specified filename.
		 * @param filename The new filename to rename the file to.
		 * @return True if the file was successfully renamed.
		 */
		Boolean rename(const Char *filename);

		/**
		 * @brief Gets the string representation of the File object.
		 * @return A string representation of the File object.
		 */
		inline const Char* toString() const {
			return m_path.absolutePath();
		}		

		/**
		 * @brief Increase the retain count by one.
		 */
		inline void retain() { m_retainCount.increment(); }

		/**
		 * @brief Decrement the retainCount by one.
		 * @return True if there are no more references to the Shader.
		 */
		inline Boolean release() {
			m_retainCount.decrement();			
			if (m_retainCount.val() <= 0) {
				return true;
			}
			return false;
		}

		/**
		 * @brief Get the retain count for the Shader.
		 * @return The Retain count for the shader.
		 */
		inline I32 retainCount() const { return m_retainCount.val(); }


	  private:
		Boolean	 m_deleteFileOnDestroy;
	   FilePath  m_path;		
		AtomicI32 m_retainCount;	

	};

	typedef InvasiveStrongPtr<File> FilePtr;

#if defined (DEBUG)
	inline std::ostream& operator<<(std::ostream& out, const File& file) {
		if (!file.absolutePath()) { return out << "NULL FILE"; }
		else { return out << file.toString(); }		
	}

	inline std::ostream& operator<<(std::ostream& out, const FilePtr& file) {
		if (file.isNull() || !file->absolutePath()) { return out << "NULL FILE"; }
		else { return out << file->toString(); }		
	}
#endif /* DEBUG */

	
} //namespace Cat

#endif // CAT_CORE_IO_UNIX_FILE_H


