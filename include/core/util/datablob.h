#ifndef CAT_CORE_UTIL_DATABLOB_H
#define CAT_CORE_UTIL_DATABLOB_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file datablob.h
 * @brief An object to hold and work with arbitraty data.
 *
 * @author Catlin Zilinski
 * @date June 16, 2014
 */

#include "core/io/objectinputstream.h"
#include "core/io/objectoutputstream.h"
#include "core/io/streamdescriptor.h"

namespace Cat {

	/**
	 * @class DataBlob datablob.h "core/util/datablob.h"
	 * @brief An object to hold and work with arbitraty data.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since June 16, 2014
	 */
	class DataBlob : public ObjectInputStream,
						  public ObjectOutputStream,
						  public StreamDescriptor {
	  public:

		/**
		 * @brief Create a new empty DataBlob.
		 * @param bufferSize The size of the write buffer (default = 256).
		 */
		DataBlob(Size bufferSize = 256);

		/**
		 * @brief Create a new empty named DataBlob.
		 * @param name The name of the DataBlob.
		 * @param bufferSize The size of the write buffer (default = 256).
		 */
		DataBlob(const Char* name, Size bufferSize);

		/**
		 * @brief Destructor, destroys everything.
		 */
		~DataBlob();

		/**
		 * @brief Test if the stream can be read from in its current state.
		 * @return True if the stream can be read from.
		 */
		inline Boolean canRead() { return m_canRead; }

		/**
		 * @brief Test if the stream can be written to in its current state.
		 * @return True if the stream can be written to.
		 */
		inline Boolean canWrite() { return m_canWrite; }		

		/**
		 * @brief Close the input stream.
		 */
		inline void close() {}

		/**
		 * @brief flushes the stream specified by the descriptor.
		 */
		void flush();

		/**
		 * @brief Gets the name of the stream.  
		 * Ex., for a file, would be the filename, for a 
		 * socket, would be the host:port.
		 * @return The name of the stream.
		 */
		inline const Char* getName() const {
			return m_pName;
		}		

		/**
		 * @brief Gets the StreamDescriptor we are getting input from.
		 * @return The StreamDescriptor describing the open stream.
		 */
		inline StreamDescriptor* getStreamDescriptor() {
			return this;
		}

		/**
		 * @brief Tests to see if the Stream is open.
		 * @return True if the stream is open.
		 */
		inline Boolean isOpen() const {
			return m_pData != NIL;
		}

		/**
		 * @brief Determines if the stream allows rewinding. 
		 * i.e., going back in the stream.
		 * @return True if the stream allows rewinding.
		 */
		inline Boolean isPositionable() const {
			return true;
		}

		/**
		 * @brief Tests to see if the type of Stream is a certain type.
		 * @param type The type of Stream to test against.
		 * @return True if the types match.
		 */
		inline Boolean isStreamType(StreamTypeEnum type) const {
			return type == kStreamTypeDataBlob;
		}

		/**
		 * @brief Tests to see if the Stream is valid.
		 * @return True if the stream is valid.
		 */
		inline Boolean isValid() const {
			return m_pData != NIL;
		}		

		/**
		 * @brief Opens the stream descriptor, if not already open, in the specified mode.
		 * @param mode The mode for opening the stream.
		 * @return True if the stream opened successfully.
		 */
		inline Boolean open(const Char* mode) {return true;}		

		/**
		 * @brief Read a specified amount from a stream into the buffer.
		 * @param buffer The buffer to read the data into.
		 * @param toRead The amount to read from the stream.
		 * @return The amount read from the stream.
		 */
		Size read(VPtr buffer, Size toRead);
		/**
		 * @brief Read a number of elements from a stream into the buffer.
		 * @param buffer The buffer to read the data into.
		 * @param count The number of elements to read from the stream.
		 * @param size The size of each element.
		 * @return The amount read from the stream.
		 */
		inline Size read(VPtr buffer, Size count, Size size) {
			return read(buffer, count*size);
		}
		
		/**
		 * @brief Read a Serialisable object from the stream.
		 * @param object The object to read the data into.
		 * @return The number of bytes read.
		 */
		Size readObject(Serialisable* object);

		/**
		 * @brief Reserve the specified amount of bytes of free data space.
		 * This method will resize the actual data storage (not the buffer) 
		 * so that it can store at least the specified amount.
		 * @param bytes The number of bytes to reserve.
		 */
		void reserve(Size bytes);		

		/**
		 * @brief Goes back a certain amount in the stream.  
		 * Not all readers may implement this to do something useful.
		 * @param bytes The number of bytes to put back into the stream.
		 */
		Size rewind(Size bytes);		

		/**
		 * @brief Skips over a specified number of bytes in the stream.
		 * @param The number of bytes to skip over.
		 */
		Size skip(Size bytes);

		/**
		 * @brief Write a specified amount from a buffer into the stream.
		 * @param buffer The buffer to write the data from.
		 * @param toWrite The amount to write to the stream.
		 * @return The amount written to the stream.
		 */
		Size write(const void* buffer, Size toWrite);
		
		/**
		 * @brief Write a specified number of elements from a buffer into the stream.
		 * @param buffer The buffer to write the data from.
		 * @param count The number of elements to write to the stream.
		 * @param size The size of each element.
		 * @return The amount written to the stream.
		 */
		inline Size write(const void* buffer, Size count, Size size) {
			return write(buffer, count*size);
		}		

		/**
		 * @brief Write a Serialisable object to the stream.
		 * @param object The object to write to the stream.
		 * @return The number of bytes written.
		 */
		Size writeObject(Serialisable* object);

	  private:
		/**
		 * @brief Method to copy data into the data array
		 * @param buffer The buffer to copy the data from.
		 * @param bytes The bytes to copy.
		 * @return The number of bytes copied.
		 */
		Size copyBuffer(const void* buffer, Size bytes);
		
		Byte* m_pBuffer;
		Byte* m_pData;		
		
		Char* m_pName;

		Size m_bufferSize;
		Size m_dataSize;		
		
		OID m_oid;

		Size m_readIdx;
		Size m_writeIdx;
		Size m_bufferIdx;
		Size m_storedBytes;		

		Boolean m_canRead;
		Boolean m_canWrite;		
	};

} // namespace Cat

#endif // CAT_CORE_UTIL_DATABLOB_H
