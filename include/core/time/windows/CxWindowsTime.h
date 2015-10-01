#ifndef CX_CORE_TIME_WINDOWS_CXWINDOWSTIME_H
#define CX_CORE_TIME_WINDOWS_CXWINDOWSTIME_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxWindowsTime.h
 * @brief Contains the CxTime and CxAbsTime classes.
 * 
 * The CxAbsTime class can be the same as the CxTime
 * class depending on the platform, but on some platforms
 * using only positive time values (OSX) can increase 
 * the speed.
 *
 * @author Catlin Zilinski
 * @Date Sept 29, 2015
 */

#include "core/Cx.h"
#include "core/time/internal/CxTimeDefs.h"
#include "core/time/CxAbsTime.h"

namespace cat {

	/**
	 * @class CxTime CxWindowsTime.h "core/time/windows/CxWindowsTime.h"
	 * @brief A simple Time value class.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept. 29, 2015
	 */
	class CxTime : public CxAbsTime {
	  public:
		/** @brief Create a zero CxTime value */		
		CX_FORCE_INLINE CxTime() : CxAbsTime() {}
		
		/** @brief Create a new Time from the specified windows time. */
		CX_FORCE_INLINE CxTime(const LARGE_INTEGER &in_t) : CxAbsTime(in_t) {}

		/** @brief Default copy constructor. */
		CX_FORCE_INLINE CxTime(const CxTime &in_src) : CxAbsTime(in_src) {}

		/** @brief Create from an CxAbsTime */
		CX_FORCE_INLINE CxTime(const CxAbsTime &in_src) : CxAbsTime(in_src) {}

		/** @brief Default assignment operator. */
		CX_FORCE_INLINE CxTime& operator=(const CxTime& in_src) {
			m_t = in_src.m_t;  return *this;
		}

		/** @brief Assign from CxAbsTime */
		CX_FORCE_INLINE CxTime& operator=(const CxAbsTime& in_src) {
			m_t = in_src.m_t;  return *this;
		}
		
		/** @return True if the two time vals are equal. */
		CX_FORCE_INLINE CxBool operator==(const CxTime& in_t) const {
			return CxAbsTime::operator==(in_t);
		}

		/**  @Return True if the two time vals are NOT equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxTime& in_t) const {
		   return CxAbsTime::operator!=(in_t);
		}

		/** @return True if lhs < rhs. */
		CX_FORCE_INLINE CxBool operator<(const CxTime& in_rhs) const {
		   return CxAbsTime::operator<(in_rhs);
		}

		/** @return True if lhs <= rhs. */
		CX_FORCE_INLINE CxBool operator<=(const CxTime& in_rhs) const {
			return CxAbsTime::operator<=(in_rhs);
		}

	   /** @return True if lhs > rhs. */
		CX_FORCE_INLINE CxBool operator>(const CxTime& in_rhs) const {
			return CxAbsTime::operator>(in_rhs);
		}
		
	   /** @return True if lhs >= rhs. */
		CX_FORCE_INLINE CxBool operator>=(const CxTime& in_rhs) const {
			return CxAbsTime::operator>=(in_rhs);
		}

		/** @return A new time value that is the sum of lhs + rhs */
		CX_FORCE_INLINE CxTime operator+(const CxTime& in_rhs) const {
			CxTime t(*this);  t += in_rhs;  return t;
		}

		/** @return A new Time that is the difference of this - in_rhs. */
		CX_FORCE_INLINE CxTime operator-(const CxTime& in_rhs) const {
			CxTime t(*this);  t -= in_rhs;  return t;
		}

		/** @return This time after adding the rhs to it. */
		CX_FORCE_INLINE CxTime& operator+=(const CxTime& in_rhs) {
		   m_t.QuadPart += in_rhs.m_t.QuadPart;  return *this;
		}

		/** @return This time after subtracting the rhs from it. */
		CX_FORCE_INLINE CxTime& operator-=(const CxTime& in_rhs) {
		   m_t.QuadPart -= in_rhs.m_t.QuadPart;  return *this;			
		}

		/** @return The current time value using the system clock. */
		CX_FORCE_INLINE static CxTime current() {
		   LARGE_INTEGER t;  QueryPerformanceCounter(&t);
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
			return CxAbsTime::currentSec32();
		}

		/** @return The current time in seconds (64bit). */
		CX_FORCE_INLINE static CxF64 currentSec64() {
			return CxAbsTime::currentSec64();
		}

		/** @return A new CxTime value from a microsecond value */
		static CX_FORCE_INLINE CxTime fromMicro(CxI64 in_micro) {
			LARGE_INTEGER t;
			t.QuadPart = (in_micro * CxAbsTime::s_freq.QuadPart)  / CX_MICRO_PER_SEC;
			return CxTime(t);
		}

		/** @return A new CxTime value from a millisecond value */
		static CX_FORCE_INLINE CxTime fromMilli(CxI64 in_milli) {
			LARGE_INTEGER t;
			t.QuadPart = (in_milli * CxAbsTime::s_freq.QuadPart)  / CX_MILLI_PER_SEC;
			return CxTime(t);
		}

		/** @return A new CxTime value from a nanosecond value */
		static CX_FORCE_INLINE CxTime fromNano(CxI64 in_nano) {
			LARGE_INTEGER t;
			t.QuadPart = (in_nano * CxAbsTime::s_freq.QuadPart)  / CX_NANO_PER_SEC;
			return CxTime(t);
		}

		/** @return A new CxTime value from a seconds value */
		static CX_FORCE_INLINE CxTime fromSec(CxF64 in_seconds) {
			LARGE_INTEGER t;
			t.QuadPart = (CxI64)(in_seconds * CxAbsTime::s_freq.QuadPart);
			return CxTime(t);
		}
		
		/** @return The value of the Time in microseconds. */
		CX_FORCE_INLINE CxI64 micro() const {
			return (m_t.QuadPart * CX_MICRO_PER_SEC) / CxAbsTime::s_freq.QuadPart;
		}

		/** @return The value of the Time in milliseconds. */
		CX_FORCE_INLINE CxI64 milli() const {
			return (m_t.QuadPart * CX_MILLI_PER_SEC) / CxAbsTime::s_freq.QuadPart;
		}
		
		/** @return The value of the Time in nanoseconds. */
		CX_FORCE_INLINE CxI64 nano() const {
			return (m_t.QuadPart * CX_NANO_PER_SEC) / CxAbsTime::s_freq.QuadPart;
		}
	};

} // namespace cat

#endif // CX_CORE_TIME_WINDOWS_CXWINDOWSTIME_H
