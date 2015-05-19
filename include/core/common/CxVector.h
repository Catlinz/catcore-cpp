#ifndef CX_CORE_COMMON_CXVECTOR_H
#define CX_CORE_COMMON_CXVECTOR_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxVector.h
 * @brief Contains a simple Vector (resizeable array).
 *
 * @author Catlin Zilinski
 * @date Apr 27, 2015
 */

#include "core/common/CxInvasiveStrongPtr.h"

namespace cat {

	/**
	 * @class CxVector CxVector.h "core/common/CxVector.h"
	 * @brief A simple resizeable array.
	 *
	 * @author Catlin Zilinski
	 * @version 3
	 * @since Nov 1, 2013
	 */
	template<typename T>
	class CxVector {
	  public:
		/** @brief Create an empty null vector. */
		inline CxVector();

		/**
		 * @brief Create a new vector with the specified size.
		 * The values are initialise with the default values.
		 * @param in_size The initial size of the vector.
		 */
		inline CxVector(CxI32 in_size);

		/**
		 * @brief Create a new vector with the specified size.
		 * The values are initialised with the specified fill value.
		 * @param in_size The initialise size of the vector.
		 * @param in_value The value to fill the vector with.
		 */
		inline CxVector(CxI32 in_size, const T &in_value);

		/**
		 * @brief Create a new vector from the specified array.
		 * @param in_array The c-array of elements.
		 * @param in_size The number of elements in the array.
		 * @param inopt_copy If kCxNoCopy, then array is not copied, just stored.
		 */
		CxVector(T *in_array, CxI32 in_size, CxCopy inopt_copy = kCxCopy);

		/**
		 * @brief Copy constructor, copies each element.
		 * @param in_src The source vector to create a copy of.
		 */
		CxVector(const CxVector<T> &in_src);

		/** @brief Deletes the contents of the Vector. */
		~CxVector();

		/**
		 * @brief Overloaded Assignment operator, replaces contents with new contents.
		 * @param in_src The CxVector to copy.
		 * @return A reference to this Vector.
		 */
		CxVector<T>& operator=(const CxVector<T> &in_src);

		/**
		 * @brief Operator to test to vectors to equality.
		 * @param in_vec The other vector to test for equality to.
		 * @return True if the two vectors are equal.
		 */
		CxBool operator==(const CxVector<T> &in_vec) const;

		/**
		 * @brief Operator to test two vectors for non-equality.
		 * @param in_vec The other vector to test against.
		 * @return True if the two vectors are NOT equal.
		 */
		CxBool operator!=(const CxVector<T> &in_vec) const;
		
		/**
		 * @brief Get the vector element at the specified index.
		 * @param in_idx The index of the element ot access.
		 * @return A reference to the element.
		 */
		inline T & operator[](CxI32 in_idx) {
			D_CONDERR(in_idx >= m_size, "Accessing Vector element "
						 << in_idx << " outside range [0.." << m_size << "]!");
			return mp_vec[in_idx];
		}

		/**
		 * @brief Get the constant vector element at the specified index.
		 * @param in_idx The index of the element ot access.
		 * @return A constant reference to the element.
		 */
		inline const T & operator[](const CxI32 in_idx) const {
			D_CONDERR(in_idx >= m_size, "Accessing Vector element "
						 << in_idx << " outside range [0.." << m_size << "]!");
			return mp_vec[in_idx];
		}

		/**
		 * @brief Concatenate two vectors and return the resulting new vector.
		 * @param in_vec The vector to append to this one.
		 * @return A new vector that is the result of in_vec appended to this vector.
		 */
		CxVector<T> operator+(const CxVector<T> &in_vec) const;

		/**
		 * @brief Append another vector onto this vector.
		 * @see append()
		 * @see operator<<()
		 * @param in_vec The vector to append to this one.
		 * @return A reference to this vector.
		 */
		inline CxVector<T> & operator+=(const CxVector<T> &in_vec) {
			append(in_vec); return *this;
		}

		/**
		 * @brief Append a value onto this vector.
		 * @see append()
		 * @see operator<<()
		 * @param in_value The value to append to the vector.
		 * @return A reference to this vector.
		 */
		inline CxVector<T> & operator+=(const T &in_value) {
			append(in_value); return *this;
		}
		
