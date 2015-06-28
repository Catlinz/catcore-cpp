#ifndef CX_CORE_COMMON_CXDUALQUEUE_H
#define CX_CORE_COMMON_CXDUALQUEUE_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxDualQueue.h
 * @brief Contains the DualQueue class definition.
 *
 * @author Catlin Zilinski
 * @date June 28, 2015
 */

#include "core/threading/CxSpinlock.h"

namespace cat {

	/**
	 * @class CxDualQueue CxDualQueue.h "core/common/CxDualQueue.h"
	 * @brief A thread-safe queue for reading and writing.
	 * 
	 * The CxDualQueue contains two internal queues, one for reading from 
	 * (unsynchronised) and one for writing (synchronised).  Assumes a pattern
	 * of one reader - many writers.  When reader wants to read off queue, 
	 * swaps the read and write queues and reads all items off the read queue.
	 *
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Mar 18, 2015
	 */
	template<class T>
	class CxDualQueue {
	  public:
		/** @brief Create an empty queue */
		CxDualQueue()
			: mp_queue(0), mp_read(0), mp_write(0), m_capacity(0),
			  m_rStart(0), m_rEnd(0), m_wStart(0), m_wEnd(0) {}

		/**
		 * @brief Create a new queue with the specified capacity.
		 * @param in_capacity The capacity to give the queue.
		 */
		CxDualQueue(CxI32 in_capacity)
			: mp_queue(0), mp_read(0), mp_write(0), m_capacity(0),
			  m_rStart(0), m_rEnd(0), m_wStart(0), m_wEnd(0) {
			initWithCapacity(in_capacity);
		}

		/**
		 * @brief Copy constructor, tries to copy the queue.
		 * @param in_src The source queue to create a copy of.
		 */
		CxDualQueue(const CxDualQueue<T>& in_src);

		/**  @brief Destroy the data in the queue. */
		~CxDualQueue();

		/**
		 * @brief Replaces contents with copy of other queue.
		 * @param in_src The CxDualQueue to copy from.
		 * @return A reference to this CxDualQueue.
		 */
		CxDualQueue<T>& operator=(const CxDualQueue<T>& in_src);

		/**  @return The capacity of the CxDualQueue. */
		inline int capacity() const {
			return m_capacity;
		}

		/**
		 * @brief Remove all items from both queues.
		 * Clear simply resets the start and end indexes of the queues.
		 */
		CX_FORCE_INLINE void clear() {
			m_rStart = m_rEnd = 0;
			m_lock.lock();
			m_wStart = m_wEnd = 0;
			m_lock.unlock();
		}
		

		/**
		 * @brief Remove all items from the read queue (no lock).
		 * Simply resets the start and end read indexes.
		 */
		CX_FORCE_INLINE void clearRead() { m_rStart = m_rEnd = 0; }

		/**
		 * @brief Remove all items from the write queue (locks).
		 * Simply resets the start and end write indexes.
		 */
		CX_FORCE_INLINE void clearWrite() {
			m_lock.lock();
			m_wStart = m_wEnd = 0;
			m_lock.unlock();
		}

		/**
		 * @brief Delete all the items from both queues.
		 * The items stored in the queues MUST be pointers to dynamically
		 * allocated objects... or boom.  Really big boom.
		 */
		void eraseAll();

		/**
		 * @brief Delete all the items from the read queue (no lock)
		 * @see eraseAll().
		 */
		void eraseAllRead();

		/**
		 * @brief Delete all the items from the write queue (locks)
		 * @see eraseAll().
		 */
		void eraseAllWrite();

		/**
		 * @brief Initialise the queue with the specified capacity.
		 * @param in_capacity The capacity to give the queue.
		 */
		void initWithCapacity(CxI32 in_capacity);
		
		/** @return True if both queues are empty. */
		CX_FORCE_INLINE CxBool isEmpty() const {
			return (m_rStart == m_rEnd) && (m_wStart == m_wEnd);
		}

		/** @return True if the read queue is empty (no locking). */
		CX_FORCE_INLINE CxBool isReadEmpty() const { return m_rStart == m_rEnd; }

		/**  @return True if the write queue is empty (no locking). */
		CX_FORCE_INLINE CxBool isWriteEmpty() const { return m_wStart == m_wEnd; }

		/** @return A reference to the first item in the read queue. */
		CX_FORCE_INLINE T& peekRead() { return mp_read[m_rStart]; }

		/** @return A reference to the last item in the write queue (no lock). */
		CX_FORCE_INLINE T& peekLastWrite() {
			return (m_wEnd != m_wStart) mp_write[m_wEnd-1] ? mp_write[m_wEnd];
		}

