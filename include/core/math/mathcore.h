#ifndef CAT_CORE_MATH_MATHCORE_H
#define CAT_CORE_MATH_MATHCORE_H
/**
 * @copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file mathcore.h: 
 * @brief Contains functions and constants for math operations.
 *
 * @author Catlin Zilinski
 * @date Dec 2, 2013
 */

#include <cstdlib>
#include <cmath>
//#include <x86intrin.h>
#include "core/math/vec2.h"
#include "core/math/vec3.h"
#include "core/math/vec4.h"

#define CAT_RADTODEG 57.2957795130823
#define CAT_DEGTORAD 0.01745329251994

#define CAT_SQRT2 1.414213562373095

#define CAT_REAL_EPSILON REAL(0.00001)

namespace Cat {
	namespace Math {

		
		// ###################################
		// Functions for dealing with angle related math.  The functions
		// can operate on both scalar and vector quantities.
		// ###################################

		/**
		 * @brief Get the arccosine of an angle in radians.
		 * @param p_angle The angle in radians.
		 * @return The arccosine of the radian angle.
		 */
		inline Real acos(Real p_angle) {
			return ::acos(p_angle);
		}

		/**
		 * @brief Get the arccosine of an angle vector in radians.
		 * @param p_angle The angle vector in radians.
		 * @return The arccosine vector of the radian angle vector.
		 */
		inline Vec2 acos(const Vec2& p_angle) {
			return Vec2(acos(p_angle.x), acos(p_angle.y));
		}
		inline Vec3 acos(const Vec3& p_angle) {
			return Vec3(acos(p_angle.x), acos(p_angle.y), acos(p_angle.z));
		}
		inline Vec4 acos(const Vec4& p_angle) {
			return Vec4(acos(p_angle.x), acos(p_angle.y), acos(p_angle.z), acos(p_angle.w));
		}

		/**
		 * @brief Get the arcsine of an angle in radians.
		 * @param p_angle The angle in radians.
		 * @return The arcsine of the radian angle.
		 */
		inline Real asin(Real p_angle) {
			return ::asin(p_angle);
		}

		/**
		 * @brief Get the arcsine of an angle vector in radians.
		 * @param p_angle The angle vector in radians.
		 * @return The arcsine vector of the radian angle vector.
		 */
		inline Vec2 asin(const Vec2& p_angle) {
			return Vec2(asin(p_angle.x), asin(p_angle.y));
		}
		inline Vec3 asin(const Vec3& p_angle) {
			return Vec3(asin(p_angle.x), asin(p_angle.y), asin(p_angle.z));
		}
		inline Vec4 asin(const Vec4& p_angle) {
			return Vec4(asin(p_angle.x), asin(p_angle.y), asin(p_angle.z), asin(p_angle.w));
		}

			/**
		 * @brief Get the arctangent of an angle in radians.
		 * @param p_angle The angle in radians.
		 * @return The arctangent of the radian angle.
		 */
		inline Real atan(Real p_angle) {
			return ::atan(p_angle);
		}

		/**
		 * @brief Get the arctangent of an angle vector in radians.
		 * @param p_angle The angle vector in radians.
		 * @return The arctangent vector of the radian angle vector.
		 */
		inline Vec2 atan(const Vec2& p_angle) {
			return Vec2(atan(p_angle.x), atan(p_angle.y));
		}
		inline Vec3 atan(const Vec3& p_angle) {
			return Vec3(atan(p_angle.x), atan(p_angle.y), atan(p_angle.z));
		}
		inline Vec4 atan(const Vec4& p_angle) {
			return Vec4(atan(p_angle.x), atan(p_angle.y), atan(p_angle.z), atan(p_angle.w));
		}
		
		/**
		 * @brief Get the cosine of an angle in radians.
		 * @param p_angle The angle in radians.
		 * @return The cosine of the radian angle.
		 */
		inline Real cos(Real p_angle) {
			return ::cos(p_angle);
		}

		/**
		 * @brief Get the cosine of an angle vector in radians.
		 * @param p_angle The angle vector in radians.
		 * @return The cosine vector of the radian angle vector.
		 */
		inline Vec2 cos(const Vec2& p_angle) {
			return Vec2(cos(p_angle.x), cos(p_angle.y));
		}
		inline Vec3 cos(const Vec3& p_angle) {
			return Vec3(cos(p_angle.x), cos(p_angle.y), cos(p_angle.z));
		}
		inline Vec4 cos(const Vec4& p_angle) {
			return Vec4(cos(p_angle.x), cos(p_angle.y), cos(p_angle.z), cos(p_angle.w));
		}

		/**
		 * @brief Convert a radian angle to degrees.
		 * @param p_radians The radian angle to convert to degrees.
		 * @return The angle in degrees.
		 */
		inline Real degrees(Real p_radians) {
			return p_radians * CAT_RADTODEG;
		}

		/**
		 * @brief Convert a vector of radian angles to degrees.
		 * @param p_radians The radian angle vector to convert to degrees.
		 * @return The angle vector in degrees.
		 */
		inline Vec2 degrees(const Vec2& p_radians) {
			return p_radians * CAT_RADTODEG;
		}
		inline Vec3 degrees(const Vec3& p_radians) {
			return p_radians * CAT_RADTODEG;
		}
		inline Vec4 degrees(const Vec4& p_radians) {
			return p_radians * CAT_RADTODEG;
		}

		/**
		 * @brief Convert a degree angle to radians.
		 * @param p_degrees The degree angle to convert to radians.
		 * @return The angle in radians.
		 */
		inline Real radians(Real p_degrees) {
			return p_degrees * CAT_DEGTORAD;
		}

		/**
		 * @brief Convert a vector of degree angles to radians.
		 * @param p_degrees The degree angle vector to convert to radians.
		 * @return The angle vector in radians.
		 */
		inline Vec2 radians(const Vec2& p_degrees) {
			return p_degrees * CAT_DEGTORAD;
		}
		inline Vec3 radians(const Vec3& p_degrees) {
			return p_degrees * CAT_DEGTORAD;
		}
		inline Vec4 radians(const Vec4& p_degrees) {
			return p_degrees * CAT_DEGTORAD;
		}
		
