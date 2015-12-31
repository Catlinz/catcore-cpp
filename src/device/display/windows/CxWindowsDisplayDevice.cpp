#include "device/display/windows/CxWindowsDisplayDevice.h"
#include "core/common/CxUtf8.h"
#include <wchar.h>

namespace cat {

	CxDisplayDevice CxDisplayDevice::s_primaryDisplay;
	
	CxDisplayDevice::CxDisplayDevice() : mp_id(0), mp_name(0), m_flags(0) {}

	CxDisplayDevice::CxDisplayDevice(const wchar_t *in_id, const wchar_t *in_name, CxI32 in_flags)
		: mp_id(0), mp_name(0), m_flags(in_flags) {
		CxU32 id_len = (in_id != 0) ? (wcslen(in_id) + 1) : 0;
		mp_id = (wchar_t *)mem::copy(in_id, sizeof(wchar_t*)*id_len);
		
		mp_name = utf8::fromWideChars(in_name);
	}
	
	CxDisplayDevice::CxDisplayDevice(const CxDisplayDevice &in_src)
		: mp_id(0), mp_name(0), m_flags(0) { *this = in_src; }

	CxDisplayDevice::CxDisplayDevice(CxDisplayDevice &&in_src)
		: mp_id(in_src.mp_id), mp_name(in_src.mp_name), m_currentMode(in_src.m_currentMode),
		  m_flags(in_src.m_flags) {
		if (in_src.m_modes.count() > 0) {
			m_modes = static_cast< CxVector<CxDisplayMode> && >(in_src.m_modes);
		}
		in_src.mp_id = 0;  in_src.mp_name = 0;
		in_src.m_flags = 0;
	}
	
	CxDisplayDevice & CxDisplayDevice::operator=(const CxDisplayDevice &in_src) {
		mem::free(mp_id);
		utf8::free(mp_name);
		
		CxI32 id_len = (in_src.mp_id != 0) ? wcslen(in_src.mp_id) + 1 : 0;
		mp_id = (wchar_t *)mem::copy(in_src.mp_id, sizeof(wchar_t*)*id_len);

		mp_name = utf8::copy(in_src.mp_name);
		m_currentMode = in_src.m_currentMode;
		m_modes = in_src.m_modes;
		m_flags = in_src.m_flags;
		return *this;
	}

	CxDisplayDevice & CxDisplayDevice::operator=(CxDisplayDevice &&in_src) {
		mem::free(mp_id);
		utf8::free(mp_name);

		mp_id = in_src.mp_id;
		mp_name = in_src.mp_name;
		m_currentMode = in_src.m_currentMode;
		if (in_src.m_modes.count() > 0) {
			m_modes = static_cast< CxVector<CxDisplayMode> && >(in_src.m_modes);
		}
		m_flags = in_src.m_flags;
		in_src.mp_id = 0;  in_src.mp_name = 0;
		in_src.m_flags = 0;
		return *this;
	}

	CxBool CxDisplayDevice::operator==(const CxDisplayDevice &in_src) const {
		if (mp_id == 0 || in_src.mp_id == 0) { return mp_id == in_src.mp_id; }
		else { return wcscmp(mp_id, in_src.mp_id) == 0; }
	}

	
	CxBool CxDisplayDevice::operator!=(const CxDisplayDevice &in_src) const {
		if (mp_id == 0 || in_src.mp_id == 0) { return mp_id != in_src.mp_id; }
		else { return wcscmp(mp_id, in_src.mp_id) != 0; }
	}

	CxDisplayDevice::~CxDisplayDevice() {
		mem::free(mp_id);
		utf8::free(mp_name);
		m_flags = 0;
	}

