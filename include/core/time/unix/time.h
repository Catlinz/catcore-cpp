#ifndef CAT_CORE_TIME_UNIX_TIME_H
#define CAT_CORE_TIME_UNIX_TIME_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file time.h
 * @brief Contains the class that holds signed Time values and static methods.
 *
 * @author Catlin Zilinski
 * @date Feb 28, 2014
 */

#include <time.h>

#if !defined (CLOCK_MONOTONIC) && defined(CLOCK_REALTIME)
#define CLOCK_MONOTONIC CLOCK_REALTIME
#endif

#include "core/time/timedefs.h"
#if defined(OS_APPLE)
#include "core/time/osx/abstime.h"
#endif // OS APPLE

namespace Cat {

	/**
	 * @class Time time.h "core/time/time.h"
	 * @brief A simple Time value class.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Mar 7, 2015
	 */
	template <typename T64>
	class Time_T {
	  public:

		/**
		 * @brief Create an empty Time value
		 */		
		inline Time_T() {
			m_time.tv_sec = 0;
			m_time.tv_nsec = 0;
		}
		
		/**
		 * @brief Create a new Time from the specified osx time.
		 * @param in_time The platform dependant unit of time.
		 */
		inline Time_T(timespec in_time)
			: m_time(in_time) {}

		/**
		 * @brief Default copy constructor.
		 */
		inline Time_T(const Time_T &in_src) : m_time(in_src.m_time) {}

#if defined (OS_APPLE)
		/**
		 * @brief Copy constructor to copy from an AbsTime.
		 */
		inline Time_T(const AbsTime& in_time) {
			U64 n = in_time.nano();
			m_time.tv_sec = (n / NANO_PER_SEC);
			m_time.tv_nsec = (n - (t.tv_sec * NANO_PER_SEC));
		}

		/**
		 * @brief Overloaded assignment operator to assign from AbsTime_T.
		 */
		inline Time_T& operator=(const AbsTime &in_src) {
			U64 n = in_time.nano();
			m_time.tv_sec = (n / NANO_PER_SEC);
			m_time.tv_nsec = (n - (t.tv_sec * NANO_PER_SEC));
			return *this;
		}
#else /* if defined (OS_LINUX) */
		/**
		 * @brief Copy constructor to copy from different templated type.
		 */
		template<typename U>
		inline Time_T(const Time_T<U> &in_src) : m_time(in_src.m_time) {}

		/**
		 * @brief Overloaded assignment operator to assign from other templated type.
		 */
		template<typename U>
		inline Time_T& operator=(const Time_T<U> &in_src) {
			m_time = in_src.m_time;
			return *this;
		}
#endif /* defined(OS_APPLE) else defined (OS_LINUX) */

		/**
		 * @brief Default assignment operator.
		 */
		inline Time_T& operator=(const Time_T& in_src) {
			m_time = in_src.m_time;
			return *this;
		}
		
		/**
		 * @return True if the two time vals are equal.
		 */
		inline Boolean operator==(const Time_T& in_rval) const {
			return (m_time.tv_sec == in_rval.m_time.tv_sec &&
					  m_time.tv_nsec == in_rval.m_time.tv_nsec);
		}

		/**
		 * @return True if the two time vals are NOT equal.
		 */
		inline Boolean operator!=(const Time_T& in_rval) const {
			return (m_time.tv_sec != in_rval.m_time.tv_sec ||
					  m_time.tv_nsec != in_rval.m_time.tv_nsec);
		}

		/**
		 * @param in_rhs The Time to compare to.
		 * @return True if this is less than in_rhs.
		 */
		inline Boolean operator<(const Time_T& in_rhs) const {
			return ((m_time.tv_sec < in_rhs.m_time.tv_sec) ||
					  (m_time.tv_sec == in_rhs.m_time.tv_sec &&
						m_time.tv_nsec < in_rhs.m_time.tv_nsec));
		}

		/**
		 * @param in_rhs The Time to compare to.
		 * @return True if this is less than or equal to in_rhs.
		 */
		inline Boolean operator<=(const Time_T& in_rhs) const {
			return ((m_time.tv_sec < in_rhs.m_time.tv_sec) ||
					  (m_time.tv_sec == in_rhs.m_time.tv_sec &&
						m_time.tv_nsec <= in_rhs.m_time.tv_nsec));
		}

		/**
		 * @param in_rhs The Time to compare to.
		 * @return True if this is greater than in_rhs.
		 */
		inline Boolean operator>(const Time_T& in_rhs) const {
			return ((m_time.tv_sec > in_rhs.m_time.tv_sec) ||
					  (m_time.tv_sec == in_rhs.m_time.tv_sec &&
						m_time.tv_nsec > in_rhs.m_time.tv_nsec));
		}
		
