#include "engine/events/CxInputEvent.h"
#include "core/common/CxMem.h"

namespace cat {

	CxInputEvent::CxInputEvent() : m_type(0), m_mods(0) {
		mem::set(m_event, 0, sizeof(m_event));
	}

} // namespace cat