		/**
		 * @brief Append another vector onto this vector.
		 * @see append()
		 * @see operator+=()
		 * @param in_vec The vector to append to this one.
		 * @return A reference to this vector.
		 */
		inline CxVector<T> & operator<<(const CxVector<T> &in_vec) {
			append(in_vec); return *this;
		}

		/**
		 * @brief Append a value onto this vector.
		 * @see append()
		 * @see operator+=()
		 * @param in_value The value to append to the vector.
		 * @return A reference to this vector.
		 */
		inline CxVector<T> & operator<<(const T &in_value) {
			append(in_value); return *this;
		}
		
		/**
		 * @brief Append an element onto the end of the vector.
		 * @param elem The element to append to the vector.
		 */
		inline void append(const T& elem);

		/**
		 * @brief Append all the elements from another vector.
		 * @param src The other vector to append the elements from.
		 */
		void append(const CxVector<T>& src);

		/**
		 * @brief Get the value at the specified index.
		 * At always checks to make sure that the range is value, whereas
		 * operator[]() does not.
		 * @see operator[]()
		 * @see value()
		 * @param in_idx The index to get the value at (must be valid).
		 * @return The value at the specified index.
		 */
		inline const T & at(CxI32 in_idx) const;
		
		/** @return The capacity of the vector. */
		inline CxI32 capacity() const { return m_capacity; }
		
		/**
		 * @brief Remove all elements from the array and releases the memory.
		 * This method will NOT result in pointers being deleted in a vector of pointers.
		 * For this behaviour, you must call eraseAll().
		 */
		void clear();

		/**
		 * @brief Releases any unused memory the vector has.
		 * This method simply resizes the underlying array so that 
		 * the capacity is equal to the size.
		 */
		void compact();

		/** @return A const pointer to the actual data in the vector. */
		inline const T * constData() const { return mp_vec; }

		/**
		 * @brief Check to see if the vector contains a certain value.
		 * @param value The value to search for.
		 * @return True if the value is included in the vector.
		 */
		CxBool contains(const T& in_value) const;

		/** @return The number of items in the vector */
		inline CxI32 count() const { return m_size; }

		/**
		 * @brief Get the number of occurances of a specified value.
		 * @param in_value The value to count.
		 * @return The number of occurances of the value in the vector.
		 */
		CxI32 count(const T &in_value) const;

		/** @return A pointer to the actual data in the vector. */
		inline T * data() { return mp_vec; }

		/** @return A const pointer to the actual data in the vector */
		inline const T * data() const { return mp_vec; }

		/**
		 * @brief Test for equality with the last element of the vector.
		 * @param in_value The value to test against the last element.
		 * @return True if the vector is not empty and the last element is equal to in_value.
		 */
		inline CxBool endsWith(const T &in_value) const {
			return (m_size > 0 && mp_vec[m_size - 1] == in_value);
		}

		/**
		 * @brief Remove all elements from an array and delete them. 
		 * This method assumes that the vector contains dynamically allocated 
		 * pointers, and as such, calls delete on all the elements.
		 */
		void eraseAll();

		/**
		 * @brief Removes the last element from the vector and deletes it.
		 * This method assumes that the vector is storing pointers to dynamically allocated 
		 * memory, which was allocated using the 'new' operator.
		 */
		void eraseLast();

		/**
		 * @brief Fill the vector with the specified value.
		 * The method will fill the vector with the specified value, and 
		 * will resize the vector if in_size is > the current size.
		 * @param in_value The value to fill the vector with.
		 * @param inopt_size The optional number of elements to fill.
		 */
		void fill(const T &in_value, CxI32 inopt_size = -1);

		/** @return A reference to the first element in the vector */
		inline T & first() { return *this[0]; }

		/** @return A constant reference to the first element in the vector */
		inline const T & first() const { return *this[0]; }

		/**
		 * @brief Get the index of the first occurance of a value, or -1 if not found.
		 * @param in_value The element to find the index of.
		 * @param in_from The optional index to start searching from.
		 * @return The index of the element or -1 if not found.
		 */
		CxI32 indexOf(const T &in_value, CxI32 in_from = 0) const;

