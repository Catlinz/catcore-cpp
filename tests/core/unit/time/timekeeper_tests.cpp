#include "core/testcore.h"
#include "core/time/timekeeper.h"
#include <iomanip>

namespace cc {

	void testTimeKeeperCreateAndDestroy() {
		BEGIN_TEST;

		TimeKeeper t;
		ass_eq(t.accum(), 0);
		ass_eq(t.accumNano(), 0);
		ass_eq(t.accumSeconds(), 0);
		ass_gt(t.stepSize(), 0);		
		ass_eq(t.stepSizeNano(), 16666667);
		ass_eq(t.stepSizeSeconds(), REAL(0.016666667));


		TimeKeeper t2(10000000);
		ass_eq(t2.accum(), 0);
		ass_eq(t2.accumNano(), 0);
		ass_eq(t2.accumSeconds(), 0);
		ass_gt(t2.stepSize(), 0);		
		ass_eq(t2.stepSizeNano(), 10000000);
		ass_eq(t2.stepSizeSeconds(), REAL(0.01));

		ass_gt(t.stepSize(), t2.stepSize());	
		
		FINISH_TEST;
	}

	void testTimeKeeperTick() {
		BEGIN_TEST;

		TimeKeeper t(10000000);
		ass_eq(t.accum(), 0);
		ass_eq(t.accumNano(), 0);
		ass_eq(t.accumSeconds(), 0);
		ass_gt(t.stepSize(), 0);		
		ass_eq(t.stepSizeNano(), 10000000);
		ass_eq(t.stepSizeSeconds(), REAL(0.01));

		t.tick();
		ass_gt(t.accum(), 0);
		ass_gt(t.accumNano(), 0);
		ass_gt(t.accumSeconds(), 0);
		ass_lt(t.accumSeconds(), 1);
		t.setAccumZero();
		ass_eq(t.accum(), 0);
		usleep(10000);

		t.tick();
		ass_gt(t.accum(), t.stepSize());
		ass_gt(t.accumNano(), t.stepSizeNano());
		ass_gt(t.accumSeconds(), t.stepSizeSeconds());		
		t.tick();
		U64 millis = t.accumNano();	
		usleep(1);
	   t.tick();
		ass_gt(t.accumNano(), millis);		
		
		FINISH_TEST;
	}

	void testTimeKeeperConsumeSingleTime() {
		BEGIN_TEST;

		TimeKeeper t(10000000);
		ass_eq(t.accum(), 0);
		ass_eq(t.accumNano(), 0);
		ass_eq(t.accumSeconds(), 0);
		ass_gt(t.stepSize(), 0);		
		ass_eq(t.stepSizeNano(), 10000000);
		ass_eq(t.stepSizeSeconds(), REAL(0.01));

		t.tick();
		t.setAccumZero();
		usleep(10001);

		t.tick();
		ass_gt(t.accum(), t.stepSize());
		U64 millis = t.consumeSingleNano();
		ass_eq(millis, t.stepSizeNano());
		ass_gt(t.accum(), 0);
		ass_lt(t.accum(), t.stepSize());

		millis = t.consumeSingleNano();
		ass_eq(millis, 0);
		ass_gt(t.accum(), 0);
		ass_lt(t.accum(), t.stepSize());

		t.tick();
		t.setAccumZero();
		usleep(5000);
		t.tick();		
		ass_gt(t.accum(), 0);
		ass_lt(t.accum(), t.stepSize());
		millis = t.accumNano();

		U64 millis2 = t.consumeSingleNano();
		ass_eq(millis2, 0);
		ass_eq(millis, t.accumNano());

		usleep(5000);
		t.tick();		
		ass_gt(t.accum(), t.stepSize());
		millis = t.consumeSingleNano();
		ass_eq(millis, t.stepSizeNano());
		ass_gt(t.accum(), 0);
		ass_lt(t.accum(), t.stepSize());

		millis = t.consumeSingleNano();
		ass_eq(millis, 0);
		ass_gt(t.accum(), 0);
		ass_lt(t.accum(), t.stepSize());

		/* Test seconds */
		t.setAccumZero();
		ass_eq(t.accumSeconds(), 0);
		usleep(10000);
		t.tick();		
		ass_gt(t.accumSeconds(), t.stepSizeSeconds());
		Real seconds = t.consumeSingleSeconds();
		ass_eq(seconds, t.stepSizeSeconds());
		ass_lt(t.accumSeconds(), t.stepSizeSeconds());
		ass_gt(t.accumSeconds(), 0);

		seconds = t.consumeSingleSeconds();
		ass_eq(seconds, 0);
		ass_lt(t.accumSeconds(), t.stepSizeSeconds());
		ass_gt(t.accumSeconds(), 0);
		
		
		
		FINISH_TEST;
	}


