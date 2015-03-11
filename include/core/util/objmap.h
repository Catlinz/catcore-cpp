#ifndef CAT_CORE_UTIL_OBJMAP_H
#define CAT_CORE_UTIL_OBJMAP_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file objmap.h
 * @brief Defines a hashmap based on using crc32 to hash strings to 32 bit integers.
 *
 * @author Catlin Zilinski
 * @date Mar 21, 2014
 */

#include <cmath>

#include "core/util/objlist.h"
#include "core/util/callable.h"

namespace Cat {
	/**
	 * @class ObjMap objmap.h "core/util/objmap.h"
	 * @brief A hashmap using strings as keys hashed with a crc32 algorithm.
	 *
	 * This hashmap is to be used exclusivly with pointers to objects 
	 * which have the oID() and name() methods implemented.  
	 * The name() is a string uniquely identifying 
	 * the object and getOID() returns the crc32 hash of the name.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Oct 21, 2013
	 */
	template <class T>
	class ObjMap {
	  public:
		/**
		 * @brief Creates an empty map with a default initial capacity of 32.
		 */
		ObjMap() {
			createMapWithCapacity(32, 0.8);
		}		

		/**
		 * @brief Creates an empty map with the specified capacity.
		 * @param capacity The initial capacity of the hashmap.
		 */
		ObjMap(Size capacity) {
			createMapWithCapacity(capacity, 0.8);
		}		

		/**
		 * @brief Creates an empty map with the specified capacity and load factor.
		 * @param capacity The initial capacity of the hashmap.
		 * @param loadFactor The initial load factor for the hashmap.
		 */
		ObjMap(Size capacity, F32 loadFactor) {
			createMapWithCapacity(capacity, loadFactor);
		}		

		/**
		 * @brief The destructor, empties the map of all the nodes.
		 */
		~ObjMap();

		/**
		 * @brief Get the specified bucket in the map.
		 * @param idx The bucket to get.
		 * @return A pointer to the bucket.
		 */
		inline ObjList<T>* bucket(Size idx) { return &(m_pBuckets[idx]); }		
			
		/**
		 * @brief Empties the map but doesn't delete the objects stored within.
		 */
		void clear();

		/**
		 * @brief Erases all the objects stored in the map (deletes them).
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
		 * @return A pointer to the element or NIL if it is not there.
		 */
		inline T* at(OID key) {
			return m_pBuckets[key % m_numBuckets].get(key);
		}

		/**
		 * @brief Access an element of the map via its String key.
		 * @param key The name of the element (string).
		 * @return A pointer to the element or NIL if it doesn't exist.
		 */
		inline T* at(const Char* key) { return at(crc32(key)); }		

		/**
		 * @brief inserts the object in the map.
		 * @param obj The object to insert into the map.
		 * @return true if the object was inserted, false if it already existed.
		 */
		inline void insert(T* obj);

		/**
		 * @brief Removes and returns the object from the map.
		 * @param key The OID of the object to take from the map.
		 * @return The object taken from the map or NIL if it wasn't in the map.
		 */
		inline T* take(OID key) {
			T* val = m_pBuckets[key % m_numBuckets].take(key);
			if (val) { m_numObjects--; }
			return val;
		}

		/**
		 * @brief Removes and returns the object from the map.
		 * @param key The name of the object to take from the map.
		 * @return The object taken from the map or NIL if it wasn't in the map.
		 */
		inline T* take(const Char* name) { return take(crc32(name)); }
		
		/**
		 * @brief Removes and returns the object from the map.
		 * @param obj The object to take from the map.
		 * @return The object taken from the map or NIL if it wasn't in the map.
		 */
		inline T* take(T* obj) {
			if (obj) {
				return take(obj->oID());
			} else {
				return NIL;
			}			
		}

		/**
		 * @brief Removes the object from the map.
		 * @param key The OID of the object to remove from the map.
		 */
		inline void remove(OID key) {
			m_numObjects -= m_pBuckets[key % m_numBuckets].remove(key);
		}

