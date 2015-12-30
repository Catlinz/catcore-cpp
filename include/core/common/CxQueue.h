#ifndef CX_CORE_COMMON_CXQUEUE_H
#define CX_CORE_COMMON_CXQUEUE_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxQueue.h
 * @brief Contains the definintion for a simple circular Queue class.
 *
 * @author Catlin Zilinski
 * @date Nov 16, 2015
 */

#include "core/Cx.h"
#include "core/common/CxMem.h"

namespace cat {

	/**
	 * @class CxQueue CxQueue.h "core/common/CxQueue.h"
	 * @brief A simple circular queue.
	 * 
	 * The CxQueue is a simple circular queue using an array.  It can be resized,
	 * but does not automatically resize itself.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Nov 15, 2015
	 */
	template <typename T>
	class CxQueue {
	  public:
		/** @brief Creates a null queue with 0 capacity */
		CxQueue() : mp_queue(0), m_start(0), m_end(0), m_count(0), m_capacity(0) {}

		/** @brief Create a new queue with the specified capacity. */
		CxQueue(CxI32 in_capacity);

		/** @brief Copy constructor, makes copy of all the items in the queue. */
		CxQueue(const CxQueue<T> &in_src);

		/** @brief Move constructor, takes the array as its own */
		CxQueue(CxQueue<T> &&in_src);

		/** @brief Free's the memory for the queue. */
		~CxQueue();

		/** 
		 * @brief Assignment op, replaces content with src content. 
		 * The assignment operator will replace the content of this queue
		 * with the content of the source queue.  If the number of elements
		 * in the source queue is less than the current capacity of this queue, 
		 * then the queue will not be resized (no new memory allocated).
		 */
		CxQueue<T> & operator=(const CxQueue<T> &in_src);

		/** @brief Move-assign op, takes the data from the queue (no copy) */
		CxQueue<T> & operator=(CxQueue<T> &&in_src);

		/** @return The capacity of the queue */
		CX_FORCE_INLINE CxI32 capacity() const { return m_capacity; }

		/** @return Simply resets the start and end pointers */
		CX_FORCE_INLINE void clear() { m_start = m_end = 0; }

		/** @return The number of items in the queue */
		CX_FORCE_INLINE CxI32 count() const { return m_count; }

		/**
		 * @brief Expand the queue to the given size. 
		 * This method will only change the size of the queue if the new size
		 * is larger than the old size.
		 * @param in_capacity The new capacity to give to the queue.
		 */
		void expand(CxI32 in_capacity);

		/** @return True if the queue is empty. */
		CX_FORCE_INLINE CxBool isEmpty() { return m_count == 0; }

		/** @return True if the queue is full */
		CX_FORCE_INLINE CxBool isFull() { return m_count == m_capacity; }

		/** @return The number of items in the queue */
		CX_FORCE_INLINE CxI32 length() const { return m_count; }

		/** @return A reference to the first item in the queue. */
		CX_FORCE_INLINE T & peek() { return mp_queue[m_start]; }

		/** @return A const reference to the first item in the queue. */
		CX_FORCE_INLINE const T & peek() const { return mp_queue[m_start]; }

		/**
		 * @brief Remove the first item off the queue and return it.
		 * @return The first item off the queue.
		 */
		T pop();

		/**
		 * @brief Put an item onto the end of the queue.
		 * Method can fail if there is no room left on the queue.
		 * @param in_item The item to put onto the queue.
		 * @return True if the item was put onto the queue.
		 */
		CxBool push(const T &in_item);
		CxBool push(T &&in_item);

		/** @return The number of items in the queue */
		CX_FORCE_INLINE CxI32 size() const { return m_count; }

		/** @brief Set all the allocated memory to the specified byte value (default 0)*/
		CX_FORCE_INLINE void zero(CxU8 in_byte = 0) {
			if (mp_queue != 0) { mem::set(mp_queue, in_byte, sizeof(T)*m_capacity); }
		}
		
	  private:
		T *mp_queue;
		CxI32 m_start, m_end;
		CxI32 m_count, m_capacity;
		
	};

