#ifndef CAT_CORE_UTIL_MAP_H
#define CAT_CORE_UTIL_MAP_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file map.h
 * @brief Defines a hashmap based on using crc32 to hash strings to 32 bit integers.
 *
 * @author Catlin Zilinski
 * @date Mar 21, 2014
 */

#include <cmath>
#include "core/util/datanodepool.h"

namespace Cat {	
	/**
	 * @class Map map.h "core/util/map.h"
	 * @brief A hashmap using strings as keys hashed with a crc32 algorithm.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 21, 2013
	 */
	template <class T>
	class Map {
	  public:
		class Cell {
		  public:
			OID key;
			T value;
			
			inline Cell() : key(0) {}
			inline Cell(OID pKey, const T& pValue)
				: key(pKey), value(pValue) {}

			inline void set(OID pKey, const T& pValue) {
				key = pKey;
				value = pValue;
			}			
		};		
			
		/**
		 * @brief Creates an empty null map.
		 */
		Map()
			: m_pBuckets(NIL), m_capacity(0), m_numBuckets(0), m_pNodePool(NIL),
			  m_numObjects(0), m_loadFactor(0.0f) {}		

		/**
		 * @brief Creates an empty map with the specified capacity.
		 * @param capacity The initial capacity of the hashmap.
		 * @param nullValue The value of null for the Map types.
		 */
		Map(Size capacity, const T& nullValue)
			: m_pBuckets(NIL), m_capacity(capacity), m_numBuckets(0), m_pNodePool(NIL),
			  m_nullValue(nullValue), m_numObjects(0), m_loadFactor(0.8f) {
			initMapWithCapacityAndLoadFactor(capacity, 0.8f, nullValue);
		}		

		/**
		 * @brief Creates an empty map with the specified capacity and load factor.
		 * @param capacity The initial capacity of the hashmap.
		 * @param loadFactor The initial load factor for the hashmap.
		 * @param nullValue The value of null for the Map types.
		 */
		Map(Size capacity, F32 loadFactor, const T& nullValue)
			: m_pBuckets(NIL), m_capacity(capacity), m_numBuckets(0), m_pNodePool(NIL),
			  m_nullValue(nullValue), m_numObjects(0), m_loadFactor(loadFactor) {
			initMapWithCapacityAndLoadFactor(capacity, loadFactor, nullValue);
		}		

		/**
		 * @brief The destructor, empties the map of all the nodes.
		 */
		~Map();		
			
		/**
		 * @brief Empties the map but doesn't explicitly delete the objects stored within.
		 */
		void clear();

		/**
		 * @brief Erases all the objects stored in the map (deletes them).
		 * If called on a map containing non-pointer types... bad things happen.
		 * SO DON'T DO IT!
		 */
		void eraseAll();

		/**
		 * @brief Test if the map is empty or not
		 * @return true if the map is empty.
		 */
		inline Boolean isEmpty() const { return m_numObjects == 0; }		

		/**
		 * @brief Return the number of elements in the map.
		 * @return The number of elements in the map.
		 */
		inline Size size() const {  return m_numObjects; }			

		/**
		 * @brief Return the capacity of the map.
		 * The capacity refers to the number of occupied buckets we can have 
		 * before the map needs to be resized.
		 * @return The capacity of the map.
		 */
		inline Size capacity() const { return m_capacity; }		

		/**
		 * @brief Returns the number of buckets allocated.
		 * @return The number of allocated buckets.
		 */
		inline Size numBuckets() const { return m_numBuckets; }		

		/**
		 * @brief Returns the load factor for the map.
		 * @return The load factor for the map.
		 */
		inline F32 loadFactor() const {  return m_loadFactor; }

		/**
		 * @brief Access an element of the map via its crc32 object id.
		 * @param key The name of the element (string).
		 * @return A reference to the element or nullValue if not found.
		 */
		inline T& at(OID key) {
			DataNode<Cell>* node = find(&(m_pBuckets[key % m_numBuckets]), key);
			if (node) { return node->data.value; }
			else { return m_nullValue; }			
		}

		/**
		 * @brief Access an element of the map via its String key.
		 * @param key The name of the element (string).
		 * @return A reference to the element or nullValue if not found.
		 */
		inline T& at(const Char* key) { return at(crc32(key)); }

			/**
		 * @brief Access an element of the map via its crc32 object id.
		 * @param key The name of the element (string).
		 * @return A reference to the element or nullValue if not found.
		 */
		inline const T& get(OID key) const {
			const DataNode<Cell>* node = find_const(&(m_pBuckets[key % m_numBuckets]), key);			
			if (node) { return node->data.value; }
			else { return m_nullValue; }			
		}

