#ifndef CX_DEVICE_INPUT_CXKEYBOARD_H
#define CX_DEVICE_INPUT_CXKEYBOARD_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxKeyboard.h
 * Code for maintaining the current key state for keyboard input.
 *
 * @author Catlin Zilinski
 * @date Jan 1, 2016
 */

#include "core/Cx.h"
#include "device/input/CxInputTypes.h"
#include "device/input/CxKeyMap.h"


#if defined (CX_WINDOWS)
#  include "device/input/win32/CxWin32Keyboard.h"
#elif defined(CX_APPLE)
#  include "device/input/apple/CxAppleKeyboard.h"
# else
#  include "device/input/x11/CxX11Keyboard.h"
#endif

namespace cat {

	/**
	 * @class CxKeyboard CxKeyboard.h "device/input/CxKeyboard.h"
	 * 
	 * The CxKeyboard class maintains the state of the current pressed
	 * keys and has the methods to modify the keyboard state.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Jan 1, 2016
	 */
	class CxKeyboard {
	  public:

		/** @brief Initialise a new keyboard instance. */
		CxKeyboard();

		/** @return The currently active keyboard modifiers (from OS, not stored). */
		CxI8 getKeyModifiers() const;

		/**
		 * @brief Method to record keyboard input event.
		 * The action performed may be changed if it is detected
		 * that it is a repeat event.
		 * @param in_keyCode The key that was changed.
		 * @param inout_action The action performed.
		 * @param in_mods The modifier key state.
		 * @return True if there is anything to handle (i.e., any change).
		 */
		CxBool input(CxI8 in_keyCode, CxI8 &inout_action, CxI8 in_mods);


		CX_X_KEYBOARD_METHODS;
		
	  private:
		CxI8 m_keys[CX_KEY_LAST + 1];
	   CX_X_KEYBOARD_FIELDS;

		/** @brief Do platform specific initialisation for instance */
		void do_X_Init();
		
	};
} // namespace cat

#endif // CX_DEVICE_INPUT_CXKEYBOARD_H
