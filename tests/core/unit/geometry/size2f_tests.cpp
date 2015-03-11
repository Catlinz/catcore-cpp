#include "core/testcore.h"
#include "core/geometry/size2f.h"

namespace cc {


	void testSize2fCreateAndDestroy() {
		BEGIN_TEST;

		Size2f s1;
		ass_eq(s1.width, 0); ass_eq(s1.height, 0);
		
		Size2f s2(REAL(2.2), REAL(4.3));
		ass_eq(s2.width, REAL(2.2)); ass_eq(s2.height, REAL(4.3));

		s1 = s2;
		ass_eq(s1.width, REAL(2.2)); ass_eq(s1.height, REAL(4.3));

		FINISH_TEST;
	}

	



} // namespace cc

int main(int argc, char** argv) {
	cc::testSize2fCreateAndDestroy();	

	return 0;
}