		/**
		 * @brief Access an element of the map via its String key.
		 * @param key The name of the element (string).
		 * @return A reference to the element or nullValue if not found.
		 */
		inline const T& get(const Char* key) const { return get(crc32(key)); }	

		/**
		 * @brief inserts the object in the map.
		 * @param key The hashed key to insert the value into.
		 * @param value The value to insert into the map.
		 * @return true if the object was inserted.
		 */
		inline void insert(OID key, const T& value);

		/**
		 * @brief inserts the object in the map.
		 * @param key The String key to insert the value into.
		 * @param value The value to insert into the map.
		 * @return true if the object was inserted.
		 */
		inline void insert(const Char* key, const T& value) {
			return insert(crc32(key), value);
		}		

		/**
		 * @brief Removes and returns the object from the map.
		 * @param key The OID of the object to take from the map.
		 * @return The object taken from the map or NIL if it wasn't in the map.
		 */
		inline T take(OID key) {
			DataNode<Cell>* node = find(&(m_pBuckets[key % m_numBuckets]), key);
			if (node) {				
				T val = node->data.value;
				node->data.set(0, m_nullValue);	
				m_pNodePool->free(node);
				m_numObjects--;				
				return val;				
			}
			else { return m_nullValue; }
		}

		/**
		 * @brief Removes and returns the object from the map.
		 * @param key The name of the object to take from the map.
		 * @return The object taken from the map or NIL if it wasn't in the map.
		 */
		inline T take(const Char* name) { return take(crc32(name)); }
		
		/**
		 * @brief Removes and returns the object from the map.
		 * Will only remove the first occurance of the object it finds.
		 * @param value The value to take from the map.
		 * @return The value taken from the map or NullValue if it wasn't in the map.
		 */
		T take(const T& value);		

		/**
		 * @brief Removes the object from the map.
		 * @param key The OID of the object to remove from the map.
		 */
		inline void remove(OID key) {
			DataNode<Cell>* node = find(&(m_pBuckets[key % m_numBuckets]), key);
			if (node) {
				node->data.set(0, m_nullValue);	
				m_pNodePool->free(node);
				m_numObjects--;				
			}
		}

		/**
		 * @brief Removes the object from the map.
		 * @param key The name of the object to remove from the map.
		 */
		inline void remove(const Char* key) { remove(crc32(key)); }		

		/**
		 * @brief Removes the object from the map.
		 * Only removes the first occurance of the value found in the map.
		 * @param obj The object to remove from the map.
		 */
		void remove(const T& value);		

		/**
		 * @brief Removes AND DELETES the object from the map.
		 * Calling this method on a Map that does not contain pointers, is BAD!
		 * @param key The OID of the object to delete from the map.
		 */
		inline void erase(OID key) {
			DataNode<Cell>* node = find(&(m_pBuckets[key % m_numBuckets]), key);
			if (node) {
				delete (node->data.value);				
				node->data.set(0, m_nullValue);	
				m_pNodePool->free(node);
				m_numObjects--;				
			}
		}

		/**
		 * @brief Removes AND DELETES the object from the map.
		 * Calling this method on a Map that does not contain pointers, is BAD!
		 * @param key The name of the object to delete from the map.
		 */
		inline void erase(const Char* key) { erase(crc32(key)); }		

		/**
		 * @brief Removes AND DELETES the object from the map.
		 * Calling this method on a Map that does not contain pointers, is BAD!
		 * This method will only delete the first occurance of a value in the map.
		 * @param obj The object to delete from the map.
		 */
		void erase(const T& value);		

		/**
		 * @brief Tests to see if an object is in the map.
		 * @param key The OID key value of the object.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(OID key) const {
			return find_const(&(m_pBuckets[key % m_numBuckets]), key) != NIL;
		}
		
		/**
		 * @brief Tests to see if an object is the map.
		 * @param key The string key value of the object.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(const Char* key) const {
			return contains(crc32(key));
		}		

		/**
		 * @brief Tests to see if the given object is in the map.
		 * @param value The value to look for.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(const T& value) const;		

		/**
		 * @brief Reserves the specified capacity in the Map.  
		 * If capacity < current capacity, nothing happens.
		 * @param capacity The new capacity to have in the map.
		 */
		inline void reserve(Size capacity) {
			resizeMapToCapacity(capacity, m_loadFactor);
		}

		/**
		 * @brief Get the null value for the types in this map.
		 * @return The null value for the types in this map.
		 */
		inline const T& nullValue() const { return m_nullValue; }		

