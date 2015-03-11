#include <assert.h>
#include <ctime>
#include <cstdio>
#include <iostream>
#include "core/math/angle.h"
#ifndef DEBUG
#define DEBUG 1
#endif

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl)


namespace cc {

	void testBasicAngleCreationAndAssignment() {
		BEGIN_TEST;
		
		Angle a1;
		assert( a1.radians() == REAL(0.0) );
		assert( a1.degrees() == REAL(0.0) );

		Angle a2(REAL(0.7853981633973));
		assert( a2.radians() == REAL(0.7853981633973) );
		assert( approx(a2.degrees(), 45.0) );
		
		Angle a3 = Angle::fromRadians(REAL(0.7853981633973));
		assert( a3.radians() == REAL(0.7853981633973) );
		assert( approx(a3.degrees(), 45.0) );

		Angle a4 = Angle::fromDegrees(45.0);
		assert( a4.radians() == REAL(0.7853981633973) );
		assert( approx(a4.degrees(), 45.0) );

		a3 = Angle::fromRadians(0.0);
		assert( a3.radians() == REAL(0.0) );
		assert( a3.degrees() == REAL(0.0) );
		
		a4 = Angle::fromDegrees(0.0);
		assert( a4.radians() == REAL(0.0) );
		assert( a4.degrees() == REAL(0.0) );

		a3 = a2;
		assert( a3.radians() == REAL(0.7853981633973) );
		assert( approx(a3.degrees(), 45.0) );

		a1 = Angle(REAL(-0.317649924));
		assert( a1.radians() == REAL(-0.317649924) );
		assert( a1.degrees() == REAL(-18.2) );
		
		a2 = Angle::fromDegrees(-18.2);
		assert( a2.radians() == REAL(-0.317649924) );
		assert( a2.degrees() == REAL(-18.2) );

		a3 = Angle(a2);
		assert( a3.radians() == REAL(-0.317649924) );
		assert( a3.degrees() == REAL(-18.2) );

		// Test setting values
		a3.setRadians(0.7853981633973);
		assert( a3.radians() == REAL(0.7853981633973) );
		assert( approx(a3.degrees(), 45.0) );

		a3.setDegrees(-18.2);
		assert( a3.radians() == REAL(-0.317649924) );
		assert( a3.degrees() == REAL(-18.2) );

		a2.setRadians(0.0);
		assert( a2.radians() == REAL(0.0) );
		assert( a2.degrees() == REAL(0.0) );
		
		a3.setDegrees(0.0);
		assert( a3.radians() == REAL(0.0) );
		assert( a3.degrees() == REAL(0.0) );

		a2.setDegrees(0.0);
		assert( a2.radians() == REAL(0.0) );
		assert( a2.degrees() == REAL(0.0) );
		
		a3.setRadians(0.0);
		assert( a3.radians() == REAL(0.0) );
		assert( a3.degrees() == REAL(0.0) );
		
		FINISH_TEST;		
	}

