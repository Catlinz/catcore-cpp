#ifndef CX_CORE_MATH_CXMAT3_H
#define CX_CORE_MATH_CXMAT3_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxMat3.h
 * @brief Contains the definition of the 3x3 matrix class.
 *
 * @author Catlin Zilinski
 * @date Oct 5, 2015
 */

#include "core/Cx.h"
#include "core/math/CxMath.h"
#include "core/math/CxVec3.h"
#include "core/math/CxQuat.h"

namespace cat {

	/**
	 * @class CxMat3 CxMat3.h "core/math/CxMat3.h"
	 * @brief A 3x3 matrix class and the operators for it.
	 * 
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Dec 4, 2013
	 */
	class CxMat3 {
	  public:
		CxVec3 col0, col1, col2;

		/** @brief Initialise the matrix to garbage */
		CX_FORCE_INLINE CxMat3() {}

		/** @brief Initialise the matrix to all zeros */
		CX_INLINE CxMat3(CxZero)
			: col0(kCxZero), col1(kCxZero), col2(kCxZero) {}

		/** @brief Initialise to identity matrix. */
		CX_INLINE CxMat3(CxIdentity)
			: col0(1,0,0), col1(0,1,0), col2(0,0,1) {}

		/** 
		 * @brief Initialise the matrix from an array 
		 * The array is laid out in column major format, so the first three
		 * values are the x,y,z of the first column of the matrix, etc.
		 */
		explicit CX_INLINE CxMat3(const CxReal *in_src)
			: col0(in_src[0], in_src[1], in_src[2]),
			  col1(in_src[3], in_src[4], in_src[5]),
			  col2(in_src[6], in_src[7], in_src[8]) {}

		/** @brief Initialise from a unit (rotation) quaternion */
		explicit CxMat3(const CxQuat &in_q);
		
		/** @brief Initialise from three column (basis) vectors. */
		CX_INLINE CxMat3(const CxVec3 &in_col0, const CxVec3& in_col1,
							  const CxVec3 &in_col2)
			: col0(in_col0), col1(in_col1), col2(in_col2) {}

		/** 
		 * @brief Initialise all the components of the matrix. 
		 * Specified in column major format so m00, m01, m02 is the 
		 * first column of the matrix and so on.
		 */
		CX_INLINE CxMat3(CxReal in_m00, CxReal in_m01, CxReal in_m02,
							  CxReal in_m10, CxReal in_m11, CxReal in_m12,
							  CxReal in_m20, CxReal in_m21, CxReal in_m22)
			: col0(in_m00, in_m01, in_m02), col1(in_m10, in_m11, in_m12),
			  col2(in_m20, in_m21, in_m22) {}

		/** @brief Standard copy constructor */
		CX_FORCE_INLINE CxMat3(const CxMat3 &in_src)
			: col0(in_src.col0), col1(in_src.col1), col2(in_src.col2) {}

		/** @brief Standard assignment operator */
		CX_FORCE_INLINE CxMat3 & operator=(const CxMat3 &in_m) {
			col0 = in_m.col0;  col1 = in_m.col1;  col2 = in_m.col2;
			return *this;
		}

		/** @return True if the two matrices are strictly equal */
		CX_INLINE CxBool operator==(const CxMat3 &in_m) const {
			return (col0 == in_m.col0 && col1 == in_m.col1 && col2 == in_m.col2);
		}

		/** @return True if the two matrices are strictly NOT equal */
		CX_INLINE CxBool operator!=(const CxMat3 &in_m) const {
			return (col0 != in_m.col0 || col1 != in_m.col1 || col2 != in_m.col2);
		}

		/** 
		 * @brief Array access to the elements of the matrix. 
		 * The matrix is accessed in column major format so the first three entries are 
		 * the first column of the matrix, etc.
		 */
		CX_FORCE_INLINE CxReal & operator[](CxI32 in_idx) { return (&(col0.x))[in_idx]; }
		CX_FORCE_INLINE CxReal operator[](CxI32 in_idx) const { return (&(col0.x))[in_idx]; }

		/** @brief Access an element of the matrix by its column and row. */
		CX_FORCE_INLINE CxReal & operator()(CxI32 in_col, CxI32 in_row) {
			return (&(col0.x))[in_col*3 + in_row];
		}
		CX_FORCE_INLINE const CxReal operator()(CxI32 in_col, CxI32 in_row) const {
			return (&(col0.x))[in_col*3 + in_row];
		}

