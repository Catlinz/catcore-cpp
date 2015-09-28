#ifndef CX_CORE_COMMON_CXMSG_H
#define CX_CORE_COMMON_CXMSG_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxMsg.h: 
 * @brief Contains message class templates for message passing.
 *
 * @author Catlin Zilinski
 * @date Sept 28, 2015
 */

#include "core/Cx.h"

namespace cat {

	/**
	 * @class CxMsg CxMsg.h "core/common/CxMsg.h"
	 * @brief Base message class, has only a type and no arguments.
	 * 
	 * @since Aug 16, 2015
	 * @version 1
	 * @author Catlin Zilinski
	 */
	class CxMsg {
	  public:
		CX_FORCE_INLINE CxMsg(CxI32 in_id = 0) : m_id(in_id) {}
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator==(const CxMsg &in_m) const { return m_id == in_m.m_id; }
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator!=(const CxMsg &in_m) const { return m_id != in_m.m_id; }
		/** @return The message ID for the message. */
		CX_FORCE_INLINE CxI32 id() const { return m_id; }		
	  protected:
		CxI32 m_id;
	};

	/**
	 * @class CxMsg1 CxMsg.h "core/common/CxMsg.h"
	 * @brief A message class with one argument.
	 * 
	 * @since Aug 16, 2015
	 * @version 2
	 * @author Catlin Zilinski
	 */
	template <typename T0>
	class CxMsg1 {
	  public:
		/** @brief Empty message */
		CX_FORCE_INLINE CxMsg1(CxI32 in_id = 0) : m_id(in_id) {}
		/** @brief Message with specified id type and argument */
		CX_FORCE_INLINE CxMsg1(CxI32 in_id, const T0 &in_a0) : m_id(in_id), m_a0(in_a0) {}
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator==(const CxMsg1 &in_m) const {
			return (m_id == in_m.m_id && m_a0 == in_m.m_a0);
		}
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator!=(const CxMsg &in_m) const {
			return (m_id != in_m.m_id || m_a0 != in_m.m_a0);
		}
		CX_FORCE_INLINE const T0 & arg0() const { return m_a0; }
		CX_FORCE_INLINE CxI32 id() const { return m_id; }
	  protected:
		CxI32 m_id;
		T0 m_a0;
	};
	
	/**
	 * @class CxMsg2 CxMsg.h "core/common/CxMsg.h"
	 * @brief A message class with two arguments.
	 * 
	 * @since Aug 16, 2015
	 * @version 2
	 * @author Catlin Zilinski
	 */
	template <typename T0, typename T1>
	class CxMsg2 {
	  public:
		/** @brief Empty message */
		CX_FORCE_INLINE CxMsg2(CxI32 in_id = 0) : m_id(in_id) {}
		/** @brief Message with specified id type and argument */
		CX_FORCE_INLINE CxMsg2(CxI32 in_id, const T0 &in_a0, const T1 &in_a1)
			: m_id(in_id), m_a0(in_a0), m_a1(in_a1) {}
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator==(const CxMsg2 &in_m) const {
			return (m_id == in_m.m_id && m_a0 == in_m.m_a0 && m_a1 == in_m.m_a1);
		}
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator!=(const CxMsg &in_m) const {
			return (m_id != in_m.m_id || m_a0 != in_m.m_a0 || m_a1 != in_m.m_a1);
		}
		CX_FORCE_INLINE const T0 & arg0() const { return m_a0; }
		CX_FORCE_INLINE const T1 & arg1() const { return m_a1; }
		CX_FORCE_INLINE CxI32 id() const { return m_id; }
	  protected:
		CxI32 m_id;
		T0 m_a0;
		T1 m_a1;
	};

	/**
	 * @class CxMsg3 CxMsg.h "core/common/CxMsg.h"
	 * @brief A message class with two arguments.
	 * 
	 * @since Aug 16, 2015
	 * @version 1
	 * @author Catlin Zilinski
	 */
	template <typename T0, typename T1, typename T2>
	class CxMsg3 {
	  public:
		/** @brief Empty message */
		CX_FORCE_INLINE CxMsg3(CxI32 in_id = 0) : m_id(in_id) {}
		/** @brief Message with specified id type and argument */
		CX_FORCE_INLINE CxMsg3(CxI32 in_id, const T0 &in_a0, const T1 &in_a1, const T2 &in_a2)
			: m_id(in_id), m_a0(in_a0), m_a1(in_a1), m_a2(in_a2) {}
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator==(const CxMsg3 &in_m) const {
			return (m_id == in_m.m_id && m_a0 == in_m.m_a0 && m_a1 == in_m.m_a1 &&
					  m_a2 == in_m.m_a2);
		}
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator!=(const CxMsg &in_m) const {
			return (m_id != in_m.m_id || m_a0 != in_m.m_a0 || m_a1 != in_m.m_a1 ||
					  m_a2 != in_m.m_a2);
		}
		CX_FORCE_INLINE const T0 & arg0() const { return m_a0; }
		CX_FORCE_INLINE const T1 & arg1() const { return m_a1; }
		CX_FORCE_INLINE const T2 & arg2() const { return m_a2; }
		CX_FORCE_INLINE CxI32 id() const { return m_id; }
	  protected:
		CxI32 m_id;
		T0 m_a0;
		T1 m_a1;
		T2 m_a2;
	};


	/**
	 * @class CxMsg4 CxMsg.h "core/common/CxMsg.h"
	 * @brief A message class with two arguments.
	 * 
	 * @since Aug 16, 2015
	 * @version 1
	 * @author Catlin Zilinski
	 */
	template <typename T0, typename T1, typename T2, typename T3>
	class CxMsg4 {
	  public:
		/** @brief Empty message */
		CX_FORCE_INLINE CxMsg4(CxI32 in_id = 0) : m_id(in_id) {}
		/** @brief Message with specified id type and argument */
		CX_FORCE_INLINE CxMsg4(CxI32 in_id, const T0 &in_a0, const T1 &in_a1,
									  const T2 &in_a2, const T3 &in_a3)
			: m_id(in_id), m_a0(in_a0), m_a1(in_a1), m_a2(in_a2), m_a3(in_a3) {}
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator==(const CxMsg4 &in_m) const {
			return (m_id == in_m.m_id && m_a0 == in_m.m_a0 && m_a1 == in_m.m_a1 &&
					  m_a2 == in_m.m_a2 && m_a3 == in_m.m_a3);
		}
		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator!=(const CxMsg &in_m) const {
			return (m_id != in_m.m_id || m_a0 != in_m.m_a0 || m_a1 != in_m.m_a1 ||
					  m_a2 != in_m.m_a2 || m_a3 != in_m.m_a3);
		}
		CX_FORCE_INLINE const T0 & arg0() const { return m_a0; }
		CX_FORCE_INLINE const T1 & arg1() const { return m_a1; }
		CX_FORCE_INLINE const T2 & arg2() const { return m_a2; }
		CX_FORCE_INLINE const T3 & arg3() const { return m_a3; }
		CX_FORCE_INLINE CxI32 id() const { return m_id; }
	  protected:
		CxI32 m_id;
		T0 m_a0;
		T1 m_a1;
		T2 m_a2;
		T3 m_a3;
	};
} // namespace cat

#endif // CX_CORE_COMMON_CXMSG_H
