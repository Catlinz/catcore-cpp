#include "device/display/windows/CxWindowsDisplayDevice.h"
#include "core/common/CxStr.h"

namespace cat {

	CxDisplayDevice CxDisplayDevice::s_primaryDisplay;
	
	CxDisplayDevice::CxDisplayDevice() : mp_id(0), mp_name(0), m_flags(0) {}

	CxDisplayDevice::CxDisplayDevice(const CxChar16 *in_id, const CxChar16 *in_name, CxI32 in_flags)
		: mp_id(0), mp_name(0), m_flags(in_flags) {
		mp_id = str::copy(in_id);
		mp_name = str::copy(in_name);
	}
	
	CxDisplayDevice::CxDisplayDevice(const CxDisplayDevice &in_src)
		: mp_id(0), mp_name(0), m_flags(0) { *this = in_src; }
	
	CxDisplayDevice & CxDisplayDevice::operator=(const CxDisplayDevice &in_src) {
		str::free(mp_id);
		str::free(mp_name);
		mp_id = str::copy(in_src.mp_id);
		mp_name = str::copy(in_src.mp_name);
		m_currentMode = in_src.m_currentMode;
		m_modes = in_src.m_modes;
		m_flags = in_src.m_flags;
		return *this;
	}

	CxDisplayDevice::~CxDisplayDevice() {
		str::free(mp_id);
		str::free(mp_name);
		m_flags = 0;
	}

	const CxVector<CxDisplayMode> & CxDisplayDevice::fetchAvailableDisplayModes() {
		if (mp_id != 0) {
			/* First, fetch the current display mode, so we can tell
			 * which of the available ones is the current one. */
			const CxDisplayMode &cm = currentDisplayMode();
		
			m_modes = CxVector<CxDisplayMode>(16);
			DEVMODE dm;
		   CxBool succeeded = true;
			CxI32 dm_count = 0;

			/* Loop through all the display modes and add them to the list */
			while (succeeded) {
				mem::zero(&dm, sizeof(DEVMODE));
				dm.dmSize = sizeof(DEVMODE);

				succeeded = (EnumDisplaySettings(mp_id, dm_count++, &dm) == TRUE);
				if (succeeded) {
					/* Make sure to flag the current display mode when we 
					 * find it */
					CxI32 flags = 0;
					if (dm.dmPelsWidth == cm.horizRes() &&
						 dm.dmPelsHeight == cm.vertRes() &&
						 dm.dmBitsPerPel == cm.depth() &&
						 dm.dmDisplayFrequency == cm.refreshRate()) {
						flags = CxDisplayMode::kIsCurrentDisplayMode;
					}
					
					m_modes <<  CxDisplayMode(dm.dmPelsWidth, dm.dmPelsHeight,
													  dm.dmBitsPerPel,
													  dm.dmDisplayFrequency, flags);
				}
			}

			/* Make sure not to use more memory than we need */
			if (m_modes.size() > 0) { m_modes.compact(); }
			else { m_modes = CxVector<CxDisplayMode>(); }
		}
		else {
			CXD_ERR("Cannot fetch available display modes for null display!");
		}

		return m_modes;
	}

	const CxDisplayMode & CxDisplayDevice::fetchCurrentDisplayMode() {
		if (mp_id != 0) {
			DEVMODE dm;
			mem::zero(&dm, sizeof(DEVMODE));
			dm.dmSize = sizeof(DEVMODE);

			if (EnumDisplaySettings(mp_id, ENUM_CURRENT_SETTINGS, &dm)) {
				m_currentMode = CxDisplayMode(dm.dmPelsWidth, dm.dmPelsHeight,
														dm.dmBitsPerPel,
														dm.dmDisplayFrequency,
														CxDisplayMode::kIsCurrentDisplayMode);
			}
			else {
				CXD_ERR("Failed to get the current display mode for display %s.",
						  mp_name);
			}
		}
		else {
			CXD_ERR("Cannot fetch current display mode for null display!");
		}

		return m_currentMode;
	}

