#ifndef CX_CORE_TIME_CXCLOCK_H
#define CX_CORE_TIME_CXCLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxClock.h
 * @brief Defines a global class to store and retrieve time values.
 *
 * @author Catlin Zilinski
 * @date Sept. 29, 2015
 */

#include "core/time/CxAbsTime.h"

namespace cat {

	/**
	 * @class CxClock CxClock.h "core/time/CxClock.h"
	 * @brief A class to store and retrieve discrete time values.
	 *
	 * The CxClock class is a class that is able to store and retrieve 
	 * time values such as discrete time values that represent the current
	 * time in a time-stepping system (such as a game).
	 *
	 * @author Catlin Zilinski
	 * @version 3
	 * @since Mar 31, 2015
	 */
	class CxClock {
	  public:

		/** @brief Create a zeroed clock*/
		CX_FORCE_INLINE CxClock() {}

		/** @return The elapsed time between last two ticks. */
		CX_FORCE_INLINE const CxAbsTime & elapsed() const { return m_elapsed; }
		
		/** @return A reference to the global clock. */
		CX_FORCE_INLINE static CxClock& global() { return s_global; }

		/** @brief Start the clock (set the initial start time) */
		CX_FORCE_INLINE void start() {
			m_start.setToCurrentTime();  m_lastTick = m_start;
		}

		/** @brief 'Tick' the clock (store the elapsed time btwn ticks */
		CX_FORCE_INLINE void tick() {
			const CxAbsTime t = CxAbsTime::current();
			m_elapsed = t - m_lastTick;
			m_lastTick = t;
		}
		
		/** @return Time since the clock started. */
		CX_FORCE_INLINE CxAbsTime time() const { m_lastTick - m_start; }

	  private:
		CxAbsTime m_start;
		CxAbsTime m_lastTick;
		CxAbsTime m_elapsed;

		static CxClock s_global; /**< A global clock */
	};
} // namespace cat

#endif // CX_CORE_TIME_CXCLOCK_H