		/**
		 * @brief Get the sine of an angle in radians.
		 * @param p_angle The angle in radians.
		 * @return The sine of the radian angle.
		 */
		inline Real sin(Real p_angle) {
			return ::sin(p_angle);
		}

		/**
		 * @brief Get the sine of an angle vector in radians.
		 * @param p_angle The angle vector in radians.
		 * @return The sine vector of the radian angle vector.
		 */
		inline Vec2 sin(const Vec2& p_angle) {
			return Vec2(sin(p_angle.x), sin(p_angle.y));
		}
		inline Vec3 sin(const Vec3& p_angle) {
			return Vec3(sin(p_angle.x), sin(p_angle.y), sin(p_angle.z));
		}
		inline Vec4 sin(const Vec4& p_angle) {
			return Vec4(sin(p_angle.x), sin(p_angle.y), sin(p_angle.z), sin(p_angle.w));
		}

		/**
		 * @brief Get the tangent of an angle in radians.
		 * @param p_angle The angle in radians.
		 * @return The tangent of the radian angle.
		 */
		inline Real tan(Real p_angle) {
			return ::tan(p_angle);
		}

		/**
		 * @brief Get the tangent of an angle vector in radians.
		 * @param p_angle The angle vector in radians.
		 * @return The tangent vector of the radian angle vector.
		 */
		inline Vec2 tan(const Vec2& p_angle) {
			return Vec2(tan(p_angle.x), tan(p_angle.y));
		}
		inline Vec3 tan(const Vec3& p_angle) {
			return Vec3(tan(p_angle.x), tan(p_angle.y), tan(p_angle.z));
		}
		inline Vec4 tan(const Vec4& p_angle) {
			return Vec4(tan(p_angle.x), tan(p_angle.y), tan(p_angle.z), tan(p_angle.w));
		}

		// ###################################
		// Functions for dealing with exponential related math.  The functions
		// can operate on both scalar and vector quantities.
		// ###################################

		/**
		 * @brief Raise a e to a given power.
		 * @param p_x The power to raise e to.
		 * @return The value of e^p_x.
		 */
		inline Real exp(Real p_x) {
			return ::exp(p_x);
		}

		/**
		 * @brief Raise a e to a given power vector.
		 * @param p_x The power vector to raise e to.
		 * @return The value of e^p_x component-wise.
		 */
		inline Vec2 exp(const Vec2& p_x) {
			return Vec2(exp(p_x.x), exp(p_x.y));
		}
		inline Vec3 exp(const Vec3& p_x) {
			return Vec3(exp(p_x.x), exp(p_x.y), exp(p_x.z));
		}
		inline Vec4 exp(const Vec4& p_x) {
			return Vec4(exp(p_x.x), exp(p_x.y), exp(p_x.z), exp(p_x.w));
		}

		/**
		 * @brief Compute the inverse square root of a number.
		 * @param p_x The number to compute the inverse square root of.
		 * @return The inverse square root of p_x.
		 */
		inline Real inversesqrt(Real p_x) {
			return REAL(1.0) / ::sqrt(p_x);
		}

		/**
		 * @brief Compute the inverse square root of a vector, component-wise.
		 * @param p_x The vector to compute the inverse square root of.
		 * @return The inverse square root of the vector p_x, component-wise.
		 */
		inline Vec2 inversesqrt(const Vec2& p_x) {
			return Vec2(inversesqrt(p_x.x), inversesqrt(p_x.y));
		}
		inline Vec3 inversesqrt(const Vec3& p_x) {
			return Vec3(inversesqrt(p_x.x), inversesqrt(p_x.y), inversesqrt(p_x.z));
		}
		inline Vec4 inversesqrt(const Vec4& p_x) {
			return Vec4(inversesqrt(p_x.x), inversesqrt(p_x.y), inversesqrt(p_x.z), inversesqrt(p_x.w));
		}

		/**
		 * @brief Compute the natural logarithm of a number.
		 * @param p_x The number to compute the natural logarithm for.
		 * @return The natural logarithm of p_x.
		 */
		inline Real log(Real p_x) {
			return ::log(p_x);
		}

		/**
		 * @brief Compute the natural logarithm of a vector, component-wise.
		 * @param p_x The vector to compute the natural logarithm for.
		 * @return The natural logarithm of the vector p_x, component-wise.
		 */
		inline Vec2 log(const Vec2& p_x) {
			return Vec2(log(p_x.x), log(p_x.y));
		}
		inline Vec3 log(const Vec3& p_x) {
			return Vec3(log(p_x.x), log(p_x.y), log(p_x.z));
		}
		inline Vec4 log(const Vec4& p_x) {
			return Vec4(log(p_x.x), log(p_x.y), log(p_x.z), log(p_x.w));
		}

		/**
		 * @brief Compute the logarithm base 10 of a number.
		 * @param p_x The number to compute the logarithm base 10 for.
		 * @return The logarithm base 10 of p_x.
		 */
		inline Real log10(Real p_x) {
			return ::log10(p_x);
		}

		/**
		 * @brief Compute the logarithm base 10 of a vector, component-wise.
		 * @param p_x The vector to compute the logarithm base 10 for.
		 * @return The logarithm base 10 of the vector p_x, component-wise.
		 */
		inline Vec2 log10(const Vec2& p_x) {
			return Vec2(log10(p_x.x), log10(p_x.y));
		}
		inline Vec3 log10(const Vec3& p_x) {
			return Vec3(log10(p_x.x), log10(p_x.y), log10(p_x.z));
		}
		inline Vec4 log10(const Vec4& p_x) {
			return Vec4(log10(p_x.x), log10(p_x.y), log10(p_x.z), log10(p_x.w));
		}

		/**
		 * @brief Get the integer log base 2 of a number.
		 * @param x The number to get the log base 2 of.
		 * @return The integer value of the log base 2 of x.
		 */
		inline U32 log2i(Real x) {
		   U32 y = 0;
			while (x > 1) {
				x /= 2;
				y++;
			}	
			//_BitScanReverse(&y, x);
			return y;
		}
		
