#ifndef CAT_CORE_SYS_SYSTEM_H
#define CAT_CORE_SYS_SYSTEM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file system.h
 * @brief A static class to wrap certain system functionality.
 *
 * @author Catlin Zilinski
 * @date Apr 1, 2014
 */

#include "core/corelib.h"

#if defined (OS_APPLE)
#include "core/sys/osx/system.h"
#endif

#if defined (OS_WINDOWS)
#include "core/sys/win32/system.h"
#endif

#if defined (OS_UNIX)
#include "core/sys/unix/system.h"
#endif

#endif // CAT_CORE_SYS_SYSTEM_H
