#ifndef CAT_CORE_UTIL_STATICMAP_H
#define CAT_CORE_UTIL_STATICMAP_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file staticmap.h
 * @brief Defines a hashmap based on using crc32 to hash strings to 32 bit integers.
 *
 * This hashmap uses string keys to hash the locations, and assumes that after 
 * the map is built, it is never changed.
 *
 * @author Catlin Zilinski
 * @date Feb 25, 2014
 */

#include <cmath>
#include <cstring>
#include "core/corelib.h"

namespace Cat {

	/**
	 * @class StaticMap staticmap.h "core/util/staticmap.h"
	 * @brief A hashmap using strings as keys hashed with a crc32 algorithm.
	 *
	 * This hashmap uses string keys to hash the locations, and assumes that after 
	 * the map is built, it is never changed.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Feb 25, 2014
	 */
	template <class T>
	class StaticMap {
	  public:

		class Cell {
		  public:
			OID key;
			T value;
			Cell() : key(0) {}			
			Cell(OID pkey, T pvalue) : key(pkey), value(pvalue) {}			
		};		
		/**
		 * @brief Creates an empty map with no array.
		 */
		StaticMap() :
			m_capacity(0), m_arraySize(0), m_loadFactor(0.0f), m_pEntries(NIL), m_numEntries(0) {}
		

		/**
		 * @brief Creates an empty map with the specified capacity.
		 * @param capacity The initial capacity of the hashmap.
		 */
		StaticMap(Size capacity, T nullValue) :
			m_capacity(0), m_arraySize(0), m_loadFactor(0.0f), m_pEntries(NIL), m_numEntries(0), m_nullValue(nullValue) {
			createMapWithCapacity(capacity, 0.5);
		}
		
		/**
		 * @brief Creates an empty map with the specified capacity and load factor.
		 * @param capacity The initial capacity of the hashmap.
		 * @param loadFactor The initial load factor for the hashmap.
		 */
		StaticMap(Size capacity, F32 loadFactor, T nullValue)  :
			m_capacity(0), m_arraySize(0), m_loadFactor(0.0f), m_pEntries(NIL), m_numEntries(0), m_nullValue(nullValue) {
			createMapWithCapacity(capacity, loadFactor);
		}

		
		/**
		 * @brief Copy constructor, creates copy of the map and all the data.
		 * @param src The Map to copy from.
		 */
		StaticMap(const StaticMap<T>& src);

		/**
		 * @brief overloaded assignment operator.
		 * Makes sure to copy all the data.
		 * @param src The Map to copy from.
		 * @return A reference to this map.
		 */
		StaticMap<T>& operator=(const StaticMap<T>& src);		

		/**
		 * @brief The destructor, empties the map of all the nodes.
		 */
		~StaticMap();

		/**
		 * @brief Get the actual Cell array data.
		 * @return The Cell Array data.
		 */
		inline Cell* arrayData() { return m_pEntries; }
		
		/**
		 * @brief Return the actual array length.
		 * @return The actual storage array length.
		 */
		inline Size arrayLength() const { return m_arraySize; }		

		/**
		 * @brief Access an element of the map via its String key.
		 * @param key The name of the element (string).
		 * @return The element or the null element.
		 */
		inline T at(const Char* key) const {
			return at(crc32(key));
		}		

		/**
		 * @brief Access an element of the map via its crc32 object id.
		 * @param key The name of the element (string).
		 * @return The element or the null element.
		 */
		inline T at(OID key) const;
		
		/**
		 * @brief Return the capacity of the map.
		 * The capacity refers to the number of occupied buckets we can have 
		 * before the map needs to be resized.
		 * @return The capacity of the map.
		 */
		inline Size capacity() const { return m_capacity; }

		/**
		 * @brief Clear the map of all entries.
		 * Removes all entries from the map.  Does not delete anything.
		 */
		void clear();		

		/**
		 * @brief Tests to see if an object is the map.
		 * @param key The string key value of the object.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(const Char* key) const {
			return contains(crc32(key));
		}		

		/**
		 * @brief Tests to see if an object is in the map.
		 * @param key The OID key value of the object.
		 * @return true if the object is in the map.
		 */
		inline Boolean contains(OID key) const;

