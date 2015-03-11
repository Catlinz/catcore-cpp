#ifndef CAT_CORE_UTIL_OBJLIST_H
#define CAT_CORE_UTIL_OBJLIST_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file objlist.h
 * @brief Contains a simple linked list implementation.
 *
 * @author Catlin Zilinski
 * @date Mar 21, 2014
 */

#include "core/memory/poolmemoryallocator.h"

namespace Cat {

	/**
	 * @class ObjLink objlist.h "core/util/objlist.h"
	 * @brief A simple ObjLink class to represent Links in a list.
	 *  
	 * This linked list is designed only to hold objects which 
	 * have an OID associated with them.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Oct 21, 2013
	 */
	template <class T>
	class ObjLink {
	  public:
		ObjLink() : m_pElem(NIL), m_pNext(NIL), m_pPrev(NIL) {}
		ObjLink(T* value) : m_pElem(value), m_pNext(NIL), m_pPrev(NIL) {}
		ObjLink(T* value, ObjLink* next, ObjLink* prev) : m_pElem(value), m_pNext(next), m_pPrev(prev) {}


		inline void remove();
		inline void destroy();

		T*				m_pElem;
		ObjLink<T>*	m_pNext;
		ObjLink<T>*	m_pPrev;

	};

	/**
	 * @class ObjList objlist.h "core/util/objlist.h"
	 * @brief A simple list implementation.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since June 26, 2013
	 */
	template <class T>
	class ObjList {
	  public:
		/**
		 * @brief Constructor just initilizes an empty list
		 */
		ObjList()
			: m_length(0) {
			m_root.m_pNext = m_root.m_pPrev = &m_root;
		}	

		/**
		 * @brief Destructor makes sure to free up all the links.
		 */
		~ObjList();

		/**
		 * @brief Tests to see if the list is empty or not.
		 * @return true if the list is empty.
		 */
		inline Boolean isEmpty() const { return (m_length == 0); }

		/**
		 * @brief Empties the linked list of all links
		 */
		void clear();

		/**
		 * @brief Empties the linked list of all items, 
		 * 
		 * Empties the linked list of all items, with the assumption
		 * that the items are pointers, so it makes sure to delete all 
		 * of them.
		 * 
		 * @param alloc The (optional) memory allocator used to allocate the elements.
		 */
		void eraseAll(MemoryAllocator* alloc = NIL);

		/**
		 * @brief Push an item onto the end of the list.
		 * @param item The item to append onto the end of the list.
		 */
		void append(T* item);

		/**
		 * @brief Pop an item off the end of the list.
		 * @return The item takeLastped off the list, or NIL if no items.
		 */
		inline T* takeLast();

		/**
		 * @brief Remove and return an item off the front of the list.
		 * @return The previously first item in the list, or NIL if no items.
		 */
		inline T* takeFirst();

		/**
		 * @brief Take an item out of the list and return that item.
		 * @param id The OID of the object to take out of the list.
		 * @return The item taken out of the list, or NIL if the item was not in the list.
		 */
		T* take(OID id);

		/**
		 * @brief Take an item out of the list and return that item.
		 * @param name The name of the object to take out of the list.
		 * @return The item taken out of the list, or NIL if the item was not in the list.
		 */
		inline T* take(const Char* name);

		/**
		 * @brief Take an item out of the list and return that item.
		 * @param obj The object to take out of the list.
		 * @return The item taken out of the list, or NIL if the item was not in the list.
		 */
		inline T* take(T* obj);

		/**
		 * @brief Remove an arbitrary item from the list by the OID of the object. (is O(n))
		 * @param id The oid of the item to remove from the list.
		 * @return 1 if the object was removed from the list, 0 otherwise.
		 */
		U32 remove(OID id);

		/**
		 * @brief Remove an arbitrary item from the list by the name of the object. (is O(n))
		 * @param name The name of the item to remove from the list.
		 * @return 1 if the object was removed from the list, 0 otherwise.
		 */
		inline U32 remove(const Char* name);

		/**
		 * @brief Remove an arbitrary item from the list (is O(n))
		 * @param item The item to remove from the list.
		 * @return 1 if the object was removed from the list, 0 otherwise.
		 */
		inline U32 remove(T* item);

