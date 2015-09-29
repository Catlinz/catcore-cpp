#ifndef CX_CORE_TIME_UNIX_CXUNIXABSTIME_H
#define CX_CORE_TIME_UNIX_CXUNIXABSTIME_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxUnixAbsTime.h
 * @brief Contains CxAbsTime class.
 * 
 * The CxAbsTime class can be the same as the CxTime
 * class depending on the platform, but on some platforms
 * using only positive time values (OSX) can increase 
 * the speed.
 *
 * @author Catlin Zilinski
 * @date Sept 29, 2015
 */

#include "core/Cx.h"
#include "core/time/internal/CxTimeDefs.h"

namespace cat {

	/**
	 * @class CxAbsTime CxUnixAbsTime.h "core/time/unix/CxUnixAbsTime.h"
	 * @brief A simple Time value class.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 29, 2015
	 */
	class CxAbsTime {
	  public:
		/** @brief Create a zero CxAbsTime value */		
		CX_FORCE_INLINE CxAbsTime() { m_t.tv_sec = 0;  m_t.tv_nsec = 0; }
		
		/** @brief Create a new Time from the specified unix time. */
		CX_FORCE_INLINE CxAbsTime(timespec in_t) : m_t(in_time) {}

		/** @brief Default copy constructor. */
		CX_FORCE_INLINE CxAbsTime(const CxAbsTime &in_src) : m_t(in_src.m_t) {}

		/** @brief Default assignment operator. */
		CX_FORCE_INLINE CxAbsTime& operator=(const CxAbsTime& in_src) {
			m_t = in_src.m_t;  return *this;
		}
		
		/** @return True if the two time vals are equal. */
		CX_FORCE_INLINE CxBool operator==(const CxAbsTime& in_t) const {
			return (m_t.tv_sec == in_t.m_t.tv_sec &&
					  m_t.tv_nsec == in_t.m_t.tv_nsec);
		}

		/**  @return True if the two time vals are NOT equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxAbsTime& in_t) const {
			return (m_t.tv_sec != in_t.m_t.tv_sec ||
					  m_t.tv_nsec != in_t.m_t.tv_nsec);
		}

		/** @return True if lhs < rhs. */
		CX_FORCE_INLINE CxBool operator<(const CxAbsTime& in_rhs) const {
			return ((m_t.tv_sec < in_rhs.m_t.tv_sec) ||
					  (m_t.tv_sec == in_rhs.m_t.tv_sec &&
						m_t.tv_nsec < in_rhs.m_t.tv_nsec));
		}

		/** @return True if lhs <= rhs. */
		CX_FORCE_INLINE CxBool operator<=(const CxAbsTime& in_rhs) const {
			return ((m_t.tv_sec < in_rhs.m_t.tv_sec) ||
					  (m_t.tv_sec == in_rhs.m_t.tv_sec &&
						m_t.tv_nsec <= in_rhs.m_t.tv_nsec));
		}

	   /** @return True if lhs > rhs. */
		CX_FORCE_INLINE CxBool operator>(const CxAbsTime& in_rhs) const {
			return ((m_t.tv_sec > in_rhs.m_t.tv_sec) ||
					  (m_t.tv_sec == in_rhs.m_t.tv_sec &&
						m_t.tv_nsec > in_rhs.m_t.tv_nsec));
		}
		
	   /** @return True if lhs >= rhs. */
		CX_FORCE_INLINE CxBool operator>=(const CxAbsTime& in_rhs) const {
			return ((m_t.tv_sec > in_rhs.m_t.tv_sec) ||
					  (m_t.tv_sec == in_rhs.m_t.tv_sec &&
						m_t.tv_nsec >= in_rhs.m_t.tv_nsec));
		}

		/** @return A new time value that is the sum of lhs + rhs */
		CX_FORCE_INLINE CxAbsTime operator+(const CxAbsTime& in_rhs) const {
			timespec t = m_t;
			t.tv_nsec += in_rhs.m_t.tv_nsec;
			t.tv_sec += in_rhs.m_t.tv_sec;
			if (t.tv_nsec > CX_NANO_PER_SEC) {
				t.tv_sec += 1; t.tv_nsec -= CX_NANO_PER_SEC;
			}
			return CxAbsTime(t);
		}

