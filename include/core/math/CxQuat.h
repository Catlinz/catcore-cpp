#ifndef CX_CORE_MATH_CXQUAT_H
#define CX_CORE_MATH_CXQUAT_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxQuat.h
 * @brief Contains the definition of a Quaternion.
 *
 * @author Catlin Zilinski
 * @date Oct 4, 2015
 */

#include "core/Cx.h"
#include "core/math/CxMath.h"
#include "core/math/CxVec3.h"

#  define CX_QUAT_UNIT_EPS 1e-7f
#  define CX_QUAT_NORM_EPS 1e-10f

namespace cat {

	class CxMat3;

	class CxQuat {
	  public:
		CxReal x, y, z, w;

		/** @brief Quaternion is initialise to garbage. */
		CX_FORCE_INLINE CxQuat() {}
	
		/** @brief Initialise the quaternion to all zeros. */
		CX_FORCE_INLINE CxQuat(CxZero) : x(0), y(0), z(0), w(0) {}

		/** @brief Initialise the quaternion to unit quaternion. */
		CX_FORCE_INLINE CxQuat(CxIdentity) : x(0), y(0), z(0), w(1) {}

		/** @brief Initialise the elements from an array. */
		explicit CX_FORCE_INLINE CxQuat(const CxReal *in_arr)
			: x(in_arr[0]), y(in_arr[1]), z(in_arr[2]), w(in_arr[3]) {}

		/**
		 * @brief Create the quaternion from a rotation matrix.
		 * @param in_m The rotation matrix to initialise the matrix from.
		 */
		CX_INLINE CxQuat(const CxMat3 &in_m);

		/**
		 * @brief Initialise the quaternion from an axis and angle.
		 * @param in_axis The normalized axis of rotation.
		 * @param in_angleRad The angle to rotate by in radians.
		 */
		CxQuat(const CxVec3 &in_axis, CxReal in_angleRad);

		/**
		 * @brief Initialise the quaternion as rotation between two vectors.
		 * This method creates a new Quaternion that when used to rotate the 
		 * vector startVec, the result is endVec.
		 * @param in_startVec The initial vector to rotate.
		 * @param in_endVec the resulting position of the vector after rotation.
		 */
		CxQuat(const CxVec3 &in_startVec, const CxVec3 &in_endVec);

		/** @brief Initialise the elements of the quaternion. */
		CX_FORCE_INLINE CxQuat(CxReal in_x, CxReal in_y, CxReal in_z, CxReal in_w)
			: x(in_x), y(in_y), z(in_z), w(in_w) {}

		/** @brief Copy constructor, simply copies the elements. */
		CX_FORCE_INLINE CxQuat(const CxQuat& in_src)
			: x(in_src.x), y(in_src.y), z(in_src.z), w(in_src.w) {}

		/** @brief Overloaded assignment operator */
		CX_FORCE_INLINE CxQuat& operator=(const CxQuat& in_src) {
			x = in_src.x; y = in_src.y; z = in_src.z; w = in_src.w;  return *this;
		}

		/** @brief Array style element access. */
		CX_FORCE_INLINE CxReal& operator[](CxI32 in_idx) {
			CXD_IF_ERR((in_idx < 0 || in_idx > 3), "CxQuat index out of bounds!");
			return reinterpret_cast<CxReal *>(this)[in_idx];
		}

		/** @brief Const array style element access. */
		CX_FORCE_INLINE const CxReal& operator[](CxI32 in_idx) const {
			CXD_IF_ERR((in_idx < 0 || in_idx > 3), "CxQuat index out of bounds!");
			return reinterpret_cast<const CxReal *>(this)[in_idx];
		}

		/** @return True if the two quaternions are strictly equal. */
		CX_FORCE_INLINE CxBool operator==(const CxQuat &in_q) const {
			return (x == in_q.x) && (y == in_q.y) && (z == in_q.z) && (w == in_q.w);
		}

		/** @return True if the two quaternions are not strictly equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxQuat &in_q) const {
			return (x != in_q.x) || (y != in_q.y) || (z != in_q.z) || (w != in_q.w);
		}

		/** @return The negation of the quaternion. */
		CX_FORCE_INLINE CxQuat operator-() const {
			return CxQuat(-x, -y, -z, -w);
		}