		/**
		 * @brief Raise a number to a given power.
		 * @param p_x The number to raise to the power.
		 * @param p_exp The power to raise x to.
		 * @return The value of p_x^p_xep.
		 */
		inline Real pow(Real p_x, Real p_exp) {
			return ::pow(p_x, p_exp);
		}

		/**
		 * @brief Raise each component of a vector to a given power.
		 * @param p_x The vector to raise to the power.
		 * @param p_exp The power to raise the components of p_x to.
		 * @return The value of p_x^p_xep, component-wise.
		 */
		inline Vec2 pow(const Vec2& p_x, Real p_exp) {
			return Vec2(pow(p_x.x, p_exp), pow(p_x.y, p_exp));
		}
		inline Vec3 pow(const Vec3& p_x, Real p_exp) {
			return Vec3(pow(p_x.x, p_exp), pow(p_x.y, p_exp), pow(p_x.z, p_exp));
		}
		inline Vec4 pow(const Vec4& p_x, Real p_exp) {
			return Vec4(pow(p_x.x, p_exp), pow(p_x.y, p_exp), pow(p_x.z, p_exp), pow(p_x.w, p_exp));
		}

		/**
		 * @brief Raise each component of a vector to a given power.
		 * @param p_x The vector to raise to the corresponding power.
		 * @param p_exp The powers to raise the components of p_x to.
		 * @return The value of p_x^p_xep, component-wise.
		 */
		inline Vec2 pow(const Vec2& p_x, const Vec2& p_exp) {
			return Vec2(pow(p_x.x, p_exp.x), pow(p_x.y, p_exp.y));
		}
		inline Vec3 pow(const Vec3& p_x, const Vec3& p_exp) {
			return Vec3(pow(p_x.x, p_exp.x), pow(p_x.y, p_exp.y), pow(p_x.z, p_exp.z));
		}
		inline Vec4 pow(const Vec4& p_x, const Vec4& p_exp) {
			return Vec4(pow(p_x.x, p_exp.x), pow(p_x.y, p_exp.y), pow(p_x.z, p_exp.z), pow(p_x.w, p_exp.w));
		}

		/**
		 * @brief Compute the square root of a number.
		 * @param p_x The number to compute the square root of.
		 * @return The square root of p_x.
		 */
		inline Real sqrt(Real p_x) {
			return ::sqrt(p_x);
		}

		/**
		 * @brief Compute the square root of a vector, component-wise.
		 * @param p_x The vector to compute the square root of.
		 * @return The square root of the vector p_x, component-wise.
		 */
		inline Vec2 sqrt(const Vec2& p_x) {
			return Vec2(sqrt(p_x.x), sqrt(p_x.y));
		}
		inline Vec3 sqrt(const Vec3& p_x) {
			return Vec3(sqrt(p_x.x), sqrt(p_x.y), sqrt(p_x.z));
		}
		inline Vec4 sqrt(const Vec4& p_x) {
			return Vec4(sqrt(p_x.x), sqrt(p_x.y), sqrt(p_x.z), sqrt(p_x.w));
		}

		/**
		 * @brief Compute the square of a number (x*x).
		 * @param p_x The number to compute the square of.
		 * @return The square of p_x.
		 */
		inline Real squared(Real p_x) {
			return p_x*p_x;
		}

		/**
		 * @brief Compute the square of a vector, component-wise.
		 * @param p_x The vector to compute the square of.
		 * @return The square of the vector p_x, component-wise.
		 */
		inline Vec2 squared(const Vec2& p_x) {
			return Vec2(squared(p_x.x), squared(p_x.y));
		}
		inline Vec3 squared(const Vec3& p_x) {
			return Vec3(squared(p_x.x), squared(p_x.y), squared(p_x.z));
		}
		inline Vec4 squared(const Vec4& p_x) {
			return Vec4(squared(p_x.x), squared(p_x.y), squared(p_x.z), squared(p_x.w));
		}

		// ###################################
		// Commonly used mathematical functions.
		// ###################################
		
		/**
		 * @brief Get the absolute value of a number.
		 * @param p_x The value to get the absolute value of.
		 * @return The absolute value of p_x.
		 */
		inline Real abs(Real p_x) {
			return ::fabs(p_x);
		}

		/**
		 * @brief Integer abs method.
		 * @param val The value to get the absolute value of.
		 */
		inline I32 abs(I32 val) {
			return ::abs(val);			
		}

		/**
		 * @brief Get the absolute value of a vector, component-wise.
		 * @param p_x The vector to get the absolute value of.
		 * @return The absolute value of the vector p_x, component-wise.
		 */
		inline Vec2 abs(const Vec2& p_x) {
			return Vec2(abs(p_x.x), abs(p_x.y));
		}
		inline Vec3 abs(const Vec3& p_x) {
			return Vec3(abs(p_x.x), abs(p_x.y), abs(p_x.z));
		}
		inline Vec4 abs(const Vec4& p_x) {
			return Vec4(abs(p_x.x), abs(p_x.y), abs(p_x.z), abs(p_x.w));
		}


		/**
		 * @brief See if two numbers are approximately equal (within some Epsilon).
		 * @param p_x The first number to compare.
		 * @param p_y The second number to compare..
		 * @return True if the numbers are approximately equal.
		 */
		inline Boolean approx(Real p_x, Real p_y) {
			return (abs(p_x - p_y) < CAT_REAL_EPSILON);			
		}

		/**
		 * @brief Approximate if two Integer values are equal.
		 * Here for templating reasons.
		 * @param val1 The first value.
		 * @param val2 The second value.
		 * @return True if the values are equal.
		 */
		inline Boolean approx(I32 val1, I32 val2) {
			return val1 == val2;			
		}

