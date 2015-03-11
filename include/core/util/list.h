#ifndef CAT_CORE_UTIL_LIST_H
#define CAT_CORE_UTIL_LIST_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file list.h
 * @brief Contains a simple Linked list.
 *
 * @author Catlin Zilinski
 * @date Nov 10, 2013
 */
#include <cstring>
#include "core/corelib.h"

namespace Cat {

	template<typename T>
	class cc_ListNode {
	  public:
		cc_ListNode() : m_pPrev(NIL), m_pNext(NIL) {}
		cc_ListNode(const T& data, cc_ListNode<T>* prev = NIL, cc_ListNode<T>* next = NIL) : 
			m_data(data), m_pPrev(prev), m_pNext(next) {}

		T 						m_data;
		cc_ListNode<T>*	m_pPrev;
		cc_ListNode<T>*	m_pNext;
	};

	/**
	 * @class List list.h "core/util/list.h"
	 * @brief A simple LinkedList list implementation.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Nov 10, 2013
	 */
	template<typename T>
	class List {
	  public:
		/**
		 * @brief Create an empty list.
		 */
		List();

		/**
		 * @brief Create an empty list.
		 * @param nullValue The value of NIL for the specific element type.
		 */
		List(const T& nullValue);


		/**
		 * @brief Copy constructor, copies each element.
		 * @param src The source list to create a copy of.
		 */
		List(const List<T>& src);

		/**
		 * @brief Deletes all the nodes in the list.
		 */
		~List();

		/**
		 * @brief Overloaded Assignment operator, replaces contents with new contents.
		 * @param src The List to copy.
		 * @return A reference to this List.
		 */
		List& operator=(const List<T>& src);

		/**
		 * @brief Append an element onto the end of the list.
		 * @param item The element to append to the list.
		 */
		inline void append(const T& item);

		/**
		 * @brief Get the list element at the specified index.
		 * @param idx The index of the element to access.
		 * @return A reference to the element.
		 */
		T& at(Size idx);
		const T& at(Size idx) const;

		/**
		 * @brief Remove all elements from the list.
		 *
		 * This method will not result in pointers in a list of pointers.
		 * For this behaviour, you must call eraseAll().
		 */
		void clear();

		/**
		 * @brief Test to see if an element is in the list or not.
		 * @param item The element to test to see if it is in the list.
		 * @return true if the element is in the list, false otherwise.
		 */
		Boolean contains(const T& item) const;

		/**
		 * @brief Remove all elements from a list and delete them. 
		 *
		 * This method assumes that the list contains dynamically allocated 
		 * pointers, if this is not true, then calling this method will be very very bad.
		 */
		void eraseAll();

		/**
		 * @brief Remove the specified element from the list and delete it.
		 *
		 * This method assumes that the list is storing pointers to dynamically allocated 
		 * memory, which was allocated using the 'new' operator.  If this is not the case,
		 * then very very bad things happen if you call this method.
		 *
		 * @param item The element to delete.
		 * @return True if the element existed and was deleted.
		 */
		Boolean erase(const T& item);

		/**
		 * @brief Removes the first element from the list and deletes it.
		 *
		 * This method assumes that the list is storing pointers to dynamically allocated 
		 * memory, which was allocated using the 'new' operator.  If this is not the case,
		 * then very very bad things happen if you call this method.
		 */
		void eraseFirst();

		/**
		 * @brief Removes the last element from the list and deletes it.
		 *
		 * This method assumes that the list is storing pointers to dynamically allocated 
		 * memory, which was allocated using the 'new' operator.  If this is not the case,
		 * then very very bad things happen if you call this method.
		 */
		void eraseLast();

		/**
		 * @brief Get the first element in the list.
		 * @return A reference to the first element in the list.
		 */
		inline T& first();
		inline const T& first() const;

		/**
		 * @brief Get a reference to the specified element in the list.
		 * 
		 * The behaviour of this method is undefined if the element does not 
		 * exist in the list.  Always check or be sure.
		 *
		 * @param item The element to get from the list.
		 * @return A reference to the element.
		 */
		T& get(const T& item);
		const T& get(const T& item) const;

		/**
		 * @brief Insert an element into the list.
		 * 
		 * Method just appends item to the list.  Makes it compatible with BST, 
		 * cause I'm lazy and havn't implemented one yet.
		 *
		 * @param item The element to insert into the list.
		 */
		inline void insert(const T& item);

		/**
		 * @brief Test to see if the list is empty or not.
		 * @return true if the list is empty.
		 */
		inline Boolean isEmpty() const;
			
		/**
		 * @brief Get the last element in the list.
		 * @return A reference to the last element in the list.
		 */
		inline T& last();
		inline const T& last() const;