		/** @return A new Time that is the difference of this - in_rhs. */
		CX_FORCE_INLINE CxAbsTime operator-(const CxAbsTime& in_rhs) const {
			timespec t = m_t;
			t.tv_sec -= in_rhs.m_t.tv_sec;
			t.tv_nsec -= in_rhs.m_t.tv_nsec;
			if (t.tv_nsec < 0) {
				t.tv_sec -= 1; t.tv_nsec += CX_NANO_PER_SEC;
			}
			return CxAbsTime(t);
		}

		/** @return This time after adding the rhs to it. */
		CX_FORCE_INLINE CxAbsTime& operator+=(const CxAbsTime& in_rhs) {
			m_t.tv_sec += in_rhs.m_t.tv_sec;
			m_t.tv_nsec += in_rhs.m_t.tv_nsec;
			if (m_t.tv_nsec > CX_NANO_PER_SEC) {
				m_t.tv_sec += 1; m_t.tv_nsec -= CX_NANO_PER_SEC;
			}
			return *this;
		}

		/** @return This time after subtracting the rhs from it. */
		CX_FORCE_INLINE CxAbsTime& operator-=(const CxAbsTime& in_rhs) {
			m_t.tv_sec -= in_rhs.m_t.tv_sec;
			m_t.tv_nsec -= in_rhs.m_t.tv_nsec;
			if (m_t.tv_nsec > 0) {
				m_t.tv_sec -= 1; m_t.tv_nsec += CX_NANO_PER_SEC;
			}
			return *this;			
		}

		/** @return The current time value using the system clock. */
		CX_FORCE_INLINE static CxAbsTime current() {
			timespec t;  clock_gettime(CLOCK_MONOTONIC, &t);
			return CxAbsTime(t);
		}

		/** @return The current time in microseconds. */
		CX_FORCE_INLINE static CxU64 currentMicro() {
			timespec t;  clock_gettime(CLOCK_MONOTONIC, &t);
			return (CxU64)t.tv_sec * CX_MICRO_PER_SEC + (CxU64)(t.tv_nsec / CX_NANO_PER_MICRO);
		}

		/** @return The current time in milliseconds. */
		CX_FORCE_INLINE static CxU64 currentMilli() {
			timespec t;	clock_gettime(CLOCK_MONOTONIC, &t);
			return (CxU64)t.tv_sec * CX_MILLI_PER_SEC + (CxU64)(t.tv_nsec / CX_NANO_PER_MILLI);
		}

		/** @return The current time in nanoseconds. */
		CX_FORCE_INLINE static CxU64 currentNano() {
			timespec t;	clock_gettime(CLOCK_MONOTONIC, &t);
			return (CxU64)t.tv_sec * CX_NANO_PER_SEC + t.tv_nsec;
		}

		/** @return The current time in seconds (32bit). */
		CX_FORCE_INLINE static CxF32 currentSec32() {
			timespec t;	clock_gettime(CLOCK_MONOTONIC, &t);
			return ((CxF32)t.tv_sec) + ((CxF32)t.tv_nsec * CX_SEC_PER_NANO);
		}

		/** @return The current time in seconds (64bit). */
		CX_FORCE_INLINE static CxF64 currentSec64() {
			timespec t;	clock_gettime(CLOCK_MONOTONIC, &t);
			return ((CxF64)t.tv_sec) + ((CxF64)t.tv_nsec * CX_SEC_PER_NANO);
		}

		/** @return The fraction of seconds in microseconds. */
		CX_FORCE_INLINE CxU32 fracMicro() const {
			return m_t.tv_nsec / CX_NANO_PER_MICRO;
		}

		/** @return The fraction of seconds in milliseconds. */
		CX_FORCE_INLINE CxU32 fracMilli() const {
			return m_t.tv_nsec / CX_NANO_PER_MILLI;
		}

