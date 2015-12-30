#include "core/common/CxUtf8.h"
#include "core/CxTestCore.h"


namespace cat {

	/*"<cent sign>/<copyright sign>/<small capital A>"*/
	const CxU8 str0[10] = {0xC2, 0xA2, '/', 0xC2, 0xA9, '/', 0xE1, 0xB4, 0x80, 0};
	const CxU8 str1[14] = {'H', 'e', 'l', 'l', 'o', ' ', 0xE2, 0x99, 0xA5, ' ', 0xE2, 0x87, 0xBD, 0};

	/**
	 * @tests utf8::alloc(CxI32)
	 * @tests utf8::allocZero(CxI32)
	 * @tests utf8::free(CxU8 *&)
	 */
	void testUtf8Alloc() {
		BEGIN_TEST;

		/* Test utf8::alloc(CxI32) */
		CxU8 *s0 = utf8::alloc(9);
		for (CxI32 i = 0; i < 10; ++i) { s0[i] = str0[i]; }
		ass_true(s0 != 0);

		CxU8 *s1 = utf8::alloc(13);
		for (CxI32 i = 0; i < 14; ++i) { s1[i] = str1[i]; }
		ass_true(s1 != 0);

		ass_true(s0 != s1);
		utf8::free(s0);
		ass_true(s0 == 0 && s1 != 0);
		utf8::free(s1);
		ass_true(s0 == 0 && s1 == 0);

		/* Test utf8::allocZero(CxI32) */
		s0 = utf8::allocZero(9);
		for (CxI32 i = 0; i < 10; ++i) { ass_zero(s0[i]); }
		utf8::free(s0);

		s0 = utf8::allocZero(100);
	   for (CxI32 i = 0; i < 101; ++i) { ass_zero(s0[i]); }
		utf8::free(s0);
		
		FINISH_TEST;
	}

	/**
	 * @tests str::append(CxU8 *, CxI32, const CxU8 *, CxI32)
	 * @tests str::append(CxU8 *, CxI32, const CxU8 *)
	 */
	void testUtf8Append() {
		BEGIN_TEST;

		CxU8 *s0 = utf8::alloc(9);
		for (CxI32 i = 0; i < 10; ++i) { s0[i] = str0[i]; }

		CxU8 *s1 = utf8::alloc(13);
		for (CxI32 i = 0; i < 14; ++i) { s1[i] = str1[i]; }

		/* Test str::append(CxU8 *, CxI32, CxU8 *, CxI32) */
		CxU8 * s2 = utf8::allocZero(40);
		utf8::append(s2, 41, s0);
		for (CxI32 i = 0; i < 10; ++i) { ass_eq(s2[i], s0[i]); }

		utf8::append(s2, 41, s1);
		for (CxI32 i = 0; i < 9; ++i) { ass_eq(s2[i], s0[i]); }
		for (CxI32 i = 9; i < 23; ++i) { ass_eq(s2[i], s1[i-9]); }
		
		/* Test utf8::append(CxU8 *, CxI32, CxU8 *, CxI32) */
		CxU8 * s3 = utf8::allocZero(40);
		utf8::append(s3, 41, s0, 6);
		for (CxI32 i = 0; i < 6; ++i) { ass_eq(s3[i], s0[i]); }
		ass_eq(s3[6], 0);

		utf8::append(s3, 41, s1, 100);
		for (CxI32 i = 0; i < 6; ++i) { ass_eq(s3[i], s0[i]); }
		for (CxI32 i = 6; i < 20; ++i) { ass_eq(s3[i], s1[i-6]); }
		
		utf8::free(s0);
		utf8::free(s1);
		utf8::free(s2);
		utf8::free(s3);
		
		FINISH_TEST;
	}

