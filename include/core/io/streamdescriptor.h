#ifndef CAT_CORE_IO_STREAMDESCRIPTOR_H
#define CAT_CORE_IO_STREAMDESCRIPTOR_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file streamdescriptor.h
 * @brief Defines the interface StreamDescriptor for determining certain things about an input / output stream.
 *
 * @author Catlin Zilinski
 * @date Mar 20, 2014
 */

#include "core/corelib.h"

namespace Cat {

	enum StreamTypeEnum {
		STREAM_TYPE_FILE,
		STREAM_TYPE_SOCKET,
		kStreamTypeFile = 0x1,
		kStreamTypeSocket = 0x2,
		kStreamTypeDataBlob = 0x3,
	};

	/**
	 * @interface StreamDescriptor streamdescriptor.h "core/io/streamdescriptor"
	 * @brief Defines methods required by a StreamDescriptor.
	 *
	 * The StreamDescriptor Interface defines some methods that a 
	 * StreamDescriptor type needs to implement, to provide some information 
	 * about the i/o stream.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Sept 18, 2013
	 */
	class StreamDescriptor {
	  public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~StreamDescriptor() {}

		/**
		 * @brief Closes the stream specified by the descriptor.
		 */
		virtual void close() = 0;

		/**
		 * @brief flushes the stream specified by the descriptor.
		 */
		virtual void flush() = 0;

		/**
		 * @brief Gets the name of the stream.  
		 * Ex., for a file, would be the filename, for a 
		 * socket, would be the host:port.
		 * @return The name of the stream.
		 */
		virtual const Char* getName() const = 0;

		/**
		 * @brief Tests to see if the Stream is open.
		 * @return True if the stream is open.
		 */
		virtual Boolean isOpen() const = 0;

		/**
		 * @brief Tests to see if the type of Stream is a certain type.
		 * @param type The type of Stream to test against.
		 * @return True if the types match.
		 */
		virtual Boolean isStreamType(StreamTypeEnum type) const = 0;

		/**
		 * @brief Tests to see if the Stream is valid.
		 * @return True if the stream is valid.
		 */
		virtual Boolean isValid() const = 0;

		/**
		 * @brief Opens the stream descriptor, if not already, in the  mode.
		 * @param mode The mode for opening the stream.
		 * @return True if the stream opened successfully.
		 */
		virtual Boolean open(const Char* mode) = 0;

	};

} // namespace Cat

#endif // CAT_CORE_IO_STREAMDESCRIPTOR_H

