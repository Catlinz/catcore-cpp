#include "core/time/clock.h"

namespace Cat {

	U64 Clock::s_dElapsedNano = 0;
	F64 Clock::s_dElapsedSec = 0.0;
	U64 Clock::s_dTimeNano = 0;
	F64 Clock::s_dTimeSec = 0.0;

	void Clock::initialiseDiscreteTime(U64 p_nanoTime) {
		s_dElapsedNano = 0;
		s_dElapsedSec = 0.0;
		s_dTimeNano = p_nanoTime;
		s_dTimeSec = p_nanoTime / 1000000000.0;
	}

	void Clock::setDiscreteTime(U64 p_nanoTime) {
		s_dElapsedNano = p_nanoTime - s_dTimeNano;
		s_dElapsedSec = s_dElapsedNano / 1000000000.0;
		s_dTimeNano = p_nanoTime;
		s_dTimeSec = p_nanoTime / 1000000000.0;
	}

} // namespace Cat
