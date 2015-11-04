#ifndef CX_CORE_COMMON_CXFLAGS_H
#define CX_CORE_COMMON_CXFLAGS_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxFlags.h
 *
 * Templated class to hold typesafe OR's of enum flags.
 *
 * @author Catlin Zilinski
 * @date Sept. 30, 2015
 */

#include "core/Cx.h"

namespace cat {

	/**
	 * @class CxFlags CxFlags.h "core/common/CxFlags.h"
	 * @brief Templated class to hold typesafe combinations of enum flags.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Sept. 30, 2015
	 */
	template <typename E, typename T=CxU32>
	class CxFlags {
	  public:
		/** @brief Zero valued flags */
		CX_FORCE_INLINE CxFlags() : m_bits(0) {}

		/** @brief Create with single enum value */
		CX_FORCE_INLINE CxFlags(E in_val) : m_bits(e) {}

		/** @brief Create with storage value */
		explicit CX_FORCE_INLINE CxFlags(T in_val) : m_bits(in_val) {}

		CX_FORCE_INLINE CxFlags(const CxFlags<E,T> &in_src) : m_bits(in_src.m_bits) {}
		CX_FORCE_INLINE CxFlags<E,T> & operator=(E in_val) { m_bits = in_val;  return *this; }
		CX_FORCE_INLINE CxFlags<E,T> & operator=(const CxFlags<E,T> &in_src) {
			m_bits = in_src.m_bits;  return *this;
		}

		CX_FORCE_INLINE  operator CxU32() const { return static_cast<CxU32>(m_bits); }
		CX_FORCE_INLINE  operator CxI32() const { return static_cast<CxI32>(m_bits); }
		
		CX_FORCE_INLINE CxBool operator==(E in_val) const { return m_bits == in_val; }
		CX_FORCE_INLINE CxBool operator==(const CxFlags<E,T> &in_f) const { return m_bits == in_f.m_bits; }
		CX_FORCE_INLINE CxBool operator!=(E in_val) const { return m_bits != in_val; }
		CX_FORCE_INLINE CxBool operator!=(const CxFlags<E,T> &in_f) const { return m_bits != in_f.m_bits; }

		CX_FORCE_INLINE CxFlags<E,T> operator|(E in_val) const { return CxFlags<E,T>((T)(m_bits | in_val)); }
		CX_FORCE_INLINE CxFlags<E,T> operator&(E in_val) const { return CxFlags<E,T>((T)(m_bits & in_val)); }
		CX_FORCE_INLINE CxFlags<E,T> operator^(E in_val) const { return CxFlags<E,T>((T)(m_bits ^ in_val)); }

		CX_FORCE_INLINE CxFlags<E,T> & operator|=(E in_val) { m_bits |= in_val;  return *this; }
		CX_FORCE_INLINE CxFlags<E,T> & operator&=(E in_val) { m_bits &= in_val;  return *this; }
		CX_FORCE_INLINE CxFlags<E,T> & operator^=(E in_val) { m_bits ^= in_val;  return *this; }

		CX_FORCE_INLINE CxFlags<E,T> operator|(const CxFlags<E,T> &in_f) const { return CxFlags<E,T>(m_bits | in_f.m_bits); }
		CX_FORCE_INLINE CxFlags<E,T> operator&(const CxFlags<E,T> &in_f) const { return CxFlags<E,T>(m_bits & in_f.m_bits); }
		CX_FORCE_INLINE CxFlags<E,T> operator^(const CxFlags<E,T> &in_f) const { return CxFlags<E,T>(m_bits ^ in_f.m_bits); }

		CX_FORCE_INLINE CxFlags<E,T> & operator|=(const CxFlags<E,T> &in_f) { m_bits |= in_f.m_bits;  return *this; }
		CX_FORCE_INLINE CxFlags<E,T> & operator&=(const CxFlags<E,T> &in_f) { m_bits &= in_f.m_bits;  return *this; }
		CX_FORCE_INLINE CxFlags<E,T> & operator^=(const CxFlags<E,T> &in_f) { m_bits ^= in_f.m_bits;  return *this; }

		CX_FORCE_INLINE CxFlags<E,T> operator~() const { return CxFlags<E,T>(~m_bits); }

		/** @brief Clear all the flags (set to zero) */
		CX_FORCE_INLINE void clear() { m_bits = 0; }

		/** @brief Clear the specified flag */
		CX_FORCE_INLINE void clear(E in_val) { m_bits &= (~in_val); }

		/** @return True if the enum flag is set */
	  CX_FORCE_INLINE CxBool isSet(E in_val) const { return (m_bits & in_val) != 0; }

	  /** @return True if any of the flags in in_val are set */
	  CX_FORCE_INLINE CxBool isAnySet(T in_val) const { return (m_bits & in_val) != 0; }

	  /** @return True if any of the flags in in_val are set */
	  CX_FORCE_INLINE CxBool isAllSet(T in_val) const { return (m_bits & in_val) == in_val; }

		/** @brief Set the given enum flag */
		CX_FORCE_INLINE CxFlags<E,T> & set(E in_val) { m_bits |= in_val;  return *this; }

		friend CX_INLINE CxFlags<E,T> operator&(E in_val, const CxFlags<E,T> &in_f) {
			return CxFlags<E,T>((T)(in_val & in_f.m_bits));
		}
		friend CX_INLINE CxFlags<E,T> operator|(E in_val, const CxFlags<E,T> &in_f) {
			return CxFlags<E,T>((T)(in_val | in_f.m_bits));
		}
		friend CX_INLINE CxFlags<E,T> operator^(E in_val, const CxFlags<E,T> &in_f) {
			return CxFlags<E,T>((T)(in_val ^ in_f.m_bits));
		}

	  private:
		T m_bits;
	};

#define CX_FLAGS_OPS(ENUM,TYPE)														\
	CX_FORCE_INLINE CxFlags<ENUM,TYPE> operator|(ENUM e0, ENUM e1) { CxFlags<ENUM,TYPE>((TYPE)(e0 | e1));	} \
	CX_FORCE_INLINE CxFlags<ENUM,TYPE> operator&(ENUM e0, ENUM e1) { CxFlags<ENUM,TYPE>((TYPE)(e0 & e1));	} \
	CX_FORCE_INLINE CxFlags<ENUM,TYPE> operator^(ENUM e0, ENUM e1) { CxFlags<ENUM,TYPE>((TYPE)(e0 ^ e1));	} \
	CX_FORCE_INLINE CxFlags<ENUM,TYPE> operator~(ENUM e) { CxFlags<ENUM,TYPE>((TYPE)(~e)); }
	
#define CX_FLAGS(ENUM,FLAGS)							\
	typedef CxFlags<ENUM,CxU32> FLAGS;				\
	CX_FLAGS_OPS(ENUM,CxU32)

#define CX_FLAGS_EXT(ENUM,FLAGS,TYPE)				\
	typedef CxFlags<ENUM,TYPE> FLAGS;				\
	CX_FLAGS_OPS(ENUM,TYPE)
	
} // namespace cat

#endif // CX_CORE_COMMON_CXFLAGS_H
