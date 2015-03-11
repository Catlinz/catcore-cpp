#ifndef CAT_CORE_UTIL_PTRNODESTORE_H
#define CAT_CORE_UTIL_PTRNODESTORE_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file ptrnodestore.h
 * @brief Contains a storage class to store a number of PtrNode's.
 *
 * @author Catlin Zilinski
 * @date Mar 18, 2014
 */
#include "core/corelib.h"
#include "core/util/ptrnode.h"

#define kPNSMaxNumBlocks 8

namespace Cat {

	/**
	 * @class PtrNodeStore ptrnodestore.h "core/util/ptrnodestore.h"
	 * @brief A class to hold a storage of allocated PtrNodes.
	 *
	 * @author Catlin Zilinski
	 * @since Mar 18, 2014
	 * @version 1
	 */
	template<typename T>
	class PtrNodeStore {
	  public:

		/**
		 * @brief Create an empty PtrNodeStore.
		 */
		inline PtrNodeStore()
			: m_numFree(0), m_numAllocated(0), m_blockSize(0) {
			m_root.initAsRoot();
			initNodeStorage();			
		}

		/**
		 * @brief Create a new PtrNodeStore with the specified amount of storage.
		 * @param blockSize The number of PtrNode's to allocate initially and in each block.
		 */
		PtrNodeStore(Size blockSize);

		/**
		 * @brief Deletes all the nodes.
		 */
		~PtrNodeStore();

		/**
		 * @brief Deletes all the nodes.
		 */
		void destroy();		

		/**
		 * @brief Allocate the next free node to the specified root and return the node.
		 * @param root The root node to attach to.
		 */
		inline PtrNode<T>* alloc(PtrNode<T>* root) {
			if (!hasAvailableNodes()) {
				if (!increaseNodeStorage()) {
					return NIL;
				}	
			}			
			PtrNode<T>* node = m_root.next;
			node->realloc(root);
			return node;
		}

		/**
		 * @brief Allocate the next free node to the specified root and return the node.
		 * @param root The root node to attach to.
		 * @param ptr The Safe Pointer to store in the node.
		 */
		inline PtrNode<T>* alloc(PtrNode<T>* root, const T& ptr) {
			if (!hasAvailableNodes()) {
				if (!increaseNodeStorage()) {
					return NIL;
				}				
			}
			PtrNode<T>* node = m_root.next;
			node->alloc(root, ptr);
			return node;
		}	

		/**
		 * @brief Put the specified PtrNode back into the list of free nodes.
		 * @param node The Node to return to the list.
		 */
		inline void free(PtrNode<T>* node) {
			node->dealloc(&m_root);
		}		

		/**
		 * @brief Check to see if there are any available nodes in the Store.
		 * @return True if there are nodes available.
		 */
		inline Boolean hasAvailableNodes() const { return (m_numFree > 0) || (m_pNodeStorage[kPNSMaxNumBlocks-1] == NIL); }

		/**
		 * @brief Try and allocate another block of nodes.
		 * @return True if the allocation succeeded, false if no more room.
		 */
		Boolean increaseNodeStorage();

		/**
		 * @brief Initialize the node store with the specified block size.
		 * @param blockSize the size of each block of nodes to allocate.
		 */
		void initWithBlockSize(U32 blockSize);		

		/**
		 * @brief Get the number of free nodes available.
		 * @return The number of free nodes avaialable.
		 */
		inline Size numFree() const { return m_numFree; }

		/**
		 * @brief Get the number of free nodes available.
		 * @return The number of free nodes avaialable.
		 */
		inline Size numAllocated() const { return m_numAllocated; }

		/**
		 * @brief Reset the node store so all the nodes are back on the free list.
		 */
		void reset();		
	

	  private:
		void initNodeStorage();
		void addNodeBlockToFreeList(Size nodeBlock);
		
		PtrNode<T> m_root;
		PtrNode<T> *m_pNodeStorage[kPNSMaxNumBlocks];
		Size m_numFree;
		Size m_numAllocated;		
		Size m_blockSize;
	};

	template<typename T>
	PtrNodeStore<T>::PtrNodeStore(Size blockSize) {
		m_root.initAsRoot();		
		initNodeStorage();
		m_pNodeStorage[0] = new PtrNode<T>[blockSize];
		m_blockSize = blockSize;
		m_numAllocated = 0;		
		addNodeBlockToFreeList(0);
		m_numFree = blockSize;		
	}

	template<typename T>
	PtrNodeStore<T>::~PtrNodeStore() {	
		destroy();		
	}

	template<typename T>
	void PtrNodeStore<T>::destroy() {	
		m_root.initAsRoot();		
		for(Size i = 0; i < kPNSMaxNumBlocks; i++) {
			if (m_pNodeStorage[i] != NIL) {
				delete[] m_pNodeStorage[i];
				m_pNodeStorage[i] = NIL;
			}			
		}
		m_numFree = m_numAllocated = m_blockSize = 0;
	}

	template<typename T>
	Boolean PtrNodeStore<T>::increaseNodeStorage() {
		Size nextBlockIdx = (m_numFree + m_numAllocated) / m_blockSize;
		if (nextBlockIdx > kPNSMaxNumBlocks) {
			DWARN("Cannot allocate any more nodes!");
			return false;
		}
		else {
			m_pNodeStorage[nextBlockIdx] = new PtrNode<T>[m_blockSize];
			addNodeBlockToFreeList(nextBlockIdx);
			return true;
		}				
	}

	template<typename T>
	void PtrNodeStore<T>::initWithBlockSize(U32 blockSize) {
		/* Make sure not to initialize already initialized node store */
		if (m_pNodeStorage[0] != NIL) {
			DWARN("Cannot initialize PtrNodeStore twice!");
			return;			
		}		
		m_root.initAsRoot();		
		initNodeStorage();
		m_pNodeStorage[0] = new PtrNode<T>[blockSize];
		m_blockSize = blockSize;
		m_numAllocated = 0;
		addNodeBlockToFreeList(0);
		m_numFree = blockSize;
	}	

	template<typename T>
	void PtrNodeStore<T>::reset() {
		m_root.initAsRoot();
		Size blockIdx = 0;
		while (blockIdx < kPNSMaxNumBlocks && m_pNodeStorage[blockIdx] != NIL) {
			for (Size i = 0; i < m_blockSize; i++) {
				m_pNodeStorage[blockIdx][i].dealloc(&m_root);
			}
			blockIdx++;			
		}		
	}

	template<typename T>
	void PtrNodeStore<T>::initNodeStorage() {
		for(Size i = 0; i < kPNSMaxNumBlocks; i++) {
			m_pNodeStorage[i] = NIL;
		}		
	}

	template<typename T>
	void PtrNodeStore<T>::addNodeBlockToFreeList(Size nodeBlock) {
		for (Size i = 0; i < m_blockSize; i++) {			
			m_pNodeStorage[nodeBlock][i].init(&m_root);
			m_pNodeStorage[nodeBlock][i].ptr = T::nullPtr();
		}					
	}
	
		
		
	
} // namespace Cat

#endif // CAT_CORE_UTIL_PTRNODESTORE_H
