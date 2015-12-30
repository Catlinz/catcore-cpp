#ifndef CX_CORE_COMMON_CXDUALQUEUE_H
#define CX_CORE_COMMON_CXDUALQUEUE_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxDualQueue.h
 * @brief Contains the DualQueue class definition.
 *
 * @author Catlin Zilinski
 * @date Aug 15, 2015
 */

#include "core/Cx.h"
#include "core/common/CxMem.h"
#include "core/threading/CxSpinlock.h"
#include "core/threading/CxMutex.h"

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
	template<typename T, class L = CxSpinlock>
	class CxDualQueue {
	  public:
		/** @brief An enum of possible messages for the writers to give to the reader */
		enum Message {
			kRequestResize = 1 << 0,
			kRequestClear = 1 << 1,
			kRequestEraseAll = 1 << 2,
		};

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
			expandQueue(in_capacity);
			m_lock.initialize();
		}

		/**
		 * @brief Copy constructor, tries to copy the queue.
		 * @param in_src The source queue to create a copy of.
		 */
		CxDualQueue(const CxDualQueue<T,L>& in_src);

		/**
		 * @brief Move constructor, tries to steal the queue.
		 * @param in_src The source queue to move data from.
		 */
		CxDualQueue(CxDualQueue<T,L> &&in_src);

		/**  @brief Destroy the data in the queue. */
		~CxDualQueue();

		/**
		 * @brief Replaces contents with copy of other queue.
		 * @param in_src The CxDualQueue to copy from.
		 * @return A reference to this CxDualQueue.
		 */
		CxDualQueue<T,L>& operator=(const CxDualQueue<T,L>& in_src);

		/**
		 * @brief Steals the contents of the source queue.
		 * @param in_src The CxDualQueue to move data from.
		 * @return A reference to this CxDualQueue.
		 */
		CxDualQueue<T,L>& operator=(CxDualQueue<T,L> &&in_src);

		/**  @return The capacity of the CxDualQueue. */
		CX_FORCE_INLINE CxI32 capacity() const {
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
		 * @brief Expand the queue to a new capacity (locks).
		 * This method will only change the size of the queue 
		 * if the new capacity is greater than the old capacity.
		 * Also works on null queues to create the queue with a
		 * given initial capacity.
		 */
		void expandQueue(CxI32 in_capacity);

		/** @return A reference to the mutex or spinlock used to lock the queue. */
		CX_FORCE_INLINE L & getLock() { return m_lock; }

		/** @return True if there are any messages waiting (no lock) */
		CX_FORCE_INLINE void hasMessages() const { return m_messages != 0; }
		
		/** @return True if both queues are empty. */
		CX_FORCE_INLINE CxBool isEmpty() const {
			return (m_rStart == m_rEnd) && (m_wStart == m_wEnd);
		}

		/** @return True if the read queue is empty (no locking). */
		CX_FORCE_INLINE CxBool isReadEmpty() const { return m_rStart == m_rEnd; }

		/**  @return True if the write queue is empty (no locking). */
		CX_FORCE_INLINE CxBool isWriteEmpty() const { return m_wStart == m_wEnd; }

		/** @brief Lock the mutex / spinlock used by this queue. */
		CX_FORCE_INLINE void lock() { m_lock.lock(); }

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
		 * @brief Post a message from a reader to the writer.
		 * This gives the ability to the readers to ask the writer 
		 * to perform actions that should be done only by the writer 
		 * (i.e., that affect both the read and write queues and require locking).
		 * @param in_msg The message to post to the writer.
		 */
		CX_FORCE_INLINE void postMessage(Message in_m) {
			m_lock.lock();
			m_messages |= in_m;
			m_lock.unlock();
		}

		/**
		 * @brief Method to the writer to use to process any messages.
		 */
		void processMessages();

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
		CX_FORCE_INLINE CxBool push(const T& in_item, CxNoLock) {
			if (m_wEnd < m_capacity) { mp_write[m_wEnd++] = in_item; return true; }
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

		/** @brief Unlock the mutex / spinlock used by this queue. */
		CX_FORCE_INLINE void unlock() { m_lock.unlock(); }

		/** @brief Set all allocated memory to the specified byte value (DOESNT LOCK). */
		CX_FORCE_INLINE void zero(CxU8 in_byte = 0) {
			if (mp_queue != 0) { mem::set(mp_queue, in_byte, sizeof(T)*m_capacity*2); }
		}
		
	  private:
		T* mp_queue; /**< The actual queue */

		T* mp_read; /**< The queue to read from */
		T* mp_write; /**< The queue to write to */

		L m_lock;

		CxI32 m_capacity; /**< The size of each queue */
		CxI32 m_rStart; /**< First elem in read queue */
		CxI32 m_rEnd; /**< Next free pos in read queue */
		CxI32 m_wStart; /**< First elem in write queue */
		CxI32 m_wEnd;  /**< Next free pos in write queue */

		CxI32 m_messages;  /**< Gives the writers ability to request actions */
	};

	template<typename T, class L>
	CxDualQueue<T,L>::CxDualQueue(const CxDualQueue<T,L>& in_src)
		: mp_queue(0), mp_read(0), mp_write(0), m_capacity(0),
		  m_rStart(0), m_rEnd(0), m_wStart(0), m_wEnd(0) {
		if (in_src.mp_queue != 0) { m_lock.initialize(); }
		*this = in_src;
	}

	template<typename T, class L>
	CxDualQueue<T,L>::CxDualQueue(CxDualQueue<T,L> &&in_src)
		: mp_queue(in_src.mp_queue), mp_read(in_src.mp_read), mp_write(in_src.mp_write),
		  m_capacity(in_src.m_capacity),
		  m_rStart(in_src.m_rStart), m_rEnd(in_src.m_rEnd), m_wStart(in_src.m_wStart), m_wEnd(in_src.m_wEnd) {
		in_src.mp_queue = 0;
		if (mp_queue != 0) { m_lock.initialize(); }
	}
	
	template<typename T, class L>
	CxDualQueue<T,L>::~CxDualQueue() {
	   if (mp_queue != 0) { delete[] mp_queue; mp_queue = 0; }
		mp_read = mp_write = 0;
		m_capacity = 0;
	}

	template<typename T, class L>
	CxDualQueue<T,L>& CxDualQueue<T,L>::operator=(const CxDualQueue<T,L>& in_src) {
		const CxI32 new_cap = in_src.capacity();
		clear();
		expandQueue(new_cap);

		m_lock.lock();
		in_src.m_lock.lock();

		const CxI32 cap_two = new_cap*2;
		for (CxI32 i = 0; i < cap_two; ++i) { mp_queue[i] = in_src.mp_queue[i]; }

		if (in_src.mp_read == in_src.mp_queue) {
			mp_read = mp_queue;  mp_write = &(mp_queue[new_cap]);
		}
		else {
			mp_write = mp_queue;  mp_read = &(mp_queue[new_cap]);
		}

		m_rStart = in_src.m_rStart;  m_rEnd = in_src.m_rEnd;
		m_wStart = in_src.m_wStart;  m_wEnd = in_src.m_wEnd;

		in_src.m_lock.unlock();
		m_lock.unlock();

		return *this;
	}

	template<typename T, class L>
	CxDualQueue<T,L>& CxDualQueue<T,L>::operator=(CxDualQueue<T,L> &&in_src) {
		m_lock.lock();
		in_src.m_lock.lock();

		if (mp_queue) { delete[] mp_queue; }
		mp_queue = in_src.mp_queue;
		mp_read = in_src.mp_read;
		mp_write = in_src.mp_write;
		m_capacity = in_src.m_capacity;
		m_rStart = in_src.m_rStart;  m_rEnd = in_src.m_rEnd;
		m_wStart = in_src.m_wStart;  m_wEnd = in_src.m_wEnd;
		
		in_src.mp_queue = 0;
		in_src.mp_read = in_src.mp_write = 0;
		in_src.m_rStart = in_src.m_rEnd = 0;
		in_src.m_wStart = in_src.m_wEnd = 0;

		in_src.m_lock.unlock();
		m_lock.unlock();

		return *this;
	}

	template<typename T, class L>
	void CxDualQueue<T,L>::eraseAll() {
		eraseAllRead();  eraseAllWrite();
	}

	template<typename T, class L>
	void CxDualQueue<T,L>::eraseAllRead() {
		const CxI32 end = m_rEnd;
		for (CxI32 i = m_rStart; i < end; ++i) {
			if (mp_read[i] != 0) { delete (mp_read[i]); mp_read[i] = 0; }
		}
		m_rStart = m_rEnd = 0;
	}

	template<typename T, class L>
	void CxDualQueue<T,L>::eraseAllWrite() {
		m_lock.lock();
		const CxI32 end = m_wEnd;
		for (CxI32 i = m_wStart; i < end; ++i) {
			if (mp_write[i] != 0) { delete (mp_write[i]); mp_write[i] = 0; }
		}
		m_wStart = m_wEnd = 0;
		m_lock.unlock();
	}

	template<typename T, class L>
	void CxDualQueue<T,L>::expandQueue(CxI32 in_capacity) {
		if (in_capacity > m_capacity) {
			/* First, create the storage for the new queue */
			T * new_q = new T[in_capacity*2];

			/* Next, copy all the read data */
			T * new_rq = new_q + in_capacity;
			for (CxI32 i = m_rStart; i < m_rEnd; ++i) {
				new_rq[i] = mp_read[i];
			}

			T * old_queue = mp_queue;
		
			/* Next, copy the write queue (locks) */
			m_lock.lock();
			for (CxI32 i = 0; i < m_wEnd; ++i) {
				new_q[i] = mp_write[i];
			}
			mp_queue = mp_write = new_q;
			m_capacity = in_capacity;
			m_lock.unlock();

			mp_read = new_rq;

			if (old_queue) {
				delete[] old_queue;
			}
		}
	}

	template<typename T, class L>
	void CxDualQueue<T,L>::processMessages() {
		/* First, grab the messages */
		m_lock.lock();
		const CxI32 msgs = m_messages;
		m_messages = 0;
		m_lock.unlock();

		/* Now, process each message */
		if ((msgs & kRequestResize) != 0) {
			expandQueue( (m_capacity > 0) ? m_capacity*2 : 32 );
		}
		if ((msgs & kRequestClear) != 0) { clear(); }
		if ((msgs & kRequestEraseAll) != 0) { eraseAll(); }
	}
	
} // namespace cat

#endif // CX_CORE_COMMON_CXDUALQUEUE_H
