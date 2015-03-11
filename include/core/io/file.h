#ifndef CAT_CORE_IO_FILE_H
#define CAT_CORE_IO_FILE_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * file.h: Contains the definition for the File class.
 *
 * Author: Catlin Zilinski
 * Date: Sept 30, 2013
 */

#include "core/corelib.h"

#ifdef OS_WINDOWS
#include "core/io/win32/file.h"
#endif

#if defined(OS_APPLE) || defined(OS_UNIX)
#include "core/io/unix/file.h"
#endif

#endif // CAT_CORE_IO_FILE_H


