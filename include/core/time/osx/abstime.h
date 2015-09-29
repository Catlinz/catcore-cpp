#ifndef CAT_CORE_TIME_OSX_ABSTIME_H
#define CAT_CORE_TIME_OSX_ABSTIME_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file abstime.h
 * @brief Contains the definition for the AbsTime class.
 *
 * @author Catlin Zilinski
 * @date Mar 7, 2015
 */
#if defined (OS_APPLE)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif // OS_APPLE
#include "core/time/timedefs.h"

namespace Cat {

	/**
	 * @class AbsTime abstime.h "core/time/abstime.h"
	 * @brief A simple class to hold a POSITIVE Time value.
	 *
	 * @author Catlin Zilinski
	 * @version 3
	 * @since Mar 7, 2015
	 */
	class AbsTime {
	public:
		/**
		 * @brief Create an empty AbsTime
		 */		
		inline AbsTime() : m_time(0) {}

		/**
		 * @brief Create a new AbsTime from the specified osx time.
		 * @param in_time The platform dependant unit of time.
		 */
		inline AbsTime(uint64_t in_time)
			: m_time(in_time) {}

		/**
		 * @return True if the two time vals are equal.
		 */
		inline Boolean operator==(const AbsTime& in_rval) const {
			return m_time == in_rval.m_time;
		}

		/**
		 * @return True if the two time vals are NOT equal.
		 */
		inline Boolean operator!=(const AbsTime& in_rval) const {
			return m_time != in_rval.m_time;
		}

		/**
		 * @param in_rhs The AbsTime to compare to.
		 * @return True if this is less than in_rhs.
		 */
		inline Boolean operator<(const AbsTime& in_rhs) const {
			return m_time < in_rhs.m_time;
		}

		/**
		 * @param in_rhs The AbsTime to compare to.
		 * @return True if this is less than or equal to in_rhs.
		 */
		inline Boolean operator<=(const AbsTime& in_rhs) const {
			return m_time <= in_rhs.m_time;
		}

		/**
		 * @param in_rhs The AbsTime to compare to.
		 * @return True if this is greater than in_rhs.
		 */
		inline Boolean operator>(const AbsTime& in_rhs) const {
			return m_time > in_rhs.m_time;
		}
		
		/**
		 * @param in_rhs The AbsTime to compare to.
		 * @return True if this is greater than or equal to in_rhs.
		 */
		inline Boolean operator>=(const AbsTime& in_rhs) const {
			return m_time >= in_rhs.m_time;
		}

		/**
		 * @param in_rhs The AbsTime to add to this one.
		 * @return A new AbsTime that is the sum of this + in_rhs.
		 */
		inline AbsTime operator+(const AbsTime& in_rhs) const {
			return AbsTime(m_time + in_rhs.m_time);
		}

		/**
		 * @param in_rhs The AbsTime to subtract from this one.
		 * @return A new AbsTime that is the difference of this - in_rhs.
		 */
		inline AbsTime operator-(const AbsTime& in_rhs) const {
			return AbsTime(m_time - in_rhs.m_time);
		}

		/**
		 * @param in_rhs The AbsTime to add to this one.
		 * @return A reference to this AbsTime.
		 */
		inline AbsTime& operator+=(const AbsTime& in_rhs) {
		   m_time += in_rhs.m_time;
			return *this;			
		}

		/**
		 * @param in_rhs The AbsTime to subtract from this one.
		 * @return A reference to this AbsTime.
		 */
		inline AbsTime& operator-=(const AbsTime& in_rhs) {
			m_time -= in_rhs.m_time;
			return *this;			
		}

		/* ###################################################
		 * The methods for converting to and from the mach times.
		 * ################################################### */
		static inline uint64_t nanoToRaw(U64 in_nano) {
			return (uint64_t)((in_nano * s_xTimeBaseInfo.denom) / s_xTimeBaseInfo.numer);
		}
		static inline uint64_t microToRaw(U64 in_micro) {
			 return (uint64_t)((in_micro * NANO_PER_MICRO * s_xTimeBaseInfo.denom) / s_xTimeBaseInfo.numer);
		}
		static inline uint64_t milliToRaw(U64 in_milli) {
			return (uint64_t)((in_milli * NANO_PER_MILLI * s_xTimeBaseInfo.denom) / s_xTimeBaseInfo.numer);
		}
		static inline uint64_t secToRaw(F64 in_seconds) {
			return (((uint64_t)(in_seconds * NANO_PER_SEC) * m_timeBaseInfo.numer) / m_timeBaseInfo.denom);
		}
		static inline U64 rawToNano(uint64_t in_raw) {
			return (U64)((in_raw * s_xTimeBaseInfo.numer) / s_xTimeBaseInfo.denom);
		}
		static inline U64 rawToMicro(uint64_t in_raw) {
			return (U64)((in_raw * s_xTimeBaseInfo.numer) / (s_xTimeBaseInfo.denom * NANO_PER_MICRO));
		}
		static inline U64 rawToMilli(uint64_t in_raw) {
			return (U64)((in_raw * s_xTimeBaseInfo.numer) / s_xTimeBaseInfo.denom) / NANO_PER_MILLI;
		}
		static inline F32 rawToSec(uint64_t in_raw) {
			return (F32)((F64)((U64)((in_raw * s_xTimeBaseInfo.numer) / s_xTimeBaseInfo.denom)) / (F64)(NANO_PER_SEC));
		}
		static inline F64 rawToSec64(uint64_t in_raw) {
			return (F64)((U64)((in_raw * s_xTimeBaseInfo.numer) / s_xTimeBaseInfo.denom)) / (F64)(NANO_PER_SEC);
		}