		/**
		 * @brief Removes the object from the map.
		 * @param key The name of the object to remove from the map.
		 */
		inline void remove(const Char* name) { remove(crc32(key)); }		

		/**
		 * @brief Removes the object from the map.
		 * @param obj The object to remove from the map.
		 */
		inline void remove(T* obj) {
			if (obj) {
				remove(obj->oID());
			}
		}		

		/**
		 * @brief Removes AND DELETES the object from the map.
		 * @param key The OID of the object to delete from the map.
		 */
		inline void erase(OID key) {
			m_numObjects -= m_pBuckets[key % m_numBuckets].erase(key);
		}

		/**
		 * @brief Removes AND DELETES the object from the map.
		 * @param key The name of the object to delete from the map.
		 */
		inline void erase(const Char* name) { erase(crc32(name)); }		

		/**
		 * @brief Removes AND DELETES the object from the map.
		 * @param obj The object to delete from the map.
		 */
		inline void erase(T* obj) {
			if (obj) {
				erase(obj->oID());
			}
		}		

		/**
		 * @brief Tests to see if an object is in the map.
		 * @param key The OID key value of the object.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(OID key) const {
			return m_pBuckets[key % m_numBuckets].contains(key);
		}
		
		/**
		 * @brief Tests to see if an object is the map.
		 * @param name The string key value of the object.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(const Char* name) const {
			return contains(crc32(name));
		}		

		/**
		 * @brief Tests to see if the given object is in the map.
		 * @param obj The object to look for.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(T* obj) const {
			if (obj) {
				return contains(obj->oID());
			}
			else {
				return false;
			}
		}		

		/**
		 * @brief Executes the specified Callable object on each element in the map.
		 * @param func The Callable object to execute on each element in the map.
		 */
		void each(Callable* func);

		/**
		 * @brief Reserves the specified capacity in the Map.  
		 * If capacity < current capacity, nothing happens.
		 * @param capacity The new capacity to have in the map.
		 */
		inline void reserve(Size capacity) {
			resizeMapToCapacity(capacity, m_loadFactor);
		}

		class Iterator { 
		  public:
			Iterator(ObjMap<T>* map) : m_pMap(map), m_bucketIdx(0) {
				setNextBucketItr();				
			}

		   Boolean hasNext() {
				if (m_itr.hasNext()) {
					return true;
				}
				for (Size i = m_bucketIdx; i < m_pMap->numBuckets(); i++) {
					if (!m_pMap->bucket(i)->isEmpty()) {
						return true;
					}
				}
				return false;				
			}
			
			inline Boolean isValid() {
				return m_itr.isValid();				
			}

			inline void next() { 
				m_itr.next();
				if (!m_itr.isValid()) {
					m_bucketIdx++;					
					setNextBucketItr();					
				}
			}

			inline T* val() { 
				return m_itr.val();			  
			}

		  private:
			void setNextBucketItr() {
				for (Size i= m_bucketIdx; i < m_pMap->numBuckets(); i++) {
					if (!m_pMap->bucket(i)->isEmpty()) {
						m_itr = m_pMap->bucket(i)->getFirst();
						m_bucketIdx = i;
						return;						
					}
				}
				m_bucketIdx = m_pMap->numBuckets();
			}			

			ObjMap<T>* m_pMap;
			typename ObjList<T>::Iterator m_itr;
			Size m_bucketIdx;
		};

		/**
		 * @brief Get an Iterator into the Map.
		 * @return An iterator into the map.
		 */
		inline Iterator iterator() {
			return Iterator(this);
		}	  

	  private:
		/**
		 * @brief Create the initial map with the specified capacity.
		 * @param capacity The initial capacity.
		 * @param loadFactor The initial load factor to use.
		 */
		void createMapWithCapacity(Size capacity, F32 loadFactor);

		/**
		 * @brief Resize the map to the specified capacity.
		 * @param capacity The capacity the Map should have.
		 * @param loadFactor The loadFactor for the map.
		 */
		void resizeMapToCapacity(Size capacity, F32 loadFactor);

