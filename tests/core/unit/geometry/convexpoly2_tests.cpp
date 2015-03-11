#include "core/testcore.h"
#include "core/geometry/convexpoly2.h"
#include "core/geometry/convexpoly2f.h"
#include "core/geometry/convexpoly2i.h"
#include "core/math/vec2f.h"

namespace cc {


	void testConvexPolyCreateAndDestroyEmpty() {
		BEGIN_TEST;

		ConvexPoly2<Real> pr;
		ass_true(pr.isEmpty());
		ass_false(pr.isValid());
		ass_true(pr.rect().isEmpty());
		ass_eq(pr.size(), Size2f(0, 0));
		ass_eq(pr.count(), 0);		
		ass_true(pr.top() == 0 && pr.bottom() == 0 && pr.left() == 0 && pr.right() == 0);
		ass_true(pr.width() == 0 && pr.height() == 0);
		DMSG("pr = " << pr << ".");	

		ConvexPoly2<I32> pi(4);
		ass_true(pi.isEmpty());
		ass_false(pi.isValid());
		ass_true(pi.rect().isEmpty());
		ass_eq(pi.size(), Size2i(0, 0));
		ass_eq(pi.count(), 0);		
		ass_true(pi.top() == 0 && pi.bottom() == 0 && pi.left() == 0 && pi.right() == 0);
		ass_true(pi.width() == 0 && pi.height() == 0);
		DMSG("pi = " << pi << ".");	

		FINISH_TEST;
	}

	void testConvexPolyCreateAndDestroyFromRect() {
		BEGIN_TEST;

		Rectf rf(Point2f(-10.5, 10.5), Size2f(21, 21));		
		
		ConvexPoly2<Real> pr(rf);
		ass_false(pr.isEmpty());
		ass_true(pr.isValid());
		ass_false(pr.rect().isEmpty());
		ass_eq(pr.rect(), rf);		
		ass_eq(pr.size(), Size2f(21, 21));
		ass_eq(pr.count(), 4);		
		ass_true(pr.top() == 10.5 && pr.bottom() == -10.5 && pr.left() == -10.5 && pr.right() == 10.5);
		ass_true(pr.width() == 21 && pr.height() == 21);
		DMSG("pr = " << pr << ".");

		ConvexPoly2<Real> pr2 = ConvexPoly2f::fromRect(rf);		
		ass_false(pr2.isEmpty());
		ass_true(pr2.isValid());
		ass_false(pr2.rect().isEmpty());
		ass_eq(pr2.rect(), rf);		
		ass_eq(pr2.size(), Size2f(21, 21));
		ass_eq(pr2.count(), 4);		
		ass_true(pr2.top() == 10.5 && pr2.bottom() == -10.5 && pr2.left() == -10.5 && pr2.right() == 10.5);
		ass_true(pr2.width() == 21 && pr2.height() == 21);
		DMSG("pr = " << pr << ".");	

		Recti ri(Point2i(-10, 10), Size2i(20, 20));	

		ConvexPoly2<I32> pi(ri);
		ass_false(pi.isEmpty());
		ass_true(pi.isValid());
		ass_false(pi.rect().isEmpty());
		ass_eq(pi.rect(), ri);		
		ass_eq(pi.size(), Size2i(20, 20));
		ass_eq(pi.count(), 4);		
		ass_true(pi.top() == 10 && pi.bottom() == -10 && pi.left() == -10 && pi.right() == 10);
		ass_true(pi.width() == 20 && pi.height() == 20);
		DMSG("pi = " << pi << ".");	

		FINISH_TEST;
	}

