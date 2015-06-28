#include "core/math/CxMat4x4.h"

namespace cat {
	const CxMat4x4 CxMat4x4::kIdentity = CxMat4x4(kCxIdentity);

	CxReal CxMat4x4::inverse() const {
		Real det = determinant();
		if (det == 0) {
			CXD_ERR("Trying to get Inverse of non-invertable matrix!");
			return CxMat4x4(*this);
		}

		CX_TODO("Implement the inverse matrix crap.");
	}
} // namespace cat
