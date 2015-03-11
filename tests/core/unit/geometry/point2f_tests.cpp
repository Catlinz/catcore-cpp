#include "core/testcore.h"
#include "core/geometry/point2f.h"

namespace cc {


	void testPoint2fCreateAndDestroy() {
		BEGIN_TEST;

		Point2f p1;
		ass_eq(p1.x, 0); ass_eq(p1.y, 0);
		
		Point2f p2(2, 4);
		ass_eq(p2.x, 2); ass_eq(p2.y, 4);

		p1 = p2;
		ass_eq(p1.x, 2); ass_eq(p1.y, 4);

		FINISH_TEST;
	}

	



} // namespace cc

int main(int argc, char** argv) {
	cc::testPoint2fCreateAndDestroy();	

	return 0;
}

