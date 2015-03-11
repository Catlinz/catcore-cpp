#include "core/sys/osx/system.h"
#include <cerrno>
#include "core/io/filepath.h"

namespace Cat {

	StringPtr System::m_pLoginName = StringPtr::nullPtr();
	StringPtr System::m_pCwd = StringPtr::nullPtr();	

	Error::ErrEnum System::chdir(const Char* dir) {
		if (!dir ) {
			DERR("Cannot change to null directory!");			
			return Error::kENullValue;
		}		
		I32 retVal = ::chdir(dir);
		if (retVal == 0) {
			return Error::kENoError;
		} else {
			
			switch(retVal) {
			case EACCES:
				DERR("Cannot access file " << dir << "!");			
				return Error::kEAccessDenied;
				break;
			case EFAULT:
				DERR("Directory '" << dir << "' outside accessible address space!");
				return Error::kEBadAddress;
				break;			
			case EIO:
				DERR("An I/O Error has occurred.");
				return Error::kEIOError;
				break;
			case ELOOP:
				DERR("Too many symbolic links in path " << dir << "!");
				return Error::kEInvalidPath;
				break;
			case ENAMETOOLONG:
				DERR("The path '" << dir << "' is too long!");
				return Error::kEInvalidPath;
				break;
			case ENOMEM:
				DERR("Insufficient Kernel memeory available!");
				return Error::kEInsufficientMemory;
				break;
			case ENOTDIR:
				DERR("The path '" << dir << "' contains a non-directory component!");
				return Error::kEInvalidFile;
				break;
			default:
				DERR("An unknown error with code " << retVal << " has occured!");
				return Error::kEUnknownError;
			};
		}
	}

	StringPtr& System::getCwd() {
		Char buf[512];
		getcwd(buf, 511);
		Size len = strlen(buf);			
		buf[len] = FilePath::separatorChar();
		buf[len + 1] = '\0';			
		if (m_pCwd.isNull() || !m_pCwd->equals(buf)) {
			m_pCwd = String::create(buf);
		}		  		
		return m_pCwd;
	}
		
			

} // namespace Cat