	void testConvexPolyCreateAndDestroyFromVector() {
		BEGIN_TEST;

		/* Rectangle */
		Vector<Point2f> vf(4);
		vf.append(Point2f(-10.5, -10.5));
		vf.append(Point2f(10.5, 10.5));
		vf.append(Point2f(10.5, -10.5));
		vf.append(Point2f(-10.5, 10.5));		
		
		ConvexPoly2<Real> pr(vf);
		DMSG("pr = " << pr << ".");
		ass_false(pr.isEmpty());
		ass_true(pr.isValid());
		ass_false(pr.rect().isEmpty());
		ass_eq(pr.rect(), Rectf(Point2f(-10.5, 10.5), Size2f(21, 21)));		
		ass_eq(pr.size(), Size2f(21, 21));
		ass_eq(pr.count(), 4);		
		ass_true(pr.top() == 10.5 && pr.bottom() == -10.5 && pr.left() == -10.5 && pr.right() == 10.5);
		ass_true(pr.width() == 21 && pr.height() == 21);
		

		/* Rectangle with beveled top corners */
		Vector<Point2f> vf2(8);
		vf2.append(Point2f(-10.5, -10.5));
		vf2.append(Point2f(9.5, 10.5));
		vf2.append(Point2f(10.5, 9.5));
		vf2.append(Point2f(10.5, -10.5));
		vf2.append(Point2f(-9.5, 10.5));
		vf2.append(Point2f(-10.5, 9.5));
		vf2.append(Point2f(0, 0));
		vf2.append(Point2f(0, 10.5));		
		
		pr = ConvexPoly2f(vf2);
		DMSG("pr = " << pr << ".");
		ass_false(pr.isEmpty());
		ass_true(pr.isValid());
		ass_false(pr.rect().isEmpty());
		ass_eq(pr.rect(), Rectf(Point2f(-10.5, 10.5), Size2f(21, 21)));		
		ass_eq(pr.size(), Size2f(21, 21));
		ass_eq(pr.count(), 6);		
		ass_true(pr.top() == 10.5 && pr.bottom() == -10.5 && pr.left() == -10.5 && pr.right() == 10.5);
		ass_true(pr.width() == 21 && pr.height() == 21);

		/* Triangle */
		Vector<Point2f> vf3(4);
		vf3.append(Point2f(0, 10.5));
		vf3.append(Point2f(-4, -0.1));
		vf3.append(Point2f(0, 4));		
		vf3.append(Point2f(3, 2));		
	   	
		pr = ConvexPoly2f(vf3);
		DMSG("pr = " << pr << ".");
		ass_false(pr.isEmpty());
		ass_true(pr.isValid());
		ass_false(pr.rect().isEmpty());
		ass_eq(pr.rect(), Rectf(Point2f(-4, 10.5), Size2f(7, 10.6)));		
		ass_eq(pr.size(), Size2f(7, 10.6));
		ass_eq(pr.count(), 3);	
		ass_true(pr.top() == 10.5 && approx(pr.bottom(), -0.1) && pr.left() == -4 && pr.right() == 3);
		ass_true(pr.width() == REAL(7.0) && pr.height() == REAL(10.6));

		/* Triangle with extra vertex on bottom (diamondish) */
		Vector<Point2f> vf4(4);
		vf4.append(Point2f(0, 10.5));
		vf4.append(Point2f(-4, -0.1));
		vf4.append(Point2f(0, 0));		
		vf4.append(Point2f(3, 2));		
	   	
		
		pr = ConvexPoly2f(vf4);
		DMSG("pr = " << pr << ".");
		ass_false(pr.isEmpty());
		ass_true(pr.isValid());
		ass_false(pr.rect().isEmpty());
		ass_eq(pr.rect(), Rectf(Point2f(-4, 10.5), Size2f(7, 10.6)));		
		ass_eq(pr.size(), Size2f(7, 10.6));
		ass_eq(pr.count(), 4);	
		ass_true(pr.top() == 10.5 && approx(pr.bottom(), -0.1) && pr.left() == -4 && pr.right() == 3);
		ass_true(pr.width() == REAL(7.0) && pr.height() == REAL(10.6));

		/* Another Triangle */
		Vector<Point2f> vf5(3);
		vf5.append(Point2f(1.712, 8.698));
		vf5.append(Point2f(10.388, 6.280));	
		vf5.append(Point2f(6.361, 5.351));		
	   	
		pr = ConvexPoly2f(vf5);
		DMSG("pr = " << pr << ".");
		ass_false(pr.isEmpty());
		ass_true(pr.isValid());
		ass_false(pr.rect().isEmpty());
		ass_true(approx(pr.size().width, 8.676) && approx(pr.size().height, 3.347));		
		ass_eq(pr.count(), 3);	
		ass_true(approx(pr.top(), 8.698) && approx(pr.bottom(), 5.351) && approx(pr.left(), 1.712) && approx(pr.right(), 10.388));
		ass_true(approx(pr.width(), 8.676) && approx(pr.height(), 3.347));

		FINISH_TEST;
	}