		/**
		 * @brief Assumes the types are pointers and deletes them all.
		 */
		void eraseAll();

		/**
		 * @brief Initialize a static map to the specified capacity.
		 * @param capacity The capacity for the static map to have.
		 */
		void initWithCapacityAndLoadFactor(Size capacity, F32 loadFactor, T nullValue) {
			m_capacity = m_arraySize = 0;						
			if (m_pEntries) {
				delete[] m_pEntries;
				m_pEntries = NIL;
			}
			m_numEntries = 0;
			m_nullValue = nullValue;
			createMapWithCapacity(capacity, loadFactor);
		}

		/**
		 * @brief inserts the object in the map.
		 * @param key The Key of the key-value pair to insert into the map.
		 * @param value The value to insert into the map.
		 */
		void insert(OID key, T value);
		inline void insert(const Char* key, T value) {
			insert(crc32(key), value);
		}
		
		/**
		 * @brief Test if the map is empty or not
		 * @return true if the map is empty.
		 */
		inline Boolean isEmpty() const {
			return m_numEntries == 0;
		}

		/**
		 * @brief Returns the load factor for the map.
		 * @return The load factor for the map.
		 */
		inline F32 loadFactor() const {
			return m_loadFactor;
		}
		
		/**
		 * @brief Get the Null value for this map.
		 * @return The null value for this map.
		 */
		inline const T& nullValue() const { return m_nullValue; }

		/**
		 * @brief Return the number of elements in the map.
		 * @return The number of elements in the map.
		 */
		inline Size size() const {
			return m_numEntries;			
		}		

		/**
		 * @brief Return an array with all the values in it.
		 * @return An array of the values.
		 */
		T* toArray() const;

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

		/**
		 * @brief Fill the array with nullValues.
		 */
		void fillWithNull() {
			for (Size i = 0; i < m_arraySize; i++) {
				m_pEntries[i] = typename StaticMap<T>::Cell(0, m_nullValue);
			}
		}
		
			
		Size				m_capacity;    /**< The number of buckets in use before needing resizing */
		Size           m_arraySize;		
		F32				m_loadFactor;	/**< The percent of full buckets before resize */
		Cell*		      m_pEntries; 	
		Size				m_numEntries;	/**< The actual number of entries */
		T              m_nullValue;		

	};

	// ######## CONSTRUCTORS ################ //

	template <class T>
	StaticMap<T>::StaticMap(const StaticMap<T>& src) {
		m_capacity = src.m_capacity;
		m_arraySize = src.m_arraySize;
		m_loadFactor = src.m_loadFactor;
		m_pEntries = NIL;		
		m_numEntries = src.m_numEntries;
		m_nullValue = src.m_nullValue;
		if (src.m_pEntries) {
			m_pEntries = new Cell[m_arraySize];
			memcpy(m_pEntries, src.m_pEntries, sizeof(Cell)*m_arraySize);
		}
	}

	template <class T>
	StaticMap<T>& StaticMap<T>::operator=(const StaticMap<T>& src) {
		if (m_pEntries) {
			delete[] m_pEntries;
		}		
		m_capacity = src.m_capacity;
		m_arraySize = src.m_arraySize;
		m_loadFactor = src.m_loadFactor;
		m_pEntries = NIL;		
		m_numEntries = src.m_numEntries;
		m_nullValue = src.m_nullValue;
		if (src.m_pEntries) {
			m_pEntries = new Cell[m_arraySize];
			memcpy(m_pEntries, src.m_pEntries, sizeof(Cell)*m_arraySize);
		}
		return *this;		
	}
	
	template <class T>
	StaticMap<T>::~StaticMap() {
		if (m_pEntries) {			
			delete[] m_pEntries;
			m_pEntries = NIL;
			m_numEntries = 0;
			m_capacity = 0;
			m_arraySize = 0;			
		}		
	}

	template <class T>
	void StaticMap<T>::eraseAll() {
		if (m_pEntries) {
			for (Size i = 0; i < m_arraySize; i++) {
				if (m_pEntries[i].key != 0) {
					delete m_pEntries[i].value;
					m_pEntries[i] = typename StaticMap<T>::Cell(0, m_nullValue);					
				}
			}
			m_numEntries = 0;			
		}		
	}

