#ifndef CX_CORE_COMMON_CXMALLOCREF_H
#define CX_CORE_COMMON_CXMALLOCREF_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxMallocRef.h
 * @brief Contains reference counted pointer allocated with malloc family.
 *
 * @author Catlin Zilinski
 * @date Oct 15, 2015
 */

#include "core/common/CxMem.h"
#include "core/common/CxAtomic.h"

namespace cat {

	/**
	 * @class CxMallocRef CxMallocRef.h "core/common/CxMallocRef.h"
	 * @brief A reference counted pointer to a C array.
	 * 
	 * This class simply contains a pointer to an array that was
	 * allocated using the malloc family of functions, and will be deallocated 
	 * (once the reference count reaches zero) with free().
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 15, 2015
	 */
	template <typename T>
	class CxMallocRef {
	  public:
		/** @brief Create a null pointer */
		CX_FORCE_INLINE CxMallocRef() : mp_ptr(0), mp_refCount(0) {}

		/** @brief Create a pointer from an existing pointer. */
		CX_FORCE_INLINE CxMallocRef(T *in_ptr)
			: mp_ptr(in_ptr), mp_refCount(0) {
			if (in_ptr != 0) {
				mp_refCount = (CxI32 *)mem::alloc(sizeof(CxI32));
				*mp_refCount = 1;
			}
		}

		/** @brief Copy constructor, increments reference count. */
		CX_FORCE_INLINE CxMallocRef(const CxMallocRef<T> &in_src)
			: mp_ptr(in_src.mp_ptr), mp_refCount(in_src.mp_refCount) { retain(); }

		/** @brief Move constructor, steals from source */
		CX_FORCE_INLINE CxMallocRef(CxMallocRef<T> &&in_src)
			: mp_ptr(in_src.mp_ptr), mp_refCount(in_src.mp_refCount) {
			in_src.mp_ptr = 0;
			in_src.mp_refCount = 0;
		}

		/** @brief Assignment op, increments reference count */
		CX_FORCE_INLINE CxMallocRef<T> & operator=(const CxMallocRef<T> &in_src) {
			if (*this != in_src) {
				release();
				mp_ptr = in_src.mp_ptr;
				mp_refCount = in_src.mp_refCount;
				retain();
			}
			return *this;
		}

		/** @brief Move-assign op, steals from source */
		CX_FORCE_INLINE CxMallocRef<T> & operator=(CxMallocRef<T> &&in_src) {
			if (*this != in_src) {
				release();
				mp_ptr = in_src.mp_ptr;  mp_refCount = in_src.mp_refCount;
				in_src.mp_ptr = 0;  in_src.mp_refCount = 0;
			}
			return *this;
		}

		/** @return True if the two pointers are equal */
		CX_FORCE_INLINE CxBool operator==(const CxMallocRef<T> &in_ref) const {
			return mp_ptr == in_ref.mp_ptr;
		}

		/** @return True if the two pointers are NOT equal */
		CX_FORCE_INLINE CxBool operator!=(const CxMallocRef<T> &in_ref) const {
			return mp_ptr != in_ref.mp_ptr;
		}

		/** 
		 * This overloaded operator is mainly used so we can still do
		 * 'ptr != 0' or 'ptr == 0' as a null pointer test.
		 * @return True if the pointer address is equal to the passed in value 
		 **/
		CX_FORCE_INLINE CxBool operator==(const T *in_src) const {
			return mp_ptr == in_src;
		}

		/** 
		 * This overloaded operator is mainly used so we can still do
		 * 'ptr != 0' or 'ptr == 0' as a null pointer test.
		 * @return True if the pointer address is equal to the passed in value 
		 **/
		CX_FORCE_INLINE CxBool operator!=(const T *in_src) const {
			return mp_ptr != in_src;
		}
		
		/** @brief Decrements reference count and deletes if count <= 0 */
		CX_FORCE_INLINE ~CxMallocRef() { release(); }

		/** @return Reference to array item at the specified index. */
		CX_FORCE_INLINE T & operator[](CxI32 in_idx) {
			CXD_IF_ERR(!mp_ptr, "Calling CxMallocRef[] on null pointer!");
			return mp_ptr[in_idx];
		}

		/** @return Const reference to array item at the specified index. */
		CX_FORCE_INLINE const T & operator[](CxI32 in_idx) const {
			CXD_IF_ERR(!mp_ptr, "Calling CxMallocRef[] on null pointer!");
			return mp_ptr[in_idx];
		}

		/** @return The dereferenced T value that is pointed to. */
		CX_FORCE_INLINE T & operator*() {
			CXD_IF_ERR(!mp_ptr, "Trying to dereference null CxMallocRef!");
			return *mp_ptr;
		}
		
		CX_FORCE_INLINE const T & operator*() const {
			CXD_IF_ERR(!mp_ptr, "Trying to dereference null CxMallocRef!");
			return *mp_ptr;
		}

		/** @return The pointer stored in the reference */
		CX_FORCE_INLINE T * operator->() {
			CXD_IF_ERR(!mp_ptr, "Calling CxMallocRef->() on null pointer!");
			return mp_ptr;
		}

		CX_FORCE_INLINE const T * operator->() const {
			CXD_IF_ERR(!mp_ptr, "Calling CxMallocRef->() on null pointer!");
			return mp_ptr;
		}

		/** @return The pointer stored in the reference. */
		CX_FORCE_INLINE T * ptr() { return mp_ptr; }
		CX_FORCE_INLINE const T * ptr() const { return mp_ptr; }
		
		/** @return The number of references to the pointer */
		CX_FORCE_INLINE CxI32 refCount() const {
			return (mp_refCount != 0) ? *mp_refCount : 0;
		}

		/** @brief Decrement retain count by one and free if <= 0. */
		CX_INLINE void release() {
			if (mp_refCount != 0) {
				if (atomic::decr32(mp_refCount) == 0) {
					free(mp_refCount);  free(mp_ptr);
					mp_refCount = 0;  mp_ptr = 0;
#if defined(CX_TESTING)
					_testNumDeletes++;
#endif // CX_TESTING
				}
			}
		}

		/** @brief Increment the reference count by one */
		CX_FORCE_INLINE void retain() {
			CXD_IF_WARN(mp_refCount == 0, "Cannot retain null pointer reference.");
			if (mp_refCount != 0) { atomic::incr32(mp_refCount); }
		}

#if defined(CX_TESTING)
		void _testingSetNullNoDeletePtr() {
			if (mp_refCount != 0) {
				if (atomic::decr32(mp_refCount) == 0) {
					free(mp_refCount);  mp_refCount = 0;
					mp_ptr = 0;
				}
			}
		}

		static CxI32 _testNumDeletes;
#endif // CX_TESTING
		
	  private:
		T *mp_ptr;
		CxI32 *mp_refCount;
	};

#if defined(CX_TESTING)
	template<typename T> CxI32 CxMallocRef<T>::_testNumDeletes = 0;
#endif // CX_TESTING
	
} // namespace cat


#endif // CX_CORE_COMMON_CXMALLOCREF_H
