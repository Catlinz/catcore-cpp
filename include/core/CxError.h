#ifndef CX_CORE_CXERROR_H
#define CX_CORE_CXERROR_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxError.h
 * @brief Contains a list of system wide error codes.
 *
 * @author Catlin Zilinski
 * @date July 1, 2015
 */

#include "core/Cx.h"

namespace cat {

	namespace CxErr {
		enum Code {
			kNoError = 0,
			kTrueCode,
			kFalseCode,
			kNullError,    /**< Tried to do something with a null resource/value */
			kUnknownError,
			
			kInvalidValue,
			kInvalidParameter,
			kInvalidArgument,
			kInvalidHandle,
			kInvalidAddress,
			kInvalidFlag,
			kInvalidType,
			kInvalidPassword,
			kInvalidName,
			kInvalidFormat,
			kInvalidProperty,

			kInvalidFunction,
			kInvalidOperation,
			
			kInvalidPermissions,
			kAccessError,  /**< Failed to access a resource. */
			kAccessDenied,  /**< Do not have permission to access resource. */
			
		   kInsufficientResources,
			kOutOfMemory,
			
			kDeadlockDetected,

			/* IO specific errors */
			kIOReadError,  /**< Failed to read from the IO Device */
			kIOWriteError, /**< Failed to write to the IO Device */
			kIOSeekError,  /**< Failed to change position in the IO Device */
			kIOFlushError, /**< Failed to flush the IO Device */
			kIOInvalidMode, /**< Tried to open the IO Device in an invalid mode. */

			/* File specific errors */
			kFileOpenError,   /**< Failed to open the file */
			kFilePathError,   /**< Something wrong with the file path */
			kFileExists, /**< File already exists */
			kFileClosed, /**< Tried to do something on a closed file */
			kFileCopyError, /**< Error occurred when trying to copy file */
			kFileNotFound, /**< File was... not found. */
		};

		/**
		 * @brief Print out a human readable error code.
		 * @param in_err The error code to print out.
		 * @return A constant string representing the error.
		 */
		const CxChar * toStr(Code in_err);
	}

} // Namespace cat

#endif // CX_CORE_CXERROR_H
