#include "core/common/CxFlags.h"
#include "core/CxTestCore.h"

#define CXT_0 0
#define CXT_1 (1 << 0)
#define CXT_2 (1 << 1)
#define CXT_3 (1 << 2)
#define CXT_4 (1 << 3)
#define CXT_5 (1 << 4)

namespace cat {

	enum CxTestFlagsEnum {
		kZero = 0, kOne = 1 << 0, kTwo = 1 << 1,
		kThree = 1 << 2, kFour = 1 << 3, kFive = 1 << 4,
		kFourTwo = (1 << 1) | (1 << 3)
	};
	CX_FLAGS(CxTestFlagsEnum, CxTestFlags);

	void testCxFlagsCreate() {
		BEGIN_TEST;

		/* Test the construtors */
		CxTestFlags f0;
		ass_zero(f0.bits());

		CxTestFlags f1(kTwo);
		ass_eq(f1.bits(), kTwo);

		CxTestFlags f2(CXT_2 | CXT_5);
		ass_eq(f2.bits(), CXT_2 | CXT_5);

		CxTestFlags f3(f2);
		ass_eq(f3.bits(), CXT_2 | CXT_5);

		/* Test the assignment operators */
		f3 = f1;
		ass_eq(f3.bits(), kTwo);

		f3 = kFive;
		ass_eq(f3.bits(), kFive);

		f3 = (CXT_4 | CXT_3 | CXT_1);
		ass_eq(f3.bits(), (CXT_4 | CXT_3 | CXT_1));

		/* Test the enum operation creation operators (CX_FLAG_OPS) */
		/*
		CxU32 b0 = (kFour | kFive | kOne).bits();
		ass_eq(b0, (CXT_4 | CXT_5 | CXT_1));

	   CxU32 b1 = (kFour & kOne).bits();
		ass_eq(b1, 0);

		CxU32 b2 = (kFour ^ kOne).bits();
		ass_eq(b2, (CXT_4 ^ CXT_1));

		CxU32 b3 = (~kFour).bits();
		ass_eq(b3, ~CXT_4);

		CxU32 b4 = (((kFour | kFive | kOne) & (kFour | kOne)) ^ kOne).bits();
		ass_eq(b4, CXT_4);

		CxU32 b4 = (~(((kFour | kFive | kOne) & (kFour | kOne)) ^ kOne)).bits();
		ass_eq(b4, ~CXT_4);
		*/

		FINISH_TEST;
	}

	void testCxFlagsEquality() {
		BEGIN_TEST;

		CxTestFlags f0;
		ass_false((f0 == kFour) || (f0 == kOne) || (f0 == kTwo) || (f0 == kThree) || (f0 == kFive));
	   ass_true((f0 != kFour) && (f0 != kOne) && (f0 != kTwo) && (f0 != kThree) && (f0 != kFive));
		ass_true(f0 == kZero);

		CxTestFlags f1(kFour);
		ass_false((f1 == kZero) || (f1 == kOne) || (f1 == kTwo) || (f1 == kThree) || (f1 == kFive));
		ass_true((f1 != kZero) && (f1 != kOne) && (f1 != kTwo) && (f1 != kThree) && (f1 != kFive));
		ass_true(f1 == kFour);

		CxTestFlags f2;
		ass_false((f2 == f1) || (f1 == f2));
		ass_true((f2 != f1) && (f1 != f2));
		ass_true((f2 == f0) && (f0 == f2));

		CxTestFlags f3(kFour);
		ass_false((f3 == f0) || (f0 == f3));
		ass_true((f3 != f0) && (f0 != f3));
		ass_true((f3 == f1) && (f1 == f3));

		CxTestFlags f4(kOne);
		ass_false((f4 == f0) || (f0 == f4));
		ass_true((f4 != f0) && (f0 != f4));
		ass_false((f4 == f1) || (f1 == f4));
		ass_true((f4 != f1) && (f1 != f4));

		FINISH_TEST;
	}

	void testCxFlagsOperators() {
		BEGIN_TEST;

		CxTestFlags f0;          CxTestFlags f1(kOne);   CxTestFlags f2(kTwo);
		CxTestFlags f3(kThree);  CxTestFlags f4(kFour);  CxTestFlags f5(kFive);

		ass_true((CxU32)f0 == 0);      ass_true((CxI32)f0 == 0);
		ass_true((CxU32)f3 == CXT_3);  ass_true((CxI32)f3 == CXT_3);

		CxTestFlags f6 = f2 | kFour;
		ass_eq(f6.bits(), (CXT_2 | CXT_4));

		f6 = (f2 | kFour) & kTwo;
		ass_eq(f6.bits(), CXT_2);

		f6 = (f2 | kFour) ^ kTwo;
		ass_eq(f6.bits(), CXT_4);

		f6 = kFour;  ass_eq(f6.bits(), CXT_4);
		f6 |= kTwo;
		ass_eq(f6.bits(), (CXT_2 | CXT_4));
		f6 &= kTwo;
		ass_eq(f6.bits(), CXT_2);
		f6 = (f4 | kTwo);
		ass_eq(f6.bits(), (CXT_2 | CXT_4));
		f6 ^= kTwo;
		ass_eq(f6.bits(), CXT_4);

		
		
		CxTestFlags f7 = f2 | f4;
		ass_eq(f7.bits(), (CXT_2 | CXT_4));

		f7 = (f2 | f4) & f2;
		ass_eq(f7.bits(), CXT_2);

		f7 = (f2 | f4) ^ f2;
		ass_eq(f7.bits(), CXT_4);

		f7 = f4;  ass_eq(f7.bits(), CXT_4);
		f7 |= f2;
		ass_eq(f7.bits(), (CXT_2 | CXT_4));
		f7 &= f2;
		ass_eq(f7.bits(), CXT_2);
		f7 = (f4 | f2);
		ass_eq(f7.bits(), (CXT_2 | CXT_4));
		f7 ^= f2;
		ass_eq(f7.bits(), CXT_4);

		f7 = ~f3;
		ass_eq(f7.bits(), (~CXT_3));


		CxTestFlags f8 = (kFour | f2);
		ass_eq(f8.bits(), (CXT_2 | CXT_4));

		f8 = (kFourTwo) & f2;
		ass_eq(f8.bits(), CXT_2);

		f8 = (kFourTwo) ^ f2;
		ass_eq(f8.bits(), CXT_4);

		FINISH_TEST;
	}