	void testTimeKeeperConsumeAllTime() {
		BEGIN_TEST;

		TimeKeeper t(10000000);
		ass_eq(t.accum(), 0);
		ass_eq(t.accumNano(), 0);
		ass_eq(t.accumSeconds(), 0);
		ass_gt(t.stepSize(), 0);		
		ass_eq(t.stepSizeNano(), 10000000);
		ass_eq(t.stepSizeSeconds(), REAL(0.01));

		t.tick();
		t.setAccumZero();
		usleep(30003);
		t.tick();
		
		U64 millis = t.consumeAllNano();
		ass_gt(t.accum(), 0);
		ass_lt(t.accum(), t.stepSize());

		ass_eq(millis, t.stepSizeNano()*3);

		millis = t.consumeAllNano();
		ass_eq(millis, 0);
		ass_gt(t.accum(), 0);
		ass_lt(t.accum(), t.stepSize());

		usleep(10000);
		t.tick();		

		millis = t.consumeAllNano();
		ass_gt(t.accum(), 0);
		ass_lt(t.accum(), t.stepSize());

		ass_eq(millis, t.stepSizeNano()*1);

		millis = t.consumeAllNano();
		ass_eq(millis, 0);
		ass_gt(t.accum(), 0);
		ass_lt(t.accum(), t.stepSize());

		/* Test seconds */
		t.setAccumZero();
		ass_eq(t.accumSeconds(), 0);
		usleep(40000);
		t.tick();		
		ass_gt(t.accumSeconds(), t.stepSizeSeconds());
		Real seconds = t.consumeAllSeconds();
		ass_eq(seconds, t.stepSizeSeconds() * 4);
		ass_lt(t.accumSeconds(), t.stepSizeSeconds());
		ass_gt(t.accumSeconds(), 0);

		seconds = t.consumeAllSeconds();
		ass_eq(seconds, 0);
		ass_lt(t.accumSeconds(), t.stepSizeSeconds());
		ass_gt(t.accumSeconds(), 0);
		
		FINISH_TEST;
	}

	void testTimeKeeperGetFutureTime() {
		BEGIN_TEST;

		TimeKeeper t(10000000);
		ass_eq(t.accum(), 0);
		ass_eq(t.accumNano(), 0);
		ass_eq(t.accumSeconds(), 0);
		ass_gt(t.stepSize(), 0);		
		ass_eq(t.stepSizeNano(), 10000000);
		ass_eq(t.stepSizeSeconds(), REAL(0.01));

		RawTimeVal val = t.getFutureTime(10000000);	
		t.tick();
		ass_gt(val, t.lastTick());
		RawTimeVal val2 = t.getFutureTime(100000);	
		ass_gt(val, val2);
		ass_gt(val2, t.lastTick());
		t.tick();
		val = t.getFutureTime(1);
		ass_gt(val, t.lastTick());

		U64 valu = t.getFutureTimeNano(10000000);	
		t.tick();
		ass_gt(valu, t.lastTickNano());
		U64 valu2 = t.getFutureTimeNano(100000);	
		ass_gt(valu, valu2);
		ass_gt(val2, t.lastTickNano());
		t.tick();
		val = t.getFutureTimeNano(1);
		ass_gt(valu, t.lastTickNano());

		FINISH_TEST;
	}

	void testTimeKeeperHasTimeElapsed() {
		BEGIN_TEST;

		TimeKeeper t(10000000);
		ass_eq(t.accum(), 0);
		ass_eq(t.accumNano(), 0);
		ass_eq(t.accumSeconds(), 0);
		ass_gt(t.stepSize(), 0);		
		ass_eq(t.stepSizeNano(), 10000000);
		ass_eq(t.stepSizeSeconds(), REAL(0.01));

		RawTimeVal val = t.getFutureTime(10000000);	
		usleep(10000);
		ass_true(t.hasTimeElapsed(val));

		val = t.getFutureTime(10000000);	
		usleep(5000);
		ass_false(t.hasTimeElapsed(val));	
		usleep(5000);
		ass_true(t.hasTimeElapsed(val));
		
		U64 val2 = t.getFutureTimeNano(10000000);	
		usleep(10000);
		ass_true(t.hasTimeElapsedNano(val2));

		val2 = t.getFutureTimeNano(10000000);	
		usleep(5000);
		ass_false(t.hasTimeElapsedNano(val2));	
		usleep(5000);
		ass_true(t.hasTimeElapsedNano(val2));
		
		FINISH_TEST;
	}
}

int main(int argc, char** argv) {
	cc::testTimeKeeperCreateAndDestroy();
	cc::testTimeKeeperTick();
	cc::testTimeKeeperConsumeSingleTime();
	cc::testTimeKeeperConsumeAllTime();
	cc::testTimeKeeperGetFutureTime();
	cc::testTimeKeeperHasTimeElapsed();	
	return 0;
}


