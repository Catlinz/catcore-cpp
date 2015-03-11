#ifndef CAT_CORE_UTIL_SIMPLEQUEUE_H
#define CAT_CORE_UTIL_SIMPLEQUEUE_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file simplequeue.h
 * @brief Contains a simple fixed size queue with circular array.
 *
 * @author Catlin Zilinski
 * @date Mar 7, 2014
 */

#include "core/corelib.h"

namespace Cat {

	/**
	 * @class SimpleQueue simplequeue.h "core/util/simplequeue.h"
	 * @brief Simple fixed size queue with circular array.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 7, 2014
	 */
	template<typename T>
	class SimpleQueue {
	  public:
		class Iterator {
		  public:
			inline Iterator(SimpleQueue<T>* queue)
				: m_pQueue(queue),  m_idx(0), m_atStart(true) {
				m_idx = m_pQueue->m_start;				
			}

			inline Boolean isValid() const {
				return ( (m_idx != m_pQueue->m_end || m_atStart)  &&
							m_pQueue->m_pQueue[m_idx] != m_pQueue->nullValue());
			}			

			inline void next() {
			   m_idx = getNextIndex();
				m_atStart = false;				
			}
			
			inline Boolean hasNext() {
				U32 nextIdx = getNextIndex();				
				return ( isValid() &&
							( nextIdx != m_pQueue->m_end ) && 
							(m_pQueue->m_pQueue[nextIdx] != m_pQueue->nullValue()));
			}

			inline T& val() { return m_pQueue->m_pQueue[m_idx]; }			

		  private:
			inline U32 getNextIndex() const { return (m_idx + 1) % m_pQueue->capacity(); }							
			SimpleQueue<T>* m_pQueue;
			U32 m_idx;
			Boolean m_atStart;						
		};		

			
		/**
		 * @brief Creates an empty queue with Null array.
		 */
		SimpleQueue() : m_capacity(0), m_start(0), m_end(0), m_pQueue(NIL) {}		

		/**
		 * @brief Creates a SimpleQueue with the specified capacity.
		 * @param capacity The capacity of the queue.
		 */
		SimpleQueue(Size capacity, const T& nullValue)
			: m_capacity(capacity), m_start(0), m_end(0), m_nullValue(nullValue) {
		   createQueueWithCapacity(capacity);
		}

			/**
		 * @brief Copy constructor, copies each element.
		 * @param src The source queue to create a copy of.
		 */
		SimpleQueue(const SimpleQueue& src) {
			m_nullValue = src.nullValue();			
			createQueueWithCapacity(src.m_capacity);
		
			memcpy(m_pQueue, src.m_pQueue, sizeof(T)*m_capacity);
			m_start = src.m_start;
			m_end = src.m_end;			
		}		

		/**
		 * @brief Deletes the contents of the SimpleQueue.
		 */
		~SimpleQueue() {
			if (m_pQueue) {
				delete[] m_pQueue;
				m_pQueue = NIL;
			}
			m_start = m_end = 0;	
			m_capacity = 0;
		}
		

		/**
		 * @brief Overloaded Assignment operator, replaces contents with new contents.
		 * @param src The SimpleQueue to copy.
		 * @return A reference to this SimpleQueue.
		 */
		SimpleQueue& operator=(const SimpleQueue& src) {
			m_nullValue = src.m_nullValue;			
			if (m_pQueue && m_capacity != src.m_capacity) {
				delete[] m_pQueue;
				createQueueWithCapacity(src.m_capacity);
			}
			memcpy(m_pQueue, src.m_pQueue, sizeof(T)*m_capacity);
			m_start = src.m_start;
			m_end = src.m_end;			
					
			return *this;
		}

		/**
		 * @brief Get a reference the the specified location in the queue.
		 * @param position The position in the queue to get.
		 * @return A reference to the item or to the null item.
		 */
		inline T& at(U32 position) {
#if defined (DEBUG)
			if (position > m_capacity) {
				DERR("Index [" << position << "] out of bounds for SimpleQueue with capacity " << m_capacity << "!");
			}
#endif // DEBUG
			return m_pQueue[(m_start + position) % m_capacity];
		}

		/**
		 * @brief Get an iterator to the first element in the queue.
		 * @return An iterator to the first element in the queue.
		 */
		inline Iterator begin() {
			return Iterator(this);
		}		

