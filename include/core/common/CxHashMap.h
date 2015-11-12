#ifndef CX_CORE_COMMON_CXHASHMAP_H
#define CX_CORE_COMMON_CXHASHMAP_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxHashMap.h
 * @brief Contains a dictionary using a hash table.
 *
 * @author Catlin Zilinski
 * @date Sept 16, 2015
 */

#include "core/Cx.h"
#include "core/common/CxHash.h"
#include "core/common/CxStr.h"
#include "core/math/CxMath.h"

namespace cat {

	/** @brief Node class for the HashMap */
	template <typename K, typename V>
	class CxHMNode_ {
	  public:
		CxHMNode_ *next;
		K key;  V value;
		CxU32 hash;
		
		CX_FORCE_INLINE CxHMNode_() : next(0), hash(0), m_isSet(0) {}
		CxHMNode_(CxHMNode_<K, V> *in_src);
		CxHMNode_(const K &in_key, const V &in_value, CxU32 in_hash);
		CxHMNode_(const K &in_key, V &&in_value, CxU32 in_hash);
		CxHMNode_(CxHMNode_<K,V> &inout_src, CxBool in_flag);
		~CxHMNode_();
		void copy(CxHMNode_<K,V> *in_n);
		CX_FORCE_INLINE CxBool isSet() const { return m_isSet; }
		void move(CxHMNode_<K,V> *in_src);
		void set(const K &in_key, const V &in_value, CxU32 in_hash);
		void set(const K &in_key, V &&in_value, CxU32 in_hash);
		void set(CxHMNode_<K,V> *in_src);  /* Use move semantics, in_src will be deleted after */

	  private:
		CxBool m_isSet;
	};

	template <typename K, typename V>
	CX_FORCE_INLINE CxHMNode_<K,V>::CxHMNode_(CxHMNode_<K,V> *in_src)
		: next(0), key(in_src->key), value(in_src->value), hash(in_src->hash),
		  m_isSet(in_src->m_isSet) {
		next = (in_src->next == 0) ? 0 : new CxHMNode_<K,V>(in_src->next);
	}
	
	template <typename K, typename V>
	CX_FORCE_INLINE CxHMNode_<K,V>::CxHMNode_(const K &in_key, const V &in_value, CxU32 in_hash)
		: next(0), key(in_key), value(in_value), hash(in_hash), m_isSet(true) {}

	template <typename K, typename V>
	CX_FORCE_INLINE CxHMNode_<K,V>::CxHMNode_(const K &in_key, V &&in_value, CxU32 in_hash)
		: next(0), key(in_key), value(in_value), hash(in_hash), m_isSet(true) {}

	template <typename K, typename V>
	CX_FORCE_INLINE CxHMNode_<K,V>::CxHMNode_(CxHMNode_<K,V> &inout_src, CxBool in_flag)
		: next(0), key(inout_src.key), value(inout_src.value), hash(inout_src.hash),
		  m_isSet(inout_src.m_isSet) {}