	/**
	 * @tests utf8::bytes(const CxU8 *)
	 */
	void testUtf8Bytes() {
		BEGIN_TEST;

		CxU8 *s0 = utf8::allocZero(9);
		ass_eq(utf8::bytes(s0), 0);

		for (CxI32 i = 0; i < 10; ++i) { s0[i] = str0[i]; }
		ass_eq(utf8::bytes(s0), 9);


		CxU8 *s1 = utf8::allocZero(50);
		ass_eq(utf8::bytes(s1), 0);
		
		for (CxI32 i = 0; i < 14; ++i) { s1[i] = str1[i]; }
		ass_eq(utf8::bytes(s1), 13);

		utf8::free(s0);
		utf8::free(s1);
		
		FINISH_TEST;
	}

	/**
	 * @tests utf8::copy(CxU8 *, CxI32, const CxU8 *)
	 * @tests utf8::copy(CxU8 *, CxI32, const CxU8 *, CxI32)
	 * @tests utf8::copy(const CxU8 *, CxI32)
	 * @tests utf8::copy(const CxU8 *)
	 * @tests utf8::copy(const CxChar *)
	 */
	void testUtf8Copy() {
		BEGIN_TEST;

		/* test utf8::copy(const CxChar *) */
		CxU8 *s0 = utf8::copy((CxChar *)0);
		ass_zero(s0);
		
		s0 = utf8::copy("Hello there");
		ass_eq(utf8::bytes(s0), 11);
		ass_true(s0[0] == 'H' && s0[3] == 'l' && s0[6] == 't' && s0[11] == 0);

		/* test utf8::copy(const CxU8 *) */
		CxU8 *s1 = utf8::copy((CxU8 *)0);
		ass_zero(s1);
		
		s1 = utf8::copy(str0);
		ass_eq(utf8::bytes(s1), 9);
		for (CxI32 i = 0; i < 10; ++i) { ass_eq(s1[i], str0[i]); }

		/* test utf8::copy(const CxU8 *, CxI32) */
		CxU8 *s2 = utf8::copy(str1, 20);
		ass_eq(utf8::bytes(s2), 13);
		for (CxI32 i = 0; i < 14; ++i) { ass_eq(s2[i], str1[i]); }

		utf8::free(s2);
		s2 = utf8::copy(str0, 5);
		ass_eq(utf8::bytes(s2), 5);
		for (CxI32 i = 0; i < 5; ++i) { ass_eq(s2[i], str0[i]); }
		ass_zero(s2[5]);

		/* test utf8::copy(CxU8 *, CxI32, const CxU8 *, CxI32) */
		utf8::free(s2);
		s2 = utf8::alloc(20);
		s2 = utf8::copy(s2, 21, str1, 20);
		ass_eq(utf8::bytes(s2), 13);
		for (CxI32 i = 0; i < 14; ++i) { ass_eq(s2[i], str1[i]); }

		s2 = utf8::copy(s2, 21, str0, 5);
		for (CxI32 i = 0; i < 5; ++i) { ass_eq(s2[i], str0[i]); }
		ass_zero(s2[5]);

		s2 = utf8::copy(s2, 21, str1, 100);
		ass_zero(s2[0]);

		/* test utf8::copy(CxU8 *, CxI32, const CxU8 *) */
		s2 = utf8::copy(s2, 21, str1);
		ass_eq(utf8::bytes(s2), 13);
		for (CxI32 i = 0; i < 14; ++i) { ass_eq(s2[i], str1[i]); }

		s2 = utf8::copy(s2, 21, str0);
		for (CxI32 i = 0; i < 10; ++i) { ass_eq(s2[i], str0[i]); }
		ass_neq(s2[5], 0);

		utf8::free(s0);
		utf8::free(s1);
		utf8::free(s2);
		
		FINISH_TEST;
	}

