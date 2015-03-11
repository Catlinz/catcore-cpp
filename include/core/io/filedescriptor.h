#ifndef CAT_CORE_IO_FILEDESCRIPTOR_H
#define CAT_CORE_IO_FILEDESCRIPTOR_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file filedescriptor.h
 * @brief Defines the FileDescriptor class to provide a mechanism for opening files.
 *
 * @author Catlin Zilinski
 * @date Mar 30, 2014
 */

#include <cstdio>
#include "core/io/streamdescriptor.h"
#include "core/io/file.h"

namespace Cat {

	/**
	 * @class FDFileHandle
	 * @brief simply holds a FILE* and a reference count to determine if it needs to be closed.
	 */
	class FDFileHandle {
	  public:
		inline FDFileHandle() : fd(NIL) {}
		inline FDFileHandle(FILE* pFd, const char* pMode )
			: fd(pFd) {
			StringUtils::sub(mode, pMode, 0, StringUtils::length(pMode) + 1);
		}		
		~FDFileHandle();

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


		FILE*	fd;
		Char	mode[8];

	  private:
		AtomicI32   m_retainCount;	
	};

	typedef InvasiveStrongPtr<FDFileHandle> FDFileHandlePtr;	

	/**
	 * @class FileDescriptor filedescriptor.h "core/io/filedescriptor.h"
	 * @brief Describes and holds a reference to an open file.
	 *
	 * The FileDescriptor is a simple StreamDescriptor describing an 
	 * open file stream used by input / output streams for reading/writing.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Sept 18, 2013
	 */
	class FileDescriptor : public StreamDescriptor {
	  public:
		/**
		 * @brief Creates an empty, invalid file descriptor.
		 */
		inline FileDescriptor() {}		
			
		/**
		 * @brief Creates a file descriptor from the specified File object.
		 * @param file The File object to create the file descriptor from.
		 */
		inline FileDescriptor(const FilePtr& file)
			: m_pFile(file) {
			m_pHandle = FDFileHandlePtr(new FDFileHandle());			
		}	  
			
		/**
		 * @brief Creates a file descriptor from the specified filename.
		 * @param filename The filename of the file.
		 */
		inline FileDescriptor(const Char *filename) {
			m_pFile = FilePtr(new File(filename));
			m_pHandle = FDFileHandlePtr(new FDFileHandle());
		}		
			
		/**
		 * @brief The copy constructor.
		 */
		inline FileDescriptor(const FileDescriptor &src) {
			m_pHandle = src.m_pHandle;
			m_pFile = src.m_pFile;
		}		

		/**
		 * @brief The assignment operator.  
		 * @param src The FileDescriptor to make a copy of.
		 * @return A reference to this FileDescriptor object.
		 */
		inline FileDescriptor& operator=(const FileDescriptor &src) {
			m_pHandle = src.m_pHandle;
			m_pFile = src.m_pFile;
			return *this;			
		}		

		/**
		 * @brief The destructor deletes the File object (if it was created by the FileDescriptor).
		 * It DOES NOT close the file, so close() must be called prior to the FileDescriptor 
		 * being destroyed if you want this behaviour.
		 */
		virtual ~FileDescriptor();

		/**
		 * @brief Tries to open the file (if not already opened) in the specified read / write mode.
		 * @param mode The read / write mode to open the file in.
		 * @return True if the file was opened successfully.
		 */
		Boolean open(const Char* mode = NIL);

		/**
		 * @brief flushes the file.
		 */
		void flush();

		/**
		 * @brief closes the file.
		 */
		void close();

		/**
		 * @brief Tests to see if the FileDescriptor is valid, i.e., it has a File associated with it.
		 * @return True if the FileDescriptor is valid.
		 */
		inline Boolean isValid() const {
			return (m_pFile.notNull() && m_pFile->path());
		}
		

		/**
		 * @brief Tests to see if the FileDescriptor is open.
		 * @return True if the FileDescriptor is open.
		 */
		inline Boolean isOpen() const {
			return (m_pHandle.notNull() && m_pHandle->fd);
		}
		

		/**
		 * @see StreamDescriptor::isStreamType()
		 */
		inline Boolean isStreamType(StreamTypeEnum type) const {
			return (type == STREAM_TYPE_FILE);
		}
		

		/**
		 * @brief Returns the filename.
		 * @return The filename.
		 */
		inline const Char* getName() const {
			if (m_pFile.notNull()) {
				return m_pFile->absolutePath();
			} else {
				return NIL;
			}
		}		

		/**
		 * @brief Get the File object associated with the file stream.
		 * @return The File object.
		 */
		inline const FilePtr& getFile() { return m_pFile; }		

		/**
		 * @brief Get the file handle representing the file stream.
		 * @return The FILE*.
		 */
		inline FILE* getFileHandle() {
			if (m_pHandle.notNull()) {
				return m_pHandle->fd;
			}
			else {
				return NIL;
			}
		}		

		/**
		 * @brief Get the mode that the current file is open in.
		 * @return The string representing the mode the file is open in.
		 */
		inline const Char* getMode() const {
			if (m_pHandle.notNull()) {
				return m_pHandle->mode;
			}
			else {
				return NIL;
			}
		}		

	  private:
		FDFileHandlePtr	m_pHandle;	/**< The actual file handle */
		FilePtr 			m_pFile;	/**< The file object associated with the file */
			
	};

} // namespace Cat

#endif // CAT_CORE_IO_FILEDESCRIPTOR_H

