#ifndef CAT_CORE_UTIL_PTRMAP_H
#define CAT_CORE_UTIL_PTRMAP_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file ptrmap.h
 * @brief Defines a hashmap based on using crc32 to hash strings to 32 bit integers.
 *
 * @author Catlin Zilinski
 * @date Mar 21, 2014
 */

#include <cmath>
#include "core/util/ptrnodestore.h"

namespace Cat {
	/**
	 * @class PtrMap ptrmap.h "core/util/ptrmap.h"
	 * @brief A hashmap using strings as keys hashed with a crc32 algorithm.
	 *
	 * This hashmap is to be used exclusivly with Reference counted 
	 * pointers to objects which have the oID() and name() methods implemented.  
	 * The name() is a string uniquely identifying 
	 * the object and oID() returns the crc32 hash of the name.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 21, 2014
	 */
	template <class T>
	class PtrMap {
	  public:
		/**
		 * @brief Creates an empty null map.
		 */
		PtrMap()
			: m_pBuckets(NIL), m_capacity(0), m_numBuckets(0),
			  m_pNodeStore(NIL), m_loadFactor(0.0f), m_numObjects(0) {}		

		/**
		 * @brief Creates an empty map with the specified capacity.
		 * @param capacity The initial capacity of the hashmap.
		 */
		PtrMap(Size capacity)
			: m_pBuckets(NIL), m_capacity(capacity), m_numBuckets(0),
			  m_pNodeStore(NIL), m_loadFactor(0.5f), m_numObjects(0) {
		   createMapWithCapacityAndLoadFactor(m_capacity, m_loadFactor);			
		}		

		/**
		 * @brief Creates an empty map with the specified capacity and load factor.
		 * @param capacity The initial capacity of the hashmap.
		 * @param loadFactor The initial load factor for the hashmap.
		 */
		PtrMap(Size capacity, F32 loadFactor)
			: m_pBuckets(NIL), m_capacity(capacity), m_numBuckets(0),
			  m_pNodeStore(NIL), m_loadFactor(loadFactor), m_numObjects(0) {
		   createMapWithCapacityAndLoadFactor(m_capacity, m_loadFactor);			
		}		

		/**
		 * @brief The destructor, empties the map of all the nodes.
		 */
		~PtrMap();

		/**
		 * @brief Access an element of the map via its crc32 object id.
		 * @param key The name of the element (string).
		 * @return A pointer to the element or NIL if it is not there.
		 */
		inline T& at(OID key) {
			return find(key, m_pBuckets[key % m_numBuckets])->ptr;
		}		

		/**
		 * @brief Access an element of the map via its String key.
		 * @param key The name of the element (string).
		 * @return A pointer to the element or NIL if it doesn't exist.
		 */
		inline T& at(const Char* key) { return at(crc32(key)); }

		/**
		 * @brief Get the specified bucket in the map.
		 * @param idx The bucket to get.
		 * @return A pointer to the bucket.
		 */
		inline PtrNode<T>* bucket(Size idx) { return &(m_pBuckets[idx]); }

		/**
		 * @brief Return the capacity of the map.
		 * The capacity refers to the number of occupied buckets we can have 
		 * before the map needs to be resized.
		 * @return The capacity of the map.
		 */
		inline Size capacity() const { return m_capacity; }		
			
		/**
		 * @brief Empties the map.
		 */
		void clear();

		/**
		 * @brief Tests to see if an object is in the map.
		 * @param key The OID key value of the object.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(OID key) const {
			PtrNode<T>* node = m_pBuckets[key % m_numBuckets].next;
			while (node->ptr.notNull()) {
				if (node->ptr->oID() == key) {
					return true;
				}
				node = node->next;
			}
			return false;			
		}		
		
		/**
		 * @brief Tests to see if an object is the map.
		 * @param key The string key value of the object.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(const Char* key) const {
			return contains(crc32(name));
		}		

		/**
		 * @brief Tests to see if the given object is in the map.
		 * @param obj The object to look for.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(const T& ptr) const {
			if (ptr.notNull()) {
				return contains(ptr->oID());
			}
			else {
				return false;
			}
		}

		/**
		 * @brief Access an element of the map via its crc32 object id.
		 * @param key The name of the element (string).
		 * @return A pointer to the element or NIL if it is not there.
		 */
		inline const T& get(OID key) const {
			PtrNode<T>* node = m_pBuckets[key % m_numBuckets].next;
			while (node->ptr.notNull()) {
				if (node->ptr->oID() == key) {
					return node->ptr;
				}
				node = node->next;
			}
			return m_pBuckets[key % m_numBuckets].ptr;
		}		

