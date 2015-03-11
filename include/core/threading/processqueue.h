#ifndef CAT_CORE_THREADING_PROCESSQUEUE_H
#define CAT_CORE_THREADING_PROCESSQUEUE_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file processqueue.h
 * @brief Contains a Queue of Processes.
 *
 * @author Catlin Zilinski
 * @date Mar 12, 2014
 */
#include <cstring>
#include "core/corelib.h"
#include "core/threading/process.h"

namespace Cat {

	class ProcessQueueNode {
	  public:
		ProcessQueueNode* prev;
		ProcessQueueNode* next;
		ProcessPtr process;
		I32 count;		
		
		ProcessQueueNode() : prev(NIL), next(NIL), count(0) {};
		
		void alloc(ProcessQueueNode* root, const ProcessPtr& pProcess) {
			detach();
			attach(root);			
			process = pProcess;
		}

		void dealloc(ProcessQueueNode* root) {
			detach();
			attach(root);
			process.setNull();
		}

		void initAsRoot() {
			prev = next = this;
		}		

		void realloc(ProcessQueueNode* root) {
			detach();
			attach(root);
		}

		inline void detach() {
			next->prev = prev;
		   prev->next = next;
		}
		
		inline void attach(ProcessQueueNode* root) {
			root->prev->next = this;			
			prev = root->prev;
			next = root;
			root->prev = this;
		}

		inline void init(ProcessQueueNode* root) {
			root->prev->next = this;
			prev = root->prev;
			next = root;
			root->prev = this;
		}		
	};

	
	class PQICell {
	  public:
		OID pid;
		ProcessQueueNode* node;		
		
		inline PQICell() :
			pid(0), node(NIL) {}
		
		inline PQICell(OID pPid, ProcessQueueNode* pNode) :
			pid(pPid), node(pNode) {}

		inline ~PQICell() {
			pid = 0;
			node = NIL;
		}
	};
		

	/**
	 * @class ProcessQueueIndex processqueue.h "core/threading/processqueue.h"
	 * @brief An index Into the processQueue by OID of the process.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 6, 2014
	 */
	class ProcessQueueIndex {
	  public:
		/**
		 * @brief Create an empty ProcessQueueIndex.
		 */
		ProcessQueueIndex() : m_capacity(0), m_pIndices(NIL) {}		

		/**
		 * @brief Create a new ProcessQueueIndex with number of spaces.
		 * @param capacity The number of spaces to have in the index.
		 */
		ProcessQueueIndex(Size capacity) : m_capacity(capacity) {
			m_pIndices = new PQICell[m_capacity];
		}

		/**
		 * @brief Copy constructor, just in case. Copies the array data.
		 * @param src The ProcessQueueIndex to copy from.
		 */
		ProcessQueueIndex(const ProcessQueueIndex& src);		

		/**
		 * @brief Assignment operator. Copies the array data.
		 * @param src The ProcessQueueIndex to copy from.
		 * @return A reference to this ProcessQueueIndex.
		 */
		ProcessQueueIndex& operator=(const ProcessQueueIndex& src);		

		/**
		 * @brief Delete the index.
		 */
		~ProcessQueueIndex();		

		/**
		 * @brief Find the specified Process location by the OID.
		 * @param pid The OID of the process to find.
		 * @return The index in the queue of the process.
		 */
		ProcessQueueNode* find(OID pid) const {
			for (U32 i = 0; i < m_capacity; i++) {			
				if (m_pIndices[i].pid == pid) {
					return m_pIndices[i].node;
				}
			}
			return NIL;
		}

		/**
		 * @brief Initialize the ProcessQueueIndex with the specified capacity.
		 * @param capacity The Capacity for the PQI to have.
		 */
		void initWithCapacity(Size capacity) {
			PQICell* indices = new PQICell[capacity];
			PQICell* old = m_pIndices;
			m_pIndices = indices;
			m_capacity = capacity;			
			if (old) {
				delete[] old;
			}			
		}		
			

		/**
		 * @brief Remove the specified index entry from the index.
		 * @param pid The OID of the index to remove.
		 */
		void remove(OID pid) {
			for (U32 i = 0; i < m_capacity; i++) {
				if (m_pIndices[i].pid == pid) {
					m_pIndices[i].pid = 0;
				   return;					
				}
			}
		}

		/**
		 * @brief Remove all the entries from the index.
		 */
		void clear() {
			for (U32 i = 0; i < m_capacity; i++) {
				m_pIndices[i].pid = 0;				
			}
		}

		/**
		 * @brief Remove the entry at the specified index.
		 * @param index The index value to remove.
		 */
		inline void removeAtIndex(I32 index) {
			m_pIndices[index].pid = 0;
		}		
		

		/**
		 * @brief Set the specified entry in the index.
		 * @param pid The OID of the process.
		 * @param node The processqueuenode the process is in.
		 */
		inline void set(OID pid, ProcessQueueNode* node) {	
			for (U32 i = 0; i < m_capacity; i++) {
				if (m_pIndices[i].pid == 0) {									
					m_pIndices[i].node = node;
					m_pIndices[i].pid = pid;
					return;					
				}
			}				
		}

		/**
		 * @brief Set the specified entry in the index.
		 * @param pid The OID of the process.
		 * @param node The processqueuenode the process is in.
		 */
		inline void setAtIndex(U32 idx, OID pid, ProcessQueueNode* node) {
			m_pIndices[idx].pid = 0;
			m_pIndices[idx].node = node;								
		}
		

	  private:
		Size m_capacity;
		PQICell* m_pIndices;
	};

} // namespace Cat

#endif // CAT_CORE_THREADING_PROCESSQUEUE_H