	void testAngleEquality() {
		BEGIN_TEST;
		
		Angle a1(-0.317649924);
		assert( a1 == Angle::fromDegrees(-18.2) );
		assert( a1 == Angle::fromRadians(-0.317649924) );
		assert( a1 <= Angle::fromDegrees(-18.2) );
		assert( a1 >= Angle::fromRadians(-0.317649924) );
		assert( a1 != Angle::fromDegrees(-18.1) );
		assert( a1 != Angle::fromRadians(-0.3176490) );
		assert( a1 < Angle::fromDegrees(-18.1) );
		assert( a1 <= Angle::fromDegrees(-18.1) );
		assert( a1 > Angle::fromDegrees(-18.3) );
		assert( a1 >= Angle::fromDegrees(-18.3) );
		assert( a1 < Angle::fromRadians(0.0) );
		assert( !(a1 > Angle::fromDegrees(-18.2)) );
		assert( !(a1 < Angle::fromDegrees(-18.2)) );
		assert( !(a1 != Angle::fromDegrees(-18.2)) );

		Angle a2 = Angle::fromDegrees(45.0);
		assert( a2 == Angle::fromDegrees(45.0) );
		assert( a2 == Angle::fromRadians(0.7853981633973) );
		assert( a2 <= Angle::fromDegrees(45.0) );
		assert( a2 >= Angle::fromRadians(0.7853981633973) );
		assert( a2 != Angle::fromDegrees(45.1) );
		assert( a2 != Angle::fromRadians(0.78539810) );
		assert( a2 < Angle::fromDegrees(45.01) );
		assert( a2 <= Angle::fromDegrees(45.01) );
		assert( a2 > Angle::fromDegrees(44.99) );
		assert( a2 >= Angle::fromDegrees(44.99) );
		assert( a2 > Angle::fromRadians(0.0) );
	 	assert( !(a2 > Angle::fromRadians(0.7853981633973)) );
		assert( !(a2 < Angle::fromRadians(0.7853981633973)) );
		assert( !(a2 != Angle::fromRadians(0.7853981633973)) );

		Angle a3 = Angle::fromDegrees(0);
		assert( a3 == Angle::fromDegrees(0) );
		assert( a3 == Angle::fromRadians(0) );
		assert( a3 <= Angle::fromDegrees(0) );
		assert( a3 >= Angle::fromRadians(0) );
		assert( a3 != Angle::fromDegrees(-0.001) );
		assert( a3 != Angle::fromRadians(0.001) );
		assert( a3 < Angle::fromDegrees(0.001) );
		assert( a3 <= Angle::fromDegrees(0.001) );
		assert( a3 > Angle::fromDegrees(-0.001) );
		assert( a3 >= Angle::fromDegrees(-0.001) );
		assert( !(a3 > Angle::fromRadians(0.0)) );
		assert( !(a3 < Angle::fromRadians(0.0)) );
		assert( !(a3 != Angle::fromRadians(0.0)) );

		FINISH_TEST;
	}
	