		Size				m_capacity;
		F32				m_loadFactor;	
		Size				m_numBuckets; 
		Node<T>*		   m_pBuckets;
		NodePool<T>*   m_pNodePool;		
		Size				m_numObjects;  
											
	};

	template <class T>
	ObjMap<T>::~ObjMap() {
		clear();
		if (m_pBuckets) {
			delete[] m_pBuckets;
			m_pBuckets = NIL;
			m_numBuckets = 0;
		}	
	}

	template <class T>
	void ObjMap<T>::clear() {
		if (m_pBuckets) {
			for (Size i = 0; i < m_numBuckets; i++) {
				m_pBuckets[i].clear();
			}
			m_numObjects = 0;
		}
	}

	template <class T>
	void ObjMap<T>::eraseAll(MemoryAllocator* alloc) {
		if (m_pBuckets) {
			for (Size i = 0; i < m_numBuckets; i++) {
				m_pBuckets[i].eraseAll();
			}
			m_numObjects = 0;
		}
		
	}
	
	template <class T>
	inline void ObjMap<T>::insert(T* obj) {
#if defined (DEBUG)
		if (!m_pBuckets[obj->getOID() % m_numBuckets].isEmpty()) {
			DMSG("Collision inserting key: "
				  << obj->getOID()
				  << ", into bucket: "
				  << (obj->getOID() % m_numBuckets)
				  << ", already "
				  << m_pBuckets[obj->getOID() % m_numBuckets].getLength()
				  << " items in bucket!");
		}
#endif
		if (m_numObjects >= m_capacity) {
			DMSG("Resizing map automatically. [capacity: "
				  << m_capacity << ", numObjects: "
				  << m_numObjects << "]");
			resizeMapToCapacity(m_capacity*2, m_loadFactor);		
		}

		m_pBuckets[obj->getOID() % m_numBuckets].append(obj);
		m_numObjects++;
	}

	template <class T> void ObjMap<T>::each(Callable* func) {
		if (isEmpty()) {
			return;
		}
		typename ObjList<T>::Iterator itr;
		for (Size i = 0; i < m_numBuckets; i++) {
			if (!m_pBuckets[i].isEmpty()) {
				itr = m_pBuckets[i].getFirst();
				while(itr.isValid()) {
					func->call(itr.val());
					itr.next();
				}
			}
		}
	}

	template <class T>
	void ObjMap<T>::createMapWithCapacity(Size capacity, F32 loadFactor) {
		m_capacity = capacity;
		m_loadFactor = loadFactor;
		m_numObjects = 0;

		m_numBuckets = ceil((F32)capacity / loadFactor);
		m_pBuckets = new ObjList<T>[m_numBuckets];		
	}

	template <class T>
	void ObjMap<T>::resizeMapToCapacity(Size capacity, F32 loadFactor) {
		// If the capacity is lower, we don't do anything.
		if (capacity > m_capacity && ceil((F32)capacity / loadFactor) > m_numBuckets) {
			// Store the old list of buckets.
			ObjList<T>* oldBuckets = m_pBuckets;
			Size oldNumBuckets = m_numBuckets;

			// Create the new list of buckets.
			m_numBuckets = ceil((F32)capacity / loadFactor);
			m_pBuckets = new ObjList<T>[m_numBuckets];
			m_capacity = capacity;
			m_loadFactor = loadFactor;
			m_numObjects = 0;

			// Go through the old list and rehash all the elements.
			Size idx = 0;
			for (idx = 0; idx < oldNumBuckets; idx++) {
				if (!oldBuckets[idx].isEmpty()) {					
					typename ObjList<T>::Iterator itr = oldBuckets[idx].getFirst();	
					while (itr.isValid()) {
						insert(itr.val());	
						itr.next();
					}
				}
			}
			// Delete the old buckets
			delete[] oldBuckets;
		} 
		else {
			return;
		}
	}

} //namespace

#endif // CAT_CORE_UTIL_OBJMAP_H