		/** @return A copy of the matrix will all entries negated. */
		CX_INLINE CxMat3 operator-() const { return CxMat3(-col0, -col1, -col2); }

		/** @return The result of the matrix addition this + m */
		CX_INLINE CxMat3 operator+(const CxMat3 &in_m) const {
			return CxMat3(col0 + in_m.col0, col1 + in_m.col1, col2 + in_m.col2);
		}

		/** @return The result of the matrix subtraction this - m */
		CX_INLINE CxMat3 operator-(const CxMat3 &in_m) const {
			return CxMat3(col0 - in_m.col0, col1 - in_m.col1, col2 - in_m.col2);
		}

		/** @return The result of multiplying this matrix by a scalar */
		CX_INLINE CxMat3 operator*(CxReal in_s) const {
			return CxMat3(col0*in_s, col1*in_s, col2*in_s);
		}

		/** @return The result r of the vector v transformed by this matrix (r = M*v) */
		CxVec3 operator*(const CxVec3 &in_v) const;

		/** @return The result of the matrix multiplication (this * m) */
		CxMat3 operator*(const CxMat3 &in_m) const;

		/** @return The result of dividing this matrix by a scalar */
		CX_INLINE CxMat3 operator/(CxReal in_s) const {
			const CxReal r_s = 1.0f/in_s;
			return CxMat3(col0*r_s, col1*r_s, col2*r_s);
		}

		/** @brief Add another matrix to this matrix */
		CX_INLINE CxMat3 & operator+=(const CxMat3 &in_m) {
			col0 += in_m.col0;  col1 += in_m.col1;  col2 += in_m.col2;
			return *this;
		}

		/** @brief Subtract another matrix from this matrix */
		CX_INLINE CxMat3 & operator-=(const CxMat3 &in_m) {
			col0 -= in_m.col0;  col1 -= in_m.col1;  col2 -= in_m.col2;
			return *this;
		}

		/** @brief Multiply this matrix by a scalar */
		CX_INLINE CxMat3 & operator*=(CxReal in_s) {
			col0 *= in_s;  col1 *= in_s;  col2 *= in_s;
			return *this;
		}

		/** @brief Multiply this matrix by another matrix */
		CxMat3 & operator*=(const CxMat3 &in_m);

		/** @brief Divide this matrix by a scalar */
		CX_INLINE CxMat3 & operator/=(CxReal in_s) {
			const CxReal r_s = 1.0f/in_s;
			col0 *= r_s;  col1 *= r_s;  col2 *= r_s;
			return *this;
		}
		
		/** @return The matrix as a column major array stored as col0,col1,col2. */
		CX_FORCE_INLINE const CxReal * constData() const { return &(col0.x); }
		CX_FORCE_INLINE CxReal * data() { return &(col0.x); }
		CX_FORCE_INLINE const CxReal * data() const { return &(col0.x); }

		/** @return The determinant for an invertable matrix */
		CxReal determinant() const;

		/** @return The diagonal of this matrix (all off-diagonal entries = 0). */
		CxMat3 diagonal() const;

		/** @return The inverse of the matrix */
		CxMat3 inverse() const;

		/** @brief Invert this matrix in place. */
		void invert();

		/** @brief Set the matrix to a rotation matrix from a unit quaternion. */
		void setQuat(const CxQuat &in_q);

		/** @return A vector r transformed by the matrix (r = Mv). */
		CxVec3 transform(const CxVec3 &in_v) const;

		/** @return A vector r transformed by the inverse matrix (r = M^-1v). */
		CxVec3 transformInv(const CxVec3 &in_v) const;
		
		/** @return A vector r transformed by the transposed matrix (r = M^Tv). */
		CxVec3 transformTranspose(const CxVec3 &in_v) const;

		/** @brief Transpose this matrix in place */
		void transpose();
		
		/** @return A copy of the matrix, transposed */
		CxMat3 transposed() const;

		/** @return The transformed x-axis basis vector */
		CX_FORCE_INLINE const CxVec3 & xAxis() const { return col0; }

		/** @return The transformed y-axis basis vector */
		CX_FORCE_INLINE const CxVec3 & yAxis() const { return col1; }

		/** @return The transformed z-axis basis vector */
		CX_FORCE_INLINE const CxVec3 & zAxis() const { return col2; }
		

	};