		/**
		 * @brief See if two vectors are approximately equal (within some Epsilon).
		 * @param p_x The first vector to compare.
		 * @param p_y The second vector to compare..
		 * @return True if the vectors are approximately equal.
		 */
		inline Boolean approx(const Vec2& p_x, const Vec2& p_y) {
			return approx(p_x.x, p_y.x) && approx(p_x.y, p_y.y);
		}
		inline Boolean approx(const Vec3& p_x, const Vec3& p_y) {
			return approx(p_x.x, p_y.x) && approx(p_x.y, p_y.y) && approx(p_x.z, p_y.z);
		}
		inline Boolean approx(const Vec4& p_x, const Vec4& p_y) {
			return approx(p_x.x, p_y.x) && approx(p_x.y, p_y.y) && approx(p_x.z, p_y.z) && approx(p_x.w, p_y.w);
		}

		/**
		 * @brief Get the smallest number >= p_x (ceiling of p_x).
		 * @param p_x The number to get the ceiling of.
		 * @return The ceiling of p_x.
		 */
		inline Real ceil(Real p_x) {
			return ::ceil(p_x);
		}

		/**
		 * @brief Get the smallest number >= p_x (ceiling of p_x), for each component.
		 * @param p_x The vector to get the ceiling of.
		 * @return The ceiling of the vector p_x, component-wise.
		 */
		inline Vec2 ceil(const Vec2& p_x) {
			return Vec2(ceil(p_x.x), ceil(p_x.y));
		}
		inline Vec3 ceil(const Vec3& p_x) {
			return Vec3(ceil(p_x.x), ceil(p_x.y), ceil(p_x.z));
		}
		inline Vec4 ceil(const Vec4& p_x) {
			return Vec4(ceil(p_x.x), ceil(p_x.y), ceil(p_x.z), ceil(p_x.w));
		}

		/**
		 * @brief Returns x if p_min < x < p_max, else one of p_min or p_max.
		 * @param p_x The value to clamp.
		 * @param p_min The start of the range to clamp to.
		 * @param p_max The end of the range to clamp to.
		 * @return The value of p_x clamped to [p_min, p_max].
		 */
		inline Real clamped(Real p_x, Real p_min, Real p_max) {
			if (p_x < p_min) { return p_min; }
			else if (p_x > p_max) { return p_max; }
			else { return p_x; }
		}

		/**
		 * @brief Returns x if p_min < x < p_max, else one of p_min or p_max, for each component.
		 * @param p_x The vector to clamp.
		 * @param p_min The start of the range to clamp each component to.
		 * @param p_max The end of the range to clamp each component to.
		 * @return The value of p_x clamped to [p_min, p_max], component-wise.
		 */
		inline Vec2 clamped(const Vec2& p_x, Real p_min, Real p_max) {
			return Vec2(clamped(p_x.x, p_min, p_max), clamped(p_x.y, p_min, p_max));
		}
		inline Vec3 clamped(const Vec3& p_x, Real p_min, Real p_max) {
			return Vec3(clamped(p_x.x, p_min, p_max), clamped(p_x.y, p_min, p_max), clamped(p_x.z, p_min, p_max));
		}
		inline Vec4 clamped(const Vec4& p_x, Real p_min, Real& p_max) {
			return Vec4(clamped(p_x.x, p_min, p_max), clamped(p_x.y, p_min, p_max), clamped(p_x.z, p_min, p_max), clamped(p_x.w, p_min, p_max));
		}

		/**
		 * @brief Returns x if p_min < x < p_max, else one of p_min or p_max, for each component.
		 * @param p_x The vector to clamp.
		 * @param p_min The start vector of the range to clamp to.
		 * @param p_max The end vector of the range to clamp to.
		 * @return The value of p_x clamped to [p_min, p_max], component-wise.
		 */
		inline Vec2 clamped(const Vec2& p_x, const Vec2& p_min, const Vec2& p_max) {
			return Vec2(clamped(p_x.x, p_min.x, p_max.x), clamped(p_x.y, p_min.y, p_max.y));
		}
		inline Vec3 clamped(const Vec3& p_x, const Vec3& p_min, const Vec3& p_max) {
			return Vec3(clamped(p_x.x, p_min.x, p_max.x), clamped(p_x.y, p_min.y, p_max.y), clamped(p_x.z, p_min.z, p_max.z));
		}
		inline Vec4 clamped(const Vec4& p_x, const Vec4& p_min, const Vec4& p_max) {
			return Vec4(clamped(p_x.x, p_min.x, p_max.x), clamped(p_x.y, p_min.y, p_max.y), clamped(p_x.z, p_min.z, p_max.z), clamped(p_x.w, p_min.w, p_max.w));
		}

		/**
		 * @brief Get largest integer <= x (floor of x).
		 * @param p_x The number to get the floor of.
		 * @return The floor of p_x.
		 */
		inline Real floored(Real p_x) {
			return ::floor(p_x);
		}
		/**
		 * @brief Get largest integer <= x (floor of x), for each component.
		 * @param p_x The vector to get the floor of.
		 * @return The floor of the vector p_x, component-wise.
		 */
		inline Vec2 floored(const Vec2& p_x) {
			return Vec2(floored(p_x.x), floored(p_x.y));
		}
		inline Vec3 floored(const Vec3& p_x) {
			return Vec3(floored(p_x.x), floored(p_x.y), floored(p_x.z));
		}
		inline Vec4 floored(const Vec4& p_x) {
			return Vec4(floored(p_x.x), floored(p_x.y), floored(p_x.z), floored(p_x.w));
		}

		/**
		 * @brief Get the fractional part of p_x (p_x - floor(p_x)).
		 * @param p_x The number to get the fractional part of.
		 * @return The fractional part of p_x.
		 */
		inline Real fract(Real p_x) {
			return (p_x - floor(p_x));
		}
		/**
		 * @brief Get the fractional part of p_x (p_x - floor(p_x)), for each component.
		 * @param p_x The vector to get the fractional parts of.
		 * @return The fractional parts of the vector p_x, component-wise.
		 */
		inline Vec2 fract(const Vec2& p_x) {
			return Vec2(fract(p_x.x), fract(p_x.y));
		}
		inline Vec3 fract(const Vec3& p_x) {
			return Vec3(fract(p_x.x), fract(p_x.y), fract(p_x.z));
		}
		inline Vec4 fract(const Vec4& p_x) {
			return Vec4(fract(p_x.x), fract(p_x.y), fract(p_x.z), fract(p_x.w));
		}

