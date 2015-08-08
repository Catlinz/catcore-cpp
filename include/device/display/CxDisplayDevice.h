#ifndef CX_DEVICE_DISPLAY_CXDISPLAYDEVICE_H
#define CX_DEVICE_DISPLAY_CXDISPLAYDEVICE_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxDisplayDevice.h
 * Contains the methods and classes for enumerating and manipulating 
 * display devices.
 *
 * @author Catlin Zilinski
 * @date July 17, 2015
 */

#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsDisplayDevice.h"
#elif defined(CX_APPLE)
#  include "apple/CxAppleDisplayDevice.h"
#else
#  include "x11/CxX11DisplayDevice.h"
#endif

#endif // CX_DEVICE_DISPLAY_CXDISPLAYDEVICE_H
