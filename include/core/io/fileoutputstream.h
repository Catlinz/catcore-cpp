#ifndef CAT_CORE_IO_FILEOUTPUTSTREAM_H
#define CAT_CORE_IO_FILEOUTPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file fileoutputstream.h
 * @brief Defines the FileOutputStream class for writeing from a File.
 *
 * @author Catlin Zilinski
 * @date Mar 29, 2014
 */

#include "core/io/objectoutputstream.h"
#include "core/io/filedescriptor.h"

namespace Cat {

	/**
	 * @class FileOutputStream fileoutputstream.h "core/io/fileoutputstream.h"
	 * @brief Class for writing to a File.
	 *
	 * The FileOutputStream is used to write data to a file.  It inherits from 
	 * the ObjectOutputStream, so it can write basic data types and Serialiseable objects.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Oct 16, 2013
	 */
	class FileOutputStream : public ObjectOutputStream {
	  public:
		/**
		 * @brief Creates a new empty FileOutputStream with no file associated with it.
		 */
		FileOutputStream();

		/**
		 * @brief Creates a new FileOutputStream from the specified File object.
		 * @param file The File object to write to.
		 */
		FileOutputStream(const FilePtr& file);

		/**
		 * @brief Creates a new FileOutputStream and tries to open the file specified by the filename.
		 * @param filename The filename of the file to try and open.
		 */
		FileOutputStream(const Char* filename);

		/**
		 * @brief Creates a new FileOutputStream from the FileDescriptor.
		 * @param fd The FileDescriptor to write to.
		 */
		FileOutputStream(FileDescriptor* fd);

		/**
		 * @brief Copy constructor for the FileOutputStream.
		 * @param src The FileOutputStream to copy from.
		 */
		FileOutputStream(const FileOutputStream& src);

		/**
		 * @brief Assignment operator makes sure to properly copy and maintain integrity.
		 * @param src The FileOutputStream to copy from.
		 * @return A reference to this FileOutputStream object.
		 */
		FileOutputStream& operator=(const FileOutputStream& src);

		/**
		 * @brief Makes sure to close the stream if we need to.
		 */
		~FileOutputStream();		

		/**
		 * @brief write a specified amount from a buffer into the file.
		 * @param buffer The buffer to write the data from.
		 * @param toWrite The amount to write to the file.
		 * @return The amount written to the file.
		 */
		Size write(const void* buffer, Size toWrite);

		/**
		 * @brief Write a specified number of elements from a buffer into the the.
		 * @param buffer The buffer to write the data from.
		 * @param count The number of elements to write to the file.
		 * @param size The size of each element.
		 * @return The amount written to the file.
		 */
		Size write(const void* buffer, Size count, Size size);
		
		/**
		 * @brief Close the output file.
		 */
		void close();

		/**
		 * @brief Flush any buffered output to the file.
		 */
		void flush();

		/**
		 * @brief Gets the StreamDescriptor representing the file we are writing to. 
		 * @return The StreamDescriptor describing the open file.
		 */
		StreamDescriptor* getStreamDescriptor();

		/**
		 * @brief Test to see if we can write to the file in its current state.
		 * @return True if we can write to the file.
		 */
		Boolean canWrite();

		/**
		 * @brief Just a convience method to get the actual FileDescriptor instead of having to cast it.
		 * @return A pointer to the FileDescriptor.
		 */
		inline FileDescriptor* getFD();

		/**
		 * @brief Write a Serialisable object to the file.
		 * @param object The object to write the data from.
		 * @return The number of bytes written.
		 */
		Size writeObject(Serialisable* object);

	  private:
		/**
		 * @brief Try and open the FileDescriptor for output.
		 * @return True if the FileDescriptor is opened.
		 */
		Boolean openFileDescriptor();

		FileDescriptor m_fileDescriptor;

	};

	inline FileDescriptor* FileOutputStream::getFD() {
		return &m_fileDescriptor;
	}

} // namespace Cat


#endif // CAT_CORE_IO_FILEOUTPUTSTREAM_H


