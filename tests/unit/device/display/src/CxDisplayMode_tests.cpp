#include "core/CxTestCore.h"
#include "device/display/CxDisplayMode.h"

namespace cat {

	void populateModesForTest(CxVector<CxDisplayMode> &inout_modes) {
		inout_modes << CxDisplayMode(1280, 800, 16, 100);
		inout_modes << CxDisplayMode(1280, 960, 16, 100);
		inout_modes << CxDisplayMode(1280, 1024, 16, 100);
		inout_modes << CxDisplayMode(1600, 900, 16, 100);
		inout_modes << CxDisplayMode(1600, 1024, 16, 100);
		inout_modes << CxDisplayMode(1680, 1050, 16, 100);
		inout_modes << CxDisplayMode(1768, 992, 16, 100);
		inout_modes << CxDisplayMode(1920, 1080, 16, 100);
		
		inout_modes << CxDisplayMode(1280, 800, 32, 59);
		inout_modes << CxDisplayMode(1280, 800, 32, 60);
		inout_modes << CxDisplayMode(1280, 960, 32, 59);
		inout_modes << CxDisplayMode(1280, 960, 32, 60);
		inout_modes << CxDisplayMode(1280, 1024, 32, 59);
		inout_modes << CxDisplayMode(1280, 1024, 32, 60);
		
		inout_modes << CxDisplayMode(1600, 900, 32, 59);
		inout_modes << CxDisplayMode(1600, 900, 32, 60);
		inout_modes << CxDisplayMode(1600, 1024, 32, 59);
		inout_modes << CxDisplayMode(1600, 1024, 32, 60);

		inout_modes << CxDisplayMode(1680, 1050, 32, 59);
		inout_modes << CxDisplayMode(1680, 1050, 32, 60);

		inout_modes << CxDisplayMode(1768, 992, 32, 59);
		inout_modes << CxDisplayMode(1768, 992, 32, 60);

		inout_modes << CxDisplayMode(1920, 1080, 32, 59);
		inout_modes << CxDisplayMode(1920, 1080, 32, 60);
	}

