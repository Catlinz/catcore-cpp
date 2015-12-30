#include "core/common/CxMem.h"
#include "core/CxTestCore.h"

namespace cat {

	struct MemTestObj0 {
		float x, y, z;
	};

	/**
	 * @tests mem::alloc(CxU32)
	 * @tests mem::alloc(void *, CxU32)
	 * @tests mem::free(T *&)
	 */
	void testMemAlloc() {
		BEGIN_TEST;

		/* Test mem::alloc(CxU32) */
		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*10);
		ass_neq(a0, 0);

		MemTestObj0 *a1 = (MemTestObj0 *)mem::alloc(sizeof(MemTestObj0)*1);
		ass_neq(a1, 0);

		/* Test mem::alloc(void *, CxU32) */
		a0 = (CxI32 *)mem::alloc(a0, sizeof(CxI32)*5);
		ass_neq(a0, 0);

		CxI32 *a2 = 0;
		ass_zero(a2);
		a2 = (CxI32 *)mem::alloc(a2, sizeof(CxI32)*3);
		ass_neq(a2, 0);
		
		a2[0] = 0;  a2[1] = 1;  a2[2] = 2;
		ass_true(a2[0] == 0 && a2[1] == 1 && a2[2] == 2);
		a2 = (CxI32 *)mem::alloc(a2, sizeof(CxI32)*10);
		ass_true(a2[0] == 0 && a2[1] == 1 && a2[2] == 2);
		a2[3] = 3;  a2[4] = 4;
		a2 = (CxI32 *)mem::alloc(a2, sizeof(CxI32)*2);
		ass_true(a2[0] == 0 && a2[1] == 1);
		

		ass_true(a0 != 0 && a1 != 0 && a2 != 0);
		mem::free(a0);
		ass_true(a0 == 0 && a1 != 0 && a2 != 0);
		mem::free(a2);
		ass_true(a0 == 0 && a1 != 0 && a2 == 0);
		mem::free(a1);
		ass_true(a0 == 0 && a1 == 0 && a2 == 0);
		