		/**
		 * @brief Get the maximum of two numbers.
		 * @param p_x The first number.
		 * @param p_y The second number.
		 * @return The maximum of p_x, p_y.
		 */
		inline I32 max(I32 p_x, I32 p_y) {
			return (p_x > p_y) ? p_x : p_y;
		}
		inline Real max(Real p_x, Real p_y) {
			return fmaxf(p_x, p_y);
		}

		/**
		 * @brief Get the maximum of a vector with a scalar.
		 * @param p_x The vector to compare with the scalar.
		 * @param p_y The scalar to compare with each component of the vector.
		 * @return The maximum of p_x, p_y, component-wise.
		 */
		inline Vec2 max(const Vec2& p_x, Real p_y) {
			return Vec2(max(p_x.x, p_y), max(p_x.y, p_y));
		}
		inline Vec3 max(const Vec3& p_x, Real p_y) {
			return Vec3(max(p_x.x, p_y), max(p_x.y, p_y), max(p_x.z, p_y));
		}
		inline Vec4 max(const Vec4& p_x, Real p_y) {
			return Vec4(max(p_x.x, p_y), max(p_x.y, p_y), max(p_x.z, p_y), max(p_x.w, p_y));
		}
		
		/**
		 * @brief Get the maximum of two vectors, component-wise.
		 * @param p_x The first vector.
		 * @param p_y The second vector.
		 * @return The maximum of p_x, p_y, component-wise.
		 */
		inline Vec2 max(const Vec2& p_x, const Vec2& p_y) {
			return Vec2(max(p_x.x, p_y.x), max(p_x.y, p_y.y));
		}
		inline Vec3 max(const Vec3& p_x, const Vec3& p_y) {
			return Vec3(max(p_x.x, p_y.x), max(p_x.y, p_y.y), max(p_x.z, p_y.z));
		}
		inline Vec4 max(const Vec4& p_x, const Vec4& p_y) {
			return Vec4(max(p_x.x, p_y.x), max(p_x.y, p_y.y), max(p_x.z, p_y.z), max(p_x.w, p_y.w));
		}
		
		/**
		 * @brief Get the minimum of two numbers.
		 * @param p_x The first number.
		 * @param p_y The second number.
		 * @return The minimum of p_x, p_y.
		 */
		inline I32 min(I32 p_x, I32 p_y) {
			return (p_x < p_y) ? p_x : p_y;
		}
		inline Real min(Real p_x, Real p_y) {
			return fminf(p_x, p_y);
		}

		/**
		 * @brief Get the minimum of a vector with a scalar.
		 * @param p_x The vector to compare with the scalar.
		 * @param p_y The scalar to compare with each component of the vector.
		 * @return The minimum of p_x, p_y, component-wise.
		 */
		inline Vec2 min(const Vec2& p_x, Real p_y) {
			return Vec2(min(p_x.x, p_y), min(p_x.y, p_y));
		}
		inline Vec3 min(const Vec3& p_x, Real p_y) {
			return Vec3(min(p_x.x, p_y), min(p_x.y, p_y), min(p_x.z, p_y));
		}
		inline Vec4 min(const Vec4& p_x, Real p_y) {
			return Vec4(min(p_x.x, p_y), min(p_x.y, p_y), min(p_x.z, p_y), min(p_x.w, p_y));
		}
		
		/**
		 * @brief Get the minimum of two vectors, component-wise.
		 * @param p_x The first vector.
		 * @param p_y The second vector.
		 * @return The minimum of p_x, p_y, component-wise.
		 */
		inline Vec2 min(const Vec2& p_x, const Vec2& p_y) {
			return Vec2(min(p_x.x, p_y.x), min(p_x.y, p_y.y));
		}
		inline Vec3 min(const Vec3& p_x, const Vec3& p_y) {
			return Vec3(min(p_x.x, p_y.x), min(p_x.y, p_y.y), min(p_x.z, p_y.z));
		}
		inline Vec4 min(const Vec4& p_x, const Vec4& p_y) {
			return Vec4(min(p_x.x, p_y.x), min(p_x.y, p_y.y), min(p_x.z, p_y.z), min(p_x.w, p_y.w));
		}

		/**
		 * @brief Get the value of x mixed with y by a factor of a (x*(1-a) + y*a).
		 * @param p_x The starting value of the range.
		 * @param p_y The ending value of the range.
		 * @param p_a The distance towards p_y from p_x [0-1].
		 * @return The value of p_x mixed with p_y.
		 */
		inline Real mixed(Real p_x, Real p_y, Real p_a) {
			return (p_x - p_x*p_a) + p_y*p_a;
		}

		/**
		 * @brief Get the value of x mixed with y by a factor of a (x*(1-a) + y*a), for each component.
		 * @param p_x The starting vector of the range.
		 * @param p_y The ending vector of the range.
		 * @param p_a The distance towards p_y from p_x [0-1].
		 * @return The value of p_x mixed with p_y, component-wise.
		 */
		inline Vec2 mixed(const Vec2& p_x, const Vec2& p_y, Real p_a) {
			return Vec2(mixed(p_x.x, p_y.x, p_a), mixed(p_x.y, p_y.y, p_a));
		}
		inline Vec3 mixed(const Vec3& p_x, const Vec3& p_y, Real p_a) {
			return Vec3(mixed(p_x.x, p_y.x, p_a), mixed(p_x.y, p_y.y, p_a), mixed(p_x.z, p_y.z, p_a));
		}
		inline Vec4 mixed(const Vec4& p_x, const Vec4& p_y, Real p_a) {
			return Vec4(mixed(p_x.x, p_y.x, p_a), mixed(p_x.y, p_y.y, p_a), mixed(p_x.z, p_y.z, p_a), mixed(p_x.w, p_y.w, p_a));
		}

