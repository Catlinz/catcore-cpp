#ifndef CAT_CORE_IO_FILEINPUTSTREAM_H
#define CAT_CORE_IO_FILEINPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file fileinputstream.h
 * @brief Defines the FileInputStream class for reading from a File.
 *
 * @author Catlin Zilinski
 * @date Mar 30, 2014
 */

#include "core/io/objectinputstream.h"
#include "core/io/filedescriptor.h"

namespace Cat {

	/**
	 * @class FileInputStream fileinputstream.h "core/io/fileinputstream.h"
	 * @brief Class for reading from a File.
	 *
	 * The FileInputStream is used to read data from a file.  It inherits from 
	 * the ObjectInputStream, so it can read basic data types and Serialiseable objects.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Oct 11, 2013
	 */
	class FileInputStream : public ObjectInputStream {
	  public:
		/**
		 * @brief Creates a new empty FileInputStream with no file associated with it.
		 */
		FileInputStream();

		/**
		 * @brief Creates a new FileInputStream from the specified File object.
		 * @param file The File object to read from.
		 */
		FileInputStream(const FilePtr& file);

		/**
		 * @brief Creates a new FileInputStream and tries to open the file specified by the filename.
		 * @param filename The filename of the file to try and open.
		 */
		FileInputStream(const Char* filename);

		/**
		 * @brief Creates a new FileInputStream from the FileDescriptor.
		 * @param fd The FileDescriptor to read from.
		 */
		FileInputStream(FileDescriptor* fd);

		/**
		 * @brief Copy constructor for the FileInputStream.
		 * @param src The FileInputStream to copy from.
		 */
		FileInputStream(const FileInputStream& src);

		/**
		 * @brief Assignment operator makes sure to properly copy and maintain integrity.
		 * @param src The FileInputStream to copy from.
		 * @return A reference to this FileInputStream object.
		 */
		FileInputStream& operator=(const FileInputStream& src);

		/**
		 * @brief Makes sure to close the stream if we need to.
		 */
		~FileInputStream();		

		/**
		 * @brief Read a specified amount from a file into the buffer.
		 * @param buffer The buffer to read the data into.
		 * @param toRead The amount to read from the file.
		 * @return The amount read from the file.
		 */
		Size read(void* buffer, Size toRead);

		/**
		 * @brief Read a specified number of elements from a file into the buffer.
		 * @param buffer The buffer to read the data into.
		 * @param count The number of elements to read from the file.
		 * @param size The size of each element.
		 * @return The amount read from the file.
		 */
		Size read(void* buffer, Size count, Size size);
		
		/**
		 * @brief Close the input file.
		 */
		void close();

		/**
		 * @brief Skips over a specified number of bytes in the file.
		 * @param The number of bytes to skip over.
		 */
		Size skip(Size bytes);

		/**
		 * @brief Goes back a certain amount in the file.  
		 * @param bytes The number of bytes to put back into the file.
		 * @return The number of bytes actually rewound.
		 */
		Size rewind(Size bytes);

		/**
		 * @brief Determines if the file allows rewinding. 
		 * @return True if the file allows rewinding.
		 */
		Boolean isPositionable() const;

		/**
		 * @brief Gets the StreamDescriptor representing the file we are reading from. 
		 * @return The StreamDescriptor describing the open file.
		 */
		StreamDescriptor* getStreamDescriptor();

		/**
		 * @brief Test to see if the file can be read from in the current state.
		 * @return True if we can read from the file.
		 */
		Boolean canRead();

		/**
		 * @brief Just a convience method to get the actual FileDescriptor instead of having to cast it.
		 * @return A pointer to the FileDescriptor.
		 */
		inline FileDescriptor* getFD();

		/**
		 * @brief Read a Serialisable object from the file.
		 * @param object The object to read the data into.
		 * @return The number of bytes read.
		 */
		Size readObject(Serialisable* object);

		/**
		 * @brief Read and return the contents of the file as a Char array.
		 * @return The contents of the file as a dynamically allocated Char array.
		 */
		Char* getContents();

		/**
		 * @brief Read and return the contents of the file as a UByte array.
		 * @return The contents of the file as a dynamically allocated UByte array.
		 */
		UByte* getData();

	  private:
		/**
		 * @brief Try and open the FileDescriptor for input.
		 * @return True if the FileDescriptor is opened.
		 */
		Boolean openFileDescriptor();

		FileDescriptor m_fileDescriptor;

	};

	inline FileDescriptor* FileInputStream::getFD() {
		return &m_fileDescriptor;
	}

} // namespace Cat


#endif // CAT_CORE_IO_FILEINPUTSTREAM_H

