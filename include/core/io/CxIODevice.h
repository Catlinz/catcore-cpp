#ifndef CX_CORE_IO_CXIODEVICE_H
#define CX_CORE_IO_CXIODEVICE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxIODevice.h
 *
 * Defines the base class for all IO classes (files, sockets, etc.).
 *
 * @author Catlin Zilinski
 * @date Sept. 30, 2015
 */

#include "core/Cx.h"

namespace cat {

	/**
	 * @interface CxIODevice CxIODevice.h "core/io/CxIODevice.h"
	 * @brief Interface for IO classes to implement.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 30, 2015
	 */
	class CxIODevice {
	  public:
		/** @brief Enum of possible open modes */
		enum IOModeFlag {
			kNotOpen = 0, /**< Device is not open */

			/* Main flags */
			kRead = 1 << 0, /**< Device is open for reading. */
			kWrite = 1 << 1, /**< Device is open for writing. */
			kReadWrite = (1 << 0) | (1 << 1), /**< Device open for reading + writing */

			/* Modifier flags */
			kAppend = 1 << 2, /**< All data written to end of device */
			kTruncate = 1 << 3, /**< Device is emptied when opened */
			kText = 1 << 4, /**< Device is opened in text mode */
			kUnbuffered = 1 << 5, /**< Don't use any internal buffering */
		};
		CX_FLAGS(IOModeFlag, IOMode);

		/** @brief Set all the default values. */
		CX_FORCE_INLINE CxIODevice() : m_mode(kNotOpen) {}

		/** @brief Virtual destructor for base classes. */
		virtual ~CxIODevice() {}

		/** @return True if the IODevice is open in a readable mode */
		CX_FORCE_INLINE CxBool canRead() const { return m_mode.isSet(kRead); }

		/** @return True if the IODevice is open in a writable mode */
		CX_FORCE_INLINE CxBool canWrite() const { return m_mode.isSet(kWrite); }

		/** @brief Method to close the IODevice. */
		CX_INLINE virtual void close() { m_mode = kNotOpen; }

		/**
		 * @brief Read a character from the device.
		 * @param out_char The character to read into.
		 * @return True if the character was read.
		 */
		CxBool getChar(CxChar *out_char);

		/** @return True if the IODevice is open */
		CX_FORCE_INLINE CxBool isOpen() const { return m_mode != kNotOpen; }

		/** @return True if the IODevice is open in text mode */
		CX_FORCE_INLINE CxBool isText() const { return m_mode.isSet(kText); }

		/** @return The mode flags in while the IODevice is opened. */
		CX_FORCE_INLINE IOMode mode() const { return m_mode; }

		/**
		 * @brief Method to open the IODevice for reading and/or writing.
		 * @param in_mode A combination of CxIOMode flags decribing the mode.
		 * @return True if the IODevice was opened successfully.
		 */
		virtual CxBool open(IOMode in_mode) = 0;

		/**
		 * @brief Method to read data from the device without moving forward.
		 * If the IODevice doesn't support peeking in the current mode, will return -1.
		 * @param out_data The place to read the data into.
		 * @param in_maxBytes The maximum number of bytes to read.
		 * @return The number of bytes read or -1 on error.
		 */
		CxI64 peek(void *out_data, CxI64 in_maxBytes);

		/** @return The current position within the device. */
		virtual CxI64 pos() = 0;

		/**
		 * @brief Place the specified character onto the device.
		 * @return True if the character was placed onto the device.
		 */
		CxBool putChar(CxChar in_char);

		/** 
		 * @brief Read data from the device.
		 * @param out_data The buffer to read the data into.
		 * @param in_maxBytes The max number of bytes to read.
		 * @return The number of bytes read or -1 on error.
		 */
		virtual void read(void *out_data, CxI64 in_maxBytes) = 0;

		/**
		 * @brief Read a single line of data.
		 * This method will read data up until it encounters a new line, the 
		 * end of file, or until maxBytes are read.
		 * @param out_data The buffer to read the data into.
		 * @param in_maxBytes The max number of bytes to read.
		 * @return The number of bytes read or -1 on error.
		 */
		virtual void readLine(void *out_data, CxI64 in_maxBytes) = 0;

		/** 
		 * @brief Reset the device to the beginning of the stream. 
		 * @return True if the device was reset properly.
		 */
		virtual CxBool reset() = 0;

		/**
		 * @brief Move to the given position in the IODevice.
		 * @param in_pos The position (from the start of the device) to move to.
		 * @return True if the seek was successfull.
		 */
		virtual CxBool seek(CxI64 in_pos) = 0;

		/** @return Get the size of the IODevice in bytes. */
		virtual CxI64 size() const = 0;

		/**
		 * @brief Method to write data to the IODevice.
		 * @param in_data The data to write to the IODevice.
		 * @param in_bytes The number of bytes to write.
		 * @return The number of bytes written or -1 on error.
		 */
		virtual CxI64 write(void *in_data, CxI64 in_bytes) = 0;
		
	  protected:
	   IOMode m_mode;
		
	};

	CX_FORCE_INLINE CxBool CxIODevice::getChar(CxChar *out_char) {
		return read(out_char, sizeof(CxChar)) == sizeof(CxChar);
	}

	CX_FORCE_INLINE CxBool CxIODevice::putChar(CxChar in_char) {
		return write(&in_char, sizeof(CxChar)) == sizeof(CxChar);
	}

} // namespace cat

#endif // CX_CORE_IO_CXIODEVICE_H
