#include "core/defer/deferredaction.h"

namespace Cat {

	DeferredAction::~DeferredAction() {
		m_pHandler = NIL;
		m_value.vPtr = NIL;
		m_action = 0;
	}	

} // namespace Cat
