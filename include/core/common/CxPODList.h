#ifndef CX_CORE_COMMON_CXPODLIST_H
#define CX_CORE_COMMON_CXPODLIST_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxList.h
 * @brief The CxList class is an array-based list class simlar to a CxVector.
 * The CxList class is an array-based list class simular to a CxVector.
 * 
 * The CxPODList differs from the CxPODVector in one important way.
 * 1.) The CxList puts the 'first' item in the middle of the array storage, so that
 * both appending and prepending are ammoritized to O(1).
 *
 * @author Catlin Zilinski
 * @date May 28, 2015
 */

#include "core/common/CxInvasiveStrongPtr.h"
#include "core/common/CxMem.h"

namespace cat {

	/**
	 * @class CxPODList CxPODList.h "core/common/CxPODList.h"
	 * @brief See the file comments.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since May 28, 2015
	 */
	template<typename T>
	class CxPODList {
	  public:
		/** @brief Create an empty null list. */
		CX_FORCE_INLINE CxPODList();

		/**
		 * @brief Create a new list with the specified size.
		 * The values are initialise with the default values.
		 * @param in_size The initial size of the list.
		 */
		CX_FORCE_INLINE CxPODList(CxI32 in_size);

		/**
		 * @brief Create a new list with the specified size.
		 * The values are initialised with the specified fill value.
		 * @param in_size The initialise size of the list.
		 * @param in_value The value to fill the list with.
		 */
		CX_FORCE_INLINE CxPODList(CxI32 in_size, const T &in_value);

		/**
		 * @brief Create a new list from the specified array.
		 * @param in_array The c-array of elements.
		 * @param in_size The number of elements in the array.
		 * @param inopt_copy If kCxNoCopy, then array is not copied, just stored.
		 */
		CxPODList(T *in_array, CxI32 in_size, CxCopy inopt_copy = kCxCopy);

		/**
		 * @brief Copy constructor, copies each element.
		 * @param in_src The source list to create a copy of.
		 */
		CxPODList(const CxPODList<T> &in_src);

		/** @brief Deletes the contents of the List. */
		~CxPODList();

		/**
		 * @brief Overloaded Assignment operator, replaces contents with new contents.
		 * @param in_src The CxPODList to copy.
		 * @return A reference to this List.
		 */
		CxPODList<T>& operator=(const CxPODList<T> &in_src);

		/**
		 * @brief Operator to test to lists for equality.
		 * @param in_list The other list to test for equality to.
		 * @return True if the two lists are equal.
		 */
		CxBool operator==(const CxPODList<T> &in_list) const;

		/**
		 * @brief Operator to test two lists for non-equality.
		 * @param in_vec The other list to test against.
		 * @return True if the two lists are NOT equal.
		 */
		CxBool operator!=(const CxPODList<T> &in_list) const;
		
		/**
		 * @brief Get the list element at the specified index.
		 * @param in_idx The index of the element to access.
		 * @return A reference to the element.
		 */
		CX_FORCE_INLINE T & operator[](CxI32 in_idx) {
			D_CONDERR(in_idx >= m_size, "Accessing List element "
						 << in_idx << " outside range [0.." << m_size << "]!");
			return mp_list[in_idx];
		}

		/**
		 * @brief Get the constant list element at the specified index.
		 * @param in_idx The index of the element ot access.
		 * @return A constant reference to the element.
		 */
		CX_FORCE_INLINE const T & operator[](const CxI32 in_idx) const {
			D_CONDERR(in_idx >= m_size, "Accessing List element "
						 << in_idx << " outside range [0.." << m_size << "]!");
			return mp_list[in_idx];
		}

		/**
		 * @brief Concatenate two lists and return the resulting new list.
		 * @param in_vec The list to append to this one.
		 * @return A new list that is the result of in_list appended to this list.
		 */
		CxPODList<T> operator+(const CxPODList<T> &in_list) const;