		/**
		 * @param in_rhs The Time to compare to.
		 * @return True if this is greater than or equal to in_rhs.
		 */
		inline Boolean operator>=(const Time_T& in_rhs) const {
			return ((m_time.tv_sec > in_rhs.m_time.tv_sec) ||
					  (m_time.tv_sec == in_rhs.m_time.tv_sec &&
						m_time.tv_nsec >= in_rhs.m_time.tv_nsec));
		}

		/**
		 * @param in_rhs The Time to add to this one.
		 * @return A new Time_T that is the sum of this + in_rhs.
		 */
		inline Time_T operator+(const Time_T& in_rhs) const {
			timespec t = m_time;
			t.tv_sec += in_rhs.m_time.tv_sec;
			t.tv_nsec += in_rhs.m_time.tv_nsec;
			if (t.tv_nsec > NANO_PER_SEC) {
				t.tv_sec += 1; t.tv_nsec -= NANO_PER_SEC;
			}
			return Time_T(t);
		}

		/**
		 * @param in_rhs The Time to subtract from this one.
		 * @return A new Time that is the difference of this - in_rhs.
		 */
		inline Time_T operator-(const Time_T& in_rhs) const {
			timespec t = m_time;
			t.tv_sec -= in_rhs.m_time.tv_sec;
			t.tv_nsec -= in_rhs.m_time.tv_nsec;
			if (t.tv_nsec < 0) {
				t.tv_sec -= 1; t.tv_nsec += NANO_PER_SEC;
			}
			return Time_T(t);
		}

		/**
		 * @param in_rhs The Time to add to this one.
		 * @return A reference to this Time.
		 */
		inline Time_T& operator+=(const Time_T& in_rhs) {
			m_time.tv_sec += in_rhs.m_time.tv_sec;
			m_time.tv_nsec += in_rhs.m_time.tv_nsec;
			if (m_time.tv_nsec > NANO_PER_SEC) {
				m_time.tv_sec += 1; m_time.tv_nsec -= NANO_PER_SEC;
			}
			return *this;
		}

		/**
		 * @param in_rhs The Time to subtract from this one.
		 * @return A reference to this Time.
		 */
		inline Time_T& operator-=(const Time_T& in_rhs) {
			m_time.tv_sec -= in_rhs.m_time.tv_sec;
			m_time.tv_nsec -= in_rhs.m_time.tv_nsec;
			if (m_time.tv_nsec > 0) {
				m_time.tv_sec -= 1; m_time.tv_nsec += NANO_PER_SEC;
			}
			return *this;			
		}

		/**
		 * @return An Time with the current time in it.
		 */
		inline static Time_T currentTime() {
			timespec t;
#if defined (OS_APPLE)
			U64 n = AbsTime::currentTimeNano();
			t.tv_sec = (n / NANO_PER_SEC);
			t.tv_nsec = (n - (t.tv_sec * NANO_PER_SEC));
#else // UNIX
			clock_gettime(CLOCK_MONOTONIC, &t);
#endif
			return Time_T(t);
		}

		/**
		 * @return The current time in microseconds.
		 */
		inline static U64 currentTimeMicro() {
#if defined (OS_APPLE)
			return AbsTime::currentTimeMicro();
#else // UNIX
			timespec t; clock_gettime(CLOCK_MONOTONIC, &t);
			return (U64)t.tv_sec * MICRO_PER_SEC + (U64)(t.tv_nsec / NANO_PER_MICRO);
#endif
		}

		/**
		 * @return The current time in milliseconds.
		 */
		inline static U64 currentTimeMilli() {
#if defined (OS_APPLE)
			return AbsTime::currentTimeMilli();
#else // UNIX
			timespec t;	clock_gettime(CLOCK_MONOTONIC, &t);
			return (U64)t.tv_sec * MILLI_PER_SEC + (U64)(t.tv_nsec / NANO_PER_MILLI);
#endif
		}

		/**
		 * @return The current time in nanoseconds.
		 */
		inline static U64 currentTimeNano() {
#if defined (OS_APPLE)
			return AbsTime::currentTimeNano();
#else // UNIX
			timespec t;	clock_gettime(CLOCK_MONOTONIC, &t);
			return (U64)t.tv_sec * NANO_PER_SEC + t.tv_nsec;
#endif
		}

		/**
		 * @return The current time in seconds (32bit).
		 */
		inline static F32 currentTimeSec() {
#if defined (OS_APPLE)
			return AbsTime::currentTimeSec();
#else // UNIX
			timespec t;	clock_gettime(CLOCK_MONOTONIC, &t);
			return ((F32)t.tv_sec) + ((F32)t..tv_nsec / (F32)NANO_PER_SEC);
#endif
		}

		/**
		 * @return The current time in seconds (64bit).
		 */
		inline static F64 currentTimeSec64() {
#if defined (OS_APPLE)
			return AbsTime::currentTimeSec64();
#else // UNIX
			timespec t;	clock_gettime(CLOCK_MONOTONIC, &t);
			return ((F64)t.tv_sec) + ((F64)t.tv_nsec / (F64)NANO_PER_SEC);
#endif
		}