		/**
		 * @brief Removes AND DELETES an arbitrary item from the list by the OID of the object. (is O(n))
		 * @param id The oid of the item to delete from the list.
		 * @param alloc The (optional) allocator used to allocate the element to erase.
		 * @return 1 if the object was deleted from the list, 0 otherwise.
		 */
		U32 erase(OID id, MemoryAllocator* alloc = NIL);

		/**
		 * @brief Removes AND DELETES an arbitrary item from the list by the name of the object. (is O(n))
		 * @param name The name of the item to delete from the list.
		 * @param alloc The (optional) allocator used to allocate the element to erase.
		 * @return 1 if the object was deleted from the list, 0 otherwise.
		 */
		inline U32 erase(const Char* name, MemoryAllocator* alloc = NIL);

		/**
		 * @brief @brief Removes AND DELETES an arbitrary item from the list. (is O(n))
		 * @param item The item to delete from the list.
		 * @param alloc The (optional) allocator used to allocate the element to erase.
		 * @return 1 if the object was deleted from the list, 0 otherwise.
		 */
		inline U32 erase(T* item, MemoryAllocator* alloc = NIL);

		/**
		 * @brief Gets the item with the specified OID, if it exists.
		 * @param id The OID of the object to find.
		 * @return A pointer to the object or NIL if it is not in the list.
		 */
		inline T* get(OID id);

		/**
		 * @brief Gets the item with the specified name, if it exists.
		 * @param name The name of the object to find.
		 * @return A pointer to the object or NIL if it is not in the list.
		 */
		inline T* get(const Char* name);


		/**
		 * @brief Checks to see if the specified object exists in the list.
		 * @param id The OID of the object to find.
		 * @return True if the object is in the list.
		 */
		inline Boolean contains(OID id) const;

		/**
		 * @brief Checks to see if the specified object exists in the list.
		 * @param name The name of the object to find.
		 * @return True if the object is in the list.
		 */
		inline Boolean contains(const Char* name) const;

		/**
		 * @brief Checks to see if the specified object exists in the list.
		 * @param obj The object to find.
		 * @return True if the object is in the list.
		 */
		inline Boolean contains(T* obj) const;


		/**
		 * @brief Get the length of the list.
		 * @return The number of items in the list
		 */
		inline Size getLength() const;

		/**
		 * @brief Sets the allocator to use for creating node entries. 
		 * This method will fail if there are any elements already in the list, 
		 * the list must be empty when this method is called.
		 * @param allocator The MemoryAllocator to use for allocating the node entries.
		 * @return true if the allocator was set successfully, false otherwise.
		 */
		Boolean setAllocator(MemoryAllocator* allocator);

		/**
		 * @brief Create and set the allocator to use for creating bucket entries. 
		 * 
		 * This method will fail if there are any elements already in the map, 
		 * the map must be empty when this method is called.
		 * If the allocator fails to be set, then the allocator will be deleted.
		 * 
		 * @param blocks The number of blocks the allocator should have.
		 * @return true if the allocator was set successfully, false otherwise.
		 */
		Boolean createAllocator(U32 blocks);

		/**
		 * @brief Returns the MemoryAllocator that the List is using.
		 * @return A pointer to the MemoryAllocator.
		 */
		inline MemoryAllocator* getAllocator();

		/**
		 * @brief Tests whether or not the list owns its allocator (if it has one).
		 * @return true if the list has and owns its allocator.
		 */
		inline Boolean doesOwnAllocator() const;

		class Iterator { 
		  public:
			Iterator() : m_pNode(NIL) {}
			Iterator(ObjLink<T>* node) : m_pNode(node) {}

			inline Boolean hasNext() {
				return m_pNode->m_pNext->m_pElem != NIL;	
			}
			inline Boolean hasPrev() {
				return m_pNode->m_pPrev->m_pElem != NIL;	
			}

			inline Boolean isValid() {
				return m_pNode->m_pElem != NIL;
			}

			inline void next() { 
				m_pNode = m_pNode->m_pNext;
			}

			inline void prev() { 
				m_pNode = m_pNode->m_pPrev;
			}

			inline T* val() { 
				return m_pNode->m_pElem;	
			}

		  private:
			ObjLink<T>*	m_pNode;
		};

		/**
		 * @brief Return an iterator to the first element.
		 * @return An iterator to the first element.
		 */
		inline Iterator getFirst();

		/**
		 * @brief Return an iterator to the last element.
		 * @return An iterator to the last element.
		 */
		inline Iterator getLast();
	  private:

