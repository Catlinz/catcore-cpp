#ifndef CAT_CORE_IO_OBJECTINPUTSTREAM_H
#define CAT_CORE_IO_OBJECTINPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file objectinputstream.h
 * @brief Defines the ObjectInputStream interface for classes to implement.
 * 
 * Defines the interface ObjectInputStream for input readers to 
 * implement if they wish to be able to read serializeable objects.
 *
 * @author: Catlin Zilinski
 * @date: Sept 18, 2013
 */

#include "core/io/datainputstream.h"

namespace Cat {

	class Serialisable;

	/**
	 * @interface ObjectInputStream objectinputstream.h "core/io/objectinputstream.h"
	 * @brief Defines the ObjectInputStream interface for classes to implement.
	 *
	 * The ObjectInputStream interface defines the methods required for reading 
	 * Serialisable objects from an input stream.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 18, 2013
	 */
	class ObjectInputStream : public DataInputStream {
		public:
			/**
			 * @brief Read a Serialisable object from the stream.
			 * @param object The object to read the data into.
			 * @return The number of bytes read.
			 */
			virtual Size readObject(Serialisable* object) = 0;
	};


} // namespace Cat

#endif // CAT_CORE_IO_OBJECTINPUTSTREAM_H

