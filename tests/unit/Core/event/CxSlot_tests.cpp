#include "core/event/CxSlot.h"
#include "core/CxTestCore.h"

namespace cat {

	void testSlotFn1(void *in_recv, const CxArgs &in_args) {
		ass_true(in_args[0].ptr == in_recv);
		ass_true(in_args[1].f32 == 1.5f);
	}

	/**
	 * @tests CxSlot()
	 * @tests CxSlot(CxSlotFunc, void *)
	 * @tests operator==(const CxSlot &) const
	 * @tests operator!=(const CxSlot &) const
	 * @tests call(const CxArgs &)
	 * @tests recv() const;
	 * @tests slot() const;
	 */
	void testCxSlotCreateDestroyAndCall() {
		BEGIN_TEST;

		CxSlot s0;
		ass_true(s0.recv() == 0 && s0.slot() == 0);
		ass_true(s0 == s0 && !(s0 != s0));

		CxSlot s1(testSlotFn1, &s0);
		ass_true(s1.recv() == &s0 && s1.slot() == &testSlotFn1);
		ass_true(s1 == s1 && !(s1 != s1));
		ass_true(s1 != s0 && s0 != s1);
		s1.call(CxArgs2<void *, CxF32>(&s0, 1.5f));

		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxSlotCreateDestroyAndCall();
	

	return 0;
}
