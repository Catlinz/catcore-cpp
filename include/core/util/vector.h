#ifndef CAT_CORE_UTIL_VECTOR_H
#define CAT_CORE_UTIL_VECTOR_H

/**
 * @copyright Copyright Catlin Zilinski, 2014.  All rights reserved.
 *
 * @file vector.h
 * @brief Contains a simple Vector (resizeable array).
 *
 * @author Catlin Zilinski
 * @date Apr 5, 2014
 */
#include <cstring>
#include <cstdlib>
#include "core/util/invasivestrongptr.h"
#include "core/threading/atomic.h"

namespace Cat {

	/**
	 * @class Vector vector.h "core/util/vector.h"
	 * @brief A simple resizeable array.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Nov 1, 2013
	 */
	template<typename T>
	class Vector {
	  public:
		/**
		 * @brief Create an empty null vector.
		 */
		inline Vector()
			: m_capacity(0), m_length(0), m_pVector(NIL) {}		

		/**
		 * @brief Create a new vector with the specified number of available spaces.
		 * @param capacity The initial capacity of the Vector.
		 */
		inline Vector(Size capacity)
			: m_capacity(0), m_length(0), m_pVector(NIL) {
			initVectorWithCapacity(capacity);
		}		

		/**
		 * @brief Copy constructor, copies each element.
		 * @param src The source vector to create a copy of.
		 */
		Vector(const Vector<T>& src);

		/**
		 * @brief Deletes the contents of the Vector.
		 */
		~Vector();

		/**
		 * @brief Overloaded Assignment operator, replaces contents with new contents.
		 * @param src The Vector to copy.
		 * @return A reference to this Vector.
		 */
		Vector& operator=(const Vector<T>& src);

		/**
		 * @brief Append an empty element to simply increase the length.
		 */
		inline void append() {
			if (m_length == m_capacity) {
				DMSG("AUTO Resizing Vector from with length "
					  << m_length << " from " << m_capacity
					  << " to " << m_capacity*2);
				resizeVectorToCapacity(m_capacity*2);
			}
			++m_length;
		}
		
		
		/**
		 * @brief Append an element onto the end of the vector.
		 * @param elem The element to append to the vector.
		 */
		inline void append(const T& elem) {
			if (m_length == m_capacity) {
				DMSG("AUTO Resizing Vector from with length "
					  << m_length << " from " << m_capacity
					  << " to " << m_capacity*2);
				resizeVectorToCapacity(m_capacity*2);
			}
			m_pVector[m_length] = elem;
			++m_length;
		}

		/**
		 * @brief Append all the elements from another vector.
		 * @param src The other vector to append the elements from.
		 */
		void appendAll(const Vector<T>& src);

		/**
		 * @brief Get the vector element at the specified index.
		 * @param idx The index of the element to access.
		 * @return A reference to the element.
		 */
		inline T& at(Size idx) {
			D_CONDERR(idx >= m_length, "Accessing Vector element "
						 << idx << " outside range [0.." << m_length << "]!");
			return m_pVector[idx];
		}
		/**
		 * @deprecated
		 * Should use get() for const access.
		 */
		inline const T& at(Size idx) const {
			D_CONDERR(idx >= m_length, "Accessing Vector element "
						 << idx << " outside range [0.." << m_length << "]!");
			return m_pVector[idx];
		}		
		
		/**
		 * @brief Gets the capacity of the vector.
		 * @return The capacity of the vector.
		 */
		inline Size capacity() const { return m_capacity; }
		
		/**
		 * @brief Remove all elements from the array.
		 * This method will not result in pointers being deleted in a vector of pointers.
		 * For this behaviour, you must call eraseAll().
		 */
		inline void clear() {
			m_length = 0;
		}

		/**
		 * @brief Check to see if the vector contains a certain value.
		 * @param value The value to search for.
		 * @return True if the value is included in the vector.
		 */
		Boolean contains(const T& value) const;

		/**
		 * @brief Get a pointer to the actual array storing the data.
		 * USE CAREFULLY!
		 * @return A pointer to the actual data in the vector.
		 */
		inline T* dataPtr() const {
			return m_pVector;
		}

		/**
		 * @brief Destroys the Vector.
		 * This method destroys all the memory associated with the Vector 
		 * and resets it to an uninitialised state.
		 */
		void destroy();		

