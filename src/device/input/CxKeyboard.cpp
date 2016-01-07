#include "device/input/CxKeyboard.h"
#include "core/common/CxMem.h"

namespace cat {

	CxKeyboard::CxKeyboard() {
		mem::set(m_keys, 0, sizeof(CxI8)*(CX_KEY_LAST + 1));
		do_X_Init();
	}

	CxBool CxKeyboard::input(CxI8 in_keyCode, CxI8 &inout_action, CxI8 in_modes) {
		if (in_keyCode >= 0 && in_keyCode <= CX_KEY_LAST) {
			const CxI8 state = m_keys[in_keyCode];

			/* Check to see if anything has actually changed. */
			if (inout_action == CxInput::kRelease && state == CxInput::kRelease) {
			   return false;
			}

			m_keys[in_keyCode] = inout_action;

			/* Check to see if we should send the event as a repeat press. */
			if (inout_action == CxInput::kPress && state == CxInput::kPress) {
				inout_action = CxInput::kRepeat;
			}
			
			return true;
		}
	}

} // namespace cat

#if defined (CX_WINDOWS)
#  include "win32/CxWin32Keyboard.cpp"
#elif defined(CX_APPLE)
#  include "apple/CxAppleKeyboard.cpp"
# else
#  include "x11/CxX11Keyboard.cpp"
#endif
