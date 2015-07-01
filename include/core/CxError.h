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

namespace cat {

	namespace CxError {
		enum Code {
			kNoError = 0,
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
			kAccessDenied,
			
		   kInsufficientResources,
			
			kDeadlockDetected,
		};

		/**
		 * @brief Print out a human readable error code.
		 * @param in_err The error code to print out.
		 * @return A constant string representing the error.
		 */
		const char * toString(CxError::Code in_err);
	}

} // Namespace cat

#endif // CX_CORE_CXERROR_H
