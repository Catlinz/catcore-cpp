#ifndef CAT_CORE_MATH_QUATERNION_H
#define CAT_CORE_MATH_QUATERNION_H
/**
 * @copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file quaternion.h: 
 *
 * Contains the definitions of the Quaternion type and math functions to 
 * operate on Quaternions.
 *
 * @author Catlin Zilinski
 * @date Dec 3, 2013
 */

#include "core/math/mathcore.h"
#include "core/math/angle.h"
#include "core/math/mat3.h"

namespace Cat {

	/**
	 * @class Quaternion quaternion.h "core/math/quaternion.h"
	 * @brief A class representing a Quaternion.
	 * 
	 * @author Catlin Zilinski
	 * @version 3
	 * @since June 13, 2013
	 */
	class Quaternion {
	  public:
		static const Real kEpsilon;
		static const Quaternion kZero;
		static const Quaternion kIdentity;
		
		Real x, y, z, w;
		
		/**
		 * @brief Creates an identity quaternion.
		 */
		inline Quaternion()
			: x(REAL(0.0)), y(REAL(0.0)), z(REAL(0.0)), w(REAL(1.0)) {}

		/**
		 * @brief Creates a Quaternion from the specified components.
		 * @param qx The x component of the quaternion.
		 * @param qy The y component of the quaternion.
		 * @param qz The z component of the quaternion.
		 * @param qw The w component of the quaternion.
		 */
		inline Quaternion(Real qx, Real qy, Real qz, Real qw) 
			: x(qx), y(qy), z(qz), w(qw) {}

