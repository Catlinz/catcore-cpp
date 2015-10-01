#ifndef CX_CORE_TIME_WINDOWS_CXWINDOWSABSTIME_H
#define CX_CORE_TIME_WINDOWS_CXWINDOWSABSTIME_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxWindowsAbsTime.h
 * @brief Contains the CxAbsTime class.
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
	 * @class CxAbsTime CxWindowsAbsTime.h "core/time/windows/CxWindowsAbsTime.h"
	 * @brief A simple Time value class.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept. 29, 2015
	 */
	class CxAbsTime {
	  public:
		/** @brief Create a zero CxAbsTime value */		
		CX_FORCE_INLINE CxAbsTime() { m_t.QuadPart = 0; }
		
		/** @brief Create a new Time from the specified windows time. */
		CX_FORCE_INLINE CxAbsTime(const LARGE_INTEGER &in_t) : m_t(in_t) {}

		/** @brief Default copy constructor. */
		CX_FORCE_INLINE CxAbsTime(const CxAbsTime &in_src) : m_t(in_src.m_t) {}

		/** @brief Default assignment operator. */
		CX_FORCE_INLINE CxAbsTime& operator=(const CxAbsTime& in_src) {
			m_t = in_src.m_t;  return *this;
		}
		
		/** @return True if the two time vals are equal. */
		CX_FORCE_INLINE CxBool operator==(const CxAbsTime& in_t) const {
			return (m_t.QuadPart == in_t.m_t.QuadPart);
		}

		/**  @return True if the two time vals are NOT equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxAbsTime& in_t) const {
			return (m_t.QuadPart != in_t.m_t.QuadPart);
		}

		/** @return True if lhs < rhs. */
		CX_FORCE_INLINE CxBool operator<(const CxAbsTime& in_rhs) const {
			return (m_t.QuadPart < in_rhs.m_t.QuadPart);
		}

		/** @return True if lhs <= rhs. */
		CX_FORCE_INLINE CxBool operator<=(const CxAbsTime& in_rhs) const {
			return (m_t.QuadPart <= in_rhs.m_t.QuadPart);
		}

	   /** @return True if lhs > rhs. */
		CX_FORCE_INLINE CxBool operator>(const CxAbsTime& in_rhs) const {
			return (m_t.QuadPart > in_rhs.m_t.QuadPart);
		}
		
	   /** @return True if lhs >= rhs. */
		CX_FORCE_INLINE CxBool operator>=(const CxAbsTime& in_rhs) const {
			return (m_t.QuadPart >= in_rhs.m_t.QuadPart);
		}

		/** @return A new time value that is the sum of lhs + rhs */
		CX_FORCE_INLINE CxAbsTime operator+(const CxAbsTime& in_rhs) const {
		   CxAbsTime t(*this);
			t += in_rhs;  return t;
		}

		/** @return A new Time that is the difference of this - in_rhs. */
		CX_FORCE_INLINE CxAbsTime operator-(const CxAbsTime& in_rhs) const {
		   CxAbsTime t(*this);
			t -= in_rhs;  return t;
		}

		/** @return This time after adding the rhs to it. */
		CX_FORCE_INLINE CxAbsTime& operator+=(const CxAbsTime& in_rhs) {
		   m_t.QuadPart += in_rhs.m_t.QuadPart;
			return *this;
		}

		/** @return This time after subtracting the rhs from it. */
		CX_FORCE_INLINE CxAbsTime& operator-=(const CxAbsTime& in_rhs) {
		   m_t.QuadPart -= in_rhs.m_t.QuadPart;
			return *this;			
		}

		/** @return The current time value using the system clock. */
		CX_FORCE_INLINE static CxAbsTime current() {
		   LARGE_INTEGER t;  QueryPerformanceCounter(&t);
			return CxAbsTime(t);
		}

		/** @return The current time in microseconds. */
		CX_FORCE_INLINE static CxU64 currentMicro() {
			LARGE_INTEGER t;  QueryPerformanceCounter(&t);
			return (t.QuadPart * CX_MICRO_PER_SEC) / s_freq.QuadPart;
		}

		/** @return The current time in milliseconds. */
		CX_FORCE_INLINE static CxU64 currentMilli() {
		   LARGE_INTEGER t;  QueryPerformanceCounter(&t);
			return (t.QuadPart * CX_MILLI_PER_SEC) / s_freq.QuadPart;
		}

		/** @return The current time in nanoseconds. */
		CX_FORCE_INLINE static CxU64 currentNano() {
		   LARGE_INTEGER t;  QueryPerformanceCounter(&t);
			return (t.QuadPart * CX_NANO_PER_SEC) / s_freq.QuadPart;
		}

		/** @return The current time in seconds (32bit). */
		CX_FORCE_INLINE static CxF32 currentSec32() {
		   LARGE_INTEGER t;  QueryPerformanceCounter(&t);
			return (CxF32)((CxF64)t.QuadPart / (CxF64)s_freq.QuadPart);
		}

		/** @return The current time in seconds (64bit). */
		CX_FORCE_INLINE static CxF64 currentSec64() {
			LARGE_INTEGER t;  QueryPerformanceCounter(&t);
			return (CxF64)t.QuadPart / (CxF64)s_freq.QuadPart;
		}

		/** @return The fraction of seconds in microseconds. */
		CX_FORCE_INLINE CxU32 fracMicro() const {
			const CxF64 sec = (CxF64)m_t.QuadPart / (CxF64)s_freq.QuadPart;
			return (CxU32)((sec - ((CxI64)sec)) * CX_MICRO_PER_SEC);
		}

		/** @return The fraction of seconds in milliseconds. */
		CX_FORCE_INLINE CxU32 fracMilli() const {
			const CxF64 sec = (CxF64)m_t.QuadPart / (CxF64)s_freq.QuadPart;
			return (CxU32)((sec - ((CxI64)sec)) * CX_MILLI_PER_SEC);
		}

		/** @return The fraction of seconds in nanoseconds. */
		CX_FORCE_INLINE CxU32 fracNano() const {
			const CxF64 sec = (CxF64)m_t.QuadPart / (CxF64)s_freq.QuadPart;
			return (CxU32)((sec - ((CxI64)sec)) * CX_NANO_PER_SEC);
		}

		/** @return The fraction of seconds in seconds (64 bit). */
		CX_FORCE_INLINE CxF64 fracSec64() const {
		   const CxF64 sec = (CxF64)m_t.QuadPart / (CxF64)s_freq.QuadPart;
			return sec - ((CxI64)sec);
		}

		/** @return The fraction of seconds in seconds (32 bit). */
		CX_FORCE_INLINE CxF32 fracSec32() const {
			return (CxF32)fracSec64();
		}

		/** @return A new CxTime value from a microsecond value */
		static CX_FORCE_INLINE CxAbsTime fromMicro(CxI64 in_micro) {
			LARGE_INTEGER t;
			t.QuadPart = (in_micro * s_freq.QuadPart)  / CX_MICRO_PER_SEC;
			return CxAbsTime(t);
		}

		/** @return A new CxTime value from a millisecond value */
		static CX_FORCE_INLINE CxAbsTime fromMilli(CxI64 in_milli) {
			LARGE_INTEGER t;
			t.QuadPart = (in_milli * s_freq.QuadPart)  / CX_MILLI_PER_SEC;
			return CxAbsTime(t);
		}

		/** @return A new CxTime value from a nanosecond value */
		static CX_FORCE_INLINE CxAbsTime fromNano(CxI64 in_nano) {
			LARGE_INTEGER t;
			t.QuadPart = (in_nano * s_freq.QuadPart)  / CX_NANO_PER_SEC;
			return CxAbsTime(t);
		}

		/** @return A new CxTime value from a seconds value */
		static CX_FORCE_INLINE CxAbsTime fromSec(CxF64 in_seconds) {
			LARGE_INTEGER t;
			t.QuadPart = (CxI64)(in_seconds * s_freq.QuadPart);
			return CxAbsTime(t);
		}
		
		/** @return The value of the Time in microseconds. */
		CX_FORCE_INLINE CxI64 micro() const {
			return (m_t.QuadPart * CX_MICRO_PER_SEC) / s_freq.QuadPart;
		}

		/** @return The value of the Time in milliseconds. */
		CX_FORCE_INLINE CxI64 milli() const {
			return (m_t.QuadPart * CX_MILLI_PER_SEC) / s_freq.QuadPart;
		}
		
		/** @return The value of the Time in nanoseconds. */
		CX_FORCE_INLINE CxI64 nano() const {
			return (m_t.QuadPart * CX_NANO_PER_SEC) / s_freq.QuadPart;
		}

		/** @return The RawTime backing this Time. */
		CX_FORCE_INLINE const LARGE_INTEGER & raw() const { return m_t; }		

		/**  @return The value of the Time in seconds (32bit fp). */
		CX_FORCE_INLINE CxF32 sec32() const {
			return (CxF32)((CxF64)m_t.QuadPart / (CxF64)s_freq.QuadPart);
		}

		/** @return The value of the Time in seconds (64bit fp). */
		CX_FORCE_INLINE CxF64 sec64() const {
			return (CxF64)m_t.QuadPart / (CxF64)s_freq.QuadPart;
		}

		/** @brief Set the value of the Time in microseconds. */
		CX_FORCE_INLINE void setMicro(CxI64 in_micro) {
			m_t.QuadPart = (in_micro * s_freq.QuadPart)  / CX_MICRO_PER_SEC;
		}

		/** @brief Set the value of the Time in milliseconds. */
		CX_FORCE_INLINE void setMilli(CxI64 in_milli) {
			m_t.QuadPart = (in_milli * s_freq.QuadPart) / CX_MILLI_PER_SEC;
		}

		/** @brief Set the value of the Time in nanoseconds. */
		CX_FORCE_INLINE void setNano(CxI64 in_nano) {
			m_t.QuadPart = (in_nano * s_freq.QuadPart) / CX_NANO_PER_SEC;
		}

		/** @brief Set the value of the Time in platform dependant units. */
		CX_FORCE_INLINE void setRaw(const LARGE_INTEGER & in_time) { m_t = in_time; }
		
		/** @brief Set the value of the Time in seconds (64bit fp). */
		CX_FORCE_INLINE void setSec(CxF64 in_seconds) {
		   m_t.QuadPart = (CxI64)(in_seconds * s_freq.QuadPart);
		}

		/** @brief Set the value of the Time to the current time. */
		CX_FORCE_INLINE void setToCurrentTime() { QueryPerformanceCounter(&m_t); }

		friend class CxTime;
		
	  protected:
	   LARGE_INTEGER m_t;
		static LARGE_INTEGER s_freq;
	};

} // namespace cat

#endif // CX_CORE_TIME_WINDOWS_CXWINDOWSABSTIME_H