		/**
		 * @brief Removes a node from the list, making sure to maintain the list.
		 * @param node The node to remove from the list.
		 */
		inline void removeNode(ObjLink<T>* node);

		ObjLink<T>			m_root;
		Size 					m_length;
		MemoryAllocator* 	m_pAllocator;
		Boolean				m_bOwnsAlloc;
	};
	
	template <class T>
	ObjList<T>::~ObjList() {
		clear();
		if (m_pAllocator && m_bOwnsAlloc) {
			delete m_pAllocator;
			m_pAllocator = NIL;
			m_bOwnsAlloc = false;
		}
	}

	template <class T>
	void ObjList<T>::clear() {
		ObjLink<T>* ptr = m_root.m_pNext;
		while(ptr->m_pElem) { /* will be null once we get to the root. */
			m_root.m_pNext = ptr->m_pNext;

			/* Delete the node itself if we have to right now. */
			if (m_pAllocator) {
				if (!m_bOwnsAlloc) { m_pAllocator->dealloc(ptr); }
			} else {
				delete ptr;
			}
			ptr = m_root.m_pNext;
		}
		if (m_pAllocator && m_bOwnsAlloc) {
			m_pAllocator->reset();
		}
		m_root.m_pNext = m_root.m_pPrev = &m_root;
		m_length = 0;
	}

	template <class T>
	void ObjList<T>::eraseAll(MemoryAllocator* alloc) {
		ObjLink<T>* ptr = m_root.m_pNext;
		while(ptr->m_pElem) { /* Will be null once we get to the root. */
			m_root.m_pNext = ptr->m_pNext;
			
			/* Delete the value stored in the node */
			if (alloc) { 
				ptr->m_pElem->~T();
				alloc->dealloc(ptr->m_pElem);
			}
			else { delete ptr->m_pElem; }

			/* Delete the node itself if we have to right now. */
			if (m_pAllocator) {
				if (!m_bOwnsAlloc) { m_pAllocator->dealloc(ptr); }
			} else {
				delete ptr;
			}
			ptr = m_root.m_pNext;

		}
		if (m_pAllocator && m_bOwnsAlloc) {
			m_pAllocator->reset();
		}
		m_root.m_pNext = m_root.m_pPrev = &m_root;
		m_length = 0;
	}

	template <class T>
	void ObjList<T>::append(T* item) {
		if (m_pAllocator) {
			m_root.m_pPrev->m_pNext = new (*m_pAllocator) ObjLink<T>(item, &m_root, m_root.m_pPrev);
		} else {
			m_root.m_pPrev->m_pNext = new ObjLink<T>(item, &m_root, m_root.m_pPrev);
		}
		m_root.m_pPrev = m_root.m_pPrev->m_pNext;
		m_length += 1;
	}

	
	template <class T>
	inline T* ObjList<T>::takeLast() {
		if (m_length > 0) {
			T* val = m_root.m_pPrev->m_pElem;
			removeNode(m_root.m_pPrev);
			m_length -= 1;
			return val;
		} else {
			return NIL;
		}
	}

	template <class T>
	inline T* ObjList<T>::takeFirst() {
		if (m_length > 0) {
			T* val = m_root.m_pNext->m_pElem;
			removeNode(m_root.m_pNext);
			m_length -= 1;
			return val;
		} else {
			return NIL;
		}
	}

	template <class T>
	T* ObjList<T>::take(OID id) {
		ObjLink<T>* ptr = m_root.m_pNext;
		while (ptr->m_pElem) {
			if (ptr->m_pElem->getOID() == id) {
				T* val = ptr->m_pElem;
				removeNode(ptr);
				m_length -= 1;
				return val;
			}
			ptr = ptr->m_pNext;
		}
		return NIL;
	}

	template <class T>
	inline T* ObjList<T>::take(const Char* name) {
		return take(crc32(name));
	}
	template <class T>
	inline T* ObjList<T>::take(T* obj) {
		return take(obj->getOID());
	}


	template <class T>
	U32 ObjList<T>::remove(OID id) {
		ObjLink<T>* ptr = m_root.m_pNext;
		while (ptr->m_pElem) {
			if (ptr->m_pElem->getOID() == id) {
				removeNode(ptr);
				m_length -= 1;
				return 1;
			}
			ptr = ptr->m_pNext;
		}
		return 0;
	}
	template <class T>
	inline U32 ObjList<T>::remove(const Char* name) {
		return remove(crc32(name));
	}
	template <class T>
	inline U32 ObjList<T>::remove(T* item) {
		return remove(item->getOID());
	}

