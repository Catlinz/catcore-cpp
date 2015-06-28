#include "core/threading/apple/CxAppleSpinlock.h"

namespace cat {

	CxSpinlock::CxSpinlock() { m_spinlock = OS_SPINLOCK_INIT; }

} // namespace cat
