#ifndef CAT_CORE_UTIL_DATANODEPOOL_H
#define CAT_CORE_UTIL_DATANODEPOOL_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file datanodepool.h
 * @brief Contains a storage class to store a number of DataNodes.
 *
 * @author Catlin Zilinski
 * @date Mar 21, 2014
 */
#include "core/corelib.h"
#include "core/util/datanode.h"

namespace Cat {

	/**
	 * @class DataNodePool datanodepool.h "core/util/datanodepool.h"
	 * @brief A class to hold a storage of allocated DataNodes.
	 *
	 * @author Catlin Zilinski
	 * @since Mar 21, 2014
	 * @version 1
	 */
	template<typename t>
	class datanodepool {
	  public:

		/**
		 * @brief create an empty datanodepool.
		 */
		inline DataNodePool()
			: m_numFree(0), m_pNodeStorage(NIL), m_blockSize(0) {
		}

		/**
		 * @brief Create a new DataNodePool with the specified amount of storage.
		 * @param blockSize The number of DataNodes to allocate.
		 */
		DataNodePool(Size blockSize);

		/**
		 * @brief Deletes all the nodes.
		 */
		~DataNodePool();

		/**
		 * @brief Deletes all the nodes.
		 */
		void destroy();		

		/**
		 * @brief Allocate the next free node to the specified root and return the node.
		 * @param root The root node to attach to.
		 */
		inline DataNode<T>* alloc(DataNode<T>* root) {
			if (hasAvailableNodes()) {
				DataNode<T>* node = m_root.next;
				node->realloc(root);
				m_numFree--;
				return node;
			}
			else {
				return NIL;
			}			
		}

		/**
		 * @brief Allocate the next free node to the specified root and return the node.
		 * @param root The root node to attach to.
		 * @param data The data to store in the node.
		 */
		inline DataNode<T>* alloc(DataNode<T>* root, const T& data) {
			if (hasAvailableNodes()) {
				DataNode<T>* node = m_root.next;
				node->alloc(root, data);
				m_numFree--;
				return node;
			}
			else {
				return NIL;
			}		
		}

		/**
		 * @brief Get the number of DataNodes that were allocated.
		 * @return The number of nodes that were allocated.
		 */
		inline Size blockSize() const { return m_blockSize; }		

		/**
		 * @brief Put the specified DataNode back into the list of free nodes.
		 * @param node The DataNode to return to the list.
		 */
		inline void free(DataNode<T>* node) {
			node->dealloc(&m_root);
			m_numFree++;			
		}		

		/**
		 * @brief Check to see if there are any available nodes in the Store.
		 * @return True if there are nodes available.
		 */
		inline Boolean hasAvailableNodes() const { return (m_root.next != &m_root); }

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
		inline Size numAllocated() const { return m_blockSize - m_numFree; }

		/**
		 * @brief Reset the node store so all the nodes are back on the free list.
		 */
		void reset();		
	

	  private:
		void putAllNodesOnFreeList();
		
		DataNode<T> m_root;
		DataNode<T> *m_pNodeStorage;
		Size m_numFree;
		Size m_blockSize;
	};

	template<typename T>
	DataNodePool<T>::DataNodePool(Size blockSize) {
		m_root.initAsRoot();		
		m_pNodeStorage = new DataNode<T>[blockSize];
		m_blockSize = blockSize;
		putAllNodesOnFreeList();
	}

	template<typename T>
	DataNodePool<T>::~DataNodePool() {	
		destroy();		
	}

	template<typename T>
	void DataNodePool<T>::destroy() {	
		m_root.initAsRoot();
		if (m_pNodeStorage) {
			delete m_pNodeStorage;
			m_pNodeStorage = NIL;
		}		
		m_numFree =  m_blockSize = 0;
	}

	template<typename T>
	void DataNodePool<T>::initWithBlockSize(U32 blockSize) {
		/* Make sure not to initialize already initialized node store */
		if (m_pNodeStorage != NIL) {
			DWARN("Cannot initialize DataNodePool twice!");
			return;			
		}		
		m_root.initAsRoot();		
		m_pNodeStorage = new DataNode<T>[blockSize];
		m_blockSize = blockSize;
		putAllNodesOnFreeList();
	}	

	template<typename T>
	void DataNodePool<T>::reset() {
		m_root.initAsRoot();
		for (Size i = 0; i < m_blockSize; i++) {
			m_pNodeStorage[i].dealloc(&m_root);
		}
		m_numFree = m_blockSize;		
	}

	template<typename T>
	void DataNodePool<T>::putAllNodesOnFreeList() {
		for (Size i = 0; i < m_blockSize; i++) {			
			m_pNodeStorage[i].init(&m_root);
		}
		m_numFree = m_blockSize;		
	}
} // namespace Cat

#endif // CAT_CORE_UTIL_DATANODEPOOL_H