		/** @return The result of the quaternion addition this + in_q. */
		CX_FORCE_INLINE CxQuat operator+(const CxQuat &in_q) const {
			return CxQuat(x + in_q.x, y + in_q.y, z + in_q.z, w + in_q.w);
		}

		/** @return The result of the quaternion subtraction this - in_q. */
		CX_FORCE_INLINE CxQuat operator-(const CxQuat &in_q) const {
			return CxQuat(x - in_q.x, y - in_q.y, z - in_q.z, w - in_q.w);
		}

		/** @return The result of quaternion multiplication. */
		CX_FORCE_INLINE CxQuat operator*(const CxQuat &in_q) const;

		/** @return The result of the scalar multiplication this * in_s. */
		CX_FORCE_INLINE CxQuat operator*(CxReal in_s) const {
			return CxQuat(x*in_s, y*in_s, z*in_s, w*in_s);
		}

		/** @brief Add another quaternion to this quaternion */
		CX_FORCE_INLINE CxQuat& operator+=(const CxQuat &in_v) {
			x += in_v.x; y += in_v.y; z += in_v.z; w += in_v.w;  return *this;
		}

		/** @brief Subtract another quaternion from this quaternion */
		CX_FORCE_INLINE CxQuat& operator-=(const CxQuat &in_v) {
			x -= in_v.x; y -= in_v.y; z -= in_v.z; w-= in_v.w;  return *this;
		}

		/** @brief Multiply this quaternion by another quaternion. */
		CxQuat& operator*=(const CxQuat &in_v);

		/** @brief Multiply this quaternion by a scalar. */
		CX_FORCE_INLINE CxQuat& operator*=(CxReal in_s) {
			x *= in_s; y *= in_s; z *= in_s; w *= in_s;  return *this;
		}

		/** @return The angle of rotation around the axis in the axis-angle repr. */
		CX_FORCE_INLINE CxReal angle() const { return CxAcos(w) * 2.0f; }

		/** @return The axis of rotation for the axis-angle repr. */
		CxVec3 axis() const;

		/** @brief Set the unit axis and angle (in radius) for the quaternion */
		void axisAngle(CxVec3 &in_axis, CxReal &in_angleRad) const;

		/** @return The conjugate of the quaternion (-x,-y,-z,w) */
		CX_FORCE_INLINE CxQuat conjugate() const {
			return CxQuat(-x,-y,-z,w);
		}

		/** @return True if the quaternion is roughly of unit length. */
		CX_FORCE_INLINE CxBool isCloseToUnit() const {
			const CxReal unit_eps = 1e-3f;
			return CxAbs(magnitudeSqr() - 1) < unit_eps;
		}

		/** @return True if the quaternion is roughly of unit length. */
		CX_FORCE_INLINE CxBool isUnit() const {
			const CxReal unit_eps = CX_QUAT_UNIT_EPS;
			return CxAbs(magnitudeSqr() - 1) < unit_eps;
		}
	
		/** @return True if all the elements are exactly zero */
		CX_FORCE_INLINE CxBool isZero() const {
			return (x == 0.0f) && (y == 0.0f) && (z == 0.0f) && (w == 0.0f);
		}

		/** @return The square of the quaternion's magnitude (length). */
		CX_FORCE_INLINE CxReal magnitudeSqr() const {
			return (x*x) + (y*y) + (z*z) + (w*w);
		}

		/** @return The the quaternion's magnitude (length). */
		CX_FORCE_INLINE CxReal magnitude() const {
			return CxSqrt(magnitudeSqr());
		}

		/** @brief Normalize the quaternion. */
		CX_FORCE_INLINE void normalize() {
			const CxReal mag_squared = magnitudeSqr();
			if (mag_squared > CX_QUAT_NORM_EPS) {
				(*this) *= CxRecipSqrt(mag_squared);
			}
		}

		/** @return A copy of the quaternion, normalized. */
		CX_FORCE_INLINE CxQuat normalized() const {
			const CxReal mag_squared = magnitudeSqr();
			return (mag_squared > CX_QUAT_NORM_EPS) ? (*this) * CxRecipSqrt(mag_squared) : CxQuat(kCxZero);
		}

