#ifndef CX_CORE_GEOM_CXTRANSFORM3D_H
#define CX_CORE_GEOM_CXTRANSFORM3D_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxTransform3D.h
 * @brief Contains a class to hold position and orientation.
 *
 * @author Catlin Zilinski
 * @date Oct 7, 2015
 */

#include "core/Cx.h"
#include "core/math/CxQuat.h"
#include "core/math/CxVec3.h"

namespace cat {

	/**
	 * @class CxTransform3D CxTransform3D.h "core/geom/CxTransform3D.h"
	 *
	 * Stores orientation and position using a quaternion and vector.
	 * The CxTransform3D can be basically thought of as a coordinate frame, 
	 * with the position being the origin, relative to a parent (or the 
	 * global coordinate frame) and the orientation being the transformed xyz axis'.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 7, 2015
	 */
	class CxTransform3D {
	  public:
		CxQuat o;
		CxVec3 p;

		/** @brief Initialise to garbage. */
		CX_FORCE_INLINE CxTransform3D() : m_cached(0) {}

		/** @brief Initialise to identity transformation */
		CX_FORCE_INLINE CxTransform3D(CxIdentity)
			: o(CxIdentity), p(CxZero), m_cached(0) {}

		/** @brief Initialise with the specified orientation */
		explicit CX_FORCE_INLINE CxTransform3D(const CxQuat &in_q)
			: o(in_q), p(CxZero), m_cached(0) {
			CXD_IF_ERR(!(CxIsFinite(in_q) || in_q.isCloseToUnit()),
						  "Initialising CxTransform3D with invalid CxQuat!");
		}

		/** @brief Initialise with the specified position */
		explicit CX_FORCE_INLINE CxTransform3D(const CxVec3 &in_v)
			: o(CxIdentity), p(in_v), m_cached(0) {}

		/** @brief Initialise with the specified orientation and pos. */
		CX_FORCE_INLINE CxTransform3D(const CxQuat &in_q, const CxVec3 &in_v)
			: o(in_q), p(in_v), m_cached(0) {
			CXD_IF_ERR(!(CxIsFinite(in_q) || in_q.isCloseToUnit()),
						  "Initialising CxTransform3D with invalid CxQuat!");
		}

		/** @brief Initialise the transformation from a CxMat4 */
		explicit CxTransform3D(const CxMat4 &in_m);

		/** @brief Copy constructor */
		CX_FORCE_INLINE CxTransform3D(const CxTransform3D &in_tx)
			: o(in_tx.o), p(in_tx.p), m_cached(0) {}

		/** @brief Assignment operator */
		CX_FORCE_INLINE CxTransform3D & operator=(const CxTransform3D &in_tx) {
			o = in_tx.o;  p = in_tx.p;
			m_cached = 0;  return *this;
		}

		/** @return True if the two transformations are strictly equal. */
		CX_FORCE_INLINE CxBool operator==(const CxTransform3D &in_tx) const {
			return o == in_tx.o && p == in_tx.p;
		}

		/** @return True if the two transformations are strictly NOT equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxTransform3D &in_tx) const {
			return o != in_tx.o || p != in_tx.p;
		}

		/** @return A transformation transformed by this one (in_tx * this) */
		CxTransform3D operator*(const CxTransform3D &in_tx) const;

		/** @return Set this transform to in_tx * this */
		CxTransform3D & operator*=(const CxTransform3D &in_tx) const;

		/** @brief Copy the current transformation into the specified matrix. */
		void cacheMatrix(CxMat4 &out_m) const;

		/** @brief Copy the current transformation relative to a parent space. */
		void cacheMatrix(CxMat4 &out_m, const CxMat4 &in_parent);

		/** @return True if the transformation is finite w/ close to unit quat */
		CX_FORCE_INLINE CxBool isCloseToValid() const {
			return o.isFinite() && o.isCloseToUnit() && p.isFinite();
		}
		
		/** @return True if the transformation is finite w/ unit quat */
		CX_FORCE_INLINE CxBool isValid() const {
			return o.isFinite() && o.isUnit() && p.isFinite();
		}

		/** @return The inverse transformation (flip orientation and negate position). */
		CxTransform3D inverse() const;

		/** @brief Set to the inverse of the transformation */
		void invert();

		/** @brief Translate the transfomration on the local axis'. */
		void moveBy(const CxVec3 &in_dv);

		/** @brief Translate the transfomration on the local axis'. */
		void moveBy(CxReal in_dx, CxReal in_dy, CxReal in_dz);

		/** @brief Translate the transfomration on the global / parent axis'. */
		void moveByAbs(const CxVec3 &in_dv);

