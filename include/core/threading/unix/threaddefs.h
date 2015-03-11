#ifndef CAT_CORE_THREADING_UNIX_THREAD_DEFS_H
#define CAT_CORE_THREADING_UNIX_THREAD_DEFS_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file threaddefs.h
 * File that contains the core includes / definitions for the unix threading libraries
 *
 * @author Catlin Zilinski
 * @date Aug 6, 2013
 */

#include <pthread.h>
#include <errno.h>
#include "core/corelib.h"



namespace Cat {

	enum RunStatus { RUN_STARTING, RUN_RUNNING, RUN_FINISHING, RUN_FINISHED };

	typedef pthread_t ThreadHandle;

#ifdef ENV_IS_64
	typedef U64 ThreadStatus; 
#else
	typedef U32 ThreadStatus; 
#endif


	inline Boolean equals(ThreadHandle t1, ThreadHandle t2) {
		return pthread_equal(t1, t2);
	}
	
	const char* getStringErrorCode(I32 error);

} // namespace Cat


#endif // CAT_CORE_THREADING_UNIX_THREAD_DEFS_H