	template <typename T>
	CxQueue<T>::CxQueue(CxI32 in_capacity)
		: mp_queue(0), m_start(0), m_end(0), m_count(0), m_capacity(in_capacity) {
		mp_queue = new T[in_capacity];
	}

	template <typename T>
	CxQueue<T>::CxQueue(const CxQueue<T> &in_src)
		: mp_queue(0), m_start(0), m_end(0), m_count(0), m_capacity(0) {
		*this = in_src;
	}

	template <typename T>
	CxQueue<T>::CxQueue(CxQueue<T> &&in_src)
		: mp_queue(in_src.mp_queue), m_start(in_src.m_start), m_end(in_src.m_end),
		  m_count(in_src.m_count), m_capacity(in_src.m_capacity) {
		in_src.mp_queue = 0;
		in_src.m_start = in_src.m_end = 0;
		in_src.m_count = in_src.m_capacity = 0;
	}

	template <typename T>
	CxQueue<T>::~CxQueue() {
		if (mp_queue != 0) {
			delete[] mp_queue;  mp_queue = 0;
		}
		m_start = m_end = 0;
		m_count = m_capacity = 0;
	}

	template <typename T>
	CxQueue<T> & CxQueue<T>::operator=(const CxQueue<T> &in_src) {
	   const CxI32 src_count = in_src.m_count;
		const CxI32 src_cap = in_src.m_capacity;
		if (m_capacity < src_count) {
			if (mp_queue != 0) { delete[] mp_queue; }
			m_capacity = src_cap;
			mp_queue = new T[src_cap];
		}

		CxI32 old_i = in_src.m_start;
		for(CxI32 i = 0; i < src_count; ++i) {
			mp_queue[i] = in_src.mp_queue[old_i];
			old_i = (old_i + 1) % src_cap;
		}

		m_start = 0;
		m_end = src_count;
		m_count = src_count;

		return *this;
	}

	template <typename T>
	CxQueue<T> & CxQueue<T>::operator=(CxQueue<T> &&in_src) {
		if (mp_queue != 0) { delete[] mp_queue; }
		mp_queue = in_src.mp_queue;
		m_start = in_src.m_start;  m_end = in_src.m_end;
		m_count = in_src.m_count;  m_capacity = in_src.m_capacity;

		in_src.mp_queue = 0;
		in_src.m_start = in_src.m_end = 0;
		in_src.m_count = in_src.m_capacity = 0;

		return *this;
	}
	
	template <typename T>
	void CxQueue<T>::expand(CxI32 in_capacity) {
		const CxI32 old_cap = m_capacity;
		if (in_capacity > old_cap) {
			/* First, create storage for the new queue */
			T * new_q = new T[in_capacity];

			/* Try and use the move-assignment operator to copy the data */
			const CxI32 old_count = m_count;
			CxI32 old_i = m_start;
			for(CxI32 i = 0; i < old_count; ++i) {
				new_q[i] = static_cast<T &&>(m_queue[old_i]);
				old_i = (old_i + 1) % old_cap;
			}

			/* now set the indices and the array and delete old array */
			T * old_queue = mp_queue;
			m_start = 0;  m_end = old_count;  mp_queue = new_q;
			delete[] old_queue;
		}
	}

	template <typename T>
	CX_FORCE_INLINE T CxQueue<T>::pop() {
		if (m_count != 0) {
			const CxI32 idx = m_start;  const CxI32 cap = m_capacity;
			--m_count;
			T *item = mp_queue[idx];
			m_start = (idx + 1) % cap;
			return *item;
		}
	}

	template <typename T>
	CX_FORCE_INLINE CxBool CxQueue<T>::push(const T &in_item) {
		if (m_count < m_capacity) {
			const CxI32 idx = m_end;  const CxI32 cap = m_capacity;
			mp_queue[idx] = in_item;  m_end = (idx + 1) % cap;
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CX_FORCE_INLINE CxBool CxQueue<T>::push(T &&in_item) {
		if (m_count < m_capacity) {
			const CxI32 idx = m_end;  const CxI32 cap = m_capacity;
			mp_queue[idx] = in_item;  m_end = (idx + 1) % cap;
			return true;
		}
		else { return false; }
	}
	

} // namespace cat

#endif // CX_CORE_COMMON_CXQUEUE_H