	/**
	 * @tests utf8::endsWith(const CxU8 *, const CxU8 *)
	 * @tests utf8::endsWith(const CxU8 *, const CxChar *)
	 */
	void testUtf8EndsWith() {
		BEGIN_TEST;

		const CxU8 end1[2] = {0xBD, 0};
		const CxU8 end2[3] = {0x87, 0xBD, 0};
		const CxU8 end3[4] = {0xE2, 0x87, 0xBD, 0};
		const CxU8 end4[5] = {' ', 0xE2, 0x87, 0xBD, 0};
		const CxU8 end5[6] = {0xA5, ' ', 0xE2, 0x87, 0xBD, 0};
		const CxU8 end12[13] = {'e', 'l', 'l', 'o', ' ', 0xE2, 0x99, 0xA5, ' ', 0xE2, 0x87, 0xBD, 0};

		const CxU8 end1_bad[2] = {0x87, 0};
		const CxU8 end2_bad[3] = {0xBD, 0x87, 0};
		const CxU8 end3_bad[4] = {0x87, 0xE2, 0xBD, 0};
		const CxU8 end4_bad[5] = {'o', 0xE2, 0x87, 0xBD, 0};
		const CxU8 end12_bad[13] = {'h', 'l', 'l', 'o', ' ', 0xE2, 0x99, 0xA5, ' ', 0xE2, 0x87, 0xBD, 0};

		/* Test utf8::endsWith(const CxU8 *, const CxU8 *) */
		CxU8 *s0 = utf8::copy(str1);
		ass_true(utf8::endsWith(s0, end1));
		ass_true(utf8::endsWith(s0, end2));
		ass_true(utf8::endsWith(s0, end3));
		ass_true(utf8::endsWith(s0, end4));
		ass_true(utf8::endsWith(s0, end5));
		ass_true(utf8::endsWith(s0, end12));
		ass_true(utf8::endsWith(s0, str1));

		ass_false(utf8::endsWith(s0, end1_bad));
		ass_false(utf8::endsWith(s0, end2_bad));
		ass_false(utf8::endsWith(s0, end3_bad));
		ass_false(utf8::endsWith(s0, end4_bad));
		ass_false(utf8::endsWith(s0, end12_bad));

		ass_false(utf8::endsWith(0, (const CxU8 *)0));
		ass_false(utf8::endsWith(s0, (const CxU8 *)0));
		ass_false(utf8::endsWith(0, end1));
		

		/* Test utf8::endsWith(const CxU8 *, const CxChar *) */
		CxU8 *s1 = utf8::copy("  Meow01");
		s1[0] = 0xC2;  s1[1] = 0xA2;  /* Cent sign */
		ass_true(utf8::endsWith(s1, "1"));
		ass_true(utf8::endsWith(s1, "01"));
		ass_true(utf8::endsWith(s1, "w01"));
		ass_true(utf8::endsWith(s1, "ow01"));
		ass_true(utf8::endsWith(s1, "eow01"));
		ass_true(utf8::endsWith(s1, "Meow01"));

		ass_false(utf8::endsWith(s1, "10"));
		ass_false(utf8::endsWith(s1, "a01"));
		ass_false(utf8::endsWith(s1, "w01a"));
		ass_false(utf8::endsWith(s1, "aow1"));
		ass_false(utf8::endsWith(s1, "eow01a"));
		ass_false(utf8::endsWith(s1, "aMeow01"));

		ass_false(utf8::endsWith(0, (const CxChar *)0));
		ass_false(utf8::endsWith(s1, (const CxChar *)0));
		ass_false(utf8::endsWith(0, "sd"));

		utf8::free(s0);
		utf8::free(s1);
		
		FINISH_TEST;
	}