		/**
		 * @brief Gets the capacity of the SimpleQueue.
		 * @return The capacity of the SimpleQueue.
		 */
		inline Size capacity() const {
			return m_capacity;
		}

		/**
		 * @brief Remove all items from the queue.
		 */
		void clear() {
			while (m_pQueue[m_start] != m_nullValue) {
				m_pQueue[m_start] = m_nullValue;				
				m_start = (m_start + 1) % m_capacity;			
			}
			m_start = m_end = 0;
		}

		/**
		 * @brief Delete all the items from the queue.
		 * Items MUST be dynamically allocated pointers.
		 */
		void eraseAll() {
			while (m_pQueue[m_start] != m_nullValue) {
				delete m_pQueue[m_start];
				m_pQueue[m_start] = NIL;
				m_start = (m_start + 1) % m_capacity;
			}
			m_start = m_end = 0;
		}
		
		

		/**
		 * @brief Get whether or not the queue has elements in it or not.
		 * @return True if the queue is empty
		 */
		inline Boolean isEmpty() const {
			return m_pQueue[m_start] == m_nullValue;
		}

		/**
		 * @brief Get whether or not the queue has room in it or not.
		 * @return True if the queue is full
		 */
		inline Boolean isFull() const {
			return m_pQueue[m_end] != m_nullValue;
		}	

		/**
		 * @brief Initialize the queue with the specified capacity and null value.
		 * @param capacity The initial capacity of the queue.
		 * @param nullValue The value considered to be NIL for the queue.
		 */
		void initQueueWithCapacityAndNull(Size capacity, const T& nullValue) {
			if (m_pQueue) {
				delete[] m_pQueue;
				m_pQueue = NIL;
			}
			m_nullValue = nullValue;
			createQueueWithCapacity(capacity);
		}
		
		/**
		 * @brief Initialize the input process queue with the specified capacity.
		 * @param capacity The initial capacity of the queue.
		 */
		inline void initWithCapacity(Size capacity, const T& nullValue) {
			initQueueWithCapacityAndNull(capacity, nullValue);			
		}
	
		

		/**
		 * @brief Get the null value for this queue.
		 * @return The null value for this queue.
		 */
		inline const T& nullValue() const { return m_nullValue; }		

		/**
		 * @brief Returns the first item in the queue, but doesn't remove it.
		 * @return A reference to the first item in the queue.
		 */
		inline T& peek() {
			return m_pQueue[m_start];
		}

		/**
		 * @brief Returns the last item in the queue.
		 * @return A reference to the last item in the queue.
		 */
		inline T& peekLast() {
			if (m_end == 0) {
				return m_pQueue[m_capacity-1];
			}
			else {				
				return m_pQueue[m_end - 1];
			}
		}

		/**
		 * @brief Pop a process off the front of the queue.
		 * @return The process from the front of the queue.
		 */
		inline T pop() {			
			T item = m_pQueue[m_start];			
			if (m_pQueue[m_start] != m_nullValue) {
				m_pQueue[m_start] = m_nullValue;
				m_start = (m_start + 1) % m_capacity;			
			}			
			return item;			
		}

		/**
		 * @brief Push an item onto the end of the queue.
		 * @param item The Item to push onto the queue.
		 * @return True if the item was put onto the queue.
		 */
		inline Boolean push(const T& item) {
			if (m_pQueue[m_end] != m_nullValue) {
				DWARN("Cannot put item onto queue, queue full!");
				return false;				
			}
			m_pQueue[m_end] = item;
			m_end = (m_end + 1) % m_capacity;
			return true;			
		}

		/**
		 * @brief Remove the first item off the queue without returning it.
		 */
		inline void remove() {
			if (m_pQueue[m_start] != m_nullValue) {
				m_pQueue[m_start] = m_nullValue;
				m_start = (m_start + 1) % m_capacity;			
			}
		}
		

	  private:
		void createQueueWithCapacity(Size capacity) {
			m_capacity = capacity;
			m_start = m_end = 0;	
			m_pQueue = new T[m_capacity];
			for (U32 i = 0; i < m_capacity; i++) {
				m_pQueue[i] = m_nullValue;
			}			
		}
		
		Size		   m_capacity;
		Size		   m_start;
		Size		   m_end;
		T           m_nullValue;		
		T*          m_pQueue;
	};

} // namepsace cc

#endif // CAT_CORE_UTIL_SIMPLEQUEUE_H

		
		