	template <class T>
	void StaticMap<T>::clear() {
		if (m_pEntries) {
			for (Size i = 0; i < m_arraySize; i++) {
				if (m_pEntries[i].key != 0) {
					m_pEntries[i] = StaticMap<T>::Cell(0, m_nullValue);					
				}
			}
			m_numEntries = 0;			
		}
	}

	template <class T> inline T StaticMap<T>::at(OID key) const {
		Size idx = key % m_arraySize;
		if (m_pEntries[idx].key == key || m_pEntries[idx].key == 0) {
			return m_pEntries[idx].value;
		}
		else {
			while (m_pEntries[idx % m_arraySize].key != key && m_pEntries[idx % m_arraySize].key != 0) {
				idx++;
			}
			return m_pEntries[idx % m_arraySize].value;
		}		
	}
	
	template <class T>
	void StaticMap<T>::insert(OID key, T value) {
#if defined (DEBUG)
		if (m_pEntries[key % m_arraySize].key != 0) {
			DMSG("Collision inserting key: " << key << ", into StaticMap, already occupied by value with key " << m_pEntries[key % m_arraySize].key << ".");
		}
#endif
		if (m_numEntries >= m_capacity) {
			DMSG("Resizing map automatically. [capacity: " << m_capacity << ", numEntries: " << m_numEntries << "]");
			resizeMapToCapacity(m_capacity*2, m_loadFactor);		
		}

		Size idx = key % m_arraySize;		
	   while (m_pEntries[idx % m_arraySize].key != 0) {
			idx++;			
		}
		
		m_pEntries[idx % m_arraySize] = typename StaticMap<T>::Cell(key, value);	
		m_numEntries++;
	}


	template <class T> inline Boolean StaticMap<T>::contains(OID key) const {
		Size idx = key % m_arraySize;
		if (m_pEntries[idx].key == key || m_pEntries[idx].key == 0) {
			return m_pEntries[idx].key != 0;			
		}		
		else {
			while (m_pEntries[idx % m_arraySize].key != key && m_pEntries[idx % m_arraySize].key != 0) {
				idx++;
			}
			return m_pEntries[idx % m_arraySize].key != 0;
		}				
	}

	template <class T> T* StaticMap<T>::toArray() const {
		T* array = new T[m_numEntries];
		Size idx = 0;		
		for (U32 i = 0; i < m_arraySize; i++) {
			if (m_pEntries[i].key != 0) {
				array[idx++] = m_pEntries[i].value;
			}
		}
		return array;
	}	


	template <class T>
	void StaticMap<T>::createMapWithCapacity(Size capacity, F32 loadFactor) {
		m_capacity = capacity;
		m_loadFactor = loadFactor;	

		m_arraySize = ceil((F32)m_capacity / m_loadFactor);
		if (m_arraySize < 1) {
			DERR("Array size < 1.... not good.  very not good.");
			DERR("Capacity: " << capacity << ", loadFactor: " << loadFactor);
		}		
		m_pEntries = new Cell[m_arraySize];
		fillWithNull();		
	}

	template <class T>
	void StaticMap<T>::resizeMapToCapacity(Size capacity, F32 loadFactor) {
		// If the capacity is lower, we don't do anything.
		if (capacity > m_capacity && ceil((F32)capacity / loadFactor) > m_arraySize) {
			// Store the old list of buckets.
			Cell* oldEntries = m_pEntries;
			Size oldArraySize = m_arraySize;

			// Create the new list of buckets.
			m_arraySize = ceil((F32)capacity / loadFactor);
			m_pEntries = new Cell[m_arraySize];
			m_capacity = capacity;
			m_loadFactor = loadFactor;
			m_numEntries = 0;
			fillWithNull();			

			// Go through the old list and rehash all the elements.
			for (Size idx = 0; idx < oldArraySize; idx++) {
				if (oldEntries[idx].value != m_nullValue) {				
				   insert(oldEntries[idx].key, oldEntries[idx].value);					
				}
			}
			
			// Delete the old buckets
			delete[] oldEntries;
		} 
		else {
			return;
		}
	}
	
} //namespace

#endif // CAT_CORE_UTIL_STATICMAP_H
