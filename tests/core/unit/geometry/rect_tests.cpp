#include "core/testcore.h"
#include "core/geometry/rect.h"
#include "core/geometry/rectf.h"
#include "core/geometry/recti.h"

namespace cc {


	void testRectCreateAndDestroy() {
		BEGIN_TEST;

		Rect<Real> rr;
		ass_true(rr.top() == 0 && rr.left() == 0 &&
					rr.bottom() == 0 && rr.right() == 0);
		ass_true(rr.width() == 0 && rr.height() == 0);

		Rect<I32> ri;
		ass_true(ri.top() == 0 && ri.left() == 0 &&
					ri.bottom() == 0 && ri.right() == 0);
		ass_true(ri.width() == 0 && ri.height() == 0);

		rr = Rect<Real>(Size2f(20, 30));
		ass_true(rr.top() == 0 && rr.left() == 0 &&
					rr.bottom() == -30 && rr.right() == 20);
		ass_true(rr.width() == 20 && rr.height() == 30);

		ri = Rect<I32>(20, 30);		
		ass_true(ri.top() == 0 && ri.left() == 0 &&
					ri.bottom() == -30 && ri.right() == 20);
		ass_true(ri.width() == 20 && ri.height() == 30);

		rr = Rect<Real>(Point2f(-10.5, 10.5), Point2f(10.5, -10.5));
		ass_true(rr.top() == 10.5 && rr.left() == -10.5 &&
					rr.bottom() == -10.5 && rr.right() == 10.5);
		ass_true(rr.width() == 21 && rr.height() == 21);

		ri = Rect<I32>(Point2i(-10, 10), Size2i(20, 30));		
		ass_true(ri.top() == 10 && ri.left() == -10 &&
					ri.bottom() == -20 && ri.right() == 10);
		ass_true(ri.width() == 20 && ri.height() == 30);

		rr = Rect<Real>(-10.5, 10.5, 21, 21);
		ass_true(rr.top() == 10.5 && rr.left() == -10.5 &&
					rr.bottom() == -10.5 && rr.right() == 10.5);
		ass_true(rr.width() == 21 && rr.height() == 21);

		FINISH_TEST;
	}

	void testRectProperties() {
		BEGIN_TEST;

		Rect<Real> rr(-10.5, 11, 20, 30);
		ass_true(rr.top() == 11 && rr.bottom() == -19 &&
					rr.left() == -10.5 && rr.right() == 9.5);

		ass_eq(rr.bottom(), -19);
		ass_eq(rr.bottomRight(), Point2f(9.5, -19));
		ass_eq(rr.center(), Point2f(-0.5, -4));
		ass_eq(rr.height(), 30);
		ass_false(rr.isEmpty());
		ass_true(rr.isValid());
		ass_eq(rr.left(), -10.5);
		ass_eq(rr.right(), 9.5);
		ass_eq(rr.size(), Size2f(20, 30));
		ass_eq(rr.top(), 11);
		ass_eq(rr.topLeft(), Point2f(-10.5, 11));
		ass_eq(rr.topRight(), Point2f(9.5, 11));
		ass_eq(rr.width(), 20);
		ass_eq(rr.x(), -10.5);
		ass_eq(rr.y(), 11);

		Rect<I32> ri(-10, 11, 19, 30);
		ass_true(ri.top() == 11 && ri.bottom() == -19 &&
					ri.left() == -10 && ri.right() == 9);

		ass_eq(ri.bottom(), -19);
		ass_eq(ri.bottomRight(), Point2i(9, -19));
		ass_eq(ri.center(), Point2f(-0.5, -4));
		ass_eq(ri.height(), 30);
		ass_false(ri.isEmpty());
		ass_true(ri.isValid());
		ass_eq(ri.left(), -10);
		ass_eq(ri.right(), 9);
		ass_eq(ri.size(), Size2i(19, 30));
		ass_eq(ri.top(), 11);
		ass_eq(ri.topLeft(), Point2i(-10, 11));
		ass_eq(ri.topRight(), Point2i(9, 11));
		ass_eq(ri.width(), 19);
		ass_eq(ri.x(), -10);
		ass_eq(ri.y(), 11);
		
		FINISH_TEST;
	}