	CxDisplayMode CxDisplayDevice::getClosestDisplayMode(const CxDisplayMode &in_mode) {
		const CxVector<CxDisplayMode> &modes = getDisplayModes();

		const CxI32 nm_modes = modes.count();
		if (nm_modes != 0) {
			CxI32 res_diff = CX_MAX_I32;
			CxI32 depth_diff = CX_MAX_I32;
			CxI32 refresh = 0;
			CxI32 closest_idx = -1;

			for (CxI32 i = 0; i < nm_modes; ++i) {
				const CxDisplayMode &mode = modes[i];
				
				const CxI32 d_diff = in_mode.depth() - mode.depth();
				if (d_diff >= 0 && d_diff <= depth_diff) {
					/* Depth is a valid choice */
					const CxI32 h_diff = in_mode.horizRes() - mode.horizRes();
					const CxI32 v_diff = in_mode.vertRes() - mode.vertRes();

					if (h_diff >= 0 && v_diff >= 0) {
						/* Resolution is valid choice */

						const CxI32 r_diff = h_diff + v_diff;
						if ((d_diff < depth_diff) ||  /* Closer depth, or */
							 (r_diff < res_diff) ||      /* Closer resolution, or */
							 (mode.refreshRate() > refresh)) { /* Better refresh */
							/* Choose this one for now */
							res_diff = r_diff;
							depth_diff = d_diff;
							refresh = mode.refreshRate();
							closest_idx = i;
						}
					}
				}
			}

			/* If have mode, return it */
			if (closest_idx != -1) {
				return modes[closest_idx];
			}
		}
		
		/* If modes <= available, return invalid display mode */
		return CxDisplayMode();			
	}

	void CxDisplayDevice::getAllDisplayDevices(CxVector<CxDisplayDevice> &inout_devices) {
		/* Make sure the vector is initialised */
		if (inout_devices.capacity() == 0) {
			inout_devices = CxVector<CxDisplayDevice>(4);
		}
		else { inout_devices.reserve(4); }

		CxBool adpt_good = true;
		CxI32 adpt_idx = 0;
		DISPLAY_DEVICE adapter;

		/* Loop over all the display adapters */
		while (adpt_good) {
			mem::zero(&adapter, sizeof(DISPLAY_DEVICE));
			adapter.cb = sizeof(DISPLAY_DEVICE);

			adpt_good = (EnumDisplayDevices(0, adpt_idx++, &adapter, 0) == TRUE);
			if (adpt_good) { /* Have an adapter to get the displays on */
				CxBool dsp_good = true;
				CxI32 dsp_idx = 0;
				DISPLAY_DEVICE disp;

				/* Loop over all the displays on the adapter */
				while (dsp_good) {
					mem::zero(&disp, sizeof(DISPLAY_DEVICE));
					disp.cb = sizeof(DISPLAY_DEVICE);

					dsp_good = (EnumDisplayDevices(adapter.DeviceName, dsp_idx++, &disp, 0) == TRUE);
					if (dsp_good && (disp.StateFlags & DISPLAY_DEVICE_ACTIVE) != 0) {
						/* Have display */
						const CxI32 flags = ((disp.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0) ? CxDisplayDevice::kIsPrimaryDisplay : 0;
						inout_devices << CxDisplayDevice(disp.DeviceName, disp.DeviceString, flags);
					}
				}
			}
		}
	}

	CxDisplayDevice & CxDisplayDevice::primaryDisplay() {
		if (s_primaryDisplay.mp_id == 0) {
			/* Get the primary display */
			CxBool adpt_good = true;
			CxI32 adpt_idx = 0;
			DISPLAY_DEVICE adapter;

			/* Loop over all the display adapters */
			while (adpt_good) {
				mem::zero(&adapter, sizeof(DISPLAY_DEVICE));
				adapter.cb = sizeof(DISPLAY_DEVICE);

				adpt_good = (EnumDisplayDevices(0, adpt_idx++, &adapter, 0) == TRUE);
				if (adpt_good) { /* Have an adapter to get the displays on */
					CxBool dsp_good = true;
					CxI32 dsp_idx = 0;
					DISPLAY_DEVICE disp;

					/* Loop over all the displays on the adapter */
					while (dsp_good) {
						mem::zero(&disp, sizeof(DISPLAY_DEVICE));
						disp.cb = sizeof(DISPLAY_DEVICE);

						dsp_good = (EnumDisplayDevices(adapter.DeviceName, dsp_idx++, &disp, 0) == TRUE);
						if (dsp_good && (disp.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0) {
							s_primaryDisplay =  CxDisplayDevice(disp.DeviceName, disp.DeviceString,
																			CxDisplayDevice::kIsPrimaryDisplay);
						}
					}
				}
			}
		}

		return s_primaryDisplay;
	}
	
} // namespace cat
