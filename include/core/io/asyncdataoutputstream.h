#ifndef CAT_CORE_IO_ASYNCDATAOUTPUTSTREAM_H
#define CAT_CORE_IO_ASYNCDATAOUTPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file asyncdataoutputstream.h
 * @brief Wrapper around any DataOutputStream to make async write calls.
 *
 * @author: Catlin Zilinski
 * @date: Oct 18, 2013
 */

#include "core/io/asyncoutputstream.h"

namespace Cat {

	class DataOutputStream;

	/**
	 * @class AsyncDataOutputStream asyncdataoutputstream.h "core/io/asyncdataoutputstream.h"
	 * @brief Defines the AsyncDataOutputStream class to wrap around any DataOutputStream.
	 *
	 * The AsyncDataOutputStream class wraps around any DataOutputStream class to enable the user to 
	 * make asynchronous output calls with that output stream.  The results of these calls are 
	 * returned as AsyncResult objects.  *The IOManager must be initialized before any Async 
	 * I/O calls take place since it uses its AsyncTaskRunner to run the output tasks.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 18, 2013
	 */
	class AsyncDataOutputStream : public AsyncOutputStream {
		public:
			/**
			 * @brief Initializes an empty AsyncDataOutputStream that can't do anything.
			 */
			AsyncDataOutputStream() {}

			/**
			 * @brief Initialize the AsyncDataOutputStream wrapped around the specified DataOutputStream.
			 * @param outputStream The DataOutputStream to wrap around.
			 */
			AsyncDataOutputStream(DataOutputStream* outputStream) : AsyncOutputStream((OutputStream*)outputStream) {}

			/** 
			 * @brief Write one or more unsigned 32 bit integers to the stream.
			 * @param buffer The address to write the integer(s) from.
			 * @param count The number of integers to write (>= 1)
			 * @return The AsyncWriteResult that will hold the result of the write.
			 */
			AsyncWriteResult* writeU32(VPtr buffer, Size count);
			/** 
			 * @brief Write one or more unsigned 64 bit integers to the stream.
			 * @param buffer The address to write the integer(s) from.
			 * @param count The number of integers to write (>= 1)
			 * @return The AsyncWriteResult that will hold the result of the write.
			 */
			AsyncWriteResult* writeU64(VPtr buffer, Size count);
			/** 
			 * @brief Write one or more signed 32 bit integers to the stream.
			 * @param buffer The address to write the integer(s) from.
			 * @param count The number of integers to write (>= 1)
			 * @return The AsyncWriteResult that will hold the result of the write.
			 */
			AsyncWriteResult* writeI32(VPtr buffer, Size count);
			/** 
			 * @brief Write one or more signed 64 bit integers to the stream.
			 * @param buffer The address to write the integer(s) from.
			 * @param count The number of integers to write (>= 1)
			 * @return The AsyncWriteResult that will hold the result of the write.
			 */
			AsyncWriteResult* writeI64(VPtr buffer, Size count);
			/** 
			 * @brief Write one or more 32 bit floating point values to the stream.
			 * @param buffer The address to write the floating point value(s) from.
			 * @param count The number of floating point values to write (>= 1)
			 * @return The AsyncWriteResult that will hold the result of the write.
			 */
			AsyncWriteResult* writeF32(VPtr buffer, Size count);
			/** 
			 * @brief Write one or more 64 bit floating point values to the stream.
			 * @param buffer The address to write the floating point value(s) from.
			 * @param count The number of floating point values to write (>= 1)
			 * @return The AsyncWriteResult that will hold the result of the write.
			 */
			AsyncWriteResult* writeF64(VPtr buffer, Size count);
			/** 
			 * @brief Write one or more Boolean values to the stream.
			 * @param buffer The address to write the Boolean value(s) from.
			 * @param count The number of Boolean values to write (>= 1)
			 * @return The AsyncWriteResult that will hold the result of the write.
			 */
			AsyncWriteResult* writeBoolean(VPtr buffer, Size count);
			/** 
			 * @brief Write one or more Char values to the stream.
			 * @param buffer The address to write the Char value(s) from.
			 * @param count The number of Char values to write (>= 1)
			 * @return The AsyncWriteResult that will hold the result of the write.
			 */
			AsyncWriteResult* writeChar(VPtr buffer, Size count);
			/** 
			 * @brief Write a String of characters to the stream.  Different output types may define
			 * as what consititutes a string differently.
			 * @param string The C string to write the String from.
			 * @return The AsyncWriteResult that will hold the result of the write.
			 */
			AsyncWriteResult* writeCStr(Char* str);
	};


} // namespace Cat

#endif // CAT_CORE_IO_ASYNCDATAOUTPUTSTREAM_H
