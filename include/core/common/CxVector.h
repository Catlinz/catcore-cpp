#ifndef CX_CORE_COMMON_CXVECTOR_H
#define CX_CORE_COMMON_CXVECTOR_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxVector.h
 * @brief Contains a simple Vector (resizeable array).
 *
 * @author Catlin Zilinski
 * @date Sept 29, 2015
 */

#include "core/common/CxInvasiveStrongPtr.h"
#include "core/common/CxMem.h"

namespace cat {

		/**
	 * @class _CxBaseVector CxVector.h "core/common/CxVector.h"
	 * @brief A base class for the comples and simple vector classes.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 29, 2015
	 */
	template<typename T>
	class _CxBaseVector {
		 public:
		
		/** @brief Create an empty null vector. */
		_CxBaseVector();

		/**
		 * @brief Move constructor, steals the array.
		 * @param in_src The source vector to move the data from.
		 */
		_CxBaseVector(_CxBaseVector<T> &&in_src);

		/**
		 * @brief Operator to test to vectors to equality.
		 * @param in_vec The other vector to test for equality to.
		 * @return True if the two vectors are equal.
		 */
		CxBool operator==(const _CxBaseVector<T> &in_vec) const;

		/**
		 * @brief Operator to test two vectors for non-equality.
		 * @param in_vec The other vector to test against.
		 * @return True if the two vectors are NOT equal.
		 */
		CxBool operator!=(const _CxBaseVector<T> &in_vec) const;
		
		/**
		 * @brief Get the vector element at the specified index.
		 * @param in_idx The index of the element ot access.
		 * @return A reference to the element.
		 */
		T & operator[](CxI32 in_idx);

		/**
		 * @brief Get the constant vector element at the specified index.
		 * @param in_idx The index of the element ot access.
		 * @return A constant reference to the element.
		 */
		const T & operator[](const CxI32 in_idx) const;

		/**
		 * @brief Get the value at the specified index.
		 * At always checks to make sure that the range is value, whereas
		 * operator[]() does not.
		 * @see operator[]()
		 * @see value()
		 * @param in_idx The index to get the value at (must be valid).
		 * @return The value at the specified index.
		 */
		const T & at(CxI32 in_idx) const;
		
		/** @return The capacity of the vector. */
		CX_FORCE_INLINE CxI32 capacity() const { return m_capacity; }
		
		/**
		 * @brief Clears the size of the vector to 0.
		 * This method does not deallocate any memory or delete any pointers.
		 * eraseAll() is the equivilent when dealing with pointers that need 
		 * to be deleted.
		 */
		CX_FORCE_INLINE void clear() { m_size = 0; }

		/** @return A const pointer to the actual data in the vector. */
		CX_FORCE_INLINE const T * constData() const { return mp_vec; }

		/**
		 * @brief Check to see if the vector contains a certain value.
		 * @param value The value to search for.
		 * @return True if the value is included in the vector.
		 */
		CxBool contains(const T& in_value) const;

		/** @return The number of items in the vector */
		CX_FORCE_INLINE CxI32 count() const { return m_size; }

		/**
		 * @brief Get the number of occurances of a specified value.
		 * @param in_value The value to count.
		 * @return The number of occurances of the value in the vector.
		 */
		CxI32 count(const T &in_value) const;

		/** @return A pointer to the actual data in the vector. */
		CX_FORCE_INLINE T * data() { return mp_vec; }

		/** @return A const pointer to the actual data in the vector */
		CX_FORCE_INLINE const T * data() const { return mp_vec; }
		
		/**
		 * @brief Test for equality with the last element of the vector.
		 * @param in_value The value to test against the last element.
		 * @return True if the vector is not empty and the last element is equal to in_value.
		 */
		CX_FORCE_INLINE CxBool endsWith(const T &in_value) const {
			return (m_size > 0 && mp_vec[m_size - 1] == in_value);
		}

		/** @return A reference to the first element in the vector */
		CX_FORCE_INLINE T & first() { return (*this)[0]; }

		/** @return A constant reference to the first element in the vector */
		CX_FORCE_INLINE const T & first() const { return (*this)[0]; }

		/**
		 * @brief Get the index of the first occurance of a value, or -1 if not found.
		 * @param in_value The element to find the index of.
		 * @param in_from The optional index to start searching from.
		 * @return The index of the element or -1 if not found.
		 */
		CxI32 indexOf(const T &in_value, CxI32 in_from = 0) const;

		/** @return True if the vector is empty. */
		CX_FORCE_INLINE CxBool isEmpty() const { return m_size == 0; }
		
		/** @return A reference to the last element in the vector. */
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

		/** @return The current length of the vector. */
		CX_FORCE_INLINE CxI32 length() const { return m_size; }

		/** @brief Remove the last element from the vector. */
		void removeLast();

		/**
		 * @brief Replace the first occurance of a specified value with a new value.
		 * @param in_old The value to replace.
		 * @param in_new The value to replace with.
		 * @return True if a value was found and replaced.
		 */
		CxBool replace(const T &in_old, const T &in_new);
		CxBool replace(const T &in_old, T &&in_new);

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
		CxBool replaceAt(CxI32 in_idx, const T &in_value);
		CxBool replaceAt(CxI32 in_idx, T &&in_value);
		
		/**
		 * @brief Set all the values in the vector to the specified value.
		 * This method sets all allocated values to the specified value, 
		 * i.e., from idx = 0, to idx = capacity - 1.
		 * @param in_value The value to set all the values to.
		 */
		void setAll(const T &in_value);

		/** @return The number of elements in the vector. */
		CX_FORCE_INLINE CxI32 size() const { return m_size; }

		/**
		 * @brief Method to sort the vector using the given comparator
		 * This method uses the built in qsort to sort the vector.
		 * @param in_compare The method to use to compare two elements.
		 */
		CX_FORCE_INLINE void sort(CxI32 (*in_compare)(const void*, const void*)) {
			qsort(mp_vec, m_size, sizeof(T), in_compare);
		}

		/**
		 * @brief Method to sort the vector in ascending order.
		 * The objects just implement the == and < operators.
		 */
		CX_FORCE_INLINE void sortAsc() {
			qsort(mp_vec, m_size, sizeof(T), CxComparePtrAsc<T>);
		}

		/**
		 * @brief Method to sort the vector in descending order.
		 * The objects just implement the == and < operators.
		 */
		CX_FORCE_INLINE void sortDesc() {
			qsort(mp_vec, m_size, sizeof(T), CxComparePtrDesc<T>);
		}

		/**
		 * @brief Test to see if the vector begins with the specified value.
		 * @param in_value The value to see if the vector starts with.
		 * @return True if the vector's first element is in_value.
		 */
		CX_FORCE_INLINE CxBool startsWith(const T &in_value) const {
			return (m_size > 0 && mp_vec[0] == in_value);
		}

		/**
		 * @brief Swaps the contents of two vectors.
		 * @param in_vec The other vector to swap with.
		 */
		void swap(_CxBaseVector<T> &in_vec);
		
