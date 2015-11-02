#ifndef CX_CORE_IO_FILE_CXFILE_H
#define CX_CORE_IO_FILE_CXFILE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 * @file CxFile.h
 *
 * Defines the CxFile class.
 *
 * @author Catlin Zilinski
 * @date Oct 15, 2015
 */

#if defined(CX_WINDOWS)
#  include <Windows.h>
#else // POSIX
#  include <stdio.h>
#endif // Windows or POSIX
#include "core/Cx.h"
#include "core/common/CxMallocRef.h"
#include "core/io/CxIODevice.h"

namespace cat {

	/**
	 * @class CxFile CxFile.h "core/io/file/CxFile.h"
	 * @brief A basic File object class.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 15, 2015
	 */
	class CxFile {
	  public:
		/** @brief Enum of file permission flags */
		enum Permission {
			ExecOther = 1 << 0,
			WriteOther = 1 << 1,
			ReadOther = 1 << 2,

			ExecGroup = 1 << 3,
			WriteGroup = 1 << 4,
			ReadGroup = 1 << 5,

			ExecOwner = 1 << 6,
			WriteOwner = 1 << 7,
			ReadOwner = 1 << 8
		};
		CX_FLAGS(Permission, Permissions);

		/** @brief Flags to describe how to handle file handles. */
		enum FileHandleFlag {
			kCloseHandle = 0, /**< File handles passed in will be closed with close() */
			kDontCloseHandle = 1  /**< File handles passed in will only be flushed by close() */
		};

		/** @brief Flags to pass to the constructor (used internally) */
		enum UsePathFlag { kUsePath };
		
		/** @brief Null file constructor. */
		CX_INLINE CxFile() : CxIODevice(), mp_fd(0) {}

		/** @brief Use the specified filename for the CxFile. */
		CxFile(const CxChar *in_filename);

		/** @brief Use the specified filepath for the CxFile (don't copy or transform). */
		CxFile(const CxChar *in_path, UsePathFlag);
		
		/** @brief Move constructor. */
		CxFile(CxFile &&in_src);

		/** @brief Flush the file, then close if (if open in the first place) */
		virtual ~CxFile();

		/** @brief Move-assignment operator. */
		CxFile & operator=(CxFile &&in_src);

		/** @return True if the end of file has been reached (or file closed) */
		virtual CxBool atEnd();

		/** @brief Method to close the IODevice. */
		virtual void close();

		/**
		 * @brief Make a deep copy of the file.
		 * This method copies the current file to a new filename.  This file will
		 * be closed to copy the file.  The copy will fail if the file we are 
		 * copying to already exists.  If the copy partially succeeds, then the 
		 * new file will still be returned, but the error on both files will be 
		 * set to CxErr::kFileCopyError.
		 * @param in_newName The new filename to copy the file to.
		 * @return The new file if the file was copied, else a null file.
		 */
		CxFile copy(const CxChar *in_filename);

		/**
		 * @brief Make a deep copy of a file.
		 * @see copy(CxChar *)
		 * @return The new file if the file was copied, else a null file.
		 */
		static CxFile copy(const CxChar *in_srcName, const CxChar *in_dstName);

		/** @return True if the file specified by the path/filename exists. */
		static CxBool exists(const CxChar *in_filename);
		
		/** @return True if the file exists. */
		CxBool exists() const;

		/** @return The name (full path) of the file */
		CX_FORCE_INLINE const CxChar * filename() const {
			return m_filename.ptr();
		}

		/**
		 * @brief Flush the output from a device if the device supports it.
		 * @return True if the flush was successful.
		 */
		virtual CxBool flush();

		/**
		 * @brief Allocate and return entire file contents.
		 * @see readAll()
		 * Will fail if open in a write only mode, or if cannot open for 
		 * reading.  If already open, will read the contents and leave file
		 * open positioned at the end of the file, if not open, will close it
		 * after reading.
		 * @param out_bytes Optional storage for the number of bytes read (size of file).
		 * @return A newly allocated buffer with the contents, or null.
		 */
		void * getContents(CxI64 *out_bytes = 0);

		/** @return True if the file is open */
		CX_FORCE_INLINE CxBool isOpen() const {
			return m_mode != CxIODevice::kNotOpen;
		}

		/**
		 * @brief Method to open the IODevice for reading and/or writing.
		 * Will fail if the file is already open in a different mode.  If neither
		 * kAppend nor kTruncate are specified, then kWrite will default to 
		 * append and kReadWrite will default to simply reading and writing (no
		 * appending or truncation, and file must exist).
		 * @param in_mode A combination of CxIOMode flags decribing the mode.
		 * @return True if the IODevice was opened successfully.
		 */
		virtual CxBool open(CxIODevice::IOMode in_mode);

		/** 
		 * This method returns the number of bytes from the beginning of
		 * the file for a binary file.  For a text file, the value is not
		 * meaningful, but can still be used to restore the position to a 
		 * previous position using seek.
		 * @return The current position within the device or -1 if error.
		 */
		virtual CxI64 pos();

		/** 
		 * @brief Reset the device to the beginning of the stream. 
		 * @return True if the device was reset properly.
		 */
		virtual CxBool reset();

		/**
		 * @brief Read all the contest of the file into the buffer.
		 * The buffer must have enough space for the entire contents of 
		 * the file.  If not enough room, will fail.  If file is not open
		 * or not readable, will fail.
		 * @param out_data The buffer to store the data into.
		 * @return True if it was successfull, false otherwise.
		 */
		CxBool readAll(void *out_data, CxI64 in_maxBytes);
		
		/**
		 * @brief Move to the given position in the file.
		 * This method takes in either the number of bytes from the start of
		 * the file to move to (binary) or a value from pos() if opened in 
		 * text mode.
		 * @param in_pos The position (from the start of the device) to move to.
		 * @return True if the seek was successfull.
		 */
		virtual CxBool seek(CxI64 in_pos);

		/** @return Get the size of the IODevice in bytes. */
		virtual CxI64 size() const;

	  protected:
		CxMallocRef<CxChar> m_filename;
#if defined(CX_WINDOWS)
		HANDLE mp_fd;
		CxBool m_atEnd;
		CxBool m_eof;
#else // POSIX
	   FILE *mp_fd;
#endif // Windows or POSIX


		virtual CxI64 read_impl(void *out_data, CxI64 in_maxBytes);
		virtual CxI64 readLine_impl(void *out_data, CxI64 in_maxChars);
		virtual CxI64 write_impl(void *in_data, CxI64 in_bytes);

	  private:
#if !defined(CX_WINDOWS)
		CxI64 readLineBinary_priv(CxChar *out_data, CxI64 in_maxChars);
		CxI64 readLineText_priv(CxChar *out_data, CxI64 in_maxChars);
#endif // POSIX
		void setErrorFromInternalCode(CxErr::Code in_default);
		static CxErr::Code exists_priv(const CxChar *in_filename);
		
	};

} // namespace cat

#endif // CX_CORE_IO_FILE_CXFILE_H
