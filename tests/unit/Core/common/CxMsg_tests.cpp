#include "core/common/CxMsg.h"
#include "core/CxTestCore.h"

namespace cat {

	void testCxMsg() {
		BEGIN_TEST;

		CxMsg m0;
		ass_eq(m0.id(), 0);
		m0 = CxMsg(23);
		ass_eq(m0.id(), 23);

		CxMsg1<CxI32> m1;
		ass_eq(m1.id(), 0);
		m1 = CxMsg1<CxI32>(23, -222);
		ass_eq(m1.id(), 23);
		ass_eq(m1.arg0(), -222);

		CxMsg2<CxF32, CxI32> m2;
		ass_eq(m2.id(), 0);
		m2 = CxMsg2<CxF32, CxI32>(22, 1.5f, -33);
		ass_eq(m2.id(), 22);
		ass_eq(m2.arg0(), 1.5f);
		ass_eq(m2.arg1(), -33);


		CxMsg3<CxF32, CxI32, CxMsg2<CxF32, CxI32> *> m3;
		ass_eq(m3.id(), 0);
		m3 = CxMsg3<CxF32, CxI32, CxMsg2<CxF32, CxI32> *>(22, 1.5f, -33, &m2);
		ass_eq(m3.id(), 22);
		ass_eq(m3.arg0(), 1.5f);
		ass_eq(m3.arg1(), -33);
		ass_eq(m3.arg2(), &m2);

		CxMsg4<CxF32, CxI32, CxMsg2<CxF32, CxI32> *, CxBool> m4;
		ass_eq(m4.id(), 0);
		m4 = CxMsg4<CxF32, CxI32, CxMsg2<CxF32, CxI32> *, CxBool>(22, 1.5f, -33, &m2, true);
		ass_eq(m4.id(), 22);
		ass_eq(m4.arg0(), 1.5f);
		ass_eq(m4.arg1(), -33);
		ass_eq(m4.arg2(), &m2);
		ass_eq(m4.arg3(), true);
		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxMsg();
	

	return 0;
}
