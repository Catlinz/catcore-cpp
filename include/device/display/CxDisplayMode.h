#ifndef CX_DEVICE_DISPLAY_CXDISPLAYMODE_H
#define CX_DEVICE_DISPLAY_CXDISPLAYMODE_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxDisplayMode.h
 * Simple class to hold information about the display mode for a
 * display device.
 *
 * @author Catlin Zilinski
 * @date July 17, 2015
 */

#include "core/Cx.h"
#include "core/common/CxVector.h"

namespace cat {


	/**
	 * @class CxDisplayMode CxDisplayMode.h "device/display/CxDisplayMode.h"
	 * 
	 * The CxDisplayMode class holds the depth, resolution and refresh rate
	 * information for a possible display mode for a display device. 
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since July 17, 2015
	 */
	class CxDisplayMode {
	  public:
		/** @brief An enum of flags for the display modes. */
		enum DisplayModeFlag {
			kIsCurrentDisplayMode = (1 << 0),
		};
		
		/** @brief Create a null display mode */
		CX_FORCE_INLINE CxDisplayMode()
			: m_depth(0), m_hRes(0), m_vRes(0), m_refresh(0), m_flags(0) {}

		/**
		 * @brief Create a new display mode.
		 * A value of -1 for the hRes, vRes or Depth can be used when
		 * creating a DisplayMode to try and change to to indicate that
		 * those values do not matter.  A value of 0 for the refresh rate
		 * does the same.
		 * @param in_hRes The horizontal resolution in pixels.
		 * @param in_vRes The vertical resolution in pixels.
		 * @param inopt_depth The Color depth in bits (defaults to 32).
		 * @param inopt_refresh The refresh rate in hertz (default = 0).
		 * @param inopt_flags The optional additional flags for the display mode.
		 */
		CX_FORCE_INLINE CxDisplayMode(CxI32 in_hRes, CxI32 in_vRes,
												CxI32 inopt_depth = 32, CxI32 inopt_refresh = 0,
												CxI32 inopt_flags = 0)
			: m_depth(inopt_depth), m_hRes(in_hRes), m_vRes(in_vRes),
			  m_refresh(inopt_refresh), m_flags(inopt_flags) {}

		/** @brief Tests all fields for equality */
		CX_FORCE_INLINE CxBool operator==(const CxDisplayMode &in_src) const {
			return ((m_depth == in_src.m_depth) && (m_hRes == in_src.m_hRes) &&
					  (m_vRes == in_src.m_vRes) && (m_refresh == in_src.m_refresh) &&
					  (m_flags == in_src.m_flags));
		}

		/** @brief Tests all fields for inequality */
		CX_FORCE_INLINE CxBool operator!=(const CxDisplayMode &in_src) const {
			return !(*this == in_src);
		}
		
		/** @return The color depth of the display mode (16, 32, etc.) */
		CX_FORCE_INLINE CxI32 depth() const { return m_depth; }

		/** @return Horizontal resolution of the screen in pixels. */
		CX_FORCE_INLINE CxI32 horizRes() const { return m_hRes; }

		/** @return True if the display mode is the current one for a display. */
		CX_FORCE_INLINE CxBool isCurrentDisplayMode() const {
			return (m_flags & CxDisplayMode::kIsCurrentDisplayMode) != 0;
		}

		/** @return True if the display mode is valid. */
		CX_FORCE_INLINE CxBool isValid() const {
			return (m_depth != 0) && (m_hRes != 0) && (m_vRes != 0);
		}

		/** @return True if all fields (except the flags) are equal. */
		CX_FORCE_INLINE CxBool matchesAll(const CxDisplayMode &in_m) const {
			return ((m_depth == in_m.m_depth) && (m_hRes == in_m.m_hRes) &&
					  (m_vRes == in_m.m_vRes) && (m_refresh == in_m.m_refresh));
		}

		/** @return True if resolution and depth match. */
		CX_FORCE_INLINE CxBool matchesResAndDepth(const CxDisplayMode &in_m) const {
			return ((m_depth == in_m.m_depth) && (m_hRes == in_m.m_hRes) &&
					  (m_vRes == in_m.m_vRes));
		}

		/** @return True if resolution matches. */
		CX_FORCE_INLINE CxBool matchesRes(const CxDisplayMode &in_m) const {
			return ((m_hRes == in_m.m_hRes) && (m_vRes == in_m.m_vRes));
		}
		
		/** @return The refresh rate in Hertz. */
		CX_FORCE_INLINE CxI32 refreshRate() const { return m_refresh; }

		/** @return Vertical resolution of the screen in pixels. */
		CX_FORCE_INLINE CxI32 vertRes() const { return m_vRes; }

		/** @brief Find the display mode that matches a given mode the best.
		 * The criteria in determining the best match is first to find the 
		 * one with the closest color depth match, then to find the one
		 * with the smallest difference between the resolution, then the one
		 * with the largest refresh rate.
		 * @param in_dm The display mode to find the closest match for.
		 * @param in_choices The list of display modes to search.
		 * @return The best match for the given display mode in the list.
		 */
		static CxDisplayMode getBestMatch(const CxDisplayMode &in_dm,
													 const CxVector<CxDisplayMode> &in_choices);

   	/**
		 * @brief Like getBestMatch() but will never choose a larger mode.
		 * This method works exactly like getBestMatch, except that it will never choose a 
		 * display mode such that the depth, vRes or hRes are greater than then that of the requested mode.
		 * @see getBestMatch()
		 * @param in_dm The display mode to find the closest match for.
		 * @param in_choices The list of display modes to search.
		 * @return The best match for the given display mode in the list.
		 */
		static CxDisplayMode getBestMatchLessOrEq(const CxDisplayMode &in_dm,
																const CxVector<CxDisplayMode> &in_choices);
		
	  private:
		CxI32 m_depth;
		CxI32 m_hRes;
		CxI32 m_vRes;
		CxI32 m_refresh;
		CxI32 m_flags;
	};
	
} // namespace cat

#endif // CX_DEVICE_DISPLAY_CXDISPLAYMODE_H