		/**
		 * @brief Prepends an item to the front of the list.
		 * @param item The item to prepend.
		 */
		inline void prepend(const T& item);

		/**
		 * @brief Remove the specified element from the list.
		 * @param item The element to remove from the list.
		 * @return true If the element existed and was removed.
		 */
		Boolean remove(const T& item);

		/**
		 * @brief Remove the first element from the list.
		 */
		void removeFirst();

		/**
		 * @brief Remove the last element from the list.
		 */
		void removeLast();

		/**
		 * @brief Set the null value for the list.
		 * @param The null value for the list.
		 */
		inline void setNullValue(const T& nullValue) { m_root.m_data = nullValue; }		

		/**
		 * @brief Removes the specified element from the list and returns it.
		 * Behaviour is undefined if element is not in the list.
		 * @param item The item to take from the list.
		 * @return The element removed from the list.
		 */
		T take(const T& item);
		
		/**
		 * @brief Removes the first element from the list and returns it.
		 * Behaviour is undefined if list is empty.
		 * @return The element removed from the list.
		 */
		T takeFirst();

		/**
		 * @brief Removes the last element from the list and returns it.
		 * Behaviour is undefined if list is empty.
		 * @return The element removed from the list.
		 */
		T takeLast();

		/**
		 * @brief Gets the length of the list.
		 * @return The current length of the list.
		 */
		inline Size length() const;
		inline Size size() const { return m_length; }		

		/**
		 * @brief Gets the value of NIL for the elements in the list.
		 * @return The value of NIL.
		 */
		inline const T& getNullValue() const;

		class Iterator { 
		  public:
			Iterator() : m_pNode(NIL) {}
			Iterator(cc_ListNode<T>* node, cc_ListNode<T>* root) : m_pNode(node), m_pRoot(root) {}

			inline Boolean hasNext() {
				return isValid() && m_pNode->m_pNext != m_pRoot;	
			}
			inline Boolean hasPrev() {
				return isValid() && m_pNode->m_pPrev != m_pRoot;	
			}

			inline Boolean isValid() {
				return m_pNode != m_pRoot;
			}

			inline void next() { 
				m_pNode = m_pNode->m_pNext;
			}

			inline void prev() { 
				m_pNode = m_pNode->m_pPrev;
			}

			inline T& val() { 
				return m_pNode->m_data;	
			}

		  private:
			cc_ListNode<T>*	m_pNode;
			cc_ListNode<T>*	m_pRoot;
			
		};

		inline Iterator begin() { return Iterator(m_root.m_pNext, &m_root); }
		inline Iterator end() { return Iterator(m_root.m_pPrev, &m_root); }	

	  private:
		inline void removeNode(cc_ListNode<T>* node);

		Size				m_length;		/**< The number of elements in the List */
		cc_ListNode<T>	m_root;			
	};

	template <typename T> List<T>::List() {
		m_root.m_pNext = m_root.m_pPrev = &m_root;
		m_length = 0;	
	}

	template <typename T> List<T>::List(const T& nullValue) {
		m_root.m_pNext = m_root.m_pPrev = &m_root;
		m_root.m_data = nullValue;
		m_length = 0;	
	}