		/** @return The fraction of seconds in nanoseconds. */
		CX_FORCE_INLINE CxU32 fracNano() const { return m_t.tv_nsec; }

		/** @return The fraction of seconds in seconds (64 bit). */
		CX_FORCE_INLINE CxF64 fracSec64() const {
			return (CxF64)m_t.tv_nsec * CX_SEC_PER_NANO;
		}

		/** @return The fraction of seconds in seconds (32 bit). */
		CX_FORCE_INLINE CxF32 fracSec32() const {
			return (CxF32)m_t.tv_nsec * CX_SEC_PER_NANO;
		}
		
		/** @return The value of the Time in microseconds. */
		CX_FORCE_INLINE CxU64 micro() const {
			return (CxU64)m_t.tv_sec * CX_MICRO_PER_SEC + (CxU64)(m_t.tv_nsec / CX_NANO_PER_MICRO);
		}

		/** @return The value of the Time in milliseconds. */
		CX_FORCE_INLINE CxU64 milli() const {
			return (CxU64)m_t.tv_sec * CX_MILLI_PER_SEC + (CxU64)(m_t.tv_nsec / CX_NANO_PER_MILLI);
		}
		
		/** @return The value of the Time in nanoseconds. */
		CX_FORCE_INLINE CxU64 nano() const {
			return (CxU64)m_t.tv_sec * CX_NANO_PER_SEC + (CxU64)m_t.tv_nsec;
		}

		/** @return The RawTime backing this Time. */
		CX_FORCE_INLINE timespec raw() const { return m_t; }		

		/**  @return The value of the Time in seconds (32bit fp). */
		CX_FORCE_INLINE CxF32 sec32() const {
			return ((CxF32)m_t.tv_sec) + ((CxF32)m_t.tv_nsec * CX_SEC_PER_NANO);
		}

		/** @return The value of the Time in seconds (64bit fp). */
		CX_FORCE_INLINE CxF64 sec64() const {
			return ((CxF64)m_t.tv_sec) + ((CxF64)m_t.tv_nsec * CX_SEC_PER_NANO);
		}

		/** @brief Set the value of the Time in microseconds. */
		CX_FORCE_INLINE void setMicro(CxU64 in_micro) {
			m_t.tv_sec = in_micro / CX_MICRO_PER_SEC;
			m_t.tv_nsec = (in_micro - (m_t.tv_sec * CX_MICRO_PER_SEC)) * CX_NANO_PER_MICRO;
		}

		/** @brief Set the value of the Time in milliseconds. */
		CX_FORCE_INLINE void setMilli(CxU64 in_milli) {
			m_t.tv_sec = in_milli / CX_MILLI_PER_SEC;
			m_t.tv_nsec = (in_milli - (m_t.tv_sec * CX_MILLI_PER_SEC)) * CX_NANO_PER_MILLI;
		}

		/** @brief Set the value of the Time in nanoseconds. */
		CX_FORCE_INLINE void setNano(CxU64 in_nano) {
			m_t.tv_sec = in_nano / CX_NANO_PER_SEC;
			m_t.tv_nsec = (in_nano - (m_t.tv_sec * CX_NANO_PER_SEC))
		}

		/** @brief Set the value of the Time in platform dependant units. */
		CX_FORCE_INLINE void setRaw(timespec in_time) { m_t = in_time; }
		
		/** @brief Set the value of the Time in seconds (64bit fp). */
		CX_FORCE_INLINE void setSec(CxF64 in_seconds) {
			m_t.tv_sec = (time_t)in_seconds;
			m_t.tv_nsec = (long)((in_seconds - (CxF64)(m_t.tv_sec)) * CX_NANO_PER_MILLI);
		}

		/** @brief Set the value of the Time to the current time. */
		CX_FORCE_INLINE void setToCurrentTime() {
			clock_gettime(CLOCK_MONOTONIC, &m_t);
		}

	  private:
		timespec m_t;
	};

} // namespace cat

#endif // CX_CORE_TIME_UNIX_CXUNIXABSTIME_H
