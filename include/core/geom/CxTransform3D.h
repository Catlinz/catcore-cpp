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
	 * Stores orientation and position using a quaternion and 
	 * vector.
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
		CX_FORCE_INLINE CxTransform3D() {}

		/** @brief Initialise to identity transformation */
		CX_FORCE_INLINE CxTransform3D(CxIdentity)
			: o(CxIdentity), p(CxZero) {}

		/** @brief Initialise with the specified orientation */
		explicit CX_FORCE_INLINE CxTransform3D(const CxQuat &in_q)
			: o(in_q), p(CxZero) {
			CXD_IF_ERR(!(CxIsFinite(in_q) || in_q.isCloseToUnit()),
						  "Initialising CxTransform3D with invalid CxQuat!");
		}

		/** @brief Initialise with the specified position */
		explicit CX_FORCE_INLINE CxTransform3D(const CxVec3 &in_v)
			: o(CxIdentity), p(in_v) {}

		/** @brief Initialise with the specified orientation and pos. */
		CX_FORCE_INLINE CxTransform3D(const CxQuat &in_q, const CxVec3 &in_v)
			: o(in_q), p(in_v) {
			CXD_IF_ERR(!(CxIsFinite(in_q) || in_q.isCloseToUnit()),
						  "Initialising CxTransform3D with invalid CxQuat!");
		}

		/** @brief Initialise the transformation from a CxMat4 */
		explicit CxTransform3D(const CxMat4 &in_m);

		/** @brief Copy constructor */
		CX_FORCE_INLINE CxTransform3D(const CxTransform3D &in_tx)
			: o(in_tx.o), p(in_tx.p) {}

		/** @brief Assignment operator */
		CX_FORCE_INLINE CxTransform3D & operator=(const CxTransform3D &in_tx) {
			o = in_tx.o;  p = in_tx.p;  return *this;
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

		/** @return True if the transformation is finite w/ close to unit quat */
		CX_FORCE_INLINE CxBool isCloseToValid() const {
			return o.isFinite() && o.isCloseToUnit() && p.isFinite();
		}
		
		/** @return True if the transformation is finite w/ unit quat */
		CX_FORCE_INLINE CxBool isValid() const {
			return o.isFinite() && o.isUnit() && p.isFinite();
		}

		/** @return The inverse transformation */
		CxTransform3D inverse() const;

		/** @brief Set to the inverse of the transformation */
		void invert();

		/** @brief Set to the normalized transform (unit quaternion) */
		void normalize();

		/** @return A copy of the transform normalized( unit quaternion) */
		CxTransform3D normalized() const;

		/** @return A vector transformed by this transformation */
		CxVec3 transform(const CxVec3 &in_v) const;

		/** @return A vector transformed by the inverse of the transformation */
		CxVec3 transformInv(const CxVec3 &in_v) const;

		/** @return The result of a parent transformation transformed by this. */
		CxTransform3D transform(const CxTransform3D &in_v) const;

		/** @return The result of a parent transformation transformed by the inverse of this. */
		CxTransform3D transformInv(const CxTransform3D &in_v) const;
	};

	CX_INLINE CxTransform3D::CxTransform3D(const CxMat4 &in_m) {
		IMPLEMENT_THIS;
	}

	CX_FORCE_INLINE CxTransform3D CxTransform3D::operator*(const CxTransform3D &in_tx) const {
		return transform(in_tx);
	}

	CX_FORCE_INLINE CxTransform3D & CxTransform3D::operator*=(const CxTransform3D &in_tx) const {
		*this = transform(in_tx);  return *this;
	}

	CX_FORCE_INLINE CxTransform3D CxTransform3D::inverse() const {
		CXD_IF_ERR(!isValid(), "Getting inverse of invalid CxTransform3D!");
		return CxTransform3D(o.rotateInv(-p), o.conjugate());
	}

	CX_FORCE_INLINE void CxTransform3D::invert() {
		CXD_IF_ERR(!isValid(), "Setting to inverse of invalid CxTransform3D!");
		p = o.rotateInv(-p);  o = o.conjugate();
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
