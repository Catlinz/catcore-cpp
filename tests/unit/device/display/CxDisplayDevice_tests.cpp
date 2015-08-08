#include "core/CxTestCore.h"
#include "device/display/CxDisplayDevice.h"

namespace cat {

	/**
	 * @tests CxDisplayDevice()
	 * @tests CxDisplayDevice(const CxDisplayDevice &)
	 * @tests operator=(const CxDisplayDevice &)
	 * @tests ~CxDisplayDevice()
	 */
	void testCxDisplayDeviceCreateAndDestroy() {
		BEGIN_TEST;

		/* Test the default constructor */
		CxDisplayDevice d;
		ass_false(d.isPrimaryDisplay());
		ass_zero(d.name());

		/* Test the copy constructor on null objects */
		CxDisplayDevice dc(d);
		ass_false(dc.isPrimaryDisplay());
		ass_zero(dc.name());

		/* Test assignment operator on null objects */
		CxDisplayDevice da(d);
		ass_false(da.isPrimaryDisplay());
		ass_zero(da.name());

		/* Test destructor on null objects */
		CxDisplayDevice *dp = new CxDisplayDevice();
		delete dp;
		dp = 0;

		FINISH_TEST;
	}

	void testCxDisplayDeviceCurrentDisplayMode() {
		BEGIN_TEST;

		CxDisplayDevice d;
		CxDisplayMode m = d.currentDisplayMode();
		ass_gt(m.horizRes(), 0);  ass_gt(m.vertRes(), 0);  ass_eq(m.depth(), 32);
		ass_true(m.isCurrentDisplayMode());  ass_true(m.isValid());
		
		FINISH_TEST;
	}
	
} // namespace cat

int main(int argc, char **argv) {
	cat::testCxDisplayDeviceCreateAndDestroy();
	cat::testCxDisplayDeviceCurrentDisplayMode();
	
	return 0;
}