		/**
		 * @return The fraction of seconds in microseconds.
		 */
		inline U32 fracMicro() const {
			return m_time.tv_nsec / NANO_PER_MICRO
		}

		/**
		 * @return The fraction of seconds in milliseconds.
		 */
		inline U32 fracMilli() const {
			return m_time.tv_nsec / NANO_PER_MILLI;
		}

		/**
		 * @return The fraction of seconds in nanoseconds.
		 */
		inline U32 fracNano() const {
			return m_time.tv_nsec;
		}

		/**
		 * @return The fraction of seconds in seconds (64 bit).
		 */
		inline F64 fracSec64() const {
			return (F64)m_time.tv_nsec / (F64)NANO_PER_SEC;
		}

		/**
		 * @return The fraction of seconds in seconds (32 bit).
		 */
		inline F32 fracSec() const {
		   return (F32)m_time.tv_nsec / (F32)NANO_PER_SEC;
		}
		
		/**
		 * @return The value of the Time in microseconds.
		 */
		inline T64 micro() const {
		   return (T64)m_time.tv_sec * MICRO_PER_SEC + (T64)(m_time.tv_nsec / NANO_PER_MICRO);
		}

		/**
		 * @return The value of the Time in milliseconds.
		 */
		inline T64 milli() const {
			return (T64)m_time.tv_sec * MILLI_PER_SEC + (T64)(m_time.tv_nsec / NANO_PER_MILLI);
		}
		
		/**
		 * @return The value of the Time in nanoseconds.
		 */
		inline T64 nano() const {
			return (T64)m_time.tv_sec * NANO_PER_SEC + (T64)m_time.tv_nsec;
		}

		/**
		 * @return The RawTimeue backing this Timeue.
		 */
		inline timespec raw() const {
			return m_time;
		}		

		/**
		 * @return The value of the Time in seconds (32bit fp).
		 */
		inline F32 sec() const {
			return ((F32)m_time.tv_sec) + ((F32)m_time.tv_nsec / (F32)NANO_PER_SEC);
		}

		/**
		 * @return The value of the Time in seconds (64bit fp).
		 */
		inline F64 sec64() const {
			return ((F64)m_time.tv_sec) + ((F64)m_time.tv_nsec / (F64)NANO_PER_SEC);
		}

		/**
		 * @brief Set the value of the Time in microseconds.
		 * @param in_micro The time in microseconds.
		 */
		inline void setMicro(T64 in_micro) {
			m_time.tv_sec = in_micro / MICRO_PER_SEC;
			m_time.tv_nsec = (in_micro - (m_time.tv_sec * MICRO_PER_SEC)) * NANO_PER_MICRO;
		}

		/**
		 * @brief Set the value of the Time in milliseconds.
		 * @param in_milli The time in milliseconds.
		 */
		inline void setMilli(T64 in_milli) {
			m_time.tv_sec = in_milli / MILLI_PER_SEC;
			m_time.tv_nsec = (in_milli - (m_time.tv_sec * MILLI_PER_SEC)) * NANO_PER_MILLI;
		}

		/**
		 * @brief Set the value of the Time in nanoseconds.
		 * @param in_nano The time in nanoseconds.
		 */
		inline void setNano(T64 in_nano) {
			m_time.tv_sec = in_nano / NANO_PER_SEC;
			m_time.tv_nsec = (in_nano - (m_time.tv_sec * NANO_PER_SEC))
		}

		/**
		 * @brief Set the value of the Time in platform dependant units.
		 * @param time The Time in platform dependant units.
		 */
		inline void setRaw(timespec in_time) {
			m_time = in_time;
		}
		
		/**
		 * @brief Set the value of the Time in seconds (64bit fp).
		 * @param in_seconds The time in seconds (64bit fp).
		 */
		inline void setSec(F64 in_seconds) {
			m_time.tv_sec = (time_t)in_seconds;
			m_time.tv_nsec = (long)((in_seconds - (F64)(m_time.tv_sec)) * NANO_PER_MILLI);
		}

		/**
		 * @brief Set the value of the Time to the current time.
		 */
		inline void setToCurrentTime() {
#if defined (OS_APPLE)
			U64 n = AbsTime::currentTimeNano();
			m_time.tv_sec = (n / NANO_PER_SEC);
			m_time.tv_nsec = (n - (m_time.tv_sec * NANO_PER_SEC));
#else // UNIX
			clock_gettime(CLOCK_MONOTONIC, &m_time);
#endif
		}

	  private:
		timespec m_time;
	};

	typedef Time Time_T<I64>;

#if !defined (OS_APPLE) && defined(OS_UNIX)
#include "core/time/unix/abstime.h"
#endif // OS_UNIX
	
} // namespace Cat

#endif // CAT_CORE_TIME_UNIX_TIME_H
