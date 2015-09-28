#ifndef CX_CORE_COMMON_CXARGS_H
#define CX_CORE_COMMON_CXARGS_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxArgs.h
 * @brief Contains the Args class to storing and retrieving arguments.
 *
 *
 * @author Catlin Zilinski
 * @date Sept 4, 2015
 */

#include "core/Cx.h"

namespace cat {

	/**
	 * @class CxArgs CxArgs.h "core/common/CxArgs.h"
	 * @brief The base Args class for storing / retreiving arguments.
	 * 
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Mar 11, 2015
	 */
	class CxArgs {
	  public:
		CX_FORCE_INLINE CxArgs() : mp_data(0) {}

		/**
		 * @brief Array index operator to retreive an argument.
		 * @param in_idx The index of the argument to retrieve.
		 * @return A reference to the argument at the given index.
		 */
		CX_FORCE_INLINE CxCoreType & operator[](CxI32 in_idx) { return mp_data[in_idx]; }
		
		/**
		 * @brief Array index operator to retreive an argument.
		 * @param in_idx The index of the argument to retrieve.
		 * @return A const reference to the argument at the given index.
		 */
		CX_FORCE_INLINE const CxCoreType & operator[](const CxI32 in_idx) const {
			return mp_data[in_idx];
		}
		
	  protected:
		CxCoreType* mp_data;
	};

	template <typename T0>
	class CxArgs1 : public CxArgs {
	  public:
		CX_FORCE_INLINE CxArgs1() : CxArgs() { mp_data = &m_arg0;  m_arg0.u64 = 0; }
		CX_FORCE_INLINE CxArgs1(T0 in_a0) : CxArgs() { mp_data = &m_arg0;  CxAssign(m_arg0, in_a0); }
		
	  protected:
		CxCoreType m_arg0;
	};

	template <typename T0, typename T1>
	class CxArgs2 : public CxArgs {
	  public:
		CX_FORCE_INLINE CxArgs2() : CxArgs() { mp_data = &m_arg0; m_arg0.u64 = m_arg1.u64 = 0; }
		CX_FORCE_INLINE CxArgs2(T0 in_a0, T1 in_a1) : CxArgs() {
			mp_data = &m_arg0;
			CxAssign(m_arg0, in_a0);
			CxAssign(m_arg1, in_a1);
		}
	  protected:
		CxCoreType m_arg0, m_arg1;
	};

	template <typename T0, typename T1, typename T2>
	class CxArgs3 : public CxArgs {
	  public:
		CX_FORCE_INLINE CxArgs3() : CxArgs() {
			mp_data = &m_arg0; m_arg0.u64 = m_arg1.u64 = m_arg2.u64 = 0;
		}
		CX_FORCE_INLINE CxArgs3(T0 in_a0, T1 in_a1, T2 in_a2) : CxArgs() {
			mp_data = &m_arg0;
			CxAssign(m_arg0, in_a0);
			CxAssign(m_arg1, in_a1);
			CxAssign(m_arg2, in_a2);
		}
	  protected:
		CxCoreType m_arg0, m_arg1, m_arg2;
	};

	template <typename T0, typename T1, typename T2, typename T3>
	class CxArgs4 : public CxArgs {
	  public:
		CX_FORCE_INLINE CxArgs4() : CxArgs() {
			mp_data = &m_arg0; m_arg0.u64 = m_arg1.u64 = m_arg2.u64 = m_arg3.u64 = 0;
		}
		CX_FORCE_INLINE CxArgs4(T0 in_a0, T1 in_a1, T2 in_a2, T3 in_a3) : CxArgs() {
			mp_data = &m_arg0;
			CxAssign(m_arg0, in_a0);
			CxAssign(m_arg1, in_a1);
			CxAssign(m_arg2, in_a2);
			CxAssign(m_arg3, in_a3);
		}
	  protected:
		CxCoreType m_arg0, m_arg1, m_arg2, m_arg3;
	};
	
	template <int N>
	class CxArgsN : public CxArgs {
	  public:
		CX_FORCE_INLINE CxArgsN() : CxArgs() { mp_data = m_data; }

	  protected:
		CxCoreType m_data[N];
	};
	
} // namespace Cat

#endif // CX_CORE_COMMON_CXARGS_H