		/** @brief Translate the transfomration on the global / parent  axis'. */
		void moveByAbs(CxReal in_dx, CxReal in_dy, CxReal in_dz);

		/** @brief Set to the normalized transform (unit quaternion) */
		void normalize();

		/** @return A copy of the transform normalized( unit quaternion) */
		CxTransform3D normalized() const;

		/** @brief Apply a rotation around the local x-axis */
		void pitch(CxReal in_radians);

		/** @brief Apply a rotation around the global (or parent) x-axis */
		void pitchAbs(CxReal in_radians);

		/** @brief Apply a rotation around the local z-axis */
		void roll(CxReal in_radians);

		/** @brief Apply a rotation around the global (or parent) z-axis */
		void rollAbs(CxReal in_radians);

		/** 
		 * @brief Rotate relative to local space by a rotation quaternion. 
		 * This method will rotate the transform by a quaternion in local space.
		 * For example, if the rotation is around the y-axis, then this transform will
		 * be rotated around its LOCAL y-axis, not the parent/global y-axis.
		 */
		void rotateBy(const CxQuat &in_q);

		/**
		 * @brief Rotate the transform around an arbitrary axis.
		 * @see rotateBy(const CxQuat &)
		 */
		void rotateBy(const CxVec3 &in_axis, CxReal in_radians);

		/** 
		 * @brief Rotate relative to the parent space by a rotation quaternion 
		 * This method will rotate the transformation by a quaternion in 
		 * the parent (or global if no parent) coordinate space.  For example, 
		 * if the rotation is around the y-axis, then this transform will be rotated
		 * around the parent/global y-axis not the one local to this transformation.
		 **/
		void rotateByAbs(const CxQuat &in_q);

		/**
		 * @brief Rotate the transform around an arbitrary axis.
		 * @see rotateByParent(const CxQuat &)
		 */
		void rotateByAbs(const CxVec3 &in_axis, CxReal in_radians);

		/** 
		 * @brief Rotate a vector relative to the local space to global/parent space.
		 * This method takes a vector relative to the local space into the global
		 * (or parent) space.  e.g., if the local space is rotated 45* around the z-axis,
		 * then rotating the local x-axis (1,0,0) will return that axis relative to 
		 * either the global or parent space.
		 * @return The local vector relative to the parent/global space.
		 */
		CxVec3 rotate(const CxVec3 &in_v) const;

		/** 
		 * @brief transform a vector relative to the parent/global space to local space.
		 * This method takes a vector relative to the global (or parent) space and 
		 * transforms it into the local space.
		 * @return The global/parent space vector relative to this local space.
		 */
		CxVec3 rotateInv(const CxVec3 &in_v) const;

		/** 
		 * @brief transform a point relative to the local space to global/parent space.
		 * This method takes a point relative to the local space and transforms it 
		 * into the global (or parent) space.
		 * @return The local point relative to the parent/global space.
		 */
		CxVec3 transform(const CxVec3 &in_v) const;

		/** 
		 * @brief transform a point relative to the parent/global space to local space.
		 * This method takes a point relative to the global (or parent) space and 
		 * transforms it into the local space.
		 * @return The global/parent space point relative to this local space.
		 */
		CxVec3 transformInv(const CxVec3 &in_v) const;

		/** 
		 * @brief Transforms another coordinate space.
		 * This method transforms another coordinate space that is relative to this one, 
		 * into a coordinate space relative to the parent of this one (or global).
		 * @return The coordinate space relative to the parent/global space.
		 */
		CxTransform3D transform(const CxTransform3D &in_tx) const;

		/** 
		 * @brief Transforms another coordinate space.
		 * This method transforms another coordinate space that is relative to the 
		 * parent of this one (or global if no parent) into being relative to this
		 * local coordinate space.
		 * @return The coordinate space relative to the this coordinate space.
		 */
		CxTransform3D transformInv(const CxTransform3D &in_tx) const;

		/** @brief Apply a rotation around the local y-axis */
		void yaw(CxReal in_radians);

		/** @brief Apply a rotation around the global (or parent) y-axis */
		void yawAbs(CxReal in_radians);

	  private:
		CxU32 m_cached;
	};

	CX_INLINE CxTransform3D::CxTransform3D(const CxMat4 &in_m) : m_cached(0) {
		o = CxQuat(in_m.mat3());
		p.setXYZ(in_m.col3.x, in_m.col3.y, in_m.col3.z);
	}

	CX_FORCE_INLINE CxTransform3D CxTransform3D::operator*(const CxTransform3D &in_tx) const {
		return transform(in_tx);
	}