		/**
		 * @brief Removes and returns the last element in the vector.
		 * @return A copy of the previously last element in the vector.
		 */
		T takeLast();

		/**
		 * @brief Get a reference of the value at the specified index.
		 * At always checks to make sure that the range is value, whereas
		 * operator[] does not.
		 * @see operator[]()
		 * @see at()
		 * @param in_idx The index to get the value at (must be valid).
		 * @return A reference of the value at the specified index.
		 */
		const T & value(CxI32 in_idx) const;

		/**
		 * @brief Get a reference of the value at the specified index.
		 * @see value()
		 * @param in_idx The index to get the value at (must be valid).
		 * @param in_oobValue The value to return for an out-of-bounds index.
		 * @return A reference of the value at the specified index or in_oobValue.
		 */
		const T & value(CxI32 in_idx, const T &in_oobValue) const;

		CX_ISPTR_METHODS;

	  protected:
		T		  *mp_vec;		   /**< The actual vector data */
		T m_invalidValue; /**< The value returned if outside bounds */
		CxI32		m_capacity;		/**< The current capacity of the Vector */
		CxI32		m_size;		/**< The number of elements in the Vector */

	  private:
		CX_ISPTR_FIELDS;
	};

		template <typename T>
	CX_FORCE_INLINE _CxBaseVector<T>::_CxBaseVector()
		: mp_vec(0), m_capacity(0), m_size(0) { CX_ISPTR_INIT; }

	template <typename T>
	CX_FORCE_INLINE _CxBaseVector<T>::_CxBaseVector(_CxBaseVector<T> &&in_src)
		: mp_vec(in_src.mp_vec), m_capacity(in_src.m_capacity), m_size(in_src.m_size) {
		CX_ISPTR_INIT;
		in_src.mp_vec = 0;
	}

