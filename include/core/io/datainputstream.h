#ifndef CAT_CORE_IO_DATAINPUTSTREAM_H
#define CAT_CORE_IO_DATAINPUTSTREAM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file datainputstream.h
 * @brief Defines the DataInputStream interface for classes to implement.
 *
 * Defines the interface DataInputStream for input readers to 
 * implement if they can read specific data types.
 *
 * @author: Catlin Zilinski
 * @date: Sept 18, 2013
 */

#include "core/io/inputstream.h"

namespace Cat {

	/**
	 * @interface DataInputStream datainputstream.h "core/io/datainputstream.h"
	 * @brief Defines interface for DataInputStream classes to implement.
	 *
	 * The DataInputStream interface defines methods for reading the base data types from 
	 * a stream, such as ints, floats, strings, etc.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 18, 2013
	 */
	class DataInputStream : public InputStream {
	  public:	

		/** 
		 * @brief Read one or more Boolean values from the stream.
		 * @param buffer The address to read the Boolean value(s) into.
		 * @param count The number of Boolean values to read (>= 1)
		 * @return The number of bytes read.
		 */
		inline virtual Size readBoolean(VPtr buffer, Size count) {
			return read(buffer, count, sizeof(Boolean));
		}		
		
		/** 
		 * @brief Read one or more Char values from the stream.
		 * @param buffer The address to read the Char value(s) into.
		 * @param count The number of Char values to read (>= 1)
		 * @return The number of bytes read.
		 */
		inline virtual Size readChar(VPtr buffer, Size count) {
			return read(buffer, count, sizeof(Char));
		}		
		
		/** 
		 * @brief Read a String of characters from the file.  
		 * Different input types may define
		 * as what consititutes a string differently.
		 * @param string The C string to read the String into.
		 * @return The number of bytes read.
		 */
		virtual Size readCStr(CStr string);		
		
		/** 
		 * @brief Read 32 bit floating point values from the stream.
		 * @param buffer The address to read the floating point value(s) into.
		 * @param count The number of floating point values to read (>= 1)
		 * @return The number of bytes read.
		 */
		inline virtual Size readF32(VPtr buffer, Size count) {
			return read(buffer, count, sizeof(F32));
		}
		
		/** 
		 * @brief Read one or more 64 bit floating point values from the stream.
		 * @param buffer The address to read the floating point value(s) into.
		 * @param count The number of floating point values to read (>= 1)
		 * @return The number of bytes read.
		 */
		inline virtual Size readF64(VPtr buffer, Size count) {
			return read(buffer, count, sizeof(F64));
		}		
	
		/** 
		 * @brief Read one or more signed 32 bit integers from the stream.
		 * @param buffer The address to read the integer(s) into.
		 * @param count The number of integers to read (>= 1)
		 * @return The number of bytes read.
		 */
		inline virtual Size readI32(VPtr buffer, Size count) {
			return read(buffer, count, sizeof(I32));
		}
		
		/** 
		 * @brief Read one or more signed 64 bit integers from the stream.
		 * @param buffer The address to read the integer(s) into.
		 * @param count The number of integers to read (>= 1)
		 * @return The number of bytes read.
		 */
		inline virtual Size readI64(VPtr buffer, Size count) {
			return read(buffer, count, sizeof(I64));
		}		
		
		/** 
		 * @brief Read unsigned 32 bit integers from the stream.
		 * @param buffer The address to read the integer(s) into.
		 * @param count The number of integers to read (>= 1)
		 * @return The number of bytes read.
		 */
		inline virtual Size readU32(VPtr buffer, Size count) {
			return read(buffer, count, sizeof(U32));
		}
		
		/** 
		 * @brief Read one or more unsigned 64 bit integers from the stream.
		 * @param buffer The address to read the integer(s) into.
		 * @param count The number of integers to read (>= 1)
		 * @return The number of bytes read.
		 */
		inline virtual Size readU64(VPtr buffer, Size count) {
			return read(buffer, count, sizeof(U64));
		}		
	};

} // namespace Cat


#endif // CAT_CORE_IO_DATAINPUTSTREAM_H
