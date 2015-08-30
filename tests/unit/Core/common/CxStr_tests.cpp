#include "core/common/CxStr.h"
#include "core/CxTestCore.h"


namespace cat {

	/**
	 * @tests str::alloc8(CxI32)
	 * @tests str::alloc16(CxI32)
	 * @tests str::free(CxChar *&)
	 * @tests str::free(CxChar16 *&)
	 */
	void testStrAlloc() {
		BEGIN_TEST;

		/* Test str::alloc8(CxI32) */
		CxChar *s0 = str::alloc8(10);
		for (CxI32 i = 0; i < 10; ++i) { s0[i] = 'a'; }
		s0[10] = 0;
		ass_true(strcmp(s0, "aaaaaaaaaa") == 0);

		/* Test str::alloc16(CxI32) */
		CxChar16 *s1 = str::alloc16(10);
		for (CxI32 i = 0; i < 10; ++i) { s1[i] = L'\u2764'; }
		s1[10] = 0;
		ass_true(wcscmp(s1, L"\u2764\u2764\u2764\u2764\u2764\u2764\u2764\u2764\u2764\u2764") == 0);

		ass_true(s0 != 0 && s1 != 0);
		str::free(s0);
		ass_true(s0 == 0 && s1 != 0);
		str::free(s1);
		ass_true(s0 == 0 && s1 == 0);
		
		FINISH_TEST;
	}

		/**
	 * @tests str::append(CxChar *, const CxChar *, CxI32)
	 * @tests str::append(CxChar16 *, const CxChar16 *, CxI32)
	 * @tests str::append(CxChar *, const CxChar *)
	 * @tests str::append(CxChar16 *, const CxChar16 *)
	 */
	void testStrAppend() {
		BEGIN_TEST;

		/* Test str::concat(CxChar *, const CxChar *) */
		CxChar *s0 = str::alloc8(10);
		s0[0] = 'a'; s0[1] = 'b'; s0[2] = 0;

		CxChar *s1 = str::append(s0, 11, "Hello");
		ass_true(strcmp(s0, "abHello") == 0);
		ass_eq(s0, s1);

		/* Test str::concat(CxChar *, const CxChar *, CxI32) */
		mem::zero(s0, sizeof(CxChar)*11);
		s0[0] = 'a'; s0[1] = 'b'; s0[2] = 0;
		s1 = str::append(s0, 11, "Hello", 3);
		ass_true(strcmp(s0, "abHel") == 0);
		ass_eq(s0, s1);

		/* Test str::concat(CxChar16 *, const CxChar16 *) */
		CxChar16 *s2 = str::alloc16(10);
		s2[0] = L'\u2764'; s2[1] = L'\u2764'; s2[2] = L'\u2605';  s2[3] = 0;

		CxChar16 *s3 = str::append(s2, 11, L"\u2602\u265E\u262F\u262D\u2622");
		ass_true(wcscmp(s2, L"\u2764\u2764\u2605\u2602\u265E\u262F\u262D\u2622") == 0);
		ass_eq(s3, s2);

		/* Test str::concat(CxChar16 *, const CxChar16 *, CxI32) */
		mem::zero(s2, sizeof(CxChar16)*11);
		s2[0] = L'\u2764'; s2[1] = L'\u2764'; s2[2] = L'\u2605';
		s3 = str::append(s2, 11, L"\u2602\u265E\u262F\u262D\u2622", 3);
		ass_true(wcscmp(s2,  L"\u2764\u2764\u2605\u2602\u265E\u262F") == 0);
		ass_eq(s2, s3);

		str::free(s0);
		str::free(s2);
		
		FINISH_TEST;
	}

	/**
	 * @tests str::len(const CxChar *)
	 * @tests str::len(const CxChar16 *)
	 */
	void testStrLen() {
		BEGIN_TEST;

		/* Test str::len(const CxChar *) */
		CxChar *s0 = str::alloc8(10);
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
		mem::free(s0);
		ass_eq(str::len(s0), 0);
		
		/* Test str::len(const CxChar16 *) */
		CxChar16 *s1 = str::alloc16(10);
		for (CxI32 i = 0; i < 10; ++i) { s1[i] = L'\u2764'; }
		s1[10] = 0;

		ass_eq(str::len(s1), 10);
		s1[9] = 0;
		ass_eq(str::len(s1), 9);
		s1[1] = 0;
		ass_eq(str::len(s1), 1);
		s1[0] = 0;
		ass_eq(str::len(s1), 0);

		s1[0] = L'\u2764';
		ass_eq(str::len(s1), 1);
		mem::free(s1);
		ass_eq(str::len(s1), 0);
		

		FINISH_TEST;
	}

