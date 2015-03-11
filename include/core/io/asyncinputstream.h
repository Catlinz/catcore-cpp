#ifndef CAT_CORE_IO_ASYNCINPUTSTREAM_H
#define CAT_CORE_IO_ASYNCINPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file asyncinputstream.h
 * @brief Wrapper around any InputStream to make async read calls.
 *
 * @author: Catlin Zilinski
 * @date: Oct 17, 2013
 */

#include "core/corelib.h"

namespace Cat {

	class InputStream;
	class AsyncReadResult;

	/**
	 * @class AsyncInputStream asyncinputstream.h "core/io/asyncinputstream.h"
	 * @brief Defines the AsyncInputStream class to wrap around any InputStream.
	 *
	 * The AsyncInputStream class wraps around any InputStream class to enable the user to 
	 * make asynchronous input calls with that input stream.  The results of these calls are 
	 * returned as AsyncResult objects.  *The IOManager must be initialized before any Async 
	 * I/O calls take place since it uses its AsyncTaskRunner to run the input tasks.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 17, 2013
	 */
	class AsyncInputStream {
		public:
			/**
			 * @brief Initializes an empty AsyncInputStream that can't do anything.
			 */
			AsyncInputStream() : stream_(NIL) {}

			/**
			 * @brief Initialize the AsyncInputStream wrapped around the specified InputStream.
			 * @param inputStream The InputStream to wrap around.
			 */
			AsyncInputStream(InputStream* inputStream);

			/**
			 * @brief Read a specified amount from the InputStream into the buffer.
			 * @param buffer The buffer to read the data into.
			 * @param toRead The amount to read from the stream.
			 * @return An AsyncReadResult object that will hold the result of the read.
			 */
			AsyncReadResult* read(VPtr buffer, Size toRead);

			/**
			 * @brief Read a specified amount from the InputStream into the buffer.
			 * @param buffer The buffer to read the data into.
			 * @param count The number of elements to read from the stream.
			 * @param size The size of the elements to read.
			 * @return An AsyncReadResult object that will hold the result of the read.
			 */
			AsyncReadResult* read(VPtr buffer, Size count, Size size);

			/**
			 * @brief Closes the underlying InputStream.
			 */
			void close();

			/**
			 * @brief Tests to see if we can read from the underlying InputStream.
			 * @return True if we can read.
			 */
			Boolean canRead();

			/**
			 * @brief Get the underlying InputStream that we are reading from.
			 * @return The InputStream we are reading from.
			 */
			inline InputStream* getInputStream();


		private:
			InputStream*	stream_;
	};

	inline InputStream* AsyncInputStream::getInputStream() {
		return stream_;
	}

} // namespace Cat

#endif //CAT_CORE_IO_ASYNCINPUTSTREAM_H 
