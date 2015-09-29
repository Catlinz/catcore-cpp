#include "core/time/windows/CxWindowsTime.h"

namespace cat {

	/* Initialise the static frequncy for QueryPerformanceCounter */
	LARGE_INTEGER _x_CxTimeInitQPCFrequency() {
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		return f;
	}

	LARGE_INTEGER CxTime::s_freq = _x_CxTimeInitQPCFrequency();

} // namespace cat
