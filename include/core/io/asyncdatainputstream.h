#ifndef CAT_CORE_IO_ASYNCDATAINPUTSTREAM_H
#define CAT_CORE_IO_ASYNCDATAINPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file asyncdatainputstream.h
 * @brief Wrapper around any DataInputStream to make async read calls.
 *
 * @author: Catlin Zilinski
 * @date: Oct 17, 2013
 */

#include "core/io/asyncinputstream.h"

namespace Cat {

	class DataInputStream;

	/**
	 * @class AsyncDataInputStream asyncdatainputstream.h "core/io/asyncdatainputstream.h"
	 * @brief Defines the AsyncDataInputStream class to wrap around any DataInputStream.
	 *
	 * The AsyncDataInputStream class wraps around any DataInputStream class to enable the user to 
	 * make asynchronous input calls with that input stream.  The results of these calls are 
	 * returned as AsyncResult objects.  *The IOManager must be initialized before any Async 
	 * I/O calls take place since it uses its AsyncTaskRunner to run the input tasks.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 17, 2013
	 */
	class AsyncDataInputStream : public AsyncInputStream {
		public:
			/**
			 * @brief Initializes an empty AsyncDataInputStream that can't do anything.
			 */
			AsyncDataInputStream() {}

			/**
			 * @brief Initialize the AsyncDataInputStream wrapped around the specified DataInputStream.
			 * @param inputStream The DataInputStream to wrap around.
			 */
			AsyncDataInputStream(DataInputStream* inputStream) : AsyncInputStream((InputStream*)inputStream) {}

			/** 
			 * @brief Read one or more unsigned 32 bit integers from the stream.
			 * @param buffer The address to read the integer(s) into.
			 * @param count The number of integers to read (>= 1)
			 * @return The AsyncReadResult that will hold the result of the read.
			 */
			AsyncReadResult* readU32(VPtr buffer, Size count);
			/** 
			 * @brief Read one or more unsigned 64 bit integers from the stream.
			 * @param buffer The address to read the integer(s) into.
			 * @param count The number of integers to read (>= 1)
			 * @return The AsyncReadResult that will hold the result of the read.
			 */
			AsyncReadResult* readU64(VPtr buffer, Size count);
			/** 
			 * @brief Read one or more signed 32 bit integers from the stream.
			 * @param buffer The address to read the integer(s) into.
			 * @param count The number of integers to read (>= 1)
			 * @return The AsyncReadResult that will hold the result of the read.
			 */
			AsyncReadResult* readI32(VPtr buffer, Size count);
			/** 
			 * @brief Read one or more signed 64 bit integers from the stream.
			 * @param buffer The address to read the integer(s) into.
			 * @param count The number of integers to read (>= 1)
			 * @return The AsyncReadResult that will hold the result of the read.
			 */
			AsyncReadResult* readI64(VPtr buffer, Size count);
			/** 
			 * @brief Read one or more 32 bit floating point values from the stream.
			 * @param buffer The address to read the floating point value(s) into.
			 * @param count The number of floating point values to read (>= 1)
			 * @return The AsyncReadResult that will hold the result of the read.
			 */
			AsyncReadResult* readF32(VPtr buffer, Size count);
			/** 
			 * @brief Read one or more 64 bit floating point values from the stream.
			 * @param buffer The address to read the floating point value(s) into.
			 * @param count The number of floating point values to read (>= 1)
			 * @return The AsyncReadResult that will hold the result of the read.
			 */
			AsyncReadResult* readF64(VPtr buffer, Size count);
			/** 
			 * @brief Read one or more Boolean values from the stream.
			 * @param buffer The address to read the Boolean value(s) into.
			 * @param count The number of Boolean values to read (>= 1)
			 * @return The AsyncReadResult that will hold the result of the read.
			 */
			AsyncReadResult* readBoolean(VPtr buffer, Size count);
			/** 
			 * @brief Read one or more Char values from the stream.
			 * @param buffer The address to read the Char value(s) into.
			 * @param count The number of Char values to read (>= 1)
			 * @return The AsyncReadResult that will hold the result of the read.
			 */
			AsyncReadResult* readChar(VPtr buffer, Size count);
			/** 
			 * @brief Read a String of characters from the file.  Different input types may define
			 * as what consititutes a string differently.
			 * @param string The C string to read the String into.
			 * @return The AsyncReadResult that will hold the result of the read.
			 */
			AsyncReadResult* readCStr(CStr string);
	};


} // namespace Cat

#endif //CAT_CORE_IO_ASYNCDATAINPUTSTREAM_H 

