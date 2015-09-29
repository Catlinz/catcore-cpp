#ifndef CX_CORE_TIME_APPLE_CXAPPLEABSTIME_H
#define CX_CORE_TIME_APPLE_CXAPPLEABSTIME_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxAppleAbsTime.h
 * @brief Contains the definition for the Apple version of the AbsTime class.
 *
 * @author Catlin Zilinski
 * @date Sept 29, 2015
 */

#include "core/Cx.h"
#include "core/time/internal/CxTimeDefs.h"

namespace cat {

	/**
	 * @class CxAbsTime CxAppleAbsTime.h "core/time/apple/CxAppleAbsTime.h"
	 * @brief A simple class to hold a POSITIVE Time value.
	 *
	 * @author Catlin Zilinski
	 * @version 4
	 * @since Mar 7, 2015
	 */
	class CxAbsTime {
	public:
		/** @brief Create an empty CxAbsTime */		
		Cx_FORCE_INLINE CxAbsTime() : m_t(0) {}

		/** @brief Create a new CxAbsTime from the specified osx time. */
		CX_FORCE_INLINE CxAbsTime(uint64_t in_time) : m_t(in_time) {}

		/** @brief Default copy constructor */
		CX_FORCE_INLINE CxAbsTime(const CxAbsTime &in_src) : m_t(in_src.m_t) {}

		/** @brief Assignment operator */
		CX_FORCE_INLINE CxAbsTime & operator=(const CxAbsTime &in_src) {
			m_t = in_src.m_t;  return *this;
		}

		/** @return True if the two time vals are equal. */
		CX_FORCE_INLINE CxBool operator==(const CxAbsTime& in_rval) const {
			return m_t == in_rval.m_t;
		}

		/**
		 * @return True if the two time vals are NOT equal.
		 */
		CX_FORCE_INLINE CxBool operator!=(const CxAbsTime& in_rval) const {
			return m_t != in_rval.m_t;
		}

		/**
		 * @param in_rhs The CxAbsTime to compare to.
		 * @return True if this is less than in_rhs.
		 */
		CX_FORCE_INLINE CxBool operator<(const CxAbsTime& in_rhs) const {
			return m_t < in_rhs.m_t;
		}

		/**
		 * @param in_rhs The CxAbsTime to compare to.
		 * @return True if this is less than or equal to in_rhs.
		 */
		CX_FORCE_INLINE CxBool operator<=(const CxAbsTime& in_rhs) const {
			return m_t <= in_rhs.m_t;
		}

		/**
		 * @param in_rhs The CxAbsTime to compare to.
		 * @return True if this is greater than in_rhs.
		 */
		CX_FORCE_INLINE CxBool operator>(const CxAbsTime& in_rhs) const {
			return m_t > in_rhs.m_t;
		}
		
		/**
		 * @param in_rhs The CxAbsTime to compare to.
		 * @return True if this is greater than or equal to in_rhs.
		 */
		CX_FORCE_INLINE CxBool operator>=(const CxAbsTime& in_rhs) const {
			return m_t >= in_rhs.m_t;
		}

		/**
		 * @param in_rhs The CxAbsTime to add to this one.
		 * @return A new CxAbsTime that is the sum of this + in_rhs.
		 */
		CX_FORCE_INLINE CxAbsTime operator+(const CxAbsTime& in_rhs) const {
			return CxAbsTime(m_t + in_rhs.m_t);
		}

		/**
		 * @param in_rhs The CxAbsTime to subtract from this one.
		 * @return A new CxAbsTime that is the difference of this - in_rhs.
		 */
		CX_FORCE_INLINE CxAbsTime operator-(const CxAbsTime& in_rhs) const {
			return CxAbsTime(m_t - in_rhs.m_t);
		}

		/**
		 * @param in_rhs The CxAbsTime to add to this one.
		 * @return A reference to this CxAbsTime.
		 */
		CX_FORCE_INLINE CxAbsTime& operator+=(const CxAbsTime& in_rhs) {
		   m_t += in_rhs.m_t;  return *this;			
		}

		/**
		 * @param in_rhs The CxAbsTime to subtract from this one.
		 * @return A reference to this CxAbsTime.
		 */
		CX_FORCE_INLINE CxAbsTime& operator-=(const CxAbsTime& in_rhs) {
			m_t -= in_rhs.m_t;  return *this;			
		}

		/* ###################################################
		 * The methods for converting to and from the mach times.
		 * ################################################### */
		static CX_FORCE_INLINE uint64_t nanoToRaw(CxU64 in_nano) {
			return (uint64_t)((in_nano * sx_timebaseInfo.denom) / sx_timebaseInfo.numer);
		}
		static CX_FORCE_INLINE uint64_t microToRaw(CxU64 in_micro) {
			 return (uint64_t)((in_micro * CX_NANO_PER_MICRO * sx_timebaseInfo.denom) / sx_timebaseInfo.numer);
		}
		static CX_FORCE_INLINE uint64_t milliToRaw(CxU64 in_milli) {
			return (uint64_t)((in_milli * CX_NANO_PER_MILLI * sx_timebaseInfo.denom) / sx_timebaseInfo.numer);
		}
		static CX_FORCE_INLINE uint64_t secToRaw(CxF64 in_seconds) {
			return (((uint64_t)(in_seconds * CX_NANO_PER_SEC) * m_tBaseInfo.numer) / m_tBaseInfo.denom);
		}
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