		/**
		 * @brief Remove the first item off the read queue and return it.
		 * @return The first item off the read queue.
		 */
		CX_FORCE_INLINE T pop() {
			T item = mp_read[m_rStart];
			if (m_rStart != m_rEnd) { ++m_rStart; }
			return item;
		}

		/**
		 * @brief Put an item onto the end of the write queue.
		 * Method can failed and return false if no room left on the queue.
		 * @param in_item The item to put onto the write queue.
		 * @return True if the item was put onto the queue.
		 */
		CX_FORCE_INLINE CxBool push(const T& in_item) {
			if (m_wEnd < m_capacity) {
				m_lock.lock();
				mp_write[m_wEnd++] = in_item;
				m_lock.unlock();
				return true;
			}
			return false;
		}

		/**
		 * @brief Swap the read and write queues (Locks).
		 * Assumes read queue is empty and resets start and end indexes 
		 * for new write queue.
		 */
		CX_FORCE_INLINE void swap() {
			m_lock.lock();
			T* tmp = mp_read;
			/* Set the read indices to the old write indices. */
			m_rStart = m_wStart;
			m_rEnd = m_wEnd;

			/* Swap read queue with write queue. */
			mp_read = mp_write;

			/* Reset write indices */
			m_wStart = m_wEnd = 0;

			/* Set the write queue to the old read queue */
			mp_write = tmp;
			m_lock.unlock();
		}
		
	  private:
		T* mp_queue; /**< The actual queue */

		T* mp_read; /**< The queue to read from */
		T* mp_write; /**< The queue to write to */

		CxSpinlock m_lock;

		CxI32 m_capacity; /**< The size of each queue */
		CxI32 m_rStart; /**< First elem in read queue */
		CxI32 m_rEnd; /**< Next free pos in read queue */
		CxI32 m_wStart; /**< First elem in write queue */
		CxI32 m_wEnd;  /**< Next free pos in write queue */
	};

	template<class T>
	CxDualQueue<T>::CxDualQueue(const CxDualQueue<T>& in_src) {
		*this = in_src;
	}
	
	template<class T>
	CxDualQueue<T>::~CxDualQueue() {
	   if (mp_queue != 0) { delete[] mp_queue; mp_queue = 0; }
		mp_read = mp_write = 0;
		m_capacity = 0;
	}

	template<class T>
	CxDualQueue<T>& CxDualQueue<T>::operator=(const CxDualQueue<T>& in_src) {
		initWithCapacity(in_src.capacity());

		const int cap_two = m_capacity*2;
		for (CxI32 i = 0; i < cap_two; ++i) { mp_queue[i] = in_src.mp_queue[i]; }

		const int cap = m_capacity;
		if (in_src.mp_read == in_src.mp_queue) {
			mp_read = mp_queue;  mp_write = &(mp_queue[cap]);
		}
		else {
			mp_write = mp_queue;  mp_read = &(mp_queue[cap]);
		}

		m_rStart = in_src.m_rStart;  m_rEnd = in_src.m_rEnd;
		m_wStart = in_src.m_wStart;  m_wEnd = in_src.m_wEnd;
	}

	template<class T>
	void CxDualQueue<T>::eraseAll() {
		eraseAllRead();  eraseAllWrite();
	}

	template<class T>
	void CxDualQueue<T>::eraseAllRead() {
		const CxI32 end = m_rEnd;
		for (CxI32 i = m_rStart; i < end; ++i) {
			if (mp_read[i] != 0) { delete (mp_read[i]); mp_read[i] = 0; }
		}
		m_rStart = m_rEnd = 0;
	}

	template<class T>
	void CxDualQueue<T>::eraseAllWrite() {
		m_lock.lock();
		const CxI32 end = m_wEnd;
		for (CxI32 i = m_wStart; i < end; ++i) {
			if (mp_write[i] != 0) { delete (mp_write[i]); mp_write[i] = 0; }
		}
		m_wStart = m_wEnd = 0;
		m_lock.unlock();
	}

	template<class T>
	void CxDualQueue<T>::initWithCapacity(CxI32 in_capacity) {
		m_lock.lock();
		const CxI32 cap_two = in_capacity*2;
		if (mp_queue != 0) { delete[] mp_queue; mp_queue = 0; }

		mp_queue = new T[cap_two];
		mp_write = mp_queue;
		mp_read = &(mp_queue[in_capacity]);
		
		m_capacity = in_capacity;
		m_lock.unlock();
	}
	
} // namespace Cat

#endif // CX_CORE_COMMON_CXDUALQUEUE_H
