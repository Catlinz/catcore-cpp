#ifndef CAT_CORE_IO_ASYNCOBJECTINPUTSTREAM_H
#define CAT_CORE_IO_ASYNCOBJECTINPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file asyncdatainputstream.h
 * @brief Wrapper around any ObjectInputStream to make async read calls.
 *
 * @author: Catlin Zilinski
 * @date: Oct 17, 2013
 */

#include "core/io/asyncdatainputstream.h"

namespace Cat {

	class ObjectInputStream;
	class Serialisable;

	/**
	 * @class AsyncObjectInputStream asyncobjectinputstream.h "core/io/asyncobjectinputstream.h"
	 * @brief Defines the AsyncObjectInputStream class to wrap around any ObjectInputStream.
	 *
	 * The AsyncObjectInputStream class wraps around any ObjectInputStream class to enable the user to 
	 * make asynchronous input calls with that input stream.  The results of these calls are 
	 * returned as AsyncResult objects.  *The IOManager must be initialized before any Async 
	 * I/O calls take place since it uses its AsyncTaskRunner to run the input tasks.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 17, 2013
	 */
	class AsyncObjectInputStream : public AsyncDataInputStream {
		public:
			/**
			 * @brief Initializes an empty AsyncObjectInputStream that can't do anything.
			 */
			AsyncObjectInputStream() {}

			/**
			 * @brief Initialize the AsyncObjectInputStream wrapped around the specified ObjectInputStream.
			 * @param inputStream The ObjectInputStream to wrap around.
			 */
			AsyncObjectInputStream(ObjectInputStream* inputStream) : AsyncDataInputStream((DataInputStream*)inputStream) {}

			/**
			 * @brief Read a Serialisable object from the stream.
			 * @param object The object to read the data into.
			 * @return The AsyncResult object.
			 */
			AsyncReadResult* readObject(Serialisable* object);

		
	};


} // namespace Cat

#endif //CAT_CORE_IO_ASYNCOBJECTINPUTSTREAM_H 


