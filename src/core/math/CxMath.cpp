#include "core/math/CxMath.h"

namespace cat {

	CxI32 CxNextPrime(CxI32 in_n) {
		if (in_n <= 2) { return 2; }

	   CxI32 p = (in_n % 2 == 0) ? in_n + 1 : in_n;
		for (; true; p += 2) {
			const CxI32 max = (CxI32)CxSqrt((CxF32)p);
			CxI32 f = 3;

			for (; f <= max; f += 2) {
				if (p % f == 0) { break; }
			}

			if (f > max) { return p; }
		}
	}
} // namespace cat