	/**
	 * @tests str::copy(CxChar *, CxI32, const CxChar *)
	 * @tests str::copy(CxChar *, CxI32, const CxChar *, CxI32)
	 * @tests str::copy(CxChar16 *, CxI32, const CxChar16 *)
	 * @tests str::copy(CxChar16 *, CxI32, const CxChar16 *, CxI32)
	 * @tests str::copy(const CxChar *, CxI32)
	 * @tests str::copy(const CxChar16 *, CxI32)
	 * @tests str::copy(const CxChar *)
	 * @tests str::copy(const CxChar16 *)
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

		/* test str::copy(const CxChar16 *) */
		CxChar16 *s2 = str::copy((CxChar16 *)0);
		ass_zero(s2);

		s2 = str::copy(L"\u2764\u2764\u2605\u2602\u265E\u262F\u262D\u2622");
		ass_true(wcscmp(s2, L"\u2764\u2764\u2605\u2602\u265E\u262F\u262D\u2622") == 0);

		/* test str::copy(const CxChar16 *, CxI32) */
		CxChar16 *s3 = str::copy(L"\u2764\u2764\u2605\u2602\u265E\u262F\u262D\u2622", 5);
		ass_true(wcscmp(s3, L"\u2764\u2764\u2605\u2602\u265E") == 0);

		/* test str::copy(CxChar *, CxI32, const CxChar *) */
		CxChar *s4 = str::copy(s0, 6, "Meow");
		ass_eq(s4, s0);
		ass_true(strcmp(s0, "Meow") == 0);

		/* test str::copy(CxChar *, CxI32, const CxChar *, CxI32) */
		s4 = str::copy(s0, 6, "Woofy", 3);
		ass_eq(s4, s0);
		ass_true(strcmp(s0, "Woo") == 0);

		/* test str::copy(CxChar16*, CxI32, const CxChar16 *) */
		CxChar16 *s5 = str::copy(s2, 9, L"\u265E\u262F\u262D\u2622\u262F");
		ass_eq(s5, s2);
		ass_true(wcscmp(s2, L"\u265E\u262F\u262D\u2622\u262F") == 0);

		/* test str::copy(CxChar16*, CxI32, const CxChar16 *, CxI32) */
		s5 = str::copy(s2, 9, L"\u262D\u262D\u262D\u262D\u262D", 3);
		ass_eq(s5, s2);
		ass_true(wcscmp(s2, L"\u262D\u262D\u262D") == 0);

		str::free(s0);
		str::free(s1);
		str::free(s2);
		str::free(s3);

