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
			kBinary = 1 << 5,  /**< Device is opened in binary mode (default) */
			kUnbuffered = 1 << 6, /**< Don't use any internal buffering */
		};
		CX_FLAGS(IOModeFlag, IOMode);

		/** @brief Set all the default values. */
		CX_FORCE_INLINE CxIODevice()
			: m_mode(kNotOpen), m_err(CxErr::kNoError) {}

		/** @brief Virtual destructor for base classes. */
		virtual ~CxIODevice() {}

		/** @return True if the device is at the end */
		virtual CxBool atEnd() const;

		/** @return True if the IODevice is open in a readable mode */
		CX_FORCE_INLINE CxBool canRead() const { return m_mode.isSet(kRead); }

		/** @return True if the IODevice is open in a writable mode */
		CX_FORCE_INLINE CxBool canWrite() const { return m_mode.isSet(kWrite); }

		/** @brief Clear the currently set error code. */
		CX_FORCE_INLINE void clearError() { m_err = CxErr::kNoError; }

		/** @brief Method to close the IODevice. */
		CX_INLINE virtual void close() { m_mode = kNotOpen; }

		/** @return The last error encountered. */
		CX_FORCE_INLINE CxErr::Code error() const { return m_err; }
		
		/**
		 * @brief Flush the output from a device if the device supports it.
		 * @return True if the flush was successful.
		 */
		virtual CxBool flush();

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
		virtual CxI64 pos();

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
		CxI64 read(void *out_data, CxI64 in_maxBytes);

		/**
		 * @brief Read a single line of data.
		 * This method will read data up until it encounters a new line, the 
		 * end of file, or until maxChars are read.  If it reads a line, the 
		 * data returned will end with a newline character and a null-terminating 
		 * character.  If returns before finding a newline (maxChars hit), then 
		 * will just end with the null-terminating character.
		 * This method will replace '\r\n' line endings with '\n' line endings.
		 * @param out_data The buffer to read the data into.
		 * @param in_maxChars The max number of chars to read (should be at least 2)
		 * @return The number of bytes read or -1 on error.
		 */
		void CxI64 readLine(CxChar *out_data, CxI64 in_maxChars);

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
		CxI64 write(void *in_data, CxI64 in_bytes);
		
	  protected:
	   IOMode m_mode;
		CxErr::Code m_err;

		/** 
		 * @brief Read data from the device.
		 * @param out_data The buffer to read the data into.
		 * @param in_maxBytes The max number of bytes to read.
		 * @return The number of bytes read or -1 on error.
		 */
		virtual CxI64 read_impl(void *out_data, CxI64 in_maxBytes) = 0;

		/**
		 * @brief Read a single line of data.
		 * The implementation does not need to replace '\r\n' with '\n', 
		 * and does not need to append a null terminating character as the
		 * CxIODevice::readLine() method will do both.  Also, do not need to
		 * worry about having space for the null-terminating character.
		 * @param out_data The buffer to read the data into.
		 * @param in_maxChars The max number of chars to read.
		 * @return The number of chars read or -1 on error.
		 */
		virtual CxI64 readLine_impl(CxChar *out_data, CxI64 in_maxChars) = 0;

		/**
		 * @brief Method to write data to the IODevice.
		 * @param in_data The data to write to the IODevice.
		 * @param in_bytes The number of bytes to write.
		 * @return The number of bytes written or -1 on error.
		 */
		virtual CxI64 write_impl(void *in_data, CxI64 in_bytes) = 0;
		
	};

	CX_FORCE_INLINE CxBool CxIODevice::getChar(CxChar *out_char) {
		return read(out_char, sizeof(CxChar)) == sizeof(CxChar);
	}

	CX_FORCE_INLINE CxBool CxIODevice::putChar(CxChar in_char) {
		return write(&in_char, sizeof(CxChar)) == sizeof(CxChar);
	}

} // namespace cat

#endif // CX_CORE_IO_CXIODEVICE_H
