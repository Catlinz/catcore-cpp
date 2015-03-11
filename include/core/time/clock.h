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

#include "core/corelib.h"

namespace Cat {

	/**
	 * @class Clock clock.h "core/time/clock.h"
	 * @brief A global class to store and retrieve time values.
	 *
	 * The Clock class is a global class that is able to store and retrieve 
	 * time values such as discrete time values that represent the current
	 * time in a time-stepping system (such as a game).
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 30, 2014
	 */
	class Clock {
	  public:

		/**
		 * @brief Initialises the current discrete system time.
		 * Stores the current discrete system time in nanoseconds and
		 * initialises elapsed to 0.
		 * @param p_nanoTime The time to initialise to in nanoseconds.
		 */
		static void initialiseDiscreteTime(U64 p_nanoTime);
		
		/**
		 * @brief Set the current discrete system time.
		 * This methods stores the time in nanoseconds and seconds,
		 * as well as updates an elapsed time since the last update.
		 * @param p_nanoTime The current time in nanoseconds.
		 */
		static void setDiscreteTime(U64 p_nanoTime);

		/**
		 * @brief Get the current discrete time in nanoseconds.
		 * @return The current discrete time in nanoseconds.
		 */
		static inline U64 discreteTimeNano() {
			return s_dTimeNano;
		}

		/**
		 * @brief Get the current discrete time in seconds.
		 * @return The current discrete time in seconds.
		 */
		static inline F64 discreteTimeSec() {
			return s_dTimeSec;
		}

		/**
		 * @brief Get the elapsed time in nanoseconds.
		 * @return The elapsed time in nanoseconds.
		 */
		static inline U64 discreteElapsedNano() {
			return s_dElapsedNano;
		}

		/**
		 * @brief Get the elapsed time in seconds.
		 * @return The elapsed time in seconds.
		 */
		static inline F64 discreteElapsedSec() {
			return s_dElapsedSec;
		}

	  private:
		static U64 s_dElapsedNano;
		static F64 s_dElapsedSec;
		static U64 s_dTimeNano;
		static F64 s_dTimeSec;
	};
} // namespace Cat

#endif // CAT_CORE_TIME_CLOCK_H
