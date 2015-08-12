#ifndef CX_CORE_COMMON_CXPODVECTOR_H
#define CX_CORE_COMMON_CXPODVECTOR_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxPODVector.h
 * @brief Contains a simple Vector (resizeable array) for plain data types.
 *
 * @author Catlin Zilinski
 * @date Aug 12, 2015
 */

#include "core/common/CxVector.h"

namespace cat {

	/**
	 * @class CxPODVector CxPODVector.h "core/common/CxPODVector.h"
	 * @brief A simple resizeable array for plain data types.
	 * The difference between this and the CxVector is that the 
	 * CxPODVector assumes that the types are all plain ol' data types,
	 * (most likely the core types or small structs) and uses that knowledge
	 * to make things faster, and justs malloc and realloc instead of new 
	 * and delete.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since May 28, 2015
	 */
	template<typename T>
	class CxPODVector : public CxVector<T> {
	  public:

		/** @brief Typdef for the InvasiveStronPtr template for the vector. */
		typedef typename CxInvasiveStrongPtr< CxPODVector<T> > Ptr;
		
		/** @brief Create an empty null vector. */
		CX_FORCE_INLINE CxPODVector();

		/**
		 * @brief Create a new vector with the specified size.
		 * The values are initialise with the default values.
		 * @param in_size The initial size of the vector.
		 */
		CX_FORCE_INLINE CxPODVector(CxI32 in_size);

		/**
		 * @brief Create a new vector with the specified size.
		 * The values are initialised with the specified fill value.
		 * @param in_size The initialise size of the vector.
		 * @param in_value The value to fill the vector with.
		 */
		CX_FORCE_INLINE CxPODVector(CxI32 in_size, const T &in_value);

		/**
		 * @brief Create a new vector from the specified array.
		 * @param in_array The c-array of elements.
		 * @param in_size The number of elements in the array.
		 * @param inopt_copy If kCxNoCopy, then array is not copied, just stored.
		 */
		CxPODVector(T *in_array, CxI32 in_size, CxCopy inopt_copy = kCxCopy);

		/**
		 * @brief Copy constructor, copies each element.
		 * @param in_src The source vector to create a copy of.
		 */
		CxPODVector(const CxPODVector<T> &in_src);

		/** @brief Deletes the contents of the Vector. */
		~CxPODVector();

		/**
		 * @brief Overloaded Assignment operator, replaces contents with new contents.
		 * @param in_src The CxPODVector to copy.
		 * @return A reference to this Vector.
		 */
		CxPODVector<T>& operator=(const CxPODVector<T> &in_src);

		/**
		 * @brief Operator to test to vectors to equality.
		 * @param in_vec The other vector to test for equality to.
		 * @return True if the two vectors are equal.
		 */
		CxBool operator==(const CxPODVector<T> &in_vec) const;

		/**
		 * @brief Operator to test two vectors for non-equality.
		 * @param in_vec The other vector to test against.
		 * @return True if the two vectors are NOT equal.
		 */
		CxBool operator!=(const CxPODVector<T> &in_vec) const;

		/**
		 * @brief Concatenate two vectors and return the resulting new vector.
		 * @param in_vec The vector to append to this one.
		 * @return A new vector that is the result of in_vec appended to this vector.
		 */
		CxPODVector<T> operator+(const CxPODVector<T> &in_vec) const;

		/**
		 * @brief Append another vector onto this vector.
		 * @see append()
		 * @see operator<<()
		 * @param in_vec The vector to append to this one.
		 * @return A reference to this vector.
		 */
		CX_FORCE_INLINE CxPODVector<T> & operator+=(const CxPODVector<T> &in_vec) {
			append(in_vec); return *this;
		}

		/**
		 * @brief Append a value onto this vector.
		 * @see append()
		 * @see operator<<()
		 * @param in_value The value to append to the vector.
		 * @return A reference to this vector.
		 */
		CX_FORCE_INLINE CxPODVector<T> & operator+=(const T &in_value) {
			append(in_value); return *this;
		}
		
		/**
		 * @brief Append another vector onto this vector.
		 * @see append()
		 * @see operator+=()
		 * @param in_vec The vector to append to this one.
		 * @return A reference to this vector.
		 */
		CX_FORCE_INLINE CxPODVector<T> & operator<<(const CxPODVector<T> &in_vec) {
			append(in_vec); return *this;
		}

		/**
		 * @brief Append a value onto this vector.
		 * @see append()
		 * @see operator+=()
		 * @param in_value The value to append to the vector.
		 * @return A reference to this vector.
		 */
		CX_FORCE_INLINE CxPODVector<T> & operator<<(const T &in_value) {
			append(in_value); return *this;
		}
		
		/**
		 * @brief Append an element onto the end of the vector.
		 * @param in_value The value to append to the vector.
		 */
		CX_FORCE_INLINE void append(const T& in_value);

		/**
		 * @brief Append all the elements from an array onto this vector.
		 * @param in_src The array to append the elements from.
		 */
		void append(const T *in_src, CxI32 in_size);