	template <typename T>
	CxBool _CxBaseVector<T>::operator==(const _CxBaseVector<T> &in_vec) const {
		if (m_size == in_vec.m_size) {
			for (CxI32 i = 0; i < m_size; ++i) {
				if (mp_vec[i] != in_vec.mp_vec[i]) { return false; }
			}
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CX_FORCE_INLINE CxBool _CxBaseVector<T>::operator!=(const _CxBaseVector<T> &in_vec) const {
	   return !(*this == in_vec);
	}

	template <typename T>
	CX_FORCE_INLINE T & _CxBaseVector<T>::operator[](CxI32 in_idx) {
		CXD_IF_ERR(in_idx >= m_size,
					  "Accessing CxVector [%d] outside range [0...%d]",
					  in_idx, m_size-1);
		return mp_vec[in_idx];
	}

	template <typename T>
	CX_FORCE_INLINE const T & _CxBaseVector<T>::operator[](const CxI32 in_idx) const {
		CXD_IF_ERR(in_idx >= m_size,
					  "Accessing CxVector [%d] outside range [0...%d]",
					  in_idx, m_size-1);
		return mp_vec[in_idx];
	}

	template <typename T>
	CX_FORCE_INLINE const T & _CxBaseVector<T>::at(CxI32 in_idx) const {
		if (in_idx >= 0 && in_idx < m_size) {
			return mp_vec[in_idx];
		}
		else {
			CXD_ERR("Accessing _CxBaseVector [%d] outside range [0...%d]",
					  in_idx, m_size-1);
			return m_invalidValue;
		}
	}
	
	template <typename T>
	CxBool _CxBaseVector<T>::contains(const T &in_value) const {
		const CxI32 v_size = m_size;
		for (CxI32 i = 0; i < v_size; ++i) {
			if (mp_vec[i] == in_value) { return true; }
		}
		return false;
	}

	template <typename T>
	CxI32 _CxBaseVector<T>::count(const T &in_value) const {
		const CxI32 v_size = m_size;
		CxI32 nm_occurances = 0;
		for (CxI32 i = 0; i < v_size; ++i) {
			if (mp_vec[i] == in_value) { ++nm_occurances; }
		}
		return nm_occurances;
	}

	template <typename T>
	CxI32 _CxBaseVector<T>::indexOf(const T &in_value, CxI32 in_from) const {
		CXD_IF_ERR(in_from < 0, "Cannot start searching from index %d!",
					  in_from);
		const CxI32 v_size = m_size;
		for (CxI32 i = in_from; i < v_size; ++i) {
			if (mp_vec[i] == in_value) { return i; }
		}
		return -1;
	}

	template <typename T>
	CxI32 _CxBaseVector<T>::lastIndexOf(const T &in_value, CxI32 in_from) const {
		CXD_IF_ERR(in_from >= m_size,
					  "Cannot search _CxBaseVector[0...%d] from index %d!",
					  m_size - 1, in_from);
		if (in_from < 0) { in_from = m_size - 1; }
		
		for (CxI32 i = in_from; i >= 0; --i) {
			if (mp_vec[i] == in_value) { return i; }
		}
		return -1;
	}

	template <typename T>
	void _CxBaseVector<T>::removeLast() {
		CXD_IF_WARN(m_size <= 0, "Trying to remove last item from empty CxVector.");
		if (m_size > 0) { mp_vec[--m_size] = m_invalidValue; }
	}

	template <typename T>
	CX_FORCE_INLINE CxBool _CxBaseVector<T>::replace(const T &in_old, const T &in_new) {
		CxI32 idx = indexOf(in_old);
		if (idx != -1) { mp_vec[idx] = in_new;  return true; }
		else { return false; }
	}

	template <typename T>
	CX_FORCE_INLINE CxBool _CxBaseVector<T>::replace(const T &in_old, T &&in_new) {
		CxI32 idx = indexOf(in_old);
		if (idx != -1) { mp_vec[idx] = in_new;  return true; }
		else { return false; }
	}

	template <typename T>
	CxI32 _CxBaseVector<T>::replaceAll(const T &in_old, const T &in_new) {
		const CxI32 v_size = m_size;
		CxI32 count = 0;
		CxI32 i = 0;
		
		while (i < v_size) {
			if (mp_vec[i] == in_old) { mp_vec[i] = in_new;  ++count; }
			else { ++i; }
		}
		return count;
	}

	template <typename T>
	CX_FORCE_INLINE CxBool _CxBaseVector<T>::replaceAt(CxI32 in_idx, const T &in_value) {
		if (in_idx >= 0 && in_idx < m_size) {
			mp_vec[in_idx] = in_value;  return true;
		}
		else { return false; }
	}

	template <typename T>
	CX_FORCE_INLINE CxBool _CxBaseVector<T>::replaceAt(CxI32 in_idx, T &&in_value) {
		if (in_idx >= 0 && in_idx < m_size) {
			mp_vec[in_idx] = in_value;  return true;
		}
		else { return false; }
	}

	template <typename T>
	void _CxBaseVector<T>::setAll(const T &in_value) {
		for (CxI32 i = 0; i < m_capacity; ++i) { mp_vec[i] = in_value; }
	}

	template <typename T>
	CX_FORCE_INLINE void _CxBaseVector<T>::swap(_CxBaseVector<T> &in_vec) {
		T *tmp = mp_vec;
		CxI32 tmp_cap = m_capacity;
		CxI32 tmp_size = m_size;

		mp_vec = in_vec.mp_vec;
		m_capacity = in_vec.m_capacity;
		m_size = in_vec.m_size;

		in_vec.mp_vec = tmp;
		in_vec.m_capacity = tmp_cap;
		in_vec.m_size = tmp_size;
	}

	template <typename T>
	CX_FORCE_INLINE T _CxBaseVector<T>::takeLast() {
		if (m_size > 0) {
			T ret = static_cast<T &&>(mp_vec[m_size - 1]);
			mp_vec[--m_size] = m_invalidValue;
			return ret;
		}
		else {
			CXD_WARN("Taking last element of empty CxVector.");
			return m_invalidValue;
		}
	}

	template <typename T>
	CX_FORCE_INLINE const T & _CxBaseVector<T>::value(CxI32 in_idx) const {
		if (in_idx >= 0 && in_idx < m_size) { return mp_vec[in_idx]; }
		else {
			CXD_ERR("Cannot get value of [%d] from CxVector[0...%d]!",
					  in_idx, m_size-1);
			return m_invalidValue;
		}
	}

	template <typename T>
	CX_FORCE_INLINE const T & _CxBaseVector<T>::value(CxI32 in_idx, const T &in_oobValue) const {
		if (in_idx >= 0 && in_idx < m_size) { return mp_vec[in_idx]; }
		else { return in_oobValue; }
	}

	/**
	 * @class CxVector CxVector.h "core/common/CxVector.h"
	 * @brief A simple resizeable array.
	 *
	 * @author Catlin Zilinski
	 * @version 4
	 * @since Nov 1, 2013
	 */
	template<typename T>
	class CxVector : public _CxBaseVector<T> {
	  public:

		/** @brief Typdef for the InvasiveStrongPtr template for the CxVector. */
		typedef typename CxInvasiveStrongPtr< CxVector<T> > Ptr;
		
		/** @brief Create an empty null vector. */
		CX_FORCE_INLINE CxVector() : _CxBaseVector<T>() {}
		
		/**
		 * @brief Create a new vector with the specified size.
		 * The values are initialise with the default values.
		 * @param in_size The initial size of the vector.
		 */
		CxVector(CxI32 in_size);

		/**
		 * @brief Create a new vector with the specified size.
		 * The values are initialised with the specified fill value.
		 * @param in_size The initialise size of the vector.
		 * @param in_value The value to fill the vector with.
		 */
		CxVector(CxI32 in_size, const T &in_value);

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

		/**
		 * @brief Move constructor, steals the array.
		 * @param in_src The source vector to move the data from.
		 */
		CX_FORCE_INLINE CxVector(CxVector<T> &&in_src) : _CxBaseVector<T>(in_src) {}

		/** @brief Deletes the contents of the Vector. */
		~CxVector();

		/**
		 * @brief Overloaded Assignment operator, replaces contents with new contents.
		 * @param in_src The CxVector to copy.
		 * @return A reference to this Vector.
		 */
		CxVector<T>& operator=(const CxVector<T> &in_src);

		/**
		 * @brief Overloaded Move-Assignment operator, steals the src's contents.
		 * @param in_src The CxVector to move from.
		 * @return A reference to this Vector.
		 */
		CxVector<T>& operator=(CxVector<T> &&in_src);

		/**
		 * @brief Operator to test to vectors to equality.
		 * @param in_vec The other vector to test for equality to.
		 * @return True if the two vectors are equal.
		 */
		CX_FORCE_INLINE CxBool operator==(const CxVector<T> &in_vec) const {
			return _CxBaseVector<T>::operator==(in_vec);
		}

		/**
		 * @brief Operator to test two vectors for non-equality.
		 * @param in_vec The other vector to test against.
		 * @return True if the two vectors are NOT equal.
		 */
		CX_FORCE_INLINE CxBool operator!=(const CxVector<T> &in_vec) const {
			return _CxBaseVector<T>::operator!=(in_vec);
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
		CX_FORCE_INLINE CxVector<T> & operator+=(const CxVector<T> &in_vec) {
			append(in_vec); return *this;
		}

		/**
		 * @brief Append a value onto this vector.
		 * @see append()
		 * @see operator<<()
		 * @param in_value The value to append to the vector.
		 * @return A reference to this vector.
		 */
		CX_FORCE_INLINE CxVector<T> & operator+=(const T &in_value) {
			append(in_value); return *this;
		}
		CX_FORCE_INLINE CxVector<T> & operator+=(T &&in_value) {
			append(in_value); return *this;
		}
		
		/**
		 * @brief Append another vector onto this vector.
		 * @see append()
		 * @see operator+=()
		 * @param in_vec The vector to append to this one.
		 * @return A reference to this vector.
		 */
		CX_FORCE_INLINE CxVector<T> & operator<<(const CxVector<T> &in_vec) {
			append(in_vec); return *this;
		}

		/**
		 * @brief Append a value onto this vector.
		 * @see append()
		 * @see operator+=()
		 * @param in_value The value to append to the vector.
		 * @return A reference to this vector.
		 */
		CX_FORCE_INLINE CxVector<T> & operator<<(const T &in_value) {
			append(in_value); return *this;
		}
		CX_FORCE_INLINE CxVector<T> & operator<<(T &&in_value) {
			append(in_value); return *this;
		}
		
		/**
		 * @brief Append an element onto the end of the vector.
		 * @param elem The element to append to the vector.
		 */
		void append(const T &in_elem);
		void append(T &&in_elem);
		
	   /**
		 * @brief Append all the elements from an array onto this vector.
		 * @param in_src The array to append the elements from.
		 */
		void append(const T *in_src, CxI32 in_size);

		/**
		 * @brief Append all the elements from another vector.
		 * @param in_src The other vector to append the elements from.
		 */
		void append(const CxVector<T> &in_src);

		/**
		 * @brief Releases any unused memory the vector has.
		 * This method simply resizes the underlying array so that 
		 * the capacity is equal to the size.
		 */
		void compact();

		/**
		 * @brief Deallocate all the memory for the vector.
		 * This method deallocates the memory for the vector and resets the 
		 * size and capacity to 0.
		 */
		void free();

		/**
		 * @brief Fill the vector with the specified value.
		 * The method will fill the vector with the specified value, and 
		 * will resize the vector if in_size is > the current size.
		 * @param in_value The value to fill the vector with.
		 * @param inopt_size The optional number of elements to fill.
		 */
		void fill(const T &in_value, CxI32 inopt_size = -1);

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
		void insert(CxI32 in_idx, T &&in_elem);

		/**
		 * @brief Insert in_count copies of a value inot the vector.
		 * @see insert(CxI32, const T&).
		 * @param in_idx The index to start inserting into.
		 * @param in_elem The element to insert.
		 * @param in_count The number of times to insert the element.
		 */
		void insert(CxI32 in_idx, const T &in_elem, CxI32 in_count);

		/**
		 * @brief Insert a value into the front of the vector.
		 * @param in_value The value to insert into the front of the vector.
		 */
		CX_FORCE_INLINE void prepend(const T &in_value) { insert(0, in_value); }
		CX_FORCE_INLINE void prepend(T &&in_value) { insert(0, in_value); }
		
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

		/**
		 * @brief Reserve the specified capacity in the vector.
		 * @param in_capacity The amount of elements to reserve for the vector.
		 */
		void reserve(CxI32 in_capacity);

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
		void set(CxI32 in_idx, T &&in_value);

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
		CX_FORCE_INLINE T takeFirst() { return takeAt(0); }

	  private:
		CX_FORCE_INLINE void priv_removeAt(CxI32 in_idx) {
			/* Shift everything down by one */
			for (CxI32 i = in_idx; i < m_size-1; ++i) {
				mp_vec[i] = static_cast<T &&>(mp_vec[i+1]);
			}
			mp_vec[--m_size] = m_invalidValue;
		}

		void resizeToCapacity(CxI32 capacity);
	};

	template <typename T>
	CX_FORCE_INLINE CxVector<T>::CxVector(CxI32 in_size) : _CxBaseVector<T>() {
		resize(in_size);
	}

	template <typename T>
	CX_FORCE_INLINE CxVector<T>::CxVector(CxI32 in_size, const T &in_value)
		: _CxBaseVector<T>() { resize(in_size);  fill(in_value); }

	template <typename T>
	CxVector<T>::CxVector(T *in_array, CxI32 in_size, CxCopy inopt_copy)
		: _CxBaseVector<T>() {
		m_capacity = m_size = in_size;
		
		if (inopt_copy == kCxNoCopy) { mp_vec = in_array; }
		else if (in_size > 0) {
			mp_vec = new T[in_size];
			for (CxI32 i = 0; i < in_size; ++i) { mp_vec[i] = in_array[i]; }
		}
	}
	
	template <typename T>
	CxVector<T>::CxVector(const CxVector<T> &in_src) : _CxBaseVector<T>() {
		*this = in_src;
	}

	template <typename T> CxVector<T>::~CxVector() { free(); }

	template <typename T>
	CxVector<T> & CxVector<T>::operator=(const CxVector<T> &in_src) {
		CxI32 capacity = in_src.m_capacity;
		m_capacity = capacity;
		m_size = in_src.m_size;

		T *old_vec = mp_vec;
		if (capacity > 0) {
			T *new_vec = new T[capacity];
			/* Copy all the elements (calls copy constr.) */
			for (CxI32 i = 0; i < in_src.m_size; ++i) {
				new_vec[i] = in_src.mp_vec[i];
			}
			mp_vec = new_vec;
		}
		else { mp_vec = 0; }

		if (old_vec != 0) { delete[] old_vec; }
		return *this;
	}

	template <typename T>
	CxVector<T> & CxVector<T>::operator=(CxVector<T> &&in_src) {
	   if (mp_vec != 0) { delete[] mp_vec; }
		mp_vec = in_src.mp_vec;
		m_capacity = in_src.m_capacity;
		m_size = in_src.m_size;
		in_src.mp_vec = 0;
		return *this;
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
			return CxVector<T>(ret, ret_size, kCxNoCopy);
		}
		else {
			return CxVector<T>();
		}
	}

	template <typename T>
	CX_FORCE_INLINE void CxVector<T>::append(const T &in_elem) {
		if (m_size == m_capacity) {
			CXD_MSG("Auto resizing CxVector[%d] (%d to %d).",
					  m_size, m_capacity, m_capacity*2 + 2);
			resizeToCapacity(m_capacity*2 + 2);
		}
		mp_vec[m_size++] = in_elem;
	}	

	template <typename T>
	CX_FORCE_INLINE void CxVector<T>::append(T &&in_elem) {
		if (m_size == m_capacity) {
			CXD_MSG("Auto resizing CxVector[%d] (%d to %d).",
					  m_size, m_capacity, m_capacity*2 + 2);
			resizeToCapacity(m_capacity*2 + 2);
		}
		mp_vec[m_size++] = in_elem;
	}	
	
	template <typename T>
	void CxVector<T>::append(const T *in_src, CxI32 in_size) {
		const CxI32 capacity = m_size + in_size;
		if (m_capacity < capacity) { resizeToCapacity(capacity); }
		
		/* Copy all the elements (calls copy constr.). */
		T *ptr = mp_vec + m_size;
		for (CxI32 i = 0; i < in_size; ++i) { ptr[i] = in_src[i]; }
		m_size += in_size;
	}

	template <typename T>
	void CxVector<T>::append(const CxVector<T> & in_vec) {
		const CxI32 v_size = in_vec.m_size;
		const CxI32 capacity = m_size + v_size;
		if (m_capacity < capacity) { resizeToCapacity(capacity); }
		
		/* Copy all the elements (calls copy constr.). */
		T *ptr = mp_vec + m_size;
		const T *v_src = in_vec.mp_vec;
		for (CxI32 i = 0; i < v_size; ++i) { ptr[i] = v_src[i]; }
		m_size += v_size;
	}

	template <typename T>
	void CxVector<T>::compact() {
		if (m_size < m_capacity) {
			T *new_vec = new T[m_size];
			for (int i = 0; i < m_size; ++i) { new_vec[i] = static_cast<T &&>(mp_vec[i]); }
			m_capacity = m_size;
			delete[] mp_vec;
			mp_vec = new_vec;
		}
	}

	template <typename T>
	CX_FORCE_INLINE void CxVector<T>::free() {
		if (mp_vec != 0) { delete[] mp_vec;  mp_vec = 0; }
		m_capacity = m_size = 0;
	}
	
	template <typename T>
   void CxVector<T>::fill(const T &in_value, CxI32 inopt_size) {
		if (inopt_size == -1) { inopt_size = m_size; }
		resize(inopt_size);
		for (CxI32 i = 0; i < inopt_size; ++i) { mp_vec[i] = in_value; }
	}

	template <typename T>
	void CxVector<T>::insert(CxI32 in_idx, const T &in_elem) {
		if (m_size == m_capacity) {
			CXD_MSG("Auto resizing CxVector[%d] (%d to %d).",
					  m_size, m_capacity, m_capacity*2 + 2);
			resizeToCapacity(m_capacity*2 + 2);
		}
		/* Restrict the index value */
		if (in_idx > m_size) { in_idx = m_size; }
		else if (in_idx < 0) { in_idx = 0; }
		
		/* Move all the elements after down one */
		for (CxI32 i = m_size - 1; i >= in_idx; --i) {
			mp_vec[i+1] = static_cast<T &&>(mp_vec[i]);
		}
		mp_vec[in_idx] = in_elem;
		++m_size;
	}

	template <typename T>
	void CxVector<T>::insert(CxI32 in_idx, T &&in_elem) {
		if (m_size == m_capacity) {
			CXD_MSG("Auto resizing CxVector[%d] (%d to %d).",
					  m_size, m_capacity, m_capacity*2 + 2);
			resizeToCapacity(m_capacity*2 + 2);
		}
		/* Restrict the index value */
		if (in_idx > m_size) { in_idx = m_size; }
		else if (in_idx < 0) { in_idx = 0; }
		
		/* Move all the elements after down one */
		for (CxI32 i = m_size - 1; i >= in_idx; --i) {
			mp_vec[i+1] = static_cast<T &&>(mp_vec[i]);
		}
		mp_vec[in_idx] = in_elem;
		++m_size;
	}

	template <typename T>
	void CxVector<T>::insert(CxI32 in_idx, const T &in_elem, CxI32 in_count) {
		const CxI32 n_size = m_size + in_count;
		if (n_size > m_capacity) {
			const CxI32 cap_2 = m_capacity*2 + 2;
			const CxI32 resize_to = (n_size > cap_2) ? n_size : cap_2;
			CXD_MSG("Auto resizing CxVector[%d] (%d to %d).",
					  m_size, m_capacity, resize_to);
			resizeToCapacity(resize_to);
		}
		/* Move all the elements after down by in_count */
		for (CxI32 i = m_size - 1; i >= in_idx; --i) {
			mp_vec[i + in_count] = static_cast<T &&>(mp_vec[i]);
		}
		/* insert all the copies of the value */
		for (CxI32 i = in_idx; i < in_idx + in_count; ++i) { mp_vec[i] = in_elem; }
		m_size += in_count;
	}

	template <typename T>
	CxBool CxVector<T>::remove(const T &in_value) {
		CxI32 idx = indexOf(in_value);
		if (idx != -1) { priv_removeAt(idx);  return true; }
		else { return false; }
	}

	template <typename T>
	CxI32 CxVector<T>::removeAll(const T &in_value) {
		CxI32 count = 0;  CxI32 i = 0;
		while (i < m_size) {
			if (mp_vec[i] == in_value) { priv_removeAt(i);  ++count; }
			else { ++i; }
		}
		return count;
	}

	template <typename T>
	CxBool CxVector<T>::removeAt(CxI32 in_idx) {
		if (in_idx >= 0 && in_idx < m_size) {
			priv_removeAt(in_idx);  return true;
		}
		else {
			CXD_WARN("Cannot remove CxVector element [%d], from [0...%d]",
						in_idx, m_size-1);
			return false;			
		}		
	}
	
	template <typename T>
	void CxVector<T>::removeFirst() {
		CXD_IF_WARN(m_size <= 0, "Trying to remove first item from empty CxVector.");
		if (m_size > 0) {
			for (CxI32 i = 0; i < m_size-1; ++i) { mp_vec[i] = static_cast<T &&>(mp_vec[i+1]); }
			mp_vec[--m_size] = m_invalidValue;
		}
	}

	template <typename T>
	CX_FORCE_INLINE void CxVector<T>::reserve(CxI32 in_capacity) {
		/* Only if reserving more than we have */
		if (in_capacity > m_capacity) { resizeToCapacity(in_capacity); }
	}

	template <typename T>
	void CxVector<T>::resize(CxI32 in_size) {
		if (in_size > m_capacity) { resizeToCapacity(in_size); }
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
				CXD_MSG("Auto resizing CxVector[%d] (%d to %d).",
						  m_size, m_capacity, in_idx*2 + 2);
				resizeToCapacity(in_idx*2 + 2);
			}
			m_size = in_idx + 1;
		}
		mp_vec[in_idx] = in_value;
	}