	void testRectEquality() {
		BEGIN_TEST;

		Rect<Real> rr(-10.5, 11, 20, 30);
		ass_true(rr.top() == 11 && rr.bottom() == -19 &&
					rr.left() == -10.5 && rr.right() == 9.5);

		ass_eq(rr, Rectf(Point2f(-10.5, 11), Size2f(20, 30)));
		ass_neq(rr, Rectf(Point2f(-10.5, 11), Size2f(20.001, 30)));
		ass_neq(rr, Rectf(Point2f(-10.501, 11), Size2f(20, 30)));
		ass_neq(rr, Rectf(Point2f(-10.5, 11), Size2f(20., 30.001)));
		ass_neq(rr, Rectf(Point2f(-10.5, 11.001), Size2f(20, 30)));

		Rect<I32> ri(-10, 11, 19, 30);
		ass_true(ri.top() == 11 && ri.bottom() == -19 &&
					ri.left() == -10 && ri.right() == 9);

		ass_eq(ri, Recti(Point2i(-10, 11), Size2i(19, 30)));
		ass_neq(ri, Recti(Point2i(-10, 11), Size2i(20, 30)));
		ass_neq(ri, Recti(Point2i(-11, 11), Size2i(19, 30)));
		ass_neq(ri, Recti(Point2i(-10, 11), Size2i(19, 31)));
		ass_neq(ri, Recti(Point2i(-10, 10), Size2i(19, 30)));
		
		FINISH_TEST;
	}

	void testRectContainsPoint() {
		BEGIN_TEST;

		Rect<Real> rr(-10.5, 11, 20, 30);
		ass_true(rr.top() == 11 && rr.bottom() == -19 &&
					rr.left() == -10.5 && rr.right() == 9.5);

		ass_true(rr.contains(Point2f(0, 0)));
		ass_true(rr.contains(Point2f(-10.49, 10.99)));
		ass_true(rr.contains(Point2f(9.49, 10.99)));
		ass_true(rr.contains(Point2f(9.49, -18.99)));
		ass_true(rr.contains(Point2f(-10.49, -18.99)));

		ass_false(rr.contains(Point2f(99, -99)));
		ass_false(rr.contains(Point2f(-10.51, 11.01)));
		ass_false(rr.contains(Point2f(9.51, 11.01)));
		ass_false(rr.contains(Point2f(9.51, -19.01)));
		ass_false(rr.contains(Point2f(-10.51, -19.01)));

		Rect<I32> ri(-10, 11, 19, 30);
		ass_true(ri.top() == 11 && ri.bottom() == -19 &&
					ri.left() == -10 && ri.right() == 9);

		ass_true(ri.contains(Point2i(0, 0)));
		ass_true(ri.contains(Point2i(-9, 10)));
		ass_true(ri.contains(Point2i(8, 10)));
		ass_true(ri.contains(Point2i(8, -18)));
		ass_true(ri.contains(Point2i(-9, -18)));

		ass_true(ri.contains(0.5, 1.8));
		ass_true(ri.contains(-9.99, 10.99));
		ass_true(ri.contains(8.99, 10.99));
		ass_true(ri.contains(8.99, -18.99));
		ass_true(ri.contains(-9.99, -18.99));

		ass_false(ri.contains(Point2i(99, -99)));
		ass_false(ri.contains(Point2i(-11, 12)));
		ass_false(ri.contains(Point2i(10, 12)));
		ass_false(ri.contains(Point2i(10, -20)));
		ass_false(ri.contains(Point2i(-11, -20)));
		
		FINISH_TEST;
	}
	

} // namespace cc

int main(int argc, char** argv) {
	cc::testRectCreateAndDestroy();
	cc::testRectProperties();
	cc::testRectEquality();
	cc::testRectContainsPoint();	
	return 0;
}

