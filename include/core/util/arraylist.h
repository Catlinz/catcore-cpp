#ifndef CAT_CORE_UTIL_ARRAYLIST_H
#define CAT_CORE_UTIL_ARRAYLIST_H

/**
 * @copyright Copyright Catlin Zilinski, 2014.  All rights reserved.
 *
 * @file arraylist.h
 * @brief Contains a dynamic array by use of lists of arrays.
 *
 * @author Catlin Zilinski
 * @date May 17, 2014
 */

#include <cstring>
#include <cmath>
#include "core/corelib.h"


namespace Cat {
	

	/**
	 * @class ArrayList arraylist.h "core/util/arraylist.h"
	 * @brief A simple resizeable array.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since May 17, 2014
	 */
	template<typename T>
	class ArrayList {
	  public:

		class ArrayListBlock {
		  public:
			T* array;
			Size idx;		
			Size start;
			Size end;	
			ArrayListBlock* next;
			ArrayListBlock* prev;			

			inline ArrayListBlock()
				: array(NIL), idx(0), start(0), end(0), next(NIL), prev(NIL) {}

			inline ArrayListBlock(Size pStart, Size pEnd,
										 ArrayListBlock* pNext, ArrayListBlock* pPrev)
				: array(NIL), idx(0), start(pStart), end(pEnd),
				  next(pNext), prev(pPrev) {
				array = new T[end - start + 1];				
			}

			~ArrayListBlock() {
				if (array) {
					delete[] array;
					array = NIL;
				}
				idx = start = end = 0;
				next = prev = NIL;				
			}			
								
		};
		
		
		/**
		 * @brief Create an empty uninitialized ArrayList.
		 */
		inline ArrayList()
			: m_blockSize(0), m_length(0), m_capacity(0), 
			  m_pLastAccessed(NIL) {
			m_root.next = m_root.prev = &m_root;	
		}

		/**
		 * @brief Create a new ArrayList with the specified block size.
		 * @param The size of the initial block to allocate.
		 */
		inline ArrayList(Size blockSize)
			: m_blockSize(0), m_length(0), m_capacity(0),
			  m_pLastAccessed(NIL) {
			m_root.next = m_root.prev = &m_root;
			initArrayListWithBlockSize(blockSize);			
		}

		/**
		 * @brief Copy constructor, copies each element.
		 * @param src The source ArrayList to create a copy of.
		 */
		ArrayList(const ArrayList<T>& src);

		/**
		 * @brief Deletes the contents of the ArrayList.
		 */
		~ArrayList();

		/**
		 * @brief Overloaded assignment operator.  Replaces contents.
		 * @param src The ArrayList to copy.
		 * @return A reference to this ArrayList.
		 */
		ArrayList<T>& operator=(const ArrayList<T>& src);

		/**
		 * @brief Append nothing to the ArrayList, just increment the length by one.
		 * This method doesnt actually append anything to the array list, but it 
		 * makes the next element in the list accessible with at().
		 */
		inline void append();		

		/**
		 * @brief Append an element onto the end of the ArrayList.
		 * @param elem The element to append to the ArrayList.
		 */
		inline void append(const T& elem);		

		/**
		 * @brief Get the ArrayList element at the specified index.
		 * @param idx The index of the element to access.
		 * @return A reference to the element.
		 */
		inline T& at(Size idx) {
			D_CONDERR(idx >= m_length, "Accessing ArrayList element "
						 << idx << " outside range [0.." << m_length << "]!");
			ArrayListBlock* block = m_pLastAccessed;			
			if (idx < block->start || idx > block->end) {
				block = m_root.next;				
				while (idx > block->end) {
					block = block->next;					
				}
			}
			m_pLastAccessed = block;			
			return block->array[idx - block->start];
		}
		
		/**
		 * @brief Get the ArrayList element at the specified index.
		 * @param idx The index of the element to access.
		 * @return A reference to the element.
		 */
		inline const T& at(Size idx) const {
			D_CONDERR(idx >= m_length, "Accessing ArrayList element "
						 << idx << " outside range [0.." << m_length << "]!");
			ArrayListBlock* block = m_pLastAccessed;			
			if (idx < block->start || idx > block->end) {
				block = m_root.next;				
				while (idx > block->end) {
					block = block->next;					
				}
			}
			m_pLastAccessed = block;			
			return block->array[idx - block->start];
		}