	template <typename K, typename V>
	CX_FORCE_INLINE CxHMNode_<K,V>::~CxHMNode_() {
		if (next != 0) { delete next;  next = 0; }
		m_isSet = false;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE void CxHMNode_<K,V>::copy(CxHMNode_<K,V> *in_n) {
		key = in_n->key;
		value = in_n->value;
		hash = in_n->hash;
		m_isSet = in_n->m_isSet;
		next = (in_n->next == 0) ? 0 : new CxHMNode_<K,V>(in_n->next);
	}

	template <typename K, typename V>
	CX_FORCE_INLINE void CxHMNode_<K,V>::move(CxHMNode_<K,V> *in_src) {
		next = in_src->next;
		key = in_src->key;
		value = in_src->value;
		hash = in_src->hash;
		m_isSet = in_src->m_isSet;

		in_src->next = 0;
		in_src->m_isSet = false;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE void CxHMNode_<K,V>::set(const K &in_key, const V &in_value, CxU32 in_hash) {
		key = in_key;
		value = in_value;
		hash = in_hash;
		m_isSet = true;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE void CxHMNode_<K,V>::set(const K &in_key, V &&in_value, CxU32 in_hash) {
		key = in_key;
		value = in_value;
		hash = in_hash;
		m_isSet = true;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE void CxHMNode_<K,V>::set(CxHMNode_<K,V> *in_src) {
		next = 0;
		key = in_src->key;
		value = in_src->value;
		hash = in_src->hash;
		m_isSet = in_src->m_isSet;

	}

	/* ############## HM NODE SPECIIALIZATION FOR STRING KEYS ######################### */

	template <typename V>
	class CxHMNode_<CxChar *, V> {
	  public:
		CxHMNode_ *next;
		CxChar *key;  V value;
		CxU32 hash;
		
		CX_FORCE_INLINE CxHMNode_() : next(0), key(0), hash(0) {}
		CxHMNode_(CxHMNode_<CxChar *, V> *in_src);
		CxHMNode_(CxChar *in_key, const V &in_value, CxU32 in_hash);
		CxHMNode_(CxChar *in_key, V &&in_value, CxU32 in_hash);
		CxHMNode_(CxHMNode_<CxChar *,V> &inout_src, CxBool in_flag);
		~CxHMNode_();
		void copy(CxHMNode_<CxChar *,V> *in_n);
		CX_FORCE_INLINE CxBool isSet() const { return key != 0; }
		void move(CxHMNode_<CxChar *,V> *in_src);
		void set(CxChar *in_key, const V &in_value, CxU32 in_hash);
		void set(CxChar *in_key, V &&in_value, CxU32 in_hash);
		void set(CxHMNode_<CxChar *,V> *in_src);
	};

	template <typename V>
	CX_FORCE_INLINE CxHMNode_<CxChar *,V>::CxHMNode_(CxHMNode_<CxChar *, V> *in_src)
		: next(0), key(0), value(in_src->value), hash(in_src->hash){
		key = str::copy(in_src->key);
		next = (in_src->next == 0) ? 0 : new CxHMNode_<CxChar *,V>(in_src->next);
	}

	template <typename V>
	CX_FORCE_INLINE CxHMNode_<CxChar *,V>::CxHMNode_(CxChar *in_key, const V &in_value, CxU32 in_hash)
		: next(0), key(in_key), value(in_value), hash(in_hash) {}

	template <typename V>
	CX_FORCE_INLINE CxHMNode_<CxChar *,V>::CxHMNode_(CxChar *in_key, V &&in_value, CxU32 in_hash)
		: next(0), key(in_key), value(in_value), hash(in_hash) {}

	template <typename V>
	CX_FORCE_INLINE CxHMNode_<CxChar *,V>::CxHMNode_(CxHMNode_<CxChar *,V> &inout_src, CxBool in_flag)
		: next(0), key(inout_src.key), value(inout_src.value), hash(inout_src.hash) {
		inout_src.key = 0;
	}

	template <typename V> CX_FORCE_INLINE CxHMNode_<CxChar *,V>::~CxHMNode_() {
		if (next != 0) { delete next;  next = 0; }  str::free(key);
	}

	template <typename V>
	CX_FORCE_INLINE void CxHMNode_<CxChar *,V>::copy(CxHMNode_<CxChar *,V> *in_n) {
		key = str::copy(in_n->key);
		value = in_n->value;
		hash = in_n->hash;
		next = (in_n->next == 0) ? 0 : new CxHMNode_<CxChar *,V>(in_n->next);
	}

	template <typename V>
	CX_FORCE_INLINE void CxHMNode_<CxChar *,V>::move(CxHMNode_<CxChar *,V> *in_src) {
		next = in_src->next;
		key = in_src->key;  value = in_src->value;
		hash = in_src->hash;

		in_src->next = 0;  in_src->key = 0;
	}

	template <typename V>
	CX_FORCE_INLINE void CxHMNode_<CxChar *,V>::set(CxChar *in_key, const V &in_value, CxU32 in_hash) {
		key = in_key;
		value = in_value;
		hash = in_hash;
	}
	
	template <typename V>
	CX_FORCE_INLINE void CxHMNode_<CxChar *,V>::set(CxChar *in_key, V &&in_value, CxU32 in_hash) {
		key = in_key;
		value = in_value;
		hash = in_hash;
	}


	template <typename V>
	CX_FORCE_INLINE void CxHMNode_<CxChar *,V>::set(CxHMNode_<CxChar *,V> *in_src) {
		next = 0;
		key = in_src->key;  value = in_src->value;
		hash = in_src->hash;

		in_src->key = 0;
	}

	/** @brief Internal class for managing the HashMap data. */
	template<typename K, typename V>
	class CxHMData_ {
	  public:

		class Itr {
		  public:
			CX_FORCE_INLINE Itr() : mp_node(0), mp_map(0), m_mapIdx(0), m_capacity(0) {}
			CX_FORCE_INLINE Itr(CxHMNode_<K,V> *in_node, CxHMNode_<K,V> *in_map,
									  CxI32 in_idx, CxI32 in_capacity)
				: mp_node(in_node), mp_map(in_map), m_mapIdx(in_idx), m_capacity(in_capacity) {}

			CX_FORCE_INLINE Itr(const Itr &in_src)
				: mp_node(in_src.mp_node), mp_map(in_src.mp_map), m_mapIdx(in_src.m_mapIdx),
				  m_capacity(in_src.m_capacity){}

			/** @return True if the two iterators are equal */
			CX_FORCE_INLINE CxBool operator==(const Itr &in_itr) const { return mp_node == in_itr.mp_node; }

			/** @return True if the two iterators are NOT equal */
			CX_FORCE_INLINE CxBool operator!=(const Itr &in_itr) const { return mp_node != in_itr.mp_node; }

			/** @return A const pointer to the value for the map entry */
			CX_FORCE_INLINE V * operator->() { return &(mp_node->value); }

			/** @return A reference to the value for the map entry */
			CX_FORCE_INLINE V & operator*() { return mp_node->value; }

			/** @return The key for the map entry */
			CX_FORCE_INLINE const K & key() const { return mp_node->key; }
			/** @return The value for the map entry */
			CX_FORCE_INLINE V & value() { return mp_node->value; }

			/** @brief Get an iterator i positions forward (or back if negative). */
			Itr operator+(CxI32 in_i) const;
			/** @brief Move to next item and returns itself (prefix)*/
			Itr & operator++();
			/** @brief Move to next item and return previous iterator (postfix)*/
			Itr operator++(CxI32);
			/** @brief Move iterator i positions and return the iterator */
			Itr & operator+=(CxI32 in_i);
			
			/** @brief Get an iterator i positions back (or forward if negative). */
			Itr operator-(CxI32 in_i) const;
			/** @brief Move to next item and returns itself (prefix) */
			Itr & operator--();
			/** @brief Move to next item and return previous iterator (postfix) */
			Itr operator--(CxI32);
			/** @brief Move iterator i positions and return the iterator */
			Itr & operator-=(CxI32 in_i);

			friend class CxHMData_<K,V>;
		  private:
			CxHMNode_<K,V> *mp_node;
			CxHMNode_<K,V> *mp_map;
			CxI32 m_mapIdx;
			CxI32 m_capacity;
		};
		
		CxHMData_() : mp_map(0), m_capacity(0), m_size(0), m_maxSize(0), m_maxLoad(0.8f) {}
		~CxHMData_();

		/** @brief Return true if have same key,value pairs */
		CxBool operator==(const CxHMData_ &in_map) const;

		/** @brief Return true if have different key,value pairs */
		CxBool operator!=(const CxHMData_ &in_map) const;

		/** @return An iterator to the 'first' item in the map */
		Itr begin();

		/** @return The current capacity of the hash map. */
		CX_FORCE_INLINE CxI32 capacity() const { return m_capacity; }

		/** @return Remove all the items from the map */
		void clear();

		/** @return True if the hash is in the map */
		CxBool contains(CxU32 in_hash) const;

		/** @return True if the hash-value pair is in the map */
		CxBool contains(CxU32 in_hash, const V &in_value) const;

		/** @brief Copy from another map */
		void copy(const CxHMData_<K,V> &in_data);

		/** @return The number of occurences of the given hashed key */
		CxI32 count(CxU32 in_hash) const;

		/** @return An iterator pointing to item (not real) beyond last item */
		CX_FORCE_INLINE const Itr & end() const { return m_endItr; }

		/** @return An iterator to the first occurance of the hashed key */
		Itr find(CxU32 in_hash);

		/** @return A constant iterator to the first occurance of the hashed key */
		Itr find(CxU32 in_hash) const;

		/** @brief Free the memory used by the map and reset */
		void free();

		/** @return The value at the specified hash index, or a default value */
		const V & get(CxU32 in_hash, const V &in_default) const;

		/** @return The first node that has the specified hashed key (or null) */
		const CxHMNode_<K,V> * getNodeByHash(CxU32 in_hash) const;

		/** @return The first node that has the specified value (or null) */
		const CxHMNode_<K,V> * getNodeByValue(const V &in_value) const;

		/** @brief Initialise the map data */
		void initialise(CxI32 in_maxSize, CxF32 in_loadFactor);

		/** @brief Insert a new value into the map and return iterator to it. */
		Itr insert(const K &in_key, const V &in_value, CxU32 in_hash);
		Itr insert(const K &in_key, V &&in_value, CxU32 in_hash);
		
		/** @brief Move the data from another map */
		void move(CxHMData_<K,V> &&in_data);

		/**
		 * @brief Remove all the items that match the specified key.
		 * @param in_hash The hashed key to remove from the map.
		 * @return The number of items removed (usually will be 1 or 0).
		 */
		CxI32 remove(CxU32 in_hash);

		/** @brief Remove the key-value pair specified by the iterator. */
		Itr remove(Itr &m_item);

		/** @brief Resize the map to hold at least as many items */
		void resize(CxI32 in_maxSize);

		/** @return The number of items in the hash map */
		CX_FORCE_INLINE CxI32 size() const { return m_size; }

		/** @brief Swap the contents of the two map datas */
		void swap(CxHMData_<K,V> &inout_map);

		/** @brief Remove and return a value (or default if key not found */
		V take(CxU32 in_hash, const V &in_default);

	  private:
		CxHMNode_<K,V> *mp_map;
		Itr m_endItr;
		
		CxI32 m_capacity;
		CxI32 m_size;
		CxI32 m_maxSize;
		CxF32 m_maxLoad;

		void rehash(CxHMNode_<K,V> *inout_n, CxHMNode_<K,V> *inout_map, CxI32 in_capacity);
		static CxI32 capacityForMaxItems(CxI32 in_maxSize, CxF32 in_loadFactor);
	};

	template <typename K, typename V>
	CxHMData_<K,V>::~CxHMData_() { free(); }

	template <typename K, typename V>
	CxBool CxHMData_<K,V>::operator==(const CxHMData_<K,V> &in_map) const {
		if (size() == in_map.size()) {
			const CxI32 cap = m_capacity;
			for (CxI32 i = 0; i < cap; ++i) {
				CxHMNode_<K,V> *n = mp_map + i;
				if (n->isSet()) {
					do {
						if (!in_map.contains(n->hash, n->value)) { return false; }
						n = n->next;
					} while (n != 0);
				}
			}
			return true;
		}
		else { return false; }
	}

	template <typename K, typename V>
	CX_FORCE_INLINE CxBool CxHMData_<K,V>::operator!=(const CxHMData_<K,V> &in_map) const {
		return !(*this == in_map);
	}

	template <typename K, typename V>
	CX_FORCE_INLINE typename CxHMData_<K,V>::Itr CxHMData_<K,V>::begin() {
		const CxI32 cap = m_capacity;
		for (CxI32 i = 0; i < cap; ++i) {
			if (mp_map[i].isSet()) { return Itr(mp_map + i, mp_map, i, m_capacity); }
		}
		return end();
	}

	template <typename K, typename V>
	CX_FORCE_INLINE CxI32 CxHMData_<K,V>::capacityForMaxItems(CxI32 in_maxSize,
																				 CxF32 in_loadFactor) {
		return CxNextPrime((CxI32)((CxF32)in_maxSize / in_loadFactor));
	}

	template <typename K, typename V>
	void CxHMData_<K,V>::clear() {
		const CxI32 cap = m_capacity;
		CxHMNode_<K,V> default_node;
		for (CxI32 i = 0; i < cap; ++i) {
			CxHMNode_<K,V> *n = mp_map + i;
			if (n->isSet()) {
				if (n->next != 0) { delete n->next; }
				*n = default_node;
			}
		}
		m_size = 0;
	}

	template <typename K, typename V>
	CxBool CxHMData_<K,V>::contains(CxU32 in_hash) const {
		if (m_capacity > 0) {
			const CxU32 idx = in_hash % m_capacity;
			CxHMNode_<K,V> *n = mp_map + idx;
			if (n->isSet()) {
				if (n->hash == in_hash) { return true; }
				else {
					n = n->next;
					while (n != 0) {
						if (n->hash == in_hash) { return true; }
						else { n = n->next; }
					} return false;
				}
			}
			else { return false; }
		}
		else { return false; }
	}
	
	template <typename K, typename V>
	CxBool CxHMData_<K,V>::contains(CxU32 in_hash, const V &in_value) const {
		if (m_capacity > 0) {
			const CxU32 idx = in_hash % m_capacity;
			CxHMNode_<K,V> *n = mp_map + idx;
			if (n->isSet()) {
				if (n->hash == in_hash && n->value == in_value) { return true; }
				else {
					n = n->next;
					while (n != 0) {
						if (n->hash == in_hash && n->value == in_value) { return true; }
						else { n = n->next; }
					} return false;
				}
			}
			else { return false; }
		}
		else { return false; }
	}

	template <typename K, typename V>
	void CxHMData_<K,V>::copy(const CxHMData_<K,V> &in_src) {
		if (mp_map != 0) { delete[] mp_map; mp_map = 0; }

		const CxI32 cap = in_src.m_capacity;
		if (cap > 0) {
			mp_map = new CxHMNode_<K,V>[cap];
			for (CxI32 i = 0; i < cap; ++i) {
				CxHMNode_<K,V> *n = in_src.mp_map + i;
				if (n->isSet()) { mp_map[i].copy(n); }
			}
		}
		m_capacity = cap;
		m_size = in_src.m_size;
		m_maxSize = in_src.m_maxSize;
		m_maxLoad = in_src.m_maxLoad;
	}

	template <typename K, typename V>
	CxI32 CxHMData_<K,V>::count(CxU32 in_hash) const {
		CxI32 count = 0;
		if (m_capacity > 0) {
			const CxU32 idx = in_hash % m_capacity;
			CxHMNode_<K,V> *n = mp_map + idx;
			if (n->isSet()) {
				do {
					if (n->hash == in_hash) { ++count; }
					n = n->next;
				} while (n != 0);
			}
		}
		return count;
	}

	template <typename K, typename V>
	typename CxHMData_<K,V>::Itr CxHMData_<K,V>::find(CxU32 in_hash) {
		if (m_capacity > 0) {
			const CxU32 idx = in_hash % m_capacity;
			CxHMNode_<K,V> *n = mp_map + idx;

			if (n->isSet()) {
				if (n->hash == in_hash) { return Itr(n, mp_map, idx, m_capacity); }
				else {
					n = n->next;
					while (n != 0) {
						if (n->hash == in_hash) { return Itr(n, mp_map, idx, m_capacity); }
						n = n->next;
					}
					return end(); /* No matching entries */
				}
			}
			else { return end(); } /* No matching entries */
		}
		else { return end(); }
	}

	template <typename K, typename V>
	typename CxHMData_<K,V>::Itr CxHMData_<K,V>::find(CxU32 in_hash) const {
		if (m_capacity > 0) {
			const CxU32 idx = in_hash % m_capacity;
			CxHMNode_<K,V> *n = mp_map + idx;

			if (n->isSet()) {
				if (n->hash == in_hash) { return Itr(n, mp_map, idx. m_capacity); }
				else {
					n = n->next;
					while (n != 0) {
						if (n->hash == in_hash) { return Itr(n, mp_map, idx, m_capacity); }
						n = n->next;
					}
					return end(); /* No matching entries */
				}
			}
			else { return end(); } /* No matching entries */
		}
		else { return end(); }
	}
	

	template <typename K, typename V>
	void CxHMData_<K,V>::free() {
		if (mp_map != 0) { delete[] mp_map;  mp_map = 0; }
		m_capacity = m_size = m_maxSize = 0;
	}

	
	template <typename K, typename V>
	const V & CxHMData_<K,V>::get(CxU32 in_hash, const V &in_default) const {
		if (m_capacity > 0) {
			const CxU32 idx = in_hash % m_capacity;
			CxHMNode_<K,V> *n = mp_map + idx;
			if (n->isSet()) {
				if (n->hash == in_hash) { return n->value; }
				else {
					n = n->next;
					while (n != 0) {
						if (n->hash == in_hash) { return n->value; }
						else { n = n->next; }
					} return in_default;
				}
			}
			else { return in_default; }
		}
		else { return in_default; }
	}

	template <typename K, typename V>
	const CxHMNode_<K,V> * CxHMData_<K,V>::getNodeByHash(CxU32 in_hash) const {
	   if (m_capacity > 0) {
			const CxU32 idx = in_hash % m_capacity;
			CxHMNode_<K,V> *n = mp_map + idx;
			if (n->isSet()) {
				if (n->hash == in_hash) { return n; }
				else {
					n = n->next;
					while (n != 0) {
						if (n->hash == in_hash) { return n; } else { n = n->next; }
					} return 0;
				}
			}
			else { return 0; }
		}
		else { return 0; }
	}

	template <typename K, typename V>
	const CxHMNode_<K,V> * CxHMData_<K,V>::getNodeByValue(const V &in_value) const {
		const CxI32 cap = m_capacity;
		for (CxI32 i = 0; i < cap; ++i) {
			CxHMNode_<K,V> *n = mp_map + i;
			if (n->isSet()) {
				do {
					if (n->value == in_value) { return n; }
					else { n = n->next; }
				} while (n != 0);
			}
		}
		return 0; /* No node with that value found */
	}
	
	template <typename K, typename V>
	void CxHMData_<K,V>::initialise(CxI32 in_maxSize, CxF32 in_loadFactor) {
		const CxI32 cap = capacityForMaxItems(in_maxSize, in_loadFactor);
		m_capacity = cap;
		m_size = 0;
		m_maxSize = (CxI32)(cap * in_loadFactor);
		m_maxLoad = in_loadFactor;

		if (cap > 0) { mp_map = new CxHMNode_<K,V>[cap]; }
	}

	template <typename K, typename V>
	typename CxHMData_<K,V>::Itr CxHMData_<K,V>::insert(const K &in_key, const V &in_value, CxU32 in_hash) {
		if (m_size >= m_maxSize) { resize((m_maxSize + 1) * 2); }
		const CxI32 idx = in_hash % m_capacity;
		CxHMNode_<K,V> *n = mp_map + idx;
		if (!n->isSet()) { n->set(in_key, in_value, in_hash); }
		else {
			while (n->next != 0) { n = n->next; }
			n->next = new CxHMNode_<K,V>(in_key, in_value, in_hash);
			n = n->next;
		}
		++m_size;
		return Itr(n, mp_map, idx, m_capacity);
	}

	template <typename K, typename V>
	typename CxHMData_<K,V>::Itr CxHMData_<K,V>::insert(const K &in_key, V &&in_value, CxU32 in_hash) {
		if (m_size >= m_maxSize) { resize((m_maxSize + 1) * 2); }

		const CxI32 idx = in_hash % m_capacity;
		CxHMNode_<K,V> *n = mp_map + idx;
		if (!n->isSet()) { n->set(in_key, in_value, in_hash); }
		else {
			while (n->next != 0) { n = n->next; }
			n->next = new CxHMNode_<K,V>(in_key, in_value, in_hash);
			n = n->next;
		}
		++m_size;
		return Itr(n, mp_map, idx, m_capacity);
	}

	template <typename K, typename V>
	void CxHMData_<K,V>::move(CxHMData_<K,V> &&in_data) {
		if (mp_map != 0) { delete[] mp_map; mp_map = 0; }
		mp_map = in_data.mp_map;
		m_capacity = in_data.m_capacity;
		m_size = in_data.m_size;
		m_maxSize = in_data.m_maxSize;
		m_maxLoad = in_data.m_maxLoad;
		in_data.mp_map = 0;
		in_data.m_capacity = in_data.m_size = 0;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE void CxHMData_<K,V>::rehash(CxHMNode_<K,V> *inout_n,
															 CxHMNode_<K,V> *inout_map,
															 CxI32 in_capacity) {
		const CxU32 idx = inout_n->hash % in_capacity;
		CxHMNode_<K,V> *n = inout_map + idx;
		if (!n->isSet()) { /* No value in the bucket yet */
		   n->set(inout_n);
		}
		else { /* At least one value in the bucket */
			while (n->next != 0) { n = n->next; }
			n->next = new CxHMNode_<K,V>(*inout_n, true);
		}
	}

	template <typename K, typename V>
	CxI32 CxHMData_<K,V>::remove(CxU32 in_hash) {
		if (m_capacity > 0) {
			const CxU32 idx = in_hash % m_capacity;
			CxHMNode_<K,V> *n = mp_map + idx;
			/* First, check the case of a single possible match */
			if (n->isSet() && n->next == 0) {
				if (n->hash == in_hash) {
					*n = CxHMNode_<K,V>();  --m_size;
					return 1;
				}
				else { return 0; }
			}
			/* Then check for multiple possible matches */
			else if (n->isSet()) {
				/* We ignore the first value for now, and check it at the end */
				CxI32 count = 0;
				CxHMNode_<K,V> *first = n, *prev = n;
				n = n->next;
				do {
					if (n->hash == in_hash) {
						prev->next = n->next;
						n->next = 0;  delete n;
						n = prev->next;
						++count;
					}
					else { prev = n;  n = n->next; }
				} while (n != 0);

				/* Check the first node */
				if (first->hash == in_hash) {
					++count;
					if (first->next != 0) {
						n = first->next;  first->move(n);
						delete n;
					}
					else { *first = CxHMNode_<K,V>(); }
				}
				m_size -= count;
				return count;
			}
			else { return 0; }  /* No matching nodes */
		}
		else { return 0; }
	}

	template <typename K, typename V>
	typename CxHMData_<K,V>::Itr CxHMData_<K,V>::remove(typename CxHMData_<K,V>::Itr &in_itr) {
		/* First, see if the item is an item in the actual array (static) */
		Itr next = in_itr + 1;
		CxHMNode_<K,V> *n = mp_map + in_itr.m_mapIdx;
		if (n == in_itr.mp_node) { /* First item in bucket */
			m_size--;
			if (n->next == 0) { *n = CxHMNode_<K,V>(); }
			else {
				CxHMNode_<K,V> *n_next = n->next;
				n->move(n_next);  delete n_next;
				return Itr(n, mp_map, in_itr.m_mapIdx, m_capacity);
			}
		}
		else {
			CxHMNode_<K,V> *target = in_itr.mp_node;
			while (n != 0 && n->next != target) { n = n->next; }

			if (n->next == target) {
				n->next = target->next;
				target->next = 0;  delete target;
				m_size--;
				if (n->next != 0) { return Itr(n->next, mp_map, in_itr.m_mapIdx, m_capacity); }
			}
			else { return end(); } /* Trying to remove node not in map */
		}

		/* If get here, have to search for next item */
		CxI32 idx = in_itr.m_mapIdx + 1;
		for (CxI32 idx = in_itr.m_mapIdx + 1; idx < m_capacity; ++idx) {
			if (mp_map[idx].isSet()) { return Itr(mp_map + idx, mp_map, idx, m_capacity); }
		}

		/* No next item */
		return end();
	}
	
	template <typename K, typename V>
	void CxHMData_<K,V>::resize(CxI32 in_maxSize) {
		if (in_maxSize > m_maxSize) {
			const CxI32 cap = capacityForMaxItems(in_maxSize, m_maxLoad);
			CxHMNode_<K,V> *map = new CxHMNode_<K,V>[cap];

			if (mp_map != 0) {
				/* Need to rehash all the items into the new map */
				const CxI32 p_cap = m_capacity;
				for (CxI32 i = 0; i < p_cap; ++i) {
					CxHMNode_<K,V> *n = mp_map + i;
					/* If have a value in the bucket, rehash them all */
					if (n->isSet()) {
						do { rehash(n, map, cap);  n = n->next; } while (n != 0);
					}
				}

				/* Delete the old map */
				delete[] mp_map;
			}
			mp_map = map;
			m_capacity = cap;
			m_maxSize = (CxI32)(cap * m_maxLoad);
		}
	}
	
	template <typename K, typename V>
	void CxHMData_<K,V>::swap(CxHMData_<K,V> &inout_map) {
		CxHMNode_<K,V> *map = mp_map;
		const CxI32 cap = m_capacity;
		const CxI32 size = m_size;
		const CxI32 maxSize = m_maxSize;
		const CxF32 maxLoad = m_maxLoad;

		mp_map = inout_map.mp_map;
		m_capacity = inout_map.m_capacity;
		m_size = inout_map.m_size;
		m_maxSize = inout_map.m_maxSize;
		m_maxLoad = inout_map.m_maxLoad;

		inout_map.mp_map = map;
		inout_map.m_capacity = cap;
		inout_map.m_size = size;
		inout_map.m_maxSize = maxSize;
		inout_map.m_maxLoad = maxLoad;
	}

	template <typename K, typename V>
	V CxHMData_<K,V>::take(CxU32 in_hash, const V &in_default) {
		if (m_capacity > 0) {
			const CxU32 idx = in_hash % m_capacity;
			CxHMNode_<K,V> *n = mp_map + idx;

			if (n->isSet()) {
				/* Check first node first */
				if (n->hash == in_hash) {
					V value = n->value;
					if (n->next == 0) { *n = CxHMNode_<K,V>(); }
					else {
						CxHMNode_<K,V> *n_next = n->next;
						n->move(n_next);  delete n_next;
					}
					m_size--;
					return value;
				}
				/* Check the rest of the nodes if the first fails */
				else {
					CxHMNode_<K,V> *prev = n;
					n = n->next;
					while (n != 0) {
						if (n->hash == in_hash) {
							V value = n->value;
							prev->next = n->next;  n->next = 0;  delete n;
							m_size--;
							return value;
						}
						else { prev = n;  n = n->next; }
					}
				}
				return in_default;  /* No nodes found */
			}
			else { return in_default; }			
		}
		else { return in_default; }
	}
	

	template <typename K, typename V>
	CX_FORCE_INLINE typename CxHMData_<K,V>::Itr CxHMData_<K,V>::Itr::operator+(CxI32 in_i) const {
		Itr i(*this);  i += in_i;  return i;
	}

	template <typename K, typename V>
	typename CxHMData_<K,V>::Itr & CxHMData_<K,V>::Itr::operator++() {
		if (mp_node->next == 0) {
			CxI32 idx = m_mapIdx + 1;
			for (; idx < m_capacity; ++idx) {
				if (mp_map[idx].isSet()) {
					mp_node = mp_map + idx;  m_mapIdx = idx;  return *this;
				}
			}
			/* Run out of items */
			mp_node = 0;
		}
		else { mp_node = mp_node->next; }
		return *this;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE typename CxHMData_<K,V>::Itr CxHMData_<K,V>::Itr::operator++(CxI32) {
		Itr i(*this);  ++(*this);  return i;
	}

	template <typename K, typename V>
	typename CxHMData_<K,V>::Itr & CxHMData_<K,V>::Itr::operator+=(CxI32 in_i) {
		if (in_i >= 0) {
			CxI32 idx = m_mapIdx;
			CxHMNode_<K,V> *n = mp_node;
			while (in_i > 0) {
				if (n->next == 0) {
					for (idx = idx + 1; idx < m_capacity; ++idx) {
						if (mp_map[idx].isSet()) { n = mp_map + idx; break;}
					}
					if (idx == m_capacity) { n = 0;  break; } /* Hit end of map */
				}
				else { n = n->next; }
				--in_i;
			}
			mp_node = n;
			m_mapIdx = idx;
			return *this;
		}
		else { return (*this) -= (-in_i); }
	}

	template <typename K, typename V>
	CX_FORCE_INLINE typename CxHMData_<K,V>::Itr CxHMData_<K,V>::Itr::operator-(CxI32 in_i) const {
		Itr i(*this);  i -= in_i;  return i;
	}

	template <typename K, typename V>
	typename CxHMData_<K,V>::Itr & CxHMData_<K,V>::Itr::operator--() {
		CxHMNode_<K,V> *n = mp_node;
		CxHMNode_<K,V> *root = mp_map + m_mapIdx;
		if (n == root) { /* Have to go back up the bucket list */
			CxI32 idx = m_mapIdx - 1;
			for (; idx >= 0; --idx) {
				if (mp_map[idx].isSet()) {
					root = mp_map + idx;
					while (root->next != 0) { root = root->next; }
					mp_node = root;  m_mapIdx = idx;  break;
				}
			}
		}
		else { /* Search forward from the root of bucket list */
			while (root->next != n) { root = root->next; }
			mp_node = root;
		}
		return *this;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE typename CxHMData_<K,V>::Itr CxHMData_<K,V>::Itr::operator--(CxI32) {
		Itr i(*this);  --(*this);  return i;
	}

	template <typename K, typename V>
	typename CxHMData_<K,V>::Itr & CxHMData_<K,V>::Itr::operator-=(CxI32 in_i) {
		if (in_i >= 0) {
			for (CxI32 i = 0; i < in_i; ++i) { (*this)--; }
			return *this;
		}
		else { return (*this) += (-in_i); }
	}

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

		typedef typename CxHMData_<K,V>::Itr Itr;
		
		/** @brief Create an empty zero-sized HashMap. */
		CX_FORCE_INLINE CxHashMap() {}

		/**
		 * @brief Create a new hashmap.
		 * @param in_capacity The initial max number of items to store.
		 * @param in_loadFactor The percent of the map that can be full before resizing.
		 */
		CxHashMap(CxI32 in_capacity, CxF32 in_loadFactor = 0.8f);

		/** @brief Copies all the data */
		CxHashMap(const CxHashMap<K, V> &in_src);

		/** @brief Moves all the data to the new hash map. */
		CxHashMap(CxHashMap<K, V> &&in_src);

		/** @brief Frees all the memory (Will not delete stored pointers) */
		~CxHashMap();

		/** @brief Copies all the data from the other hash map */
		CxHashMap<K, V> & operator=(const CxHashMap<K, V> &in_src);

		/** @brief Moves all the data from the other hash map */
		CxHashMap<K, V> & operator=(CxHashMap<K, V> &&in_src);

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
		V & operator[](const K &in_key);

		/**
		 * @brief Const array index operator to access element in map by key.
		 * @param in_key The key to lookup in the map.
		 * @return The value at the key location or a default value if none found.
		 */
		const V & operator[](const K &in_key) const;

		/** @return An iterator to the 'first' item in the map */
		CX_FORCE_INLINE Itr begin() { return m_map.begin(); }

		/** @return The current capacity of the hash map. */
		CX_FORCE_INLINE CxI32 capacity() const { return m_map.capacity(); }

		/** @brief Removes all items from the map (doesnt free memory). */
		CX_FORCE_INLINE void clear() { m_map.clear(); }

		/** @return True if the key is contained in the map */
		CxBool contains(const K &in_key) const;

		/** @return The number of items in the hash map */
		CX_FORCE_INLINE CxI32 count() const { return m_map.size(); }

		/** @return The number of occurances of the given key in the map */
		CxI32 count(const K &in_key) const;

		/** @return The default key value for the map */
		CX_FORCE_INLINE const K & defaultKey() const { return m_defaultKey; }

		/** @return The default value for the map */
		CX_FORCE_INLINE const V & defaultValue() const { return m_defaultValue; }

		/** @return An iterator pointing to item (not real) beyond last item */
		CX_FORCE_INLINE const Itr & end() const { return m_map.end(); }

		/** @return An iterator to the first occurance of the given key */
		Itr find(const K &in_key);

		/** @return An iterator to the first occurance of the given key */
		Itr find(const K &in_key) const;

		/** @brief Releases all the memory and resets map to 0 size */
		CX_FORCE_INLINE void free() { m_map.free(); }

		/** 
		 * @brief Insert a new key-value pair into the map.
		 * There are no checks for duplicates.
		 * @param in_key The key to use to locate the value.
		 * @param in_value The value to store at the specified key.
		 * @return An iterator to the inserted key-value pair.
		 */
		Itr insert(const K &in_key, const V &in_value);
		Itr insert(const K &in_key, V &&in_value);

		/** @return True if the hash map is empty */
		CX_FORCE_INLINE CxBool isEmpty() const { return m_map.size() == 0; }

		/** @return The key associated with the value, or the default key. */
		const K & key(const V &in_value) const;

		/** @return The key associated with the value, or a default key. */
		const K & key(const V &in_value, const K &in_default) const;

		/**
		 * @brief Remove all the items that match the specified key.
		 * @param in_key The key to remove from the map.
		 * @return The number of items removed (usually will be 1 or 0).
		 */
		CxI32 remove(const K &in_key);

		/**
		 * @brief Remove the key-value pair specified by the iterator.
		 * @param in_itr The iterator to the key-value pair to remove from the map.
		 * @return An iterator to the next item in the map (or the end if no more).
		 */
		CX_FORCE_INLINE Itr remove(Itr &in_itr) { return m_map.remove(in_itr); }

		/**
		 * @brief Increase the size of the map to hold a specified number of items.
		 * @param in_capacity The number of items that the map should be able to hold.
		 */
		void reserve(CxI32 in_capacity);

		/** @return The number of items in the hash map */
		CX_FORCE_INLINE CxI32 size() const { return m_map.size(); }

		/**
		 * @brief Swap the contents of the two maps (fast).
		 * @param inout_map The map to swap the contents with.
		 */
		void swap(CxHashMap<K,V> &inout_map);

		/**
		 * @brief Remove and return the value at the given key.
		 * @param in_key The key to take the value of.
		 * @return The value corresponding to the given key (or default value if no key).
		 */
		V take(const K &in_key);

		/**
		 * @brief Remove and return the value at the given key.
		 * @param in_key The key to take the value of.
		 * @param in_default Default value returned if key is not found.
		 * @return The value corresponding to the given key (or default value if no key).
		 */
		V take(const K &in_key, const V &in_default);

		/**
		 * @brief Get the value corresponding to the given key.
		 * @param in_key The key to lookup.
		 * @return The value corresponding to the given key, or a default value if no matching key is found.
		 */
		const V & value(const K &in_key) const;

		/**
		 * @brief Get the value corresponding to the given key, or return a default value.
		 * @param in_key The key to lookup.
		 * @return The value corresponding to the given key, or a specified default value if no matching key is found.
		 */
		const V & value(const K &in_key, const V &in_default) const;
		
		
	  private:
		CxHMData_<K,V> m_map;
		V m_defaultValue;
		K m_defaultKey;
	};

	template <typename K, typename V>
	CxHashMap<K,V>::CxHashMap(CxI32 in_capacity, CxF32 in_loadFactor) {
		m_map.initialise(in_capacity, in_loadFactor);
	}

	template <typename K, typename V>
	CxHashMap<K,V>::CxHashMap(const CxHashMap<K,V> &in_src) {
		m_map.copy(in_src.m_map);
	}

	template <typename K, typename V>
	CX_FORCE_INLINE CxHashMap<K,V>::CxHashMap(CxHashMap<K,V> &&in_src) {
		m_map.move(static_cast< CxHMData_<K,V> &&>(in_src.m_map));
	}

	template <typename K, typename V>
	CxHashMap<K,V>::~CxHashMap() {}

	template <typename K, typename V>
	CxHashMap<K,V> & CxHashMap<K,V>::operator=(const CxHashMap<K,V> &in_src) {
		m_map.copy(in_src.m_map); return *this;
	}

	template <typename K, typename V>
	CxHashMap<K,V> & CxHashMap<K,V>::operator=(CxHashMap<K,V> &&in_src) {
		m_map.move(static_cast< CxHMData_<K,V> && >(in_src.m_map));
		return *this;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE CxBool CxHashMap<K,V>::operator==(const CxHashMap<K,V> &in_map) const {
	   return m_map == in_map.m_map;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE CxBool CxHashMap<K,V>::operator!=(const CxHashMap<K,V> &in_map) const {
	   return m_map != in_map.m_map;
	}

	template <typename K, typename V>
	V & CxHashMap<K,V>::operator[](const K &in_key) {
		const CxU32 hash = CxHash(in_key);
	   CxHMNode_<K,V> *n = const_cast<CxHMNode_<K,V> *>(m_map.getNodeByHash(hash));
		
		if (n != 0) { return n->value; }
		else {
			return *(m_map.insert(in_key, m_defaultValue, hash));
		}
	}

	template <typename K, typename V>
	CX_FORCE_INLINE const V & CxHashMap<K,V>::operator[](const K &in_key) const {
	   return m_map.get(CxHash(in_key), m_defaultValue);
	}

	template <typename K, typename V>
	CX_FORCE_INLINE CxBool CxHashMap<K,V>::contains(const K &in_key) const {
		return m_map.contains(CxHash(in_key));
	}

	template <typename K, typename V>
	CX_FORCE_INLINE CxI32 CxHashMap<K,V>::count(const K &in_key) const {
		return m_map.count(CxHash(in_key));
	}

	template <typename K, typename V>
	CX_FORCE_INLINE typename CxHashMap<K,V>::Itr CxHashMap<K,V>::find(const K &in_key) {
		return m_map.find(CxHash(in_key));
	}

	template <typename K, typename V>
	CX_FORCE_INLINE typename CxHashMap<K,V>::Itr CxHashMap<K,V>::find(const K &in_key) const {
		return m_map.find(CxHash(in_key));
	}

	template <typename K, typename V>
	CX_FORCE_INLINE typename CxHashMap<K,V>::Itr CxHashMap<K,V>::insert(const K &in_key,
																				  const V &in_value) {
		return m_map.insert(in_key, in_value, CxHash(in_key));
	}

	template <typename K, typename V>
	CX_FORCE_INLINE typename CxHashMap<K,V>::Itr CxHashMap<K,V>::insert(const K &in_key,
																							  V &&in_value) {
		return m_map.insert(in_key, in_value, CxHash(in_key));
	}

	template <typename K, typename V>
	CX_FORCE_INLINE const K & CxHashMap<K,V>::key(const V &in_value) const {
		const CxHMNode_<K,V> *n = m_map.getNodeByValue(in_value);
		return (n != 0) ? n->key : m_defaultKey;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE const K & CxHashMap<K,V>::key(const V &in_value,
																 const K &in_default) const {
		const CxHMNode_<K,V> *n = m_map.getNodeByValue(in_value);
		return (n != 0) ? n->key : in_default;
	}

	template <typename K, typename V>
	CX_FORCE_INLINE CxI32 CxHashMap<K,V>::remove(const K &in_key) {
		return m_map.remove(CxHash(in_key));
	}

	template <typename K, typename V>
	CX_FORCE_INLINE void CxHashMap<K,V>::reserve(CxI32 in_capacity) {
		m_map.resize(in_capacity);
	}

	template <typename K, typename V>
	CX_FORCE_INLINE void CxHashMap<K,V>::swap(CxHashMap<K,V> &inout_map) {
		m_map.swap(inout_map.m_map);
	}

	template <typename K, typename V>
	CX_FORCE_INLINE V CxHashMap<K,V>::take(const K &in_key) {
		return m_map.take(CxHash(in_key), m_defaultValue);
	}

	template <typename K, typename V>
	CX_FORCE_INLINE V CxHashMap<K,V>::take(const K &in_key, const V &in_default) {
		return m_map.take(CxHash(in_key), in_default);
	}

	template <typename K, typename V>
	CX_FORCE_INLINE const V & CxHashMap<K,V>::value(const K &in_key) const {
	   return m_map.get(CxHash(in_key), m_defaultValue);
	}

	template <typename K, typename V>
	CX_FORCE_INLINE const V & CxHashMap<K,V>::value(const K &in_key,
																	const V &in_default) const {
		return m_map.get(CxHash(in_key), in_default);
	}


	/* ############ HASH MAP SPECIIALIZATION FOR STRING KEYS ################## */
	/**
	 * Partial specialization of the hash map to be able to better use string keys.
	 * The specialization makes it so that string constants can be used to index
	 * the hashmap, and that the keys are copied before set when inserting.  It also
	 * hash methods that allow indexing through the actual hashes of the string keys.
	 * @see CxHashMap<K,V> for class details.	 
	 */
	template<typename V>
	class CxHashMap<CxChar *, V> {
	  public:

		typedef typename CxHMData_<CxChar *,V>::Itr Itr;
		
		CX_FORCE_INLINE CxHashMap() {}
		CxHashMap(CxI32 in_capacity, CxF32 in_loadFactor = 0.8f);
		CxHashMap(const CxHashMap<CxChar *, V> &in_src);
		CxHashMap(CxHashMap<CxChar *, V> &&in_src);
		~CxHashMap();
		CxHashMap<CxChar *, V> & operator=(const CxHashMap<CxChar *, V> &in_src);
		CxHashMap<CxChar *, V> & operator=(CxHashMap<CxChar *, V> &&in_src);
		CxBool operator==(const CxHashMap<CxChar *, V> &in_map) const;
		CxBool operator!=(const CxHashMap<CxChar *, V> &in_map) const;
		V & operator[](const CxChar *in_key);
		const V & operator[](const CxChar *in_key) const;
		CX_FORCE_INLINE const V & operator[](CxU32 in_hash) const;
		CX_FORCE_INLINE Itr begin() { return m_map.begin(); }
		CX_FORCE_INLINE CxI32 capacity() const { return m_map.capacity(); }
		CX_FORCE_INLINE void clear() { m_map.clear(); }
		CxBool contains(const CxChar *in_key) const;
		CX_FORCE_INLINE CxI32 count() const { return m_map.size(); }
		CxI32 count(const CxChar *in_key) const;
		CX_FORCE_INLINE const CxChar * defaultKey() const { return 0; }
		CX_FORCE_INLINE const V & defaultValue() const { return m_defaultValue; }
		CX_FORCE_INLINE const Itr & end() const { return m_map.end(); }
		Itr find(const CxChar *in_key);
		Itr find(const CxChar *in_key) const;
		CX_FORCE_INLINE void free() { m_map.free(); }
		Itr insert(const CxChar *in_key, const V &in_value);
		Itr insert(const CxChar *in_key, V &&in_value);
		CX_FORCE_INLINE CxBool isEmpty() const { return m_map.size() == 0; }
		const CxChar * key(const V &in_value) const;
		const CxChar * key(const V &in_value, const CxChar *in_default) const;
		CxI32 remove(const CxChar *in_key);
		CX_FORCE_INLINE Itr remove(Itr &in_itr) { return m_map.remove(in_itr); }
		void reserve(CxI32 in_capacity);
		CX_FORCE_INLINE CxI32 size() const { return m_map.size(); }
		void swap(CxHashMap<CxChar *, V> &inout_map);
		V take(const CxChar *in_key);
		V take(const CxChar *in_key, const V &in_default);
		const V & value(const CxChar *in_key) const;
		const V & value(CxU32 in_hash) const;
		const V & value(const CxChar *in_key, const V &in_default) const;
		const V & value(CxU32 in_hash, const V &in_default) const;
		
		
	  private:
		CxHMData_<CxChar *, V> m_map;
		V m_defaultValue;
	};

	template <typename V>
	CxHashMap<CxChar *, V>::CxHashMap(CxI32 in_capacity, CxF32 in_loadFactor) {
		m_map.initialise(in_capacity, in_loadFactor);
	}

	template <typename V>
	CxHashMap<CxChar *, V>::CxHashMap(const CxHashMap<CxChar *, V> &in_src) {
		m_map.copy(in_src.m_map);
	}

	template <typename V>
	CX_FORCE_INLINE CxHashMap<CxChar *, V>::CxHashMap(CxHashMap<CxChar *, V> &&in_src) {
		m_map.move(static_cast< CxHMData_<CxChar *, V> &&>(in_src.m_map));
	}

	template <typename V> CxHashMap<CxChar *, V>::~CxHashMap() {}

	template <typename V>
	CxHashMap<CxChar *, V> & CxHashMap<CxChar *, V>::operator=(const CxHashMap<CxChar *, V> &in_src) {
		m_map.copy(in_src.m_map);  return *this;
	}

	template <typename V>
	CxHashMap<CxChar *, V> & CxHashMap<CxChar *, V>::operator=(CxHashMap<CxChar *, V> &&in_src) {
		m_map.move(static_cast< CxHMData_<CxChar *, V> && >(in_src.m_map));
		return *this;
	}

	template <typename V>
	CX_FORCE_INLINE CxBool CxHashMap<CxChar *, V>::operator==(const CxHashMap<CxChar *, V> &in_map) const {
	   return m_map == in_map.m_map;
	}

	template <typename V>
	CX_FORCE_INLINE CxBool CxHashMap<CxChar *, V>::operator!=(const CxHashMap<CxChar *, V> &in_map) const {
	   return m_map != in_map.m_map;
	}

	template <typename V> V & CxHashMap<CxChar *, V>::operator[](const CxChar *in_key) {
		const CxU32 hash = CxHash(in_key);
		CxHMNode_<CxChar *, V> *n = const_cast<CxHMNode_<CxChar *, V> *>(m_map.getNodeByHash(hash));
		
		if (n != 0) { return n->value; }
		else {
			return *(m_map.insert(str::copy(in_key), m_defaultValue, hash));
		}
	}

	template <typename V>
	CX_FORCE_INLINE const V & CxHashMap<CxChar *, V>::operator[](const CxChar *in_key) const {
	   return m_map.get(CxHash(in_key), m_defaultValue);
	}

	template <typename V>
	CX_FORCE_INLINE const V & CxHashMap<CxChar *, V>::operator[](CxU32 in_hash) const {
		return m_map.get(in_hash, m_defaultValue);
	}

	template <typename V>
	CX_FORCE_INLINE CxBool CxHashMap<CxChar *, V>::contains(const CxChar *in_key) const {
		return m_map.contains(CxHash(in_key));
	}

	template <typename V>
	CX_FORCE_INLINE CxI32 CxHashMap<CxChar *, V>::count(const CxChar *in_key) const {
		return m_map.count(CxHash(in_key));
	}

	template <typename V>
	CX_FORCE_INLINE typename CxHashMap<CxChar *, V>::Itr CxHashMap<CxChar *, V>::find(const CxChar *in_key) {
		return m_map.find(CxHash(in_key));
	}

	template <typename V>
	CX_FORCE_INLINE typename CxHashMap<CxChar *, V>::Itr CxHashMap<CxChar *, V>::find(const CxChar *in_key) const {
		return m_map.find(CxHash(in_key));
	}

	template <typename V>
	CX_FORCE_INLINE typename CxHashMap<CxChar *, V>::Itr CxHashMap<CxChar *, V>::insert(const CxChar *in_key,
																										const V &in_value) {
		return m_map.insert(str::copy(in_key), in_value, CxHash(in_key));
	}

	template <typename V>
	CX_FORCE_INLINE typename CxHashMap<CxChar *, V>::Itr CxHashMap<CxChar *, V>::insert(const CxChar *in_key, V &&in_value) {
		return m_map.insert(str::copy(in_key), in_value, CxHash(in_key));
	}

	template <typename V>
	CX_FORCE_INLINE const CxChar * CxHashMap<CxChar *, V>::key(const V &in_value) const {
		const CxHMNode_<CxChar *, V> *n = m_map.getNodeByValue(in_value);
		return (n != 0) ? n->key : 0;
	}

	template <typename V>
	CX_FORCE_INLINE const CxChar * CxHashMap<CxChar *, V>::key(const V &in_value,
																			const CxChar *in_default) const {
		const CxHMNode_<CxChar *, V> *n = m_map.getNodeByValue(in_value);
		return (n != 0) ? n->key : in_default;
	}

	template <typename V>
	CX_FORCE_INLINE CxI32 CxHashMap<CxChar *, V>::remove(const CxChar *in_key) {
		return m_map.remove(CxHash(in_key));
	}

	template <typename V>
	CX_FORCE_INLINE void CxHashMap<CxChar *, V>::reserve(CxI32 in_capacity) {
		m_map.resize(in_capacity);
	}

	template <typename V>
	CX_FORCE_INLINE void CxHashMap<CxChar *, V>::swap(CxHashMap<CxChar *, V> &inout_map) {
		m_map.swap(inout_map.m_map);
	}

	template <typename V>
	CX_FORCE_INLINE V CxHashMap<CxChar *, V>::take(const CxChar *in_key) {
		return m_map.take(CxHash(in_key), m_defaultValue);
	}

	template <typename V>
	CX_FORCE_INLINE V CxHashMap<CxChar *, V>::take(const CxChar *in_key, const V &in_default) {
		return m_map.take(CxHash(in_key), in_default);
	}

	template <typename V>
	CX_FORCE_INLINE const V & CxHashMap<CxChar *, V>::value(const CxChar *in_key) const {
	   return m_map.get(CxHash(in_key), m_defaultValue);
	}

	template <typename V>
	CX_FORCE_INLINE const V & CxHashMap<CxChar *, V>::value(CxU32 in_hash) const {
	   return m_map.get(in_hash, m_defaultValue);
	}

	template <typename V>
	CX_FORCE_INLINE const V & CxHashMap<CxChar *, V>::value(const CxChar *in_key,
																			  const V &in_default) const {
		return m_map.get(CxHash(in_key), in_default);
	}

	template <typename V>
	CX_FORCE_INLINE const V & CxHashMap<CxChar *, V>::value(CxU32 in_hash, const V &in_default) const {
		return m_map.get(in_hash, in_default);
	}

} // namespace cat

#endif // CX_CORE_COMMON_CXHASHMAP_H
