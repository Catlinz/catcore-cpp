#include "core/CxTestCore.h"
#include "device/display/CxDisplayMode.h"

namespace cat {

	/**
	 * @tests CxDisplayMode()
	 * @tests CxDisplayMode(CxI32, CxI32, CxI32, CxI32, CxI32)
	 * @tests depth() const
	 * @tests horizRes() const
	 * @tests isCurrentDisplayMode() const
	 * @tests isValid() const
	 * @tests refreshRate() const
	 * @tests vertRes() const
	 */
	void testCxDisplayModeCreateAndDestroy() {
		BEGIN_TEST;

		CxDisplayMode d;
		ass_zero(d.horizRes());  ass_zero(d.vertRes());
		ass_zero(d.depth());     ass_zero(d.refreshRate());
		ass_false(d.isCurrentDisplayMode());  ass_false(d.isValid());
	
		CxDisplayMode d2(1024, 768);
		ass_eq(d2.horizRes(), 1024);  ass_eq(d2.vertRes(), 768);
		ass_eq(d2.depth(), 32);     ass_eq(d2.refreshRate(), 0);
		ass_false(d2.isCurrentDisplayMode());  ass_true(d2.isValid());

		d = CxDisplayMode(1024, 768, 16, 60, CxDisplayMode::kIsCurrentDisplayMode);
		ass_eq(d.horizRes(), 1024);  ass_eq(d.vertRes(), 768);
		ass_eq(d.depth(), 16);       ass_eq(d.refreshRate(), 60);
		ass_true(d.isCurrentDisplayMode());  ass_true(d.isValid());

		FINISH_TEST;
	}

} // namespace cat


int main(int argc, char **argv) {
	cat::testCxDisplayModeCreateAndDestroy();
	
	return 0;
}