		/**
		 * @brief Remove all elements from an array and delete them. 
		 *
		 * This method assumes that the vector contains dynamically allocated 
		 * pointers, if this is not true, then calling this method will be very very bad.
		 */
		void eraseAll();

		/**
		 * @brief Removes the last element from the vector and deletes it.
		 *
		 * This method assumes that the vector is storing pointers to dynamically allocated 
		 * memory, which was allocated using the 'new' operator.  If this is not the case,
		 * then very very bad things happen if you call this method.
		 */
		void eraseLast();

		/**
		 * @brief Method to extend the active length of the Vector.
		 * @param length The length to extend the Vector to.
		 */
		void extendTo(Size length);

		/**
		 * @brief Get the vector element at the specified index.
		 * This method is for getting a read-only reference to the element.
		 * @param idx The index of the element to access.
		 * @return A const reference to the element.
		 */
		inline const T& get(Size idx) const {
			D_CONDERR(idx >= m_length, "Accessing Vector element "
						 << idx << " outside range [0.." << m_length << "]!");
			return m_pVector[idx];
		}

		/**
		 * @brief Get the index of the specified element or -1 if not found.
		 * @param elem The element to find the index of.
		 * @return The index of the element or -1 if not found.
		 */
		I32 indexOf(const T& elem) const;

		/**
		 * @brief Initialise the Vector with the specified capacity.
		 * @param capacity The initial capacity of the Vector.
		 */
		void initVectorWithCapacity(Size capacity);

		/**
		 * @brief Insert an item into the Vector.
		 * Inserting an element into the Vector is an O(n) operation, since 
		 * all the elements after the new element will have to be moved down.
		 * The index value to insert into must be within the range [0..length], 
		 * inclusive.
		 * @param idx The index to insert the element into.
		 * @param elem The element to insert.
		 */
		void insertAt(Size idx, const T& elem);		
		
		/**
		 * @brief Get the last element in the vector.
		 * @return A reference to the last element in the vector.
		 */
		inline T& last() {
			return at(m_length - 1);
		}
		
		inline const T& last() const {
			return at(m_length - 1);
		}		

		/**
		 * @brief Gets the length of the vector.
		 * @return The current length of the vector.
		 */
		inline Size length() const {
			return m_length;
		}

		/**
		 * @brief Decrement the retainCount by one.
		 * @return True if there are no more references to the Shader.
		 */
		inline Boolean release() {
		   return m_retainCount.decrement() <= 0;	
		}

		/**
		 * @brief Try and remove the specified element from the Vector.
		 * @see removeAt(Size idx);
		 * @param elem The element to try and remove from the Vector.
		 * @return True if the element was found and removed.
		 */
		inline Boolean remove(const T& elem) {
			I32 idx = indexOf(elem);
			if (idx != -1) {
				return removeAt(idx);
			}
			else {
				DWARN("Cannot remove element that does not exist!");
				return false;
			}
		}

		/**
		 * @brief Try and remove the specified element from the Vector.
		 * @see removeAt(Size idx, const T& replaceWith);
		 * @param elem The element to try and remove from the Vector.
		 * @param replaceWith The element to replace the removed element with.
		 * @return True if the element was found and removed.
		 */
		inline Boolean remove(const T& elem, const T& replaceWith) {
			I32 idx = indexOf(elem);
			if (idx != -1) {
				return removeAt(idx, replaceWith);
			}
			else {
				DWARN("Cannot remove element that does not exist!");
				return false;
			}
		}


		/**
		 * @brief Remove an arbitrary element from the Vector.
		 * This method removes an arbitrary element from the vector by 
		 * shifting all the elements after it, up by one, and then 
		 * shrinks the length by one.  This runs in O(n) time.
		 * @param idx The indx of the element to remove at.
		 * @return True if the element was removed.
		 */
		Boolean removeAt(Size idx);

		/**
		 * @brief Remove an arbitrary element from the Vector.
		 * This method removes an arbitrary element from the vector by 
		 * shifting all the elements after it, up by one, and then 
		 * shrinks the length by one.  This runs in O(n) time.
		 * @param idx The indx of the element to remove at.
		 * @parm replaceWith Basically a NIL value to be able to replace with.
		 * @return True if the element was removed.
		 */
		Boolean removeAt(Size idx, const T& replaceWith);	

