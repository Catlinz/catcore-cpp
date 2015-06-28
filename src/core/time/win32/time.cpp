#include "core/time/win32/time.h"

namespace Cat {

	U32 Time::fracMicro() const {
		I32 t_sec = m_time / s_xFreq;
		I64 t_micro = micro();
		I64 val = t_micro - (t_sec * MICRO_PER_SEC);
		if (val < 0) {
			val += MICRO_PER_SEC;
		}
		return (U32)val;
	}

	U32 Time::fracMilli() const {
		I32 t_sec = m_time / s_xFreq;
		I64 t_milli = milli();
		I64 val = t_milli - (t_sec * MILLI_PER_SEC);
		if (val < 0) {
			val += MILLI_PER_SEC;
		}
		return (U32)val;
	}

	U32 Time::fracNano() const {
		I32 t_sec = m_time / s_xFreq;
		I64 t_nano = nano();
		I64 val = t_nano - (t_sec * NANO_PER_SEC);
		if (val < 0) {
			val += NANO_PER_SEC;
		}
		return (U32)val;
	}

	F32 Time::fracSec() const {
		I32 t_sec = m_time / s_xFreq;
		F32 val = sec() - (F32)t_sec;
		if (val < 0) {
			val += 1.0;
		}
		return val;
	}

	F64 Time::fracSec64() const {
		I32 t_sec = m_time / s_xFreq;
		F64 val = sec64() - (F64)t_sec;
		if (val < 0) {
			val += 1.0;
		}
		return val;
	}
} // namespace Cat