		/**
		 * @brief Get the value of x mixed with y by a factor of a (x*(1-a) + y*a), for each component.
		 * @param p_x The starting vector of the range.
		 * @param p_y The ending vector of the range.
		 * @param p_a The distance vector towards p_y from p_x [0-1], for each component.
		 * @return The value of p_x mixed with p_y, component-wise.
		 */
		inline Vec2 mixed(const Vec2& p_x, const Vec2& p_y, const Vec2& p_a) {
			return Vec2(mixed(p_x.x, p_y.x, p_a.x), mixed(p_x.y, p_y.y, p_a.y));
		}
		inline Vec3 mixed(const Vec3& p_x, const Vec3& p_y, const Vec3& p_a) {
			return Vec3(mixed(p_x.x, p_y.x, p_a.x), mixed(p_x.y, p_y.y, p_a.y), mixed(p_x.z, p_y.z, p_a.z));
		}
		inline Vec4 mixed(const Vec4& p_x, const Vec4& p_y, const Vec4& p_a) {
			return Vec4(mixed(p_x.x, p_y.x, p_a.x), mixed(p_x.y, p_y.y, p_a.y), mixed(p_x.z, p_y.z, p_a.z), mixed(p_x.w, p_y.w, p_a.w));
		}

		/**
		 * @brief Return the negative of a number.
		 * @param p_x The number to negate.
		 * @return The negative of p_x.
		 */
		inline Real negated(Real p_x) {
			return -p_x;
		}

		/**
		 * @brief Return the negative of a vector.
		 * @param p_x The vector to negate.
		 * @return The negative of the vector p_x.
		 */
		inline Vec2 negated(const Vec2& p_x) {
			return Vec2(-p_x.x, -p_x.y);
		}
		inline Vec3 negated(const Vec3& p_x) {
			return Vec3(-p_x.x, -p_x.y, -p_x.z);
		}
		inline Vec4 negated(const Vec4& p_x) {
			return Vec4(-p_x.x, -p_x.y, -p_x.z, -p_x.w);
		}

		/**
		 * @brief Returns p_x if p_x < 0 otherwise, returns 0.
		 * @param p_x The number to clamp to (-inf, 0].
		 * @return p_x or 0 if x >= 0.
		 */
		inline Real negative(Real p_x) {
			return (p_x > 0) ? p_x : 0;
		}
		
		/**
		 * @brief Returns p_x if p_x < 0 otherwise, returns 0, for each component of p_x.
		 * @param p_x The vector to clamp the components of to (-inf, 0].
		 * @return p_x or 0 if x >= 0, component-wise.
		 */
		inline Vec2 negative(const Vec2& p_x) {
			return Vec2(negative(p_x.x), negative(p_x.y));
		}
		inline Vec3 negative(const Vec3& p_x) {
			return Vec3(negative(p_x.x), negative(p_x.y), negative(p_x.z));
		}
		inline Vec4 negative(const Vec4& p_x) {
			return Vec4(negative(p_x.x), negative(p_x.y), negative(p_x.z), negative(p_x.w));
		}

		/**
		 * @brief Returns p_x if p_x > 0 otherwise, returns 0.
		 * @param p_x The number to clamp to [0-inf).
		 * @return p_x or 0 if x <= 0.
		 */
		inline Real positive(Real p_x) {
			return (p_x > 0) ? p_x : 0;
		}
		
		/**
		 * @brief Returns p_x if p_x > 0 otherwise, returns 0, for each component of p_x.
		 * @param p_x The vector to clamp the components of to [0-inf).
		 * @return p_x or 0 if x <= 0, component-wise.
		 */
		inline Vec2 positive(const Vec2& p_x) {
			return Vec2(positive(p_x.x), positive(p_x.y));
		}
		inline Vec3 positive(const Vec3& p_x) {
			return Vec3(positive(p_x.x), positive(p_x.y), positive(p_x.z));
		}
		inline Vec4 positive(const Vec4& p_x) {
			return Vec4(positive(p_x.x), positive(p_x.y), positive(p_x.z), positive(p_x.w));
		}

		/**
		 * @brief Get the integer power of two from 2^0 to 2^32
		 * @param power An integer from 0 to 32
		 */
		inline U32 pow2i(U32 power) {
			return 1 << power;			
		}

		/**
		 * @brief Find the nearest integer to x.
		 * @param p_x The number to round to the nearest integer.
		 * @return The value of p_x rounded to the nearest integer.
		 */
		inline Real rounded(Real p_x) {
			return ::round(p_x);
		}

		/**
		 * @brief Find the nearest integer to x, for each component.
		 * @param p_x The vector to round to the nearest integer.
		 * @return The value of the vector x, with each component rounded to the nearest integer.
		 */
		inline Vec2 rounded(const Vec2& p_x) {
			return Vec2(rounded(p_x.x), rounded(p_x.y));
		}
		inline Vec3 rounded(const Vec3& p_x) {
			return Vec3(rounded(p_x.x), rounded(p_x.y), rounded(p_x.z));
		}
		inline Vec4 rounded(const Vec4& p_x) {
			return Vec4(rounded(p_x.x), rounded(p_x.y), rounded(p_x.z), rounded(p_x.w));
		}

		/**
		 * @brief Get the sign of a number.
		 * @param p_x The number to get the sign of.
		 * @return -1 if the number is negative, 1 if the number is >= 0.
		 */
		inline Real sign(Real p_x) {
			return (p_x < REAL(0.0)) ? -1 : 1;
		}

		/**
		 * @brief Get the sign vector of a vector.
		 * @param p_x The vector to get the sign vector of.
		 * @return -1 if the number is negative, 1 if the number is >= 0, for each component.
		 */
		inline Vec2 sign(const Vec2& p_x) {
			return Vec2(sign(p_x.x), sign(p_x.y));
		}
		inline Vec3 sign(const Vec3& p_x) {
			return Vec3(sign(p_x.x), sign(p_x.y), sign(p_x.z));
		}
		inline Vec4 sign(const Vec4& p_x) {
			return Vec4(sign(p_x.x), sign(p_x.y), sign(p_x.z), sign(p_x.w));
		}
		
		/**
		 * @brief Return 0 if x < edge otherwise, return 1.
		 * @param p_edge The edge of the step function.
		 * @param p_x The value to step.
		 * @return 0 if p_x < p_edge, otherwise, 1.
		 */
		inline Real step(Real p_edge, Real p_x) {
			return (p_x < p_edge) ? 0 : 1;
		}

