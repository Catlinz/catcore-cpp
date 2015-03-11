#ifndef CAT_CORE_TIME_WIN32_TIME_H
#define CAT_CORE_TIME_WIN32_TIME_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file time.h
 * @brief Contains the class that holds signed Time values and static methods.
 *
 * @author Catlin Zilinski
 * @date Mar 11, 2015
 */

#include "core/time/timedefs.h"

namespace Cat {

	/**
	 * @class Time time.h "core/time/time.h"
	 * @brief A simple Time value class.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 11, 2015
	 */
	template <typename T64>
	class Time_T {
	  public:

		/**
		 * @brief Create an empty Time value
		 */		
		inline Time_T() {
			m_time = 0;
		}
		
		/**
		 * @brief Create a new Time from the specified osx time.
		 * @param in_time The platform dependant unit of time.
		 */
		inline Time_T(LARGE_INTEGER in_time)
			: m_time(in_time.QuadPart) {}

		/**
		 * @brief Default copy constructor.
		 */
		inline Time_T(const Time_T &in_src) : m_time(in_src.m_time) {}

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
			return m_time == in_rval.m_time;
		}

		/**
		 * @return True if the two time vals are NOT equal.
		 */
		inline Boolean operator!=(const Time_T& in_rval) const {
			return m_time != in_rval.m_time;
		}

		/**
		 * @param in_rhs The Time to compare to.
		 * @return True if this is less than in_rhs.
		 */
		inline Boolean operator<(const Time_T& in_rhs) const {
			return m_time < in_rval.m_time;
		}
		
		/**
		 * @param in_rhs The Time to compare to.
		 * @return True if this is less than or equal to in_rhs.
		 */
		inline Boolean operator<=(const Time_T& in_rhs) const {
		   return m_time <= in_rval.m_time;
		}

		/**
		 * @param in_rhs The Time to compare to.
		 * @return True if this is greater than in_rhs.
		 */
		inline Boolean operator>(const Time_T& in_rhs) const {
			return m_time > in_rval.m_time;
		}
		
		/**
		 * @param in_rhs The Time to compare to.
		 * @return True if this is greater than or equal to in_rhs.
		 */
		inline Boolean operator>=(const Time_T& in_rhs) const {
			return m_time >= in_rval.m_time;
		}

		/**
		 * @param in_rhs The Time to add to this one.
		 * @return A new Time_T that is the sum of this + in_rhs.
		 */
		inline Time_T operator+(const Time_T& in_rhs) const {
			return Time_T(m_time + in_rhs.m_time);
		}

		/**
		 * @param in_rhs The Time to subtract from this one.
		 * @return A new Time that is the difference of this - in_rhs.
		 */
		inline Time_T operator-(const Time_T& in_rhs) const {
			return Time_T(m_time - in_rhs.m_time);
		}

		/**
		 * @param in_rhs The Time to add to this one.
		 * @return A reference to this Time.
		 */
		inline Time_T& operator+=(const Time_T& in_rhs) {
			m_time += in_rhs.m_time;
		}

		/**
		 * @param in_rhs The Time to subtract from this one.
		 * @return A reference to this Time.
		 */
		inline Time_T& operator-=(const Time_T& in_rhs) {
			m_time -= in_rhs.m_time;
		}

		/**
		 * @return A Time with the current time in it.
		 */
		inline static Time_T currentTime() {
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			return Time_T(t);
		}

		/**
		 * @return The current time in microseconds.
		 */
		inline static U64 currentTimeMicro() {
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			return (t.QuadPart * MICRO_PER_SEC) / s_xFreq;
		}

		/**
		 * @return The current time in milliseconds.
		 */
		inline static U64 currentTimeMilli() {
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			return (t.QuadPart * MILLI_PER_SEC) / s_xFreq;
		}

		/**
		 * @return The current time in nanoseconds.
		 */
		inline static U64 currentTimeNano() {
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			return (t.QuadPart * NANO_PER_SEC) / s_xFreq;
		}

		/**
		 * @return The current time in seconds (32bit).
		 */
		inline static F32 currentTimeSec() {
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			return (F32)(t.QuadPart) / (F32)s_xFreq;
		}

		/**
		 * @return The current time in seconds (64bit).
		 */
		inline static F64 currentTimeSec64() {
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			return (F64)(t.QuadPart) / (F64)s_xFreq;
		}

		/**
		 * @return The fraction of seconds in microseconds.
		 */
		U32 fracMicro() const;
		
		/**
		 * @return The fraction of seconds in milliseconds.
		 */
	   U32 fracMilli() const;

		/**
		 * @return The fraction of seconds in nanoseconds.
		 */
		U32 fracNano() const;

		/**
		 * @return The fraction of seconds in seconds (64 bit).
		 */
		F64 fracSec64() const;

		/**
		 * @return The fraction of seconds in seconds (32 bit).
		 */
		F32 fracSec() const;
		
		/**
		 * @return The value of the Time in microseconds.
		 */
		inline T64 micro() const {
			return ((T64)m_time * MICRO_PER_SEC) / s_xFreq;
		}

		/**
		 * @return The value of the Time in milliseconds.
		 */
		inline T64 milli() const {
			return ((T64)m_time * MILLI_PER_SEC) / s_xFreq;
		}
		
		/**
		 * @return The value of the Time in nanoseconds.
		 */
		inline T64 nano() const {
			return ((T64)m_time * NANO_PER_SEC) / s_xFreq;
		}

		/**
		 * @return The RawTimeue backing this Timeue.
		 */
		inline T64 raw() const {
			return m_time;
		}		

		/**
		 * @return The value of the Time in seconds (32bit fp).
		 */
		inline F32 sec() const {
			return (F32)m_time / (F32)s_xFreq;
		}

		/**
		 * @return The value of the Time in seconds (64bit fp).
		 */
		inline F64 sec64() const {
			return (F64)m_time / (F64)s_xFreq;
		}

		/**
		 * @brief Set the value of the Time in microseconds.
		 * @param in_micro The time in microseconds.
		 */
		inline void setMicro(T64 in_micro) {
			m_time = (in_micro * s_xFreq) / MICRO_PER_SEC;
		}

		/**
		 * @brief Set the value of the Time in milliseconds.
		 * @param in_milli The time in milliseconds.
		 */
		inline void setMilli(T64 in_milli) {
			m_time = (in_milli * s_xFreq) / MILLI_PER_SEC;
		}

		/**
		 * @brief Set the value of the Time in nanoseconds.
		 * @param in_nano The time in nanoseconds.
		 */
		inline void setNano(T64 in_nano) {
			m_time = (in_nano * s_xFreq) / NANO_PER_SEC;
		}
			

		/**
		 * @brief Set the value of the Time in platform dependant units.
		 * @param time The Time in platform dependant units.
		 */
		inline void setRaw(LARGE_INTEGER in_time) {
			m_time = in_time.QuadPart;
		}
		
		/**
		 * @brief Set the value of the Time in seconds (64bit fp).
		 * @param in_seconds The time in seconds (64bit fp).
		 */
		inline void setSec(F64 in_seconds) {
			m_time = in_seconds * s_xFreq;
		}

		/**
		 * @brief Set the value of the Time to the current time.
		 */
		inline void setToCurrentTime() {
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			m_time = t.QuadPart;
		}

	  private:
		T64 m_time;
	};

	typedef Time Time_T<I64>;

#include "core/time/win32/abstime.h"
	
} // namespace Cat

#endif // CAT_CORE_TIME_WIN32_TIME_H
