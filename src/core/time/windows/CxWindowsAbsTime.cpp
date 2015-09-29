#include "core/time/windows/CxWindowsAbsTime.h"

namespace cat {

	/* Initialise the static frequncy for QueryPerformanceCounter */
	LARGE_INTEGER _x_CxAbsTimeInitQPCFrequency() {
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		return f;
	}

	LARGE_INTEGER CxAbsTime::s_freq = _x_CxAbsTimeInitQPCFrequency();

} // namespace cat