	/**
	 * @tests CxDisplayMode()
	 * @tests CxDisplayMode(CxI32, CxI32, CxI32, CxI32, CxI32)
	 * @tests operator==(const CxDisplayMode &)
	 * @tests operator!=(const CxDisplayMode &)
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

	/**
	 * @tests operator==(const CxDisplayMode &) const
	 * @tests operator!=(const CxDisplayMode &) const
	 */
	void testCxDisplayModeEquality() {
		BEGIN_TEST;

		CxDisplayMode d0(1024, 768, 16, 60, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d1(1024, 768, 16, 60, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d2(1024, 768, 16, 60, 0);
		CxDisplayMode d3(1024, 768, 16, 59, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d4(1024, 768, 32, 60, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d5(1024, 763, 16, 60, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d6(1023, 768, 16, 60, CxDisplayMode::kIsCurrentDisplayMode);

		ass_true(d0 == d0 && !(d0 != d0));
		ass_true(d0 == d1 && d1 == d0);
		ass_false(d0 == d2 && d2 == d0);
		ass_false(d0 == d3 && d3 == d0);
		ass_false(d0 == d4 && d4 == d0);
		ass_false(d0 == d5 && d5 == d0);
		ass_false(d0 == d6 && d6 == d0);
		
		FINISH_TEST;
	}

	/**
	 * @tests matchesAll(const CxDisplayMode &) const
	 * @tests matchesResAndDepth(const CxDisplayMode &) const
	 * @tests matchesRes(const CxDisplayMode &) const
	 */
	void testCxDisplayModeMatches() {
		BEGIN_TEST;

		CxDisplayMode d0(1024, 768, 16, 60, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d1(1024, 768, 16, 60, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d2(1024, 768, 16, 60, 0);
		CxDisplayMode d3(1024, 768, 16, 59, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d4(1024, 768, 32, 60, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d5(1024, 763, 16, 60, CxDisplayMode::kIsCurrentDisplayMode);
		CxDisplayMode d6(1023, 768, 16, 60, CxDisplayMode::kIsCurrentDisplayMode);

		ass_true(d0.matchesAll(d0));
		ass_true(d0.matchesAll(d1) && d1.matchesAll(d0));
		ass_true(d0.matchesAll(d2) && d2.matchesAll(d0));
		ass_false(d0.matchesAll(d3) || d3.matchesAll(d0));
		ass_false(d0.matchesAll(d4) || d4.matchesAll(d0));
		ass_false(d0.matchesAll(d5) || d5.matchesAll(d0));
		ass_false(d0.matchesAll(d6) || d6.matchesAll(d0));

		ass_true(d0.matchesResAndDepth(d0));
		ass_true(d0.matchesResAndDepth(d1) && d1.matchesResAndDepth(d0));
		ass_true(d0.matchesResAndDepth(d2) && d2.matchesResAndDepth(d0));
		ass_true(d0.matchesResAndDepth(d3) && d3.matchesResAndDepth(d0));
		ass_false(d0.matchesResAndDepth(d4) || d4.matchesResAndDepth(d0));
		ass_false(d0.matchesResAndDepth(d5) || d5.matchesResAndDepth(d0));
		ass_false(d0.matchesResAndDepth(d6) || d6.matchesResAndDepth(d0));

		ass_true(d0.matchesRes(d0));
		ass_true(d0.matchesRes(d1) && d1.matchesRes(d0));
		ass_true(d0.matchesRes(d2) && d2.matchesRes(d0));
		ass_true(d0.matchesRes(d3) && d3.matchesRes(d0));
		ass_true(d0.matchesRes(d4) && d4.matchesRes(d0));
		ass_false(d0.matchesRes(d5) || d5.matchesRes(d0));
		ass_false(d0.matchesRes(d6) || d6.matchesRes(d0));
		
		FINISH_TEST;
	}

	/**
	 * @tests getBestMatch(const CxDisplayMode &, const CxVector<CxDisplayMode> &)
	 */
	void testCxDisplayModeGetBestMatch() {
		BEGIN_TEST;

		CxVector<CxDisplayMode> modes(32);
		populateModesForTest(modes);

		CxDisplayMode m0(1920, 1080, 32, 60);
		CxDisplayMode m1(1768, 992, 32, 60);
		CxDisplayMode m2(1600, 1024, 32, 60);
		CxDisplayMode m2b(1280, 1024, 32, 40);

		CxDisplayMode m3(1900, 1055, 31);
		CxDisplayMode m4(1700, 900, 30);
		CxDisplayMode m5(1200, 1080, 33);

		CxDisplayMode m6(1900, 1055, 15);
		CxDisplayMode m7(1700, 900, 12);
		CxDisplayMode m8(1200, 1080, 20);

	   CxDisplayMode m = CxDisplayMode::getBestMatch(m0, modes);
		ass_true(m.matchesAll(m0));

		m = CxDisplayMode::getBestMatch(m1, modes);
		ass_true(m.matchesAll(m1));

		m = CxDisplayMode::getBestMatch(m2, modes);
		ass_true(m.matchesAll(m2));

		m = CxDisplayMode::getBestMatch(m2b, modes);
		ass_true(m.matchesResAndDepth(m2b) && m.refreshRate() == 60);



		m = CxDisplayMode::getBestMatch(m3, modes);
		ass_true(m.matchesAll(m0));

		m = CxDisplayMode::getBestMatch(m4, modes);
		ass_true(m.matchesAll(m1));

		m = CxDisplayMode::getBestMatch(m5, modes);
		ass_true(m.matchesResAndDepth(m2b) && m.refreshRate() == 60);



		m = CxDisplayMode::getBestMatch(m6, modes);
		ass_true(m.matchesRes(m0) && m.depth() == 16 && m.refreshRate() == 100);

		m = CxDisplayMode::getBestMatch(m7, modes);
		ass_true(m.matchesRes(m1) && m.depth() == 16 && m.refreshRate() == 100);

		m = CxDisplayMode::getBestMatch(m8, modes);
		ass_true(m.matchesRes(m2b) && m.depth() == 16 && m.refreshRate() == 100);

		FINISH_TEST;
	}

	/**
	 * @tests getBestMatchLessOrEq(const CxDisplayMode &, const CxVector<CxDisplayMode> &)
	 */
	void testCxDisplayModeGetBestMatchLessOrEq() {
		BEGIN_TEST;

		CxVector<CxDisplayMode> modes(32);
		populateModesForTest(modes);

		CxDisplayMode m0(1920, 1080, 32, 60);
		CxDisplayMode m1(1768, 992, 32, 60);
		CxDisplayMode m2(1600, 1024, 32, 60);
		CxDisplayMode m2b(1280, 1024, 32, 40);

		CxDisplayMode m3(2000, 2000, 33);
		CxDisplayMode m4(1700, 1025, 32);
		CxDisplayMode m5(1700, 1023, 36);

		CxDisplayMode m6(1900, 1055, 31);
		CxDisplayMode m7(1700, 900, 16);
		CxDisplayMode m8(1290, 1080, 20);
		
		CxDisplayMode m9(1200, 1080, 20);
		CxDisplayMode m10(1900, 1055, 15);

	   CxDisplayMode m = CxDisplayMode::getBestMatchLessOrEq(m0, modes);
		ass_true(m.matchesAll(m0));

		m = CxDisplayMode::getBestMatchLessOrEq(m1, modes);
		ass_true(m.matchesAll(m1));

		m = CxDisplayMode::getBestMatchLessOrEq(m2, modes);
		ass_true(m.matchesAll(m2));

		m = CxDisplayMode::getBestMatchLessOrEq(m2b, modes);
		ass_true(m.matchesResAndDepth(m2b) && m.refreshRate() == 60);



		m = CxDisplayMode::getBestMatchLessOrEq(m3, modes);
	   ass_true(m.horizRes() == 1920 && m.vertRes() == 1080 && m.depth() == 32 && m.refreshRate() == 60);

		m = CxDisplayMode::getBestMatchLessOrEq(m4, modes);
	   ass_true(m.horizRes() == 1600 && m.vertRes() == 1024 && m.depth() == 32 && m.refreshRate() == 60);

		m = CxDisplayMode::getBestMatchLessOrEq(m5, modes);
		ass_true(m.horizRes() == 1600 && m.vertRes() == 900 && m.depth() == 32 && m.refreshRate() == 60);



		m = CxDisplayMode::getBestMatchLessOrEq(m6, modes);
		ass_true(m.horizRes() == 1768 && m.vertRes() == 992 && m.depth() == 16 && m.refreshRate() == 100);

		m = CxDisplayMode::getBestMatchLessOrEq(m7, modes);
		ass_true(m.horizRes() == 1600 && m.vertRes() == 900 && m.depth() == 16 && m.refreshRate() == 100);

		m = CxDisplayMode::getBestMatchLessOrEq(m8, modes);
		ass_true(m.horizRes() == 1280 && m.vertRes() == 1024 && m.depth() == 16 && m.refreshRate() == 100);


		
		m = CxDisplayMode::getBestMatchLessOrEq(m9, modes);
		ass_false(m.isValid());
		
		m = CxDisplayMode::getBestMatchLessOrEq(m10, modes);
		ass_false(m.isValid());

		FINISH_TEST;
	}
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxDisplayModeCreateAndDestroy();
	cat::testCxDisplayModeEquality();
	cat::testCxDisplayModeMatches();
	cat::testCxDisplayModeGetBestMatch();
	cat::testCxDisplayModeGetBestMatchLessOrEq();
	return 0;
}

