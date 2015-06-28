#ifndef CAT_CORE_UTIL_DUALQUEUE_H
#define CAT_CORE_UTIL_DUALQUEUE_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file dualqueue.h
 * @brief Contains the DualQueue class definition.
 *
 * @author Catlin Zilinski
 * @date Mar 18, 2015
 */

#include "core/threading/spinlock.h"

namespace Cat {

	/**
	 * @class DualQueue dualqueue.h "core/util/dualqueue.h"
	 * @brief A thread-safe queue for reading and writing.
	 * 
	 * The DualQueue contains two internal queues, one for reading from 
	 * (unsynchronised) and one for writing (synchronised).  Assumes a pattern
	 * of one reader - many writers.  When reader wants to read off queue, 
	 * swaps the read and write queues and reads all items off the read queue.
	 *
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 18, 2015
	 */
	template<class T>
	class DualQueue {
	  public:
		/**
		 * @brief Create an empty queue
		 */
		DualQueue()
			: mp_queue(0), mp_read(0), mp_write(0), m_capacity(0),
			  m_rStart(0), m_rEnd(0), m_wStart(0), m_wEnd(0) {}

		/**
		 * @brief Create a new queue with the specified capacity.
		 * @param in_capacity The capacity to give the queue.
		 */
		DualQueue(int in_capacity)
			: mp_queue(0), mp_read(0), mp_write(0), m_capacity(0),
			  m_rStart(0), m_rEnd(0), m_wStart(0), m_wEnd(0) {
			initWithCapacity(in_capacity);
		}

		/**
		 * @brief Copy constructor, tries to copy the queue.
		 * @param in_src The source queue to create a copy of.
		 */
		DualQueue(const DualQueue<T>& in_src);

		/**
		 * @brief Destroy the data in the queue.
		 */
		~DualQueue();

		/**
		 * @brief Replaces contents with copy of other queue.
		 * @param in_src The DualQueue to copy from.
		 * @return A reference to this DualQueue.
		 */
		DualQueue<T>& operator=(const DualQueue<T>& in_src);

		/**
		 * @return The capacity of the DualQueue.
		 */
		inline int capacity() const {
			return m_capacity;
		}

		/**
		 * @brief Remove all items from both queues.
		 * Clear simply resets the start and end indexes of the queues.
		 */
		void clear();

		/**
		 * @brief Remove all items from the read queue (no lock).
		 * Simply resets the start and end read indexes.
		 */
		void clearRead();

		/**
		 * @brief Remove all items from the write queue (locks).
		 * Simply resets the start and end write indexes.
		 */
		void clearWrite();

		/**
		 * @brief Delete all the items from both queues.
		 * The items stored in the queues MUST be pointers to dynamically
		 * allocated objects... or boom.  Really big boom.
		 */
		inline void eraseAll();

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
		void initWithCapacity(int in_capacity);
		
		/**
		 * @return True if both queues are empty.
		 */
		Boolean isEmpty() const;

		/**
		 * @return True if the read queue is empty (no locking).
		 */
		inline Boolean isReadEmpty() const {
			return m_rStart == m_rEnd;
		}

		/**
		 * @return True if the write queue is empty (no locking).
		 */
		inline Boolean isWriteEmpty() const {
			return m_wStart == m_wEnd;
		}

		/**
		 * @return A reference to the first item in the read queue.
		 */
		inline T& peekRead() {
			return mp_read[m_rStart];
		}

		/**
		 * @return A reference to the last item in the write queue (no lock).
		 */
		inline T& peekLastWrite() {
			return (m_wEnd != m_wStart) mp_write[m_wEnd-1] ? mp_write[m_wEnd];
		}

		/**
		 * @brief Remove the first item off the read queue and return it.
		 * @return The first item off the read queue.
		 */
		inline T pop() {
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
		inline Boolean push(const T& in_item) {
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
		void swap();
		
		
	  private:
		T* mp_queue; /**< The actual queue */

		T* mp_read; /**< The queue to read from */
		T* mp_write; /**< The queue to write to */

		Spinlock m_lock;

		int m_capacity; /**< The size of each queue */
		int m_rStart; /**< First elem in read queue */
		int m_rEnd; /**< Next free pos in read queue */
		int m_wStart; /**< First elem in write queue */
		int m_wEnd;  /**< Next free pos in write queue */
	};

	template<class T>
	DualQueue<T>::DualQueue(const DualQueue<T>& in_src) {
		*this = in_src;
	}
	
	template<class T>
	DualQueue<T>::~DualQueue() {
		CC_SAFE_DELETE_ARRAY(mp_queue);
		mp_read = mp_write = 0;
		m_capacity = 0;
	}

	template<class T>
	DualQueue<T>& DualQueue<T>::operator=(const DualQueue<T>& in_src) {
		initWithCapacity(in_src.capacity());
		for (int i = 0; i < m_capacity*2; ++i) {
			mp_queue[i] = in_src.mp_queue[i];
		}

		if (in_src.mp_read == in_src.mp_queue) {
			mp_read = mp_queue;
			mp_write = &(mp_queue[m_capacity]);
		}
		else {
			mp_write = mp_queue;
			mp_read = &(mp_queue[m_capacity]);
		}

		m_rStart = in_src.m_rStart;
		m_rEnd = in_src.m_rEnd;
		m_wStart = in_src.m_wStart;
		m_wEnd = in_src.m_wEnd;
	}

	template<class T>
	void DualQueue<T>::clear() {
		clearRead();
		clearWrite();
	}

	template<class T>
	void DualQueue<T>::clearRead() {
		m_rStart = m_rEnd = 0;
	}

	template<class T>
	void DualQueue<T>::clearWrite() {
		m_lock.lock();
		m_wStart = m_wEnd = 0;
		m_lock.unlock();
	}

	template<class T>
	void DualQueue<T>::eraseAll() {
		eraseAllRead();
		eraseAllWrite();
	}

	template<class T>
	void DualQueue<T>::eraseAllRead() {
		for (int i = m_rStart; i < m_rEnd; ++i) {
			CC_SAFEDELETE(mp_read[i]);
		}
		m_rStart = m_rEnd = 0;
	}

	template<class T>
	void DualQueue<T>::eraseAllWrite() {
		m_lock.lock();
		for (int i = m_wStart; i < m_wEnd; ++i) {
			CC_SAFEDELETE(mp_write[i]);
		}
		m_wStart = m_wEnd = 0;
		m_lock.unlock();
	}

	template<class T>
	void DualQueue<T>::initWithCapacity(int in_capacity) {
		m_lock.lock();
		CC_SAFEDELETE_ARRAY(mp_queue);
		mp_queue = new T[in_capacity*2];

		mp_write = mp_queue;
		mp_read = &(mp_queue[in_capacity]);
		
		m_capacity = in_capacity;
		m_lock.unlock();
	}

	template<class T>
	Boolean DualQueue<T>::isEmpty() const {
		return isReadEmpty() && isWriteEmpty();
	}

	template<class T>
	void DualQueue<T>::swap() {
		m_lock.lock();
		/* Swap queues */
		T* tmp = mp_read;
		mp_read = mp_write;
		mp_write = tmp;
		/* Swap the read and write indices and reset write */
		m_rStart = m_wStart;
		m_rEnd = m_wEnd;
		m_wStart = m_wEnd = 0;
		m_lock.unlock();
	}
	
} // namespace Cat

#endif // CAT_CORE_UTIL_DUALQUEUE_H
