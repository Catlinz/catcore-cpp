#ifndef CAT_CORE_UTIL_STACK_H
#define CAT_CORE_UTIL_STACK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file stack.h
 * @brief A simple Stack template class.
 *
 * @author Catlin Zilinski
 * @date Apr 10, 2014
 */

#include "core/corelib.h"

namespace Cat {	
	/**
	 * @class Stack stack.h "core/util/stack.h"
	 * @brief A simple Stack Data structure class.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Apr 10, 2014
	 */
	template <class T>
	class Stack {
	  public:
			
		/**
		 * @brief Creates an empty null stack.
		 */
		Stack()
			: m_pStack(NIL), m_size(0), m_capacity(0) {}		

		/**
		 * @brief Creates an empty stack with the specified capacity.
		 * @param capacity The initial capacity of the stack.
		 */
		Stack(Size capacity)
			: m_pStack(NIL), m_size(0), m_capacity(0) {
			initStackWithCapacity(capacity);
		}

		/**
		 * @brief Copy constructor, copies entire stack.
		 * @param src The Stack to copy.
		 */
		Stack(const Stack<T>& src);

		/**
		 * @brief The destructor, deletes the stack memory.
		 */
		~Stack();

		/**
		 * @brief Overloaded assignment operator to copy stack.
		 * @param src The Stack to copy from.
		 * @return A reference to this Stack.
		 */
		Stack<T>& operator=(const Stack<T>& src);

		/**
		 * @brief Return the capacity of the stack before need to resize.
		 * @return The capacity of the stack.
		 */
		inline Size capacity() const { return m_capacity; }
			
		/**
		 * @brief Empties the stack but doesn't explicitly delete the objects.
		 */
		inline void clear() { m_size = 0; }		

		/**
		 * @brief Empties the stack but doesn't explicitly delete the objects.
		 * @param replacement The item to replace everything with.
		 */
		void clear(const T& replacement);

		/**
		 * @brief Erase an item from the top of the stack.
		 * This method removes and DELETES an item from the top of the stack
		 * without returning it.
		 * @return True if something was erased.
		 */
		inline Boolean erase() {
			if (m_size > 0) {
				--m_size;
				delete m_pStack[m_size];
				m_pStack[m_size] = NIL;
				return true;				
			}
			else { return false; }			
		}		

		/**
		 * @brief Erases all the objects stored in the stack (deletes them).
		 * If called on a stack containing non-pointer types... 
		 * bad things happen. SO DON'T DO IT!
		 */
		void eraseAll();

		/**
		 * @brief Create the initial stack with the specified capacity.
		 * @param capacity The initial capacity.
		 */
		void initStackWithCapacity(Size capacity);

		/**
		 * @brief Test if the stack is empty or not
		 * @return true if the stack is empty.
		 */
		inline Boolean isEmpty() const { return m_size == 0; }		

		/**
		 * @brief Access an element of the stack at a specified index.
		 * @param idx The index of the element in the stack.
		 * @return A reference to the element.
		 */
		inline T& peek(Size idx) {
			D_CONDERR(idx >= m_size, "Accessing element [ "
						 << idx << "] out of bounds!  Should be in range [0.."
						 << m_size << "]!");
			return m_pStack[idx];
		}

		/**
		 * @brief Access an constant element of the stack.
		 * @param idx The index of the element in the stack.
		 * @return A constant reference to the element.
		 */
		inline const T& peek_const(Size idx) const {
			D_CONDERR(idx >= m_size, "Accessing element [ "
						 << idx << "] out of bounds!  Should be in range [0.."
						 << m_size << "]!");
			return m_pStack[idx];
		}

		/**
		 * @brief Pop an item off the stack and return it.
		 * Always make sure to check that there is something on 
		 * the stack to pop, otherwise method return value is
		 * undefined.
		 * @return The item popped off the top of the stack.
		 */
		inline T pop() {
			if (m_size > 0) { --m_size; }			
			return m_pStack[m_size];
		}		

		/**
		 * @brief Pop an item off the stack and return it.
		 * @param replacement The item to replace the popped 
		 * item with (so we can make items null, delete them, etc.).
		 * @return The item popped off the top of the stack.
		 */
		inline T pop(const T& replacement) {
		   if (m_size > 0) { --m_size; }	
			T val = m_pStack[m_size];
			m_pStack[m_size] = replacement;
			return val;
		}		

		/**
		 * @brief Push an object onto the Stack.
		 * @param value The value to push onto the stack.
		 * @return true if the object was pushed.
		 */
		inline void push(const T& value);

