#ifndef CX_CORE_IO_FILE_CXFILE_H
#define CX_CORE_IO_FILE_CXFILE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxFile.h
 *
 * Defines a file class.
 *
 * @author Catlin Zilinski
 * @date Sept. 30, 2015
 */

#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsFile.h"
#else // POSIX
#  include "posix/CxPOSIXFile.h"
#endif

#endif // CX_CORE_IO_FILE_CXFILE_H
