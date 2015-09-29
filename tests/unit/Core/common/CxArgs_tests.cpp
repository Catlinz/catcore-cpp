#include "core/common/CxArgs.h"
#include "core/CxTestCore.h"

namespace cat {

	void testCxArgsCreateAndDestroyAndAccess() {
		BEGIN_TEST;

		CxArgs1<CxF32> a1;
		ass_eq(a1[0].u64, 0);
		a1 = CxArgs1<CxF32>(1.5f);
		ass_eq(a1[0].f32, 1.5f);

		CxArgs a0 = a1;
		ass_eq(a0[0].f32, 1.5f);

		CxArgs2<CxI32, CxF32> a2;
		ass_true(a2[0].u64 == 0 && a2[1].u64 == 0);
		a2 = CxArgs2<CxI32, CxF32>(-99, 1.5f);
		ass_true(a2[0].i32 == -99 && a2[1].f32 == 1.5f);

		a0 = a2;
	   ass_true(a0[0].i32 == -99 && a0[1].f32 == 1.5f);

		CxArgs3<CxI32, CxF32, CxBool> a3;
		ass_true(a3[0].i32 == 0 && a3[1].f32 == 0 && a3[2].boolean == false);
		a3 = CxArgs3<CxI32, CxF32, CxBool>(20, 1.5f, true);
		ass_true(a3[0].i32 == 20 && a3[1].f32 == 1.5f && a3[2].boolean == true);

		a0 = a3;
		ass_true(a0[0].i32 == 20 && a0[1].f32 == 1.5f && a0[2].boolean == true);

		CxArgs4<CxF32, CxI32, CxBool, void *> a4;
		ass_true(a4[0].f32 == 0 && a4[1].i32 == 0 && a4[2].boolean == false && a4[3].ptr == 0);
		a4 = CxArgs4<CxF32, CxI32, CxBool, void *>(2.5f, -342, true, &a0);
		ass_true(a4[0].f32 == 2.5f && a4[1].i32 == -342 && a4[2].boolean == true && a4[3].ptr == &a0);

		a0 = a4;
		ass_true(a0[0].f32 == 2.5f && a0[1].i32 == -342 && a0[2].boolean == true && a0[3].ptr == &a0);

		a0 = CxArgs1<CxI32>(1000);
		ass_true(a0[0].i32 == 1000);

		a0 = CxArgs2<CxI32, CxF32>(-99, 1.5f);
		ass_true(a0[0].i32 == -99 && a0[1].f32 == 1.5f);
		
		a0 = CxArgs3<CxI32, CxF32, CxBool>(20, 1.5f, true);
		ass_true(a0[0].i32 == 20 && a0[1].f32 == 1.5f && a0[2].boolean == true);
		
		a0 = CxArgs4<CxF32, CxI32, CxBool, void *>(2.5f, -342, true, &a0);
		ass_true(a0[0].f32 == 2.5f && a0[1].i32 == -342 && a0[2].boolean == true && a0[3].ptr == &a0);

		CxArgsN<3> an3;
		an3[0].i32 = 100;
		an3[1].f32 = 1.5f;
		an3[2].boolean = true;

		ass_true(an3[0].i32 == 100 && an3[1].f32 == 1.5f && an3[2].boolean == true);

		a0 = an3;
		ass_true(a0[0].i32 == 100 && a0[1].f32 == 1.5f && a0[2].boolean == true);

		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxArgsCreateAndDestroyAndAccess();
	

	return 0;
}
