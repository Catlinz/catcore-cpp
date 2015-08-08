#include "ui/core/win32/CxWin32Window.h"

namespace cat {

	CxNameGenerator CxWindow::s_classNameGen("CxGLWindow");

	CxWindow::CxWindow(const Char *in_title, CxI32 in_width, CxI32 in_height,
							 CxI32 in_flags)
		: mp_winHandle(0), mp_devHandle(0), mp_winClass(0) {
		
		/* First, create and register the window class for the window */
		/* Generate the class name */
		mp_winClass = s_classNameGen.generate();

		/* Get the the handle to the process that we need */
		HINSTANCE h_instance = GetModuleHandle(0);

		WNDCLASSEX ex;
		ex.cbSize = sizeof(WNDCLASSEX); /* Size of the struct */
		ex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		ex.lpfnWndProc = WinProc;
		ex.cbClsExtra = ex.cbWndExtra = 0;
		ex.hInstance = h_instance;
		ex.hIcon = LoadIcon(0, IDI_APPLICATION);
		ex.hCursor = LoadCursor(0, IDC_ARROW);
		ex.hbrBackground = 0;  /* Don't draw the background, we do that */
		ex.lpszMenuName = 0;
		ex.lpszClassName = mp_winClass;
		ex.hIconSm = 0;

		RegisterClassEx(&ex);

		/* Calculate where we want the window to appear (centered). */
		CxI32 pos_x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (in_width / 2);
		CxI32 pos_y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (in_height / 2);

		/* Assume we want a non-fullscreen window by default */
		CxI32 wnd_style = WS_OVERLAPPEDWINDOW;
		CxI32 mode = WINDOWED;

		/* Check to see if we want a fullscreen window */
		if ((in_flags & CxWindow::kFullscreen) != 0) {
			wnd_style = WS_POPUP;
			mode = FULLSCREEN;
			pos_x = pos_y = 0;

			/* Get the resolution of the main display */
			const CxI32 screen_w = GetSystemMetrics(SM_CXFULLSCREEN);
			const CxI32 screen_h = GetSystemMetrics(SM_CYFULLSCREEN);
			
			/* If the width and height are zero, set to the current resolution. */
			if (in_width == 0 || in_height == 0) {
				in_width = screen_w;
				in_height = screen_h;
			}
			else {
				/* Otherwise, if width and height are not equal to 
				 * current resolution, then change the resolution. */
				SysSetDisplayMode(screen_w, screen_h, 32);
			}
		}
		
	}
} // namespace cat
