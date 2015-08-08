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
		
		/** @return The refresh rate in Hertz. */
		CX_FORCE_INLINE CxI32 refreshRate() const { return m_refresh; }

		/** @return Vertical resolution of the screen in pixels. */
		CX_FORCE_INLINE CxI32 vertRes() const { return m_vRes; }
		
	  private:
		CxI32 m_depth;
		CxI32 m_hRes;
		CxI32 m_vRes;
		CxI32 m_refresh;
		CxI32 m_flags;
	};
	
} // namespace cat

#endif // CX_DEVICE_DISPLAY_CXDISPLAYMODE_H
