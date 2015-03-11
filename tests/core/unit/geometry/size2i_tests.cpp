#include "core/testcore.h"
#include "core/geometry/size2i.h"

namespace cc {


	void testSize2iCreateAndDestroy() {
		BEGIN_TEST;

		Size2i s1;
		ass_eq(s1.width, 0); ass_eq(s1.height, 0);
		
		Size2i s2(2, 4);
		ass_eq(s2.width, 2); ass_eq(s2.height, 4);

		s1 = s2;
		ass_eq(s1.width, 2); ass_eq(s1.height, 4);

		FINISH_TEST;
	}

	



} // namespace cc

int main(int argc, char** argv) {
	cc::testSize2iCreateAndDestroy();	

	return 0;
}

