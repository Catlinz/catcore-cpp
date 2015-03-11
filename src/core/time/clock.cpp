#include "core/time/clock.h"

namespace Cat {

	Clock Clock::s_global;

	Clock::Clock() {
		m_tNano = m_teNano = 0;
		m_tSec = m_teSec = 0.0;
	}

	Clock::Clock(const AbsTime &in_time) {
		initialise(in_time);
	}

	void Clock::initialise(const AbsTime& in_time) {
		m_teNano = 0;
		m_teSec = 0.0;
		m_tNano = in_time.nano();
		m_tSec = m_tNano / (F64)NANO_PER_SEC;
	}

	void Clock::set(const AbsTime& in_time) {
	   U64 new_time = in_time.nano();
		m_teNano = new_time - m_tNano;
		m_teSec = m_teNano / (F64)NANO_PER_SEC;
		m_tNano = new_time;
		m_tSec = new_time / (F64)NANO_PER_SEC;
	}
	
} // namespace Cat
