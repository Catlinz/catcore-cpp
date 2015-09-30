#ifndef CX_CORE_TIME_APPLE_CXAPPLETIME_H
#define CX_CORE_TIME_APPLE_CXAPPLETIME_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxAppleTime.h
 * @brief Contains the CxTime and CxAbsTime classes.
 * 
 * The CxAbsTime class can be the same as the CxTime
 * class depending on the platform, but on some platforms
 * using only positive time values (OSX) can increase 
 * the speed.
 *
 * @author Catlin Zilinski
 * @date Sept 30, 2015
 */

#include "core/Cx.h"
#include "core/time/internal/CxTimeDefs.h"
#include "core/time/CxAbsTime.h"

namespace cat {

	/**
	 * @class CxTime CxTime.h "core/time/CxTime.h"
	 * @brief A simple Time value class.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept 30, 2015
	 */
	class CxTime {
	  public:
		/** @brief Create a zero CxTime value */		
		CX_FORCE_INLINE CxTime() { m_t.tv_sec = 0;  m_t.tv_nsec = 0; }
		
		/** @brief Create a new Time from the specified unix time. */
		CX_FORCE_INLINE CxTime(timespec in_t) : m_t(in_time) {}

		/** @brief Default copy constructor. */
		CX_FORCE_INLINE CxTime(const CxTime &in_src) : m_t(in_src.m_t) {}

		/** @brief Copy constructor to copy from an CxAbsTime. */
		CX_FORCE_INLINE  CxTime(const CxAbsTime& in_t) {
			const CxU64 n = in_t.nano();
			const CxU64 sec = n / CX_NANO_PER_SEC;
			m_t.tv_sec = sec;
			m_t.tv_nsec = (n - (sec * CX_NANO_PER_SEC));
		}

		/** @brief Overloaded assignment operator to assign from CxAbsTime. */
		CX_FORCE_INLINE CxTime& operator=(const CxAbsTime &in_src) {
			const CxU64 n = in_time.nano();
			const CxU64 sec = n / CX_NANO_PER_SEC;
			m_t.tv_sec = sec;
			m_t.tv_nsec = (n - (sec * CX_NANO_PER_SEC));
			return *this;
		}

		/** @brief Default assignment operator. */
		CX_FORCE_INLINE CxTime& operator=(const CxTime& in_src) {
			m_t = in_src.m_t;  return *this;
		}
		
		/** @return True if the two time vals are equal. */
		CX_FORCE_INLINE CxBool operator==(const CxTime& in_t) const {
			return (m_t.tv_sec == in_t.m_t.tv_sec &&
					  m_t.tv_nsec == in_t.m_t.tv_nsec);
		}

		/**  @return True if the two time vals are NOT equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxTime& in_t) const {
			return (m_t.tv_sec != in_t.m_t.tv_sec ||
					  m_t.tv_nsec != in_t.m_t.tv_nsec);
		}

		/** @return True if lhs < rhs. */
		CX_FORCE_INLINE CxBool operator<(const CxTime& in_rhs) const {
			return ((m_t.tv_sec < in_rhs.m_t.tv_sec) ||
					  (m_t.tv_sec == in_rhs.m_t.tv_sec &&
						m_t.tv_nsec < in_rhs.m_t.tv_nsec));
		}

		/** @return True if lhs <= rhs. */
		CX_FORCE_INLINE CxBool operator<=(const CxTime& in_rhs) const {
			return ((m_t.tv_sec < in_rhs.m_t.tv_sec) ||
					  (m_t.tv_sec == in_rhs.m_t.tv_sec &&
						m_t.tv_nsec <= in_rhs.m_t.tv_nsec));
		}

	   /** @return True if lhs > rhs. */
		CX_FORCE_INLINE CxBool operator>(const CxTime& in_rhs) const {
			return ((m_t.tv_sec > in_rhs.m_t.tv_sec) ||
					  (m_t.tv_sec == in_rhs.m_t.tv_sec &&
						m_t.tv_nsec > in_rhs.m_t.tv_nsec));
		}
		
	   /** @return True if lhs >= rhs. */
		CX_FORCE_INLINE CxBool operator>=(const CxTime& in_rhs) const {
			return ((m_t.tv_sec > in_rhs.m_t.tv_sec) ||
					  (m_t.tv_sec == in_rhs.m_t.tv_sec &&
						m_t.tv_nsec >= in_rhs.m_t.tv_nsec));
		}

		/** @return A new time value that is the sum of lhs + rhs */
		CX_FORCE_INLINE CxTime operator+(const CxTime& in_rhs) const {
			timespec t = m_t;
			t.tv_nsec += in_rhs.m_t.tv_nsec;
			t.tv_sec += in_rhs.m_t.tv_sec;
			if (t.tv_nsec > CX_NANO_PER_SEC) {
				t.tv_sec += 1; t.tv_nsec -= CX_NANO_PER_SEC;
			}
			return CxTime(t);
		}

		/** @return A new Time that is the difference of this - in_rhs. */
		CX_FORCE_INLINE CxTime operator-(const CxTime& in_rhs) const {
			timespec t = m_t;
			t.tv_sec -= in_rhs.m_t.tv_sec;
			t.tv_nsec -= in_rhs.m_t.tv_nsec;
			if (t.tv_nsec < 0) {
				t.tv_sec -= 1; t.tv_nsec += CX_NANO_PER_SEC;
			}
			return CxTime(t);
		}

		/** @return This time after adding the rhs to it. */
		CX_FORCE_INLINE CxTime& operator+=(const CxTime& in_rhs) {
			m_t.tv_sec += in_rhs.m_t.tv_sec;
			m_t.tv_nsec += in_rhs.m_t.tv_nsec;
			if (m_t.tv_nsec > CX_NANO_PER_SEC) {
				m_t.tv_sec += 1; m_t.tv_nsec -= CX_NANO_PER_SEC;
			}
			return *this;
		}