		/**
		 * @brief Append another list onto this list.
		 * @see append()
		 * @see operator<<()
		 * @param in_vec The list to append to this one.
		 * @return A reference to this list.
		 */
		CX_FORCE_INLINE CxPODList<T> & operator+=(const CxPODList<T> &in_list) {
			append(in_list); return *this;
		}

		/**
		 * @brief Append a value onto this list.
		 * @see append()
		 * @see operator<<()
		 * @param in_value The value to append to the list.
		 * @return A reference to this list.
		 */
		CX_FORCE_INLINE CxPODList<T> & operator+=(const T &in_value) {
			append(in_value); return *this;
		}
		
		/**
		 * @brief Append another list onto this list.
		 * @see append()
		 * @see operator+=()
		 * @param in_vec The list to append to this one.
		 * @return A reference to this list.
		 */
		CX_FORCE_INLINE CxPODList<T> & operator<<(const CxPODList<T> &in_list) {
			append(in_list); return *this;
		}

		/**
		 * @brief Append a value onto this list.
		 * @see append()
		 * @see operator+=()
		 * @param in_value The value to append to the list.
		 * @return A reference to this list.
		 */
		CX_FORCE_INLINE CxPODList<T> & operator<<(const T &in_value) {
			append(in_value); return *this;
		}
		
		/**
		 * @brief Append an element onto the end of the list.
		 * @param in_elem The element to append to the list.
		 */
		CX_FORCE_INLINE void append(const T& in_elem);

		/**
		 * @brief Append all the elements from another list.
		 * @param in_src The other list to append the elements from.
		 */
		void append(const CxPODList<T>& in_src);

		/**
		 * @brief Get the value at the specified index.
		 * At always checks to make sure that the range is value, whereas
		 * operator[]() does not.
		 * @see operator[]()
		 * @see value()
		 * @param in_idx The index to get the value at (must be valid).
		 * @return The value at the specified index.
		 */
		CX_FORCE_INLINE const T & at(CxI32 in_idx) const;
		
		/** @return The capacity of the list. */
		CX_FORCE_INLINE CxI32 capacity() const { return m_capacity; }
		
		/**
		 * @brief Remove all elements from the list and releases the memory.
		 * This method will NOT result in pointers being deleted in a list of pointers.
		 * For this behaviour, you must call eraseAll().
		 */
		void clear();

		/**
		 * @brief Releases any unused memory the list has.
		 * This method simply resizes the underlying array so that 
		 * the capacity is equal to the size.
		 */
		void compact();
		
		/**
		 * @brief Check to see if the list contains a certain value.
		 * @param value The value to search for.
		 * @return True if the value is included in the list.
		 */
		CxBool contains(const T& in_value) const;

		/** @return The number of items in the list */
		CX_FORCE_INLINE CxI32 count() const { return m_size; }

		/**
		 * @brief Get the number of occurances of a specified value.
		 * @param in_value The value to count.
		 * @return The number of occurances of the value in the list.
		 */
		CxI32 count(const T &in_value) const;

		/**
		 * @brief Test for equality with the last element of the list.
		 * @param in_value The value to test against the last element.
		 * @return True if the list is not empty and the last element is equal to in_value.
		 */
		CX_FORCE_INLINE CxBool endsWith(const T &in_value) const {
			return (m_size > 0 && mp_list[m_size - 1] == in_value);
		}

		/**
		 * @brief Remove all elements from a list and delete them. 
		 * This method assumes that the list contains dynamically allocated 
		 * pointers, and as such, calls delete on all the elements.
		 */
		void eraseAll();

		/**
		 * @brief Removes the last element from the list and deletes it.
		 * This method assumes that the list is storing pointers to dynamically allocated 
		 * memory, which was allocated using the 'new' operator.
		 */
		void eraseLast();

		/**
		 * @brief Fill the list with the specified value.
		 * The method will fill the list with the specified value, and 
		 * will resize the list if in_size is > the current size.
		 * @param in_value The value to fill the list with.
		 * @param inopt_size The optional number of elements to fill.
		 */
		void fill(const T &in_value, CxI32 inopt_size = -1);