	template <class T>
	U32 ObjList<T>::erase(OID id, MemoryAllocator* alloc) {
		ObjLink<T>* ptr = m_root.m_pNext;
		while (ptr->m_pElem) {
			if (ptr->m_pElem->getOID() == id) {
				if (alloc) {
					ptr->m_pElem->~T();
					alloc->dealloc(ptr->m_pElem);
				} else {
					delete ptr->m_pElem;
				}
				removeNode(ptr);
				m_length -= 1;
				return 1;
			}
			ptr = ptr->m_pNext;
		}
		return 0;
	}
	template <class T>
	inline U32 ObjList<T>::erase(const Char* name, MemoryAllocator* alloc) {
		return erase(crc32(name), alloc);
	}
	template <class T>
	inline U32 ObjList<T>::erase(T* item, MemoryAllocator* alloc) {
		return erase(item->getOID(), alloc);
	}

	template <class T>
	inline T* ObjList<T>::get(OID id) {
		ObjLink<T>* ptr = m_root.m_pNext;
		while (ptr->m_pElem) {
			if (ptr->m_pElem->getOID() == id) {
				return ptr->m_pElem;
			}
			ptr = ptr->m_pNext;
		}
		return NIL;
	}

	template <class T>
	inline T* ObjList<T>::get(const Char* name) {
		return get(crc32(name));
	}

	template <class T>
	inline Boolean ObjList<T>::contains(OID id) const {
		ObjLink<T>* ptr = m_root.m_pNext;
		while (ptr->m_pElem) {
			if (ptr->m_pElem->getOID() == id) {
				return true;
			}
			ptr = ptr->m_pNext;
		}
		return false;
	}

	template <class T>
	inline Boolean ObjList<T>::contains(const Char* name) const {
		return contains(crc32(name));
	}
	template <class T>
	inline Boolean ObjList<T>::contains(T* obj) const {
		return contains(obj->getOID());
	}

	template <class T>
	inline Size ObjList<T>::getLength() const {
		return m_length;
	}

	template <class T>
	inline Size len(const ObjList<T> &list) {
		return list.getLength(); 
	}

	template <class T>
	inline Size len(const ObjList<T> *list) {
		return list->getLength(); 
	}

	template <class T>
	Boolean ObjList<T>::setAllocator(MemoryAllocator* allocator) {
		if (!m_pAllocator) {	/* No existing allocator */
			m_pAllocator = allocator;
		} else if (m_length == 0) { /* Existing allocator with nothing in map */
			if (m_bOwnsAlloc) {
				delete m_pAllocator;
			}
			m_pAllocator = allocator;
		} else {
			DWARN("Cannot set allocator on non-empty ObjList!");
			return false;
		}
		m_bOwnsAlloc = false;
		return true;
	}
	template <class T>
	Boolean ObjList<T>::createAllocator(U32 blocks) {
		if ( setAllocator(new PoolMemoryAllocator(sizeof(ObjLink<T>), blocks, ObjLink<T>::getMemoryAlignment())) ) {
			m_bOwnsAlloc = true;
			return true;
		} else {
			return false;
		}
	}

	template <class T>
	inline MemoryAllocator* ObjList<T>::getAllocator() {
		return m_pAllocator;
	}
	template <class T>
	inline Boolean ObjList<T>::doesOwnAllocator() const {
		return m_pAllocator && m_bOwnsAlloc;
	}



	template <class T>
	inline typename ObjList<T>::Iterator ObjList<T>::getFirst() {
		return typename ObjList<T>::Iterator(m_root.m_pNext);
	}
	template <class T>
	inline typename ObjList<T>::Iterator ObjList<T>::getLast() {
		return typename ObjList<T>::Iterator(m_root.m_pPrev);
	}



	template <class T>
	inline void ObjList<T>::removeNode(ObjLink<T>* node) {
#if defined (DEBUG)
		if (!node->m_pNext || !node->m_pPrev) { 
			DWARN("Cannot remove node with null next or prev pointer!");
			return;
		}		
#endif
		node->m_pNext->m_pPrev = node->m_pPrev;
		node->m_pPrev->m_pNext = node->m_pNext;
		if (m_pAllocator) {
			m_pAllocator->dealloc(node);
		} else {
			delete node;
		}
	}


}


#endif // CAT_CORE_UTIL_OBJLIST_H