		/**
		 * @brief Gets the capacity of the ArrayList.
		 * @return The capacity of the ArrayList.
		 */
		inline Size capacity() const { return m_capacity; }

			/**
		 * @brief Remove all elements from the array.
		 * This method will not result in pointers being deleted in a array of pointers.
		 * For this behaviour, you must call eraseAll().
		 */
		void clear();

		/**
		 * @brief Destroys the ArrayList.
		 * This method destroys all the memory associated with the ArrayList 
		 * and resets it to an uninitialised state.
		 */
		void destroy();

		/**
		 * @brief Remove all elements from an array and delete them. 
		 *
		 * This method assumes that the array contains dynamically allocated 
		 * pointers, if this is not true, then calling this method will be very very bad.
		 */
		void eraseAll();

		/**
		 * @brief Removes the last element from the array and deletes it.
		 *
		 * This method assumes that the array is storing pointers to dynamically allocated 
		 * memory, which was allocated using the 'new' operator.  If this is not the case,
		 * then very very bad things happen if you call this method.
		 */
		void eraseLast();

		/**
		 * @brief Method to extend the active length of the array.
		 * @param length The length to extend the array to.
		 */
		void extendTo(Size length);

		/**
		 * @brief Get the first element of the array.
		 * @return A reference to the first element of the array.
		 */
		inline T& first() {
			D_CONDERR(m_length == 0, "Accessing first element of 0 length ArrayList!");			
			return m_root.next->array[0];
		}

		inline const T& first() const {
			D_CONDERR(m_length == 0, "Accessing first element of 0 length ArrayList!");			
			return m_root.next->array[0];
		}
		

		/**
		 * @brief Initialise the ArrayList with the specified block size.
		 * @param blockSize The blockSize for the ArrayList to have.
		 */
		void initArrayListWithBlockSize(Size blockSize);

		/**
		 * @brief Insert an item into the ArrayList.
		 * Inserting an element into the ArrayList is an O(n) operation, since 
		 * all the elements after the new element will have to be moved down.
		 * The index value to insert into must be within the range [0..length], 
		 * inclusive.
		 * @param idx The index to insert the element into.
		 * @param elem The element to insert.
		 */
		void insertAt(Size idx, const T& elem);

		/**
		 * @brief Get the last element in the ArrayList.
		 * @return A reference to the last element in the array.
		 */
		inline T& last() {
			D_CONDERR(m_length == 0, "Accessing last element of 0 length ArrayList!");			
			return m_root.prev->array[m_root.prev->idx-1];
		}

		inline const T& last() const {
			D_CONDERR(m_length == 0, "Accessing last element of 0 length ArrayList!");			
			return m_root.prev->array[m_root.prev->idx-1];
		}

		/**
		 * @brief Remove an arbitrary element from the ArrayList.
		 * This method removes an arbitrary element from the ArrayList by 
		 * shifting all the elements after it, up by one, and then 
		 * shrinks the length by one.  This runs in O(n) time.
		 * @param idx The indx of the element to remove at.
		 * @return True if the element was removed.
		 */
		Boolean removeAt(Size idx);

		/**
		 * @brief Remove an arbitrary element from the ArrayList.
		 * This method removes an arbitrary element from the ArrayList by 
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
		 * @brief Reserve the specified capacity in the array.
		 * @param capacity The amount of elements to reserve for the array.
		 */
		void reserve(Size capacity);

		/**
		 * @brief Set the array element at the specified index.
		 * If the index value is greater than the current length, then 
		 * the length of the array is set to encompass the newly set element.
		 * @param idx The index of the element to set.
		 * @param value The value of the element.
		 */
		void set(Size idx, const T& value);

		/**
		 * @brief Set all the values in the array to the specified value.
		 * This method sets all allocated values to the specified value, 
		 * i.e., from idx = 0, to idx = capacity.
		 * @param value The value to set all the values to.
		 */
		void setAll(const T& value);

		/**
		 * @brief Return the size / length of the ArrayList.
		 * @return The number of elements in the ArrayList.
		 */
		inline Size size() const { return m_length; }

		/**
		 * @brief Removes the last element from the array and returns it.
		 */
		T takeLast();
			
