#ifndef CX_CORE_IO_FILE_CXFILE_H
#define CX_CORE_IO_FILE_CXFILE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxFile.h
 *
 * Defines a file class.
 *
 * @author Catlin Zilinski
 * @date Sept. 30, 2015
 */

#include "core/Cx.h"
#include "core/io/CxIODevice.h"

namespace cat {

	/**
	 * @interface CxFile CxFile.h "core/io/file/CxFile.h"
	 * @brief A basic File object class.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 30, 2015
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
		
		
		/** @brief Set all the default values. */
		CX_FORCE_INLINE CxFile() : CxIODevice() {}

		/** @brief Virtual destructor for base classes. */
		virtual ~CxFile();

		/** @return True if the end of file has been reached */
		virtual CxBool atEnd();

		/** @brief Method to close the IODevice. */
		virtual void close();

		/**
		 * @brief Method to open the IODevice for reading and/or writing.
		 * @param in_mode A combination of CxIOMode flags decribing the mode.
		 * @return True if the IODevice was opened successfully.
		 */
		virtual CxBool open(IOMode in_mode);

		/** @return The current position within the device. */
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
		 * @brief Move to the given position in the IODevice.
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
	   FILE *mp_file;
		
	};

} // namespace cat

#endif // CX_CORE_IO_FILE_CXFILE_H