		/**
		 * @brief Insert an item into the Vector.
		 * Inserting an element into the Vector is an O(n) operation, since 
		 * all the elements after the new element will have to be moved down.
		 * The index value to insert into must be within the range [0..length], 
		 * inclusive.
		 * @param in_idx The index to insert the element into.
		 * @param in_elem The element to insert.
		 */
		void insert(CxI32 in_idx, const T &in_elem);

		/**
		 * @brief Insert in_count copies of a value inot the vector.
		 * @see insert(CxI32, const T&).
		 * @param in_idx The index to start inserting into.
		 * @param in_elem The element to insert.
		 * @param in_count The number of times to insert the element.
		 */
		void insert(CxI32 in_idx, const T &in_elem, CxI32 in_count);

		/** @return True if the vector is empty. */
		inline CxBool isEmpty() const { return m_size == 0; }
		
		/** @return A reference to the last element in the vector. */
		inline T & last() { return *this[(m_size - 1)]; }

		/** @return A constant reference to the last element in the vector. */
		inline const T& last() const { return *this[(m_size - 1)]; }

		/**
		 * @brief Get the index of the last occurance of a value, or -1 if not found.
		 * @param in_value The element to find the index of.
		 * @param in_from The optional index to start searching from.
		 * @return The index of the element or -1 if not found.
		 */
		CxI32 lastIndexOf(const T &in_value, CxI32 in_from = -1) const;

		/** @return The current length of the vector. */
		inline CxI32 length() const { return m_size; }

		/**
		 * @brief Insert a value into the front of the vector.
		 * @param in_value The value to insert into the front of the vector.
		 */
		inline void prepend(const T &in_value) { insert(0, in_value); }
		
		/**
		 * @brief Try and remove the specified element from the Vector.
		 * Will only remove the first occurance.
		 * @see removeAt(CxI32);
		 * @param in_value The element to try and remove from the Vector.
		 * @return True if the element was found and removed.
		 */
		CxBool remove(const T &in_value);

		/**
		 * @brief Remove all occurances of a value from the vector.
		 * @see remove()
		 * @param in_value The value to remove all occurances of.
		 * @return The number of occurances found / removed.
		 */
		CxI32 removeAll(const T &in_value);

		/**
		 * @brief Remove an arbitrary element from the Vector.
		 * This method removes an arbitrary element from the vector by 
		 * shifting all the elements after it, up by one, and then 
		 * shrinks the length by one.  This runs in O(n) time.
		 * @param in_idx The indx of the element to remove at.
		 * @return True if the element was removed.
		 */
		CxBool removeAt(CxI32 in_idx);

		/** @brief Remove the first element from the vector. */
		void removeFirst();

		/** @brief Remove the last element from the vector. */
		void removeLast();

		/**
		 * @brief Replace the first occurance of a specified value with a new value.
		 * @param in_old The value to replace.
		 * @param in_new The value to replace with.
		 * @return True if a value was found and replaced.
		 */
		inline CxBool replace(const T &in_old, const T &in_new);

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
		inline CxBool replaceAt(CxI32 in_idx, const T &in_value);

		/**
		 * @brief Reserve the specified capacity in the vector.
		 * @param in_capacity The amount of elements to reserve for the vector.
		 */
		inline void reserve(CxI32 in_capacity);

		/**
		 * @brief Resize the vector to the specified size.
		 * If in_size is greater than the current size, then new elements 
		 * are added to the end of the vector, if less than the current size, 
		 * then elements are removed from the end of the vector.
		 * @param in_size The new size to give the vector.
		 */
		void resize(CxI32 in_size);

		/**
		 * @brief Set the vector element at the specified index.
		 * If the index value is greater than the current length, then 
		 * the length of the array is set to encompass the newly set element.
		 * @param in_idx The index of the element to set.
		 * @param in_value The value of the element.
		 */
		void set(CxI32 in_idx, const T &in_value);

		/**
		 * @brief Set all the values in the vector to the specified value.
		 * This method sets all allocated values to the specified value, 
		 * i.e., from idx = 0, to idx = capacity - 1.
		 * @param in_value The value to set all the values to.
		 */
		void setAll(const T &in_value);

		/** @return The number of elements in the vector. */
		inline CxI32 size() const { return m_size; }