	void testConvexPolyEquality() {
		BEGIN_TEST;

		/* Rectangle with beveled top corners */
		Vector<Point2f> vf(8);
		vf.append(Point2f(-10.5, -10.5));
		vf.append(Point2f(9.5, 10.5));
		vf.append(Point2f(10.5, 9.5));
		vf.append(Point2f(10.5, -10.5));
		vf.append(Point2f(-9.5, 10.5));
		vf.append(Point2f(-10.5, 9.5));
		vf.append(Point2f(0, 0));
		vf.append(Point2f(0, 10.5));		
		
		ConvexPoly2<Real> pr = ConvexPoly2f(vf);
		ass_false(pr.isEmpty());
		ass_true(pr.isValid());
		ass_false(pr.rect().isEmpty());
		ass_eq(pr.rect(), Rectf(Point2f(-10.5, 10.5), Size2f(21, 21)));		
		ass_eq(pr.size(), Size2f(21, 21));
		ass_eq(pr.count(), 6);		
		ass_true(pr.top() == 10.5 && pr.bottom() == -10.5 && pr.left() == -10.5 && pr.right() == 10.5);
		ass_true(pr.width() == 21 && pr.height() == 21);

		Vector<Point2f> vf2(8);
		vf2.append(Point2f(-9.5, 10.5));
		vf2.append(Point2f(-10.5, 9.5));
		vf2.append(Point2f(9.5, 10.5));
		vf2.append(Point2f(10.5, 9.5));
		vf2.append(Point2f(0, 0));
		vf2.append(Point2f(0, 10.5));	
		vf2.append(Point2f(10.5, -10.5));
		vf2.append(Point2f(-10.5, -10.5));

		ConvexPoly2<Real> pr2;
		ass_neq(pr, pr2);
		ass_true(pr != pr2 && pr2 != pr);
		ass_false(pr == pr2 || pr2 == pr);
		
		pr2.setToConvexHull(vf2);
		ass_eq(pr, pr2);
		ass_true(pr == pr2 && pr2 == pr);
		ass_false(pr != pr2 || pr2 != pr);

		Vector<Point2f> vf3(8);
		vf3.append(Point2f(-9.5, 10.5));
		vf3.append(Point2f(-10.5, 9.5));
		vf3.append(Point2f(9.5, 10.5));
		vf3.append(Point2f(10.5, 8.5));
		vf3.append(Point2f(0, 0));
		vf3.append(Point2f(0, 10.5));	
		vf3.append(Point2f(10.5, -10.5));
		vf3.append(Point2f(-10.5, -10.5));

		pr2.setToConvexHull(vf3);
		ass_neq(pr2, pr);
		ass_true(pr != pr2 && pr2 != pr);
		ass_false(pr == pr2 || pr2 == pr);
		pr.setToConvexHull(vf3);
		ass_eq(pr, pr2);
		ass_true(pr == pr2 && pr2 == pr);
		ass_false(pr != pr2 || pr2 != pr);
		
		
		FINISH_TEST;
	}