		/** @return A reference to the first element in the list */
		CX_FORCE_INLINE T & first() { return (*this)[0]; }

		/** @return A constant reference to the first element in the list */
		CX_FORCE_INLINE const T & first() const { return (*this)[0]; }

		/**
		 * @brief Get the index of the first occurance of a value, or -1 if not found.
		 * @param in_value The element to find the index of.
		 * @param in_from The optional index to start searching from.
		 * @return The index of the element or -1 if not found.
		 */
		CxI32 indexOf(const T &in_value, CxI32 in_from = 0) const;

		/**
		 * @brief Insert an item into the List.
		 * Inserting an element into the List is an O(n) operation, since 
		 * all the elements after the new element will have to be moved down.
		 * The index value to insert into must be within the range [0..length], 
		 * inclusive.
		 * @param in_idx The index to insert the element into.
		 * @param in_elem The element to insert.
		 */
		void insert(CxI32 in_idx, const T &in_elem);

		/**
		 * @brief Insert in_count copies of a value inot the List.
		 * @see insert(CxI32, const T&).
		 * @param in_idx The index to start inserting into.
		 * @param in_elem The element to insert.
		 * @param in_count The number of times to insert the element.
		 */
		void insert(CxI32 in_idx, const T &in_elem, CxI32 in_count);

		/** @return True if the list is empty. */
		CX_FORCE_INLINE CxBool isEmpty() const { return m_size == 0; }
		
		/** @return A reference to the last element in the list. */
		CX_FORCE_INLINE T & last() { return (*this)[(m_size - 1)]; }

		/** @return A constant reference to the last element in the vector. */
		CX_FORCE_INLINE const T& last() const { return (*this)[(m_size - 1)]; }

		/**
		 * @brief Get the index of the last occurance of a value, or -1 if not found.
		 * @param in_value The element to find the index of.
		 * @param in_from The optional index to start searching from.
		 * @return The index of the element or -1 if not found.
		 */
		CxI32 lastIndexOf(const T &in_value, CxI32 in_from = -1) const;

		/** @return The current length of the list. */
		CX_FORCE_INLINE CxI32 length() const { return m_size; }

		/**
		 * @brief Insert a value into the front of the list.
		 * @param in_value The value to insert into the front of the list.
		 */
		CX_FORCE_INLINE void prepend(const T &in_value);
		
		/**
		 * @brief Try and remove the specified element from the List.
		 * Will only remove the first occurance.
		 * @see removeAt(CxI32);
		 * @param in_value The element to try and remove from the List.
		 * @return True if the element was found and removed.
		 */
		CxBool remove(const T &in_value);

		/**
		 * @brief Remove all occurances of a value from the List.
		 * @see remove()
		 * @param in_value The value to remove all occurances of.
		 * @return The number of occurances found / removed.
		 */
		CxI32 removeAll(const T &in_value);

		/**
		 * @brief Remove an arbitrary element from the List.
		 * This method removes an arbitrary element from the list by 
		 * shifting all the elements after it, up by one, and then 
		 * shrinks the length by one.  This runs in O(n) time.
		 * @param in_idx The indx of the element to remove at.
		 * @return True if the element was removed.
		 */
		CxBool removeAt(CxI32 in_idx);

		/** @brief Remove the first element from the list. */
		CX_FORCE_INLINE void removeFirst();

		/** @brief Remove the last element from the list. */
		void removeLast();

		/**
		 * @brief Replace the first occurance of a specified value with a new value.
		 * @param in_old The value to replace.
		 * @param in_new The value to replace with.
		 * @return True if a value was found and replaced.
		 */
		CX_FORCE_INLINE CxBool replace(const T &in_old, const T &in_new);

		/**
		 * @brief Replace all occurances of a specified value with a new value.
		 * @param in_old The value to replace.
		 * @param in_new The value to replace with.
		 * @return The number of occurances replaced.
		 */
	   CxI32 replaceAll(const T &in_old, const T &in_new);