	template <typename T>
	void CxVector<T>::set(CxI32 in_idx, T &&in_value) {
		if (in_idx >= m_size) {
			if (in_idx >= m_capacity) {
				CXD_MSG("Auto resizing CxVector[%d] (%d to %d).",
						  m_size, m_capacity, in_idx*2 + 2);
				resizeToCapacity(in_idx*2 + 2);
			}
			m_size = in_idx + 1;
		}
		mp_vec[in_idx] = in_value;
	}

	template <typename T>
	T CxVector<T>::takeAt(CxI32 in_idx) {
		if (in_idx >= 0 && in_idx < m_size) {
			T ret = static_cast<T &&>(mp_vec[in_idx]);
			priv_removeAt(in_idx);
			return ret;
		}
		else {
			CXD_ERR("Cannot take [%d] from CxVector[0...%d]!", in_idx, m_size-1);
			return m_invalidValue;
		}
	}

	template <typename T>
	void CxVector<T>::resizeToCapacity(CxI32 in_capacity) {
		m_capacity = in_capacity;
		T* newData = new T[in_capacity];
		if (mp_vec) {
			for (CxI32 i = 0; i < m_size; ++i) {
				newData[i] = static_cast<T &&>(mp_vec[i]);
			}
			delete[] mp_vec;
		}			
		mp_vec = newData;
	}