		/**
		 * @brief Remove the last element from the array.
		 */
		void removeLast();

		/**
		 * @brief Reserve the specified capacity in the vector.
		 * @param capacity The amount of elements to reserve for the vector.
		 */
		void reserve(Size capacity);

		/**
		 * @brief Increase the retain count by one.
		 */
		inline void retain() {
			m_retainCount.increment();
		}

		/**
		 * @brief Get the retain count for the Shader.
		 * @return The Retain count for the shader.
		 */
		inline I32 retainCount() const {
			return m_retainCount.val();
		}

		/**
		 * @brief Set the vector element at the specified index.
		 * If the index value is greater than the current length, then 
		 * the length of the array is set to encompass the newly set element.
		 * @param idx The index of the element to set.
		 * @param value The value of the element.
		 */
		void set(Size idx, const T& value);

		/**
		 * @brief Set all the values in the vector to the specified value.
		 * This method sets all allocated values to the specified value, 
		 * i.e., from idx = 0, to idx = capacity.
		 * @param value The value to set all the values to.
		 */
		inline void setAll(const T& value) {
			for (Size i = 0; i < m_capacity; ++i) {
				m_pVector[i] = value;
			}
		}

		/**
		 * @brief Return the size / length of the vector.
		 * @return The number of elements in the vector.
		 */
		inline Size size() const {
			return m_length;
		}

		/**
		 * @brief Method to sort the vector using the given comparator
		 * This method uses the built in qsort to sort the vector.
		 * @param compar The method to use to compare two elements.
		 */
		inline void sort(I32 (*compar)(const void*, const void*)) {
			qsort(m_pVector, m_length, sizeof(T), compar);
		}		
		
		/**
		 * @brief Removes the last element from the array and returns it.
		 */
		T takeLast();

	  private:
		void resizeVectorToCapacity(Size capacity);

		Size		m_capacity;		/**< The current capacity of the Vector */
		Size		m_length;		/**< The number of elements in the Vector */
		T*			m_pVector;		/**< The actual vector data */
		AtomicI32   m_retainCount;
	};

	

	template <typename T> Vector<T>::Vector(const Vector<T>& src) {
		m_capacity = m_length = 0;
		m_pVector = NIL;
		if (src.m_capacity > 0) {			
			initVectorWithCapacity(src.m_capacity);
			for (Size i = 0; i < src.m_length; ++i) {
				m_pVector[i] = src.m_pVector[i];
			}
			m_length = src.m_length;
		}		
	}

	template <typename T> Vector<T>::~Vector() {
	   destroy();		
	}

	template <typename T> Vector<T>& Vector<T>::operator=(const Vector<T>& src) {
		if (m_pVector) {
			delete[] m_pVector;
			m_pVector = NIL;			
		}
		m_capacity = src.m_capacity;
		m_length = src.m_length;
		if (m_capacity > 0) {
			m_pVector = new T[m_capacity];
			for (Size i = 0; i < m_length; ++i) {
				m_pVector[i] = src.m_pVector[i];
			}
		}		
		return *this;
	}

	template <typename T>
	void Vector<T>::appendAll(const Vector<T>& src) {
		if (capacity() < size() + src.size()) {
			reserve(size() + src.size());
		}
		memcpy(&(m_pVector[m_length]), src.m_pVector, sizeof(T)*src.size());
		m_length += src.size();		
	}

	template <typename T>
	Boolean Vector<T>::contains(const T& value) const {
		for (Size i = 0; i < m_length; ++i) {
			if (m_pVector[i] == value) {
				return true;
			}
		}
		return false;		
	}
	

	template <typename T> void Vector<T>::destroy() {
		if (m_pVector) {
			delete[] m_pVector;
			m_pVector = NIL;
		}
		m_length = m_capacity = 0;
	}
	

	template <typename T> void Vector<T>::eraseAll() {
		for (Size i = 0; i < m_length; ++i) {
			if (m_pVector[i]) {
				delete m_pVector[i];
				m_pVector[i] = NIL;
			}
		}
		m_length = 0;
	}

	template <typename T> void Vector<T>::eraseLast() {
		if (m_length > 0) {
			delete m_pVector[m_length-1];
			--m_length;
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to erase last item from empty Vector!");
		}
#endif
	}