		/**
		 * @brief Method to sort the vector using the given comparator
		 * This method uses the built in qsort to sort the vector.
		 * @param in_compare The method to use to compare two elements.
		 */
		inline void sort(CxI32 (*in_compare)(const void*, const void*)) {
			qsort(mp_vec, m_size, sizeof(T), in_compare);
		}		

		/**
		 * @brief Test to see if the vector begins with the specified value.
		 * @param in_value The value to see if the vector starts with.
		 * @return True if the vector's first element is in_value.
		 */
		inline CxBool startsWith(const T &in_value) const {
			return (m_size > 0 && mp_vec[0] == in_value);
		}

		/**
		 * @brief Swaps the contents of two vectors.
		 * @param in_vec The other vector to swap with.
		 */
		inline void swap(CxVector<T> &in_vec);

		/**
		 * @brief Removes and returns the given value from the vector.
		 * @param in_idx The index of the element to take.
		 * @return A copy of the removed element.
		 */
		T takeAt(CxI32 in_idx);

		/**
		 * @brief Removes and returns the first element in the vector.
		 * @return A copy of the previously first element in the vector.
		 */
		inline T takeFirst() { return takeAt(0); }
		
		/**
		 * @brief Removes and returns the last element in the vector.
		 * @return A copy of the previously last element in the vector.
		 */
		inline T takeLast();

		/**
		 * @brief Get a copy of the value at the specified index.
		 * At always checks to make sure that the range is value, whereas
		 * operator[] does not.
		 * @see operator[]()
		 * @see at()
		 * @param in_idx The index to get the value at (must be valid).
		 * @return A copy of the value at the specified index.
		 */
		inline T value(CxI32 in_idx) const;

		/**
		 * @brief Get a copy of the value at the specified index.
		 * @see value()
		 * @param in_idx The index to get the value at (must be valid).
		 * @param in_oobValue The value to return for an out-of-bounds index.
		 * @return A copy of the value at the specified index or in_oobValue.
		 */
		inline T value(CxI32 in_idx, const T &in_oobValue) const;

		CX_ISPTR_METHODS;

	  private:
		inline void initialise(CxI32 in_size);

		CX_FORCE_INLINE void priv_removeAt(CxI32 in_idx) {
			/* Shift everything down by one */
			for (CxI32 i = in_idx; i < m_size-1; ++i) {
				mp_vec[i] = mp_vec[i+1];
			}
			mp_vec[--m_size] = m_invalidValue;
			return true;
		}

		void resizeToCapacity(CxI32 capacity);

		T		  *mp_vec;		   /**< The actual vector data */
		T m_invalidValue; /**< The value returned if outside bounds */
		CxI32		m_capacity;		/**< The current capacity of the Vector */
		CxI32		m_size;		/**< The number of elements in the Vector */
		
		CX_ISPTR_FIELDS;
	};

	template <typename T>
	inline CxVector<T>::CxVector()
		: mp_vec(0), m_capacity(0), m_size(0) { initialise(0); }

	template <typename T>
	inline CxVector<T>::CxVector(CxI32 in_size)
		: mp_vec(0), m_capacity(0), m_size(0) { initialise(in_size); }

	template <typename T>
	inline CxVector<T>::CxVector(CxI32 in_size, const T &in_value)
		: mp_vec(0), m_capacity(0), m_size(0) {
		initialise(in_size);
		fill(in_value);
	}

	template <typename T>
	CxVector<T>::CxVector(T *in_array, CxI32 in_size, CxCopy inopt_copy)
		: mp_vec(0), m_capacity(in_size), m_size(in_size) {
		if (inopt_copy == kCxNoCopy) {
			mp_vec = in_array;
		}
		else if (in_size > 0) {
			mp_vec = new T[in_size];
			for (CxI32 i = 0; i < in_size; ++i) {
				mp_vec[i] = in_array[i];
			}
		}
	}
	template <typename T>
	CxVector<T>::CxVector(const CxVector<T> &in_src)
		: mp_vec(0), m_capacity(0), m_size(0) {
	   initialise(0);
		*this = in_src;
	}

	template <typename T>
	CxVector<T>::~CxVector() { clear(); }

