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
		/**
		 * @brief Create a null pointer.
		 */
		inline CxInvasiveStrongPtr() : m_pPtr(NIL) {}

		/**
		 * @brief Create a CxInvasiveStrongPtr wrapped around a pointer.
		 * Do NOT pass a NIL pointer to this constructor.
		 * @param ptr The pointer to wrap around.
		 */
		inline CxInvasiveStrongPtr(T* ptr) : m_pPtr(ptr) {
			m_pPtr->retain();
		}
		
		/**
		 * @brief Copy constructor, makes sure to increment the reference count.
		 * @param src The source object to copy from.
		 */
		inline CxInvasiveStrongPtr(const CxInvasiveStrongPtr<T>& src) {
			m_pPtr = src.m_pPtr;
			if (m_pPtr) {
				m_pPtr->retain();
			}			
		}

		/**
		 * @brief Assignment operator, makes sure to increment / decrement reference count.
		 * @param src The source object to copy from.
		 * @return A reference to the InvasiveStrongPtr.
		 */
		CxInvasiveStrongPtr& operator=(const CxInvasiveStrongPtr<T>& src) {
			if (src.m_pPtr) {
				src.m_pPtr->retain();
			}			
			releaseAndDeleteIfNeeded();
			m_pPtr = src.m_pPtr;			
			return *this;			
		}

		/**
		 * @brief Assignment operator, makes sure to increment / decrement reference count.
		 * @param src The object to store as a pointer.
		 * @return A reference to the InvasiveStrongPtr.
		 */
		CxInvasiveStrongPtr& operator=(T* ptr) {
			if (ptr) {
				ptr->retain();
			}			
			releaseAndDeleteIfNeeded();
			m_pPtr = ptr;			
			return *this;			
		}

		/**
		 * @brief Overloaded equality operator.
		 * @param other The Other pointer to compare to.
		 * @return True if the pointers point to the same object.
		 */
		inline CxBool operator==(const CxInvasiveStrongPtr<T>& other) const {
			return m_pPtr == other.m_pPtr;
		}

		/**
		 * @brief Overloaded not equals operator.
		 * @param other The Other pointer to compare to.
		 * @return True if the pointers point different objects.
		 */
		inline CxBool operator!=(const CxInvasiveStrongPtr<T>& other) const {
			return m_pPtr != other.m_pPtr;
		}	

		/**
		 * @brief Checks and decrements reference count and deletes pointer if needed.
		 */
		inline ~CxInvasiveStrongPtr() {
		   releaseAndDeleteIfNeeded();			
		}
		
		/**
		 * @brief Overloaded pointer dereference operator.
		 * @return The object being pointed to by this CxInvasiveStrongPtr pointer.
		 */
		inline const T& operator*() const {
#if defined(DEBUG)
			if (!m_pPtr) {
				DERR("Trying to dereference NIL pointer!");
			}
#endif
			return *m_pPtr;
		}

		inline T& operator*() {
#if defined(DEBUG)
			if (!m_pPtr) {
				DERR("Trying to dereference NIL pointer!");
			}
#endif
			return *m_pPtr;
		}

		/**
		 * @brief Overload the -> operator to allow for access to an objects members.
		 * @return The pointer.
		 */
		inline const T* operator->() const { return m_pPtr; }
		inline T* operator->() { return m_pPtr; }

		/**
		 * @brief Get the pointer.
		 * @return The actual pointer.
		 */
		inline T* ptr() const { return m_pPtr; }

		/**
		 * @brief Get the number of references to the pointer.
		 * @return The number of references to the pointer.
		 */
		inline CxI32 retainCount() const { return (m_pPtr) ? m_pPtr->retainCount() : 0; }

		/**
		 * @brief Test to see if the pointer is null.
		 * @return true If the pointer is NIL, false otherwise.
		 */
		inline CxBool isNull() const { return m_pPtr == NIL; }

		/**
		 * @brief Test to see if the pointer is NOT null.
		 * @return true If the pointer is NOT NIL, false otherwise.
		 */
		inline CxBool notNull() const { return m_pPtr != NIL; }	

		/**
		 * @brief Set the pointer to be a NIL pointer.
		 */
		inline void setNull() {
		   releaseAndDeleteIfNeeded();			
		}

		/**
		 * @brief Get a null pointer.
		 * @return A null pointer.
		 */
		inline static CxInvasiveStrongPtr<T> nullPtr() { return CxInvasiveStrongPtr<T>(); }
			
	  private:
		inline void releaseAndDeleteIfNeeded() {
			if (m_pPtr && m_pPtr->release()) {
#if defined (DEBUG_REF_POINTERS)
				DMSG("CxInvasiveStrongPtr count at 0 for reference, deleting pointer!");
#endif
				delete m_pPtr;
			}
			m_pPtr = NIL;
		}		
		
		T* m_pPtr;		
	};

} // namespace cat

#endif // CX_CORE_COMMON_CXINVASIVESTRONGPTR_H

