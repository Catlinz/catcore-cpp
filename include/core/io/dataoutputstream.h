#ifndef CAT_CORE_IO_DATAOUTPUTSTREAM_H
#define CAT_CORE_IO_DATAOUTPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file dataoutputstream.h
 * @brief Defines the DataOutputStream interface for classes to implement.
 *
 * Defines the interface DataOutputStream for output writeers to 
 * implement if they can write specific data types.
 *
 * @author: Catlin Zilinski
 * @date: Oct 11, 2013
 */

#include "core/io/outputstream.h"

namespace Cat {

	/**
	 * @interface DataOutputStream dataoutputstream.h "core/io/dataoutputstream.h"
	 * @brief Defines interface for DataOutputStream classes to implement.
	 *
	 * The DataOutputStream interface defines methods for writing the base data types to 
	 * a stream, such as ints, floats, strings, etc.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 11, 2013
	 */
	class DataOutputStream : public OutputStream {
	  public:

		/** 
		 * @brief Write one or more Boolean values to the stream.
		 * @param buffer The address to write the Boolean value(s) from.
		 * @param count The number of Boolean values to write (>= 1)
		 * @return The number of bytes written.
		 */
		inline virtual Size writeBoolean(VPtr buffer, Size count) {
			return write(buffer, count, sizeof(Boolean));
		}
		
		/** 
		 * @brief Write one or more Char values to the stream.
		 * @param buffer The address to write the Char value(s) from.
		 * @param count The number of Char values to write (>= 1)
		 * @return The number of bytes written.
		 */
		inline virtual Size writeChar(VPtr buffer, Size count) {
			return write(buffer, count, sizeof(Char));
		}
		
		/** 
		 * @brief Write a String of characters to the stream.  
		 * Different output types may define as what 
		 * consititutes a string differently.
		 * @param string The C string to write the String from.
		 * @return The number of bytes written.
		 */
		virtual Size writeCStr(const Char* string);		
		
		/** 
		 * @brief Write one or more 32 bit floating point values to the stream.
		 * @param buffer The address to write the floating point value(s) from.
		 * @param count The number of floating point values to write (>= 1)
		 * @return The number of bytes written.
		 */
		inline virtual Size writeF32(VPtr buffer, Size count) {
			return write(buffer, count, sizeof(F32));
		}
		
		/** 
		 * @brief Write one or more 64 bit floating point values to the stream.
		 * @param buffer The address to write the floating point value(s) from.
		 * @param count The number of floating point values to write (>= 1)
		 * @return The number of bytes written.
		 */
		inline virtual Size writeF64(VPtr buffer, Size count) {
			return write(buffer, count, sizeof(F64));
		}
		
		/** 
		 * @brief Write one or more signed 32 bit integers to the stream.
		 * @param buffer The address to write the integer(s) into.
		 * @param count The number of integers to write (>= 1)
		 * @return The number of bytes written.
		 */
		inline virtual Size writeI32(VPtr buffer, Size count) {
			return write(buffer, count, sizeof(I32));
		}
		
		/** 
		 * @brief Write one or more signed 64 bit integers to the stream.
		 * @param buffer The address to write the integer(s) into.
		 * @param count The number of integers to write (>= 1)
		 * @return The number of bytes written.
		 */
		inline virtual Size writeI64(VPtr buffer, Size count) {
			return write(buffer, count, sizeof(I64));
		}		

		/** 
		 * @brief Write one or more unsigned 32 bit integers to the stream.
		 * @param buffer The address to write the integer(s) from.
		 * @param count The number of integers to write (>= 1)
		 * @return The number of bytes written.
		 */
		inline virtual Size writeU32(VPtr buffer, Size count) {
			return write(buffer, count, sizeof(U32));
		}
		
		/** 
		 * @brief Write one or more unsigned 64 bit integers to the stream.
		 * @param buffer The address to write the integer(s) into.
		 * @param count The number of integers to write (>= 1)
		 * @return The number of bytes written.
		 */
		inline virtual Size writeU64(VPtr buffer, Size count) {
			return write(buffer, count, sizeof(U64));
		}
		
	};

} // namespace Cat


#endif // CAT_CORE_IO_DATAOUTPUTSTREAM_H
