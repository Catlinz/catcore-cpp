#ifndef CAT_CORE_UTIL_VECTORQUEUE_H
#define CAT_CORE_UTIL_VECTORQUEUE_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file vectorqueue.h
 * @brief Contains a queue based on a resizeable array (vector).
 *
 * @author Catlin Zilinski
 * @date Mar 4, 2014
 */
#include <cstring>
#include "core/corelib.h"

namespace Cat {

	/**
	 * @class VectorQueue vectorqueue.h "core/util/vectorqueue.h"
	 * @brief A resizeable Queue using arrays and modulus.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 4, 2014
	 */
	template<typename T>
	class VectorQueue {
	  public:
		/**
		 * @brief Create an empty vector with a default initial capacity of 8.
		 */
		inline VectorQueue() {
			createVectorQueueWithInitialCapacity(8);
		}		

		/**
		 * @brief Create a new vector with the specified number of available spaces.
		 * @param capacity The initial capacity of the VectorQueue.
		 */
		inline explicit VectorQueue(Size capacity) {
			createVectorQueueWithInitialCapacity(capacity);
		}
		
		/**
		 * @brief Copy constructor, copies each element.
		 * @param src The source vector to create a copy of.
		 */
		VectorQueue(const VectorQueue<T>& src);

		/**
		 * @brief Deletes the contents of the VectorQueue.
		 */
		~VectorQueue();

		/**
		 * @brief Overloaded Assignment operator, replaces contents with new contents.
		 * @param src The VectorQueue to copy.
		 * @return A reference to this VectorQueue.
		 */
		VectorQueue& operator=(const VectorQueue<T>& src);

		/**
		 * @brief Remove all elements from the queue.
		 *
		 * This method will not result in pointers being deleted in a vector of pointers.
		 * For this behaviour, you must call eraseAll().
		 */
		void clear();

		/**
		 * @brief Remove all elements from an queue and delete them. 
		 *
		 * This method assumes that the queue contains dynamically allocated 
		 * pointers, if this is not true, then calling this method will be very very bad.
		 */
		void eraseAll();

		/**
		 * @brief Gets the index of the element
		 * @param elem The element to find.
		 * @return The index of the element or -1 if not in queue.
		 */
		I32 indexOf(const T& elem) const;				

		/**
		 * @brief Get the queue element at the specified index.
		 * @param idx The index of the element to access.
		 * @return A reference to the element.
		 */
		inline T& at(Size idx) {
#if defined (DEBUG)
			if (idx >= m_length) {
				DERR("Accessing VectorQueue element " << idx << " which is outside current VectorQueue length " << m_length << "!");
			}
#endif
			return m_pQueue[idx];
		}
		
		inline const T& at(Size idx) const {
#if defined (DEBUG)
			if (idx >= m_length) {
				DERR("Accessing VectorQueue element " << idx << " which is outside current VectorQueue length " << m_length << "!");
			}
#endif
			return m_pQueue[idx];
		}

		/**
		 * @brief Set the vector element at the specified index.
		 * If the index value is greater than the current length, then 
		 * the length of the array is set to encompass the newly set element.
		 * @param idx The index of the element to set.
		 * @param value The value of the element.
		 */
		inline void set(Size idx, const T& value);		

		/**
		 * @brief Get the last element in the vector.
		 * @return A reference to the last element in the vector.
		 */
		inline T& last();
		inline const T& last() const;

		/**
		 * @brief Append an element onto the end of the vector.
		 * @param elem The element to append to the vector.
		 */
		void append(const T& elem);

		/**
		 * @brief Remove the last element from the array.
		 */
		void removeLast();

		/**
		 * @brief Removes the last element from the array and returns it.
		 */
		T takeLast();

		/**
		 * @brief Removes the last element from the vector and deletes it.
		 *
		 * This method assumes that the vector is storing pointers to dynamically allocated 
		 * memory, which was allocated using the 'new' operator.  If this is not the case,
		 * then very very bad things happen if you call this method.
		 */
		void eraseLast();

		/**
		 * @brief Gets the capacity of the vector.
		 * @return The capacity of the vector.
		 */
		inline Size capacity() const;

		/**
		 * @brief Gets the length of the vector.
		 * @return The current length of the vector.
		 */
		inline Size length() const;



	  private:
		void createVectorQueueWithInitialCapacity(Size capacity);
		void resizeVectorQueueToCapacity(Size capacity);

		Size		m_capacity;		/**< The current capacity of the VectorQueue */
		Size		m_length;		/**< The number of elements in the VectorQueue */
		T*			m_pVectorQueue;		/**< The actual vector data */
	};

