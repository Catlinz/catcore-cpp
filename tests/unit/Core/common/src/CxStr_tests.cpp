#include "core/common/CxStr.h"
#include "core/CxTestCore.h"


namespace cat {

	/**
	 * @tests str::alloc(CxI32)
	 * @tests str::allocZero(CxI32)
	 * @tests str::free(CxChar *&)
	 */
	void testStrAlloc() {
		BEGIN_TEST;

		/* Test str::alloc(CxI32) */
		CxChar *s0 = str::alloc(10);
		for (CxI32 i = 0; i < 10; ++i) { s0[i] = 'a'; }
		s0[10] = 0;
		ass_true(strcmp(s0, "aaaaaaaaaa") == 0);

		CxChar *s1 = str::allocZero(10);
		for(CxI32 i = 0; i < 11; ++i) { ass_zero(s1[i]); }

		ass_true(s0 != 0 && s1 != 0);
		str::free(s0);
		ass_true(s0 == 0 && s1 != 0);
		str::free(s1);
		ass_true(s0 == 0 && s1 == 0);
		
		FINISH_TEST;
	}

		/**
	 * @tests str::append(CxChar *, const CxChar *, CxI32)
	 * @tests str::append(CxChar *, const CxChar *)
	 */
	void testStrAppend() {
		BEGIN_TEST;

		/* Test str::append(CxChar *, const CxChar *) */
		CxChar *s0 = str::alloc(10);
		s0[0] = 'a'; s0[1] = 'b'; s0[2] = 0;

		CxChar *s1 = str::append(s0, 11, "Hello");
		ass_true(strcmp(s0, "abHello") == 0);
		ass_eq(s0, s1);

		/* Test str::append(CxChar *, const CxChar *, CxI32) */
		mem::zero(s0, sizeof(CxChar)*11);
		s0[0] = 'a'; s0[1] = 'b'; s0[2] = 0;
		s1 = str::append(s0, 11, "Hello", 3);
		ass_true(strcmp(s0, "abHel") == 0);
		ass_eq(s0, s1);

		str::free(s0);
		
		FINISH_TEST;
	}

	/**
	 * @tests str::len(const CxChar *)
	 */
	void testStrLen() {
		BEGIN_TEST;

		/* Test str::len(const CxChar *) */
		CxChar *s0 = str::alloc(10);
		mem::set(s0, 'a', sizeof(CxChar)*10);
		s0[10] = 0;

		ass_eq(str::len(s0), 10);
		s0[9] = 0;
		ass_eq(str::len(s0), 9);
		s0[1] = 0;
		ass_eq(str::len(s0), 1);
		s0[0] = 0;
		ass_eq(str::len(s0), 0);

		s0[0] = 'a';
		ass_eq(str::len(s0), 1);
		str::free(s0);
		ass_eq(str::len(s0), 0);
		
		FINISH_TEST;
	}

	/**
	 * @tests str::copy(CxChar *, CxI32, const CxChar *)
	 * @tests str::copy(CxChar *, CxI32, const CxChar *, CxI32)
	 * @tests str::copy(const CxChar *, CxI32)
	 * @tests str::copy(const CxChar *)
	 */
	void testStrCopy() {
		BEGIN_TEST;

		/* test str::copy(const CxChar *) */
		CxChar *s0 = str::copy((CxChar *)0);
		ass_zero(s0);
		
		s0 = str::copy("Hello there");
		ass_true(strcmp(s0, "Hello there") == 0);

		/* test str::copy(const CxChar *, CxI32) */
		CxChar *s1 = str::copy("Hello there", 5);
		ass_true(strcmp(s1, "Hello") == 0);

		/* test str::copy(CxChar *, CxI32, const CxChar *) */
		CxChar *s4 = str::copy(s0, 6, "Meow");
		ass_eq(s4, s0);
		ass_true(strcmp(s0, "Meow") == 0);

		/* test str::copy(CxChar *, CxI32, const CxChar *, CxI32) */
		s4 = str::copy(s0, 6, "Woofy", 3);
		ass_eq(s4, s0);
		ass_true(strcmp(s0, "Woo") == 0);

		str::free(s0);
		str::free(s1);

		FINISH_TEST;
	}