	/**
	 * @tests utf8::eq(const CxU8 *, const CxU8 *)
	 * @tests utf8::eq(const CxU8 *, const CxChar *)
	 * @tests utf8::eq(const CxU8 *, const CxU8 *, CxI32)
	 */
	void testUtf8Eq() {
		BEGIN_TEST;

		CxU8 * str0_bad0 = utf8::copy(str0, 10);
		str0_bad0[9] = ',';
		str0_bad0[10] = 0;

		CxU8 * str0_bad1 = utf8::copy(str0);
		str0_bad1[0] = ',';
		
		CxU8 * str0_bad2 = utf8::copy(str0);
		str0_bad2[2] = str0[3];
		str0_bad2[3] = str0[2];
		

		/* Test utf8::eq(const CxU8 *, const CxU8 *) */
		ass_true(utf8::eq((const CxU8 *)0, (const CxU8 *)0));
		CxU8 *s0 = utf8::copy(str0);
		ass_true(utf8::eq(s0, s0));
		ass_true(utf8::eq(s0, str0));
		ass_true(utf8::eq(str0, s0));
		ass_false(utf8::eq(s0, str0_bad0) || utf8::eq(str0_bad0, s0));
		ass_false(utf8::eq(s0, str0_bad1) || utf8::eq(str0_bad1, s0));
		ass_false(utf8::eq(s0, str0_bad2) || utf8::eq(str0_bad2, s0));
		s0[2] = str0[3];
		s0[3] = str0[2];
		ass_false(utf8::eq(s0, str0) || utf8::eq(str0, s0));
		ass_true(utf8::eq(s0, str0_bad2) && utf8::eq(str0_bad2, s0));
		ass_false(utf8::eq(s0, (const CxU8 *)0) || utf8::eq(0, s0));

		ass_false(utf8::eq(str0, str1) || utf8::eq(str1, str0));
		

		/* test utf8::eq(const CxU8 *, const CxChar *) */
		ass_true(utf8::eq((const CxU8 *)0, (const CxChar *)0));
		
		CxU8 *s1 = utf8::copy("Hello World.");
		ass_true(utf8::eq(s1, s1));
		ass_true(utf8::eq(s1, "Hello World."));
		ass_false(utf8::eq(s1, "Hello World,"));
		ass_false(utf8::eq(s1, "Hello World"));
		ass_false(utf8::eq(s1, (const CxChar *)0));
		ass_false(utf8::eq(0, "Hello World."));
		s1[11] = ',';
		ass_false(utf8::eq(s1, "Hello World."));
		ass_true(utf8::eq(s1, "Hello World,"));

		/* Test utf8::eq(const CxU8 *, const CxU8 *, CxI32) */
		CxU8 * s2 = utf8::copy(str0);
	   for (CxI32 i = 1; i < 10; ++i) {
			ass_true(utf8::eq(s2, str0_bad0, i));
			ass_true(utf8::eq(str0_bad0, s2, i));
		}
		ass_false(utf8::eq(s2, str0_bad0, 10) || utf8::eq(str0_bad0, s2, 10));
			

		utf8::free(s0);
		utf8::free(s1);
		utf8::free(s2);

		FINISH_TEST;
	}

	/**
	 * @tests utf8::free(CxU8 *&)
	 */
	void testUtf8Free() {
		BEGIN_TEST;

		CxU8 *s0 = utf8::alloc(10);
		CxU8 *s1 = 0;
		
		ass_true(s0 != 0 && s1 == 0);
		utf8::free(s0);
		ass_true(s0 == 0 && s1 == 0);

		utf8::free(s1);
		ass_true(s0 == 0 && s1 == 0);
		utf8::free(s0);
		ass_true(s0 == 0 && s1 == 0);

		CxU8 ** sn = (CxU8 **)mem::allocZero(sizeof(CxU8 *), 100);
		for (CxI32 i = 0; i < 100; ++i) {
			sn[i] = utf8::alloc(10 + i);  ass_neq(sn[i], 0);
		}

		for (CxI32 i = 0; i < 100; ++i) {
			ass_neq(sn[i], 0);
			utf8::free(sn[i]);
			ass_zero(sn[i]);
		}

		mem::free(sn);
		
		FINISH_TEST;
	}