	CX_FORCE_INLINE CxTransform3D & CxTransform3D::operator*=(const CxTransform3D &in_tx) const {
		*this = transform(in_tx);
		m_cached = 0; return *this;
	}

	CX_INLINE void CxTransform3D::cacheMatrix(CxMat4 &out_m) const {
		out_m.col3.setXYZ(p.x, p.y, p.z);  out_m.setQuat(o);
		m_cached = 1;
	}

	CX_INLINE void CxTransform3D::cacheMatrix(CxMat4 &out_m, const CxMat4 &in_parent) const {
		out_m = in_parent;
		CxMat4 local(o);  local.col3.setXYZW(p.x, p.y, p.z, 1);
		out_m = *= local;
		m_cached = 1;
	}

	CX_FORCE_INLINE CxTransform3D CxTransform3D::inverse() const {
		CXD_IF_ERR(!isValid(), "Getting inverse of invalid CxTransform3D!");
		return CxTransform3D(o.rotateInv(-p), o.conjugate());
	}

	CX_FORCE_INLINE void CxTransform3D::invert() {
		CXD_IF_ERR(!isValid(), "Setting to inverse of invalid CxTransform3D!");
		p = o.rotateInv(-p);  o = o.conjugate();
		m_cached = 0;
	}

	CX_FORCE_INLINE void CxTransform3D::moveBy(const CxVec3 &in_dv) {
		moveBy(in_dv.x, in_dv.y, in_dv.z);
	}
	
	CX_FORCE_INLINE void CxTransform3D::moveBy(CxReal in_dx, CxReal in_dy, CxReal in_dz) {
		p += o.translation(in_dx, in_dy, in_dz);
		m_cached = 0;
	}

	CX_FORCE_INLINE void CxTransform3D::moveByAbs(const CxVec3 &in_dv) {
	   p += in_dv;  m_cached = 0;
	}
	
	CX_FORCE_INLINE void CxTransform3D::moveByAbs(CxReal in_dx, CxReal in_dy, CxReal in_dz) {
	   moveByAbs(CxVec3(in_dx, in_dy, in_dz));
	}
	
	CX_INLINE void CxTransform3D::normalize() {
		o.normalize();  m_cached = 0;
	}

	CX_INLINE CxTransform3D CxTransform3D::normalized() const {
		return o.normalized();
	}

	CX_FORCE_INLINE void CxTransform3D::pitch(CxReal in_radians) {
		const CxReal half_a = in_radians*0.5f;
		const CxReal sin_a = CxSin(half_a);  const CxReal cos_a = CxCos(half_a);
		const CxReal x = o.x;  const CxReal y = o.y;
		const CxReal z = o.z;  const CxReal w = o.w;

		o.setXYZW(w*sin_a + x*cos_a, y*cos_a + z*sin_a,
					 z*cos_a - y*sin_a, w*cos_a - x*sin_a);
		m_cached = 0;
	}

	CX_FORCE_INLINE void CxTransform3D::pitchAbs(CxReal in_radians) {
		const CxReal half_a = in_radians*0.5f;
		const CxReal sin_a = CxSin(half_a);  const CxReal cos_a = CxCos(half_a);
		const CxReal x = o.x;  const CxReal y = o.y;
		const CxReal z = o.z;  const CxReal w = o.w;

		o.setXYZW(cos_a*x + sin_a*w, cos_a*y - sin_a*z,
					 cos_a*z + sin_a*y, cos_a*w - sin_a*x);
		m_cached = 0;
	}

	CX_FORCE_INLINE void CxTransform3D::roll(CxReal in_radians) {
		const CxReal half_a = in_radians*0.5f;
		const CxReal sin_a = CxSin(half_a);  const CxReal cos_a = CxCos(half_a);
		const CxReal x = o.x;  const CxReal y = o.y;
		const CxReal z = o.z;  const CxReal w = o.w;

		o.setXYZW(x*cos_a + y*sin_a, y*cos_a - x*sin_a,
					 w*sin_a + z*cos_a, w*cos_a - z*sin_a);
		m_cached = 0;
	}

	CX_FORCE_INLINE void CxTransform3D::rollAbs(CxReal in_radians) {
		const CxReal half_a = in_radians*0.5f;
		const CxReal sin_a = CxSin(half_a);  const CxReal cos_a = CxCos(half_a);
		const CxReal x = o.x;  const CxReal y = o.y;
		const CxReal z = o.z;  const CxReal w = o.w;

		o.setXYZW(cos_a*x - sin_a*y, cos_a*y + sin_a*x,
					 cos_a*z + sin_a*w, cos_a*w - sin_a*z);
		m_cached = 0;
	}