	/**
	 * @tests str::endsWith(const CxChar *, const CxChar *)
	 */
	void testStrEndsWith() {
		BEGIN_TEST;

		CxChar *s0 = str::copy("Meow01");
		ass_true(str::endsWith(s0, "1"));
		ass_true(str::endsWith(s0, "01"));
		ass_true(str::endsWith(s0, "w01"));
		ass_true(str::endsWith(s0, "ow01"));
		ass_true(str::endsWith(s0, "eow01"));
		ass_true(str::endsWith(s0, "Meow01"));

		ass_false(str::endsWith(s0, "10"));
		ass_false(str::endsWith(s0, "a01"));
		ass_false(str::endsWith(s0, "w01a"));
		ass_false(str::endsWith(s0, "aow1"));
		ass_false(str::endsWith(s0, "eow01a"));
		ass_false(str::endsWith(s0, "aMeow01"));

		ass_false(str::endsWith(0, 0));
		ass_false(str::endsWith(s0, 0));
		ass_false(str::endsWith(0, "sd"));
		
		FINISH_TEST;
	}

	/**
	 * @tests str::eq(const CxChar *, const CxChar *)
	 * @tests str::eq(const CxChar *, const CxChar *, CxI32)
	 */
	void testStrEq() {
		BEGIN_TEST;

		/* test str::eq(const CxChar *, const CxChar *) */
		ass_true(str::eq((const CxChar *)0, (const CxChar *)0));
		
		CxChar *s8_0 = str::copy("Hello World.");
		ass_true(str::eq(s8_0, s8_0));
		ass_true(str::eq(s8_0, "Hello World."));
		ass_false(str::eq(s8_0, "Hello World,"));
		ass_false(str::eq(s8_0, "Hello World"));
		ass_false(str::eq(s8_0, 0));
		ass_false(str::eq((CxChar *)0, s8_0));

		CxChar *s8_1 = str::copy("Hello World,");
		ass_false(str::eq(s8_0, s8_1));
		ass_false(str::eq(s8_1, s8_0));

		CxChar *s8_2 = str::copy("Hello World");
		ass_false(str::eq(s8_0, s8_2));
		ass_false(str::eq(s8_2, s8_0));

		/* test str::eq(const CxChar *, const CxChar *, CxI32) */
		ass_true(str::eq(s8_0, s8_0, 12));
		ass_true(str::eq(s8_2, s8_2, 11));
		ass_false(str::eq(s8_0, s8_1, 12));
		ass_false(str::eq(s8_1, s8_0, 12));
		ass_true(str::eq(s8_0, s8_1, 11));

		str::free(s8_0);
		str::free(s8_1);
		str::free(s8_2);

		FINISH_TEST;
	}

	/**
	 * @tests str::free(CxChar *&)
	 */
	void testStrFree() {
		BEGIN_TEST;

		CxChar *s0 = str::alloc(10);
		CxChar *s2 = 0;
		
		ass_true(s0 != 0 && s2 == 0);
		str::free(s0);
		ass_true(s0 == 0 && s2 == 0);
		str::free(s2);
		ass_true(s0 == 0 && s2 == 0);

		str::free(s0);
		ass_true(s0 == 0 && s2 == 0);

		CxChar ** sn = (CxChar **)mem::allocZero(sizeof(CxChar *), 100);
		for (CxI32 i = 0; i < 100; ++i) {
			sn[i] = str::alloc(10 + i);  ass_neq(sn[i], 0);
		}

		for (CxI32 i = 0; i < 100; ++i) {
			ass_neq(sn[i], 0);
			str::free(sn[i]);
			ass_zero(sn[i]);
		}

		mem::free(sn);

		FINISH_TEST;
	}

	/**
	 * @tests str::greater(const CxChar *, const CxChar *)
	 */
	void testStrGreater() {
		BEGIN_TEST;

		/* test str::greater(const CxChar *, const CxChar *) */
		ass_true(str::greater("12345", "12344"));
		ass_true(str::greater("12345", "1234"));
		ass_true(str::greater("1", 0));
		ass_false(str::greater("12344", "12345"));
		ass_false(str::greater("1234", "12345"));
		ass_false(str::greater(0, "1"));
		ass_false(str::greater((const CxChar *)0, (const CxChar *)0));

		FINISH_TEST;
	}

	/**
	 * @tests str::less(const CxChar *, const CxChar *)
	 */
	void testStrLess() {
		BEGIN_TEST;

		/* test str::less(const CxChar *, const CxChar *) */
		ass_false(str::less("12345", "12344"));
		ass_false(str::less("12345", "1234"));
		ass_false(str::less("1", 0));
		ass_true(str::less("12344", "12345"));
		ass_true(str::less("1234", "12345"));
		ass_true(str::less(0, "1"));
		ass_false(str::less((const CxChar *)0, (const CxChar *)0));

		FINISH_TEST;
	}

