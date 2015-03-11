#include "core/math/mat4.h"

namespace Cat {

	const Real Mat4::kEpsilon = REAL(0.00001);	
	const Mat4 Mat4::kIdentity = Mat4(REAL(1.0), REAL(0.0), REAL(0.0), REAL(0.0),
												 REAL(0.0), REAL(1.0), REAL(0.0), REAL(0.0),
												 REAL(0.0), REAL(0.0), REAL(1.0), REAL(0.0),
												 REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));	

	Real Mat4::determinant() const {
		return
		 	  (m_d.m11*m_d.m22*m_d.m33*m_d.m44) + (m_d.m11*m_d.m23*m_d.m34*m_d.m42) + (m_d.m11*m_d.m24*m_d.m32*m_d.m43)
			+ (m_d.m12*m_d.m21*m_d.m34*m_d.m43) + (m_d.m12*m_d.m23*m_d.m31*m_d.m44) + (m_d.m12*m_d.m24*m_d.m33*m_d.m41)
			+ (m_d.m13*m_d.m21*m_d.m32*m_d.m44) + (m_d.m13*m_d.m22*m_d.m34*m_d.m41) + (m_d.m13*m_d.m24*m_d.m31*m_d.m42)
			+ (m_d.m14*m_d.m21*m_d.m33*m_d.m42) + (m_d.m14*m_d.m22*m_d.m31*m_d.m43) + (m_d.m14*m_d.m23*m_d.m32*m_d.m41)
			- (m_d.m11*m_d.m22*m_d.m34*m_d.m43) - (m_d.m11*m_d.m23*m_d.m32*m_d.m44) - (m_d.m11*m_d.m24*m_d.m33*m_d.m42)
			- (m_d.m12*m_d.m21*m_d.m33*m_d.m44) - (m_d.m12*m_d.m23*m_d.m34*m_d.m41) - (m_d.m12*m_d.m24*m_d.m31*m_d.m43)
			- (m_d.m13*m_d.m21*m_d.m34*m_d.m42) - (m_d.m13*m_d.m22*m_d.m31*m_d.m44) - (m_d.m13*m_d.m24*m_d.m32*m_d.m41)
			- (m_d.m14*m_d.m21*m_d.m32*m_d.m43) - (m_d.m14*m_d.m22*m_d.m33*m_d.m41) - (m_d.m14*m_d.m23*m_d.m31*m_d.m42);
	}
	
	Mat4 Mat4::inverse() const {
		Real detM = determinant();
		if (detM == 0) {
			DWARN("Trying to get Inverse of non-invertable matrix!");
			return Mat4(*this);
		}

		Mat4 r;
		
		r.m_d.m[0] = 	
			(m_d.m22*m_d.m33*m_d.m44) + (m_d.m23*m_d.m34*m_d.m42) + (m_d.m24*m_d.m32*m_d.m43) - 
			(m_d.m22*m_d.m34*m_d.m43) - (m_d.m23*m_d.m32*m_d.m44) - (m_d.m24*m_d.m33*m_d.m42);
		
		r.m_d.m[1] =
			(m_d.m12*m_d.m34*m_d.m43) + (m_d.m13*m_d.m32*m_d.m44) + (m_d.m14*m_d.m33*m_d.m42) - 
			(m_d.m12*m_d.m33*m_d.m44) - (m_d.m13*m_d.m34*m_d.m42) - (m_d.m14*m_d.m32*m_d.m43);

		r.m_d.m[2] =
			(m_d.m12*m_d.m23*m_d.m44) + (m_d.m13*m_d.m24*m_d.m42) + (m_d.m14*m_d.m22*m_d.m43) - 
			(m_d.m12*m_d.m24*m_d.m43) - (m_d.m13*m_d.m22*m_d.m44) - (m_d.m14*m_d.m23*m_d.m42);

		r.m_d.m[3] =	
			(m_d.m12*m_d.m24*m_d.m33) + (m_d.m13*m_d.m22*m_d.m34) + (m_d.m14*m_d.m23*m_d.m32) - 
			(m_d.m12*m_d.m23*m_d.m34) - (m_d.m13*m_d.m24*m_d.m32) - (m_d.m14*m_d.m22*m_d.m33);

		r.m_d.m[4] =
			(m_d.m21*m_d.m34*m_d.m43) + (m_d.m23*m_d.m31*m_d.m44) + (m_d.m24*m_d.m33*m_d.m41) - 
			(m_d.m21*m_d.m33*m_d.m44) - (m_d.m23*m_d.m34*m_d.m41) - (m_d.m24*m_d.m31*m_d.m43);

		r.m_d.m[5] =	
			(m_d.m11*m_d.m33*m_d.m44) + (m_d.m13*m_d.m34*m_d.m41) + (m_d.m14*m_d.m31*m_d.m43) - 
			(m_d.m11*m_d.m34*m_d.m43) - (m_d.m13*m_d.m31*m_d.m44) - (m_d.m14*m_d.m33*m_d.m41);

		r.m_d.m[6] =	
			(m_d.m11*m_d.m24*m_d.m43) + (m_d.m13*m_d.m21*m_d.m44) + (m_d.m14*m_d.m23*m_d.m41) - 
			(m_d.m11*m_d.m23*m_d.m44) - (m_d.m13*m_d.m24*m_d.m41) - (m_d.m14*m_d.m21*m_d.m43);

		r.m_d.m[7] =
			(m_d.m11*m_d.m23*m_d.m34) + (m_d.m13*m_d.m24*m_d.m31) + (m_d.m14*m_d.m21*m_d.m33) - 
			(m_d.m11*m_d.m24*m_d.m33) - (m_d.m13*m_d.m21*m_d.m34) - (m_d.m14*m_d.m23*m_d.m31);

		r.m_d.m[8] =
			(m_d.m21*m_d.m32*m_d.m44) + (m_d.m22*m_d.m34*m_d.m41) + (m_d.m24*m_d.m31*m_d.m42) - 
			(m_d.m21*m_d.m34*m_d.m42) - (m_d.m22*m_d.m31*m_d.m44) - (m_d.m24*m_d.m32*m_d.m41);

		r.m_d.m[9] =
			(m_d.m11*m_d.m34*m_d.m42) + (m_d.m12*m_d.m31*m_d.m44) + (m_d.m14*m_d.m32*m_d.m41) - 
			(m_d.m11*m_d.m32*m_d.m44) - (m_d.m12*m_d.m34*m_d.m41) - (m_d.m14*m_d.m31*m_d.m42);

		r.m_d.m[10] =
			(m_d.m11*m_d.m22*m_d.m44) + (m_d.m12*m_d.m24*m_d.m41) + (m_d.m14*m_d.m21*m_d.m42) - 
			(m_d.m11*m_d.m24*m_d.m42) - (m_d.m12*m_d.m21*m_d.m44) - (m_d.m14*m_d.m22*m_d.m41);

		r.m_d.m[11] =
			(m_d.m11*m_d.m24*m_d.m32) + (m_d.m12*m_d.m21*m_d.m34) + (m_d.m14*m_d.m22*m_d.m31) - 
			(m_d.m11*m_d.m22*m_d.m34) - (m_d.m12*m_d.m24*m_d.m31) - (m_d.m14*m_d.m21*m_d.m32);

		r.m_d.m[12] =	
			(m_d.m21*m_d.m33*m_d.m42) + (m_d.m22*m_d.m31*m_d.m43) + (m_d.m23*m_d.m32*m_d.m41) - 
			(m_d.m21*m_d.m32*m_d.m43) - (m_d.m22*m_d.m33*m_d.m41) - (m_d.m23*m_d.m31*m_d.m42);

		r.m_d.m[13] =
			(m_d.m11*m_d.m32*m_d.m43) + (m_d.m12*m_d.m33*m_d.m41) + (m_d.m13*m_d.m31*m_d.m42) - 
			(m_d.m11*m_d.m33*m_d.m42) - (m_d.m12*m_d.m31*m_d.m43) - (m_d.m13*m_d.m32*m_d.m41);

		r.m_d.m[14] =
			(m_d.m11*m_d.m23*m_d.m42) + (m_d.m12*m_d.m21*m_d.m43) + (m_d.m13*m_d.m22*m_d.m41) - 
			(m_d.m11*m_d.m22*m_d.m43) - (m_d.m12*m_d.m23*m_d.m41) - (m_d.m13*m_d.m21*m_d.m42);

		r.m_d.m[15] =
			(m_d.m11*m_d.m22*m_d.m33) + (m_d.m12*m_d.m23*m_d.m31) + (m_d.m13*m_d.m21*m_d.m32) - 
			(m_d.m11*m_d.m23*m_d.m32) - (m_d.m12*m_d.m21*m_d.m33) - (m_d.m13*m_d.m22*m_d.m31);
		
		r /= detM;
		return r;		

	}

	Vec3 Mat4::rotateInverse(const Vec3& v) const {

		Real detM =
			m_d.m11*(m_d.m22*m_d.m33 - m_d.m32*m_d.m23) -
			m_d.m21*(m_d.m12*m_d.m33 - m_d.m32*m_d.m13) +
			m_d.m31*(m_d.m12*m_d.m23 - m_d.m22*m_d.m13);

	   if (detM == 0) {
			DWARN("Trying to rotate by inverse of non-invertable 3x3 matrix!");
			return v;
		}

		return Vec3(
			((m_d.m22*m_d.m33 - m_d.m32*m_d.m23)*v.x +
			 (m_d.m31*m_d.m23 - m_d.m21*m_d.m33)*v.y +
			 (m_d.m21*m_d.m32 - m_d.m31*m_d.m22)*v.z) / detM,
				
			((m_d.m32*m_d.m13 - m_d.m12*m_d.m33)*v.x +
			 (m_d.m11*m_d.m33 - m_d.m31*m_d.m13)*v.y +
			 (m_d.m31*m_d.m12 - m_d.m11*m_d.m32)*v.z) / detM,
				
			((m_d.m12*m_d.m23 - m_d.m22*m_d.m13)*v.x +
			 (m_d.m21*m_d.m13 - m_d.m11*m_d.m23)*v.y +
			 (m_d.m11*m_d.m22 - m_d.m21*m_d.m12)*v.z) / detM);
	}

	void Mat4::setToOrthoProjection(Real p_r, Real p_l, Real p_t, Real p_b,
											  Real p_n, Real p_f) {
		m_d.m11 = 2.0f/(p_r - p_l);
		m_d.m22 = 2.0f/(p_t - p_b);
		m_d.m33 = -2.0f/(p_f - p_n);
		m_d.m41 = -( (p_r + p_l) / (p_r - p_l) );
		m_d.m42 = -( (p_t + p_b) / (p_t - p_b) );
		m_d.m43 = -( (p_f + p_n) / (p_f - p_n) );
	}

	void Mat4::setToOrthoProjection(Real p_w, Real p_h, Real p_n, Real p_f) {
		Real w = p_w*REAL(0.5);
		Real h = p_h*REAL(0.5);
		setToOrthoProjection(w, -w, h, -h, p_n, p_f);
	}

	void Mat4::setToPerspectiveProjection(Real p_r, Real p_l, Real p_t, Real p_b,
													  Real p_n, Real p_f) {
		m_d.m11 = (2.0f*p_n) / (p_r - p_l);
		m_d.m22 = (2.0f*p_n) / (p_t - p_b);
		m_d.m31 = ( (p_r + p_l) / (p_r - p_l) );
		m_d.m32 = ( (p_t + p_b) / (p_t - p_b) );
		m_d.m33 = -( (p_f + p_n) / (p_f - p_n) );
		m_d.m34 = -1.0f;
		m_d.m43 = -( (2.0f*p_f*p_n) / (p_f - p_n) );
	}

	void Mat4::setToPerspectiveProjection(Real p_fov, Real p_aspect, Real p_n,
													  Real p_f) {
	   Real top = tan(CC_DEGTORAD*p_fov*0.5f) * p_n;
		Real right = p_aspect * top;
		setToPerspectiveProjection(right, -right, top, -top, p_n, p_f);
	}
	

#ifdef DEBUG
	std::ostream& operator<<(std::ostream& out, const Mat4& m) {
		const Mat4::MatrixData& matrix = m.getMatrixDataConst();
		return out << "| " << matrix.m11 << " " << matrix.m21 << " " << matrix.m31 << " " << matrix.m41 <<  " |\n" \
					  << "| " << matrix.m12 << " " << matrix.m22 << " " << matrix.m32 << " " << matrix.m42 <<  " |\n" \
					  << "| " << matrix.m13 << " " << matrix.m23 << " " << matrix.m33 << " " << matrix.m43 <<  " |\n" \
					  << "| " << matrix.m14 << " " << matrix.m24 << " " << matrix.m34 << " " << matrix.m44 <<  " |";
	}
#endif //DEBUG
	
	
} // namespace Cat




