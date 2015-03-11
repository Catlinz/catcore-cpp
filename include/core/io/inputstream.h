#ifndef CAT_CORE_IO_INPUTSTREAM_H
#define CAT_CORE_IO_INPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file inputstream.h
 * @brief Defines the interface InputStream for basic input readers to implement.
 *
 * @author Catlin Zilinski
 * @date Sept 18, 2013
 */

#include "core/corelib.h"

namespace Cat {

	class StreamDescriptor;

	/**
	 * @interface InputStream inputstream.h "core/io/inputstream.h"
	 * @brief Defines interface for any InputStream class to implement.
	 *
	 * The InputStream interface specifies a simple set of methods an input 
	 * reader class must implement.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 18, 2013
	 */
	class InputStream {
	  public:

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~InputStream() {}

		/**
		 * @brief Test if the stream can be  read from in its current state.
		 * @return True if the stream can be read from.
		 */
		virtual Boolean canRead() = 0;
		
		/**
		 * @brief Close the input stream.
		 */
		virtual void close() = 0;

		/**
		 * @brief Gets the StreamDescriptor we are getting input from.
		 * @return The StreamDescriptor describing the open stream.
		 */
		virtual StreamDescriptor* getStreamDescriptor() = 0;

		/**
		 * @brief Determines if the stream allows rewinding. 
		 * i.e., going back in the stream.
		 * @return True if the stream allows rewinding.
		 */
		virtual Boolean isPositionable() const { return false; }

		/**
		 * @brief Read a specified amount from a stream into the buffer.
		 * @param buffer The buffer to read the data into.
		 * @param toRead The amount to read from the stream.
		 * @return The amount read from the stream.
		 */
		virtual Size read(VPtr buffer, Size toRead) = 0;
		/**
		 * @brief Read a number of elements from a stream into the buffer.
		 * @param buffer The buffer to read the data into.
		 * @param count The number of elements to read from the stream.
		 * @param size The size of each element.
		 * @return The amount read from the stream.
		 */
		virtual Size read(VPtr buffer, Size count, Size size) = 0;
		
		/**
		 * @brief Goes back a certain amount in the stream.  
		 * Not all readers may implement this to do something useful.
		 * @param bytes The number of bytes to put back into the stream.
		 */
		virtual Size rewind(Size bytes) {return 0;}

		/**
		 * @brief Skips over a specified number of bytes in the stream.
		 * @param The number of bytes to skip over.
		 */
		virtual Size skip(Size bytes) = 0;

	

		

	};

} // namespace Cat

#endif // CAT_CORE_IO_INPUTSTREAM_H

