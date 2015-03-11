#include <CoreServices/CoreServices.h>
#include "core/time/osx/abstime.h"

namespace Cat {

	/* Initialize the Static TimeBaseInfo for conversions */
	mach_timebase_info_data_t xInitTimeBaseInfo() {
		mach_timebase_info_data_t timeBaseInfo;
		mach_timebase_info(&timeBaseInfo);
		return timeBaseInfo;
	}
	mach_timebase_info_data_t AbsTime::s_xTimeBaseInfo = xInitTimeBaseInfo();

#if defined (DEBUG)
	std::ostream& operator<<(std::ostream& in_out, const AbsTime& in_t) {
		return in_out << in_t.nano();
	}
#endif // DEBUG
	
} // namespace Cat
