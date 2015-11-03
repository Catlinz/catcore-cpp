#ifndef CX_CORE_THREADING_POSIX_PTHREAD_BARRIER_H
#define CX_CORE_THREADING_POSIX_PTHREAD_BARRIER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file pthread_barrier.h
 * @brief Contains fixes for not having pthreads libary with barriers.
 *
 * @author Catlin Zilinski
 * @date Nov 3, 2015
 */

#include "core/Cx.h"
#include <pthread.h>

namespace cat {

#define PTHREAD_BARRIER_SERIAL_THREAD 1
	
	struct pthread_barrier_t {
		pthread_mutex_t mutex;
		pthread_cond_t cond;
		CxU32 count, max;
		volatile CxBool flag;
	};

	CxI32 pthread_barrier_destroy(pthread_barrier_t *in_barrier);
	CxI32 pthread_barrier_init(pthread_barrier_t *in_barrier, void *in_attr,
										CxU32 in_count);
	CxI32 pthread_barrier_wait(pthread_barrier_t *in_barrier);
	

} // namespace cat

#endif // CX_CORE_THREADING_POSIX_PTHREAD_BARRIER_H