		/**
		 * @return An CxAbsTime with the current time in it.
		 */
		CX_FORCE_INLINE static CxAbsTime current() {
			return CxAbsTime(mach_absolute_time());
		}

		/**
		 * @return The current time in microseconds.
		 */
		CX_FORCE_INLINE static CxU64 currentMicro() {
			return rawToMicro(mach_absolute_time());
		}

		/**
		 * @return The current time in milliseconds.
		 */
		CX_FORCE_INLINE static CxU64 currentMilli() {
			return rawToMilli(mach_absolute_time());
		}

		/**
		 * @return The current time in nanoseconds.
		 */
		CX_FORCE_INLINE static CxU64 currentNano() {
			return rawToNano(mach_absolute_time());
		}

		/**
		 * @return The current time in seconds (32bit).
		 */
		CX_FORCE_INLINE static CxF32 currentSec32() {
			return rawToSec32(mach_absolute_time());
		}

		/**
		 * @return The current time in seconds (64bit).
		 */
		CX_FORCE_INLINE static CxF64 currentSec64() {
			return rawToSec64(mach_absolute_time());
		}

		/**
		 * @return The fraction of seconds in microseconds.
		 */
		CX_FORCE_INLINE CxU32 fracMicro() const {
			const CxU64 n = nano();
			const CxU64 sec = n / CX_NANO_PER_SEC;
			return (n - (sec * CX_NANO_PER_SEC)) / CX_NANO_PER_MICRO;
		}

		/**
		 * @return The fraction of seconds in milliseconds.
		 */
		CX_FORCE_INLINE CxU32 fracMilli() const {
			const CxU64 n = nano();
			const CxU64 sec = n / CX_NANO_PER_SEC;
			return (n - (sec * CX_NANO_PER_SEC)) / CX_NANO_PER_MILLI;
		}

		/**
		 * @return The fraction of seconds in nanoseconds.
		 */
		CX_FORCE_INLINE CxU32 fracNano() const {
			const CxU64 n = nano();
			const CxU64 sec = n / CX_NANO_PER_SEC;
			return (n - (sec * CX_NANO_PER_SEC));
		}

		/** @return The fraction of seconds in seconds (64 bit). */
		CX_FORCE_INLINE CxF64 fracSec64() const {
			const CxU64 n = nano();
			const CxU64 sec = n / CX_NANO_PER_SEC;
			return ((CxF64)(n - (sec * CX_NANO_PER_SEC))) * CX_SEC_PER_NANO;
		}

		/** @return The fraction of seconds in seconds (32 bit). */
		CX_FORCE_INLINE CxF32 fracSec32() const { return (CxF32)fracSec64(); }
		
		/** @return The value of the CxAbsTime in microseconds. */
		CX_FORCE_INLINE CxU64 micro() const { return rawToMicro(m_t); }

		/** @return The value of the CxAbsTime in milliseconds. */
		CX_FORCE_INLINE CxU64 milli() const { return rawToMilli(m_t); }
		
		/** @return The value of the CxAbsTime in nanoseconds. */
		CX_FORCE_INLINE CxU64 nano() const { return rawToNano(m_t); }

		/** @return The RawCxAbsTimeue backing this CxAbsTimeue. */
		CX_FORCE_INLINE uint64_t raw() const { return m_t; }		

		/** @return The value of the CxAbsTime in seconds (32bit fp). */
		CX_FORCE_INLINE CxF32 sec32() const { return rawToSec32(m_t); }

		/** @return The value of the CxAbsTime in seconds (64bit fp). */
		CX_FORCE_INLINE CxF64 sec64() const { return rawToSec64(m_t); }

		/** @brief Set the time value in microseconds */
		CX_FORCE_INLINE void setMicro(CxU64 in_micro) { m_t = microToRaw(in_micro); }

		/** @brief Set the time value in milliseconds */
		CX_FORCE_INLINE void setMilli(CxU64 in_milli) {	m_t = milliToRaw(in_milli); }

		/** @brief Set the time value in nanoseconds. */
		CX_FORCE_INLINE void setNano(CxU64 in_nano) { m_t = nanoToRaw(in_nano); }

		/** @brief Set the value of the CxAbsTime in platform dependant units. */
		CX_FORCE_INLINE void setRaw(uint64_t in_time) {	m_t = in_time; }
		
		/** @brief Set the value of the CxAbsTime in seconds (64bit fp). */
		CX_FORCE_INLINE void setSec(CxF64 in_seconds) { m_t = secToRaw(in_seconds); }

		/** @brief Set the value of the CxAbsTime to the current time. */
		CX_FORCE_INLINE void setToCurrentTime() { m_t = mach_absolute_time(); }

	  private:
		uint64_t m_t;
		static mach_timebase_info_data_t sx_timebaseInfo;
	};

} // namespace cat
		
#endif // CAT_CORE_TIME_OSX_ABSTIME_H