		/**
		 * @brief Return 0 if x < edge otherwise, return 1, for each component.
		 * @param p_edge The edge of the step function.
		 * @param p_x The vector to step each component of.
		 * @return 0 if p_x < p_edge, otherwise, 1, for each component.
		 */
		inline Vec2 step(Real p_edge, const Vec2& p_x) {
			return Vec2(step(p_edge, p_x.x), step(p_edge, p_x.y));
		}
		inline Vec3 step(Real p_edge, const Vec3& p_x) {
			return Vec3(step(p_edge, p_x.x), step(p_edge, p_x.y), step(p_edge, p_x.z));
		}
		inline Vec4 step(Real p_edge, const Vec4& p_x) {
			return Vec4(step(p_edge, p_x.x), step(p_edge, p_x.y), step(p_edge, p_x.z), step(p_edge, p_x.w));
		}

		/**
		 * @brief Return 0 if x < edge otherwise, return 1, for each component.
		 * @param p_edge The edge vector of the step function.
		 * @param p_x The vector to step each component of.
		 * @return 0 if p_x < p_edge, otherwise, 1, for each component.
		 */
		inline Vec2 step(const Vec2& p_edge, const Vec2& p_x) {
			return Vec2(step(p_edge.x, p_x.x), step(p_edge.y, p_x.y));
		}
		inline Vec3 step(const Vec3& p_edge, const Vec3& p_x) {
			return Vec3(step(p_edge.x, p_x.x), step(p_edge.y, p_x.y), step(p_edge.z, p_x.z));
		}
		inline Vec4 step(const Vec4& p_edge, const Vec4& p_x) {
			return Vec4(step(p_edge.x, p_x.x), step(p_edge.y, p_x.y), step(p_edge.z, p_x.z), step(p_edge.w, p_x.w));
		}


		// ###################################
		// Geometric mathematical functions.
		// ###################################

		/**
		 * @brief Calculate the cross product between two vectors.
		 * @param p_x The first vector.
		 * @param p_y The second vector.
		 * @return The cross product between p_x X p_y.
		 */
		inline Vec3 cross(const Vec3& p_x, const Vec3& p_y) {
			return Vec3(p_x.y*p_y.z - p_x.z*p_y.y, p_x.z*p_y.x - p_x.x*p_y.z, p_x.x*p_y.y - p_x.y*p_y.x);
		}

		/**
		 * @brief Get the distance between two points.
		 * @param p_p0 The start point to get the distance from.
		 * @param p_p1 The end point to get the distance from.
		 * @return The distance from p_p0 to p_p1.
		 */
		inline Real distance(Real p_p0, Real p_p1) {
			return abs(p_p1 - p_p0);
		}
		inline Real distance(const Vec2& p_p0, const Vec2& p_p1) {
			return sqrt(squared(p_p1.x - p_p0.x) + squared(p_p1.y - p_p0.y));
		}
		inline Real distance(const Vec3& p_p0, const Vec3& p_p1) {
			return sqrt(squared(p_p1.x - p_p0.x) + squared(p_p1.y - p_p0.y) + squared(p_p1.z - p_p0.z));
		}
		inline Real distance(const Vec4& p_p0, const Vec4& p_p1) {
			return sqrt(squared(p_p1.x - p_p0.x) + squared(p_p1.y - p_p0.y) + squared(p_p1.z - p_p0.z) + squared(p_p1.w - p_p0.w));
		}

		/**
		 * @brief Get the squared distance between two points.
		 * @param p_p0 The start point to get the squared distance from.
		 * @param p_p1 The end point to get the squared distance from.
		 * @return The squared distance from p_p0 to p_p1.
		 */
		inline Real distanceSquared(Real p_p0, Real p_p1) {
			return squared(p_p1 - p_p0);
		}
		inline Real distanceSquared(const Vec2& p_p0, const Vec2& p_p1) {
			return squared(p_p1.x - p_p0.x) + squared(p_p1.y - p_p0.y);
		}
		inline Real distanceSquared(const Vec3& p_p0, const Vec3& p_p1) {
			return squared(p_p1.x - p_p0.x) + squared(p_p1.y - p_p0.y) + squared(p_p1.z - p_p0.z);
		}
		inline Real distanceSquared(const Vec4& p_p0, const Vec4& p_p1) {
			return squared(p_p1.x - p_p0.x) + squared(p_p1.y - p_p0.y) + squared(p_p1.z - p_p0.z) + squared(p_p1.w - p_p0.w);
		}
		
		/**
		 * @brief Compute the dot product of two vectors.
		 * @param p_x The first vector.
		 * @param p_y The second vector.
		 * @return The dot product of p_x and p_y.
		 */
		inline Real dot(const Vec2& p_x, const Vec2& p_y) {
			return (p_x.x*p_y.x) + (p_x.y*p_y.y);
		}
		inline Real dot(const Vec3& p_x, const Vec3& p_y) {
			return (p_x.x*p_y.x) + (p_x.y*p_y.y) + (p_x.z*p_y.z);
		}
		inline Real dot(const Vec4& p_x, const Vec4& p_y) {
			return (p_x.x*p_y.x) + (p_x.y*p_y.y) + (p_x.z*p_y.z) + (p_x.w*p_y.w);
		}

		/**
		 * @brief Get the length of a vector.
		 * @param p_x The vector to get the length of.
		 * @return The length of the vector.
		 */
		inline Real length(const Vec2& p_x) {
			return sqrt(p_x.x*p_x.x + p_x.y*p_x.y);
		}
		inline Real length(const Vec3& p_x) {
			return sqrt(p_x.x*p_x.x + p_x.y*p_x.y + p_x.z*p_x.z);
		}
		inline Real length(const Vec4& p_x) {
			return sqrt(p_x.x*p_x.x + p_x.y*p_x.y + p_x.z*p_x.z + p_x.w*p_x.w);
		}

