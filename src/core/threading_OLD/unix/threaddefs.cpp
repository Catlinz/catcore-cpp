#include "core/threading/unix/threaddefs.h"


namespace Cat {
	
	const char* getStringErrorCode(I32 error) {
		switch(error) {
			case ETIMEDOUT:
				return "ETIMEDOUT";
				break;
			case EINVAL:
				return "EINVAL";
				break;
			case EPERM: 
				return "EPERM";
				break;
			case EBUSY:
				return "EBUSY";
				break;
			case EAGAIN:
				return "EAGAIN";
				break;
			case ENOMEM:
				return "ENOMEM";
				break;
			case 0:
				return "GOOD";
				break;
			default:
				return "ERROR";
				break;
		}
	}


} // namespace Cat