	/**
	 * @tests utf8::greater(const CxU8 *, const CxU8 *)
	 */
	void testUtf8Greater() {
		BEGIN_TEST;

		CxU8 * s0 = utf8::copy("12345");
		CxU8 * s1 = utf8::copy("12344");

		ass_false(utf8::greater(s0, s0));
		ass_false(utf8::greater(s1, s1));

		ass_true(utf8::greater(s0, s1));
		ass_false(utf8::greater(s1, s0));

		s1[4] = 0; /* "12345" > "1234" */
		ass_true(utf8::greater(s0, s1));
		ass_false(utf8::greater(s1, s0));

		s0[3] = 0; /* "123" < "1234" */
		ass_false(utf8::greater(s0, s1));
		ass_true(utf8::greater(s1, s0));

		ass_true(utf8::greater(s0, 0));
		ass_false(utf8::greater(0, s0));
		s0[1] = 0;
		ass_true(utf8::greater(s0, 0));
		ass_false(utf8::greater(0, s0));

		ass_false(utf8::greater(0, 0));

		ass_true(utf8::greater(str0, str1));
		ass_false(utf8::greater(str1, str0));
		ass_false(utf8::greater(str0, str0));
		ass_false(utf8::greater(str1, str1));

		utf8::free(s0);
		utf8::free(s1);
		FINISH_TEST;
	}

	/**
	 * @tests utf8::less(const CxU8 *, const CxU8 *)
	 */
	void testUtf8Less() {
		BEGIN_TEST;

		CxU8 * s0 = utf8::copy("12345");
		CxU8 * s1 = utf8::copy("12344");

		ass_false(utf8::less(s0, s0));
		ass_false(utf8::less(s1, s1));

		ass_false(utf8::less(s0, s1));
		ass_true(utf8::less(s1, s0));

		s1[4] = 0; /* "12345" > "1234" */
		ass_false(utf8::less(s0, s1));
		ass_true(utf8::less(s1, s0));

		s0[3] = 0; /* "123" < "1234" */
		ass_true(utf8::less(s0, s1));
		ass_false(utf8::less(s1, s0));

		ass_false(utf8::less(s0, 0));
		ass_true(utf8::less(0, s0));
		s0[1] = 0;
		ass_false(utf8::less(s0, 0));
		ass_true(utf8::less(0, s0));

		ass_false(utf8::less(0, 0));

		ass_false(utf8::less(str0, str1));
		ass_true(utf8::less(str1, str0));
		ass_false(utf8::less(str0, str0));
		ass_false(utf8::less(str1, str1));

		utf8::free(s0);
		utf8::free(s1);
		FINISH_TEST;
	}