	template <typename T> VectorQueue<T>::VectorQueue(const VectorQueue<T>& src) {
		createVectorQueueWithInitialCapacity(src.m_capacity);
		for (Size i = 0; i < src.m_length; i++) {
			m_pQueue[i] = src.m_pQueue[i];
		}
		m_length = src.m_length;
	}

	template <typename T> VectorQueue<T>::~VectorQueue() {
		if (m_pQueue) {
			delete[] m_pQueue;
			m_pQueue = NIL;
		}
		m_length = 0;
		m_capacity = 0;
	}

	template <typename T> VectorQueue<T>& VectorQueue<T>::operator=(const VectorQueue<T>& src) {
		if (src.m_length > m_capacity) {
			m_capacity = src.m_capacity;
			delete[] m_pQueue;
			m_pQueue = new T[m_capacity];;
		} 
	
		for (Size i = 0; i < src.m_length; i++) {
			m_pQueue[i] = src.m_pQueue[i];
		}
		m_length = src.m_length;
		return *this;
	}


	template <typename T> void VectorQueue<T>::clear() {
		m_length = 0;
	}

	template <typename T> void VectorQueue<T>::eraseAll() {
		for (Size i = 0; i < m_length; i++) {
			if (m_pVectorQueue[i]) {
				delete m_pVectorQueue[i];
				m_pVectorQueue[i] = NIL;
			}
		}
		m_length = 0;
	}


	template <typename T> inline T& VectorQueue<T>::at(Size idx) {
#if defined (DEBUG)
		if (idx >= m_length) {
			DERR("Accessing VectorQueue element " << idx << " which is outside current VectorQueue length " << m_length << "!");
		}
#endif
		return m_pVectorQueue[idx];
	}

	template <typename T> inline const T& VectorQueue<T>::at(Size idx) const {
#if defined (DEBUG)
		if (idx >= m_length) {
			DERR("Accessing VectorQueue element " << idx << " which is outside current VectorQueue length " << m_length << "!");
		}
#endif
		return m_pVectorQueue[idx];
	}

	template <typename T> inline void VectorQueue<T>::set(Size idx, const T& value) {
		if (idx >= m_length) {
			if (idx >= m_capacity) {
				DMSG("AUTO Resizing VectorQueue with capacity " << m_capacity << " to fit index " << idx << ".  Resizing to " << (idx*2) << ".");
				resizeVectorQueueToCapacity(idx*2);
			}
			m_length = idx + 1;
		}
		m_pVectorQueue[idx] = value;
	}	

	template <typename T> inline T& VectorQueue<T>::last() {
		return at(m_length-1);
	}
	template <typename T> inline const T& VectorQueue<T>::last() const {
		return at(m_length-1);
	}

	template <typename T> void VectorQueue<T>::append(const T& elem) {
		if (m_length == m_capacity) {
			DMSG("AUTO Resizing VectorQueue from with length " << m_length << " from " << m_capacity << " to " << m_capacity*2);
			resizeVectorQueueToCapacity(m_capacity*2);
		}
		m_pVectorQueue[m_length] = elem;
		m_length++;
	}

	template <typename T> void VectorQueue<T>::removeLast() {
		if (m_length > 0) {
			m_length--;	
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to remove last item from empty VectorQueue!");
		}
#endif
	}

	template <typename T> T VectorQueue<T>::takeLast() {
		if (m_length > 0) {
			T elem = last();
			m_length--;	
			return elem;
		} else {
			return at(0);
		}
	}

	template <typename T> void VectorQueue<T>::eraseLast() {
		if (m_length > 0) {
			delete m_pVectorQueue[m_length-1];
			m_length--;
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to erase last item from empty VectorQueue!");
		}
#endif
	}

	template <typename T> inline Size VectorQueue<T>::capacity() const {
		return m_capacity;
	}

	template <typename T> inline Size VectorQueue<T>::length() const {
		return m_length;
	}

	template <typename T> void VectorQueue<T>::createVectorQueueWithInitialCapacity(Size capacity) {
		m_capacity = capacity;
		m_length = 0;
		m_pVectorQueue = new T[m_capacity];
	}

	template <typename T> void VectorQueue<T>::resizeVectorQueueToCapacity(Size capacity) {
		if (capacity > m_length) {
			m_capacity = capacity;
			T* newData = new T[m_capacity];
			memcpy(newData, m_pVectorQueue, sizeof(T)*m_length);
			delete[] m_pVectorQueue;
			m_pVectorQueue = newData;
		} 
		else {
			DWARN("Cannot resize VectorQueue with length " << m_length << " to capacity " << capacity << "!");
			return;
		}
	}

} // namespace Cat

#endif // CAT_CORE_UTIL_VECTORQUEUE_H