		/**
		 * @brief Rotate a vector by the quaternion (assumes unit quaternion).
		 */
		CxVec3 rotate(const CxVec3 &in_v) const;

		/**
		 * @brief Inversly rotate a vector by the quaternion (assumes unit quaternion).
		 */
		CxVec3 rotateInv(const CxVec3 &in_v) const;

		/** @brief Method to set all the components of the quaternion. */
		CX_FORCE_INLINE void setXYZW(CxReal in_x, CxReal in_y, CxReal in_z, CxReal in_w) {
			x = in_x;  y = in_y;  z = in_z;  w = in_w;
		}
		
		/** @brief Set the quaternion to a zero quaternion. */
		CX_FORCE_INLINE void setZero() { x = y = z = w = 0; }

		/**
		 * Calculate the vector dv that will translate by the specified 
		 * dx, dy, dz along the LOCAL axis represented by the orientation of 
		 * this quaternion.
		 * @param in_dx The translation along the local x-axis.
		 * @param in_dy The translation along the local y-axis.
		 * @praam in_dz The translation along the local z-axis.
		 * @return A vector representing the local translation vector.
		 */
		CxVec3 translation(CxReal in_dx, CxReal in_dy, CxReal in_dz) const;

		/** @return The first (x-axis) basis vector for the quaternion */
		CxVec3 xAxis() const;
		
		/** @return A CxVec3 from the x, y and z components of this quaternion. */
		CX_FORCE_INLINE CxVec3 xyz() const { return CxVec3(x, y, z); }
				
		/** @return The second (y-axis) basis vector for the quaternion */
		CxVec3 yAxis() const;
		
		/** @return The third (z-axis) basis vector for the quaternion */
		CxVec3 zAxis() const;

	};

	CX_FORCE_INLINE CxQuat::CxQuat(const CxVec3 &in_axis, CxReal in_angleRad) {
		CXD_IF_ERR(!in_axis.isUnit(), "Axis vector needs to be unit length!");
		const CxReal half_a = in_angleRad*0.5f;
		const CxReal sin_a = CxSin(half_a);
		w = CxCos(half_a);
		x = in_axis.x*sin_a;  y = in_axis.y*sin_a;  z = in_axis.z*sin_a;
	}

	CX_FORCE_INLINE CxQuat::CxQuat(const CxVec3 &in_startVec, const CxVec3 &in_endVec) {
		const CxVec3 axis = CxCross(in_startVec, in_endVec);
		x = axis.x;  y = axis.y;  z = axis.z;
		w = CxDot(in_startVec, in_endVec);
		w += magnitude();
		normalize();
	}

	CX_FORCE_INLINE CxQuat CxQuat::operator*(const CxQuat &in_q) const {
		return CxQuat(w*in_q.x + x*in_q.w + y*in_q.z - z*in_q.y,
						  w*in_q.y + y*in_q.w + z*in_q.x - x*in_q.z,
						  w*in_q.z + z*in_q.w + x*in_q.y - y*in_q.x,
						  w*in_q.w - x*in_q.x - y*in_q.y - z*in_q.z);
	}

	CX_FORCE_INLINE CxQuat & CxQuat::operator*=(const CxQuat &in_q) {
		const CxReal new_x = w*in_q.x + x*in_q.w + y*in_q.z - z*in_q.y;
		const CxReal new_y = w*in_q.y + y*in_q.w + z*in_q.x - x*in_q.z;
		const CxReal new_z = w*in_q.z + z*in_q.w + x*in_q.y - y*in_q.x;
		w = w*in_q.w - x*in_q.x - y*in_q.y - z*in_q.z;
		x = new_x;  y = new_y;  z = new_z;
		return *this;
	}

	CX_FORCE_INLINE CxVec3 CxQuat::axis() const {
		const CxReal eps = CX_QUAT_NORM_EPS;
		const CxReal m_2 = x*x + y*y + z*z;
		
		if (m_2 > eps) {
			const CxReal m = CxRecipSqrt(m_2);
			return CxVec3(x*m, y*m, z*m);
		}
		/* If zero-length axis, assume 0 rotation around x-axis */
		else { return CxVec3(1,0,0); }
	}