	/**
	 * @tests utf8::startsWith(const CxU8 *, const CxU8 *)
	 * @tests utf8::startsWith(const CxU8 *, const CxChar *)
	 */
	void testUtf8StartsWith() {
		BEGIN_TEST;

		const CxU8 st1[2] = {'H', 0};
		const CxU8 st2[3] = {'H', 'e', 0};
		const CxU8 st3[4] = {'H', 'e', 'l', 0};
		const CxU8 st9[10] = {'H', 'e', 'l', 'l', 'o', ' ', 0xE2, 0x99, 0xA5, 0};
		const CxU8 st12[13] = {'H', 'e', 'l', 'l', 'o', ' ', 0xE2, 0x99, 0xA5, ' ', 0xE2, 0x87, 0};
		
		const CxU8 st1_bad[2] = {'e', 0};
		const CxU8 st2_bad[3] = {'H', 'l', 0};
		const CxU8 st3_bad[4] = {'H', 'l', 'l', 0};
		const CxU8 st9_bad[10] = {'H', 'e', 'l', 'l', 'o', ' ', 0xE2, 0xA5, 0x99, 0};
		const CxU8 st12_bad[13] = {'H', 'e', 'l', 'l', 'o', ' ', 0xE2, 0x99, 0xA5, ' ', 0xE2, 0xBD, 0};

		/* Test utf8::startsWith(const CxU8 *, const CxU8 *) */
		CxU8 *s0 = utf8::copy(str1);
		ass_true(utf8::startsWith(s0, st1));
		ass_true(utf8::startsWith(s0, st2));
		ass_true(utf8::startsWith(s0, st3));
		ass_true(utf8::startsWith(s0, st9));
		ass_true(utf8::startsWith(s0, st12));
		ass_true(utf8::startsWith(s0, str1));

		ass_false(utf8::startsWith(s0, st1_bad));
		ass_false(utf8::startsWith(s0, st2_bad));
		ass_false(utf8::startsWith(s0, st3_bad));
		ass_false(utf8::startsWith(s0, st9_bad));
		ass_false(utf8::startsWith(s0, st12_bad));

		ass_false(utf8::startsWith(0, (const CxU8 *)0));
		ass_false(utf8::startsWith(s0, (const CxU8 *)0));
		ass_false(utf8::startsWith(0, st1));

		/* Test utf8::startsWith(const CxU8 *, const CxChar *) */
		CxU8 *s1 = utf8::copy("Meow01  ");
		s1[6] = 0xC2;  s1[7] = 0xA2;  /* Cent sign */
		ass_true(utf8::startsWith(s1, "M"));
		ass_true(utf8::startsWith(s1, "Me"));
		ass_true(utf8::startsWith(s1, "Meo"));
		ass_true(utf8::startsWith(s1, "Meow"));
		ass_true(utf8::startsWith(s1, "Meow0"));
		ass_true(utf8::startsWith(s1, "Meow01"));

		ass_false(utf8::startsWith(s1, "0M"));
		ass_false(utf8::startsWith(s1, "M0"));
		ass_false(utf8::startsWith(s1, "m"));
		ass_false(utf8::startsWith(s1, "Mea"));
		ass_false(utf8::startsWith(s1, "aMeo"));
		ass_false(utf8::startsWith(s1, "Meowa"));
		ass_false(utf8::startsWith(s1, "aMeow0"));
		ass_false(utf8::startsWith(s1, "Meow01a"));

		ass_false(utf8::startsWith(0, (const CxChar *)0));
		ass_false(utf8::startsWith(s1, (const CxChar *)0));
		ass_false(utf8::startsWith(0, "sd"));

		utf8::free(s0);
		utf8::free(s1);
		
		FINISH_TEST;
	}

	/**
	 * @tests utf8::fromWideChars(const wchar_t *in_src)
	 */
	void testUtf8FromWideChars() {
		BEGIN_TEST;

		const wchar_t *sw0 = L"Hi \u2764 \u00A9";
		CxU8 su0[10] = {'H', 'i', ' ', 0xE2, 0x9D, 0xA4, ' ', 0xC2, 0xA9, 0};

		CxU8 *s0 = utf8::fromWideChars(0);
		ass_zero(s0);
		s0 = utf8::fromWideChars(sw0);

		for(CxI32 i = 0; i < 10; ++i) { ass_eq(s0[i], su0[i]); }
		utf8::free(s0);

		FINISH_TEST;
	}

	/**
	 * @tests utf8::toWideChars(const CxU8 *in_src)
	 */
	void testUtf8ToWideChars() {
		BEGIN_TEST;

		CxU8 s0[10] = {'H', 'i', ' ', 0xE2, 0x9D, 0xA4, ' ', 0xC2, 0xA9, 0};

		wchar_t *sw0 = utf8::toWideChars(0);
		ass_zero(sw0);
		sw0 = utf8::toWideChars(s0);
		ass_true(wcscmp(sw0, L"Hi \u2764 \u00A9") == 0);

		mem::free(sw0);

		FINISH_TEST;
	}
} // namespace cat

int main(int argc, char **argv) {
	cat::testUtf8Alloc();
	cat::testUtf8Append();
	cat::testUtf8Bytes();
	cat::testUtf8Copy();
	cat::testUtf8EndsWith();
	cat::testUtf8Eq();
	cat::testUtf8Free();
	cat::testUtf8Greater();
	cat::testUtf8Less();
	cat::testUtf8StartsWith();
	cat::testUtf8FromWideChars();
	cat::testUtf8ToWideChars();
	return 0;
}
