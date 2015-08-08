#include "core/CxError.h"

namespace cat {

	namespace CxError {

		const char * toString(CxError::Code in_err) {
			switch(in_err) {
			case kNoError: return "No error"; break;
			case kUnknownError: return "Unknown error"; break;
				
			case kInvalidValue: return "Invalid value"; break;
			case kInvalidParameter: return "Invalid parameter"; break;
			case kInvalidArgument: return "Invalid argument"; break;
			case kInvalidHandle: return "Invalid handle"; break;
			case kInvalidAddress: return "Invalid address"; break;
			case kInvalidFlag: return "Invalid flag"; break;
			case kInvalidType: return "Invalid type"; break;
			case kInvalidPassword: return "Invalid password"; break;
			case kInvalidName: return "Invalid name"; break;
			case kInvalidFormat: return "Invalid format"; break;
			case kInvalidProperty: return "Invalid property"; break;

			case kInvalidFunction: return "Invalid function"; break;
			case kInvalidOperation: return "Invalid operation"; break;

			case kInvalidPermissions: return "Invalid permissions"; break;
			case kAccessDenied: return "Access denied"; break;
				
			case kInsufficientResources: return "Insufficient resources"; break;

			case kDeadlockDetected: return "Deadlock detected"; break;

			default: return "Unknown error code"; break;
			}
		}

	} // namespace CxError

} // namespace cat