	CX_FORCE_INLINE CxMat3::CxMat3(const CxQuat &in_q) { setQuat(in_q); }

	CX_FORCE_INLINE CxVec3 CxMat3::operator*(const CxVec3 &in_v) const {
	   return transform(in_v);
	}

	CX_FORCE_INLINE CxMat3 CxMat3::operator*(const CxMat3 &in_m) const {
		const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y; const CxReal m02 = in_m.col0.z;
		const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y; const CxReal m12 = in_m.col1.z;
		const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y; const CxReal m22 = in_m.col2.z;

		const CxReal c0x = col0.x;  const CxReal c1x = col1.x;  const CxReal c2x = col2.x;
		const CxReal c0y = col0.y;  const CxReal c1y = col1.y;  const CxReal c2y = col2.y;
		
	   const CxReal n00 = (c0x*m00) + (c1x*m01) + (c2x*m02);
		const CxReal n10 = (c0x*m10) + (c1x*m11) + (c2x*m12);
		const CxReal n20 = (c0x*m20) + (c1x*m21) + (c2x*m22);

		const CxReal c0z = col0.z;  const CxReal c1z = col1.z;  const CxReal c2z = col2.z;

	   const CxReal n01 = (c0y*m00) + (c1y*m01) + (c2y*m02);
	   const CxReal n11 = (c0y*m10) + (c1y*m11) + (c2y*m12);
	   const CxReal n21 = (c0y*m20) + (c1y*m21) + (c2y*m22);

		const CxReal n02 = (c0z*m00) + (c1z*m01) + (c2z*m02);
		const CxReal n12 = (c0z*m10) + (c1z*m11) + (c2z*m12);
		const CxReal n22 = (c0z*m20) + (c1z*m21) + (c2z*m22);

		return CxMat3(n00, n01, n02, n10, n11, n12, n20, n21, n22);
	}

	CX_FORCE_INLINE CxMat3 & CxMat3::operator*=(const CxMat3 &in_m) {
		const CxReal m00 = in_m.col0.x;  const CxReal m01 = in_m.col0.y; const CxReal m02 = in_m.col0.z;
		const CxReal m10 = in_m.col1.x;  const CxReal m11 = in_m.col1.y; const CxReal m12 = in_m.col1.z;
		const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y; const CxReal m22 = in_m.col2.z;

		const CxReal c0x = col0.x;  const CxReal c1x = col1.x;  const CxReal c2x = col2.x;
		const CxReal c0y = col0.y;  const CxReal c1y = col1.y;  const CxReal c2y = col2.y;
		
	   col0.x = (c0x*m00) + (c1x*m01) + (c2x*m02);
	   col1.x = (c0x*m10) + (c1x*m11) + (c2x*m12);
		col2.x = (c0x*m20) + (c1x*m21) + (c2x*m22);

		const CxReal c0z = col0.z;  const CxReal c1z = col1.z;  const CxReal c2z = col2.z;

	   col0.y = (c0y*m00) + (c1y*m01) + (c2y*m02);
	   col1.y = (c0y*m10) + (c1y*m11) + (c2y*m12);
	   col2.y = (c0y*m20) + (c1y*m21) + (c2y*m22);

		col0.z = (c0z*m00) + (c1z*m01) + (c2z*m02);
		col1.z = (c0z*m10) + (c1z*m11) + (c2z*m12);
		col2.z = (c0z*m20) + (c1z*m21) + (c2z*m22);

		return *this;
	}

	CX_INLINE CxReal CxMat3::determinant() const {
		const CxReal m00 = col0.x;  const CxReal m01 = col0.y; const CxReal m02 = col0.z;
		const CxReal m10 = col1.x;  const CxReal m11 = col1.y; const CxReal m12 = col1.z;
		const CxReal m20 = col2.x;  const CxReal m21 = col2.y; const CxReal m22 = col2.z;

		return (m00*(m11*m22 - m21*m12) -
				  m01*(m12*m20 - m10*m22) -
				  m02*(m10*m21 - m11*m20));
	}

	CX_FORCE_INLINE CxMat3 CxMat3::diagonal() const {
		return CxMat3(col0.x, 0, 0, 0, col1.y, 0, 0, 0, col2.z);
	}