	void testCxFlagsClearAndTest() {
		BEGIN_TEST;

		/* Test clear all */
		CxTestFlags f0(kFour | kTwo | kOne);
		ass_eq(f0.bits(), (CXT_4 | CXT_2 | CXT_1));
		f0.clear();
		ass_zero(f0.bits());

		/* Test clear specific bit(s) */
		f0 = (kFour | kTwo | kOne);
		ass_eq(f0.bits(), (CXT_4 | CXT_2 | CXT_1));
		f0.clear(kFour);
		ass_eq(f0.bits(), (CXT_2 | CXT_1));
		f0.clear(kOne);
		ass_eq(f0.bits(), CXT_2);
		f0.clear(kFive);
		ass_eq(f0.bits(), CXT_2);
		f0.clear(kTwo);
		ass_zero(f0.bits());

		f0 = (kFour | kTwo | kOne);
		ass_eq(f0.bits(), (CXT_4 | CXT_2 | CXT_1));
		f0.clear(kFourTwo);
		ass_eq(f0.bits(), CXT_1);
		f0.clear(kFourTwo);
		ass_eq(f0.bits(), CXT_1);
		f0.clear(kOne);
		ass_zero(f0.bits());

		/* Test isSet(E) */
		f0 = (kFour | kTwo | kOne);
		ass_eq(f0.bits(), (CXT_4 | CXT_2 | CXT_1));
		ass_true(f0.isSet(kFour) && f0.isSet(kOne) && f0.isSet(kTwo));
		ass_true(f0.isSet(kFourTwo));
		ass_false(f0.isSet(kThree) || f0.isSet(kFive));

		f0 = (kFour | kThree | kOne);
		ass_eq(f0.bits(), (CXT_4 | CXT_3 | CXT_1));
		ass_true(f0.isSet(kFour) && f0.isSet(kOne) && f0.isSet(kThree));
		ass_false(f0.isSet(kFourTwo));
		ass_false(f0.isSet(kTwo) || f0.isSet(kFive));

		/* Test isAnySet(T) */
		f0 = (kFour | kTwo | kOne);
		ass_eq(f0.bits(), (CXT_4 | CXT_2 | CXT_1));
		ass_true(f0.isAnySet(kFour) && f0.isAnySet(kOne) && f0.isAnySet(kTwo));
		ass_true(f0.isAnySet(kFourTwo));
		ass_true(f0.isAnySet(CXT_4 | CXT_2 | CXT_1));
		ass_true(f0.isAnySet(CXT_1 | CXT_2 | CXT_3 | CXT_4 | CXT_5));
		ass_false(f0.isAnySet(kThree) || f0.isAnySet(kFive));
		ass_false(f0.isAnySet(CXT_3 | CXT_5));

		/* Test isAllSet(T) */
		f0 = (kFour | kTwo | kOne);
		ass_eq(f0.bits(), (CXT_4 | CXT_2 | CXT_1));
		ass_true(f0.isAllSet(kFour) && f0.isAllSet(kOne) && f0.isAllSet(kTwo));
		ass_true(f0.isAllSet(kFourTwo));
		ass_true(f0.isAllSet(CXT_4 | CXT_2 | CXT_1));
		ass_false(f0.isAllSet(CXT_1 | CXT_2 | CXT_3 | CXT_4 | CXT_5));
		ass_false(f0.isAllSet(kThree) || f0.isAllSet(kFive));
		ass_false(f0.isAllSet(CXT_3 | CXT_4));

		f0 = (kFour | kThree | kOne);
		ass_eq(f0.bits(), (CXT_4 | CXT_3 | CXT_1));
		ass_true(f0.isAllSet(kFour) && f0.isAllSet(kOne) && f0.isAllSet(kThree));
		ass_false(f0.isAllSet(kFourTwo));
		ass_false(f0.isAllSet(kTwo) || f0.isAllSet(kFive));

		/* Test set(E) */
		f0 = kZero;
		ass_zero(f0.bits());

		f0.set(kThree);
		ass_eq(f0.bits(), CXT_3);
		f0.set(kFourTwo);
		ass_eq(f0.bits(), (CXT_2 | CXT_3 | CXT_4));
		f0.set(kZero);
		ass_eq(f0.bits(), (CXT_2 | CXT_3 | CXT_4));
		
		FINISH_TEST;
	}
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxFlagsCreate();
	cat::testCxFlagsEquality();
	cat::testCxFlagsOperators();
	cat::testCxFlagsClearAndTest();
	return 0;
}
