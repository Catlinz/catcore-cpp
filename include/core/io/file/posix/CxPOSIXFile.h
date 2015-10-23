#ifndef CX_CORE_IO_FILE_POSX_CXPOSIXFILE_H
#define CX_CORE_IO_FILE_POSX_CXPOSIXFILE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxPOSIXFile.h
 *
 * Defines the POSIX version of the CxFile class.
 *
 * @author Catlin Zilinski
 * @date Oct 15, 2015
 */

#include <stdio.h>
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

		/** @brief An error enum for files */
		enum FileError {
			kNoError = 0,
			kOpenError = 1,  /**< Failed to open the file */
			kReadError = 2,  /**< Failed to read from the file */
			kWriteError = 3, /**< Failed to write to the file */
			kSeekError = 4,  /**< Failed to change position in the file */
			kAccessError = 5, /**< Failed to access file */
			kExistsError = 6, /**< File already exists */
			kFlushError = 7, /**< Failed to flush a file */
			kClosedError = 8, /**< Tried to do something on a closed file */
			kPathError = 9, /**< Something wrong with the file path */
			kNullError = 10, /**< Tried to do something with null file */
			
			kUnknownError = 20, /**< Something bad happened... */
		};
		
		/** @brief Null file constructor. */
		CX_INLINE CxFile() : CxIODevice(), mp_fd(0) {}

		/** @brief Use the specified filename for the CxFile. */
		CxFile(const CxChar *in_filename);

		/** @brief Flush the file, then close if (if open in the first place) */
		virtual ~CxFile();

		/** @return True if the end of file has been reached (or file closed) */
		virtual CxBool atEnd();

		/** @brief Method to close the IODevice. */
		virtual void close();

		/**
		 * @brief Make a copy of the file.
		 * This method copies the current file to newName.  This file is closed 
		 * before it is copied.  If there already exists a file in the 
		 * destination, then the copy will fail.
		 * @param in_newName The new filename to copy the file to.
		 * @return The new file if the file was copied, else a null file.
		 */
		CxFile copy(const CxChar *in_filename);

		/** @return The last error encountered. */
		CX_FORCE_INLINE FileError error() const { return m_err; }

		/** @return True if the file specified by the path/filename exists. */
		static CxBool exists(const CxChar *in_filename);
		
		/** @return True if the file exists. */
		CX_FORCE_INLINE CxBool exists() const {
			return exists(m_filename.ptr());
		}

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
		 * @brief Method to open the IODevice for reading and/or writing.
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
		 * @brief Read data from the device.
		 * @param out_data The buffer to read the data into.
		 * @param in_maxBytes The max number of bytes to read.
		 * @return The number of bytes read or -1 on error.
		 */
		virtual void read(void *out_data, CxI64 in_maxBytes);

		/**
		 * @brief Read a single line of data.
		 * This method will read data up until it encounters a new line, the 
		 * end of file, or until maxBytes are read.
		 * @param out_data The buffer to read the data into.
		 * @param in_maxBytes The max number of bytes to read.
		 * @return The number of bytes read or -1 on error.
		 */
		virtual void readLine(void *out_data, CxI64 in_maxBytes);

		/** 
		 * @brief Reset the device to the beginning of the stream. 
		 * @return True if the device was reset properly.
		 */
		virtual CxBool reset();

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

		/**
		 * @brief Method to write data to the IODevice.
		 * @param in_data The data to write to the IODevice.
		 * @param in_bytes The number of bytes to write.
		 * @return The number of bytes written or -1 on error.
		 */
		virtual CxI64 write(void *in_data, CxI64 in_bytes);
		
	  protected:
	   FILE *mp_fd;
		CxMallocRef<CxChar> m_filename;
		
	};

} // namespace cat

#endif // CX_CORE_IO_FILE_POSX_CXPOSIXFILE_H
