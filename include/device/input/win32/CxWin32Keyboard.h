#ifndef CX_DEVICE_INPUT_WIN32_CXWIN32KEYBOARD_H
#define CX_DEVICE_INPUT_WIN32_CXWIN32KEYBOARD_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxWin32Keyboard.h
 * Windows specific keyboard shit.
 *
 * @author Catlin Zilinski
 * @date Jan 2, 2016
 */

#include <Windows.h>

#define CX_X_KEYBOARD_METHODS															\
	CxI8 toCxKey(WPARAM in_wParam, LPARAM in_lParam) const;							\
	CX_FORCE_INLINE CxI16 toNativeKey(CxI8 in_cxKey) {	return m_nativeKeyMap[in_cxKey]; }
	

#define CX_X_KEYBOARD_FIELDS							\
	CxI8 m_keyMap[350];									\
	CxI16 m_nativeKeyMap[CX_KEY_LAST + 1]

#endif // CX_DEVICE_INPUT_WIN32_CXWIN32KEYBOARD_H