		/**
		 * @brief Access an element of the map via its String key.
		 * @param key The name of the element (string).
		 * @return A pointer to the element or NIL if it doesn't exist.
		 */
		inline const T& get(const Char* key) const { return get(crc32(key)); }

		/**
		 * @brief Create the initial map with the specified capacity.
		 * @param capacity The initial capacity.
		 * @param loadFactor The initial load factor to use.
		 */
		void initMapWithCapacityAndLoadFactor(Size capacity, F32 loadFactor);

		/**
		 * @brief inserts the object in the map.
		 * @param obj The object to insert into the map.
		 * @return true if the object was inserted, false if it already existed.
		 */
		inline void insert(const T& ptr);
		
		/**
		 * @brief Test if the map is empty or not
		 * @return true if the map is empty.
		 */
		inline Boolean isEmpty() const { return m_numObjects == 0; }

		/**
		 * @brief Returns the load factor for the map.
		 * @return The load factor for the map.
		 */
		inline F32 loadFactor() const { return m_loadFactor; }

			/**
		 * @brief Returns the number of buckets allocated.
		 * @return The number of allocated buckets.
		 */
		inline Size numBuckets() const { return m_numBuckets; }

		/**
		 * @brief Removes the object from the map.
		 * @param key The OID of the object to remove from the map.
		 */
		inline void remove(OID key) {
			PtrNode<T>* node = find(key, m_pBuckets[key % m_numBuckets]);
			if (node->ptr.notNull()) {
				m_pNodeStore->free(node);
			}
		}

		/**
		 * @brief Removes the object from the map.
		 * @param key The name of the object to remove from the map.
		 */
		inline void remove(const Char* name) { remove(crc32(name)); }		

		/**
		 * @brief Removes the object from the map.
		 * @param obj The object to remove from the map.
		 */
		inline void remove(const T& ptr) {
			if (ptr.notNull()) {				
				remove(ptr->oID());
			}			
		}

		/**
		 * @brief Reserves the specified capacity in the Map.  
		 * If capacity < current capacity, nothing happens.
		 * @param capacity The new capacity to have in the map.
		 */
		inline void reserve(Size capacity) {
			resizeMapToCapacity(capacity, m_loadFactor);
		}

		/**
		 * @brief Return the number of elements in the map.
		 * @return The number of elements in the map.
		 */
		inline Size size() const { m_numObjects; }

		/**
		 * @brief Removes and returns the object from the map.
		 * @param key The OID of the object to take from the map.
		 * @return The object taken from the map or NIL if it wasn't in the map.
		 */
		inline T take(OID key) {
			PtrNode<T>* node = find(key, m_pBuckets[key % m_numBuckets]);
			T ptr = node->ptr;
			if (node->ptr.notNull()) {				
				m_pNodeStore->free(node);
			}			
			return ptr;			
		}
		
		/**
		 * @brief Removes and returns the object from the map.
		 * @param key The name of the object to take from the map.
		 * @return The object taken from the map or NIL if it wasn't in the map.
		 */
		inline T take(const Char* name) { return take(crc32(name)); }		

		/**
		 * @brief Removes and returns the object from the map.
		 * @param obj The object to take from the map.
		 * @return The object taken from the map or NIL if it wasn't in the map.
		 */
		inline T take(const T& ptr) {
			if (ptr.notNull()) {				
				return take(ptr->oID());
			}
			return T::nullPtr();			
		}		

		class Iterator { 
		  public:
			inline Iterator(PtrNode<T>* rootList, Size numRoots)
				: m_pNode(NIL), m_pNext(NIL), m_pRootList(rootList),
				  m_numRoots(numRoots), m_idx(0) {
				m_pNode = m_pRootList[0].next;
				m_pNext = m_pNode;				
				findNextNode();							
			}

		   inline Boolean hasNext() {
				m_pNext->ptr.notNull();						
			}
			
			inline Boolean isValid() {
				return m_pNode->ptr.notNull();				
			}

			inline void next() { 
				m_pNode = m_pNext;
				if (hasNext()) {					
					findNextNode();
				}				
			}

			inline T& val() { 
				return m_pNode->ptr;				
			}

		  private:
			inline void findNextNode() {
				m_pNext = m_pNext->next;
				while (m_pNext->ptr.isNull() && (m_idx+1) < m_numRoots) {
					m_idx++;
					m_pNext = m_pRootList[m_idx].next;
				}
			}
			
			PtrNode<T>* m_pNode;
			PtrNode<T>* m_pNext;
			PtrNode<T*> m_RootList;
			Size		   m_numRoots;
			Size		   m_idx;			

		};

		/**
		 * @brief Get an Iterator into the Map.
		 * @return An iterator into the map.
		 */
		inline Iterator iterator() {
			return Iterator(m_pBuckets, m_numBuckets);
		}	  