	template <typename T> void Vector<T>::extendTo(Size length) {
		if (length > m_capacity) {
			resizeVectorToCapacity(length);
		}
		if (m_length < length) {
			m_length = length;
		}
	}

	template <typename T> I32 Vector<T>::indexOf(const T& elem) const {
		for (Size i = 0; i < m_length; ++i) {
			if (m_pVector[i] == elem) {
				return (I32)i;
			}
		}
		return -1;
	}

	template <typename T> void Vector<T>::initVectorWithCapacity(Size capacity) {
		if (!m_pVector) {				
			m_capacity = capacity;
			m_pVector = new T[m_capacity];
		} else {
			DWARN("Cannot initialise already initialised Vector.");
		}		
	}
	
	template <typename T> void Vector<T>::insertAt(Size idx, const T& elem) {
		if (m_length == m_capacity) {
			DMSG("AUTO Resizing Vector from with length "
				  << m_length << " from " << m_capacity
				  << " to " << m_capacity*2);
			resizeVectorToCapacity(m_capacity*2);
		}
		/* Move all the elements after down one */
		for (I32 i = (I32)m_length - 1; i >= (I32)idx; --i) {
			m_pVector[i+1] = m_pVector[i];
		}
		m_pVector[idx] = elem;
		++m_length;
	}

	template <typename T> Boolean Vector<T>::removeAt(Size idx) {
		if (idx < m_length) {
			/* First, store the value we are removing, 
				so we can replace the last value we move with it. */
			T val = m_pVector[idx];

			/* Now shift everything down by one */
			for (Size i = idx + 1; i < m_length; ++i) {
				m_pVector[i-1] = m_pVector[i];
			}

			/* Replace the last element (now a duplicate of last-1)
			 * with the removed element */
			m_pVector[m_length-1] = val;

			--m_length;
			return true;			
		}
		else {
			DWARN("Cannot remove element at ["
					<< idx << "], must be within [0.."
					<< m_length-1 << "].");
			return false;			
		}		
	}

	template <typename T> Boolean Vector<T>::removeAt(Size idx,
																	  const T& replaceWith) {
		if (idx < m_length) {
			/* Shift everything up by one */
			for (Size i = idx + 1; i < m_length; ++i) {
				m_pVector[i-1] = m_pVector[i];
			}
			
			/* Replace the last element (now a duplicate of last-1)
			 * with the specified value */
			m_pVector[m_length-1] = replaceWith;
			--m_length;
			return true;			
		}
		else {
			DWARN("Cannot remove element at ["
					<< idx << "], must be within [0.."
					<< m_length-1 << "].");
			return false;			
		}		
	}
	

	template <typename T> void Vector<T>::removeLast() {
		if (m_length > 0) {
			--m_length;	
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to remove last item from empty Vector!");
		}
#endif
	}

	template <typename T> void Vector<T>::reserve(Size capacity) {
		if (capacity > m_capacity) { /* Only if reserving more than we have */
			resizeVectorToCapacity(capacity);
		}
	}			

	template <typename T> void Vector<T>::set(Size idx, const T& value) {
		if (idx >= m_length) {
			if (idx >= m_capacity) {
				DMSG("AUTO Resizing Vector with capacity "
					  << m_capacity << " to fit index "
					  << idx << ".  Resizing to " << (idx*2) << ".");
				resizeVectorToCapacity(idx*2);
			}
			m_length = idx + 1;
		}
		m_pVector[idx] = value;
	}
	
	template <typename T> T Vector<T>::takeLast() {
		if (m_length > 0) {
			T elem = last();
			--m_length;	
			return elem;
		} else {
			DWARN("Taking last element of EMPTY Vector!");			
			return at(0);
		}
	}

	template <typename T> void Vector<T>::resizeVectorToCapacity(Size capacity) {
		if (capacity > m_length) {
			m_capacity = capacity;
			T* newData = new T[m_capacity];
			if (m_pVector) {
				for (Size i = 0; i < m_length; ++i) {
					newData[i] = m_pVector[i];
				}
				delete[] m_pVector;
			}			
			m_pVector = newData;
		} 
		else {
			DWARN("Cannot resize Vector with length "
					<< m_length << " to capacity "
					<< capacity << "!");
			return;
		}
	}

} // namespace Cat

#endif // CAT_CORE_UTIL_VECTOR_H
