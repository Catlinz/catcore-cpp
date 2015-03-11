#include "core/math/mat3.h"

namespace Cat {
	const Real Mat3::kEpsilon = REAL(0.00001);	
	const Mat3 Mat3::kIdentity = Mat3(REAL(1.0), REAL(0.0), REAL(0.0), 
												 REAL(0.0), REAL(1.0), REAL(0.0),
												 REAL(0.0), REAL(0.0), REAL(1.0));
	
	Mat3 Mat3::inverse() const {
		Real detM = determinant();
		if (detM == 0) {
			DWARN("Trying to get Inverse of non-invertable 3x3 matrix!");
			return Mat3(*this);
		}

		return Mat3( (m_d.m22*m_d.m33 - m_d.m32*m_d.m23) / detM,
						 (m_d.m32*m_d.m13 - m_d.m12*m_d.m33) / detM,
						 (m_d.m12*m_d.m23 - m_d.m22*m_d.m13) / detM,

						 (m_d.m31*m_d.m23 - m_d.m21*m_d.m33) / detM,
						 (m_d.m11*m_d.m33 - m_d.m31*m_d.m13) / detM,
						 (m_d.m21*m_d.m13 - m_d.m11*m_d.m23) / detM,

						 (m_d.m21*m_d.m32 - m_d.m31*m_d.m22) / detM,
						 (m_d.m31*m_d.m12 - m_d.m11*m_d.m32) / detM,
						 (m_d.m11*m_d.m22 - m_d.m21*m_d.m12) / detM );		

	}

#ifdef DEBUG
	std::ostream& operator<<(std::ostream& out, const Mat3& m) {
		const Mat3::MatrixData& matrix = m.getMatrixDataConst();
		return out << "| " << matrix.m11 << " " << matrix.m21 << " " << matrix.m31 <<  " |\n" \
					  << "| " << matrix.m12 << " " << matrix.m22 << " " << matrix.m32 <<  " |\n" \
					  << "| " << matrix.m13 << " " << matrix.m23 << " " << matrix.m33 <<  " |";
	}
#endif //DEBUG
	
	
} // namespace Cat