		/**
		 * @brief Append all the elements from another vector.
		 * @param in_src The other vector to append the elements from.
		 */
		void append(const CxPODVector<T>& in_src);

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
		CX_FORCE_INLINE void dealloc();

		/**
		 * @brief Deletes pointers and deallocate all the memory for the vector.
		 * This method deallocates the memory for the vector and resets the 
		 * size and capacity to 0.  It will call delete for each non-null element
		 * (pointer) in the vector.
		 */
		CX_FORCE_INLINE void deallocPtr();

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
		CX_FORCE_INLINE void removeFirst();

		/**
		 * @brief Reserve the specified capacity in the vector.
		 * @param in_capacity The amount of elements to reserve for the vector.
		 */
		CX_FORCE_INLINE void reserve(CxI32 in_capacity);

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
			mem::move(mp_vec + in_idx, mp_vec + in_idx + 1, sizeof(T)*(m_size - in_idx - 1));
			--m_size;
		}

		CX_FORCE_INLINE void resizeToCapacity(CxI32 capacity);
	};

	template <typename T>
	CX_FORCE_INLINE CxPODVector<T>::CxPODVector()
		: CxVector<T>() {}

	template <typename T>
	CX_FORCE_INLINE CxPODVector<T>::CxPODVector(CxI32 in_size)
		: CxVector<T>() {
		resize(in_size);
	}

	template <typename T>
	CX_FORCE_INLINE CxPODVector<T>::CxPODVector(CxI32 in_size, const T &in_value)
		: CxVector<T>() {
		resize(in_size);
		fill(in_value);
	}

	template <typename T>
	CxPODVector<T>::CxPODVector(T *in_array, CxI32 in_size, CxCopy inopt_copy)
		: CxVector<T>() {
		m_capacity = in_size;  m_size = in_size;
		
		if (inopt_copy == kCxNoCopy) {
			mp_vec = in_array;
		}
		else if (in_size > 0) {
			mp_vec = (T *)mem::alloc(sizeof(T)*in_size);
			mem::copy(mp_vec, in_array, sizeof(T)*in_size);
		}
	}
	template <typename T>
	CxPODVector<T>::CxPODVector(const CxPODVector<T> &in_src)
		: CxVector<T>() {
		*this = in_src;
	}

	template <typename T>
	CxPODVector<T>::~CxPODVector() { dealloc(); }

	template <typename T>
	CxPODVector<T> & CxPODVector<T>::operator=(const CxPODVector<T> &in_src) {
		CxI32 capacity = in_src.m_capacity;
		m_capacity = capacity;
		m_size = in_src.m_size;

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
	CxBool CxPODVector<T>::operator==(const CxPODVector<T> &in_vec) const {
		if (m_size == in_vec.m_size) {
			for (CxI32 i = 0; i < m_size; ++i) {
				if (mp_vec[i] != in_vec.mp_vec[i]) { return false; }
			}
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CxBool CxPODVector<T>::operator!=(const CxPODVector<T> &in_vec) const {
		if (m_size == in_vec.m_size) {
			for (CxI32 i = 0; i < m_size; ++i) {
				if (mp_vec[i] != in_vec.mp_vec[i]) { return true; }
			}
			return false;
		}
		else { return true; }
	}

	template <typename T>
	CxPODVector<T> CxPODVector<T>::operator+(const CxPODVector<T> &in_vec) const {
		CxI32 ret_size = m_size + in_vec.m_size;
		if (ret_size > 0) {
			T *ret = (T*)mem::alloc(sizeof(T)*ret_size);
			/* Copy from this vector */
			mem::copy(ret, mp_vec, sizeof(T)*m_size);
			/* Copy from other vector */
			mem::copy(ret + m_size, in_vec.mp_vec, sizeof(T)*in_vec.m_size);
			/* Return the new vector */
			return CxPODVector<T>(ret, ret_size, kCxNoCopy);
		}
		else {
			return CxPODVector<T>();
		}
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
	CX_FORCE_INLINE void CxPODVector<T>::dealloc() {
		mem::free(mp_vec);
		m_capacity = m_size = 0;
	}

	template <typename T>
	CX_FORCE_INLINE void CxPODVector<T>::deallocPtr() {
		eraseAll();
	   dealloc();
	}

	template <typename T>
   void CxPODVector<T>::fill(const T &in_value, CxI32 inopt_size) {
		if (inopt_size == -1) {
			inopt_size = m_size;
		}
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
		if (idx != -1) {
			priv_removeAt(idx);
			return true;
		}
		else { return false; }
	}

	template <typename T>
	CxI32 CxPODVector<T>::removeAll(const T &in_value) {
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
	CxBool CxPODVector<T>::removeAt(CxI32 in_idx) {
		if (in_idx >= 0 && in_idx < m_size) {
			priv_removeAt(in_idx);
			return true;
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
		if (in_capacity > m_capacity) { 
			resizeToCapacity(in_capacity);
		}
	}

	template <typename T>
	void CxPODVector<T>::resize(CxI32 in_size) {
		if (in_size > m_capacity) {
			resizeToCapacity(in_size);
		}
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

} // namespace cat

#endif // CX_CORE_COMMON_CXPODVECTOR_H
