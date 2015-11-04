#include "core/threading/processqueue.h"

namespace Cat {

	/* ############################# */
	/* #### PROCESS QUEUE INDEX #### */
	/* ############################# */
	ProcessQueueIndex::ProcessQueueIndex(const ProcessQueueIndex& src) {
		m_capacity = src.m_capacity;
		if (src.m_pIndices) {
			m_pIndices = new PQICell[m_capacity];
			memcpy(m_pIndices, src.m_pIndices, sizeof(PQICell)*m_capacity);
		}
		else {
			m_pIndices = NIL;
		}
	}

	ProcessQueueIndex& ProcessQueueIndex::operator=(const ProcessQueueIndex& src) {
		PQICell* old = m_pIndices;		
		if (src.m_pIndices) {
			PQICell* indices = new PQICell[src.m_capacity];						
			memcpy(indices, src.m_pIndices, sizeof(PQICell)*src.m_capacity);
			m_pIndices = indices;
			m_capacity = src.m_capacity;			
		}
		if (old) {
			delete[] old;
		}		
		return *this;			
	}	

	ProcessQueueIndex::~ProcessQueueIndex() {
		if (m_pIndices) {
			delete[] m_pIndices;
			m_pIndices = NIL;
		}
		m_capacity = 0;		
	}

} // namespace Cat