		/**
		 * @brief Replace the value at the specified index.
		 * @param in_idx The index to replace the value at.
		 * @param in_value The new value to replace with.
		 * @return True if the value was replaced.
		 */
		CX_FORCE_INLINE CxBool replaceAt(CxI32 in_idx, const T &in_value);

		/**
		 * @brief Reserve the specified capacity in the list.
		 * @param in_capacity The amount of elements to reserve for the list.
		 */
		CX_FORCE_INLINE void reserve(CxI32 in_capacity);

		/**
		 * @brief Resize the list to the specified size.
		 * If in_size is greater than the current size, then new elements 
		 * are added to the end of the list, if less than the current size, 
		 * then elements are removed from the end of the list.
		 * @param in_size The new size to give the list.
		 */
		void resize(CxI32 in_size);

		/**
		 * @brief Set the list element at the specified index.
		 * If the index value is greater than the current length, then 
		 * the length of the array is set to encompass the newly set element.
		 * @param in_idx The index of the element to set.
		 * @param in_value The value of the element.
		 */
		void set(CxI32 in_idx, const T &in_value);

		/**
		 * @brief Set all the values in the list to the specified value.
		 * This method sets all allocated values to the specified value, 
		 * i.e., from idx = 0, to idx = capacity - 1.
		 * @param in_value The value to set all the values to.
		 */
		void setAll(const T &in_value);

		/** @return The number of elements in the list. */
		CX_FORCE_INLINE CxI32 size() const { return m_size; }

		/**
		 * @brief Method to sort the list using the given comparator
		 * This method uses the built in qsort to sort the list.
		 * NOTE: The pointers in the in_compare method will actually be
		 * pointers to pointers, since the list is stored using pointers 
		 * to the objects, not the actual objects themselves.
		 * @param in_compare The method to use to compare two elements.
		 */
		CX_FORCE_INLINE void sort(CxI32 (*in_compare)(const void*, const void*)) {
			qsort(mp_vec, m_size, sizeof(T), in_compare);
		}		

		/**
		 * @brief Test to see if the list begins with the specified value.
		 * @param in_value The value to see if the list starts with.
		 * @return True if the list's first element is in_value.
		 */
		CX_FORCE_INLINE CxBool startsWith(const T &in_value) const {
			return (m_size > 0 && mp_list[0] == in_value);
		}

		/**
		 * @brief Swaps the contents of two lists.
		 * @param in_list The other list to swap with.
		 */
		CX_FORCE_INLINE void swap(CxPODList<T> &in_list);

		/**
		 * @brief Removes and returns the given value from the list.
		 * @param in_idx The index of the element to take.
		 * @return A copy of the removed element.
		 */
		T takeAt(CxI32 in_idx);

		/**
		 * @brief Removes and returns the first element in the list.
		 * @return A copy of the previously first element in the list.
		 */
		CX_FORCE_INLINE T takeFirst();
		
		/**
		 * @brief Removes and returns the last element in the list.
		 * @return A copy of the previously last element in the list.
		 */
		CX_FORCE_INLINE T takeLast();

		/**
		 * @brief Get a copy of the value at the specified index.
		 * At always checks to make sure that the range is value, whereas
		 * operator[] does not.
		 * @see operator[]()
		 * @see at()
		 * @param in_idx The index to get the value at (must be valid).
		 * @return A copy of the value at the specified index.
		 */
		CX_FORCE_INLINE T value(CxI32 in_idx) const;

		/**
		 * @brief Get a copy of the value at the specified index.
		 * @see value()
		 * @param in_idx The index to get the value at (must be valid).
		 * @param in_oobValue The value to return for an out-of-bounds index.
		 * @return A copy of the value at the specified index or in_oobValue.
		 */
		CX_FORCE_INLINE T value(CxI32 in_idx, const T &in_oobValue) const;

		CX_ISPTR_METHODS;

	  private:
		CX_FORCE_INLINE void initialise(CxI32 in_size);

		void priv_removeAt(CxI32 in_idx);

