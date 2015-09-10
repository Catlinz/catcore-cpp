#ifndef CX_CORE_COMMON_CXHASHMAP_H
#define CX_CORE_COMMON_CXHASHMAP_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxHashMap.h
 * @brief Contains a dictionary using a hash table.
 *
 * @author Catlin Zilinski
 * @date Sept 5, 2015
 */

#include "core/Cx.h"
#include "core/common/CxHash.h"

namespace cat {

	/**
	 * @class CxHashMap CxHashMap.h "core/common/CxHashMap.h"
	 * @brief A basic dictionary that uses a hash table.
	 *
	 * The hash map relies on the keys to have defined the 
	 * operator==() method and an overload of the global CxHash 
	 * method that accepts the key value as an argument and returns
	 * a CxU32 value as the hash.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 5, 2015
	 */
	template<typename K, typename V>
	class CxHashMap {
	  public:
		
		class Node {
		  public:
			Node *next;
			CxU32 key;  V value;
			CX_FORCE_INLINE Node() : next(0), key(0) {}
			CX_FORCE_INLINE Node(Node *in_n) : next(0), key(in_n->key), value(in_n->value) {}
			CX_FORCE_INLINE Node(CxU32 in_key, const V &in_value)
				: next(0), key(in_key), value(in_value) {}
		};

		
		/** @brief Create an empty zero-sized HashMap. */
		CX_FORCE_INLINE CxHashMap()
			: mp_map(0), m_capacity(0), m_size(0), m_maxSize(0), m_maxLoad(0) {}

		/**
		 * @brief Create a new hashmap.
		 * @param in_capacity The initial max number of items to store.
		 * @param in_loadFactor The percent of the map that can be full before resizing.
		 */
		CxHashMap(CxI32 in_capacity, CxF32 in_loadFactor = 0.8f);

		/** @brief Copies all the data */
		CxHashMap(const CxHashMap<K, V> &in_src);

		/** @brief Moves all the data to the new hash map. */
		CX_FORCE_INLINE CxHashMap(CxHashMap<K, V> &&in_src);

		/** @brief Frees all the memory (Will not delete stored pointers) */
		~CxHashMap();

		/** @brief Copies all the data from the other hash map */
		CxHashMap<K, V> operator=(const CxHashMap<K, V> &in_src);

		/** @brief Moves all the data from the other hash map */
		CxHashMap<K, V> operator=(CxHashMap<K, V> &&in_src);

		/** @return True if the hash maps store all the same key,value pairs. */
		CxBool operator==(const CxHashMap<K, V> &in_map) const;

		/** @return True if the hash maps store at least one different key,value pair. */
		CxBool operator!=(const CxHashMap<K, V> &in_map) const;

		/**
		 * @brief Array index operator to access an element in the map by a key.
		 * This method will insert a new default constructed element in the key's location
		 * if there is nothing there yet.  Use value(K) if you do not want this behaviour.
		 * @param in_key The key to lookup in the map.
		 * @return The value at the key location.
		 */
		T & operator[](const K &in_key);
		
		
	  private:
		Node **mp_map;
		V m_default;

		CxI32 m_capacity;
		CxI32 m_size;
		CxI32 m_maxSize;
		CxF32 m_maxLoad;
	};

	template <typename K, typename V>
	CxHashMap<K,V>::CxHashMap(CxI32 in_capacity, CxF32 in_loadFactor = 0.8f) {
		const CxI32 base_cap = (CxI32)((CxF32)in_capacity / in_loadFactor);
		const CxI32 cap = CxNextPrime(base_cap);
		m_capacity = cap;
		m_size = 0;
		m_maxSize = in_capacity;
		m_maxLoad = in_loadFactor;

		mp_map = (Node **)mem::allocZero(sizeof(Node *), cap);
	}

	template <typename K, typename V>
	CxHashMap<K,V>::CxHashMap(const CxHashMap<K,V> &in_src) {
		const CxI32 cap = in_src.m_capacity;
		if (cap > 0) {
			mp_map = (Node **)mem::allocZero(sizeof(Node *), cap);
			for (CxI32 i = 0; i < cap; ++i) {
				Node *n = in_src.mp_map[i];
				if (n != 0) {
					mp_map[i] = new Node(n);
					n = n->next;
					while (n != 0) {
						
				}
			}
		}
		m_capacity = cap;
		m_size = in_src.m_size;
		m_maxSize = in_src.m_maxSize;
		m_maxLoad = in_src.m_maxLoad;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE CxHashMap<K,V>::CxHashMap(CxHashMap<K,V> &&in_src)
		: mp_map(in_src.mp_map), m_capacity(in_src.m_capacity),
		  m_size(in_src.m_size), m_maxSize(in_src.m_maxSize),
		  m_maxLoad(in_src.m_maxLoad) { in_src.mp_map = 0; }

	template <typename K, typename V>
	CxHashMap<K,V>::~CxHashMap() {
		if (mp_map != 0) {
			
		}
		m_capacity = m_size = m_maxSize = 0;
		m_maxLoad = 0.0f;
	}
	
	template <V>
	class CxHashMap<CxChar *, V> {

	};
		

} // namespace cat

#endif // CX_CORE_COMMON_CXHASHMAP_H