		/**
	 * @tests readI32(CxChar *&, CxI32)
	 * @tests readI64(CxChar *&, CxI32)
	 * @tests readU32(CxChar *&, CxI32)
	 * @tests readU64(CxChar *&, CxI32)
	 */
	void testStrReadInt() {
		BEGIN_TEST;

		/* test readI32(CxChar *& CxI32) */
		CxChar *s0 = str::copy("12 34 -20 1234 asd 1222");
		CxChar *s0_p = s0;

		assert(str::readI32(s0_p) == 12);
		assert(str::readI32(s0_p) == 34);
		assert(str::readI32(s0_p) == -20);
		assert(str::readI32(s0_p) == 1234);
		assert(str::readI32(s0_p) == 0);
		assert(str::readI32(s0_p) == 0);

		/* test readI64(CxChar *& CxI32) */
		s0_p = s0;
		assert(str::readI64(s0_p) == 12);
		assert(str::readI64(s0_p) == 34);
		assert(str::readI64(s0_p) == -20);
		assert(str::readI64(s0_p) == 1234);
		assert(str::readI64(s0_p) == 0);
		assert(str::readI64(s0_p) == 0);

		/* test readU32(CxChar *& CxI32) */
		s0_p = s0;
		assert(str::readU32(s0_p) == 12);
		assert(str::readU32(s0_p) == 34);
		assert(str::readU32(s0_p) == 0xFFFFFFEC);
		assert(str::readU32(s0_p) == 1234);
		assert(str::readU32(s0_p) == 0);
		assert(str::readU32(s0_p) == 0);

		/* test readU64(CxChar *& CxI32) */
		s0_p = s0;
		assert(str::readU64(s0_p) == 12);
		assert(str::readU64(s0_p) == 34);
		assert(str::readU64(s0_p) == 0xFFFFFFEC);
		assert(str::readU64(s0_p) == 1234);
		assert(str::readU64(s0_p) == 0);
		assert(str::readU64(s0_p) == 0);

		str::free(s0);

		FINISH_TEST;
	}

	/**
	 * @tests str::readF32(CxChar *)
	 * @tests str::readF64(CxChar *)
	 */
	void testStrReadFloat() {
		BEGIN_TEST;

		/* test readF32(CxChar *&) */
		CxChar *s0 = str::copy("12.5 34.5 -20.5 5e-11 asd 1222.4");
		CxChar *s0_p = s0;

		assert(str::readF32(s0_p) == 12.5);
		assert(str::readF32(s0_p) == 34.5);
		assert(str::readF32(s0_p) == -20.5);
		assert(fabs(str::readF32(s0_p) - 5e-11) < 0.0001);
		assert(str::readF32(s0_p) == 0);
		assert(str::readF32(s0_p) == 0);

		/* test readF64(CxChar *&) */
		s0_p = s0;
		assert(str::readF64(s0_p) == 12.5);
		assert(str::readF64(s0_p) == 34.5);
		assert(str::readF64(s0_p) == -20.5);
		assert(fabs(str::readF64(s0_p) - 5e-11) < 0.0000001);
		assert(str::readF64(s0_p) == 0);
		assert(str::readF64(s0_p) == 0);

		str::free(s0);

		FINISH_TEST;
	}