	const CxVector<CxDisplayMode> & CxDisplayDevice::fetchAvailableDisplayModes() {
		if (mp_id != 0) {
			/* First, fetch the current display mode, so we can tell
			 * which of the available ones is the current one. */
			const CxDisplayMode &cm = currentDisplayMode();
		
			m_modes = CxVector<CxDisplayMode>(16);
			DEVMODEW dm;
		   CxBool succeeded = true;
			CxI32 dm_count = 0;

			/* Loop through all the display modes and add them to the list */
			while (succeeded) {
				mem::zero(&dm, sizeof(DEVMODEW));
				dm.dmSize = sizeof(DEVMODEW);

				succeeded = (EnumDisplaySettingsW(mp_id, dm_count++, &dm) == TRUE);
				if (succeeded) {
					/* Test to make sure is valid display mode */
					if (dm.dmPelsWidth > 0 && dm.dmPelsHeight > 0) {
						/* Make sure to flag the current display mode when we find it */
						CxI32 flags = 0;
						if (dm.dmPelsWidth == cm.horizRes() && dm.dmPelsHeight == cm.vertRes() &&
							 dm.dmBitsPerPel == cm.depth() && dm.dmDisplayFrequency == cm.refreshRate()) {
							flags = CxDisplayMode::kIsCurrentDisplayMode;
						}
						m_modes <<  CxDisplayMode(dm.dmPelsWidth, dm.dmPelsHeight, dm.dmBitsPerPel,
														  dm.dmDisplayFrequency, flags);
					}
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
			DEVMODEW dm;
			mem::zero(&dm, sizeof(DEVMODEW));
			dm.dmSize = sizeof(DEVMODEW);

			if (EnumDisplaySettingsW(mp_id, ENUM_CURRENT_SETTINGS, &dm)) {
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
		return CxDisplayMode::getBestMatch(in_mode, getDisplayModes());
	}

	CxDisplayMode CxDisplayDevice::getClosestDisplayModeLessOrEq(const CxDisplayMode &in_mode) {
		return CxDisplayMode::getBestMatchLessOrEq(in_mode, getDisplayModes());
	}

	void CxDisplayDevice::getAllDisplayDevices(CxVector<CxDisplayDevice> &inout_devices) {
		/* Make sure the vector is initialised */
		if (inout_devices.capacity() == 0) {
			inout_devices = CxVector<CxDisplayDevice>(4);
		}
		else { inout_devices.reserve(4); }

		CxBool adpt_good = true;
		CxI32 adpt_idx = 0;
		DISPLAY_DEVICEW adapter;

		/* Loop over all the display adapters */
		while (adpt_good) {
			mem::zero(&adapter, sizeof(DISPLAY_DEVICEW));
			adapter.cb = sizeof(DISPLAY_DEVICEW);

			adpt_good = (EnumDisplayDevicesW(0, adpt_idx++, &adapter, 0) == TRUE);
			if ((adapter.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) != 0) {
				const CxI32 flags = ((adapter.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0)
							? CxDisplayDevice::kIsPrimaryDisplay : 0;
				inout_devices << CxDisplayDevice(adapter.DeviceName, adapter.DeviceString, flags);
			}
		}
	}

	CxDisplayDevice & CxDisplayDevice::primaryDisplay() {
		if (s_primaryDisplay.mp_id == 0) {
			/* Get the primary display */
			CxBool adpt_good = true;
			CxI32 adpt_idx = 0;
			DISPLAY_DEVICEW adapter;

			/* Loop over all the display adapters */
			while (adpt_good) {
				mem::zero(&adapter, sizeof(DISPLAY_DEVICEW));
				adapter.cb = sizeof(DISPLAY_DEVICEW);

				adpt_good = (EnumDisplayDevicesW(0, adpt_idx++, &adapter, 0) == TRUE);
				if ((adapter.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0) {
					s_primaryDisplay = CxDisplayDevice(adapter.DeviceName, adapter.DeviceString,
																  CxDisplayDevice::kIsPrimaryDisplay);
				}
			}
		}

		return s_primaryDisplay;
	}
	
} // namespace cat