		FINISH_TEST;
	}

	/**
	 * @tests str::eq(const CxChar *, const CxChar *)
	 * @tests str::eq(const CxChar16 *, const CxChar *)
	 * @tests str::eq(const CxChar16 *, const CxChar16 *)
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

		/* test str::eq(const CxChar16 *, const CxChar *) */
		CxChar16 *s16_0 = str::copy(L"Hii Cat.");
		ass_true(str::eq((CxChar16 *)0, (CxChar *)0));
		ass_false(str::eq(s16_0, (CxChar *)0));
		ass_false(str::eq((CxChar16 *)0, "Hiii!"));
		ass_false(str::eq(s16_0, "Hello world."));
		ass_false(str::eq(s16_0, "Hii Cat"));
		ass_false(str::eq(s16_0, "Hii Cat,"));
		ass_true(str::eq(s16_0, "Hii Cat."));

		CxChar16 *s16_1 = str::copy(L"Star Mode \u2605");
		ass_false(str::eq(s16_1, "Star Mode *"));
		ass_false(str::eq(s16_1, "Star Mode "));

		/* test str::eq(const CxChar 16 *, const CxChar 16 *) */
		ass_true(str::eq((CxChar16 *)0, (CxChar16 *)0));
		ass_true(str::eq(s16_1, s16_1));
		ass_false(str::eq(s16_1, (CxChar16 *)0));
		ass_false(str::eq((CxChar16 *)0, s16_1));
		ass_false(str::eq(s16_1, L"Star Mode "));
		ass_false(str::eq(s16_1, L"Star Mode *"));
		ass_true(str::eq(s16_1, L"Star Mode \u2605"));
		ass_true(str::eq(L"Star Mode \u2605", s16_1));
		
		FINISH_TEST;
	}

	/**
	 * @tests str::free(CxChar *&)
	 * @tests str::free(CxChar16 *&)
	 */
	void testStrFree() {
		BEGIN_TEST;

		CxChar *s0 = str::alloc8(10);
		CxChar16 *s1 = str::alloc16(10);

		CxChar *s2 = 0;
		CxChar16 *s3 = 0;
		
		ass_true(s0 != 0 && s1 != 0 && s2 == 0 && s3 == 0);
		str::free(s0);
		ass_true(s0 == 0 && s1 != 0 && s2 == 0 && s3 == 0);
		str::free(s1);
		ass_true(s0 == 0 && s1 == 0 && s2 == 0 && s3 == 0);

		str::free(s2);
		str::free(s3);
		ass_true(s0 == 0 && s1 == 0 && s2 == 0 && s3 == 0);
		str::free(s0);
		str::free(s1);
		ass_true(s0 == 0 && s1 == 0 && s2 == 0 && s3 == 0);

		FINISH_TEST;
	}

	/**
	 * @tests str::greater(const CxChar *, const CxChar *)
	 * @tests str::greater(const CxChar16 *, const CxChar16 *)
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

		/* test str::greater(const CxChar16 *, const CxChar16 *) */
		ass_true(str::greater(L"12345", L"12344"));
		ass_true(str::greater(L"12345", L"1234"));
		ass_true(str::greater(L"1", 0));
		ass_false(str::greater(L"12344", L"12345"));
		ass_false(str::greater(L"1234", L"12345"));
		ass_false(str::greater(0, L"1"));
		ass_false(str::greater((const CxChar16 *)0, (const CxChar16 *)0));
		FINISH_TEST;
	}

	/**
	 * @tests str::less(const CxChar *, const CxChar *)
	 * @tests str::less(const CxChar16 *, const CxChar16 *)
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

		/* test str::less(const CxChar16 *, const CxChar16 *) */
		ass_false(str::less(L"12345", L"12344"));
		ass_false(str::less(L"12345", L"1234"));
		ass_false(str::less(L"1", 0));
		ass_true(str::less(L"12344", L"12345"));
		ass_true(str::less(L"1234", L"12345"));
		ass_true(str::less(0, L"1"));
		ass_false(str::less((const CxChar16 *)0, (const CxChar16 *)0));
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
	 * @tests readI32(CxChar16 *&, CxI32)
	 * @tests readI64(CxChar16 *&, CxI32)
	 * @tests readU32(CxChar16 *&, CxI32)
	 * @tests readU64(CxChar16 *&, CxI32)
	 */
	void testStrReadInt16() {
		BEGIN_TEST;

		/* test readI32(CxChar16 *& CxI32) */
		CxChar16 *s0 = str::copy(L"12 34 -20 1234 asd 1222");
		CxChar16 *s0_p = s0;

		assert(str::readI32(s0_p) == 12);
		assert(str::readI32(s0_p) == 34);
		assert(str::readI32(s0_p) == -20);
		assert(str::readI32(s0_p) == 1234);
		assert(str::readI32(s0_p) == 0);
		assert(str::readI32(s0_p) == 0);

		/* test readI64(CxChar16 *& CxI32) */
		s0_p = s0;
		assert(str::readI64(s0_p) == 12);
		assert(str::readI64(s0_p) == 34);
		assert(str::readI64(s0_p) == -20);
		assert(str::readI64(s0_p) == 1234);
		assert(str::readI64(s0_p) == 0);
		assert(str::readI64(s0_p) == 0);

		/* test readU32(CxChar16 *& CxI32) */
		s0_p = s0;
		assert(str::readU32(s0_p) == 12);
		assert(str::readU32(s0_p) == 34);
		assert(str::readU32(s0_p) == 0xFFFFFFEC);
		assert(str::readU32(s0_p) == 1234);
		assert(str::readU32(s0_p) == 0);
		assert(str::readU32(s0_p) == 0);

		/* test readU64(CxChar16 *& CxI32) */
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
	 * @tests str::readF32(CxChar16 *)
	 * @tests str::readF64(CxChar16 *)
	 */
	void testStrReadFloat16() {
		BEGIN_TEST;

		/* test readF32(CxChar16 *&) */
		CxChar16 *s0 = str::copy(L"12.5 34.5 -20.5 5e-11 asd 1222.4");
		CxChar16 *s0_p = s0;

		assert(str::readF32(s0_p) == 12.5);
		assert(str::readF32(s0_p) == 34.5);
		assert(str::readF32(s0_p) == -20.5);
		assert(fabs(str::readF32(s0_p) - 5e-11) < 0.0001);
		assert(str::readF32(s0_p) == 0);
		assert(str::readF32(s0_p) == 0);

		/* test readF64(CxChar16 *&) */
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
	 * @tests str::toI32(const CxChar16 *, CxI32)
	 * @tests str::toI64(const CxChar16 *, CxI32)
	 * @tests str::toU32(const CxChar16 *, CxI32)
	 * @tests str::toU64(const CxChar16 *, CxI32)
	 */
	void testStrToInt16() {
		BEGIN_TEST;

		/* test str::toI32(const CxChar *, CxI32) */
	   assert(str::toI32((CxChar *)0) == 0);
		assert(str::toI32(L"abcd") == 0);
		assert(str::toI32(L"-abdc") == 0);
		assert(str::toI32(L"asd102") == 0);
		assert(str::toI32(L"-asd102") == 0);

		assert(str::toI32(L"1234") == 1234);
		assert(str::toI32(L"-1234") == -1234);
		assert(str::toI32(L"0x12Fa") == 0x12FA);
		assert(str::toI32(L"-0X12Fa") == -0x12FA);
		assert(str::toI32(L"01732") == 01732);
		assert(str::toI32(L"-01732") == -01732);
		assert(str::toI32(L"1234", 10) == 1234);
		assert(str::toI32(L"123F", 16) == 0x123F);
		assert(str::toI32(L"1234", 8) == 01234);
		assert(str::toI32(L"10", 2) == 2);
		assert(str::toI32(L"1234 999") == 1234);
		assert(str::toI32(L"1234asd") == 1234);

		/* test str::toI64(const CxChar *, CxI32) */
	   assert(str::toI64((CxChar *)0) == 0);
		assert(str::toI64(L"abcd") == 0);
		assert(str::toI64(L"-abdc") == 0);
		assert(str::toI64(L"asd102") == 0);
		assert(str::toI64(L"-asd102") == 0);

		assert(str::toI64(L"1234") == 1234);
		assert(str::toI64(L"-1234") == -1234);
		assert(str::toI64(L"0x12Fa") == 0x12FA);
		assert(str::toI64(L"-0X12Fa") == -0x12FA);
		assert(str::toI64(L"01732") == 01732);
		assert(str::toI64(L"-01732") == -01732);
		assert(str::toI64(L"1234", 10) == 1234);
		assert(str::toI64(L"123F", 16) == 0x123F);
		assert(str::toI64(L"1234", 8) == 01234);
		assert(str::toI64(L"10", 2) == 2);
		assert(str::toI64(L"1234 999") == 1234);
		assert(str::toI64(L"1234asd") == 1234);

		/* test str::toU32(const CxChar *, CxI32) */
	   assert(str::toU32((CxChar *)0) == 0);
		assert(str::toU32(L"abcd") == 0);
		assert(str::toU32(L"-abdc") == 0);
		assert(str::toU32(L"asd102") == 0);
		assert(str::toU32(L"-asd102") == 0);
		assert(str::toU32(L"-1234") == 0xFFFFFB2E);

		assert(str::toU32(L"1234") == 1234);
		assert(str::toU32(L"0x12Fa") == 0x12FA);
		assert(str::toU32(L"01732") == 01732);
		assert(str::toU32(L"1234", 10) == 1234);
		assert(str::toU32(L"123F", 16) == 0x123F);
		assert(str::toU32(L"1234", 8) == 01234);
		assert(str::toU32(L"10", 2) == 2);
		assert(str::toU32(L"1234 999") == 1234);
		assert(str::toU32(L"1234asd") == 1234);
		
		/* test str::toU64(const CxChar *, CxI32) */
	   assert(str::toU64((CxChar *)0) == 0);
		assert(str::toU64(L"abcd") == 0);
		assert(str::toU64(L"-abdc") == 0);
		assert(str::toU64(L"asd102") == 0);
		assert(str::toU64(L"-asd102") == 0);
		assert(str::toU64(L"-1234") == 0xFFFFFB2E);

		assert(str::toU64(L"1234") == 1234);
		assert(str::toU64(L"0x12Fa") == 0x12FA);
		assert(str::toU64(L"01732") == 01732);
		assert(str::toU64(L"1234", 10) == 1234);
		assert(str::toU64(L"123F", 16) == 0x123F);
		assert(str::toU64(L"1234", 8) == 01234);
		assert(str::toU64(L"10", 2) == 2);
		assert(str::toU64(L"1234 999") == 1234);
		assert(str::toU64(L"1234asd") == 1234);

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
	 * @tests str::toF32(const CxChar16 *)
	 * @tests str::toF64(const CxChar16 *)
	 */
	void testStrToFloat16(){
		BEGIN_TEST;
		/* test str::toF32(const CxChar *) */
	   assert(str::toF32(L"abcd") == 0);
		assert(str::toF32(L"-abdc") == 0);
		assert(str::toF32(L"asd102") == 0);
		assert(str::toF32(L"-asd102") == 0);

		assert(str::toF32(L"1234") == 1234);
		assert(str::toF32(L"-1234") == -1234);
		assert(str::toF32(L"1234.5") == 1234.5);
		assert(str::toF32(L"-1234.5") == -1234.5);
		assert(str::toF32(L"1234.5 99") == 1234.5);
		assert(str::toF32(L"1234.5asd") == 1234.5);
		
		assert(fabs(str::toF32(L"5E-12") - 5e-12) < 0.001);
		assert(fabs(str::toF32(L"5e-12") - 5e-12) < 0.001);
		assert(fabs(str::toF32(L"-5e-12") - -5e-12) < 0.001);
		assert(fabs(str::toF32(L"-5E-12") - -5e-12) < 0.001);
		
		assert(fabs(str::toF32(L"5E12") - 5e12) < 100000);
		assert(fabs(str::toF32(L"5e12") - 5e12) < 100000);
		assert(fabs(str::toF32(L"-5e12") - -5e12) < 100000);
		assert(fabs(str::toF32(L"-5E12") - -5e12) < 100000);


		/* test str::toF64(const CxChar *) */
	   assert(str::toF64(L"abcd") == 0);
		assert(str::toF64(L"-abdc") == 0);
		assert(str::toF64(L"asd102") == 0);
		assert(str::toF64(L"-asd102") == 0);

		assert(str::toF64(L"1234") == 1234);
		assert(str::toF64(L"-1234") == -1234);
		assert(str::toF64(L"1234.5") == 1234.5);
		assert(str::toF64(L"-1234.5") == -1234.5);
		assert(str::toF64(L"1234.5 99") == 1234.5);
		assert(str::toF64(L"1234.5asd") == 1234.5);
		
		assert(fabs(str::toF64(L"5E-12") - 5e-12) < 0.000001);
		assert(fabs(str::toF64(L"5e-12") - 5e-12) < 0.000001);
		assert(fabs(str::toF64(L"-5e-12") - -5e-12) < 0.000001);
		assert(fabs(str::toF64(L"-5E-12") - -5e-12) < 0.000001);

		assert(fabs(str::toF64(L"5E12") - 5e12) < 0.000001);
		assert(fabs(str::toF64(L"5e12") - 5e12) < 0.000001);
		assert(fabs(str::toF64(L"-5e12") - -5e12) < 0.000001);
		assert(fabs(str::toF64(L"-5E12") - -5e12) < 0.000001);
		
		
		FINISH_TEST;
	}

	/**
	 * @tests str::format(CxChar *, CxI32, const CxChar *, ...)
	 * @tests str::format(CxChar16 *, CxI32, const CxChar16 *, ...)
	 */
	void testStrFormat() {
		BEGIN_TEST;

		CxChar str[128];
		CxI32 nm_chars = str::format(str, 128, "Int1 %d, Int2 %d, string %s", 10, -1003, "Hello");
		ass_eq(nm_chars, 33);
		assert(strcmp(str, "Int1 10, Int2 -1003, string Hello") == 0);

		CxChar16 str16[128];
		nm_chars = str::format(str16, 128, L"Int1 %d, Int2 %d, \u2622 string %s", 10, -1003, L"Hello\u2764");
		ass_eq(nm_chars, 36);
		assert(wcscmp(str16, L"Int1 10, Int2 -1003, \u2622 string Hello\u2764") == 0);

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
		
		nm_args = str::scan(L"\u2622 Int1 10, Int2 -1003, float1 -1.5", L"\u2622 Int1 %d, Int2 %d, float1 %f", &i0, &i1, &f0, &f1);
		ass_eq(nm_args, 3);
		ass_eq(i0, 10);
		ass_eq(i1, -1003);
		ass_eq(f0, -1.5);
		ass_eq(f1, 0);

		FINISH_TEST;
	}
	
} // namespace cat

int main(int argc, char **argv) {
	cat::testStrAlloc();
	cat::testStrAppend();
	cat::testStrLen();
	cat::testStrCopy();
	cat::testStrEq();
	cat::testStrFree();
	cat::testStrGreater();
	cat::testStrLess();
	cat::testStrReadInt();
	cat::testStrReadInt16();
	cat::testStrReadFloat();
	cat::testStrReadFloat16();
	cat::testStrToInt();
	cat::testStrToInt16();
	cat::testStrToFloat();
	cat::testStrToFloat16();
	cat::testStrFormat();
	cat::testStrScan();
	
	return 0;
}
