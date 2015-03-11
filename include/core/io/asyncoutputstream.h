#ifndef CAT_CORE_IO_ASYNCOUTPUTSTREAM_H
#define CAT_CORE_IO_ASYNCOUTPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file asyncoutputstream.h
 * @brief Wrapper around any OutputStream to make async write calls.
 *
 * @author: Catlin Zilinski
 * @date: Oct 18, 2013
 */

#include "core/corelib.h"

namespace Cat {

	class OutputStream;
	class AsyncWriteResult;

	/**
	 * @class AsyncOutputStream asyncoutputstream.h "core/io/asyncoutputstream.h"
	 * @brief Defines the AsyncOutputStream class to wrap around any OutputStream.
	 *
	 * The AsyncOutputStream class wraps around any OutputStream class to enable the user to 
	 * make asynchronous output calls with that output stream.  The results of these calls are 
	 * returned as AsyncResult objects.  *The IOManager must be initialized before any Async 
	 * I/O calls take place since it uses its AsyncTaskRunner to run the output tasks.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 18, 2013
	 */
	class AsyncOutputStream {
		public:
			/**
			 * @brief Initializes an empty AsyncOutputStream that can't do anything.
			 */
			AsyncOutputStream() : stream_(NIL) {}

			/**
			 * @brief Initialize the AsyncOutputStream wrapped around the specified OutputStream.
			 * @param inputStream The OutputStream to wrap around.
			 */
			AsyncOutputStream(OutputStream* inputStream);

			/**
			 * @brief Write a specified amount to the OutputStream from the buffer.
			 * @param buffer The buffer to write the data from.
			 * @param toWrite The amount to written to the stream.
			 * @return An AsyncWriteResult object that will hold the result of the write.
			 */
			AsyncWriteResult* write(VPtr buffer, Size toWrite);

			/**
			 * @brief Write a specified amount to the OutputStream from the buffer.
			 * @param buffer The buffer to write the data from.
			 * @param count The number of elements to write to the stream.
			 * @param size The size of the elements to write.
			 * @return An AsyncWriteResult object that will hold the result of the write.
			 */
			AsyncWriteResult* write(VPtr buffer, Size count, Size size);

			/**
			 * @brief Closes the underlying OutputStream.
			 */
			void close();

			/**
			 * @brief Tests to see if we can write from the underlying OutputStream.
			 * @return True if we can write.
			 */
			Boolean canWrite();

			/**
			 * @brief Get the underlying OutputStream that we are writing to.
			 * @return The OutputStream we are writing to.
			 */
			inline OutputStream* getOutputStream();


		private:
			OutputStream*	stream_;
	};

	inline OutputStream* AsyncOutputStream::getOutputStream() {
		return stream_;
	}

} // namespace Cat

#endif //CAT_CORE_IO_ASYNCOUTPUTSTREAM_H 