	/**
	 * @tests str::toI32(const CxChar *, CxI32)
	 * @tests str::toI64(const CxChar *, CxI32)
	 * @tests str::toU32(const CxChar *, CxI32)
	 * @tests str::toU64(const CxChar *, CxI32)
	 */
	void testStrToInt() {
		BEGIN_TEST;

		/* test str::toI32(const CxChar *, CxI32) */
	   assert(str::toI32((CxChar *)0) == 0);
		assert(str::toI32("abcd") == 0);
		assert(str::toI32("-abdc") == 0);
		assert(str::toI32("asd102") == 0);
		assert(str::toI32("-asd102") == 0);

		assert(str::toI32("1234") == 1234);
		assert(str::toI32("-1234") == -1234);
		assert(str::toI32("0x12Fa") == 0x12FA);
		assert(str::toI32("-0X12Fa") == -0x12FA);
		assert(str::toI32("01732") == 01732);
		assert(str::toI32("-01732") == -01732);
		assert(str::toI32("1234", 10) == 1234);
		assert(str::toI32("123F", 16) == 0x123F);
		assert(str::toI32("1234", 8) == 01234);
		assert(str::toI32("10", 2) == 2);
		assert(str::toI32("1234 999") == 1234);
		assert(str::toI32("1234asd") == 1234);

		/* test str::toI64(const CxChar *, CxI32) */
	   assert(str::toI64((CxChar *)0) == 0);
		assert(str::toI64("abcd") == 0);
		assert(str::toI64("-abdc") == 0);
		assert(str::toI64("asd102") == 0);
		assert(str::toI64("-asd102") == 0);

		assert(str::toI64("1234") == 1234);
		assert(str::toI64("-1234") == -1234);
		assert(str::toI64("0x12Fa") == 0x12FA);
		assert(str::toI64("-0X12Fa") == -0x12FA);
		assert(str::toI64("01732") == 01732);
		assert(str::toI64("-01732") == -01732);
		assert(str::toI64("1234", 10) == 1234);
		assert(str::toI64("123F", 16) == 0x123F);
		assert(str::toI64("1234", 8) == 01234);
		assert(str::toI64("10", 2) == 2);
		assert(str::toI64("1234 999") == 1234);
		assert(str::toI64("1234asd") == 1234);

		/* test str::toU32(const CxChar *, CxI32) */
	   assert(str::toU32((CxChar *)0) == 0);
		assert(str::toU32("abcd") == 0);
		assert(str::toU32("-abdc") == 0);
		assert(str::toU32("asd102") == 0);
		assert(str::toU32("-asd102") == 0);
		assert(str::toU32("-1234") == 0xFFFFFB2E);

		assert(str::toU32("1234") == 1234);
		assert(str::toU32("0x12Fa") == 0x12FA);
		assert(str::toU32("01732") == 01732);
		assert(str::toU32("1234", 10) == 1234);
		assert(str::toU32("123F", 16) == 0x123F);
		assert(str::toU32("1234", 8) == 01234);
		assert(str::toU32("10", 2) == 2);
		assert(str::toU32("1234 999") == 1234);
		assert(str::toU32("1234asd") == 1234);
		
		/* test str::toU64(const CxChar *, CxI32) */
	   assert(str::toU64((CxChar *)0) == 0);
		assert(str::toU64("abcd") == 0);
		assert(str::toU64("-abdc") == 0);
		assert(str::toU64("asd102") == 0);
		assert(str::toU64("-asd102") == 0);
		assert(str::toU64("-1234") == 0xFFFFFB2E);

		assert(str::toU64("1234") == 1234);
		assert(str::toU64("0x12Fa") == 0x12FA);
		assert(str::toU64("01732") == 01732);
		assert(str::toU64("1234", 10) == 1234);
		assert(str::toU64("123F", 16) == 0x123F);
		assert(str::toU64("1234", 8) == 01234);
		assert(str::toU64("10", 2) == 2);
		assert(str::toU64("1234 999") == 1234);
		assert(str::toU64("1234asd") == 1234);
		
		FINISH_TEST;
	}

	/**
	 * @tests str::startsWith(const CxChar *, const CxChar *)
	 */
	void testStrStartsWith() {
		BEGIN_TEST;

		CxChar *s0 = str::copy("Meow01");
		ass_true(str::startsWith(s0, "M"));
		ass_true(str::startsWith(s0, "Me"));
		ass_true(str::startsWith(s0, "Meo"));
		ass_true(str::startsWith(s0, "Meow"));
		ass_true(str::startsWith(s0, "Meow0"));
		ass_true(str::startsWith(s0, "Meow01"));

		ass_false(str::startsWith(s0, "0M"));
		ass_false(str::startsWith(s0, "M0"));
		ass_false(str::startsWith(s0, "m"));
		ass_false(str::startsWith(s0, "Mea"));
		ass_false(str::startsWith(s0, "aMeo"));
		ass_false(str::startsWith(s0, "Meowa"));
		ass_false(str::startsWith(s0, "aMeow0"));
		ass_false(str::startsWith(s0, "Meow01a"));

		ass_false(str::startsWith(0, 0));
		ass_false(str::startsWith(s0, 0));
		ass_false(str::startsWith(0, "sd"));
		
		FINISH_TEST;
	}