		CX_FORCE_INLINE void resizeToCapacity(CxI32 capacity);

		T *mp_raw;   /**< Pointer to the beginning of the storage. */
		T *mp_list;	 /**< Pointer to the beginning of the list */

		T m_invalidValue; /**< The value returned if outside bounds */
		CxI32		m_capacity;		/**< The current capacity of the list */
		CxI32		m_size;		/**< The number of elements in the list */
		
		CX_ISPTR_FIELDS;
	};

	template <typename T>
	CX_FORCE_INLINE CxPODList<T>::CxPODList()
		: mp_raw(0), mp_list(0), m_capacity(0), m_size(0) { initialise(0); }

	template <typename T>
	CX_FORCE_INLINE CxPODList<T>::CxPODList(CxI32 in_size)
		: mp_raw(0), mp_list(0), m_capacity(0), m_size(0) { initialise(in_size); }

	template <typename T>
	CX_FORCE_INLINE CxPODList<T>::CxPODList(CxI32 in_size, const T &in_value)
		: mp_raw(0), mp_list(0), m_capacity(0), m_size(0) {
		initialise(in_size);
		fill(in_value);
	}

	template <typename T>
	CxPODList<T>::CxPODList(T *in_array, CxI32 in_size, CxCopy inopt_copy)
		: mp_raw(0), mp_list(0), m_capacity(in_size), m_size(in_size) {
		if (in_size > 0) {
			if (inopt_copy == kCxNoCopy) {
				mp_raw = mp_list = in_array;
			}
			else {
			   mp_raw = mp_list = (T *)mem::alloc(sizeof(T)*in_size);
				mem::copy(mp_raw, in_array, sizeof(T)*in_size);
			}
		}
	}

	template <typename T>
	CxPODList<T>::CxPODList(const CxPODList<T> &in_src)
		: mp_raw(0), mp_list(0), mp_store(0), mp_lastStore(0),
		  m_capacity(0), m_size(0) {
	   initialise(0);
		*this = in_src;
	}

	template <typename T>
	CxPODList<T>::~CxPODList() { clear(); }

	template <typename T>
	CxPODList<T> & CxPODList<T>::operator=(const CxPODList<T> &in_src) {
		clear();

		const CxI32 capacity = in_src.m_capacity;
		m_capacity = capacity;
		m_size = in_src.m_size;

		if (capacity > 0) {
			mp_raw = (T *)mem::alloc(sizeof(T)*capacity);
			mp_list = mp_raw + (in_src.mp_list - in_src.mp_raw);
			mem::copy(mp_list, in_src.mp_list, sizeof(T)*m_size);
		}		
		return *this;
	}

