#ifndef CAT_CORE_IO_ASYNCOBJECTOUTPUTSTREAM_H
#define CAT_CORE_IO_ASYNCOBJECTOUTPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file asyncdataoutputstream.h
 * @brief Wrapper around any ObjectOutputStream to make async write calls.
 *
 * @author: Catlin Zilinski
 * @date: Oct 18, 2013
 */

#include "core/io/asyncdataoutputstream.h"

namespace Cat {

	class ObjectOutputStream;
	class Serialisable;

	/**
	 * @class AsyncObjectOutputStream asyncobjectoutputstream.h "core/io/asyncobjectoutputstream.h"
	 * @brief Defines the AsyncObjectOutputStream class to wrap around any ObjectOutputStream.
	 *
	 * The AsyncObjectOutputStream class wraps around any ObjectOutputStream class to enable the user to 
	 * make asynchronous output calls with that output stream.  The results of these calls are 
	 * returned as AsyncResult objects.  *The IOManager must be initialized before any Async 
	 * I/O calls take place since it uses its AsyncTaskRunner to run the output tasks.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 18, 2013
	 */
	class AsyncObjectOutputStream : public AsyncDataOutputStream {
		public:
			/**
			 * @brief Initializes an empty AsyncObjectOutputStream that can't do anything.
			 */
			AsyncObjectOutputStream() {}

			/**
			 * @brief Initialize the AsyncObjectOutputStream wrapped around the specified ObjectOutputStream.
			 * @param outputStream The ObjectOutputStream to wrap around.
			 */
			AsyncObjectOutputStream(ObjectOutputStream* outputStream) : AsyncDataOutputStream((DataOutputStream*)outputStream) {}

			/**
			 * @brief Write a Serialisable object to the stream.
			 * @param object The object to write the data from.
			 * @return The AsyncResult object.
			 */
			AsyncWriteResult* writeObject(Serialisable* object);

		
	};


} // namespace Cat

#endif //CAT_CORE_IO_ASYNCOBJECTOUTPUTSTREAM_H 



