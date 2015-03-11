#ifndef CAT_CORE_UTIL_DATANODE_H
#define CAT_CORE_UTIL_DATANODE_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file datanode.h
 * @brief Contains a simple DataNode to hold data.
 *
 * @author Catlin Zilinski
 * @date Mar 21, 2014
 */
#include "core/corelib.h"

namespace Cat {

	/**
	 * @class DataNode datanode.h "core/util/datanode.h"
	 * @brief A simple DataNode class to hold only a single item and no extra data.
	 *
	 * @author Catlin Zilinski
	 * @since Mar 21, 2014
	 * @version 1
	 */
	template<typename T>
	class DataNode {
	  public:
		DataNode<T>* prev;
		DataNode<T>* next;
		Boolean isRoot;
		T data;
				
		DataNode() : prev(NIL), next(NIL), isRoot(true) {
			prev = next = this;			
		}
		
		void alloc(DataNode<T>* pRoot, const T& pData) {
			detach();
			attach(pRoot);
			data = pData;			
		}

		void dealloc(DataNode<T>* pRoot) {
			detach();
			attach(pRoot);
		}

		void initAsRoot() {
			prev = next = this;
			isRoot = true;			
		}		

		void realloc(DataNode<T>* pRoot) {
			detach();
			attach(pRoot);
		}

		inline void detach() {
			next->prev = prev;
		   prev->next = next;
		}
		
		inline void attach(DataNode<T>* pRoot) {
			pRoot->prev->next = this;			
			prev = pRoot->prev;
			next = pRoot;
			pRoot->prev = this;
			isRoot = false;			
		}

		inline void init(DataNode<T>* pRoot) {
			pRoot->prev->next = this;
			prev = pRoot->prev;
			next = pRoot;
			pRoot->prev = this;
			isRoot = false;			
		}

#ifdef DEBUG
		friend std::ostream& operator<<(std::ostream& out, const DataNode<T>& p) {
			out << "DataNode<T>: " << &p << " ";			
			return out << "{ root: " << p.isRoot << ", next: " << p.next << ", prev: " << p.prev << ", data: " <<  &(p.data) << "}";
		}	
#endif // DEBUG
		
	};

	
} // namespace Cat

#endif // CAT_CORE_UTIL_DATANODE_H
