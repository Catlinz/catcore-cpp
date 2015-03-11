#include "core/testcore.h"
#include "core/time/time.h"
#include <iomanip>

namespace cc {


	void testTimeValCreateAndDestroy() {
		BEGIN_TEST;

		TimeVal t1;
		ass_eq(t1.nanoseconds(), 0);
		ass_eq(t1.seconds(), 0);
		ass_eq(t1.seconds64(), 0);

		TimeVal t2 = Time::currentTime();
		ass_gt(t2.nanoseconds(), 0);
		ass_gt(t2.seconds(), 0);
		ass_gt(t2.seconds64(), 0);
		
		FINISH_TEST;
	}

	void testTimeCompare() {
		BEGIN_TEST;

		RawTimeVal r1 = Time::currentTimeRaw();
		usleep(10);
		RawTimeVal r2 = Time::currentTimeRaw();

		ass_lt(Time::compareRaw(r1, r2), 0);
		ass_gt(Time::compareRaw(r2, r1), 0);
		ass_eq(Time::compareRaw(r1, r1), 0);
		ass_eq(Time::compareRaw(r2, r2), 0);

		U64 u1 = Time::currentTimeNano();
		usleep(10);
		U64 u2 = Time::currentTimeNano();

		ass_lt(Time::compareNano(u1, u2), 0);
		ass_gt(Time::compareNano(u2, u1), 0);
		ass_eq(Time::compareNano(u1, u1), 0);
		ass_eq(Time::compareNano(u2, u2), 0);

		F64 s1 = Time::currentTimeSeconds64();
		usleep(1000000);
		F64 s2 = Time::currentTimeSeconds64();

		ass_lt(Time::compareNano(s1, s2), 0);
		ass_gt(Time::compareNano(s2, s1), 0);
		ass_eq(Time::compareNano(s1, s1), 0);
		ass_eq(Time::compareNano(s2, s2), 0);

		FINISH_TEST;
	}
	


}

int main(int argc, char** argv) {
	
	cc::testTimeValCreateAndDestroy();
	cc::testTimeCompare();	
   
	return 0;
}


