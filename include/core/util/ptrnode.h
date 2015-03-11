#ifndef CAT_CORE_UTIL_PTRNODE_H
#define CAT_CORE_UTIL_PTRNODE_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file ptrnode.h
 * @brief Contains the template(s) for a simple Node class to hold reference counted pointers.
 *
 * @author Catlin Zilinski
 * @date Mar 18, 2014
 */
#include "core/corelib.h"

namespace Cat {

	/**
	 * @class PtrNode ptrnode.h "core/util/ptrnode.h"
	 * @brief A simple Node class to hold only a single item and no extra data.
	 *
	 * @author Catlin Zilinski
	 * @since Mar 18, 2014
	 * @version 1
	 */
	template<typename T>
	class PtrNode {
	  public:
		PtrNode<T>* prev;
		PtrNode<T>* next;
		T ptr;		
		
		PtrNode() : prev(NIL), next(NIL) {}
		
		void alloc(PtrNode<T>* root, const T& pPtr) {
			detach();
			attach(root);			
			ptr = pPtr;
		}

		void dealloc(PtrNode<T>* root) {
			detach();
			attach(root);
			ptr.setNull();
		}

		void initAsRoot() {
			prev = next = this;
		}		

		void realloc(PtrNode<T>* root) {
			detach();
			attach(root);
		}

		inline void detach() {
			next->prev = prev;
		   prev->next = next;
		}
		
		inline void attach(PtrNode<T>* root) {
			root->prev->next = this;			
			prev = root->prev;
			next = root;
			root->prev = this;
		}

		inline void init(PtrNode<T>* root) {
			root->prev->next = this;
			prev = root->prev;
			next = root;
			root->prev = this;
		}

#ifdef DEBUG
		friend std::ostream& operator<<(std::ostream& out, const PtrNode<T>& p) {
			out << "PtrNode<T>: " << &p << " ";			
			return out << "{ next: " << p.next << ", prev: " << p.prev << ", ptr: " <<  &(p.ptr) << "}";
		}	
#endif // DEBUG
		
	};

	
} // namespace Cat

#endif // CAT_CORE_UTIL_PTRNODE_H
