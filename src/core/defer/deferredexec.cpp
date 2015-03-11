#include "core/defer/deferredexec.h"

namespace Cat {

	DeferredExec* DeferredExec::s_pGlobal = NIL;

	DeferredExecCall::DeferredExecCall()
		: m_pFunc(NIL), m_pObj(NIL) {
		m_data.vPtr = NIL;
	}

	DeferredExecCall::DeferredExecCall(void (*p_func)(void*, IntegralType),
												  void* p_obj, IntegralType p_data)
		: m_pFunc(p_func), m_pObj(p_obj), m_data(p_data) {

	}


	DeferredExec::DeferredExec()
		: m_pCalls(NIL), m_pProcessing(NIL) {
	}

	DeferredExec::DeferredExec(Size p_capacity) {
		m_callsOne.initQueueWithCapacityAndNull(p_capacity, DeferredExecCall());
		m_callsTwo.initQueueWithCapacityAndNull(p_capacity, DeferredExecCall());
		m_pCalls = &m_callsOne;
		m_pProcessing = &m_callsTwo;
	}

	DeferredExec::~DeferredExec() {
		m_lock.lock();		
		if (m_callsOne.capacity() > 0) {			
			m_callsOne.clear();
		}
		if (m_callsTwo.capacity() > 0) {			
			m_callsTwo.clear();
		}	  
		m_pCalls = NIL;
		m_pProcessing = NIL;		
		m_lock.unlock();		
	}

	void DeferredExec::executeCalls() {
		/* Swap the queues to prevent infinite queuing */
		SimpleQueue<DeferredExecCall>* tmpForSwap = m_pProcessing;		
		m_lock.lock();
		m_pProcessing = m_pCalls;
		m_pCalls = tmpForSwap;	
		m_lock.unlock();

		while (!m_pProcessing->isEmpty()) {
		   m_pProcessing->pop().execute();
		}
	}

	void DeferredExec::initialiseGlobalDeferredExecQueue(Size p_capacity) {
		if (s_pGlobal) {
			DWARN("Cannot initialise global DeferredExec queue more than once!");
		}
		else {
			s_pGlobal = new DeferredExec(p_capacity);
		}
	}
	
	void DeferredExec::destroyGlobalDeferredExecQueue() {
		if (s_pGlobal) {
			delete s_pGlobal;
			s_pGlobal = NIL;
		}
		else {
			DWARN("Cannot delete already deleted global DeferredExec queue!");
		}
	}
	

} // namespace Cat
