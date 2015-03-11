#ifndef CAT_CORE_IO_OUTPUTSTREAM_H
#define CAT_CORE_IO_OUTPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file outputstream.h
 * @brief Defines the interface OutputStream for basic output readers to implement.
 *
 * @author Catlin Zilinski
 * @date Oct 11, 2013
 */

#include "core/corelib.h"

namespace Cat {

	class StreamDescriptor;

	/**
	 * @interface OutputStream outputstream.h "core/io/outputstream.h"
	 * @brief Defines interface for any OutputStream class to implement.
	 *
	 * The OutputStream interface specifies a simple set of methods an output 
	 * writer class must implement.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 11, 2013
	 */
	class OutputStream {
	  public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~OutputStream() {}
		
		/**
		 * @brief Write a specified amount from a buffer into the stream.
		 * @param buffer The buffer to write the data from.
		 * @param toWrite The amount to write to the stream.
		 * @return The amount written to the stream.
		 */
		virtual Size write(const void* buffer, Size toWrite) = 0;
		/**
		 * @brief Write a number of elements from a buffer into the stream.
		 * @param buffer The buffer to write the data from.
		 * @param count The number of elements to write to the stream.
		 * @param size The size of each element.
		 * @return The amount written to the stream.
		 */
		virtual Size write( const void* buffer, Size count, Size size) = 0;

		/**
		 * @brief Flush any buffered output to the stream.
		 */
		virtual void flush() = 0;

		/**
		 * @brief Close the output stream.
		 */
		virtual void close() = 0;
			
		/**
		 * @brief Gets the StreamDescriptor for the stream we are writing to.
		 * @return The StreamDescriptor describing the open stream.
		 */
		virtual StreamDescriptor* getStreamDescriptor() = 0;
			
		/**
		 * @brief Test if the stream can be written to in its current state.
		 * @return True if the stream can be written to.
		 */
		virtual Boolean canWrite() = 0;


	};

} // namespace Cat

#endif // CAT_CORE_IO_OUTPUTSTREAM_H