	CX_FORCE_INLINE void CxQuat::axisAngle(CxVec3 &in_axis, CxReal &in_angleRad) const {
		const CxReal eps = CX_QUAT_NORM_EPS;
		const CxReal m_2 = x*x + y*y + z*z;
		
		if (m_2 > eps) {
			const CxReal m = CxRecipSqrt(m_2);
			const CxReal a_rad = CxAcos(w)*2.0f;
			in_axis.setXYZ(x*m, y*m, z*m);
			in_angleRad = a_rad;
		}
		/* If zero-length axis, assume 0 rotation around x-axis */
		else { in_axis.setXYZ(1,0,0);  in_angleRad = 0; }
	}

	CX_FORCE_INLINE CxVec3 CxQuat::rotate(const CxVec3 &in_v) const {
		const CxF32 vxx2 = 2*in_v.x*x;
		const CxF32 vyy2 = 2*in_v.y*y;
		const CxF32 vzz2 = 2*in_v.z*z;
		const CxF32 xx = x*x;  const CxF32 yy = y*y;
		const CxF32 zz = z*z;  const CxF32 ww = w*w;
		const CxF32 dot2 = vxx2 + vyy2 + vzz2;
		const CxF32 w2 = 2*w;
				
		return CxVec3(in_v.x*( xx-yy-zz+ww) + x*(dot2 - vxx2) + w2*(in_v.z*y - in_v.y*z),
						  in_v.y*(-xx+yy-zz+ww) + y*(dot2 - vyy2) + w2*(in_v.x*z - in_v.z*x),
						  in_v.z*(-xx-yy+zz+ww) + z*(dot2 - vzz2) + w2*(in_v.y*x - in_v.x*y));
	}
	
	CX_FORCE_INLINE CxVec3 CxQuat::rotateInv(const CxVec3 &in_v) const {
		const CxF32 vxx2 = 2*in_v.x*x;
		const CxF32 vyy2 = 2*in_v.y*y;
		const CxF32 vzz2 = 2*in_v.z*z;
		const CxF32 xx = x*x;  const CxF32 yy = y*y;
		const CxF32 zz = z*z;  const CxF32 ww = w*w;
		const CxF32 dot2 = vxx2 + vyy2 + vzz2;
		const CxF32 w2 = 2*w;
				
		return CxVec3(in_v.x*( xx-yy-zz+ww) - x*(vxx2 - dot2) + w2*(in_v.y*z - in_v.z*y),
						  in_v.y*(-xx+yy-zz+ww) - y*(vyy2 - dot2) + w2*(in_v.z*x - in_v.x*z),
						  in_v.z*(-xx-yy+zz+ww) - z*(vzz2 - dot2) + w2*(in_v.x*y - in_v.y*x));
	}

	CX_FORCE_INLINE CxVec3 CxQuat::translation(CxReal in_dx, CxReal in_dy, CxReal in_dz) const {
		const CxReal x2 = 2.0f*x;              const CxReal y2 = 2.0f*y;
		const CxReal z2 = 2.0f*z;
		const CxReal f1 = x*in_dy + w*in_dz - y*in_dx;
		const CxReal f2 = x*in_dz - w*in_dy - z*in_dx;
		const CxReal f3 = y*in_dz + w*in_dx - z*in_dy;
		return CxVec3(in_dx + y2*f1 + z2*f2,
						  in_dy - x2*f1 + z2*f3,
						  in_dz - x2*f2 - y2*f3);
	}

	CX_FORCE_INLINE CxVec3 CxQuat::xAxis() const {
		const CxF32 y2 = 2.0f*y;  const CxF32 z2 = 2.0f*z;
		return CxVec3(1.0f - y2*y - z2*z, y2*x + z2*w, z2*x - y2*w);
	}

	CX_FORCE_INLINE CxVec3 CxQuat::yAxis() const {
		const CxF32 x2 = 2.0f*x;  const CxF32 z2 = 2.0f*z;
		return CxVec3(x2*y - z2*w, 1.0f - x2*x - z2*z, z2*y + x2*w);
	}
	
