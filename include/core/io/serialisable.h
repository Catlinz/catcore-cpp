#ifndef CAT_CORE_IO_SERIALIZABLE_H
#define CAT_CORE_IO_SERIALIZABLE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file serialiseable.h
 * @brief Contains the interface class for serialiseable objects. 
 *
 * Contains the interface class for serialiseable objects to 
 * be read into and written from.
 *
 * @author: Catlin Zilinski
 * @date: September 17, 2013
 */

#include "core/corelib.h"

namespace Cat {

	class InputStream;
	class OutputStream;

	/**
	 * @interface Serialisable serialisable.h "core/io/serialisable.h"
	 * @brief Defines the methods any Serialiseable object must implement.
	 *
	 * The Serialisable interface defines the methods required for an object 
	 * to be able to be read from, and written to a input or output stream.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @date Oct 11, 2013
	 */
	class Serialisable {
		public:
			/**
			 * @brief Read the object from the given InputStream.
			 *
			 * Classes that implement the read method may put restrictions on 
			 * the InputStream, such as it must be a DataInputStream or ObjectInputStream, 
			 * and if these are not followed, the program will crash.
			 *
			 * @param input The InputStream to read the object from.
			 * @return The number of bytes read from the stream.
			 */
			virtual Size read(InputStream* input) = 0;

			/**
			 * @brief write the object to the given OutputStream.
			 *
			 * Classes that implement the write method may put restrictions on 
			 * the Output, such as it must be a DataOutputStream or ObjectOutputStream, 
			 * and if these are not followed, the program will crash.

			 * @param output The OutputStream to write the object to.
			 * @return The number of bytes written to the stream.
			 */
			virtual Size write(OutputStream* output) = 0;

	};

} // namespace Cat

#endif // CAT_CORE_IO_SERIALIZABLE_H