		class Iterator { 
		  public:
			inline Iterator(DataNode<Cell>* rootList, Size numRoots)
				: m_pNode(NIL), m_pNext(NIL), m_pRootList(rootList),
				  m_numRoots(numRoots), m_idx(0) {
				if (m_pRootList) {					
					m_pNode = m_pRootList[0].next;
					m_pNext = m_pNode;				
					findNextNode();
					if (m_pNode->isRoot) {
						next();
					}
				}				
			}

		   inline Boolean hasNext() { return m_pNext && !m_pNext->isRoot; }			
			inline Boolean isValid() { return m_pNode && !m_pNode->isRoot; }

			inline void next() { 
				m_pNode = m_pNext;
				if (hasNext()) {					
					findNextNode();
				}				
			}

			inline T& val() { return m_pNode->data.value; }
			inline OID key() { return m_pNode->data.key; }			

		  private:
			inline void findNextNode() {
				m_pNext = m_pNext->next;
				while (m_pNext->isRoot && (m_idx+1) < m_numRoots) {
					m_idx++;
					m_pNext = m_pRootList[m_idx].next;
				}
			}
			
			DataNode<Cell>* m_pNode;
			DataNode<Cell>* m_pNext;
			DataNode<Cell>* m_pRootList;
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

		/**
		 * @brief Create the initial map with the specified capacity.
		 * @param capacity The initial capacity.
		 * @param loadFactor The initial load factor to use.
		 * @param nullValue The null value for the types in the Map.
		 */
		void initMapWithCapacityAndLoadFactor(Size capacity, F32 loadFactor,
														  const T& nullValue);

	  private:
		
		/**
		 * @brief Find the specified key in the specified bucket.
		 * @param bucket The bucket to look for the Key in.
		 * @param key The key to look for.
		 * @return A pointer with the Node that has the key, or NIL.
		 */
		inline DataNode<Cell>* find(const DataNode<Cell>* root, OID key) {
			DataNode<Cell>* node = root->next;
			while (node != root) {
				if (node->data.key == key) { return node; }
				node = node->next;
			}
			return NIL;			
		}
		inline const DataNode<Cell>* find_const(const DataNode<Cell>* root, OID key) const {
			DataNode<Cell>* node = root->next;
			while (node != root) {
				if (node->data.key == key) { return node; }
				node = node->next;
			}
			return NIL;		  
		}		
		
		/**
		 * @brief Get the size of a specified bucket.
		 * @param root The root of the bucket to get the size of.
		 * @return The size of the bucket.
		 */
		Size getBucketSize(const DataNode<Cell>& root) const {
			Size size = 0;			
			DataNode<Cell>* node = root.next;
			while (node != &root) { size++; node = node->next; }
			return size;
		}		
		
		/**
		 * @brief Resize the map to the specified capacity.
		 * @param capacity The capacity the Map should have.
		 * @param loadFactor The loadFactor for the map.
		 */
		void resizeMapToCapacity(Size capacity, F32 loadFactor);

		DataNode<Cell>*		   m_pBuckets;
		Size				m_capacity;	
		Size				m_numBuckets; 		
		DataNodePool<Cell>*   m_pNodePool;
		T              m_nullValue;		
		Size				m_numObjects;
		F32				m_loadFactor;
											
	};

	template <class T>
	Map<T>::~Map() {
		if (m_pBuckets) {
			delete[] m_pBuckets;
			m_pBuckets = NIL;
		}
		if (m_pNodePool) {
			delete m_pNodePool;
			m_pNodePool = NIL;
		}
		m_capacity = m_numBuckets = m_numObjects = 0;		
	}

	template <class T>
	void Map<T>::clear() {
		if (m_pBuckets) {
			for (Size i = 0; i < m_numBuckets; i++) {
				DataNode<Cell>* node = m_pBuckets[i].next;
				DataNode<Cell>* next = NIL;
				while (node != &(m_pBuckets[i])) {
					next = node->next;
					node->data.set(0, m_nullValue);					
					m_pNodePool->free(node);
					node = next;					
				}
			}
			m_numObjects = 0;
		}
	}

	template <class T>
	void Map<T>::eraseAll() {
		if (m_pBuckets) {
			for (Size i = 0; i < m_numBuckets; i++) {
				DataNode<Cell>* node = m_pBuckets[i].next;
				DataNode<Cell>* next = NIL;
				while (node != &(m_pBuckets[i])) {
					next = node->next;
					delete (node->data.value);
					node->data.set(0, m_nullValue);					
					m_pNodePool->free(node);
					node = next;					
				}
			}
			m_numObjects = 0;
		}
	}
	
