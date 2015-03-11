#include "core/testcore.h"
#include "core/geometry/rectf.h"

namespace cc {


	void testRectfCreateAndDestroy() {
		BEGIN_TEST;

		Rectf r1;
		ass_eq(r1.x(), 0); ass_eq(r1.y(), 0); ass_eq(r1.width(), 0); ass_eq(r1.height(), 0);
		ass_eq(r1.left(), 0); ass_eq(r1.right(), 0); ass_eq(r1.top(), 0); ass_eq(r1.bottom(), 0);
		
		Rectf r2(REAL(100.5), REAL(200.5));		
		ass_eq(r2.x(), 0); ass_eq(r2.y(), 0); ass_eq(r2.width(), REAL(100.5)); ass_eq(r2.height(), REAL(200.5));
		ass_eq(r2.left(), 0); ass_eq(r2.right(), REAL(100.5)); ass_eq(r2.top(), 0); ass_eq(r2.bottom(), REAL(-200.5));
		
		Rectf r22(Size2f(REAL(100.5), REAL(200.5)));
		ass_eq(r22.x(), 0); ass_eq(r22.y(), 0); ass_eq(r22.width(), REAL(100.5)); ass_eq(r22.height(), REAL(200.5));
		ass_eq(r22.left(), 0); ass_eq(r22.right(), REAL(100.5)); ass_eq(r22.top(), 0); ass_eq(r22.bottom(), REAL(-200.5));

		Rectf r3(REAL(-50.25), REAL(100.25), REAL(100.5), REAL(200.5));		
		ass_eq(r3.x(), REAL(-50.25)); ass_eq(r3.y(), REAL(100.25)); ass_eq(r3.width(), REAL(100.5)); ass_eq(r3.height(), REAL(200.5));
		ass_eq(r3.left(), REAL(-50.25)); ass_eq(r3.right(), REAL(50.25)); ass_eq(r3.top(), REAL(100.25)); ass_eq(r3.bottom(), REAL(-100.25));

		Rectf r33(Point2f(REAL(-50.25), REAL(100.25)), Size2f(REAL(100.5), REAL(200.5)));		
		ass_eq(r33.x(), REAL(-50.25)); ass_eq(r33.y(), REAL(100.25)); ass_eq(r33.width(), REAL(100.5)); ass_eq(r33.height(), REAL(200.5));
		ass_eq(r33.left(), REAL(-50.25)); ass_eq(r33.right(), REAL(50.25)); ass_eq(r33.top(), REAL(100.25)); ass_eq(r33.bottom(), REAL(-100.25));
		
		Rectf r4(Point2f(REAL(-50.25), REAL(100.25)), Point2f(REAL(50.25), REAL(-100.25)));		
		ass_eq(r4.x(), REAL(-50.25)); ass_eq(r4.y(), REAL(100.25)); ass_eq(r4.width(), REAL(100.5)); ass_eq(r4.height(), REAL(200.5));
		ass_eq(r4.left(), REAL(-50.25)); ass_eq(r4.right(), REAL(50.25)); ass_eq(r4.top(), REAL(100.25)); ass_eq(r4.bottom(), REAL(-100.25));

		r2 = r1;
		ass_eq(r2.x(), 0); ass_eq(r2.y(), 0); ass_eq(r2.width(), 0); ass_eq(r2.height(), 0);
		ass_eq(r2.left(), 0); ass_eq(r2.right(), 0); ass_eq(r2.top(), 0); ass_eq(r2.bottom(), 0);

		r3 = r1;
		ass_eq(r3.x(), 0); ass_eq(r3.y(), 0); ass_eq(r3.width(), 0); ass_eq(r3.height(), 0);
		ass_eq(r3.left(), 0); ass_eq(r3.right(), 0); ass_eq(r3.top(), 0); ass_eq(r3.bottom(), 0);

		FINISH_TEST;
	}

	void testRectfSizeAndPosition() {
		BEGIN_TEST;
		
		Rectf r(Point2f(REAL(-50.25), REAL(100.25)), Point2f(REAL(100.25), REAL(-300.25)));
		ass_eq(r.x(), REAL(-50.25)); ass_eq(r.y(), REAL(100.25)); ass_eq(r.width(), REAL(150.5)); ass_eq(r.height(), REAL(400.5));
		ass_eq(r.left(), REAL(-50.25)); ass_eq(r.right(), REAL(100.25)); ass_eq(r.top(), REAL(100.25)); ass_eq(r.bottom(), REAL(-300.25));
		ass_eq(r.size(), Size2f(REAL(150.5), REAL(400.5)));
		ass_eq(r.topLeft(), Point2f(REAL(-50.25), REAL(100.25)));
		ass_eq(r.bottomRight(), Point2f(REAL(100.25), REAL(-300.25)));
		
		FINISH_TEST;
	}
	

	



} // namespace cc

int main(int argc, char** argv) {
	cc::testRectfCreateAndDestroy();	
	cc::testRectfSizeAndPosition();	

	return 0;
}