	  private:
		void increaseArrayListCapacityTo(Size blocks);

		inline ArrayListBlock* getBlockForIndex(Size idx) {
			ArrayListBlock* block = m_pLastAccessed;
			if (idx < block->start || idx > block->end) {
				block = m_root.next;
				while (idx > block->end) { block = block->next; }
			}
			return block;
		}

		inline ArrayListBlock* getLastBlock() {
			if (m_length >= m_root.prev->start) { return m_root.prev; }
			else {
				ArrayListBlock* block = m_root.prev;
				while (m_length < block->start) { block = block->prev; }
				return block;
			}
		}

		inline void decreaseSize() {
			if (m_length > m_root.prev->start) { --(m_root.prev->idx); }
			else {
				ArrayListBlock* block = m_root.prev->prev;
				while (m_length <= block->start) { block = block->prev; }
				--(block->idx);				
			}
			--m_length;			
		}		
		
		Size m_blockSize;
		Size m_length;
		Size m_capacity;		
	   ArrayListBlock m_root;
		ArrayListBlock* m_pLastAccessed;		
		
	};

	template <typename T> ArrayList<T>::ArrayList(const ArrayList<T>& src)
		: m_blockSize(0), m_length(0), m_capacity(0), m_pLastAccessed(NIL) {
		m_root.next = m_root.prev = &m_root;
		if (src.m_capacity > 0) {
			initArrayListWithBlockSize(src.m_capacity);
			
			ArrayListBlock* block = m_root.next;
			ArrayListBlock* srcBlock = src.m_root.next;
			while (srcBlock->array != NIL && srcBlock->idx > 0) {
				memcpy(&(block->array[block->idx]), srcBlock->array, sizeof(T)*src.m_blockSize);
				block->idx += srcBlock->idx;
				srcBlock = srcBlock->next;				
			}
			m_length = src.m_length;			
			m_pLastAccessed = m_root.next;			
		}
	}

	template <typename T> ArrayList<T>::~ArrayList() {
		destroy();
	}
	
