
namespace cat {

	void CxKeyboard::do_X_Init() {
		mem::set(m_keyMap, -1, sizeof(m_keyMap));
		mem::set(m_nativeKeyMap, -1, sizeof(m_nativeKeyMap));

		/* Set the LUT for windows Virtual Key to CxKey. */
		m_keyMap[0x01E] = CX_KEY_A;
		m_keyMap[0x030] = CX_KEY_B;
		m_keyMap[0x02E] = CX_KEY_C;
		m_keyMap[0x020] = CX_KEY_D;
		m_keyMap[0x012] = CX_KEY_E;
		m_keyMap[0x021] = CX_KEY_F;
		m_keyMap[0x022] = CX_KEY_G;
		m_keyMap[0x023] = CX_KEY_H;
		m_keyMap[0x017] = CX_KEY_I;
		m_keyMap[0x024] = CX_KEY_J;
		m_keyMap[0x025] = CX_KEY_K;
		m_keyMap[0x026] = CX_KEY_L;
		m_keyMap[0x032] = CX_KEY_M;
		m_keyMap[0x031] = CX_KEY_N;
		m_keyMap[0x018] = CX_KEY_O;
		m_keyMap[0x019] = CX_KEY_P;
		m_keyMap[0x010] = CX_KEY_Q;
		m_keyMap[0x013] = CX_KEY_R;
		m_keyMap[0x01F] = CX_KEY_S;
		m_keyMap[0x014] = CX_KEY_T;
		m_keyMap[0x016] = CX_KEY_U;
		m_keyMap[0x02F] = CX_KEY_V;
		m_keyMap[0x011] = CX_KEY_W;
		m_keyMap[0x02D] = CX_KEY_X;
		m_keyMap[0x015] = CX_KEY_Y;
		m_keyMap[0x02C] = CX_KEY_Z;

		m_keyMap[0x002] = CX_KEY_1;
		m_keyMap[0x003] = CX_KEY_2;
		m_keyMap[0x004] = CX_KEY_3;
		m_keyMap[0x005] = CX_KEY_4;
		m_keyMap[0x006] = CX_KEY_5;
		m_keyMap[0x007] = CX_KEY_6;
		m_keyMap[0x008] = CX_KEY_7;
		m_keyMap[0x009] = CX_KEY_8;
		m_keyMap[0x00A] = CX_KEY_9;
		m_keyMap[0x00B] = CX_KEY_0;

		m_keyMap[0x01C] = CX_KEY_ENTER;
		m_keyMap[0x001] = CX_KEY_ESC;
		m_keyMap[0x00E] = CX_KEY_BACKSPACE;
		m_keyMap[0x00F] = CX_KEY_TAB;
		m_keyMap[0x039] = CX_KEY_SPACE;

		m_keyMap[0x00C] = CX_KEY_MINUS;
		m_keyMap[0x00D] = CX_KEY_EQUAL;
		m_keyMap[0x01A] = CX_KEY_LEFT_BRACKET;
		m_keyMap[0x01B] = CX_KEY_RIGHT_BRACKET;
		m_keyMap[0x02B] = CX_KEY_BACKSLASH;

		m_keyMap[0x027] = CX_KEY_SEMICOLON;
		m_keyMap[0x028] = CX_KEY_APOSTROPHE;
		m_keyMap[0x029] = CX_KEY_GRAVE_ACCENT;
		m_keyMap[0x033] = CX_KEY_COMMA;
		m_keyMap[0x034] = CX_KEY_PERIOD;
		m_keyMap[0x035] = CX_KEY_SLASH;

		m_keyMap[0x03A] = CX_KEY_CAPSLOCK;

		m_keyMap[0x03B] = CX_KEY_F1;
		m_keyMap[0x03C] = CX_KEY_F2;
		m_keyMap[0x03D] = CX_KEY_F3;
		m_keyMap[0x03E] = CX_KEY_F4;
		m_keyMap[0x03F] = CX_KEY_F5;
		m_keyMap[0x040] = CX_KEY_F6;
		m_keyMap[0x041] = CX_KEY_F7;
		m_keyMap[0x042] = CX_KEY_F8;
		m_keyMap[0x043] = CX_KEY_F9;
		m_keyMap[0x044] = CX_KEY_F10;
		m_keyMap[0x057] = CX_KEY_F11;
		m_keyMap[0x058] = CX_KEY_F12;


		m_keyMap[0x137] = CX_KEY_PRINT_SCREEN;
		m_keyMap[0x046] = CX_KEY_SCROLL_LOCK;
		m_keyMap[0x045] = CX_KEY_PAUSE;

		m_keyMap[0x152] = CX_KEY_INSERT;
		m_keyMap[0x147] = CX_KEY_HOME;
		m_keyMap[0x149] = CX_KEY_PAGE_UP;
		m_keyMap[0x153] = CX_KEY_DEL;
		m_keyMap[0x14F] = CX_KEY_END;
		m_keyMap[0x151] = CX_KEY_PAGE_DOWN;

		m_keyMap[0x14D] = CX_KEY_RIGHT;
		m_keyMap[0x14B] = CX_KEY_LEFT;
		m_keyMap[0x150] = CX_KEY_DOWN;
		m_keyMap[0x148] = CX_KEY_UP;

		m_keyMap[0x145] = CX_KEY_NUM_LOCK;
		m_keyMap[0x135] = CX_KEY_KP_DIVIDE;
		m_keyMap[0x037] = CX_KEY_KP_MULTIPLY;
		m_keyMap[0x04A] = CX_KEY_KP_MINUS;
		m_keyMap[0x04E] = CX_KEY_KP_PLUS;
		m_keyMap[0x11C] = CX_KEY_KP_ENTER;
		m_keyMap[0x04F] = CX_KEY_KP_1;
		m_keyMap[0x050] = CX_KEY_KP_2;
		m_keyMap[0x051] = CX_KEY_KP_3;
		m_keyMap[0x04B] = CX_KEY_KP_4;
		m_keyMap[0x04C] = CX_KEY_KP_5;
		m_keyMap[0x04D] = CX_KEY_KP_6;
		m_keyMap[0x047] = CX_KEY_KP_7;
		m_keyMap[0x048] = CX_KEY_KP_8;
		m_keyMap[0x049] = CX_KEY_KP_9;
		m_keyMap[0x052] = CX_KEY_KP_0;
		m_keyMap[0x053] = CX_KEY_KP_PERIOD;

		m_keyMap[0x056] = CX_KEY_WORLD_2;
		m_keyMap[0x15D] = CX_KEY_MENU;

		m_keyMap[0x064] = CX_KEY_F13;
		m_keyMap[0x065] = CX_KEY_F14;
		m_keyMap[0x066] = CX_KEY_F15;
		m_keyMap[0x067] = CX_KEY_F16;
		m_keyMap[0x068] = CX_KEY_F17;
		m_keyMap[0x069] = CX_KEY_F18;
		m_keyMap[0x06A] = CX_KEY_F19;
		m_keyMap[0x06B] = CX_KEY_F20;
		m_keyMap[0x06C] = CX_KEY_F21;
		m_keyMap[0x06D] = CX_KEY_F22;
		m_keyMap[0x06E] = CX_KEY_F23;
		m_keyMap[0x076] = CX_KEY_F24;

		m_keyMap[0x01D] = CX_KEY_LEFT_CTRL;
		m_keyMap[0x02A] = CX_KEY_LEFT_SHIFT;
		m_keyMap[0x038] = CX_KEY_LEFT_ALT;
		m_keyMap[0x15B] = CX_KEY_LEFT_SUPER;

		m_keyMap[0x11D] = CX_KEY_RIGHT_CTRL;
		m_keyMap[0x036] = CX_KEY_RIGHT_SHIFT;
		m_keyMap[0x138] = CX_KEY_RIGHT_ALT;
		m_keyMap[0x15C] = CX_KEY_RIGHT_SUPER;

		/* Now make sure to set up the reverse LUT. */
		for (CxI32 code = 0; code < 350; ++code) {
			if (m_keyMap[code] != -1) {
				m_nativeKeyMap[m_keyMap[code]] = code;
			}
		}
	}

