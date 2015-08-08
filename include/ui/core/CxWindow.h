#ifndef CX_UI_CORE_CXWINDOW_H
#define CX_UI_CORE_CXWINDOW_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxWindow.h
 * @brief Contains a cross-platform window object for creating opengl capable windows.
 *
 * @author Catlin Zilinski
 * @date July 17, 2015
 */

#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWin32Window.h"
#elif defined(CX_APPLE)
#  include "apple/CxOSXWindow.h"
#else
#  include "x11/CxX11Window.h"
#endif

#endif // CX_UI_CORE_CXWINDOW_H
