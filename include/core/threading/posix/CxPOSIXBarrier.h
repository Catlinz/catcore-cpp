#ifndef CX_CORE_THREADING_POSIX_CXPOSIXBARRIER_H
#define CX_CORE_THREADING_POSIX_CXPOSIXBARRIER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxBarrier.h
 * @brief Contains the definition of the CxBarrier class for threading.
 *
 * @author Catlin Zilinski
 * @date Nov 3, 2015
 */

#include "core/Cx.h"
#include <pthread.h>

#if defined(CX_APPLE) || !defined(_POSIX_BARRIERS)
#  include "core/threading/posix/pthread_barrier.h"
#endif // No Pthread barriers.

namespace cat {

	/**
	 * @class CxBarrier CxBarrier.h "core/threading/CxBarrier.h"
	 * The Barrier class encapsulates the functionality of a Barrier lock for 
	 * syncronization with multiple threads.
	 * @version 1
	 * @author Catlin Zilinski
	 * @since Nov 3, 2015
	 */
	class CxBarrier {
	  public:

		/** @brief Create an uninitialized CxBarrier. */
		CX_FORCE_INLINE CxBarrier()
			: mp_barrier(0), m_count(0) {}

		/** @brief Create and initialise a new barier.. */
		CX_FORCE_INLINE CxBarrier(CxU32 in_count)
			: mp_barrier(0), m_count(0) { initialize(in_count); }

			/** @brief Move constructor */
		CX_FORCE_INLINE CxBarrier(CxBarrier &&in_src)
			: mp_barrier(in_src.mp_barrier), m_count(in_src.m_count) {
			in_src.mp_barrier = 0;
			in_src.m_count = 0;
		}

		/** @brief DO NOT PASS BY VALUE */
		CxBarrier(const CxBarrier &);

		/** @brief DO NOT PASS BY VALUE */
		CxBarrier & operator=(const CxBarrier &);

		/** @brief Move-assign operator. */
		CX_FORCE_INLINE CxBarrier & operator=(CxBarrier &&in_src) {
		   if (mp_barrier != 0) { destroy(); }
			mp_barrier = in_src.mp_barrier;
			m_count = in_src.m_count;
			in_src.mp_barrier = 0;
			in_src.m_count = 0;
			return *this;
		}

		/** @brief Destroy the CxBarrier. */
		CX_FORCE_INLINE ~CxBarrier() { destroy(); }

		/** 
		 * @brief Destroy the Barrier. 
		 * @return True if barrier destroyed or was already destroyed.
		 */
		CxBool destroy();
		
		/** @brief Initialise the Barrier before the first use */
		void initialize(CxU32 in_count);

		/** @brief Wait at the barrier */
		CX_FORCE_INLINE CxBool wait() {
			CxI32 e = pthread_barrier_wait(mp_barrier);
			CXD_IF_ERR((e == EINVAL), "Called wait() on uninitialized barrier.");
			return (e == PTHREAD_BARRIER_SERIAL_THREAD || e == 0);
		}

	  private:
		pthread_barrier_t	*mp_barrier;
		CxU32 m_count;
	};

}

#endif // CX_CORE_THREADING_POSIX_CXPOSIXBARRIER_H