	CX_FORCE_INLINE void CxTransform3D::rotateBy(const CxQuat &in_q) {
		/* orientation * rot = local */
		o *= in_q;  m_cached = 0;
	}

	CX_FORCE_INLINE void CxTransform3D::rotateBy(const CxVec3 &in_axis, CxReal in_radians) {
		o *= CxQuat(in_axis, in_radians);  m_cached = 0;
	}

	CX_FORCE_INLINE void CxTransform3D::rotateByAbs(const CxQuat &in_q) {
		/* rot * orientation = global/parent */
		o = in_q*o;  m_cached = 0;
	}

	CX_FORCE_INLINE void CxTransform3D::rotateByAbs(const CxVec3 &in_axis, CxReal in_radians) {
		o = CxQuat(in_axis, in_radians)*o;  m_cached = 0;
	}

	CxVec3 CxTransform3D::rotate(const CxVec3 &in_v) const {
		CXD_ASSERT(CxIsFinite(*this));  CXD2_ASSERT(o.isCloseToUnit());
		return o.rotate(in_v);
	}

	CxVec3 CxTransform3D::rotateInv(const CxVec3 &in_v) const {
		CXD_ASSERT(CxIsFinite(*this));  CXD2_ASSERT(o.isCloseToUnit());
		return o.rotateInv(in_v);
	}

	CxVec3 CxTransform3D::transform(const CxVec3 &in_v) const {
		CXD_ASSERT(CxIsFinite(*this));  CXD2_ASSERT(o.isCloseToUnit());
		return o.rotate(in_v) + p;
	}

	CxVec3 CxTransform3D::transformInv(const CxVec3 &in_v) const {
		CXD_ASSERT(CxIsFinite(*this));  CXD2_ASSERT(o.isCloseToUnit());
		return o.rotateInv(in_v - p);
	}

	CxTransform3D CxTransform3D::transform(const CxTransform3D &in_v) const {
		CXD_ASSERT(CxIsFinite(*this));  CXD2_ASSERT(o.isCloseToUnit());
		CXD_ASSERT(CxIsFinite(in_v));  CXD2_ASSERT(in_v.o.isCloseToUnit());
		return CxTransform3D(o*in_v.o, o.rotate(in_v) + p);
	}

	CxTransform3D CxTransform3D::transformInv(const CxTransform3D &in_v) const {
		CXD_ASSERT(CxIsFinite(*this));  CXD2_ASSERT(o.isCloseToUnit());
		CXD_ASSERT(CxIsFinite(in_v));  CXD2_ASSERT(in_v.o.isCloseToUnit());
		const CxQuat o_inv = o.conjugate();
		return CxTransform3D(o_inv*in_v.o, o_inv.rotate(in_v - p));
	}


	CX_FORCE_INLINE void CxTransform3D::yaw(CxReal in_radians) {
		const CxReal half_a = in_radians*0.5f;
		const CxReal sin_a = CxSin(half_a);  const CxReal cos_a = CxCos(half_a);
		const CxReal x = o.x;  const CxReal y = o.y;
		const CxReal z = o.z;  const CxReal w = o.w;

		o.setXYZW(x*cos_a - z*sin_a, w*sin_a + y*cos_a,
					 z*cos_a + x*sin_a, w*cos_a - y*sin_a);
		m_cached = 0;
	}

	CX_FORCE_INLINE void CxTransform3D::yawAbs(CxReal in_radians) {
		const CxReal half_a = in_radians*0.5f;
		const CxReal sin_a = CxSin(half_a);  const CxReal cos_a = CxCos(half_a);
		const CxReal x = o.x;  const CxReal y = o.y;
		const CxReal z = o.z;  const CxReal w = o.w;

		o.setXYZW(cos_a*x + sin_a*z, cos_a*y + sin_a*w,
					 cos_a*z - sin_a*x, cos_a*w - sin_a*y);
		m_cached = 0;
	}


	/** @return Return true if the two transforms (a and b) are equal within a given error. */
	CX_INLINE CxQuat CxEq(const CxTransform3D &in_a, const CxTransform3D &in_b,
										 CxReal in_epsilon = CX_REAL_EPSILON) {
		return CxEq(in_a.o, in_b.o, in_epsilon) && CxEq(in_a.p, in_b.p, in_epsilon);
	}

	/** @return True if the orientation and position are finite (not NaN or INF). */
	CX_FORCE_INLINE CxBool CxIsFinite(const CxTransform3D &in_tx) {
		return isFinite(in_tx.o) && isFinite(in_tx.p);
	}

} // namespace cat