	void testConvexPolyContainsPoint() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> pr(points);
		DMSG("pr = " << pr << ".");
		ass_false(pr.isEmpty());
		ass_true(pr.isValid());
		ass_false(pr.rect().isEmpty());	
		ass_true(approx(pr.size().width, 8.875) && approx(pr.size().height, 6.937));		
		ass_eq(pr.count(), 6);	
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) && approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));

		ass_true(pr.contains(2.52, 5.45));
		ass_true(pr.contains(-1.31, 7.52));

		ass_true(pr.contains(-1.51, 8.7));
		ass_false(pr.contains(-1.61, 8.78));

		ass_true(pr.contains(1.04, 10.77));
		ass_false(pr.contains(1.08, 10.96));

		ass_true(pr.contains(3.66, 11.93));
		ass_false(pr.contains(3.73, 12.07));

		ass_true(pr.contains(5.65, 5.80));
		ass_false(pr.contains(5.72, 5.73));

		ass_true(pr.contains(3.63, 5.43));
		ass_false(pr.contains(3.62, 5.35));

		ass_true(pr.contains(-0.16, 6.36));
		ass_false(pr.contains(-0.33, 6.39));
		
		ass_false(pr.contains(0.99, 11.04));
		ass_false(pr.contains(6.08, 6.40));
		ass_false(pr.contains(-0.2, 6.1));
		ass_false(pr.contains(4.41, 5.36));
		ass_false(pr.contains(-1.58, 9.15));
		
		FINISH_TEST;
	}

	void testConvexPolyContainsPolygon() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> pr(points);
		DMSG("pr = " << pr << ".");
		ass_false(pr.isEmpty());
		ass_true(pr.isValid());
		ass_false(pr.rect().isEmpty());	
		ass_true(approx(pr.size().width, 8.875) && approx(pr.size().height, 6.937));		
		ass_eq(pr.count(), 6);	
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) && approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));


		/* Triangle inside */
		Vector<Point2f> tp(3);
		tp.append(Point2f(3.010, 10.305));
		tp.append(Point2f(1.242, 7.980));
		tp.append(Point2f(4.730, 6.976));

		ConvexPoly2<Real> pr2(tp);
		DMSG("pr2 = " << pr2 << ".");
		ass_false(pr2.isEmpty());
		ass_true(pr2.isValid());
		ass_false(pr2.rect().isEmpty());	
		ass_eq(pr2.count(), 3);	
		ass_true(approx(pr2.top(), 10.305) && approx(pr2.bottom(), 6.976) &&
					approx(pr2.left(), 1.242) && approx(pr2.right(), 4.730));

		ass_true(pr.contains(pr2));
		ass_false(pr2.contains(pr));

		/* Triangle just outside */
		Vector<Point2f> tp2(3);
		tp2.append(Point2f(-1.8, 7.9));
		tp2.append(Point2f(-0.032, 10.305));
		tp2.append(Point2f(1.688, 6.976));

		ConvexPoly2<Real> pr3(tp2);
		DMSG("pr3 = " << pr3 << ".");
		ass_false(pr3.isEmpty());
		ass_true(pr3.isValid());
		ass_false(pr3.rect().isEmpty());	
		ass_eq(pr3.count(), 3);	
		ass_true(approx(pr3.top(), 10.305) && approx(pr3.bottom(), 6.976) &&
					approx(pr3.left(), -1.8) && approx(pr3.right(), 1.688));

	   ass_false(pr.contains(pr3));
		ass_false(pr2.contains(pr3));
		ass_false(pr3.contains(pr));
		ass_false(pr3.contains(pr2));
		
		
		FINISH_TEST;
	}

	void testConvexPolyMoveBottomTo() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));

		pr.moveBottomTo(-1.232);
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 5.705) && approx(pr.bottom(), -1.232) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
		ass_false(pr.approx(original) || original.approx(pr));
		for (Size i = 0; i < original.count(); ++i) {
			ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y - 6.458));
			ass_eq(pr.pointAt(i).x, original.pointAt(i).x);			
		}
		pr.moveBottomTo(5.226);
		ass_eq(pr, original);
		ass_true(pr.approx(original) && original.approx(pr));		
		

		FINISH_TEST;
	}	


	void testConvexPolyMoveBottomLeftTo() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));

		pr.moveBottomLeftTo(Point2f(4.3, 1.88));
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 8.817) && approx(pr.bottom(), 1.88) &&
					approx(pr.left(), 4.3) && approx(pr.right(), 13.175));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y - 3.346));
		   ass_true(approx(pr.pointAt(i).x, original.pointAt(i).x + 6.102));		
		}
		pr.moveBottomLeftTo(Point2f(-1.802, 5.226));

		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}

	void testConvexPolyMoveBottomRightTo() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));

		pr.moveBottomRightTo(Point2f(10, 3));
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 9.937) && approx(pr.bottom(), 3) &&
					approx(pr.left(), 1.125) && approx(pr.right(), 10));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y - 2.226));
		   ass_true(approx(pr.pointAt(i).x, original.pointAt(i).x + 2.927));		
		}
		pr.moveBottomLeftTo(Point2f(-1.802, 5.226));

		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}

	void testConvexPolyMoveCenterTo() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.moveCenterTo(Point2f(-1, 12));
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 15.4685) && approx(pr.bottom(), 8.5315) &&
					approx(pr.left(), -5.4375) && approx(pr.right(), 3.4375));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y + 3.3055));
		   ass_true(approx(pr.pointAt(i).x, original.pointAt(i).x - 3.6355));		
		}
		pr.moveCenterTo(Point2f(2.6355, 8.6945));
		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}

	void testConvexPolyMoveLeftTo() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.moveLeftTo(10.4);
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), 10.4) && approx(pr.right(), 19.275));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			//ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y + 3.3055));
			ass_eq(pr.pointAt(i).y, original.pointAt(i).y);			
			ass_true(approx(pr.pointAt(i).x, original.pointAt(i).x + 12.202));		
		}
		pr.moveLeftTo(-1.802);
		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}

	void testConvexPolyMoveRightTo() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.moveRightTo(0);
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -8.875) && approx(pr.right(), 0));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			//ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y + 3.3055));
			ass_eq(pr.pointAt(i).y, original.pointAt(i).y);			
			ass_true(approx(pr.pointAt(i).x, original.pointAt(i).x - 7.073));		
		}
		pr.moveRightTo(7.073);
		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}

	void testConvexPolyMoveTo() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.moveTo(Point2f(0, 0));
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 0) && approx(pr.bottom(), -6.937) &&
					approx(pr.left(), 0) && approx(pr.right(), 8.875));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y - 12.163));
			ass_true(approx(pr.pointAt(i).x, original.pointAt(i).x + 1.802));				
		}
		pr.moveTo(-1.802, 12.163);
		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}	

	void testConvexPolyMoveTopTo() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.moveTopTo(0);
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 0) && approx(pr.bottom(), -6.937) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y - 12.163));
			ass_eq(pr.pointAt(i).x, original.pointAt(i).x);					
		}
		pr.moveTopTo(12.163);
		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}

	void testConvexPolyMoveTopRightTo() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.moveTopRightTo(Point2f(0, 0));
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 0) && approx(pr.bottom(), -6.937) &&
					approx(pr.left(), -8.875) && approx(pr.right(), 0));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y - 12.163));
			ass_true(approx(pr.pointAt(i).x, original.pointAt(i).x - 7.073));				
		}
		pr.moveTopRightTo(Point2f(7.073, 12.163));
		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}

	void testConvexPolySetBottom() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setBottom(1.2);		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 1.2) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 10.963));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).y, 5.04345));
		ass_true(approx(pr.pointAt(1).y, 8.17732));
		ass_true(approx(pr.pointAt(2).y, 12.163));
		ass_true(approx(pr.pointAt(3).y, 7.32234));
		ass_true(approx(pr.pointAt(4).y, 1.76893));
		ass_true(approx(pr.pointAt(5).y, 1.2));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_eq(pr.pointAt(i).x, original.pointAt(i).x);			
		}
		pr.setBottom(5.226);
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolySetBottomLeft() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setBottomLeft(Point2f(4.3, 1.2));		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 1.2) &&
					approx(pr.left(), 4.3) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 2.773) && approx(pr.height(), 10.963));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).y, 5.04345));
		ass_true(approx(pr.pointAt(0).x, 4.3));
		ass_true(approx(pr.pointAt(1).y, 8.17732));
		ass_true(approx(pr.pointAt(1).x, 4.42654));
		ass_true(approx(pr.pointAt(2).y, 12.163));
		ass_true(approx(pr.pointAt(2).x, 5.96099));
		ass_true(approx(pr.pointAt(3).y, 7.32234));
		ass_true(approx(pr.pointAt(3).x, 7.073));
		ass_true(approx(pr.pointAt(4).y, 1.76893));
		ass_true(approx(pr.pointAt(4).x, 6.60839));
		ass_true(approx(pr.pointAt(5).y, 1.2));
		ass_true(approx(pr.pointAt(5).x, 5.21486));	

		pr.setBottomLeft(Point2f(-1.802, 5.226));
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolySetBottomRight() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setBottomRight(Point2f(10, 1.2));		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 1.2) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 10));
		ass_true(approx(pr.width(), 11.802) && approx(pr.height(), 10.963));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).y, 5.04345));
		ass_true(approx(pr.pointAt(0).x, -1.802));
		ass_true(approx(pr.pointAt(1).y, 8.17732));
		ass_true(approx(pr.pointAt(1).x, -1.26343));
		ass_true(approx(pr.pointAt(2).y, 12.163));
		ass_true(approx(pr.pointAt(2).x, 5.26723));
		ass_true(approx(pr.pointAt(3).y, 7.32234));
		ass_true(approx(pr.pointAt(3).x, 10));
		ass_true(approx(pr.pointAt(4).y, 1.76893));
		ass_true(approx(pr.pointAt(4).x, 8.02258));
		ass_true(approx(pr.pointAt(5).y, 1.2));
		ass_true(approx(pr.pointAt(5).x, 2.09166));		

		pr.setBottomRight(Point2f(7.073, 5.226));
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolySetHeight() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setHeight(4);		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 8.163) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 4));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).y, 9.56534));
		ass_true(approx(pr.pointAt(1).y, 10.7088));
		ass_true(approx(pr.pointAt(2).y, 12.163));
		ass_true(approx(pr.pointAt(3).y, 10.3968));
		ass_true(approx(pr.pointAt(4).y, 8.37058));
		ass_true(approx(pr.pointAt(5).y, 8.163));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_eq(pr.pointAt(i).x, original.pointAt(i).x);			
		}
		pr.setHeight(6.937);
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolySetLeft() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setLeft(4.3);		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), 4.3) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 2.773) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).x, 4.3));
		ass_true(approx(pr.pointAt(1).x, 4.42654));
		ass_true(approx(pr.pointAt(2).x, 5.96099));
		ass_true(approx(pr.pointAt(3).x, 7.073));
		ass_true(approx(pr.pointAt(4).x, 6.60839));
		ass_true(approx(pr.pointAt(5).x, 5.21486));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_eq(pr.pointAt(i).y, original.pointAt(i).y);			
		}
		pr.setLeft(-1.802);
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolySetBounds() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setBounds(Rectf(-3.5, 3.5, 7.0, 7.0));		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 3.5) && approx(pr.bottom(), -3.5) &&
					approx(pr.left(), -3.5) && approx(pr.right(), 3.5));
		ass_true(approx(pr.width(), 7) && approx(pr.height(), 7));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).y, -1.04591));
		ass_true(approx(pr.pointAt(0).x, -3.5));
		ass_true(approx(pr.pointAt(1).y, 0.955095));
		ass_true(approx(pr.pointAt(1).x, -3.18056));
		ass_true(approx(pr.pointAt(2).y, 3.5));
		ass_true(approx(pr.pointAt(2).x, 0.692901));
		ass_true(approx(pr.pointAt(3).y, 0.409184));
		ass_true(approx(pr.pointAt(3).x, 3.5));
		ass_true(approx(pr.pointAt(4).y, -3.13673));
		ass_true(approx(pr.pointAt(4).x, 2.32716));
		ass_true(approx(pr.pointAt(5).y, -3.5));
		ass_true(approx(pr.pointAt(5).x, -1.19059));		

		pr.setBounds(Rectf(-1.802, 12.163, 8.875, 6.937));
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolySetRight() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setRight(10);		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 10));
		ass_true(approx(pr.width(), 11.802) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).x, -1.802));
		ass_true(approx(pr.pointAt(1).x, -1.26343));
		ass_true(approx(pr.pointAt(2).x, 5.26723));
		ass_true(approx(pr.pointAt(3).x, 10));
		ass_true(approx(pr.pointAt(4).x, 8.02258));
		ass_true(approx(pr.pointAt(5).x, 2.09166));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_eq(pr.pointAt(i).y, original.pointAt(i).y);			
		}
		pr.setRight(7.073);
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolySetSize() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setSize(Size2f(10, 4));		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 8.163) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 8.198));
		ass_true(approx(pr.width(), 10) && approx(pr.height(), 4));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).y, 9.56534));
		ass_true(approx(pr.pointAt(0).x, -1.802));
		ass_true(approx(pr.pointAt(1).y, 10.7088));
		ass_true(approx(pr.pointAt(1).x, -1.34566));
		ass_true(approx(pr.pointAt(2).y, 12.163));
		ass_true(approx(pr.pointAt(2).x, 4.18786));
		ass_true(approx(pr.pointAt(3).y, 10.3968));
		ass_true(approx(pr.pointAt(3).x, 8.198));
		ass_true(approx(pr.pointAt(4).y, 8.37058));
		ass_true(approx(pr.pointAt(4).x, 6.52251));
		ass_true(approx(pr.pointAt(5).y, 8.163));
		ass_true(approx(pr.pointAt(5).x, 1.49715));		

		pr.setSize(8.875, 6.937);
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}
	
	void testConvexPolySetTop() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setTop(7.0);		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 7.0) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 1.774));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).y, 5.848));
		ass_true(approx(pr.pointAt(1).y, 6.355));
		ass_true(approx(pr.pointAt(2).y, 7));
		ass_true(approx(pr.pointAt(3).y, 6.2167));
		ass_true(approx(pr.pointAt(4).y, 5.31806));
		ass_true(approx(pr.pointAt(5).y, 5.226));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_eq(pr.pointAt(i).x, original.pointAt(i).x);			
		}
		pr.setTop(12.163);
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolySetTopLeft() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setTopLeft(Point2f(4, 7));		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 7) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), 4) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 3.073) && approx(pr.height(), 1.774));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).y, 5.84794));
		ass_true(approx(pr.pointAt(0).x, 4));
		ass_true(approx(pr.pointAt(1).y, 6.35505));
		ass_true(approx(pr.pointAt(1).x, 4.14023));
		ass_true(approx(pr.pointAt(2).y, 7));
		ass_true(approx(pr.pointAt(2).x, 5.84068));
		ass_true(approx(pr.pointAt(3).y, 6.2167));
		ass_true(approx(pr.pointAt(3).x, 7.073));
		ass_true(approx(pr.pointAt(4).y, 5.31806));
		ass_true(approx(pr.pointAt(4).x, 6.55812));
		ass_true(approx(pr.pointAt(5).y, 5.226));
		ass_true(approx(pr.pointAt(5).x, 5.01383));		

		pr.setTopLeft(Point2f(-1.802, 12.163));
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolySetTopRight() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setTopRight(Point2f(10, 7));		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 7) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 10));
		ass_true(approx(pr.width(), 11.802) && approx(pr.height(), 1.774));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).y, 5.84794));
		ass_true(approx(pr.pointAt(0).x, -1.802));
		ass_true(approx(pr.pointAt(1).y, 6.35505));
		ass_true(approx(pr.pointAt(1).x, -1.26343));
		ass_true(approx(pr.pointAt(2).y, 7));
		ass_true(approx(pr.pointAt(2).x, 5.26723));
		ass_true(approx(pr.pointAt(3).y, 6.2167));
		ass_true(approx(pr.pointAt(3).x, 10));
		ass_true(approx(pr.pointAt(4).y, 5.31806));
		ass_true(approx(pr.pointAt(4).x, 8.02258));
		ass_true(approx(pr.pointAt(5).y, 5.226));
		ass_true(approx(pr.pointAt(5).x, 2.09166));		

		pr.setTopRight(Point2f(7.073, 12.163));
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}


	void testConvexPolySetWidth() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;		
		DMSG("pr = " << pr << ".");
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 7.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		
		pr.setWidth(3.2);		
		DMSG("pr = " << pr << ".");
		DMSG("rect: " << pr.rect());		
		ass_true(approx(pr.top(), 12.163) && approx(pr.bottom(), 5.226) &&
					approx(pr.left(), -1.802) && approx(pr.right(), 1.398));
		ass_true(approx(pr.width(), 3.2) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));
		ass_true(approx(pr.pointAt(0).x, -1.802));
		ass_true(approx(pr.pointAt(1).x, -1.656));
		ass_true(approx(pr.pointAt(2).x, 0.114755));
		ass_true(approx(pr.pointAt(3).x, 1.398));
		ass_true(approx(pr.pointAt(4).x, 0.861842));
		ass_true(approx(pr.pointAt(5).x, -0.74627));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_eq(pr.pointAt(i).y, original.pointAt(i).y);			
		}
		pr.setWidth(8.875);
		ass_true(pr.approx(original) && original.approx(pr));

		FINISH_TEST;
	}

	void testConvexPolyTranslate() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original;
		pr.translate(Vec2f(10, -4.5));
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 7.663) && approx(pr.bottom(), 0.726) &&
					approx(pr.left(), 8.198) && approx(pr.right(), 17.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y - 4.5));
			ass_true(approx(pr.pointAt(i).x, original.pointAt(i).x + 10));				
		}
		pr.translate(-10, 4.5);
		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}	


	void testConvexPolyTranslated() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> original(points);
		DMSG("original = " << original << ".");
		ass_false(original.isEmpty());
		ass_true(original.isValid());
		ass_false(original.rect().isEmpty());	
		ass_true(approx(original.size().width, 8.875) && approx(original.size().height, 6.937));		
		ass_eq(original.count(), 6);	
		ass_true(approx(original.top(), 12.163) && approx(original.bottom(), 5.226) &&
					approx(original.left(), -1.802) && approx(original.right(), 7.073));
		ass_true(approx(original.width(), 8.875) && approx(original.height(), 6.937));

		ConvexPoly2<Real> pr = original.translated(10, -4.5);
		DMSG("pr = " << pr << ".");		
		ass_true(approx(pr.top(), 7.663) && approx(pr.bottom(), 0.726) &&
					approx(pr.left(), 8.198) && approx(pr.right(), 17.073));
		ass_true(approx(pr.width(), 8.875) && approx(pr.height(), 6.937));
		ass_neq(pr, original);
	   ass_false(pr.approx(original) || original.approx(pr));		
		for (Size i = 0; i < original.count(); ++i) {
			ass_true(approx(pr.pointAt(i).y, original.pointAt(i).y - 4.5));
			ass_true(approx(pr.pointAt(i).x, original.pointAt(i).x + 10));				
		}
		pr = pr.translated(-10, 4.5);
		ass_true(pr.approx(original) && original.approx(pr));
		

		FINISH_TEST;
	}	

	void testConvexPolyUnionWith() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> poly1(points);
		DMSG("poly1 = " << poly1 << ".");
		ass_false(poly1.isEmpty());
		ass_true(poly1.isValid());
		ass_false(poly1.rect().isEmpty());	
		ass_true(approx(poly1.size().width, 8.875) && approx(poly1.size().height, 6.937));		
		ass_eq(poly1.count(), 6);	
		ass_true(approx(poly1.top(), 12.163) && approx(poly1.bottom(), 5.226) &&
					approx(poly1.left(), -1.802) && approx(poly1.right(), 7.073));
		ass_true(approx(poly1.width(), 8.875) && approx(poly1.height(), 6.937));

		ConvexPoly2<Real> poly2 = poly1;
		poly2.setBounds(Rectf(4.009, 11.231, 6.0, 4.0));
		DMSG("poly2 = " << poly2 << ".");

		poly1.unionWith(poly2);
		DMSG("poly1 = " << poly1 << ".");
		DMSG("rect: " << poly1.rect());		
		ass_false(poly1.isEmpty());
		ass_true(poly1.isValid());
		ass_false(poly1.rect().isEmpty());	
		ass_true(approx(poly1.size().width, 11.811) && approx(poly1.size().height, 6.937));		
		ass_eq(poly1.count(), 8);	
		ass_true(approx(poly1.top(), 12.163) && approx(poly1.bottom(), 5.226) &&
					approx(poly1.left(), -1.802) && approx(poly1.right(), 10.009));
		ass_true(approx(poly1.width(), 11.811) && approx(poly1.height(), 6.937));

		ass_true(approx(poly1.pointAt(0).x, -1.802));
		ass_true(approx(poly1.pointAt(0).y, 7.658));
		ass_true(approx(poly1.pointAt(1).x, -1.397));
		ass_true(approx(poly1.pointAt(1).y, 9.641));
		ass_true(approx(poly1.pointAt(2).x, 3.514));
		ass_true(approx(poly1.pointAt(2).y, 12.163));
		ass_true(approx(poly1.pointAt(3).x, 7.603));
		ass_true(approx(poly1.pointAt(3).y, 11.231));
		ass_true(approx(poly1.pointAt(4).x, 10.009));
		ass_true(approx(poly1.pointAt(4).y, 9.46482));
		ass_true(approx(poly1.pointAt(5).x, 9.0037));
		ass_true(approx(poly1.pointAt(5).y, 7.43858));
		ass_true(approx(poly1.pointAt(6).x, 5.586));
		ass_true(approx(poly1.pointAt(6).y, 5.586));
		ass_true(approx(poly1.pointAt(7).x, 1.126));
		ass_true(approx(poly1.pointAt(7).y, 5.226));
		

		FINISH_TEST;
	}

	void testConvexPolyUnionedWith() {
		BEGIN_TEST;

		Vector<Point2f> points(8);
		points.append(Point2f(-1.31, 7.52)); /* Inside */
		points.append(Point2f(7.073, 9.1));
		points.append(Point2f(3.514, 12.163));
		points.append(Point2f(-1.397, 9.641));
		points.append(Point2f(2.52, 5.45)); /* Inside */
		points.append(Point2f(-1.802, 7.658));
		points.append(Point2f(1.126, 5.226));
		points.append(Point2f(5.586, 5.586));

		ConvexPoly2<Real> poly1(points);
		DMSG("poly1 = " << poly1 << ".");
		ass_false(poly1.isEmpty());
		ass_true(poly1.isValid());
		ass_false(poly1.rect().isEmpty());	
		ass_true(approx(poly1.size().width, 8.875) && approx(poly1.size().height, 6.937));		
		ass_eq(poly1.count(), 6);	
		ass_true(approx(poly1.top(), 12.163) && approx(poly1.bottom(), 5.226) &&
					approx(poly1.left(), -1.802) && approx(poly1.right(), 7.073));
		ass_true(approx(poly1.width(), 8.875) && approx(poly1.height(), 6.937));

		ConvexPoly2<Real> poly2 = poly1;
		poly2.setBounds(Rectf(4.009, 11.231, 6.0, 4.0));
		DMSG("poly2 = " << poly2 << ".");

		ConvexPoly2<Real> poly3 = poly1.unionedWith(poly2);		
		DMSG("poly3 = " << poly3 << ".");
		DMSG("rect: " << poly3.rect());		
		ass_false(poly3.isEmpty());
		ass_true(poly3.isValid());
		ass_false(poly3.rect().isEmpty());	
		ass_true(approx(poly3.size().width, 11.811) && approx(poly3.size().height, 6.937));		
		ass_eq(poly3.count(), 8);	
		ass_true(approx(poly3.top(), 12.163) && approx(poly3.bottom(), 5.226) &&
					approx(poly3.left(), -1.802) && approx(poly3.right(), 10.009));
		ass_true(approx(poly3.width(), 11.811) && approx(poly3.height(), 6.937));

		ass_true(approx(poly3.pointAt(0).x, -1.802));
		ass_true(approx(poly3.pointAt(0).y, 7.658));
		ass_true(approx(poly3.pointAt(1).x, -1.397));
		ass_true(approx(poly3.pointAt(1).y, 9.641));
		ass_true(approx(poly3.pointAt(2).x, 3.514));
		ass_true(approx(poly3.pointAt(2).y, 12.163));
		ass_true(approx(poly3.pointAt(3).x, 7.603));
		ass_true(approx(poly3.pointAt(3).y, 11.231));
		ass_true(approx(poly3.pointAt(4).x, 10.009));
		ass_true(approx(poly3.pointAt(4).y, 9.46482));
		ass_true(approx(poly3.pointAt(5).x, 9.0037));
		ass_true(approx(poly3.pointAt(5).y, 7.43858));
		ass_true(approx(poly3.pointAt(6).x, 5.586));
		ass_true(approx(poly3.pointAt(6).y, 5.586));
		ass_true(approx(poly3.pointAt(7).x, 1.126));
		ass_true(approx(poly3.pointAt(7).y, 5.226));

		ConvexPoly2<Real> poly4 = poly2.unionedWith(poly1);
		ass_eq(poly3, poly4);		
		

		FINISH_TEST;
	}	
	
} // namespace cc