	template <class T>
	inline void Map<T>::insert(OID key, const T& value) {
#if defined (DEBUG)
		if (m_pBuckets[key % m_numBuckets].next != &(m_pBuckets[key % m_numBuckets])) {
			DMSG("Collision inserting key: "
				  << key
				  << ", into bucket: "
				  << (key % m_numBuckets)
				  << ", already "
				  << getBucketSize(m_pBuckets[key % m_numBuckets])
				  << " items in bucket!");
		}
#endif
		if (m_numObjects >= m_capacity) {
			DMSG("Resizing map automatically. [capacity: "
				  << m_capacity << ", numObjects: "
				  << m_numObjects << "]");
			resizeMapToCapacity(m_capacity*2, m_loadFactor);		
		}

		m_pNodePool->alloc(&(m_pBuckets[key % m_numBuckets]), Cell(key, value));
		m_numObjects++;
	}

	template <class T>
	T Map<T>::take(const T& value) {
		DataNode<Cell>* node = NIL;		
		for (Size i = 0; i < m_numBuckets; i++) {
			node = m_pBuckets[i].next;
			while (node != &(m_pBuckets[i])) {
				if (node->data.value == value) {
					T val = node->data.value;
					node->data.set(0, m_nullValue);	
					m_pNodePool->free(node);
					m_numObjects--;
					return val;
				}
				node = node->next;
			}
		}
		return m_nullValue;
	}

	template <class T>
   void Map<T>::remove(const T& value) {
		DataNode<Cell>* node = NIL;		
		for (Size i = 0; i < m_numBuckets; i++) {
			node = m_pBuckets[i].next;
			while (node != &(m_pBuckets[i])) {
				if (node->data.value == value) {
					node->data.set(0, m_nullValue);	
					m_pNodePool->free(node);
					m_numObjects--;
					return;					
				}
				node = node->next;
			}
		}
	}

	template <class T>
   void Map<T>::erase(const T& value) {
		DataNode<Cell>* node = NIL;		
		for (Size i = 0; i < m_numBuckets; i++) {
			node = m_pBuckets[i].next;
			while (node != &(m_pBuckets[i])) {
				if (node->data.value == value) {
					delete (node->data.value);					
					node->data.set(0, m_nullValue);	
					m_pNodePool->free(node);
					m_numObjects--;
					return;					
				}
				node = node->next;
			}
		}
	}

	template <class T>
   Boolean Map<T>::contains(const T& value) const {
		DataNode<Cell>* node = NIL;		
		for (Size i = 0; i < m_numBuckets; i++) {
			node = m_pBuckets[i].next;
			while (node != &(m_pBuckets[i])) {
				if (node->data.value == value) {			 
					return true;					
				}
				node = node->next;
			}
		}
		return false;		
	}	

	template <class T>
	void Map<T>::initMapWithCapacityAndLoadFactor(Size capacity, F32 loadFactor,
																 const T& nullValue) {
		if (!m_pBuckets || m_pNodePool) {			
			m_capacity = capacity;
			m_loadFactor = loadFactor;
			m_numObjects = 0;			
			m_numBuckets = ceil((F32)capacity / loadFactor);
			m_pBuckets = new DataNode<Cell>[m_numBuckets];			
			m_pNodePool = new DataNodePool<Cell>(m_capacity);
			m_nullValue = nullValue;			
		}
		else {
			DERR("Cannot call initMapWithCapacityAndLoadFactor on initialized Map!");
		}
	}

	template <class T>
	void Map<T>::resizeMapToCapacity(Size capacity, F32 loadFactor) {
		// If the capacity is lower, we don't do anything.
		if (capacity > m_capacity && ceil((F32)capacity / loadFactor) > m_numBuckets) {
			// Store the old list of buckets.			
			DataNode<Cell>* oldBuckets = m_pBuckets;
			Size oldNumBuckets = m_numBuckets;

			// Create the new list of buckets.
			m_numBuckets = ceil((F32)capacity / loadFactor);
			m_pBuckets = new DataNode<Cell>[m_numBuckets];
			m_capacity = capacity;
			m_loadFactor = loadFactor;
			m_numObjects = 0;

			// Store old PtrNodeStore.
			DataNodePool<Cell>* oldPool = m_pNodePool;
			
			// Create new node store
			m_pNodePool = new DataNodePool<Cell>(m_capacity);
			// rehash
			DataNode<Cell>* node;
			for (Size i = 0; i < oldNumBuckets; i++) {
				node = oldBuckets[i].next;
				while (node != &(oldBuckets[i])) {
					insert(node->data.key, node->data.value);
					node = node->next;					
				}
			}
			
			// Delete the old buckets and node store.
			delete[] oldBuckets;
			delete oldPool;	
		} 
		else {
			return;
		}
	}

} //namespace

#endif // CAT_CORE_UTIL_MAP_H
