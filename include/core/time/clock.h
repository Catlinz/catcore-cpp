#ifndef CAT_CORE_TIME_CLOCK_H
#define CAT_CORE_TIME_CLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file clock.h
 * @brief Defines a global class to store and retrieve time values.
 *
 * @author Catlin Zilinski
 * @date Sept 30, 2014
 */

#include "core/time/abstime.h"

namespace Cat {

	/**
	 * @class Clock clock.h "core/time/clock.h"
	 * @brief A class to store and retrieve discrete time values.
	 *
	 * The Clock class is a class that is able to store and retrieve 
	 * time values such as discrete time values that represent the current
	 * time in a time-stepping system (such as a game).
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Mar 31, 2015
	 */
	class Clock {
	  public:

		/**
		 * @brief Create a clock.
		 */
		Clock();

		/**
		 * @brief Create and initialise a clock.
		 * @param in_time The time to initialise the clock to.
		 */
		Clock(const AbsTime &in_time);

		/**
		 * @return A reference to the global clock.
		 */
		inline static Clock& global() { return s_global; }

		/**
		 * @brief Initialise the clock to the specified time.
		 * This method stores the current discrete time and initialises
		 * elapsed to 0.
		 * @param in_time The time to initialise the clock to.
		 */
		void initialise(const AbsTime &in_time);

		/**
		 * @brief Set the current discrete time.
		 * This method stores the specified time and computes the difference
		 * between the previous stored time and the new one, and stores that 
		 * as the elapsed time.
		 * @param in_time The time to set to.
		 */
		void set(const AbsTime &in_time);

		/**
		 * @return The current discrete time in microseconds.
		 */
		inline U64 micro() { return m_tNano / NANO_PER_MICRO; }

		/**
		 * @return The current discrete time in milliseconds.
		 */
		inline U64 milli() { return m_tNano / NANO_PER_MILLI; }

		/**
		 * @return The current discrete time in nanoseconds.
		 */
		inline U64 nano() { return m_tNano; }

		/**
		 * @return The current discrete time in seconds.
		 */
		inline F64 sec64() { return m_tSec; }

		/**
		 * @return The elapsed time in microseconds.
		 */
		inline U64 elapsedMicro() { return m_teNano / NANO_PER_MICRO; }

		/**
		 * @return The elapsed time in milliseconds.
		 */
		inline U64 elapsedMilli() { return m_teNano / NANO_PER_MILLI; }

		/**
		 * @return The elapsed time in nanoseconds.
		 */
		inline U64 elapsedNano() { return m_teNano; }

		/**
		 * @return The elapsed time in seconds.
		 */
		inline F64 elapsedSec64() { return m_teSec; }

	  private:
		U64 m_teNano;	/**< Elapsed time in nanoseconds */
		F64 m_teSec;	/**< Elapsed time in seconds */
		U64 m_tNano;	/**< Current time in nanoseconds */
		F64 m_tSec;		/**< Current time in seconds */

		static Clock s_global; /**< A global clock */
	};
} // namespace Cat

#endif // CAT_CORE_TIME_CLOCK_H