	/**
	 * @class CxPODVector CxPODVector.h "core/common/CxPODVector.h"
	 * @brief A simple resizeable array for plain data types.
	 * The difference between this and the CxVector is that the 
	 * CxPODVector assumes that the types are all plain ol' data types,
	 * (most likely the core types or small structs) and uses that knowledge
	 * to make things faster, and justs malloc and realloc instead of new 
	 * and delete.
	 *
	 * @see CxVector<T> for more info.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since May 28, 2015
	 */
	template<typename T>
	class CxPODVector : public _CxBaseVector<T> {
	  public:

		/** @brief Typdef for the InvasiveStronPtr template for the vector. */
		typedef typename CxInvasiveStrongPtr< CxPODVector<T> > Ptr;
		
		/** @brief Create an empty null vector. */
		CX_FORCE_INLINE CxPODVector() : _CxBaseVector<T>() {}

		CxPODVector(CxI32 in_size);
		CxPODVector(CxI32 in_size, const T &in_value);
		CxPODVector(T *in_array, CxI32 in_size, CxCopy inopt_copy = kCxCopy);
		CxPODVector(const CxPODVector<T> &in_src);
		CxPODVector(const CxPODVector<T> &in_src0, const CxPODVector<T> &in_src1);
		CX_FORCE_INLINE CxPODVector(CxPODVector<T> &&in_src)
			: _CxBaseVector<T>(in_src) {}

		~CxPODVector();
		CxPODVector<T>& operator=(const CxPODVector<T> &in_src);
		CxPODVector<T>& operator=(CxPODVector<T> &&in_src);
		CX_FORCE_INLINE CxBool operator==(const CxPODVector<T> &in_vec) const {
			return _CxBaseVector<T>::operator==(in_vec);
		}
		CX_FORCE_INLINE CxBool operator!=(const CxPODVector<T> &in_vec) const {
			return _CxBaseVector<T>::operator!=(in_vec);
		}