		/**
		 * @return An AbsTime with the current time in it.
		 */
		inline static AbsTime currentTime() {
			return AbsTime(mach_absolute_time());
		}

		/**
		 * @return The current time in microseconds.
		 */
		inline static U64 currentTimeMicro() {
			return rawToMicro(mach_absolute_time());
		}

		/**
		 * @return The current time in milliseconds.
		 */
		inline static U64 currentTimeMilli() {
			return rawToMilli(mach_absolute_time());
		}

		/**
		 * @return The current time in nanoseconds.
		 */
		inline static U64 currentTimeNano() {
			return rawToNano(mach_absolute_time());
		}

		/**
		 * @return The current time in seconds (32bit).
		 */
		inline static F32 currentTimeSec() {
			return rawToSec(mach_absolute_time());
		}

		/**
		 * @return The current time in seconds (64bit).
		 */
		inline static F64 currentTimeSec64() {
			return rawToSec64(mach_absolute_time());
		}

		/**
		 * @return The fraction of seconds in microseconds.
		 */
		inline U32 fracMicro() const {
			U64 n = nano();
			U64 sec = n / NANO_PER_SEC;
			return (n - (sec * NANO_PER_SEC)) / NANO_PER_MICRO;
		}

		/**
		 * @return The fraction of seconds in milliseconds.
		 */
		inline U32 fracMilli() const {
			U64 n = nano();
			U64 sec = n / NANO_PER_SEC;
			return (n - (sec * NANO_PER_SEC)) / NANO_PER_MILLI;
		}

		/**
		 * @return The fraction of seconds in nanoseconds.
		 */
		inline U32 fracNano() const {
			U64 n = nano();
			U64 sec = n / NANO_PER_SEC;
			return (n - (sec * NANO_PER_SEC));
		}

		/**
		 * @return The fraction of seconds in seconds (64 bit).
		 */
		inline F64 fracSec64() const {
			U64 n = nano();
			U64 sec = n / NANO_PER_SEC;
			return ((F64)(n - (sec * NANO_PER_SEC))) / (F64)NANO_PER_SEC;
		}

		/**
		 * @return The fraction of seconds in seconds (32 bit).
		 */
		inline F32 fracSec() const {
		   return (F32)fracSec64();
		}
		
		/**
		 * @return The value of the AbsTime in microseconds.
		 */
		inline U64 micro() const {
			return rawToMicro(m_time);
		}

		/**
		 * @return The value of the AbsTime in milliseconds.
		 */
		inline U64 milli() const {
			return rawToMilli(m_time);
		}
		
		/**
		 * @return The value of the AbsTime in nanoseconds.
		 */
		inline U64 nano() const {
			return rawToNano(m_time);
		}

		/**
		 * @return The RawAbsTimeue backing this AbsTimeue.
		 */
		inline uint64_t raw() const {
			return m_time;
		}		

		/**
		 * @return The value of the AbsTime in seconds (32bit fp).
		 */
		inline F32 sec() const {
			return rawToSec(m_time);
		}

		/**
		 * @return The value of the AbsTime in seconds (64bit fp).
		 */
		inline F64 sec64() const {
			return rawToSec64(m_time);
		}

		/**
		 * @brief Set the value of the AbsTime in microseconds.
		 * @param in_micro The time in microseconds.
		 */
		inline void setMicro(U64 in_micro) {
			m_time = microToRaw(in_micro);
		}

		/**
		 * @brief Set the value of the AbsTime in milliseconds.
		 * @param in_milli The time in milliseconds.
		 */
		inline void setMilli(U64 in_milli) {
			m_time = milliToRaw(in_milli);
		}

		/**
		 * @brief Set the value of the AbsTime in nanoseconds.
		 * @param in_nano The time in nanoseconds.
		 */
		inline void setNano(U64 in_nano) {
			m_time = nanoToRaw(in_nano);
		}

		/**
		 * @brief Set the value of the AbsTime in platform dependant units.
		 * @param time The Time in platform dependant units.
		 */
		inline void setRaw(uint64_t in_time) {
			m_time = in_time;
		}
		
		/**
		 * @brief Set the value of the AbsTime in seconds (64bit fp).
		 * @param in_seconds The time in seconds (64bit fp).
		 */
		inline void setSec(F64 in_seconds) {
			m_time = secToRaw(in_seconds);
		}

		/**
		 * @brief Set the value of the AbsTime to the current time.
		 */
		inline void setToCurrentTime() {
			m_time = mach_absolute_time();
		}

	  private:
		uint64_t m_time;
		static mach_timebase_info_data_t s_xTimeBaseInfo;
	};

#if defined (DEBUG)
	std::ostream& operator<<(std::ostream& out, const AbsTime& t);	
#endif // DEBUG

} // namespace Cat
		
#endif // CAT_CORE_TIME_OSX_ABSTIME_H
