#ifndef CX_CORE_COMMON_CXSYS_H
#define CX_CORE_COMMON_CXSYS_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxSys.h
 * @brief Contains a variety of System functions.
 *
 * @author Catlin Zilinski
 * @date Oct 29, 2015
 */

#include "core/Cx.h"

namespace cat {
	namespace sys {

		/**
		 * @brief Get an absolute path from a file/dir name/path.
		 * This method takes in a file or directory name/path and returns a 
		 * new string (allocated via mem::alloc()) that is a sanitized 
		 * absolute path relative to the sys::cwd() if the path specified 
		 * was not already absolute. 
		 * The resulting path will always be absolute and will strip any
		 * trailing /'s from the path.
		 * @param in_fdPath The file or directory name/path to sanitize.
		 * @return A new absolute path.
		 */
		CxChar * getPath(const CxChar *in_fdPath);

		/**
		 * @brief Return the stored current working directory.
		 * If the cwd has not been set or initialized, it is initialized via 
		 * a system call to get the cwd.
		 * @return The current working directory.
		 */
		const CxChar * cwd();

		/** @return The newline string/char for the platform. */
		const CxChar * newline();

		/** @return The filesystem path separator for the platform. */
		CxChar pathSep();
		
	} // namespace sys
	
} // namespace cat

#endif // CX_CORE_COMMON_CXSYS_H