		CX_FORCE_INLINE CxPODVector<T> operator+(const CxPODVector<T> &in_vec) const {
			return CxPODVector<T>(in_src0, in_src1);
		}
		CX_FORCE_INLINE CxPODVector<T> & operator+=(const CxPODVector<T> &in_vec) {
			append(in_vec); return *this;
		}
		CX_FORCE_INLINE CxPODVector<T> & operator+=(const T &in_value) {
			append(in_value); return *this;
		}
		CX_FORCE_INLINE CxPODVector<T> & operator<<(const CxPODVector<T> &in_vec) {
			append(in_vec); return *this;
		}
		CX_FORCE_INLINE CxPODVector<T> & operator<<(const T &in_value) {
			append(in_value); return *this;
		}
		void append(const T& in_value);
		void append(const T *in_src, CxI32 in_size);
		void append(const CxPODVector<T>& in_src);
		void compact();
		void free();
		void fill(const T &in_value, CxI32 inopt_size = -1);
		void insert(CxI32 in_idx, const T &in_elem);
		void insert(CxI32 in_idx, const T &in_elem, CxI32 in_count);
		CX_FORCE_INLINE void prepend(const T &in_value) { insert(0, in_value); }
		CxBool remove(const T &in_value);
		CxI32 removeAll(const T &in_value);
		CxBool removeAt(CxI32 in_idx);
		void removeFirst();
		void reserve(CxI32 in_capacity);
		void resize(CxI32 in_size);
		void set(CxI32 in_idx, const T &in_value);
		T takeAt(CxI32 in_idx);
		CX_FORCE_INLINE T takeFirst() { return takeAt(0); }

	  protected:
		CX_FORCE_INLINE void priv_removeAt(CxI32 in_idx) {
			/* Shift everything down by one */
			mem::move(mp_vec + in_idx, mp_vec + in_idx + 1, sizeof(T)*(m_size - in_idx - 1));
			--m_size;
		}

		void resizeToCapacity(CxI32 capacity);
	};
	
	template <typename T>
	CX_FORCE_INLINE CxPODVector<T>::CxPODVector(CxI32 in_size)
		: _CxBaseVector<T>() { resize(in_size); }

	template <typename T>
	CX_FORCE_INLINE CxPODVector<T>::CxPODVector(CxI32 in_size, const T &in_value)
		: _CxBaseVector<T>() { resize(in_size);  fill(in_value); }

	template <typename T>
	CxPODVector<T>::CxPODVector(T *in_array, CxI32 in_size, CxCopy inopt_copy)
		: _CxBaseVector<T>() {
		m_capacity = in_size;  m_size = in_size;
		if (inopt_copy == kCxNoCopy) { mp_vec = in_array; }
		else if (in_size > 0) {
			mp_vec = (T *)mem::alloc(sizeof(T)*in_size);
			mem::copy(mp_vec, in_array, sizeof(T)*in_size);
		}
	}
	
	template <typename T>
	CxPODVector<T>::CxPODVector(const CxPODVector<T> &in_src)
		: _CxBaseVector<T>() { *this = in_src; }

	template <typename T>
	CxPODVector<T>::CxPODVector(const CxPODVector<T> &in_src0, const CxPODVector<T> &in_src1)
		: _CxBaseVector<T> () {
		CxI32 ret_size = in_src0.m_size + in_src1.m_size;
		if (ret_size > 0) {
			mp_vec = (T*)mem::alloc(sizeof(T)*ret_size);
			/* Copy from this vector */
			mem::copy(mp_vec, in_src0.mp_vec, sizeof(T)*in_src0.m_size);
			/* Copy from other vector */
			mem::copy(mp_vec + in_src0.m_size, in_src1.mp_vec, sizeof(T)*in_src1.m_size);
		}
		m_size = ret_size;
		m_capacity = ret_size;
	}
	
	template <typename T>
	CxPODVector<T>::~CxPODVector() { free(); }

	template <typename T>
	CxPODVector<T> & CxPODVector<T>::operator=(const CxPODVector<T> &in_src) {
		CxI32 capacity = in_src.m_capacity;
		m_capacity = capacity;  m_size = in_src.m_size;

		T * old_vec = mp_vec;
		if (capacity > 0) {
			T* new_vec = (T*) mem::alloc(sizeof(T)*capacity);
			mem::copy(new_vec, in_src.mp_vec, sizeof(T)*m_size);
			mp_vec = new_vec;
		}
		else { mp_vec = 0; }
		
		mem::free(old_vec);
		return *this;
	}