	/**
	 * @tests str::toF32(const CxChar *)
	 * @tests str::toF64(const CxChar *)
	 */
	void testStrToFloat(){
		BEGIN_TEST;
		/* test str::toF32(const CxChar *) */
	   assert(str::toF32("abcd") == 0);
		assert(str::toF32("-abdc") == 0);
		assert(str::toF32("asd102") == 0);
		assert(str::toF32("-asd102") == 0);

		assert(str::toF32("1234") == 1234);
		assert(str::toF32("-1234") == -1234);
		assert(str::toF32("1234.5") == 1234.5);
		assert(str::toF32("-1234.5") == -1234.5);
		assert(str::toF32("1234.5 99") == 1234.5);
		assert(str::toF32("1234.5asd") == 1234.5);
		
		assert(fabs(str::toF32("5E-12") - 5e-12) < 0.001);
		assert(fabs(str::toF32("5e-12") - 5e-12) < 0.001);
		assert(fabs(str::toF32("-5e-12") - -5e-12) < 0.001);
		assert(fabs(str::toF32("-5E-12") - -5e-12) < 0.001);
		
		assert(fabs(str::toF32("5E12") - 5e12) < 100000);
		assert(fabs(str::toF32("5e12") - 5e12) < 100000);
		assert(fabs(str::toF32("-5e12") - -5e12) < 100000);
		assert(fabs(str::toF32("-5E12") - -5e12) < 100000);


		/* test str::toF64(const CxChar *) */
	   assert(str::toF64("abcd") == 0);
		assert(str::toF64("-abdc") == 0);
		assert(str::toF64("asd102") == 0);
		assert(str::toF64("-asd102") == 0);

		assert(str::toF64("1234") == 1234);
		assert(str::toF64("-1234") == -1234);
		assert(str::toF64("1234.5") == 1234.5);
		assert(str::toF64("-1234.5") == -1234.5);
		assert(str::toF64("1234.5 99") == 1234.5);
		assert(str::toF64("1234.5asd") == 1234.5);
		
		assert(fabs(str::toF64("5E-12") - 5e-12) < 0.000001);
		assert(fabs(str::toF64("5e-12") - 5e-12) < 0.000001);
		assert(fabs(str::toF64("-5e-12") - -5e-12) < 0.000001);
		assert(fabs(str::toF64("-5E-12") - -5e-12) < 0.000001);

		assert(fabs(str::toF64("5E12") - 5e12) < 0.000001);
		assert(fabs(str::toF64("5e12") - 5e12) < 0.000001);
		assert(fabs(str::toF64("-5e12") - -5e12) < 0.000001);
		assert(fabs(str::toF64("-5E12") - -5e12) < 0.000001);
		
		
		FINISH_TEST;
	}

	/**
	 * @tests str::format(CxChar *, CxI32, const CxChar *, ...)
	 */
	void testStrFormat() {
		BEGIN_TEST;

		CxChar str[128];
		CxI32 nm_chars = str::format(str, 128, "Int1 %d, Int2 %d, string %s", 10, -1003, "Hello");
		ass_eq(nm_chars, 33);
		assert(strcmp(str, "Int1 10, Int2 -1003, string Hello") == 0);

		FINISH_TEST;
	}

	/**
	 * @tests str::scan(const CxChar *, const CxChar *, ...)
	 */
	void testStrScan() {
		BEGIN_TEST;

		CxI32 i0 = 0, i1 = 0;
		CxF32 f0 = 0, f1 = 0;

		CxI32 nm_args = str::scan("Int1 10, Int2 -1003, float1 -1.5", "Int1 %d, Int2 %d, float1 %f", &i0, &i1, &f0, &f1);
		ass_eq(nm_args, 3);
		ass_eq(i0, 10);
		ass_eq(i1, -1003);
		ass_eq(f0, -1.5);
		ass_eq(f1, 0);

		i0 = i1 = 0;
		f0 = f1 = 0;
		nm_args = 0;

		FINISH_TEST;
	}
	
} // namespace cat

int main(int argc, char **argv) {
	cat::testStrAlloc();
	cat::testStrAppend();
	cat::testStrLen();
	cat::testStrCopy();
	cat::testStrEndsWith();
	cat::testStrEq();
	cat::testStrFree();
	cat::testStrGreater();
	cat::testStrLess();
	cat::testStrReadInt();
	cat::testStrReadFloat();
	cat::testStrStartsWith();
	cat::testStrToInt();
	cat::testStrToFloat();
	cat::testStrFormat();
	cat::testStrScan();
	
	return 0;
}