	void testAngleAddition() {
		BEGIN_TEST;
		
		Angle a1(0.392699082);
		Angle a2 = Angle::fromDegrees(22.5);
		
		assert( (a1 + a2) == Angle(0.7853981633973) );
		assert( (a2 + a1) == Angle::fromDegrees(45.0) );
	  	assert( (a1 + Angle::fromDegrees(22.5)) == Angle(0.7853981633973) );
		assert( (Angle::fromRadians(0.392699082) + a2) == Angle::fromDegrees(45.0) );

		a2 = Angle::fromRadians(-0.317649924);
		
		assert( (a1+a2).approx(Angle(0.075049158)) );
		assert( (a2+a1).approx(Angle(0.075049158)) );
		assert( (a1 + Angle::fromDegrees(-18.2)).approx(Angle(0.075049158)) );
		assert( (a1 + Angle::fromRadians(-0.317649924)).approx(Angle(0.075049158)) );
		assert( (Angle::fromDegrees(-18.2)+ a1).approx(Angle(0.075049158)) );
		assert( (Angle::fromRadians(-0.317649924) + a1).approx(Angle(0.075049158)) );
		
		assert( (a2 + Angle::fromDegrees(22.5)).approx(Angle(0.075049158)) );
		assert( (a2 + Angle::fromRadians(0.392699082)).approx(Angle(0.075049158)) );
		assert( (Angle::fromDegrees(22.5)+ a2).approx(Angle(0.075049158)) );
		assert( (Angle::fromRadians(0.392699082) + a2).approx(Angle(0.075049158)) );

		assert( (a2 + Angle::fromDegrees(0.0)) == a2 );
		assert( (a2 + Angle::fromRadians(0.0)) == a2 );
		assert( (Angle::fromDegrees(0.0)+ a2) == a2 );
		assert( (Angle::fromRadians(0.0) + a2) == a2 );
		
		assert( (a1 + Angle::fromDegrees(0.0)) == a1 );
		assert( (a1 + Angle::fromRadians(0.0)) == a1 );
		assert( (Angle::fromDegrees(0.0)+ a1) == a1 );
		assert( (Angle::fromRadians(0.0) + a1) == a1 );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 += a2;
	   assert( a1.degrees() == REAL(45.0) && a2.degrees() == REAL(22.5) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a2 += a1;
	   assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(45.0) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 += Angle(0.392699082);		
	   assert( a1.degrees() == REAL(45.0) && a2.degrees() == REAL(22.5) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 += Angle::fromRadians(0.392699082);		
	   assert( a1.degrees() == REAL(45.0) && a2.degrees() == REAL(22.5) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 += Angle::fromDegrees(22.5);		
	   assert( a1.degrees() == REAL(45.0) && a2.degrees() == REAL(22.5) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 += -Angle::fromDegrees(22.5);		
	   assert( a1.degrees() == REAL(0.0) && a2.degrees() == REAL(22.5) );

		FINISH_TEST;
	}

	void testAngleSubtraction() {
		BEGIN_TEST;
		
		Angle a1(0.392699082);
		Angle a2 = Angle::fromDegrees(22.5);
		
		assert( (a1 - a2) == Angle(0.0) );
		assert( (a2 - a1) == Angle::fromDegrees(0.0) );
	  	assert( (a1 - Angle::fromDegrees(22.5)) == Angle(0.0) );
		assert( (Angle::fromRadians(0.392699082) - a2) == Angle::fromDegrees(0.0) );

		a2 = Angle::fromRadians(-0.317649924);
		
		assert( (a1-a2).approx(Angle(0.710349006)) );
		assert( (a2-a1).approx(Angle(-0.710349006)) );
		assert( (a1 - Angle::fromDegrees(-18.2)).approx(Angle(0.710349006)) );
		assert( (a1 - Angle::fromRadians(-0.317649924)).approx(Angle(0.710349006)) );
		assert( (Angle::fromDegrees(-18.2) - a1).approx(Angle(-0.710349006)) );
		assert( (Angle::fromRadians(-0.317649924) - a1).approx(Angle(-0.710349006)) );
		
		assert( (a2 - Angle::fromDegrees(22.5)).approx(Angle(-0.710349006)) );
		assert( (a2 - Angle::fromRadians(0.392699082)).approx(Angle(-0.710349006)) );
		assert( (Angle::fromDegrees(22.5) - a2).approx(Angle(0.710349006)) );
		assert( (Angle::fromRadians(0.392699082) - a2).approx(Angle(0.710349006)) );

		assert( (a2 - Angle::fromDegrees(0.0)) == a2 );
		assert( (a2 - Angle::fromRadians(0.0)) == a2 );
		assert( (Angle::fromDegrees(0.0) - a2) == -a2 );
		assert( (Angle::fromRadians(0.0) - a2) == -a2 );
		
		assert( (a1 - Angle::fromDegrees(0.0)) == a1 );
		assert( (a1 - Angle::fromRadians(0.0)) == a1 );
		assert( (Angle::fromDegrees(0.0) - a1) == -a1 );
		assert( (Angle::fromRadians(0.0) - a1) == -a1 );
		
		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 -= a2;
	   assert( a1.degrees() == REAL(0.0) && a2.degrees() == REAL(22.5) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a2 -= a1;
	   assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(0.0) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 -= Angle(0.392699082);		
	   assert( a1.degrees() == REAL(0.0) && a2.degrees() == REAL(22.5) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 -= Angle::fromRadians(0.392699082);		
	   assert( a1.degrees() == REAL(0.0) && a2.degrees() == REAL(22.5) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 -= Angle::fromDegrees(22.5);		
	   assert( a1.degrees() == REAL(0.0) && a2.degrees() == REAL(22.5) );

		a1.setRadians(0.392699082);
		a2.setDegrees(22.5);
		assert( a1.degrees() == REAL(22.5) && a2.degrees() == REAL(22.5) );
		a1 -= -Angle::fromDegrees(22.5);		
	   assert( a1.degrees() == REAL(45.0) && a2.degrees() == REAL(22.5) );

		FINISH_TEST;
	}

	void testAngleScalarMultiplication() {
		BEGIN_TEST;
	
		Angle a(0.392699082);		
		assert(a.degrees() == REAL(22.5));
		
		assert( approx( (a*2).degrees(), 22.5*2.0) );
		assert( approx( (a*0.5).degrees(), 22.5/2.0) );
		assert( (a*1.0) == a );
		assert( (a*0.0).degrees() == REAL(0.0) );
		assert( (a*0.0).radians() == REAL(0.0) );
		assert( (a*-1.0) == -a);
		assert( ((a*-1.0)*-1.0) == a);		
		assert( approx( (a*-1.0).degrees(), -22.5) );
		
		a *= 2;
		assert( approx(a.degrees(), 22.5*2) );
		a = Angle(0.392699082);
		a *= 0.5;
		assert( approx(a.degrees(), 22.5 / 2) );
		a = Angle::fromDegrees(22.5);
		a *= 1.0;
		assert( a.radians() == REAL(0.392699082) );
		a *= -1.0;
		assert( a == Angle::fromDegrees(-22.5) );		
		a *= -1.0;
		assert( a == Angle::fromRadians(0.392699082) );
		a *= 0.0;
		assert( a.radians() == REAL(0.0) && a.degrees() == REAL(0.0) );
		assert( (a*2.0) == Angle(0.0) );
		
		FINISH_TEST;
	}

	void testAngleScalarDivision() {
		BEGIN_TEST;
	
		Angle a(0.392699082);		
		assert(a.degrees() == REAL(22.5));
		
		assert( approx( (a/2).degrees(), 22.5/2.0) );
		assert( approx( (a/0.5).degrees(), 22.5*2.0) );
		assert( (a/1.0) == a );
		assert( (a/-1.0) == -a);
		assert( ((a/-1.0)/-1.0) == a);		
		assert( approx( (a/-1.0).degrees(), -22.5) );
		
		a /= 2;
		assert( approx(a.degrees(), 22.5/2) );
		a = Angle(0.392699082);
		a /= 0.5;
		assert( approx(a.degrees(), 22.5 * 2) );
		a = Angle::fromDegrees(22.5);
		a /= 1.0;
		assert( a.radians() == REAL(0.392699082) );
		a /= -1.0;
		assert( a == Angle::fromDegrees(-22.5) );		
		a /= -1.0;
		assert( a == Angle::fromRadians(0.392699082) );

		a = Angle(0.0);
		assert( a.radians() == REAL(0.0) && a.degrees() == REAL(0.0) );
		assert( (a / 3.0) == a );
		assert( (a / -3.0) == a );
		assert( (a / 1.0) == a);
		assert( (a / 0.5) == a);
		
		a /= 3.0;
		assert( a.radians() == REAL(0.0) && a.degrees() == REAL(0.0) );
		a /= -3.0;
		assert( a.radians() == REAL(0.0) && a.degrees() == REAL(0.0) );
		a /= 1.0;
		assert( a.radians() == REAL(0.0) && a.degrees() == REAL(0.0) );
		a /= 0.5;
		assert( a.radians() == REAL(0.0) && a.degrees() == REAL(0.0) );
		
		FINISH_TEST;
	}

}

int main(int argc, char** argv) {
	cc::testBasicAngleCreationAndAssignment();
	cc::testAngleEquality();
	cc::testAngleAddition();
	cc::testAngleSubtraction();
	cc::testAngleScalarMultiplication();
	cc::testAngleScalarDivision();
	
	return 0;
}

