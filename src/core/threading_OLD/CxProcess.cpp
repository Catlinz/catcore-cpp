#include "core/threading/CxProcess.h"
#include "core/common/CxStr.h"


namespace cat {

	CxProcess::CxProcess(const CxChar *in_name)
		: m_pid(0), mp_name(0), m_priority(1), m_priorityModifier(1),
		  m_state(CxProcess::kNotStarted) {
		mp_name = str::copy(in_name);
		if (in_name) {
			m_pid = hash::crc32(name);
		}
	}

	CxProcess::~CxProcess() {
		str::free(mp_name);
	}

	void CxProcess::initialize() { m_state = kRunning; }
	
} // namespace cat