	template <typename T> ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& src) {
		destroy();
		if (src.m_capacity > 0) {
			initArrayListWithBlockSize(src.m_capacity);
			
			ArrayListBlock* block = m_root.next;
			ArrayListBlock* srcBlock = src.m_root.next;
			while (srcBlock->array != NIL && srcBlock->idx > 0) {
				memcpy(&(block->array[block->idx]), srcBlock->array, sizeof(T)*src.m_blockSize);
				block->idx += srcBlock->idx;
				srcBlock = srcBlock->next;				
			}
			m_length = src.m_length;			
			m_pLastAccessed = m_root.next;			
		}
		return *this;		
	}

	template <typename T> inline void ArrayList<T>::append() {
		if (m_length == m_capacity) {
			DMSG("AUTO Resizing ArrayList with length "
				  << m_length << " from " << m_capacity
				  << " to " << m_capacity + m_blockSize);
			increaseArrayListCapacityTo((m_capacity / m_blockSize) + 1);				
		}
		ArrayListBlock* block = getLastBlock();	
		++(block->idx);
		++m_length;
		m_pLastAccessed = block;		
	}

	template <typename T> inline void ArrayList<T>::append(const T& elem) {
		if (m_length == m_capacity) {
			DMSG("AUTO Resizing ArrayList with length "
				  << m_length << " from " << m_capacity
				  << " to " << m_capacity + m_blockSize);
			increaseArrayListCapacityTo((m_capacity / m_blockSize) + 1);				
		}
		ArrayListBlock* block = getLastBlock();	
		block->array[block->idx++] = elem;			
		++m_length;
		m_pLastAccessed = block;		
	}
	
	
	template <typename T> void ArrayList<T>::clear() {
		m_length = 0;
		ArrayListBlock* block = m_root.next;
		while (block->array != NIL) {
			block->idx = 0;
			block = block->next;			
		}
		m_pLastAccessed = m_root.next;
	}

	template <typename T> void ArrayList<T>::destroy() {
		ArrayListBlock* block = m_root.next;
		ArrayListBlock* next = NIL;	  
		while (block->array != NIL) {
			next = block->next;			
			delete block;
			block = next;
		}

		m_blockSize = m_length = m_capacity = 0;		
		m_root.next = m_root.prev = &m_root;
		m_pLastAccessed = NIL;		
	}

	template <typename T> void ArrayList<T>::eraseAll() {
		ArrayListBlock* block = m_root.next;
		while (block->array != NIL && block->idx > 0) {
			for (Size i = 0; i < block->idx; ++i) {
				if (block->array[i]) {
					delete block->array[i];
					block->array[i] = NIL;
				}
			}
			block->idx = 0;			
			block = block->next;
		}
		m_length = 0;
		m_pLastAccessed = m_root.next;		
	}

	template <typename T> void ArrayList<T>::eraseLast() {
		if (m_length > 0) {
			ArrayListBlock* block = getLastBlock();
			if (block->idx == 0) {
				block = block->prev;
			}			
			delete block->array[block->idx-1];
			block->array[block->idx-1] = NIL;			
			--(block->idx);
			--m_length;			
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to erase last item from empty ArrayList!");
		}
#endif
	}

	template <typename T> void ArrayList<T>::extendTo(Size length) {
		if (length > m_capacity) {
			increaseArrayListCapacityTo((Size)(ceil((F32)length / (F32)m_blockSize)));
		}
		
		ArrayListBlock* block = getBlockForIndex(length - 1);		
		block->idx = length - block->start;
		block = block->prev;		
		while (block->array != NIL && block->idx != m_blockSize) {
			block->idx = m_blockSize;
			block = block->prev;			
		}
		m_length = length;		
	}

	template <typename T> void ArrayList<T>::initArrayListWithBlockSize(Size blockSize) {
		if (m_capacity == 0) {
			m_blockSize = blockSize;
			m_capacity = blockSize;			
			m_root.next = m_root.prev = new ArrayListBlock(0, blockSize-1, &m_root, &m_root);
			m_pLastAccessed = m_root.next;			
		}
		else {
			DWARN("Cannot initialise already initialised ArrayList.");
		}		
	}

	template <typename T> void ArrayList<T>::insertAt(Size idx, const T& elem) {
		if (m_length == m_capacity) {
			DMSG("AUTO Resizing ArrayList from with length "
					  << m_length << " from " << m_capacity
					  << " to " << m_capacity + m_blockSize);
			increaseArrayListCapacityTo((m_capacity / m_blockSize) + 1);
		}
		/* Move all the elements after down one */
		ArrayListBlock* block = getLastBlock();
		
		if (idx < block->start) {
			/* Move down everything in the last block */
			for (I32 i = (I32)block->idx-1; i >= 0; --i) {
				block->array[i + 1] = block->array[i];
			}
			++(block->idx);		
			block = block->prev;
			/* Move down everything in the other blocks the index is not in */
			while (idx < block->start) {
				block->next->array[0] = block->array[m_blockSize-1];			
				for (I32 i = (I32)block->idx-2; i >= 0; --i) {
					block->array[i + 1] = block->array[i];
				}
				block = block->prev;			
			}
			block->next->array[0] = block->array[m_blockSize-1];	
		}

		/* Handle the block the index is in. */
				
		for (I32 i = (I32)block->idx-1; i >= idx - block->start; --i) {
			block->array[i + 1] = block->array[i];
		}
		block->array[idx - block->start] = elem;
		m_pLastAccessed = block;		
		++m_length;
	}

	template <typename T> Boolean ArrayList<T>::removeAt(Size idx) {
		if (idx < m_length) {
			ArrayListBlock* block = getBlockForIndex(idx);
			
			/* First, store the value we are removing, 
				so we can replace the last value we move with it. */					
			T val = block->array[idx - block->start];

			/* Now shift everything down by one */
			/* first, handle the block that the index is in. */
			for (Size i = idx - block->start + 1; i < m_blockSize; ++i) {
				block->array[i - 1] = block->array[i];				
			}
			block = block->next;			

			/* Now handle the rest of the blocks */
			while (block->array != NIL && block->idx > 0) {
				block->prev->array[m_blockSize - 1] = block->array[0];				
				for (Size i = 1; i < m_blockSize; ++i) {
					block->array[i - 1] = block->array[i];				
				}
				block = block->next;				
			}

			/* Now fix up the last block */
			block = block->prev;			
			--(block->idx);
			block->array[block->idx] = val;
			--m_length;
			m_pLastAccessed = block;
			return true;			
		}
		else {
			DWARN("Cannot remove element at ["
					<< idx << "], must be within [0.."
					<< m_length-1 << "].");
			return false;	
		}		
	}

	template <typename T> Boolean ArrayList<T>::removeAt(Size idx, const T& replaceWith) {
		if (idx < m_length) {
			ArrayListBlock* block = getBlockForIndex(idx);
			
			/* Now shift everything down by one */
			/* first, handle the block that the index is in. */
			for (Size i = idx - block->start + 1; i < m_blockSize; ++i) {
				block->array[i - 1] = block->array[i];				
			}
			block = block->next;			

			/* Now handle the rest of the blocks */
			while (block->array != NIL && block->idx > 0) {
				block->prev->array[m_blockSize - 1] = block->array[0];				
				for (Size i = 1; i < m_blockSize; ++i) {
					block->array[i - 1] = block->array[i];				
				}
				block = block->next;				
			}

			/* Now fix up the last block */
			block = block->prev;			
			--(block->idx);
			block->array[block->idx] = replaceWith;
			--m_length;
			m_pLastAccessed = block;
			return true;			
		}
		else {
			DWARN("Cannot remove element at ["
					<< idx << "], must be within [0.."
					<< m_length-1 << "].");
			return false;	
		}		
	}

	template <typename T> void ArrayList<T>::removeLast() {
		D_CONDERR(m_length == 0, "Trying to remove last item from empty ArrayList!");
		decreaseSize();
	}	

	template <typename T> void ArrayList<T>::reserve(Size capacity) {
		if (capacity > m_capacity) { /* Only if reserving more than we have */
			increaseArrayListCapacityTo((Size)(ceil((F32)capacity / (F32)m_blockSize)));		
		}
	}

	template <typename T> void ArrayList<T>::set(Size idx, const T& value) {
		ArrayListBlock* block = NIL;		
		if (idx >= m_length) {
			if (idx >= m_capacity) {
				DMSG("AUTO Resizing ArrayList with capacity "
					  << m_capacity << " to fit index "
					  << idx << ".  Resizing to "
					  << (Size)(ceil((F32)(idx+1) / (F32)m_blockSize)*m_blockSize)
					  << ".");
				increaseArrayListCapacityTo((Size)(ceil((F32)(idx+1) / (F32)m_blockSize)));	
			}
			block = getLastBlock();			
		   ++(block->idx);
			block = block->prev;			
			while (block->array != NIL && block->idx < m_blockSize) {
				block->idx = m_blockSize;
				block = block->prev;				
			}			
			m_length = idx + 1;
			block = m_root.prev;			
		}
		if (block == NIL) {
			block = getBlockForIndex(idx);
		}
		block->array[idx - block->start] = value;
		m_pLastAccessed = block;
	}

	template <typename T> void ArrayList<T>::setAll(const T& value) {
		ArrayListBlock* block = m_root.next;
		while (block->array != NIL) {
			for (Size i = 0; i < m_blockSize; ++i) {
				block->array[i] = value;				
			}
			block = block->next;						
		}		
	}
	

	template <typename T> T ArrayList<T>::takeLast() {
		if (m_length > 0) {
			ArrayListBlock* block = getLastBlock();
			if (block->idx == 0) {
				block = block->prev;
			}			
			T elem = block->array[block->idx-1];
			--(block->idx);
			--m_length;
			return elem;			
		}
		else {
			DWARN("Taking last element of EMPTY ArrayList!");
			return m_root.next->array[0];
		}		
	}

	template <typename T> void ArrayList<T>::increaseArrayListCapacityTo(Size blocks) {
		I32 curBlocks = (m_capacity / m_blockSize);
		if (blocks > curBlocks) {
			m_capacity = blocks * m_blockSize;
			for (Size i = 0; i < blocks - curBlocks; ++i) {
				m_root.prev->next = new ArrayListBlock(m_root.prev->end + 1, m_root.prev->end + m_blockSize,
																	&m_root, m_root.prev);
				m_root.prev = m_root.prev->next;				
			}			
		}	  
	}
} // namespace Cat

#endif // CAT_CORE_UTIL_ARRAYLIST_H