int main(int argc, char** argv) {
	cc::testConvexPolyCreateAndDestroyEmpty();
	cc::testConvexPolyCreateAndDestroyFromRect();
	cc::testConvexPolyCreateAndDestroyFromVector();
	cc::testConvexPolyEquality();
	cc::testConvexPolyContainsPoint();
	cc::testConvexPolyContainsPolygon();
	cc::testConvexPolyMoveBottomTo();
	cc::testConvexPolyMoveBottomLeftTo();
	cc::testConvexPolyMoveBottomRightTo();
	cc::testConvexPolyMoveCenterTo();
	cc::testConvexPolyMoveLeftTo();
	cc::testConvexPolyMoveRightTo();
	cc::testConvexPolyMoveTo();	
	cc::testConvexPolyMoveTopTo();
	cc::testConvexPolyMoveTopRightTo();
	cc::testConvexPolySetBottom();
	cc::testConvexPolySetBottomLeft();
	cc::testConvexPolySetBottomRight();
	cc::testConvexPolySetHeight();	
	cc::testConvexPolySetLeft();
	cc::testConvexPolySetBounds();	
	cc::testConvexPolySetRight();
	cc::testConvexPolySetSize();	
	cc::testConvexPolySetTop();
	cc::testConvexPolySetTopLeft();
	cc::testConvexPolySetTopRight();
	cc::testConvexPolySetWidth();
	cc::testConvexPolyTranslate();
	cc::testConvexPolyTranslated();
	cc::testConvexPolyUnionWith();
	cc::testConvexPolyUnionedWith();	
	return 0;
}

