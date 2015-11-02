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
		 * @brief Return the stored current working directory.
		 * If the cwd has not been set or initialized, it is initialized via 
		 * a system call to get the cwd.
		 * @return The current working directory.
		 */
		const CxChar * cwd();
		
		/**
		 * @brief Get an absolute path from a file/dir name/path.
		 * This method takes in a file or directory name/path and returns a 
		 * new string (allocated via mem::alloc()) that is a sanitized 
		 * absolute path relative to the sys::cwd() if the path specified 
		 * was not already absolute. 
		 * The resulting path will always be absolute, trailing /'s are not
		 * removed unless there are multiple ones.
		 * @param in_fdPath The file or directory name/path to sanitize.
		 * @param out_len Optional pointer to store length of path in.
		 * @return A new absolute path.
		 */
		CxChar * getPath(const CxChar *in_fdPath, CxI32 *out_len = 0);

		/** @return The newline string/char for the platform. */
		const CxChar * newline();

		/** @return The filesystem path separator for the platform. */
		CX_FORCE_INLINE CxChar pathSep() {
#if defined (CX_WINDOWS)
			return '\\';
#else // POSIX
			return '/';
#endif // Windows or POSIX
		}

		/** @return The current root directory of the cwd. */
		const CxChar * rootDir();

		/** 
		 * @brief Set the current working directory. 
		 * The path must be an absolute path.  If there is no trailing slash,
		 * one will be added.
		 * @param in_dir The absolute path to the new cwd.
		 */
		void setCwd(const CxChar *in_dir);

	} // namespace sys
	
} // namespace cat

#endif // CX_CORE_COMMON_CXSYS_H