		/**
		 * @brief Creates a Quaternion from an array of 4 Reals.
		 * @param arr The array of Reals.
		 */
		inline explicit Quaternion(const Real* const arr)
			: x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]) {}

		/**
		 * @brief Create a Quaternion from a vector with w = 0.
		 * @param p_vec The vector to create the quaternion from.
		 */
		inline explicit Quaternion(const Vec3& p_vec)
			: x(p_vec.x), y(p_vec.y), z(p_vec.z), w(0) {
		}
		
		
		/**
		 * @brief Creates a Quaternion from an axis and an angle (in radians).
		 * @param axis The vector representing the axis of rotation.
		 * @param angle The angle of rotation.
		 */
		inline Quaternion(const Vec3& axis, const Angle& angle) {
			fromAxisAngle(axis, angle);
		}

		/**
		 * @brief Creates a Quaternion that rotates p_start to p_end.
		 * @param p_start The start vector we are rotating.
		 * @param p_end The position of the vector after this quaternion applied.
		 */
		inline Quaternion(const Vec3& p_start, const Vec3& p_end) {
			fromTwoVectors(p_start, p_end);
		}
		
		/**
		 * @brief Creates a Quaternion from Euler angles in degrees.
		 * @param pitch The pitch angle.
		 * @param yaw The yaw angle.
		 * @param roll The roll angle.
		 */
		inline Quaternion(const Angle& pitch, const Angle& yaw, const Angle& roll) {
			fromEulerAngles(pitch, yaw, roll);
		}

		/** 
		 * @brief Creates a Quaternion from a local orthonormal basis.
		 * @param vec3 xaxis The x-axis vector.
		 * @param vec3 yaxis The y-axis vector.
		 * @param vec3 zaxis The x-axis vector.
		 */
		inline Quaternion(const Vec3& xaxis, const Vec3& yaxis, const Vec3& zaxis) {
			fromLocalAxes(xaxis, yaxis, zaxis);
		}
		
		/**
		 * @brief Creates a Quaternion from a 3x3 rotation matrix.
		 * @param mat The 3x3 rotation matrix.
		 */
		inline explicit Quaternion(const Mat3& mat) {
			fromMatrix(mat);
		}

		/**
		 * @brief Creates a Quaternion from an existing Quaternion.
		 * @param src The Quaternion to copy.
		 */
		inline Quaternion(const Quaternion& src) {
			x = src.x; y = src.y; z = src.z; w = src.w;
		}
		
		/**
		 * @brief Overloaded Quaternion assignment operator.
		 * @param src The Quaternion to make a copy of.
		 * @return A reference to this quaternion.
		 */
		inline Quaternion& operator=(const Quaternion& src) {
			x = src.x; y = src.y; z = src.z; w = src.w;
			return *this;
		}

		/**
		 * @brief Overloaded equality operator for Quaternion equality.
		 * @param rhs The Quaternion we are comparing to.
		 * @return True if the Quaternions are equal.
		 */
		inline Boolean operator==(const Quaternion& rhs) const {
			return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
		}
		
		/**
		 * @brief Overloaded inequality operator for Quaternions.
		 * @param rhs The Quaternion we are comparing to.
		 * @return True if the Quaternions are not equal.
		 */
		inline Boolean operator!=(const Quaternion& rhs) const {
			return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w);
		}
		
		/**
		 * @brief Tests for approximate equality.
		 * @param rhs The Quaternion we are comparing to.
		 * @return True if the two Quaternions are approximately equals.
		 */
		inline Boolean approx(const Quaternion& rhs) const {
			return (fabs(x - rhs.x) < Quaternion::kEpsilon && 
					  fabs(y - rhs.y) < Quaternion::kEpsilon &&
					  fabs(z - rhs.z) < Quaternion::kEpsilon && 
					  fabs(w - rhs.w) < Quaternion::kEpsilon);
		}
		
		/**
		 * @brief Tests to see if the Quaternion is (approximately) unit length.
		 * @return True if the Quaternion is considered to be unit length.
		 */
		inline Boolean isUnitLength() const {
			return fabs(((x*x) + (y*y) + (z*z) + (w*w)) - REAL(1.0)) < Quaternion::kEpsilon;
		}
		
		/**
		 * @brief Computes the length of the quaternion.
		 * @return The length of the quaternion.
		 */
		inline Real length() const {
			return sqrt(x*x + y*y + z*z + w*w);
		}
		
		/**
		 * @brief Computes the square of the length of the quaternion.
		 * @return The length of the quaternion, squared.
		 */
		inline Real lengthSquared() const {
			return (x*x) + (y*y) + (z*z) + (w*w);
		}
		
		/**
		 * @brief Computes the dot product between this and another Quaternion.
		 * @param rhs The Quaternion on the rhs of the dot product.
		 * @return The dot product of this*rhs.
		 */
		inline Real dotProduct(const Quaternion& rhs) const {
			return (x*rhs.x) + (y*rhs.y) + (z*rhs.z) + (w*rhs.w);
		}

		/**
		 * @brief Normalises the Quaternion.
		 * @return A reference to the Quaternion.
		 */
		inline Quaternion& normalise() {
			Real factor = lengthSquared();
			if (factor != 1) {
				factor = REAL(1.0)/sqrt(factor);
				x *= factor;
				y *= factor;
				z *= factor;
				w *= factor;
			}
			return *this;
		}

		/**
		 * @brief Get a new Quaternio that is this Quaternion, normalised.
		 * @return The normalised Quaternion.
		 */
		inline Quaternion normalised() const {
			Real factor = lengthSquared();
			if (factor != 1) {
				factor = REAL(1.0)/sqrt(factor);
				return Quaternion(x*factor, y*factor, z*factor, w*factor);				
			}
			else {
				return Quaternion(x, y, z, w);
			}
		}
		
		/**
		 * @brief Inverts a quaternion (which might not be unit length).
		 * @return A reference to the quaternion.
		 */
		inline Quaternion& invert() {
			Real factor = REAL(1.0)/lengthSquared();
			x *= -factor;
			y *= -factor;
			z *= -factor;
			w *= factor;
			return *this;
		}

		/**
		 * @brief Inverts a unit length quaternion (same as conjugate).
		 * @return A reference to the quaternion.
		 */
		inline Quaternion& invertUnit() {
			x = -x; y = -y; z = -z;
			return *this;			
		}

		/**
		 * @brief Gets the inverse of a quaternion (which might not be unit length).
		 * @return The inverse of the quaternion.
		 */
		inline Quaternion inverse() const {
			Real factor = REAL(1.0)/lengthSquared();
			return Quaternion(x*-factor, y*-factor, z*-factor, w*factor);
		}
		
		/**
		 * @brief Gets the inverse of a unit quaternion.
		 * @return The inverse of the unit quaternion.
		 */
		inline Quaternion unitInverse() const {
			return Quaternion(-x, -y, -z, w);
		}

		/**
		 * @brief Get the conjugate / unit inverse of a quaternion.
		 * @return The conjugate of a quaternion.
		 */
		inline Quaternion operator-() const {
			return Quaternion(-x, -y, -z, -w);
		}
		

		/**
		 * @brief The overloaded addition operator to add two Quaternions.
		 * @param rhs The Quaternion to add with this one.
		 * @return A new Quaternion that is the sum this+rhs.
		 */
		inline Quaternion operator+(const Quaternion& rhs) const {
			return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}

		/**
		 * @brief The overloaded operator to add a quaternion to this one.
		 * @param rhs The quaternion to add to this one.
		 * @return A reference to this quaternion.
		 */
		inline Quaternion& operator+=(const Quaternion& rhs) {
			x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
			return *this;			
		}
		
		/**
		 * @brief The overloaded subtraction operator to subtract to Quaternions.
		 * @param rhs The Quaternion to subtract from this one.
		 * @return A new Quaternion that is the difference this - rhs.
		 */
		inline Quaternion operator-(const Quaternion& rhs) const {
			return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}

		/** 
		 * @brief THe overloaded operator to subtract a quaternion from this one.
		 * @param rhs The Quaternion to subtract from this one.
		 * @return A reference to this Quaternion.
		 */
		inline Quaternion& operator-=(const Quaternion& rhs) {
			x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
			return *this;
		}

		/**
		 * @brief The overloaded multiplication operator to multiply by a scalar.
		 * @param scalar The scalar to multiply the Quaternion by.
		 * @return A copy of the Quaternion scaled by the scalar.
		 */
		inline Quaternion operator*(const Real scalar) const {
			return Quaternion(x*scalar, y*scalar, z*scalar, w*scalar);
		}
		inline friend Quaternion operator*(const Real scalar, const Quaternion& rhs) {
			return Quaternion(scalar*rhs.x, scalar*rhs.y, scalar*rhs.z, scalar*rhs.w);
		}
		

		/**
		 * @brief The overloaded operator to multiply this quaternion by a scalar.
		 * @param scalar The scalar to multiply the Quaternion by.
		 * @return A reference to this Quaternion.
		 */
		inline Quaternion& operator*=(const Real scalar) {
			x *= scalar; y *= scalar; z *= scalar; w *= scalar;
			return *this;
		}
		
		/**
		 * @brief The overloaded multiplication operator to multiply Quaternions.
		 * @param rhs The Quaternion to multiply with.
		 * @return A new Quaternion that is the product this*rhs.
		 */
		Quaternion operator*(const Quaternion& rhs) const;
		
		/**
		 * @brief The overloaded operator to multiply by another Quaternion.
		 * @param rhs The Quaternion to multiply by.
		 * @return A Reference to this quaternion.
		 */
		Quaternion& operator*=(const Quaternion& rhs);
		
		/**
		 * @brief The overloaded operator to rotate a Vector by the Quaternion.
		 * @param rhs The vector to rotate.
		 * @return The rotated Vector.
		 */
		Vec3 operator*(const Vec3& rhs) const;

		/**
		 * @brief The overloaded division operator to divide by a scalar.
		 * @param scalar The scalar to divide the Quaternion by.
		 * @return A copy of the Quaternion divided by the scalar.
		 */
		inline Quaternion operator/(const Real scalar) const {
			return Quaternion(x/scalar, y/scalar, z/scalar, w/scalar);
		}
		inline friend Quaternion operator/(const Real scalar, const Quaternion& rhs) {
			return Quaternion(scalar/rhs.x, scalar/rhs.y, scalar/rhs.z, scalar/rhs.w);
		}

		/**
		 * @brief The overloaded operator to divide this quaternion by a scalar.
		 * @param scalar The scalar to divide the Quaternion by.
		 * @return A reference to this Quaternion.
		 */
		inline Quaternion& operator/=(const Real scalar) {
			x /= scalar; y /= scalar; z /= scalar; w /= scalar;
			return *this;
		}

		/**
		 * @brief The method to rotate a vector by the quaternion.
		 * @param v The vector to be rotated.
		 */
		void rotate(Vec3& v) const;
				
		/**
		 * @brief Get a vector representing translation.
		 * This method returns a vector representing the translation 
		 * along the x,y,z axis' corresponding tho the orientation stored
		 * in the quaternion, by the specified amounts.
		 * @return A vector representing the translation.
		 */
		Vec3 getTranslation(Real dx, Real dy, Real dz) const;

		/**
		 * @brief Sets a Quaternion from an axis and an angle (in radians).
		 * @param axis The vector representing the axis of rotation.
		 * @param angle The angle of rotation in radians.
		 */
		void fromAxisAngle(const Vec3& axis, const Angle& angle);

		/**
		 * @brief Get the rotation axis and angle from the quaternion.
		 * @param axis The vector to set to the axis.
		 * @param angle The angle to set to the angle.
		 */
		void toAxisAngle(Vec3& axis, Angle& angle) const;

		/**
		 * @brief Get the rotation axis and angle from the quaternion.
		 * This method is for if you know what the axis of rotation is,
		 * and allows for you to pass in this axis as a check to make
		 * sure if the axis we get back is the negative, we negate both
		 * the axis and angle to make it consistent.
		 * @param axis The vector to set to the axis.
		 * @param angle The angle to set to the angle.
		 * @param p_checkAxis The axis that we should be rotating around.
		 */
		void toAxisAngle(Vec3& axis, Angle& angle, const Vec3& p_checkAxis) const;	
		
		/**
		 * @brief Sets a Quaternion from Euler angles in degrees.
		 * Assumes angles are applied yaw, roll, pitch
		 * @param pitch The pitch angle.
		 * @param yaw The yaw angle.
		 * @param roll The roll angle.
		 */
		void fromEulerAngles(const Angle& pitch, const Angle& yaw, const Angle& roll);

		/**
		 * @brief Converts quaternion to euler angles.
		 * Assumes angles are applied yaw, roll, pitch
		 * Code for Quaternion to Euler from:
		 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
		 * @param po_pitch The pitch (x) represented by the quaternion.
		 * @param po_yaw The yaw (y) represented by the quaternion.
		 * @param po_roll The roll (z) represented by the quaternion.
		 */
		void toEulerAngles(Angle& po_pitch, Angle& po_yaw, Angle& po_roll);

		/** 
		 * @brief Sets a Quaternion from a local orthonormal basis.
		 * @param vec3 xaxis The x-axis vector.
		 * @param vec3 yaxis The y-axis vector.
		 * @param vec3 zaxis The x-axis vector.
		 */
		void fromLocalAxes(const Vec3& xaxis, const Vec3& yaxis, const Vec3& zaxis);

		/**
		 * @brief Gets the orthonormal basis from the quaternion.
		 * @param xaxis The 'xaxis' vector.
		 * @param yaxis The 'yaxis' vector.
		 * @param zaxis The 'zaxis' vector.
		 */
		void toLocalAxes(Vec3& xaxis, Vec3& yaxis, Vec3& zaxis) const;
					
		/**
		 * @brief Sets a Quaternion from a 3x3 rotation matrix.
		 * @param mat The 3x3 rotation matrix.
		 */
		void fromMatrix(const Mat3& mat);	
		
		/**
		 * @brief Get a 3x3 rotation matrix from the quaternion.
		 * @param m The Mat3 object to set from the quaternion.
		 */
		void toMatrix(Mat3& m) const;

		/**
		 * @brief Sets a Quaternion that rotates p_start to p_end.
		 * @param p_start The start vector we are rotating.
		 * @param p_end The position of the vector after this quaternion applied.
		 */
		void fromTwoVectors(const Vec3& p_start, const Vec3& p_end);

		/**
		 * @brief Get the local x-axis from this quaternion.
		 * That is, the vector resulting from (1,0,0) * this.
		 * @return The vector corresponding to the local x-axis.
		 */
		inline Vec3 xAxis() const {
			return (*this) * Vec3(REAL(1.0), REAL(0.0), REAL(0.0));
		}

		/**
		 * @brief Get the local y-axis from this quaternion.
		 * That is, the vector resulting from (0,1,0) * this.
		 * @return The vector corresponding to the local y-axis.
		 */
		inline Vec3 yAxis() const {
			return (*this) * Vec3(REAL(0.0), REAL(1.0), REAL(0.0));
		}
		
		/**
		 * @brief Get the local z-axis from this quaternion.
		 * That is, the vector resulting from (0,0,1) * this.
		 * @return The vector corresponding to the local z-axis.
		 */
		inline Vec3 zAxis() const {
			return (*this) * Vec3(REAL(0.0), REAL(0.0), REAL(1.0));
		}

		/**
		 * @brief Method to perform LERP between two quaternions.
		 * @param p_q1 The starting quaternion.
		 * @param p_q2 The ending quaternion.
		 * @param p_t The amount of interpolation [0 - 1].
		 * @return A linearly interpolated quaternion.
		 */
		static inline Quaternion lerp(const Quaternion& p_q1, const Quaternion& p_q2, Real p_t) {
			Real one_minus = REAL(1.0) - p_t;
			return Quaternion(p_q1.x * one_minus + p_q2.x * p_t,
									p_q1.y * one_minus + p_q2.y * p_t,
									p_q1.z * one_minus + p_q2.z * p_t,
									p_q1.w * one_minus + p_q2.w * p_t);
		}
		
		/**
		 * @brief Method to perform SLERP between two quaternions.
		 * @param p_q1 The starting quaternion.
		 * @param p_q2 The ending quaternion.
		 * @param p_t The amount of interpolation [0 - 1].
		 * @return A spherically interpolated quaternion.
		 */
		static Quaternion slerp(const Quaternion& p_q1, const Quaternion& p_q2, Real p_t);

	};

#ifdef DEBUG
	std::ostream& operator<<(std::ostream& out, const Quaternion& quaternion);
#endif //DEBUG

} // namespace Cat




#endif // CAT_CORE_MATH_QUATERNION_H

