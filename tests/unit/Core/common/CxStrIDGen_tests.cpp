#include "core/common/CxStrIDGen.h"
#include "core/CxTestCore.h"


namespace cat {

	/**
	 * @tests CxStrIDGen()
	 * @tests CxStrIDGen(const CxChar *)
	 * @tests CxStrIDGen(const CxStrIDGen &)
	 * @tests ~CxStrID()
	 * @tests operator=(const CxStrIDGen &)
	 * @tests initWithPrefix(const CxChar *)
	 * @tests maxLength()
	 * @tests prefix()
	 */
	void testCxStrIDGenCreateAndDestroy() {
		BEGIN_TEST;

		CxStrIDGen g0;
		ass_zero(g0.maxLength());
		ass_zero(g0.prefix());

		CxStrIDGen g1("Meow");
		ass_eq(g1.maxLength(), 16);
		ass_true(strcmp(g1.prefix(), "Meow") == 0);

		g0 = CxStrIDGen(g1);
		ass_eq(g0.maxLength(), 16);
		ass_true(strcmp(g0.prefix(), "Meow") == 0);
		ass_eq(g1.maxLength(), 16);
		ass_true(strcmp(g1.prefix(), "Meow") == 0);
		ass_true(g0.prefix() != g1.prefix());

		g1 = CxStrIDGen("Moo");
		ass_eq(g1.maxLength(), 15);
		ass_true(strcmp(g1.prefix(), "Moo") == 0);

		g1 = CxStrIDGen();
		ass_zero(g1.maxLength());
		ass_zero(g1.prefix());
		
		FINISH_TEST;
	}

	/**
	 * @tests generate()
	 */
	void testCxStrIDGenGenerate() {
		BEGIN_TEST;

		CxStrIDGen g0("Meow");
		CxStrIDGen g1("Moo");

		CxStrID id = g0.generate();
		assert(strcmp(id.str(), "Meow_10000000000") == 0);
		id = g0.generate();
		assert(strcmp(id.str(), "Meow_20000000000") == 0);
		id = g0.generate();
		id = g0.generate();
		id = g0.generate();
		id = g0.generate();
		id = g0.generate();
		id = g0.generate();
		id = g0.generate();
		id = g0.generate();
		assert(strcmp(id.str(), "Meow_a0000000000") == 0);

		id = g1.generate();
		assert(strcmp(id.str(), "Moo_10000000000") == 0);
		id = g0.generate();
		assert(strcmp(id.str(), "Meow_b0000000000") == 0);

		for (CxI32 i = 0; i < 64; ++i) {
			id = g0.generate();
		}
		assert(strcmp(id.str(), "Meow_b1000000000") == 0);
		FINISH_TEST;
	}


} // namespace cat

int main(int argc, char **argv) {
	cat::testCxStrIDGenCreateAndDestroy();
	cat::testCxStrIDGenGenerate();
	return 0;
}

