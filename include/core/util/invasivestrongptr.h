#ifndef CAT_CORE_UTIL_INVASIVESTRONGPTR_H
#define CAT_CORE_UTIL_INVASIVESTRONGPTR_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file invasivestrongptr.h
 * @brief Contains a reference counting pointer wrapper.
 *
 * The InvasiveStrongPtr class is a strong reference counting safe pointer which assumes 
 * certain methods on the pointer it is storing to increment and decrement the 
 * reference count, which is stored on the actual object.
 *
 * @author Catlin Zilinski
 * @date Mar 4, 2014
 */

#include <cstdlib>
#include "core/corelib.h"

namespace Cat {

	/**
	 * @class InvasiveStrongPtr invasivestrongptr.h "core/util/invasivestrongptr.h"
	 * @brief A strong reference counting safe pointer.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 4, 2014
	 */
	template<typename T>
	class InvasiveStrongPtr {
	  public:		
		/**
		 * @brief Create a null pointer.
		 */
		inline InvasiveStrongPtr() : m_pPtr(NIL) {}

		/**
		 * @brief Create a InvasiveStrongPtr wrapped around a pointer.
		 * Do NOT pass a NIL pointer to this constructor.
		 * @param ptr The pointer to wrap around.
		 */
		inline InvasiveStrongPtr(T* ptr) : m_pPtr(ptr) {
			m_pPtr->retain();
		}
		
		/**
		 * @brief Copy constructor, makes sure to increment the reference count.
		 * @param src The source object to copy from.
		 */
		inline InvasiveStrongPtr(const InvasiveStrongPtr<T>& src) {
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
		InvasiveStrongPtr& operator=(const InvasiveStrongPtr<T>& src) {
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
		InvasiveStrongPtr& operator=(T* ptr) {
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
		inline Boolean operator==(const InvasiveStrongPtr<T>& other) const {
			return m_pPtr == other.m_pPtr;
		}

		/**
		 * @brief Overloaded not equals operator.
		 * @param other The Other pointer to compare to.
		 * @return True if the pointers point different objects.
		 */
		inline Boolean operator!=(const InvasiveStrongPtr<T>& other) const {
			return m_pPtr != other.m_pPtr;
		}	

		/**
		 * @brief Checks and decrements reference count and deletes pointer if needed.
		 */
		inline ~InvasiveStrongPtr() {
		   releaseAndDeleteIfNeeded();			
		}
		
		/**
		 * @brief Overloaded pointer dereference operator.
		 * @return The object being pointed to by this InvasiveStrongPtr pointer.
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
		inline I32 retainCount() const { return (m_pPtr) ? m_pPtr->retainCount() : 0; }

		/**
		 * @brief Test to see if the pointer is null.
		 * @return true If the pointer is NIL, false otherwise.
		 */
		inline Boolean isNull() const { return m_pPtr == NIL; }

		/**
		 * @brief Test to see if the pointer is NOT null.
		 * @return true If the pointer is NOT NIL, false otherwise.
		 */
		inline Boolean notNull() const { return m_pPtr != NIL; }	

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
		inline static InvasiveStrongPtr<T> nullPtr() { return InvasiveStrongPtr<T>(); }
			
	  private:
		inline void releaseAndDeleteIfNeeded() {
			if (m_pPtr && m_pPtr->release()) {
#if defined (DEBUG_REF_POINTERS)
				DMSG("InvasiveStrongPtr count at 0 for reference, deleting pointer!");
#endif
				delete m_pPtr;
			}
			m_pPtr = NIL;
		}		
		
		T* m_pPtr;		
	};

} // namespace Cat

#endif // CAT_CORE_UTIL_INVASIVESTRONGPTR_H