	template <typename T>
	CxBool CxPODList<T>::operator==(const CxPODList<T> &in_list) const {
		if (m_size == in_list.m_size) {
			for (CxI32 i = 0; i < m_size; ++i) {
				if (mp_list[i] != in_list.mp_list[i]) { return false; }
			}
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CxBool CxPODList<T>::operator!=(const CxPODList<T> &in_list) const {
		if (m_size == in_list.m_size) {
			for (CxI32 i = 0; i < m_size; ++i) {
				if (mp_list[i] != in_list.mp_list[i]) { return true; }
			}
			return false;
		}
		else { return true; }
	}

	template <typename T>
	CxPODList<T> CxPODList<T>::operator+(const CxPODList<T> &in_list) const {
		CxI32 ret_size = m_size + in_list.m_size;
		if (ret_size > 0) {
			T *ret = new T[ret_size];
			/* Copy from this list */
			mem::copy(ret, mp_list, sizeof(T)*m_size);
			/* Copy from other list */
			mem::copy(ret + m_size, in_list.mp_list, sizeof(T)*in_list.m_size);
			/* Return the new vector */
			return CxPODList<T>(ret, ret_size, kCxNoCopy);
		}
		else { return CxPODList<T>(); }
	}
	
	template <typename T>
	CX_FORCE_INLINE const T & CxPODList<T>::at(CxI32 in_idx) const {
		if (in_idx >= 0 && in_idx < m_size) { return mp_list[in_idx]; }
		else {
			DERR(in_idx >= m_size, "Accessing CxPODList element "
				  << in_idx << " outside range [0.." << m_size << "]!");
			return m_invalidValue;
		}
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODList<T>::append(const T &in_elem) {
		if (m_size == m_capacity) {
			DMSG("AUTO Resizing CxPODList from with length "
				  << m_size << " from " << m_capacity
				  << " to " << m_capacity*2);
			resizeToCapacity(m_capacity*2);
		}
		mp_list[m_size++] = elem;
	}	
	
	template <typename T>
	void CxPODList<T>::append(const CxPODList<T> &in_src) {
		CxI32 capacity = m_size + in_src.m_size;
		if (m_capacity < capacity) {
			resizeToCapacity(capacity);
		}
		CX_TODO("Fix so checks to see if need to move existing down to fit new elements");
		/* Copy all the elements. */
		mem::copy(mp_list + m_size, in_src.mp_list, sizeof(T)*in_src.m_size);
		m_size += in_src.m_size;
	}

	template <typename T>
	CxBool CxPODList<T>::contains(const T &in_value) const {
		for (CxI32 i = 0; i < m_size; ++i) {
			if (mp_list[i] == in_value) { return true; }
		}
		return false;
	}
	
	template <typename T>
	void CxPODList<T>::clear() {
		mem::free(mp_raw);
		mp_list = 0;
		m_size = m_capacity = 0;
	}

	template <typename T>
	void CxPODList<T>::compact() {
		if (m_size == 0) { clear(); }
		else if (m_size < m_capacity) {
			/* Move everything down first if we need to */
			CxI32 start_offset = (mp_list - mp_raw);
			if (start_offset != 0) {
				mem::copy(mp_raw, mp_list, sizeof(T)*m_size);
			}
			/* Then just resize the array */
			mp_list = mem::resize(mp_raw, sizeof(T)*m_size);
			m_capacity = m_size;
		}
	}

	template <typename T>
	CxI32 CxPODList<T>::count(const T &in_value) const {
		CxI32 nm_occurances = 0;
		for (CxI32 i = 0; i < m_size; ++i) {
			if (mp_list[i] == nm_occurances) { nm_occurances += 1; }
		}
		return nm_occurances;
	}

	template <typename T>
	void CxPODList<T>::eraseAll() {
		for (CxI32 i = 0; i < m_size; ++i) {
			if (mp_list[i]) {
				delete mp_list[i]; mp_list[i] = 0;
			}
		}
		m_size = 0;
	}

	template <typename T>
	void CxPODList<T>::eraseLast() {
		if (m_size > 0) {
			delete mp_list[m_size - 1];
			mp_list[--m_size] = 0;
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to erase last item from empty CxPODList!");
		}
#endif
	}

	template <typename T>
   void CxPODList<T>::fill(const T &in_value, CxI32 inopt_size) {
		if (inopt_size == -1) { inopt_size = m_size; }

		resize(inopt_size);

		if (sizeof(T) <= sizeof(CxI32)) {
			mem::set(mp_list, reinterpret_cast<CxI32>(in_value), sizeof(T)*inopt_size);
		}
		else {
			for (CxI32 i = 0; i < inopt_size; ++i) { mp_list[i] = in_value; }
		}
	}

	template <typename T>
	CxI32 CxPODList<T>::indexOf(const T &in_value, CxI32 in_from) const {
		for (CxI32 i = in_from; i < m_size; ++i) {
			if (mp_list[i] == in_value) { return i; }
		}
		return -1;
	}
	
	template <typename T>
	void CxPODList<T>::insert(CxI32 in_idx, const T &in_elem) {
		if (m_size == m_capacity) {
			DMSG("AUTO Resizing CxPODList from with length "
				  << m_size << " from " << m_capacity
				  << " to " << m_capacity*2);
			resizeToCapacity(m_capacity*2);
		}

		CxI32 mv_down = 0;  CxI32 mv_up = 0;
		const CxI32 start_offset = (mp_list - mp_raw);

		if ( (in_idx <= (m_size >> 1) && start_offset != 0) ||
			  (start_offset + m_size >= m_capacity) ) {
			/* Figure out how many elements to move down */
			mv_down = in_idx;
		} else { /* Moving elements up */
			mv_up = m_size - in_idx;
		}

		/* Now, move the elements up or down */
		if (mv_down != 0) {
			mem::copy(mp_list - 1, mp_list, sizeof(T)*mv_down);
		} else if (mv_up != 0) {
			mem::copy(mp_list + in_idx + 1, mp_list + in_idx, sizeof(T)*mv_up);
		}

		/* Adjust the start of the list */
		if (mv_down != 0 || (in_idx == 0 && mv_up == 0)) { mp_list--; }

		mp_list[in_idx] = in_elem;
		++m_size;
	}

	template <typename T>
	void CxPODList<T>::insert(CxI32 in_idx, const T &in_elem, CxI32 in_count) {
		CX_TODO("Implement this fucker");
	}

	template <typename T>
	CxI32 CxPODList<T>::lastIndexOf(const T &in_value, CxI32 in_from) const {
		if (in_from == -1) { in_from = m_size - 1; }
		
		for (CxI32 i = in_from; i >= 0; --i) {
			if (mp_list[i] == in_value) { return i; }
		}
		return -1;
	}

	template <typename T>
	CxBool CxPODList<T>::remove(const T &in_value) {
		CxI32 idx = indexOf(elem);
		if (idx != -1) {
			priv_removeAt(idx);
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CxI32 CxPODList<T>::removeAll(const T &in_value) {
		CxI32 count = 0;
		CxI32 i = 0;

		while (i < m_size) {
			if (mp_list[i] == in_value) {
			   priv_removeAt(i);
				++count;
			}
			else { ++i; }
		}
		return count;
	}

	template <typename T>
	CxBool CxPODList<T>::removeAt(CxI32 in_idx) {
		if (in_idx >= 0 && in_idx < m_size) {
			priv_removeAt(in_idx);
		}
		else {
			DWARN("Cannot remove element at ["
					<< idx << "], must be within [0.."
					<< m_size-1 << "].");
			return false;			
		}		
	}
	
	template <typename T>
	CX_FORCE_INLINE void CxPODList<T>::removeFirst() {
		if (m_size > 0) {
			++mp_list; --m_size;
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to remove first item from empty CxPODList!");
		}
#endif
	}

	template <typename T>
	void CxPODList<T>::removeLast() {
		if (m_size > 0) { --m_size; }
#if defined (DEBUG)
		else {
			DWARN("Trying to remove last item from empty CxPODList!");
		}
#endif
	}

	template <typename T>
	CX_FORCE_INLINE CxBool CxPODList<T>::replace(const T &in_old, const T &in_new) {
		CxI32 idx = indexOf(in_old);
		if (idx != -1) {
		  	mp_list[idx] = in_new;
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CxI32 CxPODList<T>::replaceAll(const T &in_old, const T &in_new) {
	   CxI32 count = 0;
		CxI32 i = 0;

		while (i < m_size) {
			if (mp_list[i] == in_old) {
				mp_list[i] = in_new;
				++count;
			}
			else { ++i; }
		}
		return count;
	}

	template <typename T>
	CX_FORCE_INLINE CxBool CxPODList<T>::replaceAt(CxI32 in_idx, const T &in_value) {
		if (in_idx >= 0 && in_idx < m_size) {
			mp_list[in_idx] = in_value;
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODList<T>::reserve(CxI32 in_capacity) {
		/* Only if reserving more than we have */
		if (in_capacity > m_capacity) { 
			resizeToCapacity(in_capacity);
		}
	}

	template <typename T>
	void CxPODList<T>::resize(CxI32 in_size) {
		if (in_size > m_capacity) {
			resizeToCapacity(in_size);
		}
	   m_size = in_size;
	}

	template <typename T>
	void CxPODList<T>::set(CxI32 in_idx, const T &in_value) {
		if (in_idx >= m_size) {
			if (in_idx >= m_capacity) {
				DMSG("AUTO Resizing CxPODList with capacity "
					  << m_capacity << " to fit index "
					  << in_idx << ".  Resizing to " << (in_idx*2) << ".");
				resizeToCapacity(in_idx*2);
			}
			m_size = in_idx + 1;
		}
		mp_vec[in_idx] = in_value;
	}

	template <typename T>
	void CxPODList<T>::setAll(const T &in_value) {
	   if (sizeof(T) <= sizeof(CxI32)) {
			mem::set(mp_list, reinterpret_cast<CxI32>(in_value), sizeof(T)*m_capacity);
		}
		else {
			for (CxI32 i = 0; i < m_capacity; ++i) {
				mp_list[i] = in_value;
			}
		}
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODList<T>::swap(CxPODList<T> &in_vec) {
		T *tmp_raw = mp_raw;
		T *tmp_list = mp_list;
		CxI32 tmp_cap = m_capacity;
		CxI32 tmp_size = m_size;

		mp_raw = in_vec.mp_raw;
		mp_list = in_vec.mp_list;
		m_capacity = in_vec.capacity;
		m_size = in_vec.size;

		in_vec.mp_raw = tmp_raw;
		in_vec.mp_list = tmp_list;
		in_vec.capacity = tmp_cap;
		in_vec.size = tmp_size;
	}

	template <typename T>
	T CxPODList<T>::takeAt(CxI32 in_idx) {
		if (in_idx >= 0 && in_idx < m_size) {
			T ret = mp_list[in_idx];
			priv_removeAt(in_idx);
			return ret;
		}
		else {
			DERR("Cannot take element at ["
				  << in_idx << "], must be within [0.."
				  << m_size-1 << "].");
			return m_invalidValue;
		}
	}
	
	template <typename T>
	CX_FORCE_INLINE T CxPODList<T>::takeLast() {
		if (m_size > 0) {
		   return mp_list[m_size--];
		}
		else {
			DWARN("Taking last element of EMPTY CxPODList!");
			return m_invalidValue;
		}
	}

	template <typename T>
	CX_FORCE_INLINE T CxPODList<T>::value(CxI32 in_idx) const {
		if (in_idx >= 0 && in_idx < m_size) { return mp_list[in_idx]; }
		else {
			DERR("Cannot get value of element at ["
				  << in_idx << "], must be within [0.."
				  << m_size-1 << "].");
			return m_invalidValue;
		}
	}

	template <typename T>
	CX_FORCE_INLINE T CxPODList<T>::value(CxI32 in_idx, const T &in_oobValue) const {
		if (in_idx >= 0 && in_idx < m_size) { return mp_list[in_idx]; }
		else { return in_oobValue; }
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODList<T>::initialise(CxI32 in_size) {
		CX_ISPTR_INIT;
		resize(in_size);
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODList<T>::resizeToCapacity(CxI32 in_capacity) {
		CxI32 start_offset = (mp_list - mp_raw);
		mem::resize(mp_raw, sizeof(T)*in_capacity);
		mp_list = mp_raw + start_offset;
		m_capacity = in_capacity;
	}

	template <typename T>
	void CxPODList<T>::priv_removeAt(CxI32 in_idx) {
		/* Shift everyting up or down */
		if (in_idx <= (in_idx >> 1)) {
			/* If index is in the first half of list, shift up */
			if (in_idx != 0) {
				mem::copy(mp_list + 1, mp_list, sizeof(T)*in_idx);
			}
			mp_list++;
		}
		else {
			/* Shift down */
			if (in_idx != (m_size - 1)) {
				mem::copy(mp_list + in_idx, mp_list + in_idx + 1, sizeof(T)*(m_size - in_idx));
			}
		}
		--m_size;
	}
	

} // namespace cat

#endif // CX_CORE_COMMON_CXPODLIST_H