		/**
		 * @brief Get the squared length of a vector.
		 * @param p_x The vector to get the squared length of.
		 * @return The squared length of the vector.
		 */
		inline Real lengthSquared(const Vec2& p_x) {
			return (p_x.x*p_x.x) + (p_x.y*p_x.y);
		}
		inline Real lengthSquared(const Vec3& p_x) {
			return (p_x.x*p_x.x) + (p_x.y*p_x.y) + (p_x.z*p_x.z);
		}
		inline Real lengthSquared(const Vec4& p_x) {
			return (p_x.x*p_x.x) + (p_x.y*p_x.y) + (p_x.z*p_x.z) + (p_x.w*p_x.w);
		}

		/**
		 * @brief Returns a vector with length 1.0 parrallell to the original vector.
		 * @param p_x The vector to normalise.
		 * @return A normalised copy of the input vector.
		 */
		inline Vec2 normalised(const Vec2& p_x) {
			Real l = lengthSquared(p_x);
			if (l != 0) { l = inversesqrt(l); }
			return Vec2(p_x.x*l, p_x.y*l);
		}
		inline Vec3 normalised(const Vec3& p_x) {
			Real l = lengthSquared(p_x);
			if (l != 0) { l = inversesqrt(l); }
			return Vec3(p_x.x*l, p_x.y*l, p_x.z*l);
		}
		inline Vec4 normalised(const Vec4& p_x) {
			Real l = lengthSquared(p_x);
			if (l != 0) { l = inversesqrt(l); }
			return Vec4(p_x.x*l, p_x.y*l, p_x.z*l, p_x.w*l);
		}

		/**
		 * @brief Normalises the specified vector.
		 * @param p_x The vector to normalise.
		 */
		inline void normalise(Vec2& p_x) {
			Real l = lengthSquared(p_x);
			if (l != 0) {
				p_x.x *= l; p_x.y *= l;
				l = inversesqrt(l);
			}
		}
		inline void normalise(Vec3& p_x) {
			Real l = lengthSquared(p_x);
			if (l != 0) {
				l = inversesqrt(l);
				p_x.x *= l; p_x.y *= l; p_x.z *= l;
			}
		}
		inline void normalise(Vec4& p_x) {
			Real l = lengthSquared(p_x);
			if (l != 0) {
				l = inversesqrt(l);
				p_x.x *= l; p_x.y *= l; p_x.z *= l; p_x.w *= l;
			}
		}

		/**
		 * @brief Get the vector projection of one vector onto another.
		 * @param p_x The vector to project.
		 * @param p_base The vector to project onto.
		 * @return The vector p_x projected onto p_base.
		 */
		inline Vec2 projected(const Vec2& p_x, const Vec2& p_base) {
			return p_base * (dot(p_base, p_x) / lengthSquared(p_base));
		}
		inline Vec3 projected(const Vec3& p_x, const Vec3& p_base) {
			return p_base * (dot(p_base, p_x) / lengthSquared(p_base));
		}
		inline Vec4 projected(const Vec4& p_x, const Vec4& p_base) {
			return p_base * (dot(p_base, p_x) / lengthSquared(p_base));
		}

		/**
		 * @brief Get a reflected vector about a normal.
		 * @param p_i The incident vector to reflect.
		 * @param p_n The normal vector to reflect by (should be normalised!).
		 * @return The vector p_i reflected by p_n.
		 */
		inline Vec2 reflected(const Vec2& p_i, const Vec2& p_n) {
			Real dot_ni = REAL(2.0)*dot(p_n, p_i);
			return Vec2(p_i.x - p_n.x*dot_ni, p_i.y - p_n.y*dot_ni);
		}
		inline Vec3 reflected(const Vec3& p_i, const Vec3& p_n) {
			Real dot_ni = REAL(2.0)*dot(p_n, p_i);
			return Vec3(p_i.x - p_n.x*dot_ni, p_i.y - p_n.y*dot_ni, p_i.z - p_n.z*dot_ni);
		}
		inline Vec4 reflected(const Vec4& p_i, const Vec4& p_n) {
			Real dot_ni = REAL(2.0)*dot(p_n, p_i);
			return Vec4(p_i.x - p_n.x*dot_ni, p_i.y - p_n.y*dot_ni, p_i.z - p_n.z*dot_ni, p_i.w - p_n.w*dot_ni);
		}

		/**
		 * @brief Get a refracted vector about a normal.
		 * @param p_i The incident vector to refract.
		 * @param p_n The normal vector to refract by (should be normalised!).
		 * @param p_eta The ratio of indices of refraction.
		 * @return The vector p_i refracted by p_n.
		 */
		inline Vec2 refracted(const Vec2& p_i, const Vec2& p_n, Real p_eta) {
			Real dot_nie = p_eta*dot(p_n, p_i);
			Real k = REAL(1.0) - p_eta*p_eta + dot_nie*dot_nie;
			if (k < 0) { return Vec2(0, 0); }
			k = dot_nie + sqrt(k);
			return Vec2(p_eta*p_i.x - k*p_n.x, p_eta*p_i.y - k*p_n.y);
		}
		inline Vec3 refracted(const Vec3& p_i, const Vec3& p_n, Real p_eta) {
			Real dot_nie = p_eta*dot(p_n, p_i);
			Real k = REAL(1.0) - p_eta*p_eta + dot_nie*dot_nie;
			if (k < 0) { return Vec3(0, 0, 0); }
			k = dot_nie + sqrt(k);
			return Vec3(p_eta*p_i.x - k*p_n.x, p_eta*p_i.y - k*p_n.y, p_eta*p_i.z - k*p_n.z);
		}
		inline Vec4 refracted(const Vec4& p_i, const Vec4& p_n, Real p_eta) {
			Real dot_nie = p_eta*dot(p_n, p_i);
			Real k = REAL(1.0) - p_eta*p_eta + dot_nie*dot_nie;
			if (k < 0) { return Vec4(0, 0, 0, 0); }
			k = dot_nie + sqrt(k);
			return Vec4(p_eta*p_i.x - k*p_n.x, p_eta*p_i.y - k*p_n.y, p_eta*p_i.z - k*p_n.z, p_eta*p_i.w - k*p_n.w);
		}
			
			
	} // namespace math

} // namespace Cat


#endif // CAT_CORE_MATH_MATHCORE_H