		/** @return This time after subtracting the rhs from it. */
		CX_FORCE_INLINE CxTime& operator-=(const CxTime& in_rhs) {
			m_t.tv_sec -= in_rhs.m_t.tv_sec;
			m_t.tv_nsec -= in_rhs.m_t.tv_nsec;
			if (m_t.tv_nsec > 0) {
				m_t.tv_sec -= 1; m_t.tv_nsec += CX_NANO_PER_SEC;
			}
			return *this;			
		}

		/** @return The current time value using the system clock. */
		CX_FORCE_INLINE static CxTime current() {
			timespec t;
			const CxU64 n = CxAbsTime::currentNano();
			const CxU64 sec = n / CX_NANO_PER_SEC;
			t.tv_sec = sec;
			t.tv_nsec = (n - (sec * CX_NANO_PER_SEC));
			return CxTime(t);
		}

		/** @return The current time in microseconds. */
		CX_FORCE_INLINE static CxU64 currentMicro() {
			return CxAbsTime::currentMicro();
		}

		/** @return The current time in milliseconds. */
		CX_FORCE_INLINE static CxU64 currentMilli() {
			return CxAbsTime::currentMilli();
		}

		/** @return The current time in nanoseconds. */
		CX_FORCE_INLINE static CxU64 currentNano() {
			return CxAbsTime::currentNano();
		}

		/** @return The current time in seconds (32bit). */
		CX_FORCE_INLINE static CxF32 currentSec32() {
			return CxAbsTime::currentSec();
		}

		/** @return The current time in seconds (64bit). */
		CX_FORCE_INLINE static CxF64 currentSec64() {
			return CxAbsTime::currentSec64();
		}

		/** @return The fraction of seconds in microseconds. */
		CX_FORCE_INLINE CxU32 fracMicro() const { return m_t.tv_nsec / CX_NANO_PER_MICRO; }

		/** @return The fraction of seconds in milliseconds. */
		CX_FORCE_INLINE CxU32 fracMilli() const { return m_t.tv_nsec / CX_NANO_PER_MILLI; }

		/** @return The fraction of seconds in nanoseconds. */
		CX_FORCE_INLINE CxU32 fracNano() const { return m_t.tv_nsec; }

		/** @return The fraction of seconds in seconds (64 bit). */
		CX_FORCE_INLINE CxF64 fracSec64() const { return (CxF64)m_t.tv_nsec * CX_SEC_PER_NANO; }

		/** @return The fraction of seconds in seconds (32 bit). */
		CX_FORCE_INLINE CxF32 fracSec32() const { return (CxF32)m_t.tv_nsec * CX_SEC_PER_NANO; }

		/** @return A new time value from microseconds */
		static CX_FORCE_INLINE CxTime fromMicro(CxI64 in_micro) {
			CxTime t;  t.setMicro(in_micro);  return t;
		}

		/** @return A new time value from milliseconds */
		static CX_FORCE_INLINE CxTime fromMilli(CxI64 in_milli) {
			CxTime t;  t.setMilli(in_milli);  return t;
		}

		/** @return A new time value from nanoseconds */
		static CX_FORCE_INLINE CxTime fromNano(CxI64 in_nano) {
			CxTime t;  t.setNano(in_nano);  return t;
		}

		/** @return A new time value from seconds */
		static CX_FORCE_INLINE CxTime fromSec(CxF64 in_sec) {
			CxTime t;  t.setSec(in_sec);  return t;
		}
		
		/** @return The value of the Time in microseconds. */
		CX_FORCE_INLINE CxI64 micro() const {
			return (CxI64)m_t.tv_sec * CX_MICRO_PER_SEC + (CxI64)(m_t.tv_nsec / CX_NANO_PER_MICRO);
		}

		/** @return The value of the Time in milliseconds. */
		CX_FORCE_INLINE CxI64 milli() const {
			return (CxI64)m_t.tv_sec * CX_MILLI_PER_SEC + (CxI64)(m_t.tv_nsec / CX_NANO_PER_MILLI);
		}
		
		/** @return The value of the Time in nanoseconds. */
		CX_FORCE_INLINE CxI64 nano() const {
			return (CxI64)m_t.tv_sec * CX_NANO_PER_SEC + (CxI64)m_t.tv_nsec;
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
		CX_FORCE_INLINE void setMicro(CxI64 in_micro) {
			m_t.tv_sec = in_micro / CX_MICRO_PER_SEC;
			m_t.tv_nsec = (in_micro - (m_t.tv_sec * CX_MICRO_PER_SEC)) * CX_NANO_PER_MICRO;
		}

		/** @brief Set the value of the Time in milliseconds. */
		CX_FORCE_INLINE void setMilli(CxI64 in_milli) {
			m_t.tv_sec = in_milli / CX_MILLI_PER_SEC;
			m_t.tv_nsec = (in_milli - (m_t.tv_sec * CX_MILLI_PER_SEC)) * CX_NANO_PER_MILLI;
		}

		/** @brief Set the value of the Time in nanoseconds. */
		CX_FORCE_INLINE void setNano(CxI64 in_nano) {
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
			const CxU64 n = CxAbsTime::currentNano();
			const CxU64 sec = n / CX_NANO_PER_SEC;
			m_t.tv_sec = sec;
			m_t.tv_nsec = (n - (sec * CX_NANO_PER_SEC));
		}

	  private:
		timespec m_t;
	};

} // namespace cat

#endif // CX_CORE_TIME_APPLE_CXAPPLETIME_H
