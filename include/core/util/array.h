#ifndef CAT_CORE_UTIL_ARRAY_H
#define CAT_CORE_UTIL_ARRAY_H

#if defined (DEBUG)
#define CAT_DEBUG_ARRAY_CHECK_BOUNDS(idx) debugCheckBounds(idx)
#else
#define CAT_DEBUG_ARRAY_CHECK_BOUNDS(idx)
#endif

/**
 * @copyright Copyright Catlin Zilinski, 2014.  All rights reserved.
 *
 * @file array.h
 * @brief Contains a simple Array wrapper class.
 *
 * The Array class is a simple wrapper class around a C++ static array, 
 * (i.e., length cannot change) which provides things like being able 
 * to know the length, etc.
 *
 * @author Catlin Zilinski
 * @date Feb 7, 2014
 */

#include "core/corelib.h"

namespace Cat {

	/**
	 * @class Array array.h "core/util/array.h"
	 * @brief A simple Array wrapper class.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Oct 18, 2013
	 */
	template<class T>
	class Array {
	  public:
		/**
		 * @brief Create an empty array with no length.
		 */
		inline Array() : m_length(0), m_pArray(NIL) {}
		
		/**
		 * @brief Create a new array with the specified length.
		 * @param capacity The length of the Array.
		 */
		explicit inline Array(U32 length) : m_length(length), m_pArray(NIL) {
			m_pArray = new T[m_length];
		}

		/**
		 * @brief Create a new array from the existing data.
		 * @param length The length of the array.
		 * @param data The array data.
		 */
		inline Array(U32 length, T* data) : m_length(length), m_pArray(data) {}		

		/**
		 * @brief Operator to access and modify elements.
		 * @param idx The index of the element to access.
		 * @return A reference to the element.
		 */
		inline T& at(Size idx) { 
			CAT_DEBUG_ARRAY_CHECK_BOUNDS(idx);			
			return m_pArray[idx];
		}		
		inline const T& at(Size idx) const { 
			CAT_DEBUG_ARRAY_CHECK_BOUNDS(idx);			
			return m_pArray[idx]; 
		}	

		/**
		 * @brief Deletes the memory associated with the array
		 */
		inline void destroy() {
			if (m_pArray) {
				delete[] m_pArray;
				m_pArray = NIL;
				m_length = 0;				
			}
		}
		
		
		/**
		 * @brief Create a copy of the array.
		 * @return A copy of the array contents.
		 */
		Array<T> getCopy() const { 
			T* dataCopy = new T[m_length];
			memcpy(dataCopy, m_pArray, sizeof(T)*m_length);
			return Array<T>(m_length, dataCopy);			
		}	  

		/**
		 * @brief Checks to see if the Array is null.
		 * @return True if the array is null.
		 */
		inline Boolean isNull() const { return m_pArray == NIL; }
		

		/**
		 * @brief Gets the length of the array.
		 * @return The length of the array.
		 */
		inline Size length() const { return m_length; }

		/**
		 * @brief Operator to set the value of an index.
		 * @param idx The index to set.
		 * @param value The value to set the index to.
		 */
		inline void set(Size idx, const T& value) { 
			CAT_DEBUG_ARRAY_CHECK_BOUNDS(idx);			
			m_pArray[idx] = value; 
		}			

	  private:
		inline void debugCheckBounds(Size idx) const {
			if (idx < 0 || idx >= m_length) {
				DERR("Array index '" << idx << "' out of bounds!  Valid range: [0, " << (m_length - 1) << "].");
			}
		}
		
		Size		 m_length;
		T*			 m_pArray;
		


	};

} // namespace Cat

#endif // CAT_CORE_UTIL_ARRAY_H