	template <typename T> List<T>::List(const List<T>& src) {
		m_root.m_pNext = m_root.m_pPrev = &m_root;
		m_length = 0;	

		m_root.m_data = src.m_root.m_data;
		cc_ListNode<T>* ptr = src.m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			append(ptr->m_data);
			ptr = ptr->m_pNext;
		}
	}

	template <typename T> List<T>::~List() {
		clear();	
	}

	template <typename T> List<T>& List<T>::operator=(const List<T>& src) {
		clear();
		m_root.m_data = src.m_root.m_data;
		cc_ListNode<T>* ptr = src.m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			append(ptr->m_data);
			ptr = ptr->m_pNext;
		}
		return *this;
	}

	template <typename T> inline void List<T>::append(const T& item) {
		m_root.m_pPrev->m_pNext = new cc_ListNode<T>(item, m_root.m_pPrev, &m_root);
		m_root.m_pPrev = m_root.m_pPrev->m_pNext;
		++m_length;
	}

	template <typename T> T& List<T>::at(Size idx) {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		for (Size i = 0; i < idx; i++) {
			ptr = ptr->m_pNext;
		}
		return ptr->m_data;
	}

	template <typename T> const T& List<T>::at(Size idx) const {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		for (Size i = 0; i < idx; i++) {
			ptr = ptr->m_pNext;
		}
		return ptr->m_data;
	}

	template <typename T> void List<T>::clear() {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			m_root.m_pNext = ptr->m_pNext;
			delete ptr;
			ptr = m_root.m_pNext;
		}
		m_root.m_pNext = m_root.m_pPrev = &m_root;
		m_length = 0;
	}

	template <typename T> Boolean List<T>::contains(const T& item) const {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			if (ptr->m_data == item) {
				return true;
			}
			ptr = ptr->m_pNext;
		}
		return false;
	}

	template <typename T> void List<T>::eraseAll() {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			m_root.m_pNext = ptr->m_pNext;
			delete ptr->m_data;
			delete ptr;
			ptr = m_root.m_pNext;
		}
		m_root.m_pNext = m_root.m_pPrev = &m_root;
		m_length = 0;
	}

	template <typename T> Boolean List<T>::erase(const T& item) {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			if (ptr->m_data == item) {
				delete ptr->m_data;
				removeNode(ptr);
				--m_length;
				return true;
			} else {
				ptr = ptr->m_pNext;
			}
		}
		return false;
	}
	
	template <typename T> void List<T>::eraseFirst() {
		if (m_length > 0) {
			delete m_root.m_pNext->m_data;
			removeNode(m_root.m_pNext);
			--m_length;
		} 
	}

	template <typename T> void List<T>::eraseLast() {
		if (m_length > 0) {
			delete m_root.m_pPrev->m_data;
			removeNode(m_root.m_pPrev);
			--m_length;
		}
	}

	template <typename T> inline T& List<T>::first() {
		return m_root.m_pNext->m_data;
	}

	template <typename T> inline const T& List<T>::first() const {
		return m_root.m_pNext->m_data;
	}

	template <typename T> T& List<T>::get(const T& item) {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			if (ptr->m_data == item) {
				return ptr->m_data;
			}
			ptr = ptr->m_pNext;
		}
		return m_root.m_data;
	}

	template <typename T> const T& List<T>::get(const T& item) const {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			if (ptr->m_data == item) {
				return ptr->m_data;
			}
			ptr = ptr->m_pNext;
		}
		return m_root.m_data;
	}

	template <typename T> inline void List<T>::insert(const T& item) {
		append(item);	
	}

	template <typename T> inline Boolean List<T>::isEmpty() const {
		return m_length == 0;
	}

	template <typename T> inline T& List<T>::last() {
		return m_root.m_pPrev->m_data;
	}

	template <typename T> inline const T& List<T>::last() const {
		return m_root.m_pPrev->m_data;
	}

	template <typename T> inline void List<T>::prepend(const T& item) {
		m_root.m_pNext->m_pPrev = new cc_ListNode<T>(item, &m_root, m_root.m_pNext);
		m_root.m_pNext = m_root.m_pNext->m_pPrev;
		++m_length;
	}

	template <typename T> Boolean List<T>::remove(const T& item) {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			if (ptr->m_data == item) {
				removeNode(ptr);
				--m_length;
				return true;
			} else {
				ptr = ptr->m_pNext;
			}
		}
		return false;
	}

	template <typename T> void List<T>::removeFirst() {
		if (m_length > 0) {
			removeNode(m_root.m_pNext);
			--m_length;
		}
	}

	template <typename T> void List<T>::removeLast() {
		if (m_length > 0) {
			removeNode(m_root.m_pPrev);
			--m_length;
		}
	}

	template <typename T> T List<T>::take(const T& item) {
		cc_ListNode<T>* ptr = m_root.m_pNext;
		while (ptr->m_data != m_root.m_data) {
			if (ptr->m_data == item) {
				T data = ptr->m_data;
				removeNode(ptr);
				--m_length;
				return data;
			} else {
				ptr = ptr->m_pNext;
			}
		}
		return m_root.m_data;
	}

	template <typename T> T List<T>::takeFirst() {
		if (m_length > 0) {
			T data = m_root.m_pNext->m_data;
			removeNode(m_root.m_pNext);
			--m_length;
			return data;
		} else {
			return m_root.m_data;
		}
	}

	template <typename T> T List<T>::takeLast() {
		if (m_length > 0) {
			T data = m_root.m_pPrev->m_data;
			removeNode(m_root.m_pPrev);
			--m_length;
			return data;
		} else {
			return m_root.m_data;
		}
	}

	template <typename T> inline Size List<T>::length() const {
		return m_length;
	}

	template <typename T> inline const T& List<T>::getNullValue() const {
		return m_root.m_data;
	}

	template <typename T>
	inline void List<T>::removeNode(cc_ListNode<T>* node) {
#if defined (DEBUG)
		if (!node->m_pNext || !node->m_pPrev) { 
			DWARN("Cannot remove node with null next or prev pointer!");
			return;
		}		
#endif
		node->m_pNext->m_pPrev = node->m_pPrev;
		node->m_pPrev->m_pNext = node->m_pNext;
		delete node;
	}



} // namespace Cat

#endif // CAT_CORE_UTIL_LIST_H

