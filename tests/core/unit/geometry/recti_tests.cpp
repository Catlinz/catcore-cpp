#include "core/testcore.h"
#include "core/geometry/recti.h"

namespace cc {


	void testRectiCreateAndDestroy() {
		BEGIN_TEST;

		Recti r1;
		ass_eq(r1.x(), 0); ass_eq(r1.y(), 0); ass_eq(r1.width(), 0); ass_eq(r1.height(), 0);
		ass_eq(r1.left(), 0); ass_eq(r1.right(), 0); ass_eq(r1.top(), 0); ass_eq(r1.bottom(), 0);
		
		Recti r2(100, 200);		
		ass_eq(r2.x(), 0); ass_eq(r2.y(), 0); ass_eq(r2.width(), 100); ass_eq(r2.height(), 200);
		ass_eq(r2.left(), 0); ass_eq(r2.right(), 100); ass_eq(r2.top(), 0); ass_eq(r2.bottom(), -200);
		
		Recti r22(Size2i(100, 200));
		ass_eq(r22.x(), 0); ass_eq(r22.y(), 0); ass_eq(r22.width(), 100); ass_eq(r22.height(), 200);
		ass_eq(r22.left(), 0); ass_eq(r22.right(), 100); ass_eq(r22.top(), 0); ass_eq(r22.bottom(), -200);

		Recti r3(-50, 100, 100, 200);		
		ass_eq(r3.x(), -50); ass_eq(r3.y(), 100); ass_eq(r3.width(), 100); ass_eq(r3.height(), 200);
		ass_eq(r3.left(), -50); ass_eq(r3.right(), 50); ass_eq(r3.top(), 100); ass_eq(r3.bottom(), -100);

		Recti r33(Point2i(-50, 100), Size2i(100, 200));		
		ass_eq(r33.x(), -50); ass_eq(r33.y(), 100); ass_eq(r33.width(), 100); ass_eq(r33.height(), 200);
		ass_eq(r33.left(), -50); ass_eq(r33.right(), 50); ass_eq(r33.top(), 100); ass_eq(r33.bottom(), -100);
		
		Recti r4(Point2i(-50, 100), Point2i(50, -100));		
		ass_eq(r4.x(), -50); ass_eq(r4.y(), 100); ass_eq(r4.width(), 100); ass_eq(r4.height(), 200);
		ass_eq(r4.left(), -50); ass_eq(r4.right(), 50); ass_eq(r4.top(), 100); ass_eq(r4.bottom(), -100);

		r2 = r1;
		ass_eq(r2.x(), 0); ass_eq(r2.y(), 0); ass_eq(r2.width(), 0); ass_eq(r2.height(), 0);
		ass_eq(r2.left(), 0); ass_eq(r2.right(), 0); ass_eq(r2.top(), 0); ass_eq(r2.bottom(), 0);

		r3 = r1;
		ass_eq(r3.x(), 0); ass_eq(r3.y(), 0); ass_eq(r3.width(), 0); ass_eq(r3.height(), 0);
		ass_eq(r3.left(), 0); ass_eq(r3.right(), 0); ass_eq(r3.top(), 0); ass_eq(r3.bottom(), 0);

		FINISH_TEST;
	}

	void testRectiSizeAndPosition() {
		BEGIN_TEST;
		
		Recti r(Point2i(-50, 100), Point2i(100, -300));
		ass_eq(r.x(), -50); ass_eq(r.y(), 100); ass_eq(r.width(), 150); ass_eq(r.height(), 400);
		ass_eq(r.left(), -50); ass_eq(r.right(), 100); ass_eq(r.top(), 100); ass_eq(r.bottom(), -300);
		ass_eq(r.size(), Size2i(150, 400));
		ass_eq(r.topLeft(), Point2i(-50, 100));
		ass_eq(r.bottomRight(), Point2i(100, -300));
		
		FINISH_TEST;
	}
	

	



} // namespace cc

int main(int argc, char** argv) {
	cc::testRectiCreateAndDestroy();	
	cc::testRectiSizeAndPosition();	

	return 0;
}