	template <typename T>
	CxVector<T> & CxVector<T>::operator=(const CxVector<T> &in_src) {
		if (mp_vec) {
			delete[] mp_vec; mp_vec = 0;			
		}
		CxI32 capacity = in_src.m_capacity;
		m_capacity = capacity;
		m_size = in_src.m_size;
		
		if (capacity > 0) {
			mp_vec = new T[capacity];
			/* Copy all the elements (calls copy constr.) */
			for (CxI32 i = 0; i < in_src.m_size; ++i) {
				mp_vec[i] = in_src.mp_vec[i];
			}
		}		
		return *this;
	}

	template <typename T>
	CxBool CxVector<T>::operator==(const CxVector<T> &in_vec) const {
		if (m_size == in_vec.m_size) {
			for (CxI32 i = 0; i < m_size; ++i) {
				if (mp_vec[i] != in_vec.mp_vec[i]) {
					return false;
				}
			}
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CxBool CxVector<T>::operator!=(const CxVector<T> &in_vec) const {
		if (m_size == in_vec.m_size) {
			for (CxI32 i = 0; i < m_size; ++i) {
				if (mp_vec[i] != in_vec.mp_vec[i]) {
					return true;
				}
			}
			return false;
		}
		else { return true; }
	}

	template <typename T>
	CxVector<T> CxVector<T>::operator+(const CxVector<T> &in_vec) const {
		CxI32 ret_size = m_size + in_vec.m_size;
		if (ret_size > 0) {
			T *ret = new T[ret_size];
			/* Copy from this vector */
			for (CxI32 i = 0; i < m_size; ++i) { ret[i] = mp_vec[i]; }

			/* Copy from other vector */
			T *ptr = &(ret[m_size]);
			for (CxI32 i = 0; i < in_vec.m_size; ++i) { ptr[i] = in_vec.mp_vec[i]; }

			/* Return the new vector */
			return CxVector<T>(ret_size, ret, kCxNoCopy);
		}
		else {
			return CxVector<T>();
		}
	}
	template <typename T>
	inline const T & CxVector<T>::at(CxI32 in_idx) const {
		if (in_idx >= 0 && in_idx < m_size) {
			return mp_vec[in_idx];
		}
		else {
			DERR(in_idx >= m_size, "Accessing CxVector element "
				  << in_idx << " outside range [0.." << m_size << "]!");
			return m_invalidValue;
		}
	}

	template <typename T>
	inline void CxVector<T>::append(const T &in_elem) {
		if (m_size == m_capacity) {
			DMSG("AUTO Resizing CxVector from with length "
				  << m_size << " from " << m_capacity
				  << " to " << m_capacity*2);
			resizeToCapacity(m_capacity*2);
		}
		mp_vec[m_size++] = elem;
	}	
	
	template <typename T>
	void CxVector<T>::append(const CxVector<T> &in_src) {
		CxI32 capacity = m_size + in_src.m_size;
		if (m_capacity < capacity) {
			resizeToCapacity(capacity);
		}
		/* Copy all the elements (calls copy constr.). */
		for (CxI32 i = 0; i < in_src.m_size; ++i) {
			mp_vec[m_size + i] = in_src.mp_vec[i];
		}
		m_size += in_src.m_size;
	}

	template <typename T>
	CxBool CxVector<T>::contains(const T &in_value) const {
		for (CxI32 i = 0; i < m_size; ++i) {
			if (mp_vec[i] == in_value) { return true; }
		}
		return false;
	}
	
	template <typename T>
	void CxVector<T>::clear() {
		if (mp_vec) {
			delete[] mp_vec; mp_vec = 0;
		}
		m_size = m_capacity = 0;
	}

	template <typename T>
	void CxVector<T>::compact() {
		if (m_size < m_capacity) {
			T *new_vec = new T[m_size];
			for (int i = 0; i < m_size; ++i) {
				new_vec[i] = mp_vec[i];
			}
			m_capacity = m_size;
			delete[] mp_vec;
			mp_vec = new_vec;
		}
	}

	template <typename T>
	CxI32 CxVector<T>::count(const T &in_value) const {
		CxI32 nm_occurances = 0;
		for (CxI32 i = 0; i < m_size; ++i) {
			if (mp_vec[i] == nm_occurances) { nm_occurances += 1; }
		}
		return nm_occurances;
	}

	template <typename T>
	void CxVector<T>::eraseAll() {
		for (CxI32 i = 0; i < m_size; ++i) {
			if (mp_vec[i]) {
				delete mp_vec[i]; mp_vec[i] = 0;
			}
		}
		m_size = 0;
	}

	template <typename T>
	void CxVector<T>::eraseLast() {
		if (m_size > 0) {
			delete mp_vec[m_size-1];
			mp_vec[--m_size] = 0;
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to erase last item from empty CxVector!");
		}
#endif
	}

	template <typename T>
   void CxVector<T>::fill(const T &in_value, CxI32 inopt_size) {
		if (inopt_size == -1) {
			inopt_size = m_size;
		}

		resize(inopt_size);
		for (CxI32 i = 0; i < inopt_size; ++i) {
			mp_vec[i] = in_value;
		}
	}

	template <typename T>
	CxI32 CxVector<T>::indexOf(const T &in_value, CxI32 in_from) const {
		for (CxI32 i = in_from; i < m_size; ++i) {
			if (mp_vec[i] == in_value) { return i; }
		}
		return -1;
	}
	
	template <typename T>
	void CxVector<T>::insert(CxI32 in_idx, const T &in_elem) {
		if (m_size == m_capacity) {
			DMSG("AUTO Resizing CxVector from with length "
				  << m_size << " from " << m_capacity
				  << " to " << m_capacity*2);
			resizeToCapacity(m_capacity*2);
		}
		/* Move all the elements after down one */
		for (CxI32 i = m_size - 1; i >= in_idx; --i) {
			mp_vec[i+1] = mp_vec[i];
		}
		mp_vec[in_idx] = in_elem;
		++m_size;
	}

	template <typename T>
	void CxVector<T>::insert(CxI32 in_idx, const T &in_elem, CxI32 in_count) {
		if (m_size + in_count > m_capacity) {
			CxI32 resize_to = ((m_size + in_count) > m_capacity * 2) ? m_size + in_count : m_capacity * 2;
			DMSG("AUTO Resizing CxVector from with length "
				  << m_size << " from " << m_capacity
				  << " to " << resize_to);
			resizeToCapacity(resize_to);
		}
		/* Move all the elements after down by in_count */
		for (CxI32 i = m_size - 1; i >= in_idx; --i) {
			mp_vec[i+in_count] = mp_vec[i];
		}
		/* insert all the copies of the value */
		for (CxI32 i = in_idx; i < in_idx + in_count; ++i) {
			mp_vec[i] = in_elem;
		}
		m_size += in_count;
	}

	template <typename T>
	CxI32 CxVector<T>::lastIndexOf(const T &in_value, CxI32 in_from) const {
		if (in_from == -1) { in_from = m_size - 1; }
		
		for (CxI32 i = in_from; i >= 0; --i) {
			if (mp_vec[i] == in_value) { return i; }
		}
		return -1;
	}

	template <typename T>
	CxBool CxVector<T>::remove(const T &in_value) {
		CxI32 idx = indexOf(elem);
		if (idx != -1) {
			priv_removeAt(idx);
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CxI32 CxVector<T>::removeAll(const T &in_value) {
		CxI32 count = 0;
		CxI32 i = 0;

		while (i < m_size) {
			if (mp_vec[i] == in_value) {
			   priv_removeAt(i);
				++count;
			}
			else { ++i; }
		}
		return count;
	}

	template <typename T>
	CxBool CxVector<T>::removeAt(CxI32 in_idx) {
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
	void CxVector<T>::removeFirst() {
		if (m_size > 0) {
			for (CxI32 i = 0; i < m_size-1; ++i) {
				mp_vec[i] = mp_vec[i+1];
			}
			mp_vec[--m_size] = m_invalidValue;
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to remove first item from empty CxVector!");
		}
#endif
	}

	template <typename T>
	void CxVector<T>::removeLast() {
		if (m_size > 0) {
			mp_vec[--m_size] = m_invalidValue;
		}
#if defined (DEBUG)
		else {
			DWARN("Trying to remove last item from empty CxVector!");
		}
#endif
	}

	template <typename T>
	inline CxBool CxVector<T>::replace(const T &in_old, const T &in_new) {
		CxI32 idx = indexOf(in_old);
		if (idx != -1) {
			mp_vec[idx] = in_new;
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CxI32 CxVector<T>::replaceAll(const T &in_old, const T &in_new) {
	   CxI32 count = 0;
		CxI32 i = 0;

		while (i < m_size) {
			if (mp_vec[i] == in_old) {
				mp_vec[i] = in_new;
				++count;
			}
			else { ++i; }
		}
		return count;
	}

	template <typename T>
	inline CxBool CxVector<T>::replaceAt(CxI32 in_idx, const T &in_value) {
		if (in_idx >= 0 && in_idx < m_size) {
			mp_vec[in_idx] = in_value;
			return true;
		}
		else { return false; }
	}

	template <typename T>
	inline void CxVector<T>::reserve(CxI32 in_capacity) {
		/* Only if reserving more than we have */
		if (in_capacity > m_capacity) { 
			resizeToCapacity(in_capacity);
		}
	}

	template <typename T>
	void CxVector<T>::resize(CxI32 in_size) {
		if (in_size > m_capacity) {
			resizeToCapacity(in_size);
		}
		else if (in_size < m_size) {
			for (int i = in_size; i < m_size; ++i) {
				mp_vec[i] = m_invalidValue;
			}
		}
	   m_size = in_size;
	}

	template <typename T>
	void CxVector<T>::set(CxI32 in_idx, const T &in_value) {
		if (in_idx >= m_size) {
			if (in_idx >= m_capacity) {
				DMSG("AUTO Resizing CxVector with capacity "
					  << m_capacity << " to fit index "
					  << in_idx << ".  Resizing to " << (in_idx*2) << ".");
				resizeToCapacity(in_idx*2);
			}
			m_size = in_idx + 1;
		}
		mp_vec[in_idx] = in_value;
	}

	template <typename T>
	void CxVector<T>::setAll(const T &in_value) {
		for (CxI32 i = 0; i < m_capacity; ++i) {
			mp_vec[i] = in_value;
		}
	}

	template <typename T>
	inline void CxVector<T>::swap(CxVector<T> &in_vec) {
		T *tmp = mp_vec;
		CxI32 tmp_cap = m_capacity;
		CxI32 tmp_size = m_size;

		mp_vec = in_vec.mp_vec;
		m_capacity = in_vec.capacity;
		m_size = in_vec.size;

		in_vec.mp_vec = tmp;
		in_vec.capacity = tmp_cap;
		in_vec.size = tmp_size;
	}

	template <typename T>
	T CxVector<T>::takeAt(CxI32 in_idx) {
		if (in_idx >= 0 && in_idx < m_size) {
			T ret = mp_vec[in_idx];
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
	inline T CxVector<T>::takeLast() {
		if (m_size > 0) {
			T ret = mp_vec[m_size - 1];
			mp_vec[--m_size] = m_invalidValue;
			return ret;
		}
		else {
			DWARN("Taking last element of EMPTY CxVector!");
			return m_invalidValue;
		}
	}

	template <typename T>
	inline T CxVector<T>::value(CxI32 in_idx) const {
		if (in_idx >= 0 && in_idx < m_size) {
			return mp_vec[in_idx];
		}
		else {
			DERR("Cannot get value of element at ["
				  << in_idx << "], must be within [0.."
				  << m_size-1 << "].");
			return m_invalidValue;
		}
	}

	template <typename T>
	inline T CxVector<T>::value(CxI32 in_idx, const T &in_oobValue) const {
		if (in_idx >= 0 && in_idx < m_size) {
			return mp_vec[in_idx];
		}
		else { return in_oobValue; }
	}

	template <typename T>
	inline void CxVector<T>::initialise(CxI32 in_size) {
		CX_ISPTR_INIT;
		resize(in_size);
	}

	template <typename T>
	void CxVector<T>::resizeToCapacity(CxI32 in_capacity) {
		m_capacity = in_capacity;
		T* newData = new T[in_capacity];
		if (mp_vec) {
			for (CxI32 i = 0; i < m_size; ++i) {
				newData[i] = mp_vec[i];
			}
			delete[] mp_vec;
		}			
		mp_vec = newData;
	}

} // namespace cat

#endif // CX_CORE_COMMON_CXVECTOR_H
