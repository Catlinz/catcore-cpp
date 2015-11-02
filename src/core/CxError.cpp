#include "core/CxError.h"

namespace cat {

	namespace CxErr {

		const char * str(CxErr::Code in_err) {
			switch(in_err) {
			case kNoError: return "No error"; break;
			case kTrueCode: return "True";  break;
			case kFalseCode: return "False";  break;
			case kNullError: return "Null error";  break;
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
			case kAccessError: return "Access error"; break;
			case kAccessDenied: return "Access denied"; break;
				
			case kInsufficientResources: return "Insufficient resources"; break;
			case kOutOfMemory: return "Out of memory"; break;
				
			case kDeadlockDetected: return "Deadlock detected"; break;

			case kIOReadError: return "IO read error";  break;
			case kIOWriteError: return "IO write error";  break;
			case kIOSeekError: return "IO seek error";  break;
			case kIOFlushError: return "IO flush error";  break;
			case kIOInvalidMode: return "IO invalid mode";  break;

			case kFileOpenError: return "File open error";  break;
			case kFilePathError: return "File path error";  break;
			case kFileExists: return "File exists";  break;
			case kFileClosed: return "File closed";  break;
			case kFileCopyError: return "File copy error";  break;
			case kFileNotFound: return "File not found";  break;
				
			default: return "Unknown error code"; break;
			}
		}

	} // namespace CxErr

} // namespace cat
