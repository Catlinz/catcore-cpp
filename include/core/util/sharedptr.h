#ifndef CAT_CORE_UTIL_SHAREDPTR_H
#define CAT_CORE_UTIL_SHAREDPTR_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file sharedptr.h
 * @brief Contains a reference counting pointer wrapper.
 *
 * The SharedPtr class is a simple reference counting safe pointer that will 
 * not destroy its data when deleted until there are no more references.
 *
 * @author Catlin Zilinski
 * @date: Nov 21, 2013
 */

#include <cstdlib>
#include "core/corelib.h"

namespace Cat {

	/**
	 * @class SharedPtr sharedptr.h "core/util/sharedptr.h"
	 * @brief A Simple reference counting safe pointer.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Oct 18, 2013
	 */
	template<typename T>
	class SharedPtr {
		public:		
			/**
			 * @brief Create an unbound, empty shared pointer.
			 * A pointer must be assigned before using.
			 */
			SharedPtr();

			/**
			 * @brief Create a SharedPtr wrapped around a pointer.
			 * @param ptr The pointer to wrap around.
			 * @param freeMethod How the pointer should be deleted.
			 */
			explicit SharedPtr(T* ptr, PtrFreeMethod freeMethod = PFM_DELETE );

			/**
			 * @brief Copy constructor, makes sure to increment the reference count.
			 * @param src The source object to copy from.
			 */
			SharedPtr(const SharedPtr<T>& src);

			/**
			 * @brief Assignment operator, makes sure to increment / decrement reference count.
			 * @param src The source object to copy from.
			 * @return A reference to the SharedPtr.
			 */
			SharedPtr& operator=(const SharedPtr<T>& src);

			/**
			 * @brief Checks and decrements reference count and deletes pointer if needed.
			 */
			~SharedPtr();

			/**
			 * @brief Overloaded pointer dereference operator.
			 * @return The object being pointed to by this SharedPtr pointer.
			 */
			inline T& operator*() const;

			/**
			 * @brief Overload the -> operator to allow for access to an objects members.
			 * @return The pointer.
			 */
			inline T* operator->() const;

			/**
			 * @brief Overload the [] operator to allow for arrays to be ref counted as well.
			 * @param idx The index of the element in the array.
			 * @return A reference to the element of the array.
			 */
			inline T& operator[](I32 idx) const;

			/**
			 * @brief Get the pointer.
			 * @return The actual pointer.
			 */
			inline T* get() const;

			/**
			 * @brief Assign a pointer to this shared pointer.
			 *
			 * Is assumed that the pointer has not been assigned to yet.
			 *
			 * @param The pointer value to assign to this shared pointer.
			 * @return A reference to this shared pointer.
			 */
			SharedPtr<T>& assign(T* ptr, PtrFreeMethod freeMethod = PFM_DELETE);			

			/**
			 * @brief Get the number of references to the pointer.
			 * @return The number of references to the pointer.
			 */
			inline I32 count() const;

			/**
			 * @brief Get the pointer to the reference count.
			 * @return The pointer to the reference counter.
			 */
			inline const I32* countPtr() const;			

			/**
			 * @brief Test to see if the pointer is null or not.
			 * @return true If the pointer is NIL, false otherwise.
			 */
			inline Boolean isNull() const;

			/**
			 * @brief Set the pointer to be a NIL pointer.
			 */
			inline void setNull();			

			/**
			 * @brief Get the PtrFreeMethod that will be used to delete the pointer.
			 * @return The PtrFreeMethod.
			 */
			inline PtrFreeMethod ptrFreeMethod() const;
			
		private:
			inline void		decrementCountAndFreeIfNecessary();
			void				deletePtr();	
		
			I32*				m_pCount;
			T*					m_pPtr;
			PtrFreeMethod	m_eFreeMethod;			
	};

	template<typename T>
	SharedPtr<T>::SharedPtr() {
		m_pCount = NIL;
		m_pPtr = NIL;
		m_eFreeMethod = PFM_FREE;
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(T* ptr, PtrFreeMethod freeMethod) {
		m_pCount = NIL;
		m_pPtr = NIL;
		assign(ptr, freeMethod);
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr<T>& src) {
		m_pCount = src.m_pCount;
		(*m_pCount) += 1;
		m_pPtr = src.m_pPtr;
		m_eFreeMethod = src.m_eFreeMethod;		
	}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& src) {
		if (m_pCount) {	
			decrementCountAndFreeIfNecessary();
		}		
		m_pCount = src.m_pCount;
		(*m_pCount) += 1;
		m_pPtr = src.m_pPtr;
		m_eFreeMethod = src.m_eFreeMethod;	
		return *this;
		
	}

	template<typename T>
	SharedPtr<T>::~SharedPtr() {
		if (m_pCount) {
			decrementCountAndFreeIfNecessary();	
		}	
	}

	template<typename T>
		inline T& SharedPtr<T>::operator*() const { 
#if defined(DEBUG)
		if (!m_pPtr) {
			DERR("Trying to dereference NIL pointer!");
		}
#endif
		return *m_pPtr;
	}

	template<typename T>
		inline T& SharedPtr<T>::operator[](I32 idx) const {
#if defined(DEBUG)
		if (!m_pPtr) {
			DWARN("Trying to access element of NIL pointer!");
		}
#endif
		return m_pPtr[idx];
	}

	template <typename T> 
	inline T* SharedPtr<T>::operator->() const {
		return m_pPtr;
	}

	template<typename T>
	inline I32 SharedPtr<T>::count() const { 
		return *m_pCount;
	}

	template<typename T>
	inline const I32* SharedPtr<T>::countPtr() const { 
		return m_pCount;
	}

	template <typename T> 
   inline T* SharedPtr<T>::get() const {
		return m_pPtr;
	}

	template <typename T>
	SharedPtr<T>& SharedPtr<T>::assign(T* ptr, PtrFreeMethod freeMethod) {
#if defined(DEBUG)
		if (m_pPtr) {
			DERR("Assigning to non-null Shared Pointer!");
		}
#endif
		m_pCount = (I32*)malloc(sizeof(I32));
		(*m_pCount) = 1;
		m_pPtr = ptr;
		m_eFreeMethod = freeMethod;
		return *this;
	}
	
	template <typename T> inline Boolean SharedPtr<T>::isNull() const {
		return m_pPtr == NIL;
	}
	template <typename T>
	inline void SharedPtr<T>::setNull() {
		decrementCountAndFreeIfNecessary();
		m_pPtr = NIL;
	}

	template <typename T>
	inline PtrFreeMethod SharedPtr<T>::ptrFreeMethod() const {
		return m_eFreeMethod;		
	}

	template<typename T>
	inline void SharedPtr<T>::decrementCountAndFreeIfNecessary() {
		if ( --(*m_pCount) <= 0) {
			deletePtr();
		}
	}
	
	template<typename T>
	void SharedPtr<T>::deletePtr() {
#if defined (DEBUG_REF_POINTERS)
		DMSG("SharedPtr count at 0 for reference, deleting pointer!");
#endif
		switch(m_eFreeMethod) {
		case PFM_DELETE:
			delete m_pPtr;
			break;
		case PFM_DELETE_OBJ:
			delete m_pPtr;
			break;
		case PFM_FREE:
			free(m_pPtr);
			break;
		case PFM_DELETE_ARRAY:
			delete[] m_pPtr;
			break;
		default:
			delete m_pPtr;
			break;
		}

		m_pPtr = NIL;
		free(m_pCount);
		m_pCount = NIL;
		
		
	}

} // namespace Cat

#endif // CAT_CORE_UTIL_SHAREDPTR_H

