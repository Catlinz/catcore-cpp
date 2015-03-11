#ifndef CAT_CORE_IO_OBJECTOUTPUTSTREAM_H
#define CAT_CORE_IO_OBJECTOUTPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file objectoutputstream.h
 * @brief Defines the ObjectOutputStream interface for classes to implement.
 * 
 * Defines the interface ObjectOutputStream for output readers to 
 * implement if they wish to be able to read serializeable objects.
 *
 * @author: Catlin Zilinski
 * @date: Oct 11, 2013
 */

#include "core/io/dataoutputstream.h"

namespace Cat {

	class Serialisable;

	/**
	 * @interface ObjectOutputStream objectoutputstream.h "core/io/objectoutputstream.h"
	 * @brief Defines the ObjectOutputStream interface for classes to implement.
	 *
	 * The ObjectOutputStream interface defines the methods required for writing 
	 * Serialisable objects to an output stream.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 11, 2013
	 */
	class ObjectOutputStream : public DataOutputStream {
		public:
			/**
			 * @brief Write a Serialisable object to the stream.
			 * @param object The object to write to the stream.
			 * @return The number of bytes written.
			 */
			virtual Size writeObject(Serialisable* object) = 0;
	};


} // namespace Cat

#endif // CAT_CORE_IO_OBJECTOUTPUTSTREAM_H