		FINISH_TEST;
	}

	/**
	 * @tests mem::allocZero(CxU32, CxU32)
	 * @tests mem::allocZero(void *, CxU32, CxU32)
	 */
	void testMemAllocZero() {
		BEGIN_TEST;

		/* Test mem::allocZero(CxU32, CxU32) */
		CxI32 *a0 = (CxI32 *)mem::allocZero(sizeof(CxI32), 10);
		for (CxI32 i = 0; i < 10; ++i) { ass_eq(a0[i], 0); }

		MemTestObj0 *a1 = (MemTestObj0 *)mem::allocZero(sizeof(MemTestObj0), 2);
		for (CxI32 i = 0; i < 2; ++i) {
			ass_eq(a1[i].x, 0);  ass_eq(a1[i].y, 0);  ass_eq(a1[i].z, 0);
		}

		/* Test mem::allocZero(void *, CxU32, CxU32) */
		a1 = (MemTestObj0 *)mem::allocZero(a1, sizeof(MemTestObj0), 10);
		for (CxI32 i = 0; i < 10; ++i) {
			ass_eq(a1[i].x, 0);  ass_eq(a1[i].y, 0);  ass_eq(a1[i].z, 0);
		}
		
		mem::free(a0);
		mem::free(a1);
		ass_true(a0 == 0 && a1 == 0);
		
		FINISH_TEST;
	}

	/**
	 * @tests mem::compare(const void *, const void *, CxU32)
	 */
	void testMemCompare() {
		BEGIN_TEST;

		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*5);
		CxI32 *a1 = (CxI32 *)mem::alloc(sizeof(CxI32)*4);

		for (int i = 0; i < 4; ++i) { a0[i] = i; a1[i] = i; }
		a0[4] = 100;
		ass_true(mem::compare(a0, a1, sizeof(CxI32)*4) == 0);
		a0[3] = 1;
		ass_true(mem::compare(a0, a1, sizeof(CxI32)*4) < 0);
		a1[3] = 0;
		ass_true(mem::compare(a0, a1, sizeof(CxI32)*4) > 0);

		mem::free(a0);
		mem::free(a1);

		FINISH_TEST;
	}

	/**
	 * @tests mem::copy(void *, const void *, CxU32)
	 * @tests mem::copy(const void *, CxU32)
	 */
	void testMemCopy() {
		BEGIN_TEST;

		/* Test mem::copy(void *, const void *, CxU32) */
		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*5);
		CxI32 *a1 = (CxI32 *)mem::alloc(sizeof(CxI32)*4);

		for (int i = 0; i < 4; ++i) { a0[i] = i; a1[i] = i+1; }
		a0[4] = 100;

		for (int i = 0; i < 4; ++i) { ass_eq(a0[i], i); }
		ass_eq(a0[4], 100);

		CxI32 *a2 = (CxI32 *)mem::copy(a0, a1, sizeof(CxI32)*4);
		for (int i = 0; i < 4; ++i) { ass_eq(a0[i], i+1); }
		ass_eq(a0[4], 100);
		ass_eq(a0, a2);

		mem::free(a0);
		mem::free(a1);
		a2 = 0;

		/* Test mem::copy(const void *, CxU32) */
		CxI32 *a3 = (CxI32 *)mem::alloc(sizeof(CxI32)*100);
		for (CxI32 i = 0; i < 100; ++i) { a3[i] = i*3; }

		CxI32 *a4 = (CxI32 *)mem::copy(a3, sizeof(CxI32)*100);
		for (CxI32 i = 0; i < 100; ++i) { ass_eq(a4[i], i*3); }

		CxI32 *a5 = mem::copy(0, 34);
		ass_zero(a5);
		a5 = mem::copy(a3, 0);
		ass_zero(a5);

		mem::free(a3);
		mem::free(a4);

		FINISH_TEST;
	}

	/**
	 * @tests mem::eq(const void *, const void *)
	 */
	void testMemEq() {
		BEGIN_TEST;

		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*5);
		CxI32 *a1 = (CxI32 *)mem::alloc(sizeof(CxI32)*4);

		for (int i = 0; i < 4; ++i) { a0[i] = i; a1[i] = i; }
		a0[4] = 100;
		ass_true(mem::eq(a0, a1, sizeof(CxI32)*4));
		a0[3] = 1;
		ass_true(!mem::eq(a0, a1, sizeof(CxI32)*4));
		a1[3] = 0;
		ass_true(!mem::eq(a0, a1, sizeof(CxI32)*4));

		mem::free(a0);
		mem::free(a1);

		FINISH_TEST;
	}

	/**
	 * @tests mem::find(void *, CxU8, CxU32)
	 * @tests mem::find(const void *, CxU8, CxU32)
	 */
	void testMemFind() {
		BEGIN_TEST;

		CxI32 *a0 = (CxI32 *)mem::allocZero(sizeof(CxI32), 10);
		a0[5] = 0xff;
		a0[6] = 0x11;
		a0[7] = 0x22;

		CxI32 *a1 = (CxI32 *)mem::find(a0, 0xff, sizeof(CxI32)*10);
		ass_true(a1[0] == 0xff && a1[1] == 0x11 && a1[2] == 0x22);

		const CxI32 *a2 = (const CxI32 *)mem::find(a0, 0xff, sizeof(CxI32)*10);
		ass_true(a2[0] == 0xff && a2[1] == 0x11 && a2[2] == 0x22);

		a1 = (CxI32 *)mem::find(a0, 0x11, sizeof(CxI32)*10);
		ass_true(a1[0] == 0x11 && a1[1] == 0x22);

		mem::free(a0);
		
		FINISH_TEST;
	}
	
	/**
	 * @tests mem::free(T *&)
	 */
	void testMemFree() {
		BEGIN_TEST;

		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*10);
		MemTestObj0 *a1 = (MemTestObj0 *)mem::alloc(sizeof(MemTestObj0)*3);
		CxI32 *a2 = 0;

		ass_true(a0 != 0 && a1 != 0 && a2 == 0);
		mem::free(a0);
		ass_true(a0 == 0 && a1 != 0 && a2 == 0);
		mem::free(a1);
		ass_true(a0 == 0 && a1 == 0 && a2 == 0);
		mem::free(a2);
		ass_true(a0 == 0 && a1 == 0 && a2 == 0);
		mem::free(a0);
		ass_true(a0 == 0 && a1 == 0 && a2 == 0);
		FINISH_TEST;
	}

	/**
	 * @tests mem::greater(const void *, const void *, CxU32)
	 */
	void testMemGreater() {
		BEGIN_TEST;

		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*5);
		CxI32 *a1 = (CxI32 *)mem::alloc(sizeof(CxI32)*4);

		for (int i = 0; i < 4; ++i) { a0[i] = i; a1[i] = i; }
		a0[4] = 100;
		ass_true(!mem::greater(a0, a1, sizeof(CxI32)*4));
		a0[3] = 1;
		ass_true(!mem::greater(a0, a1, sizeof(CxI32)*4));
		a1[3] = 0;
		ass_true(mem::greater(a0, a1, sizeof(CxI32)*4));

		mem::free(a0);
		mem::free(a1);


		FINISH_TEST;
	}

		/**
	 * @tests mem::less(const void *, const void *, CxU32)
	 */
	void testMemLess() {
		BEGIN_TEST;

		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*5);
		CxI32 *a1 = (CxI32 *)mem::alloc(sizeof(CxI32)*4);

		for (int i = 0; i < 4; ++i) { a0[i] = i; a1[i] = i; }
		a0[4] = 100;
		ass_true(!mem::less(a0, a1, sizeof(CxI32)*4));
		a0[3] = 1;
		ass_true(mem::less(a0, a1, sizeof(CxI32)*4));
		a1[3] = 0;
		ass_true(!mem::less(a0, a1, sizeof(CxI32)*4));

		mem::free(a0);
		mem::free(a1);


		FINISH_TEST;
	}

		/**
	 * @tests mem::move(void *, const void *, CxU32)
	 */
	void testMemMove() {
		BEGIN_TEST;

		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*5);
		CxI32 *a1 = (CxI32 *)mem::alloc(sizeof(CxI32)*4);

		for (int i = 0; i < 4; ++i) { a0[i] = i; a1[i] = i+1; }
		a0[4] = 100;

		for (int i = 0; i < 4; ++i) { ass_eq(a0[i], i); }
		ass_eq(a0[4], 100);

		/* Test moving to separate memory */
		CxI32 *a2 = (CxI32 *)mem::move(a0, a1, sizeof(CxI32)*4);
		for (int i = 0; i < 4; ++i) { ass_eq(a0[i], i+1); }
		ass_eq(a0[4], 100);
		ass_eq(a0, a2);

		/* Test moving to overlapping memory */
		CxI32 *a3 = (CxI32 *)mem::move(a0, a0 + 1, sizeof(CxI32)*4);
		for (int i = 0; i < 3; ++i) { ass_eq(a0[i], i+2); }
	   ass_eq(a0[3], 100);
		ass_eq(a0[4], 100);
		ass_eq(a3, a0);

		mem::free(a0);
		mem::free(a1);
		a2 = 0;
		a3 = 0;

		FINISH_TEST;
	}

	/**
	 * @test mem::resize(void *, CxU32)
	 */
	void testMemResize() {
		BEGIN_TEST;

		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*4);
		for (int i = 0; i < 4; ++i ){ a0[i] = i; }
		for (int i = 0; i < 4; ++i) { ass_eq(a0[i], i); }

	   a0 = (CxI32 *)mem::resize(a0, sizeof(CxI32)*10);
		for (int i = 0; i < 4; ++i) { ass_eq(a0[i], i); }

	   a0 = (CxI32 *)mem::resize(a0, sizeof(CxI32)*3);
		for (int i = 0; i < 3; ++i) { ass_eq(a0[i], i); }

		mem::free(a0);

		FINISH_TEST;
	}

	/**
	 * @test mem::set(void *, CxU8, CxU32)
	 * @test mem::zero(void *, CxU32)
	 */
	void testMemSetAndZero() {
		BEGIN_TEST;

		CxI32 *a0 = (CxI32 *)mem::alloc(sizeof(CxI32)*10);
		for (int i = 0; i < 10; ++i) { a0[i] = i; }
		for (int i = 0; i < 10; ++i) { ass_eq(a0[i], i); }

	   CxI32 *a1 = (CxI32 *)mem::set(a0, 1, sizeof(CxI32)*5);
		for (int i = 0; i < 5; ++i) { ass_eq(a0[i], 0x01010101); }
		for (int i = 5; i < 10; ++i) { ass_eq(a0[i], i); }
		ass_eq(a1, a0);

		CxI32 *a2 = (CxI32 *)mem::set(a1, 2, sizeof(CxI32)*10);
		for (int i = 0; i < 10; ++i) { ass_eq(a0[i], 0x02020202); }
		ass_eq(a2, a0);

		CxI32 *a3 = (CxI32 *)mem::zero(a0, sizeof(CxI32)*10);
		for (int i = 0; i < 10; ++i) { ass_eq(a0[i], 0); }
		ass_eq(a3, a0);

		mem::free(a0);
		a1 = a2 = a3 = 0;
		
		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	
	cat::testMemAlloc();
	cat::testMemAllocZero();
	cat::testMemCompare();
	cat::testMemCopy();
	cat::testMemEq();
	cat::testMemFind();
	cat::testMemFree();
	cat::testMemGreater();
	cat::testMemLess();
	cat::testMemMove();
	cat::testMemResize();
	cat::testMemSetAndZero();
	
	return 0;
}
