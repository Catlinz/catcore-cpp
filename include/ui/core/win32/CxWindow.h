#ifndef CX_UI_CORE_WIN32_CXWIN32WINDOW_H
#define CX_UI_CORE_WIN32_CXWIN32WINDOW_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxWin32Window.h
 * @brief Contains the Windows specific definition of the CxWindow class.
 *
 * @author Catlin Zilinski
 * @date July 17, 2015
 */

#include "core/Cx.h"
#include "core/string/CxNameGenerator.h"
#include <Windows.h>

namespace cat {

	/**
	 * @class CxWindow CxWin32Window.h "ui/core/win32/CxWin32Window.h"
	 * The CxWindow class encapsulates the creation and control of
	 * an OpenGL capable window for rendering things in.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since July 17, 2015
	 */
	class CxWindow {
	  public:
		/** @brief An enum of possible window creation flags. */
		enum CxWindowFlag {
			kFullscreen = 1 << 0,
		};
		
		/**
		 * @brief Create a new window.
		 * @param in_title The title to display for the new window.
		 * @param in_width The initial width of the window in pixels.
		 * @param in_height The initial height of the window in pixels.
		 * @param in_flags Additional flags for the window creation.
		 */
		CxWindow(const Char *in_title, CxI32 in_width, CxI32 in_height,
					CxI32 in_flags = 0);

		/** @brief Cleanup all the resources and destroy the window. */
		~CxWindow();
		
		
	  private:
		HWND mp_winHandle;
		HDC mp_devHandle;

		Char *mp_winClass;
		
		WNDCLASSEX m_winClass;

		static CxNameGenerator s_classNameGen;
	};
	
} // namespace cat

#endif // CX_UI_CORE_WIN32_CXWIN32WINDOW_H