	CX_INLINE CxMat3 CxMat3::inverse() const {
		const CxReal det_m = determinant();
		if (det_m != 0) {
			const CxReal det_m_recip = 1.0f/det_m;
			const CxReal m10 = col1.x;  const CxReal m11 = col1.y; const CxReal m12 = col1.z;
			const CxReal m20 = col2.x;  const CxReal m21 = col2.y; const CxReal m22 = col2.z;
			const CxReal m00 = col0.x;  const CxReal m01 = col0.y; const CxReal m02 = col0.z;
			
			return CxMat3(
				(m11*m22 - m21*m12)*det_m_recip,
				(m21*m02 - m01*m22)*det_m_recip,
				(m01*m12 - m11*m02)*det_m_recip,

				(m12*m20 - m10*m22)*det_m_recip,
				(m00*m22 - m02*m20)*det_m_recip,
				(m02*m10 - m00*m12)*det_m_recip,
							  
				(m10*m21 - m11*m20)*det_m_recip,
				(m01*m20 - m00*m21)*det_m_recip,
				(m00*m11 - m10*m01)*det_m_recip
				);
		}
		else {
			CXD_WARN("Trying to get Inverse of non-invertable CxMat3");
			return CxMat3(kCxIdentity);
		}
	}

	CX_INLINE void CxMat3::invert() {
		const CxReal det_m = determinant();
		if (det_m != 0) {
			const CxReal det_m_recip = 1.0f/det_m;
			const CxReal m10 = col1.x;  const CxReal m11 = col1.y; const CxReal m12 = col1.z;
			const CxReal m20 = col2.x;  const CxReal m21 = col2.y; const CxReal m22 = col2.z;
			const CxReal m00 = col0.x;  const CxReal m01 = col0.y; const CxReal m02 = col0.z;
			
			col0.x = (m11*m22 - m21*m12)*det_m_recip;
			col0.y = (m21*m02 - m01*m22)*det_m_recip;
			col0.z = (m01*m12 - m11*m02)*det_m_recip;

			col1.x = (m12*m20 - m10*m22)*det_m_recip;
			col1.y = (m00*m22 - m02*m20)*det_m_recip;
			col1.z = (m02*m10 - m00*m12)*det_m_recip;
							  
			col2.x = (m10*m21 - m11*m20)*det_m_recip;
			col2.y = (m01*m20 - m00*m21)*det_m_recip;
			col2.z = (m00*m11 - m10*m01)*det_m_recip;
		}
#if defined(CX_DEBUG)
		else {
			CXD_WARN("Trying to Invert non-invertable CxMat3");
		}
#endif // CX_DEBUG
	}

	CX_FORCE_INLINE void CxMat3::setQuat(const CxQuat &in_q) {
		const CxReal x = in_q.x;  const CxReal y = in_q.y;
		const CxReal z = in_q.z;  const CxReal w = in_q.w;

		const CxReal x2 = x + x;  const CxReal y2 = y + y;  const CxReal z2 = z+z;
		const CxReal xx2 = x*x2;  const CxReal yy2 = y*y2;  const CxReal zz2 = z*z2;
		const CxReal xy2 = x2*y;  const CxReal xz2 = x2*z;  const CxReal xw2 = x2*w;
		const CxReal yz2 = y2*z;  const CxReal yw2 = y2*w;  const CxReal zw2 = z2*w;

		col0.x = 1.0f - yy2 - zz2;  col0.y = xy2 + zw2;         col0.z = xz2 - yw2;
		col1.x = xy2 - zw2;         col1.y = 1.0f - xx2 - zz2;  col1.z = yz2 + xw2;
		col2.x = xz2 + yw2;         col2.y = yz2 - xw2;         col2.z = 1.0f - xx2 - yy2;
	}

		CX_FORCE_INLINE CxVec3 CxMat3::transform(const CxVec3 &in_v) const {
		const CxReal x = in_v.x;  const CxReal y = in_v.y;  const CxReal z = in_v.z;
		return CxVec3(col0.x*x + col1.x*y + col2.x*z,
						  col0.y*x + col1.y*y + col2.y*z,
						  col0.z*x + col1.z*y + col2.z*z);
	}

