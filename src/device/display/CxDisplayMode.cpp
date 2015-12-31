#include "device/display/CxDisplayMode.h"
#include "core/math/CxMath.h"

namespace cat {

	CxDisplayMode CxDisplayMode::getBestMatch(const CxDisplayMode &in_dm,
															const CxVector<CxDisplayMode> &in_choices) {
		const CxI32 nm_choices = in_choices.count();
		if (nm_choices != 0) {
			/* Store the differences between the current closest and the requested mode */
			CxI32 r_d = CX_MAX_I32;  CxI32 d_d = CX_MAX_I32;  CxI32 r_r = 0;
			CxI32 idx = -1;

			for (CxI32 i = 0; i < nm_choices; ++i) {
				const CxDisplayMode &mode = in_choices[i];

				const CxI32 h_di = CxAbs(in_dm.horizRes() - mode.horizRes());
				const CxI32 v_di = CxAbs(in_dm.vertRes() - mode.vertRes());
				const CxI32 d_di = CxAbs(in_dm.depth() - mode.depth());
				const CxI32 r_di = CxMax(h_di, v_di);

				/* See if the mode is better (priority is depth --> res --> refresh) */
				CxBool better = false;
				if (d_di < d_d) { better = true; }
				else if (d_di == d_d) {
					if (r_di < r_d) { better = true; }
					else if ((r_di == r_d) && (mode.refreshRate() > r_r)) { better = true; }
				}
				if (better) { r_d = r_di;  d_d = d_di;  r_r = mode.refreshRate();  idx = i; }
			}
			
			/* If have mode, return it */
			if (idx != -1) { return in_choices[idx]; }
		}
		/* If failed to find a mode <= the specified mode, return invalid mode */
		return CxDisplayMode();			
	}

	CxDisplayMode CxDisplayMode::getBestMatchLessOrEq(const CxDisplayMode &in_dm,
																	  const CxVector<CxDisplayMode> &in_choices) {
		const CxI32 nm_choices = in_choices.count();
		if (nm_choices != 0) {
			/* Store the differences between the current closest and the requested mode */
			CxI32 r_d = CX_MAX_I32;  CxI32 d_d = CX_MAX_I32;  CxI32 r_r = 0;
			CxI32 idx = -1;

			for (CxI32 i = 0; i < nm_choices; ++i) {
				const CxDisplayMode &mode = in_choices[i];

				const CxI32 h_di = in_dm.horizRes() - mode.horizRes();
				const CxI32 v_di = in_dm.vertRes() - mode.vertRes();
				const CxI32 d_di = in_dm.depth() - mode.depth();

				if (h_di >= 0 && v_di >= 0 && d_di >= 0) {
					/* Have valid choices for res and depth */
					const CxI32 r_di = CxMax(h_di, v_di);

					/* See if the mode is better (priority is depth --> res --> refresh) */
					CxBool better = false;
					if (d_di < d_d) { better = true; }
					else if (d_di == d_d) {
						if (r_di < r_d) { better = true; }
						else if ((r_di == r_d) && (mode.refreshRate() > r_r)) { better = true; }
					}
					if (better) { r_d = r_di;  d_d = d_di;  r_r = mode.refreshRate();  idx = i; }
				}
			}
			
			/* If have mode, return it */
			if (idx != -1) { return in_choices[idx]; }
		}
		/* If failed to find a mode <= the specified mode, return invalid mode */
		return CxDisplayMode();			
	}
} // namespace cat