	template <typename T>
	CX_FORCE_INLINE CxPODVector<T> & CxPODVector<T>::operator=(CxPODVector<T> &&in_src) {
		mem::free(mp_vec);
		mp_vec = in_src.mp_vec;
		m_capacity = in_src.m_capacity;
		m_size = in_src.m_size;
		in_src.mp_vec = 0;
		return *this;
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODVector<T>::append(const T &in_elem) {
		if (m_size == m_capacity) {
			CXD_MSG("Auto resizing CxPODVector[%d] (%d to %d).",
					  m_size, m_capacity, m_capacity*2 + 2);
			resizeToCapacity(m_capacity*2 + 2);
		}
		mp_vec[m_size++] = in_elem;
	}

	template <typename T>
	void CxPODVector<T>::append(const T *in_src, CxI32 in_size) {
		const CxI32 capacity = m_size + in_size;
		if (m_capacity < capacity) { resizeToCapacity(capacity); }

		/* Copy all the values */
		mem::copy(mp_vec + m_size, in_src, sizeof(T)*in_size);
		m_size += in_size;
	}

	template <typename T>
	void CxPODVector<T>::append(const CxPODVector &in_vec) {
		const CxI32 v_size = in_vec.m_size;
		const CxI32 capacity = m_size + v_size;
		if (m_capacity < capacity) { resizeToCapacity(capacity); }

		/* Copy all the values */
		mem::copy(mp_vec + m_size, in_vec.mp_vec, sizeof(T)*v_size);
		m_size += v_size;
	}

	template <typename T>
	void CxPODVector<T>::compact() {
		if (m_size < m_capacity) {
			mp_vec = (T *)mem::resize(mp_vec, sizeof(T)*m_size);
			m_capacity = m_size;
		}
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODVector<T>::free() {
		mem::free(mp_vec);  m_capacity = m_size = 0;
	}

	template <typename T>
   void CxPODVector<T>::fill(const T &in_value, CxI32 inopt_size) {
		if (inopt_size == -1) { inopt_size = m_size; }
		resize(inopt_size);
		for (CxI32 i = 0; i < inopt_size; ++i) { mp_vec[i] = in_value; }
	}
	
	template <typename T>
	void CxPODVector<T>::insert(CxI32 in_idx, const T &in_elem) {
		if (m_size == m_capacity) {
			CXD_MSG("Auto resizing CxPODVector[%d] (%d to %d).",
					  m_size, m_capacity, m_capacity*2 + 2);
			resizeToCapacity(m_capacity*2 + 2);
		}

		/* Restrict the index value */
		if (in_idx > m_size) { in_idx = m_size; }
		else if (in_idx < 0) { in_idx = 0; }
		
		/* Move all the elements after up one */
		T *ptr = mp_vec + in_idx;
		mem::move(ptr + 1, ptr, sizeof(T)*(m_size - in_idx));
		mp_vec[in_idx] = in_elem;
		++m_size;
	}

	template <typename T>
	void CxPODVector<T>::insert(CxI32 in_idx, const T &in_elem, CxI32 in_count) {
		const CxI32 n_size = m_size + in_count;
		if (n_size > m_capacity) {
			const CxI32 cap_2 = m_capacity*2 + 2;
			const CxI32 resize_to = (n_size > cap_2) ? n_size : cap_2;
		   CXD_MSG("Auto resizing CxPODVector[%d] (%d to %d).",
					  m_size, m_capacity, resize_to);
			resizeToCapacity(resize_to);
		}
		
		T *ptr = mp_vec + in_idx;
		mem::move(ptr + in_count, ptr, sizeof(T)*(m_size - in_idx));
		
		/* insert all the copies of the value */
		for (CxI32 i = 0; i < in_count; ++i) { ptr[i] = in_elem; }
		m_size += in_count;
	}

	template <typename T>
	CxBool CxPODVector<T>::remove(const T &in_value) {
		CxI32 idx = indexOf(in_value);
		if (idx != -1) { priv_removeAt(idx);  return true; }
		else { return false; }
	}

	template <typename T>
	CxI32 CxPODVector<T>::removeAll(const T &in_value) {
		CxI32 count = 0;  CxI32 i = 0;
		while (i < m_size) {
			if (mp_vec[i] == in_value) { priv_removeAt(i);  ++count; }
			else { ++i; }
		}
		return count;
	}

	template <typename T>
	CxBool CxPODVector<T>::removeAt(CxI32 in_idx) {
		if (in_idx >= 0 && in_idx < m_size) {
			priv_removeAt(in_idx);  return true;
		}
		else {
			CXD_WARN("Cannot remove CxPODVector element [%d], from [0...%d]",
						in_idx, m_size-1);
			return false;			
		}		
	}
	
	template <typename T>
	CX_FORCE_INLINE void CxPODVector<T>::removeFirst() {
		CXD_IF_WARN(m_size <= 0, "Trying to remove first item from empty CxPODVector.");
		if (m_size > 0) {
			mem::move(mp_vec, mp_vec + 1, sizeof(T)*(m_size - 1));
			--m_size;
		}
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODVector<T>::reserve(CxI32 in_capacity) {
		/* Only if reserving more than we have */
		if (in_capacity > m_capacity) { resizeToCapacity(in_capacity); }
	}

	template <typename T>
	void CxPODVector<T>::resize(CxI32 in_size) {
		if (in_size > m_capacity) { resizeToCapacity(in_size); }
	   m_size = in_size;
	}

	template <typename T>
	void CxPODVector<T>::set(CxI32 in_idx, const T &in_value) {
		if (in_idx >= m_size) {
			if (in_idx >= m_capacity) {
				CXD_MSG("Auto resizing CxPODVector[%d] (%d to %d).",
						  m_size, m_capacity, in_idx*2 + 2);
				resizeToCapacity(in_idx*2 + 2);
			}
			m_size = in_idx + 1;
		}
		mp_vec[in_idx] = in_value;
	}

	template <typename T>
	T CxPODVector<T>::takeAt(CxI32 in_idx) {
		if (in_idx >= 0 && in_idx < m_size) {
			T ret = mp_vec[in_idx];
			priv_removeAt(in_idx);
			return ret;
		}
		else {
			CXD_ERR("Cannot take [%d] from CxPODVector[0...%d]!",
					  in_idx, m_size-1);
			return m_invalidValue;
		}
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODVector<T>::resizeToCapacity(CxI32 in_capacity) {
		m_capacity = in_capacity;
		mp_vec = (T* )mem::alloc(mp_vec, sizeof(T)*in_capacity);
	}



	
	/** @brief Pointer type specialization */
	template<typename T>
	class CxVector<T *> : public CxPODVector<T *> {
	  public:
		typedef typename CxInvasiveStrongPtr< CxPODVector<T *> > Ptr;	
		CX_FORCE_INLINE CxVector() : CxPODVector<T *>() {}				
		CX_FORCE_INLINE CxVector(CxI32 in_size) : CxPODVector<T *>(in_size) {}	
		CX_FORCE_INLINE CxVector(CxI32 in_size, const T &in_value)			
			: CxPODVector<T *>(in_size, in_value) {}							
		CX_FORCE_INLINE CxVector(T * *in_array, CxI32 in_size, CxCopy inopt_copy = kCxCopy) 
			: CxPODVector<T *>(in_array, in_size, inopt_copy) {}			
		CX_FORCE_INLINE CxVector(const CxVector<T *> &in_src)			
			: CxPODVector<T *>(in_src) {}
		CX_FORCE_INLINE CxVector(const CxVector<T *> &in_src0, const CxVector<T *> &in_src1)
			: CxPODVector<T *>(in_src0, in_src1) {}			
		CX_FORCE_INLINE CxVector(CxVector<T *> &&in_src)					
			: CxPODVector<T *>(in_src) {}
		CxVector<T *> & operator=(const CxVector<T *> &in_src) {
				 CxPODVector<T *>::operator=(in_src);
				 return *this;
		}
		CxVector<T *> & operator=(CxVector<T *> &&in_src) {
			 CxPODVector<T *>::operator=(in_src);
			 return *this;
		}
		CX_FORCE_INLINE CxBool operator==(const CxVector<T *> &in_vec) const {	
			return _CxBaseVector<T *>::operator==(in_vec);					
		}																						
		CX_FORCE_INLINE CxBool operator!=(const CxPODVector<T *> &in_vec) const {		
			return _CxBaseVector<T *>::operator!=(in_vec);					
		}	
		CX_FORCE_INLINE CxVector<T *> operator+(const CxVector<T *> &in_vec) const { 
			return CxVector<T *>(*this, in_vec);								
		}																						
		CX_FORCE_INLINE CxVector<T *> & operator+=(const CxVector<T *> &in_vec) { 
			append(in_vec);  return *this;												
		}																						
		CX_FORCE_INLINE CxVector<T *> & operator+=(T *const in_vec) { 
			append(in_vec);  return *this;												
		}																						
		CX_FORCE_INLINE CxVector<T *> & operator<<(const CxVector<T *> &in_vec) { 
			append(in_vec);  return *this;												
		}																						
		CX_FORCE_INLINE CxVector<T *> & operator<<(T *const in_vec) { 
			append(in_vec);  return *this;												
		}		
		/**
		 * @brief Deletes pointers and deallocate all the memory for the vector.
		 * This method deallocates the memory for the vector and resets the 
		 * size and capacity to 0.  It will call delete for each non-null element
		 * (pointer) in the vector.
		 */
		void freePtr();

		/**
		 * @brief Remove all elements from an array and delete them. 
		 * This method assumes that the vector contains dynamically allocated 
		 * pointers, and as such, calls delete on all the elements.
		 */
		void eraseAll();

		/**
		 * @brief Remove and delete the element at the specified index.
		 * This method assumes that the vector contains dynamically allocated 
		 * pointers, and as such, calls delete on all the elements.
		 * @param in_idx The index of the element to erase.
		 */
		void eraseAt(CxI32 in_idx);

		/** 
		 * @brief Removes the first element and deletes it.
		 * This method assumes that the vector is storing points to 
		 * dynamically allocated memory using the 'new' operator.
		 */
		void eraseFirst();

	   /** 
		 * @brief Removes the last element and deletes it.
		 * This method assumes that the vector is storing points to 
		 * dynamically allocated memory using the 'new' operator.
		 */
		void eraseLast();
	};
	
	template <typename T>
	CX_FORCE_INLINE void CxVector<T *>::freePtr() { eraseAll();  free(); }

		
	template <typename T>
	void CxVector<T *>::eraseAll() {
		for (CxI32 i = 0; i < m_size; ++i) {
			if (mp_vec[i]) { delete mp_vec[i]; mp_vec[i] = 0; }
		}
		m_size = 0;
	}

	template <typename T>
	void CxVector<T *>::eraseAt(CxI32 in_idx) {
		if (in_idx == 0) { eraseFirst(); }
		else if (in_idx == m_size - 1) { eraseLast(); }
		
		else if (in_idx > 0 && in_idx < m_size) {
			if (mp_vec[in_idx] != 0) { delete mp_vec[in_idx]; }
			const CxI32 len = m_size - (in_idx + 1);
			mem::move(mp_vec + in_idx, mp_vec + in_idx + 1, sizeof(T)*len);
			mp_vec[--m_size] = 0;
		}
		else {
			CXD_WARN("Trying to erase [%d] from CxVector[0...%d]",
						in_idx, m_size -1);
		}
	}

	template <typename T>
	void CxVector<T *>::eraseFirst() {
		CXD_IF_WARN(m_size <= 0, "Trying to erase first item from empty CxVector!");
		if (m_size > 0) {
			if (mp_vec[0] != 0) { delete mp_vec[0]; }
			if (--m_size != 0) {
				mem::move(mp_vec, mp_vec + 1, sizeof(T)*m_size);
			}
			mp_vec[m_size] = 0;
		}
	}

	template <typename T>
	void CxVector<T *>::eraseLast() {
		CXD_IF_WARN(m_size <= 0, "Trying to erase last item from empty CxVector!");
		if (m_size > 0) {
			if (mp_vec[m_size-1] != 0) { delete mp_vec[m_size-1]; }
			mp_vec[--m_size] = 0;
		}
	}

#define CX_PODVEC_SPEC(ARG_TYPE)														\
	template <> class CxVector<ARG_TYPE> : public CxPODVector<ARG_TYPE> { \
	  public:																				\
	  typedef CxInvasiveStrongPtr< CxPODVector<ARG_TYPE> > Ptr;				\
	  CX_FORCE_INLINE CxVector() : CxPODVector<ARG_TYPE>() {}				\
	  CX_FORCE_INLINE CxVector(CxI32 in_size) : CxPODVector<ARG_TYPE>(in_size) {} \
	  CX_FORCE_INLINE CxVector(CxI32 in_size, const ARG_TYPE &in_value)	\
		  : CxPODVector<ARG_TYPE>(in_size, in_value) {}							\
	  CX_FORCE_INLINE CxVector(ARG_TYPE *in_array, CxI32 in_size, CxCopy inopt_copy = kCxCopy) \
		  : CxPODVector<ARG_TYPE>(in_array, in_size, inopt_copy) {}			\
	  CX_FORCE_INLINE CxVector(const CxVector<ARG_TYPE> &in_src)			\
		  : CxPODVector<ARG_TYPE>(in_src) {}										\
	  CX_FORCE_INLINE CxVector(const CxVector<ARG_TYPE> &in_src0, const CxVector<ARG_TYPE> &in_src1) \
		  : CxPODVector<ARG_TYPE>(in_src0, in_src1) {}							\
	  CX_FORCE_INLINE CxVector(CxVector<ARG_TYPE> &&in_src)					\
		  : CxPODVector<ARG_TYPE>(in_src) {}										\
		CxVector<ARG_TYPE> & operator=(const CxVector<ARG_TYPE> &in_src) { \
		 CxPODVector<ARG_TYPE>::operator=(in_src); \
		return *this; \
	} \
	CxVector<ARG_TYPE> & operator=(CxVector<ARG_TYPE> &&in_src) { \
		CxPODVector<ARG_TYPE>::operator=(in_src); \
		return *this; \
	} \
	  CX_FORCE_INLINE CxBool operator==(const CxVector<ARG_TYPE> &in_vec) const {	\
		  return _CxBaseVector<ARG_TYPE>::operator==(in_vec);					\
	  }																						\
	  CX_FORCE_INLINE CxBool operator!=(const CxPODVector<ARG_TYPE> &in_vec) const {		\
		  return _CxBaseVector<ARG_TYPE>::operator!=(in_vec);					\
	  }																						\
	  CX_FORCE_INLINE CxVector<ARG_TYPE> operator+(const CxVector<ARG_TYPE> &in_vec) const { \
		  return CxVector<ARG_TYPE>(*this, in_vec);								\
	  }																						\
	  CX_FORCE_INLINE CxVector<ARG_TYPE> & operator+=(const CxVector<ARG_TYPE> &in_vec) { \
		  append(in_vec);  return *this;												\
	  }																						\
	  CX_FORCE_INLINE CxVector<ARG_TYPE> & operator+=(const ARG_TYPE &in_vec) { \
		  append(in_vec);  return *this;												\
	  }																						\
	  CX_FORCE_INLINE CxVector<ARG_TYPE> & operator<<(const CxVector<ARG_TYPE> &in_vec) { \
		  append(in_vec);  return *this;												\
	  }																						\
	  CX_FORCE_INLINE CxVector<ARG_TYPE> & operator<<(const ARG_TYPE &in_vec) { \
		  append(in_vec);  return *this;												\
	  }																						\
	}

	CX_PODVEC_SPEC(CxBool);
	CX_PODVEC_SPEC(CxI8);
	CX_PODVEC_SPEC(CxU8);
	CX_PODVEC_SPEC(CxChar16);
	CX_PODVEC_SPEC(CxI16);
	CX_PODVEC_SPEC(CxU16);
	CX_PODVEC_SPEC(CxF32);
	CX_PODVEC_SPEC(CxI32);
	CX_PODVEC_SPEC(CxU32);
	CX_PODVEC_SPEC(CxF64);
	CX_PODVEC_SPEC(CxI64);
	CX_PODVEC_SPEC(CxU64);

} // namespace cat

#endif // CX_CORE_COMMON_CXVECTOR_H
