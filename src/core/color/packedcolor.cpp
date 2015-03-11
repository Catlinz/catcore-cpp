#include "core/color/packedcolor.h"
#include "core/math/mathcore.h"

namespace Cat {

	PackedColorVal PackedColor::fromRGB(Real r, Real g, Real b) {
		return fromRGB((UByte)Math::rounded(r/255.0),
							(UByte)Math::rounded(g/255.0),
							(UByte)Math::rounded(b/255.0));
	}

	PackedColorVal PackedColor::fromRGBA(Real r, Real g, Real b, Real a) {
		return fromRGBA((UByte)Math::rounded(r/255.0),
							 (UByte)Math::rounded(g/255.0),
							 (UByte)Math::rounded(b/255.0),
							 (UByte)Math::rounded(a/255.0));
	}	

} // namespace Cat