	CX_FORCE_INLINE CxVec3 CxMat3::transformInv(const CxVec3 &in_v) const {
		const CxReal det_m = determinant();
		if (det_m != 0) {
			const CxReal dr = 1.0f/det_m;
			const CxReal m10 = col1.x;    const CxReal m11 = col1.y;    const CxReal m12 = col1.z;
			const CxReal m20 = col2.x;    const CxReal m21 = col2.y;    const CxReal m22 = col2.z;
			const CxReal dx = in_v.x*dr;  const CxReal dy = in_v.y*dr;  const CxReal dz = in_v.z*dr;
			const CxReal m00 = col0.x;    const CxReal m01 = col0.y;    const CxReal m02 = col0.z;

			return CxVec3(dx*(m11*m22 - m12*m21) + dy*(m12*m20 - m10*m22) + dz*(m10*m21 - m11*m20),
							  dx*(m02*m21 - m01*m22) + dy*(m00*m22 - m02*m20) + dz*(m00*m21 - m01*m20),
							  dx*(m01*m12 - m02*m11) + dy*(m02*m10 - m00*m12) + dz*(m00*m11 - m01*m10));
		}
		else {
			CXD_WARN("Trying to transform by inverse of non-invertable CxMat3");
			return in_v;
		}
	}

	CX_FORCE_INLINE CxVec3 CxMat3::transformTranspose(const CxVec3 &in_v) const {
		const CxReal x = in_v.x;  const CxReal y = in_v.y;  const CxReal z = in_v.z;
		/* (dot(col0, v), dot(col1, v), dot(col2, v)) */
		return CxVec3(col0.x*x + col0.y*y + col0.z*z,
						  col1.x*x + col1.y*y + col1.z*z,
						  col2.x*x + col2.y*y + col2.z*z);
	}

	CX_INLINE void CxMat3::transpose() {
		const CxReal m01 = col0.y; const CxReal m02 = col0.z;
		const CxReal m10 = col1.x; const CxReal m12 = col1.z;
		const CxReal m20 = col2.x;  const CxReal m21 = col2.y;

		col0.y = col1.x;  col0.z = col2.x;
		col1.x = col0.y;  col1.z = col2.y;
		col2.x = col0.z;  col2.y = col1.z;
	}

	CX_FORCE_INLINE CxMat3 CxMat3::transposed() const {
		return CxMat3(col0.x, col1.x, col2.x,
						  col0.y, col1.y, col2.y,
						  col0.z, col1.z, col2.z);
	}

	/* From CxQuat.h */
	CX_INLINE CxQuat::CxQuat(const CxMat3 &in_m) {
		const CxReal m00 = in_m.col0.x;
		const CxReal m11 = in_m.col1.y;
		const CxReal m22 = in_m.col2.z;
		 
		const CxReal trace = m00 + m11 + m22;
		const CxReal m01 = in_m.col0.y;  const CxReal m02 = in_m.col0.z;
		const CxReal m10 = in_m.col1.x;  const CxReal m12 = in_m.col1.z;
		const CxReal m20 = in_m.col2.x;  const CxReal m21 = in_m.col2.y;
		
		if (trace > 0) {
			const CxReal s = CxSqrt(trace + 1.0f); /* s=2w */
			const CxReal s_recip = 0.5f/s;
			w = s*0.5f;
			x = (m12 - m21) * s_recip;
			y = (m20 - m02) * s_recip;
			z = (m01 - m10) * s_recip;
		}
		else if (m00 > m11 && m00 > m22) {
			const CxReal s = CxSqrt(m00 - m11 - m22 + 1.0f); /* s=2x */
			const CxReal s_recip = 0.5f/s;
			x = s*0.5f;
			y = (m10 + m01) * s_recip;
			z = (m02 + m20) * s_recip;
			w = (m12 - m21) * s_recip;
		}
		else if (m11 > m22) {
			const CxReal s = CxSqrt(m11 - m00 - m22 + 1.0f); /* s=2y */
			const CxReal s_recip = 0.5f/s;
			y = s*0.5f;
			x = (m10 + m01) * s_recip;
			z = (m21 + m12) * s_recip;
			w = (m20 - m02) * s_recip;
		}
		else {
			const CxReal s = CxSqrt(m22 - m00 - m11 + 1.0f); /* s=2z */
			const CxReal s_recip = 0.5f/s;
			z = s*0.5f;
			x = (m02 + m20) * s_recip;
			y = (m21 + m12) * s_recip;
			w = (m01 - m10) * s_recip;
		}
	}
	
} // namespace cat

#endif // CX_CORE_MATH_CXMAT3_H