	CX_FORCE_INLINE CxVec3 CxQuat::zAxis() const {
		const CxF32 x2 = 2.0f*x;  const CxF32 y2 = 2.0f*y;
		return CxVec3(x2*z + y2*w, y2*z - x2*w, 1.0f - x2*x - y2*y);
	}
	
	/** @return The dot product of two quaternions. */
	CX_FORCE_INLINE CxReal CxDot(const CxQuat &in_a, const CxQuat &in_b) {
		return (in_a.x*in_b.x) + (in_a.y*in_b.y) + (in_a.z*in_b.z) + (in_a.w*in_b.w);
	}

	/** @return The angle between to quaternion rotations in radians */
	CX_FORCE_INLINE CxReal CxAngle(const CxQuat &in_a, const CxQuat &in_b) {
		return CxAcos(CxDot(in_a, in_b))*2.0f;
	}

	/** @return Return true if the two quaternions (a and b) are equal within a given error. */
	CX_FORCE_INLINE CxBool CxEq(const CxQuat &in_a, const CxQuat &in_b, CxReal in_epsilon = CX_REAL_EPSILON) {
		return (CxEq(in_a.x, in_b.x, in_epsilon) &&
				  CxEq(in_a.y, in_b.y, in_epsilon) &&
				  CxEq(in_a.z, in_b.z, in_epsilon) &&
				  CxEq(in_a.w, in_b.w, in_epsilon));
	}

	/** @return True if all elements of the quaternion are finite values (not NaN or INF). */
	CX_FORCE_INLINE CxBool CxIsFinite(const CxQuat &in_v) {
		return CxIsFinite(in_v.x) && CxIsFinite(in_v.y) && CxIsFinite(in_v.z) && CxIsFinite(in_v.w);
	}

	/**
	 * @brief Method to perform lineary interpolation between two quaternions.
	 * @param p_t The interpolation factor [0-1].
	 */
	CX_INLINE CxQuat CxLerp(const CxQuat &in_a, const CxQuat &in_b, CxReal in_t) {
		const CxReal x1 = in_a.x;  const CxReal y1 = in_a.y;
		const CxReal z1 = in_a.z;  const CxReal w1 = in_a.w;
		
		const CxReal x2 = in_b.x;  const CxReal y2 = in_b.y;
		const CxReal z2 = in_b.z;  const CxReal w2 = in_b.w;
		
		return CxQuat(x1 + in_t*(x1 - x2), y1 + in_t*(y1 - y2),
						  z1 + in_t*(z1 - z2), w1 + in_t*(w1 - w2));
	}

		/**
	 * @brief Method to perform spherical linear interpolation between two quaternions.
	 * @param p_t The interpolation factor [0-1].
	 */
	CX_INLINE CxQuat CxSlerp(const CxQuat &in_a, const CxQuat &in_b, CxReal in_t) {
		if (in_a == in_b) { return in_a; }
		CxReal a_dot_b = CxDot(in_a, in_b);
		CxQuat qb(in_b);

		/* if dot(a,b) < 0, then angle between a,b > 90, so negating b gets the 
		 * smaller of the two rotations */
		if (a_dot_b < 0) {
			a_dot_b = -a_dot_b;
			qb.setXYZW(-in_b.x, -in_b.y, -in_b.z, -in_b.w);
		}

		/* 1.) Make sure don't divide by zero (cos(1) = 1).
		 * 2.) Use linear interpolation when near 180 to prevent NaN.
		 */
		if (a_dot_b < 0.999f) {
			const CxReal angle = CxAcos(a_dot_b);
			const CxReal s_a_recip = 1.0f / CxSin(angle);
			const CxReal a_t = in_t*angle;
			const CxReal s1 = CxSin(angle - a_t)*s_a_recip;
			const CxReal s2 = CxSin(a_t)*s_a_recip;
			return CxQuat((in_a.x*s1 + qb.x*s2), (in_a.y*s1 + qb.y*s2),
							  (in_a.z*s1 + qb.z*s2), (in_a.w*s1 + qb.w*s2));
		}
		else { return CxLerp(in_a, qb, in_t); }
	}

} // namespace cat

#endif // CX_CORE_MATH_CXQUAT_H
