#ifndef CX_CORE_TIME_APPLE_CXAPPLETIME_H
#define CX_CORE_TIME_APPLE_CXAPPLETIME_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxAppleTime.h
 * @brief Contains the CxTime class.
 * 
 *
 * @author Catlin Zilinski
 * @date Sept 30, 2015
 */

#include "core/Cx.h"
#include "core/time/internal/CxTimeDefs.h"

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
		CX_FORCE_INLINE CxTime() : m_t(0) {}
		
		/** @brief Create a new Time from the specified unix time. */
		CX_FORCE_INLINE CxTime(CxI64 in_t) : m_t(in_t) {}

		/** @brief Default copy constructor. */
		CX_FORCE_INLINE CxTime(const CxTime &in_src) : m_t(in_src.m_t) {}

		/** @brief Default assignment operator. */
		CX_FORCE_INLINE CxTime& operator=(const CxTime& in_src) {
			m_t = in_src.m_t;  return *this;
		}
		
		/** @return True if the two time vals are equal. */
		CX_FORCE_INLINE CxBool operator==(const CxTime& in_t) const {
			return m_t == in_t.m_t;
		}

		/**  @return True if the two time vals are NOT equal. */
		CX_FORCE_INLINE CxBool operator!=(const CxTime& in_t) const {
			return m_t != in_t.m_t;
		}

		/** @return True if lhs < rhs. */
		CX_FORCE_INLINE CxBool operator<(const CxTime& in_rhs) const {
			return m_t < in_rhs.m_t;
		}

		/** @return True if lhs <= rhs. */
		CX_FORCE_INLINE CxBool operator<=(const CxTime& in_rhs) const {
			return m_t <= in_rhs.m_t;
		}

	   /** @return True if lhs > rhs. */
		CX_FORCE_INLINE CxBool operator>(const CxTime& in_rhs) const {
			return m_t > in_rhs.m_t;
		}
		
	   /** @return True if lhs >= rhs. */
		CX_FORCE_INLINE CxBool operator>=(const CxTime& in_rhs) const {
			return m_t >= in_rhs.m_t;
		}

		/** @return A new time value that is the sum of lhs + rhs */
		CX_FORCE_INLINE CxTime operator+(const CxTime& in_rhs) const {
		   return CxTime(m_t + in_rhs.m_t);
		}

		/** @return A new Time that is the difference of this - in_rhs. */
		CX_FORCE_INLINE CxTime operator-(const CxTime& in_rhs) const {
		   return CxTime(m_t - in_rhs.m_t);
		}

		/** @return This time after adding the rhs to it. */
		CX_FORCE_INLINE CxTime& operator+=(const CxTime& in_rhs) {
			m_t += in_rhs.m_t;  return *this;
		}

		/** @return This time after subtracting the rhs from it. */
		CX_FORCE_INLINE CxTime& operator-=(const CxTime& in_rhs) {
		   m_t -= in_rhs.m_t;  return *this;
		}

		/* ###################################################
		 * The methods for converting from the mach times.
		 * ################################################### */
		static CX_FORCE_INLINE CxU64 rawToNano(uint64_t in_raw) {
			return (CxU64)((in_raw * sx_timebaseInfo.numer) / sx_timebaseInfo.denom);
		}
		static CX_FORCE_INLINE CxU64 rawToMicro(uint64_t in_raw) {
			return (CxU64)((in_raw * sx_timebaseInfo.numer) / (sx_timebaseInfo.denom * CX_NANO_PER_MICRO));
		}
		static CX_FORCE_INLINE CxU64 rawToMilli(uint64_t in_raw) {
			return (CxU64)((in_raw * sx_timebaseInfo.numer) / sx_timebaseInfo.denom) / CX_NANO_PER_MILLI;
		}
		static CX_FORCE_INLINE CxF32 rawToSec32(uint64_t in_raw) {
			return (CxF32)((CxF64)((CxU64)((in_raw * sx_timebaseInfo.numer) / sx_timebaseInfo.denom)) * SEC_PER_NANO);
		}
		static CX_FORCE_INLINE CxF64 rawToSec64(uint64_t in_raw) {
			return (CxF64)((CxU64)((in_raw * sx_timebaseInfo.numer) / sx_timebaseInfo.denom)) * SEC_PER_NANO;
		}

		/** @return The current time value using the system clock. */
		CX_FORCE_INLINE static CxTime current() {
			return CxTime((CxI64)rawToNano(mach_absolute_time()));
		}

		/** @return The current time in microseconds. */
		CX_FORCE_INLINE static CxU64 currentMicro() {
			return rawToMicro(mach_absolute_time());
		}

		/** @return The current time in milliseconds. */
		CX_FORCE_INLINE static CxU64 currentMilli() {
			return rawToMilli(mach_absolute_time());
		}

		/** @return The current time in nanoseconds. */
		CX_FORCE_INLINE static CxU64 currentNano() {
			return rawToNano(mach_absolute_time());
		}

		/** @return The current time in seconds (32bit). */
		CX_FORCE_INLINE static CxF32 currentSec32() {
			return rawToSec32(mach_absolute_time());
		}

		/** @return The current time in seconds (64bit). */
		CX_FORCE_INLINE static CxF64 currentSec64() {
			return rawToSec64(mach_absolute_time());
		}

		/** @return The fraction of seconds in microseconds. */
		CX_FORCE_INLINE CxI32 fracMicro() const {
			const CxI64 sec = m_t / CX_NANO_PER_SEC;
			return (m_t - (sec * CX_NANO_PER_SEC)) / CX_NANO_PER_MICRO;
		}

		/** @return The fraction of seconds in milliseconds. */
		CX_FORCE_INLINE CxI32 fracMilli() const {
			const CxI64 sec = m_t / CX_NANO_PER_SEC;
			return (m_t - (sec * CX_NANO_PER_SEC)) / CX_NANO_PER_MILLI;
		}

		/** @return The fraction of seconds in nanoseconds. */
		CX_FORCE_INLINE CxI32 fracNano() const {
			const CxI64 sec = m_t / CX_NANO_PER_SEC;
			return (m_t - (sec * CX_NANO_PER_SEC));
		}

		/** @return The fraction of seconds in seconds (64 bit). */
		CX_FORCE_INLINE CxF64 fracSec64() const {
			const CxI64 sec = m_t / CX_NANO_PER_SEC;
			return (CxF64)(m_t - (sec * CX_NANO_PER_SEC)) * CX_SEC_PER_NANO;
		}

		/** @return The fraction of seconds in seconds (32 bit). */
		CX_FORCE_INLINE CxF32 fracSec32() const { return (CxF32)fracSec64(); }

	   	/** @return A new time value from microseconds */
		static CX_FORCE_INLINE CxTime fromMicro(CxI64 in_micro) {
			return CxTime(in_micro * CX_NANO_PER_MICRO);
		}

		/** @return A new time value from milliseconds */
		static CX_FORCE_INLINE CxTime fromMilli(CxI64 in_milli) {
			return CxTime(in_milli * CX_NANO_PER_MILLI);
		}

		/** @return A new time value from nanoseconds */
		static CX_FORCE_INLINE CxTime fromNano(CxI64 in_nano) {
			return CxTime(in_nano);
		}

		/** @return A new time value from seconds */
		static CX_FORCE_INLINE CxTime fromSec(CxF64 in_sec) {
		   return CxTime((CxI64)(in_sec * CX_NANO_PER_SEC));
		}

		/** @return The value of the Time in microseconds. */
		CX_FORCE_INLINE CxI64 micro() const { return m_t / CX_NANO_PER_MICRO; }

		/** @return The value of the Time in milliseconds. */
		CX_FORCE_INLINE CxI64 milli() const { return m_t / CX_NANO_PER_MILLI; }
		
		/** @return The value of the Time in nanoseconds. */
		CX_FORCE_INLINE CxI64 nano() const { return m_t; }

		/** @return The RawTime backing this Time. */
		CX_FORCE_INLINE CxI64 raw() const { return m_t; }		

		/**  @return The value of the Time in seconds (32bit fp). */
		CX_FORCE_INLINE CxF32 sec32() const { return (CxF32)((CxF64)m_t * CX_SEC_PER_NANO); }

		/** @return The value of the Time in seconds (64bit fp). */
		CX_FORCE_INLINE CxF64 sec64() const { return (CxF64)m_t * CX_SEC_PER_NANO; }

		/** @brief Set the value of the Time in microseconds. */
		CX_FORCE_INLINE void setMicro(CxI64 in_micro) { m_t = in_micro * CX_NANO_PER_MICRO; }

		/** @brief Set the value of the Time in milliseconds. */
		CX_FORCE_INLINE void setMilli(CxI64 in_milli) { m_t = in_milli * CX_NANO_PER_MILLI; }

		/** @brief Set the value of the Time in nanoseconds. */
		CX_FORCE_INLINE void setNano(CxI64 in_nano) { m_t = in_nano }

		/** @brief Set the value of the Time in platform dependant units. */
		CX_FORCE_INLINE void setRaw(CxI64 in_time) { m_t = in_time; }
		
		/** @brief Set the value of the Time in seconds (64bit fp). */
		CX_FORCE_INLINE void setSec(CxF64 in_seconds) {
			m_t = (CxI64)(in_seconds * CX_NANO_PER_SEC);
		}

		/** @brief Set the value of the Time to the current time. */
		CX_FORCE_INLINE void setToCurrentTime() {
			m_t = (CxI64)rawToNano(mach_absolute_time());
		}

	  private:
		CxI64 m_t;
		static mach_timebase_info_data_t sx_timebaseInfo;
	};

} // namespace cat

#endif // CX_CORE_TIME_APPLE_CXAPPLETIME_H
