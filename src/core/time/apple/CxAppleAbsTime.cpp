#include <CoreServices/CoreServices.h>
#include "core/time/apple/CxAppleAbsTime.h"

namespace cat {

	/* Initialize the Static TimeBaseInfo for conversions */
	mach_timebase_info_data_t xInitTimeBaseInfo() {
		mach_timebase_info_data_t timeBaseInfo;
		mach_timebase_info(&timeBaseInfo);
		return timeBaseInfo;
	}
	mach_timebase_info_data_t CxAbsTime::sx_timebaseInfo = xInitTimeBaseInfo();
	
} // namespace cat