		/**
		 * @brief Remove an item from the top of the stack.
		 * This method removes an item from the top of the stack
		 * without returning it.
		 * @return True if something was removed.
		 */
		inline Boolean remove() {
			if (m_size > 0) {
				--m_size;
				return true;				
			}
			else { return false; }			
		}		

		/**
		 * @brief Remove an item from the top of the stack.
		 * This method removes an item from the top of the stack
		 * without returning it.
		 * @param replacement The item to replace the popped item with, 
		 * so we can null items.
		 * @return True if something was removed.
		 */
		inline Boolean remove(const T& replacement) {
			if (m_size > 0) {
				--m_size;
				m_pStack[m_size] = replacement;
				return true;				
			}
			else { return false; }
		}		

		/**
		 * @brief Reserves the specified capacity in the Stack.  
		 * If capacity < current capacity, nothing happens.
		 * @param capacity The new capacity to have in the stack.
		 */
		inline void reserve(Size capacity) {
			resizeStackToCapacity(capacity);
		}

		/**
		 * @brief Return the number of elements in the stack.
		 * @return The number of elements in the stack.
		 */
		inline Size size() const {  return m_size; }

		/**
		 * @brief Get a reference to the top Item in the stack.
		 * @return The top of the stack.
		 */
		inline T& top() {
			D_CONDERR(m_size == 0, "Cannot access top element in empty Stack!");
			return m_pStack[m_size - 1];
		}
		

	  private:	
		/**
		 * @brief Resize the stack to the specified capacity.
		 * @param capacity The capacity the Stack should have.
		 */
		void resizeStackToCapacity(Size capacity);

		T* m_pStack;
		Size m_size;
		Size m_capacity;		
											
	};

	template <class T>
	Stack<T>::Stack(const Stack<T>& src) {
		if (src.m_capacity > 0) {
			m_pStack = new T[src.m_capacity];
			for (Size i = 0; i < src.m_size; i++) {
				m_pStack[i] = src.m_pStack[i];
			}			
		}
		else {
			m_pStack = NIL;
		}
		m_size = src.m_size;
		m_capacity = src.m_capacity;
	}
	
	template <class T>
	Stack<T>::~Stack() {
		if (m_pStack) {
			delete[] m_pStack;
			m_pStack = NIL;
		}
		m_capacity = m_size = 0;		
	}

	template <class T>
	Stack<T>& Stack<T>::operator=(const Stack<T>& src) {
		if (m_pStack) {
			delete[] m_pStack;
		}		
		if (src.m_capacity > 0) {
			m_pStack = new T[src.m_capacity];
			for (Size i = 0; i < src.m_size; i++) {
				m_pStack[i] = src.m_pStack[i];
			}			
		}
		else {
			m_pStack = NIL;
		}
		m_size = src.m_size;
		m_capacity = src.m_capacity;
		return *this;		
	}	

	template <class T>
	void Stack<T>::clear(const T& replacement) {
		for (Size i = 0; i < m_size; i++) {
			m_pStack[i] = replacement;
		}		
		clear();		
	}

	template <class T>
	void Stack<T>::eraseAll() {
		for (Size i = 0; i < m_size; i++) {
		   delete m_pStack[i];
			m_pStack[i] = NIL;
		}		
		clear();		
	}
	
	template <class T>
	inline void Stack<T>::push(const T& value) {
		if (m_size >= m_capacity) {
			DMSG("Resizing stack automatically. [capacity: "
				  << m_capacity << ", Size: "
				  << m_size << "]");
			resizeStackToCapacity(m_capacity*2);		
		}
		m_pStack[m_size] = value;
		++m_size;				
	}

	template <class T>
	void Stack<T>::initStackWithCapacity(Size capacity) {
		if (!m_pStack) {
			m_capacity = capacity;
			m_size = 0;
			m_pStack = new T[capacity];
		}
		else {
			DERR("Cannot call initStackWithCapacity on initialized Stack!");
		}
	}

	template <class T>
	void Stack<T>::resizeStackToCapacity(Size capacity) {
		// If the capacity is lower, we don't do anything.
		if (capacity > m_capacity) {
			// Store the old stack.
			T* oldStack = m_pStack;					
			Size oldSize = m_size;

			// Create the new stack.
			m_capacity = capacity;
			m_size = 0;
			m_pStack = new T[capacity];

			for (Size i = 0; i < oldSize; ++i) {
				m_pStack[i] = oldStack[i];
			}
			m_size = oldSize;			
			
			// Delete the old buckets and node store.
			delete[] oldStack;
		} 
		else {
			return;
		}
	}

} //namespace

#endif // CAT_CORE_UTIL_STACK_H
