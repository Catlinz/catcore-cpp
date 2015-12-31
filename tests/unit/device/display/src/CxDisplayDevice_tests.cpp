#include "core/CxTestCore.h"
#include "device/display/CxDisplayDevice.h"
#include "core/common/CxUtf8.h"

namespace cat {

	/**
	 * @tests CxDisplayDevice()
	 * @tests CxDisplayDevice(const CxDisplayDevice &)
	 * @tests CxDisplayDevice(CxDisplayDevice &&)
	 * @tests operator=(const CxDisplayDevice &)
	 * @tests operator=(CxDisplayDevice &&)
	 * @tests ~CxDisplayDevice()
	 * @tests name() const
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

		/* Test move constructor on null objects */
		CxDisplayDevice d1(static_cast<CxDisplayDevice &&>(d));
		ass_false(d1.isPrimaryDisplay());
		ass_zero(d1.name());

		/* Test move-assignment on null objects */
		d1 = CxDisplayDevice();
		ass_false(d1.isPrimaryDisplay());
		ass_zero(d1.name());

		/* Test destructor on null objects */
		CxDisplayDevice *dp = new CxDisplayDevice();
		delete dp;
		dp = 0;

		FINISH_TEST;
	}

	/**
	 * @tests primaryDisplay()
	 * @tests isPrimaryDisplay() const
	 */
	void testCxDisplayDevicePrimaryDisplay() {
		BEGIN_TEST;

		CxDisplayDevice d = CxDisplayDevice::primaryDisplay();
		ass_true(d.name() != 0);
		ass_true(utf8::bytes(d.name()) > 0);
		ass_true(d.isPrimaryDisplay());

		CxDisplayDevice d0;

		ass_true(d == d);
		ass_false(d != d);
		ass_false(d == d0 || d0 == d);
		ass_true(d != d0 && d0 != d);
		
		FINISH_TEST;
	}

	/**
	 * @tests getAllDisplayDevices(CxVector<CxDisplayDevice> &)
	 */
	void testCxDisplayDeviceGetAllDisplayDevices() {
		BEGIN_TEST;

		CxDisplayDevice d = CxDisplayDevice::primaryDisplay();

		CxVector<CxDisplayDevice> devices;
		CxDisplayDevice::getAllDisplayDevices(devices);

		/* Make sure there is exactly one primary display */
		ass_true(devices.count() > 0);
		CxBool primary_display_count = 0;
		for (CxI32 i = 0; i < devices.count(); ++i) {
			if (devices[i] == d && d == devices[i]) { ++primary_display_count; }
		}
		ass_eq(primary_display_count, 1);

		/* Make sure none of the displays are equal to each other */
		for(CxI32 i = 0; i < devices.count(); ++i) {
			const CxDisplayDevice &di = devices[i];
			for (CxI32 j = i+1; j < devices.count(); ++j) {
				const CxDisplayDevice &dj = devices[j];
				ass_true(di != dj && dj != di);
				ass_false(di == dj || dj == di);
			}
		}
		
		FINISH_TEST;
	}

	/**
	 * @tests getAllDisplayDevices(CxVector<CxDisplayDevice> &)
	 * @tests currentDisplayMode()
	 * @tests fetchCurrentDisplayMode()
	 */
	void testCxDisplayDeviceCurrentDisplayMode() {
		BEGIN_TEST;

		/* Test the current display mode for the primary display */
		CxDisplayDevice d = CxDisplayDevice::primaryDisplay();
		CxDisplayMode m = d.currentDisplayMode();
		ass_gt(m.horizRes(), 0);  ass_gt(m.vertRes(), 0);  ass_eq(m.depth(), 32);
		ass_true(m.isCurrentDisplayMode());  ass_true(m.isValid());

		/* Test the current display mode for the other displays */
		CxVector<CxDisplayDevice> devices;
		CxDisplayDevice::getAllDisplayDevices(devices);
		
		/* Make sure all the displays have a valid display mode. */
		for(CxI32 i = 0; i < devices.count(); ++i) {
			CxDisplayDevice &di = devices[i];
			CxDisplayMode mi = di.currentDisplayMode();
			ass_gt(mi.horizRes(), 0);  ass_gt(mi.vertRes(), 0);  ass_eq(mi.depth(), 32);
			ass_true(mi.isCurrentDisplayMode());  ass_true(mi.isValid());
		}
		
		FINISH_TEST;
	}

	/**
	 * @tests getAllDisplayDevices(CxVector<CxDisplayDevice> &)
	 * @tests currentDisplayMode()
	 * @tests getDisplayModes()
	 * @tests fetchAvailableDisplayModes()
	 */
	void testCxDisplayDeviceGetDisplayModes() {
		BEGIN_TEST;

		/* Test getting all the display modes for the primary display */
		CxDisplayDevice d = CxDisplayDevice::primaryDisplay();
		const CxVector<CxDisplayMode> &modes = d.getDisplayModes();
		ass_true(modes.count() > 0);

		/* Make sure the current display mode is in the display modes */
		CxDisplayMode m = d.currentDisplayMode();
		CxI32 cur_count = 0;
		for (CxI32 i = 0; i < modes.count(); ++i) {
			ass_true(modes[i].isValid());
			if (m == modes[i]) { ++cur_count; }
		}
		ass_eq(cur_count, 1);

		/* Test getting all the display modes for the other displays */
		CxVector<CxDisplayDevice> devices;
		CxDisplayDevice::getAllDisplayDevices(devices);
		
		for(CxI32 i = 0; i < devices.count(); ++i) {
			CxDisplayDevice &di = devices[i];
			const CxVector<CxDisplayMode> &mis = di.getDisplayModes();
			ass_true(mis.count() > 0);
			
			/* Make sure the current display mode is in the display modes */
			CxDisplayMode mi = di.currentDisplayMode();
			cur_count = 0;
			for (CxI32 i = 0; i < mis.count(); ++i) {
				ass_true(mis[i].isValid());
				if (mi == mis[i]) { ++cur_count; }
			}
			ass_eq(cur_count, 1);
		}
		
		FINISH_TEST;
	}

	/**
	 * @tests getClosestDisplayMode(const CxDisplayMode &)
	 */
	void testCxDisplayDeviceGetClosestDisplayMode() {
		BEGIN_TEST;

		CxDisplayDevice d = CxDisplayDevice::primaryDisplay();
		CxDisplayMode m = d.currentDisplayMode();

		CxDisplayMode m0 = d.getClosestDisplayMode(m);
		ass_true(m0.matchesAll(m));

		CxDisplayMode m1 = d.getClosestDisplayMode(CxDisplayMode(m.horizRes(), m.vertRes(), m.depth(), 30));
		ass_true(m1.matchesResAndDepth(m));

		CxDisplayMode m2 = d.getClosestDisplayMode(CxDisplayMode(m.horizRes() - 1, m.vertRes() - 1, m.depth(), m.refreshRate()));
		ass_true(m2.matchesAll(m));

		FINISH_TEST;
	}

	/**
	 * @tests getClosestDisplayModeLessOrEq(const CxDisplayMode &)
	 */
	void testCxDisplayDeviceGetClosestDisplayModeLessOrEq() {
		BEGIN_TEST;

		CxDisplayDevice d = CxDisplayDevice::primaryDisplay();
		CxDisplayMode m = d.currentDisplayMode();

		CxDisplayMode m0 = d.getClosestDisplayModeLessOrEq(m);
		ass_true(m0.matchesAll(m));

		CxDisplayMode m1 = d.getClosestDisplayModeLessOrEq(CxDisplayMode(m.horizRes(), m.vertRes(), m.depth(), 30));
		ass_true(m1.matchesResAndDepth(m));

		CxDisplayMode m2 = d.getClosestDisplayModeLessOrEq(CxDisplayMode(m.horizRes() - 1, m.vertRes() - 1, m.depth(), m.refreshRate()));
	   ass_true(m2.horizRes() < m.horizRes() && m2.vertRes() < m.vertRes() && m2.depth() == m.depth());

		FINISH_TEST;
	}
	
} // namespace cat

int main(int argc, char **argv) {
	cat::testCxDisplayDeviceCreateAndDestroy();
	cat::testCxDisplayDevicePrimaryDisplay();
	cat::testCxDisplayDeviceGetAllDisplayDevices();
	cat::testCxDisplayDeviceCurrentDisplayMode();
	cat::testCxDisplayDeviceGetDisplayModes();
	cat::testCxDisplayDeviceGetClosestDisplayMode();
	cat::testCxDisplayDeviceGetClosestDisplayModeLessOrEq();
	return 0;
}
