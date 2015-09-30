#include "core/time/CxTime.h"
#include "core/CxTestCore.h"

namespace cat {

	/**
	 * @tests CxTime()
	 * @tests CxTime(const CxTime &in_src)
	 * @tests CxTime(const CxAbsTime &in_src)
	 * @tests operator=(const CxTime &)
	 * @tests operator=(const CxAbsTime &)
	 */
	void testCxTimeCreateCopyAssign() {
		BEGIN_TEST;

		CxTime t0;
		t0.setMilli(1000);
		ass_eq(t0.milli(), 1000);

		CxTime t1(t0);
		ass_eq(t1.milli(), 1000);

		t1 = CxTime::fromMilli(2000);
		ass_eq(t1.milli(), 2000);

		t0 = t1;
		ass_eq(t0.milli(), 2000);

		CxAbsTime t2;
		t2.setMilli(3000);

		CxTime t3(t2);
		ass_eq(t3.milli(), 3000);

		t0 = t2;
		ass_eq(t2.milli(), 3000);

		FINISH_TEST;
	}

	/**
	 * @tests operator==(const CxTime &) const
	 * @tests operator!=(const CxTime &) const
	 */
	void testCxTimeComparison() {
		BEGIN_TEST;

		CxTime t0 = CxTime::fromMilli(1000);
		CxTime t1 = CxTime::fromMilli(1000);
		CxTime t2 = CxTime::fromMilli(3000);

		ass_true(t1 == t0 && t0 == t1);
		ass_false(t1 != t0 || t0 != t1);
		ass_false(t1 == t2 || t2 == t1);
		ass_true(t1 != t2 && t2 != t1);

		ass_true(t1 < t2);
		ass_false(t2 < t1);
		ass_false(t0 < t1);
		ass_false(t1 < t0);
		
		ass_true(t1 <= t2);
		ass_false(t2 <= t1);
		ass_true(t0 <= t1);
		ass_true(t1 <= t0);
		
		ass_true(t2 > t1);
		ass_false(t1 > t2);
		ass_false(t1 > t0);
		ass_false(t0 > t1);

		ass_true(t2 >= t1);
		ass_false(t1 >= t2);
		ass_true(t1 >= t0);
		ass_true(t0 >= t1);

		FINISH_TEST;
	}

	/**
	 * @tests operator+(const CxTime &) const
	 * @tests operator-(const CxTime &) const
	 * @tests operator+=(const CxTime &)
	 * @tests operator-=(const CxTime &)
	 */
	void testCxTimeArithmetic() {
		BEGIN_TEST;

		CxTime t0 = CxTime::fromMilli(1000);
		CxTime t1 = CxTime::fromMilli(2000);
		CxTime t2 = CxTime::fromMilli(-3000);

		CxTime t3 = t0 + t1;
		ass_eq(t3.milli(), 3000);

		CxTime t4 = t3 + t2;
		ass_eq(t4.milli(), 0);

		t4 = t1 - t0;
		ass_eq(t4.milli(), 1000);

		t4 = t0 - t1;
		ass_eq(t4.milli(), -1000);

		t4 = t0 - t2;
		ass_eq(t4.milli(), 4000);

		t4 = t3 + t2;
		ass_eq(t4.milli(), 0);

		t4 += t1;
		ass_eq(t4.milli(), 2000);

		t4 += t2;
		ass_eq(t4.milli(), -1000);

		t4 -= t2;
		ass_eq(t4.milli(), 2000);

		t4 += t2;
		ass_eq(t4.milli(), -1000);

		t4 -= t2;
		ass_eq(t4.milli(), 2000);

		t4 += t0;
		ass_eq(t4.milli(), 3000);
		
		FINISH_TEST;
	}

	/**
	 * @tests current()
	 * @tests currentMicro()
	 * @tests currentMilli()
	 * @tests currentNano()
	 * @tests currentSec32()
	 * @tests currentSec64()
	 */
	void testCxTimeCurrent() {
		BEGIN_TEST;

		CxTime t0 = CxTime::current();
		ass_true(t0.milli() != 0);

		CxU64 t1 = CxTime::currentMicro();
		ass_true(t0.micro() <= t1);

		CxU64 t2 = CxTime::currentMilli();
		ass_true(t0.milli() <= t2);

		CxU64 t3 = CxTime::currentNano();
		ass_true(t0.nano() <= t3);

		CxF32 t4 = CxTime::currentSec32();
		ass_true(t0.sec32() <= t4);

		CxF64 t5 = CxTime::currentSec64();
		ass_true(t0.sec64() <= t5);

		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxTimeCreateCopyAssign();
	cat::testCxTimeComparison();
	cat::testCxTimeArithmetic();
	cat::testCxTimeCurrent();
	
	return 0;
}
