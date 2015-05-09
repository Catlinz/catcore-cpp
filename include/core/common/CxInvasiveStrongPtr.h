#ifndef CX_CORE_COMMON_CXINVASIVESTRONGPTR_H
#define CX_CORE_COMMON_CXINVASIVESTRONGPTR_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxInvasiveStrongPtr.h
 * @brief Contains a reference counting pointer wrapper.
 *
 * The CxInvasiveStrongPtr class is a strong reference counting safe pointer which assumes 
 * certain methods on the pointer it is storing to increment and decrement the 
 * reference count, which is stored on the actual object.
 *
 * @author Catlin Zilinski
 * @date Apr 27, 2014
 */

#include "core/Cx.h"

/**
 * Macros for easily making a class be an invasive strong pointer.
 * The CX_ISTPR_METHODS goes in the public section of the class,
 * the CX_ISTPR_FIELDS goes in the private / protected section of the class, 
 * the CX_ISPTR_INIT goes in the constructor of the class, and 
 * the CX_ISPTR_PTR goes after the class definition if wanted.
 */
#define CX_ISPTR_METHODS																\
	inline CxBool release() { return m_retainCount.decrement() <= 0; }	\
	inline void retain() { m_retainCount.increment(); }						\
	inline CxI32 retainCount() const { return m_retainCount.val(); }

#define CX_ISPTR_FIELDS									\
	CxAtomicI32 m_retainCount

#define CX_ISPTR_INIT									\
	m_retainCount = CxAtomicI32()

#define CX_ISPTR_PTR(isptrClassName)											\
	typedef CxInvasiveStrongPtr<isptrClassName> isptrClassName ## Ptr

namespace cat {

	/**
	 * @class CxInvasiveStrongPtr CxInvasiveStrongPtr.h "core/common/CxInvasiveStrongPtr.h"
	 * @brief A strong reference counting safe pointer.
	 * 
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Mar 4, 2014
	 */
	template<typename T>
	class CxInvasiveStrongPtr {
	  public:		
		/** @brief Create a null pointer. */
		inline CxInvasiveStrongPtr() : mp_ptr(0) {}

		/**
		 * @brief Create a CxInvasiveStrongPtr wrapped around a pointer.
		 * Do NOT pass a NIL pointer to this constructor.
		 * @param in_ptr The pointer to wrap around.
		 */
		inline CxInvasiveStrongPtr(T *in_ptr) : mp_ptr(in_ptr) {
			mp_ptr->retain();
		}
		
		/**
		 * @brief Copy constructor, makes sure to increment the reference count.
		 * @param src The source object to copy from.
		 */
		inline CxInvasiveStrongPtr(const CxInvasiveStrongPtr<T> &in_src) {
			mp_ptr = in_src.mp_ptr;
			if (mp_ptr) { mp_ptr->retain(); }			
		}

		/**
		 * @brief Assignment operator, makes sure to increment / decrement reference count.
		 * @param in_src The source object to copy from.
		 * @return A reference to the InvasiveStrongPtr.
		 */
		CxInvasiveStrongPtr& operator=(const CxInvasiveStrongPtr<T> &in_src) {
			if (in_src.mp_ptr) { in_src.mp_ptr->retain(); }			
			releaseAndDeleteIfNeeded();
			mp_ptr = in_src.mp_ptr;			
			return *this;			
		}

		/**
		 * @brief Assignment operator, makes sure to increment / decrement reference count.
		 * @param in_src The object to store as a pointer.
		 * @return A reference to the InvasiveStrongPtr.
		 */
		CxInvasiveStrongPtr& operator=(T *in_ptr) {
			if (in_ptr) { in_ptr->retain(); }			
			releaseAndDeleteIfNeeded();
			mp_ptr = in_ptr;			
			return *this;			
		}

		/**
		 * @brief Overloaded equality operator.
		 * @param in_ptr The Other pointer to compare to.
		 * @return True if the pointers point to the same object.
		 */
		inline CxBool operator==(const CxInvasiveStrongPtr<T> &in_ptr) const {
			return mp_ptr == in_ptr.mp_ptr;
		}

		/**
		 * @brief Overloaded not equals operator.
		 * @param in_ptr The Other pointer to compare to.
		 * @return True if the pointers point different objects.
		 */
		inline CxBool operator!=(const CxInvasiveStrongPtr<T> &in_ptr) const {
			return mp_ptr != in_ptr.mp_ptr;
		}	

		/** @brief Checks and decrements reference count and deletes pointer if needed. */
		inline ~CxInvasiveStrongPtr() {
		   releaseAndDeleteIfNeeded();			
		}
		
		/** @return The object being pointed to by this CxInvasiveStrongPtr pointer (const). */
		inline const T & operator*() const {
			CXD_IF_ERR(!mp_ptr, "Trying to dereference null ISPtr.");
			return *mp_ptr;
		}

		/** @return The object being pointed to by this CxInvasiveStrongPtr pointer. */
		inline T & operator*() {
			CXD_IF_ERR(!mp_ptr, "Trying to dereference null ISPtr.");
			return *mp_ptr;
		}

	   /** @return The object being pointed to by this CxInvasiveStrongPtr pointer (const). */
		inline const T * operator->() const { return mp_ptr; }

		/** @return The object being pointed to by this CxInvasiveStrongPtr pointer. */
		inline T * operator->() { return mp_ptr; }

	   /** @return The object being pointed to by this CxInvasiveStrongPtr pointer (const). */
		inline T * ptr() const { return mp_ptr; }

		/** @return The number of references to the pointer. */
		inline CxI32 retainCount() const {
			return (mp_ptr) ? mp_ptr->retainCount() : 0;
		}

		/** @return true If the pointer is null, false otherwise. */
		inline CxBool isNull() const { return mp_ptr == 0; }

		/** @return true If the pointer is NOT null, false otherwise. */
		inline CxBool notNull() const { return mp_ptr != 0; }	

		/** @brief Set the pointer to be a null pointer. */
		inline void setNull() { releaseAndDeleteIfNeeded(); }

		/** @return A null pointer. */
		inline static CxInvasiveStrongPtr<T> nullPtr() { return CxInvasiveStrongPtr<T>(); }
			
	  private:
		inline void releaseAndDeleteIfNeeded() {
			if (mp_ptr && mp_ptr->release()) {
#if defined (DEBUG_REF_POINTERS)
				CXD_MSG("CxInvasiveStrongPtr count at 0 for reference, deleting pointer!");
#endif
				delete mp_ptr;
				mp_ptr = 0;
			}
		}		
		
		T* mp_ptr;		
	};

	/**
	 * @brief Macro for safetly deleting an invasive strong pointer type.
	 */
#define CxDeleteISPtr(x) \
	if ((x) && (x)->release()) { delete (x); (x) = 0; }

} // namespace cat

#endif // CX_CORE_COMMON_CXINVASIVESTRONGPTR_H