	CxI8 CxKeyboard::getKeyModifiers() const {
		CxI8 modifiers = 0;
		const SHORT ctrl = GetKeyState(VK_CONTROL);
		const SHORT shift = GetKeyState(VK_SHIFT);
		const SHORT alt = GetKeyState(VK_MENU);
		const SHORT super = GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN);

		/* If high order bit is 1, then is down.  If low order bit is 1, then toggled. */
		if (ctrl & (1 << 31)) { modifiers |= CX_MOD_CTRL; }
		if (shift & (1 << 31)) { modifiers |= CX_MOD_SHIFT; }
	   if (alt & (1 << 31)) { modifiers |= CX_MOD_ALT; }
		if (super & (1 << 31)) { modifiers |= CX_MOD_SUPER; }

		return modifiers;
	}

	CxI8 CxKeyboard::toCxKey(WPARAM in_wParam, LPARAM in_lParam) const {
		/* The WPARAM is the virtual key code and the LPARAM contains
		 * a number of different things, most importantly the scancode and
		 * a bit that identifies extended keys. */

		/* The CTRL key needs a bunch of crap because on german keyboards, then 
		 * "ALT Gr" key is sent as "Left CTRL then Right ALT", so gives a false
		 * positive for the left CTRL. */
		if (in_wParam == VK_CONTROL) {
			/* If extended bit set, is right CTRL. */
			if (in_lParam & 0x01000000) { return CX_KEY_RIGHT_CTRL; }

			/* See if is a false positive */
			MSG next;
			DWORD time = GetMessageTime();
			
			if (PeekMessageW(&next, 0, 0, 0, PM_NOREMOVE)) {
				if ((next.message == WM_KEYDOWN || next.message == KWM_SYSKEYDOWN ||
					  next.message == WM_KEYUP || next.message == WM_SYSKEYUP) &&
					 (next.wParam == VK_MENU && (next.lParam & 0x01000000) &&
					  next.time == time)) {
					/* If next message is right ALT and sent at same time as the left CTRL, 
					 * then is false positive. */
					return CX_KEY_UNKNOWN;
				}
			}
			
			return CX_KEY_LEFT_CTRL;
		}

		/* Otherwise, just return the mapped key */
		return m_keyMap[HIWORD(in_lParam) & 0x1FF];
	}

} // namespace cat