	  private:
		/**
		 * @brief Resize the map to the specified capacity.
		 * @param capacity The capacity the Map should have.
		 * @param loadFactor The loadFactor for the map.
		 */
		void resizeMapToCapacity(Size capacity, F32 loadFactor);

		PtrNode<T>*		  m_pBuckets;
		Size				  m_capacity;
		Size				  m_numBuckets;
		PtrNodeStore<T>* m_pNodeStore;			
		F32				  m_loadFactor;		
		Size				  m_numObjects;
											
	};

	// ######## CONSTRUCTORS ################ //
	template <class T>
	PtrMap<T>::~PtrMap() {
		if (m_pBuckets) {
			delete[] m_pBuckets;
			m_pBuckets = NIL;
			m_numBuckets = 0;
		}
		if (m_pNodeStore) {
			delete m_pNodeStore;
			m_pNodeStore = NIL;
		}

		m_capacity = m_numBuckets = m_numObjects = 0;
		m_loadFactor = 0.0f;		
	}

	template <class T>
	void PtrMap<T>::clear() {
		if (m_pBuckets) {
			for (Size i = 0; i < m_numBuckets; i++) {
				PtrNode<T>* node = m_pBuckets[i].next;
				PtrNode<T>* next = NIL;
				while (node != &(m_pBuckets[i])) {
					next = node->next;					
					m_pNodeStore->free(node);
					node = next;					
				}
			}
			m_numObjects = 0;
		}
	}

	template <class T>
	inline void PtrMap<T>::insert(const T& ptr) {
#if defined (DEBUG)
		if (!m_pBuckets[ptr->getOID() % m_numBuckets].isEmpty()) {
			DMSG("Collision inserting key: "
				  << ptr->oID()
				  << ", into bucket: "
				  << (ptr->oID() % m_numBuckets)
				  << ", already "
				  << lengthOfBucket(m_pBuckets[ptr->oID() % m_numBuckets])
				  << " items in bucket!");
		}
#endif
		if (ptr.notNull()) {			
			if (m_numObjects >= m_capacity) {
				DMSG("Resizing map automatically. [capacity: "
					  << m_capacity << ", numObjects: "
					  << m_numObjects << "]");
				resizeMapToCapacity(m_capacity*2, m_loadFactor);		
			}
			m_pNodeStore->alloc(&(m_pBuckets[ptr->oID() % m_numBuckets]), ptr);		
			m_numObjects++;
		} else {
			DWARN("Cannot insert null pointer into map!");
		}				
	}	

	template <class T>
	void PtrMap<T>::initMapWithCapacityAndLoadFactor(Size capacity, F32 loadFactor) {
		if (!m_pBuckets || m_pNodeStore) {
			m_capacity = capacity;
			m_loadFactor = loadFactor;			
			m_numBuckets = ceil((F32)capacity / loadFactor);
			m_pBuckets = new PtrNode<T>[m_numBuckets];			
			m_pNodeStore = new PtrNodeStore<T>(m_capacity);			
			for (Size i = 0; i < m_numBuckets; i++) {
				m_pBuckets[i].initAsRoot();
			}			
		}
		else {
			DERR("Cannot call createMapWithCapacityAndLoadFactor on initialized Map!");
		}
	}

	template <class T>
	void PtrMap<T>::resizeMapToCapacity(Size capacity, F32 loadFactor) {
		// If the capacity is lower, we don't do anything.
		if (capacity > m_capacity && ceil((F32)capacity / loadFactor) > m_numBuckets) {

			// Store the old list of buckets.			
			PtrNode<T>* oldBuckets = m_pBuckets;
			Size oldNumBuckets = m_numBuckets;

			// Create the new list of buckets.
			m_numBuckets = ceil((F32)capacity / loadFactor);
			m_pBuckets = new PtrNode<T>[m_numBuckets];
			m_capacity = capacity;
			m_loadFactor = loadFactor;
			m_numObjects = 0;

			// Store old PtrNodeStore.
			PtrNodeStore<T>* oldStore = m_pNodeStore;
			
			// Create new node store
			m_pNodeStore = new PtrNodeStore<T>(m_capacity);
			// rehash
			PtrNode<T>* node;
			for (Size i = 0; i < oldNumBuckets; i++) {
				node = oldBuckets[i].next;
				while (node != &(oldBuckets[i])) {
					insert(node->ptr);
					node = node->next;					
				}
			}
			
			// Delete the old buckets and node store.
			delete[] oldBuckets;
			delete oldStore;			
		} 
		else {
			return;
		}
	}

} //namespace Cat

#endif // CAT_CORE_UTIL_PTRMAP_H
