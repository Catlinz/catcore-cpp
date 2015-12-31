#ifndef CX_DEVICE_DISPLAY_WINDOWS_CXWINDOWSDISPLAYDEVICE_H
#define CX_DEVICE_DISPLAY_WINDOWS_CXWINDOWSDISPLAYDEVICE_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxWindowsDisplayDevice.h
 * Contains the Windows specific implementation of the methods for 
 * managing display devices.
 *
 * @author Catlin Zilinski
 * @date July 17, 2015
 */

#include "core/Cx.h"
#include <Windows.h>

#include "device/display/CxDisplayMode.h"
#include "core/common/CxVector.h"

namespace cat {

	/**
	 * @class CxDisplayDevice CxWindowsDisplayDevice.h "device/display/windows/CxWindowsDisplayDevice.h"
	 * 
	 * The CxDisplayDevice class encapsulates the details of a specific display 
	 * device, as well as provides methods for enumerating the attached 
	 * display devices. 
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since July 17, 2015
	 */
	class CxDisplayDevice {
	  public:
		/** @brief An enum of flags for the display devices. */
		enum DeviceFlag {
			kIsPrimaryDisplay = (1 << 0),
		};
		
		/** @brief Create a null display device. */
		CxDisplayDevice();

		/** @brief Copy constructor, copies the name */
		CxDisplayDevice(const CxDisplayDevice &in_src);

		/** @brief Move assignment operator, steals data. */
		CxDisplayDevice(CxDisplayDevice &&in_src);

		/** @brief Overloaded assignment operator, copies the name. */
		CxDisplayDevice & operator=(const CxDisplayDevice &in_src);

		/** @brief Move-assignment operator, steals data */
		CxDisplayDevice & operator=(CxDisplayDevice &&in_src);

		/** @brief Simply tests the id or name for equality */
		CxBool operator==(const CxDisplayDevice &in_src) const;

		/** @brief Simply tests the id or name for equality */
		CxBool operator!=(const CxDisplayDevice &in_src) const;
		
		/** @brief Destructor free's up the memory. */
		~CxDisplayDevice();

		/**
		 * @brief Get the current display mode for the device.
		 * If the current display mode has not been populated yet, that 
		 * call will happen before it is returned.
		 * @return The current display mode for the device.
		 */
		CX_FORCE_INLINE const CxDisplayMode & currentDisplayMode() {
			if (m_currentMode.isValid()) { return m_currentMode; }
			else { return fetchCurrentDisplayMode(); }
		}

		/**
		 * @brief Fetch, store and return all available display modes.
		 * @return A reference to the list of available display modes.
		 */
		const CxVector<CxDisplayMode> & fetchAvailableDisplayModes();

		/**
		 * @brief Fetch, store and return the current display mode for the device.
		 * @return The current display mode for the device.
		 */
		const CxDisplayMode & fetchCurrentDisplayMode();

		/**
		 * @brief Find the display mode that matches a given mode best.
		 * The criteria in determining the best match is first to find the 
		 * one with the closest color depth match, then to find the one
		 * with the smallest total difference between the width and height.
		 * @param in_mode The Display mode to find the closest one to.
		 * @return The closest display mode to the requested one.
		 */
		CxDisplayMode getClosestDisplayMode(const CxDisplayMode &in_mode);

		/**
		 * @brief Like getClosestDisplayMode() but will never choose a larger mode.
		 * This method works exactly like getClosestDisplayMode, except that it will
		 * never choose a display mode such that the depth, vRes or hRes are greater than
		 * then that of the requested mode.
		 * @see getClosestDisplayMode()
		 * @param in_mode The Display mode to find the closest one to.
		 * @return The closest display mode to the requested one.
		 */
		CxDisplayMode getClosestDisplayModeLessOrEq(const CxDisplayMode &in_mode);

		/**
		 * @brief Get a list of all the available display modes.
		 * This call will enumerate through all the display modes 
		 * if it is the first time calling it.
		 * @return A reference to the list of available display modes.
		 */
		CX_FORCE_INLINE const CxVector<CxDisplayMode> & getDisplayModes() {
			if (m_modes.capacity() != 0) { return m_modes; }
			else { return fetchAvailableDisplayModes(); }
		}

		/** @return True if this is the primary display device. */
		CX_FORCE_INLINE CxBool isPrimaryDisplay() const {
			return (m_flags & CxDisplayDevice::kIsPrimaryDisplay) != 0;
		}

		/** @return The name of the device. */
		CX_FORCE_INLINE const CxU8 * name() const { return mp_name; }

		/**
		 * @brief Static method to get a list of all display devices.
		 * @param inout_devices The vector to store the list of displays in.
		 */
		static void getAllDisplayDevices(CxVector<CxDisplayDevice> &inout_devices);
		
		/**
		 * @brief Static method to get the main display.
		 * This method may need to enumerate all displays the first time it
		 * is called, if they have not been enumerated yet.
		 */
		static CxDisplayDevice & primaryDisplay();

	  protected:
		CxDisplayDevice(const wchar_t *in_id, const wchar_t *in_name, CxI32 in_flags = 0);
		
	  private:
		wchar_t *mp_id;
		CxU8 *mp_name; /**< The name of the display device. */
		CxDisplayMode m_currentMode; /**< The current display mode for the device. */
		CxVector<CxDisplayMode> m_modes; /**< The available display modes for the device. */

		CxI32 m_flags;

		static CxDisplayDevice s_primaryDisplay;
	};
} // namespace cat

#endif // CX_DEVICE_DISPLAY_WINDOWS_CXWINDOWSDISPLAYDEVICE_H
