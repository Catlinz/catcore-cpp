#ifndef CX_CORE_THREADING_WINDOWS_CXWINDOWSBARRIER_H
#define CX_CORE_THREADING_WINDOWS_CXWINDOWSBARRIER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file CxBarrier.h
 * @brief Contains the definition of the Barrier class for threading.
 *
 * @author Catlin Zilinski
 * @date Nov 3, 2015
 */

#include "core/Cx.h"
#include <Windows.h>

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

		/** @brief Destroy the mutex. */
		void destroy();
		
		/** @brief Initialise the mutex before the first use */
		void initialize();

		/** @brief Wait at the barrier */
		CX_FORCE_INLINE CxBool wait() {
			EnterSynchronizationBarrier(mp_barrier, 0);
			return true;
		}

	  private:
		
		SYNCHRONIZATION_BARRIER *mp_barrier;
	};
} // namespace cat

#endif // CX_CORE_THREADING_WINDOWS_CXWINDOWSBARRIER_H


