#ifndef CX_CORE_COMMON_CXMSG_H
#define CX_CORE_COMMON_CXMSG_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxMsg.h: 
 * @brief Contains message class templates for message passing.
 *
 * @author Catlin Zilinski
 * @date Aug 16, 2015
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
		/** @brief Empty message */
		CX_FORCE_INLINE CxMsg() : m_msgID(0) {}

		/** @brief Message with specified id type */
		CX_FORCE_INLINE CxMsg(CxI32 in_msgID) : m_msgID(in_msgID) {}

		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator==(const CxMsg &in_src) const {
			return m_msgID == in_src.m_msgID;
		}

		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator!=(const CxMsg &in_src) const {
			return m_msgID != in_src.m_msgID;
		}
		
		/** @return The message ID for the message. */
		CX_FORCE_INLINE CxI32 msgID() const { return m_msgID; }
		
	  protected:
		CxI32 m_msgID;
	};

	/**
	 * @class CxMsgOne CxMsg.h "core/common/CxMsg.h"
	 * @brief A message class with one argument.
	 * 
	 * @since Aug 16, 2015
	 * @version 1
	 * @author Catlin Zilinski
	 */
	template <typename Arg0>
	class CxMsgOne : public CxMsg {
	  public:
		/** @brief Empty message */
		CX_FORCE_INLINE CxMsgOne() : CxMsg() {}

		/** @brief Message with specified id type and argument */
		CX_FORCE_INLINE CxMsgOne(CxI32 in_msgID, Arg0 in_arg1)
			: CxMsg(in_msgID), m_arg0(in_arg0) {}

		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator==(const CxMsgOne &in_src) const {
			return (m_msgID == in_src.m_msgID && in_arg0 == in_src.in_arg0);
		}

		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator!=(const CxMsg &in_src) const {
			return (m_msgID != in_src.m_msgID || in_arg0 != in_src.in_arg0);
		}
		
		/** @return The value of Arg0 for the message */
		CX_FORCE_INLINE Arg0 & arg0() { return m_arg0; }
		
	  protected:
		Arg0 m_arg0;
	};

	/**
	 * @class CxMsgTwo CxMsg.h "core/common/CxMsg.h"
	 * @brief A message class with two arguments.
	 * 
	 * @since Aug 16, 2015
	 * @version 1
	 * @author Catlin Zilinski
	 */
	template <typename Arg0, typename Arg1>
	class CxMsgTwo : public CxMsgOne<Arg0> {
	  public:
		/** @brief Empty message */
		CX_FORCE_INLINE CxMsgTwo() : CxMsg() {}

		/** @brief Message with specified id type and one argument */
		CX_FORCE_INLINE CxMsgTwo(CxI32 in_msgID, const Arg0 &in_arg0)
			: CxMsg(in_msgID), m_arg0(in_arg0) {}

		/** @brief Message with specified id type and two arguments */
		CX_FORCE_INLINE CxMsgTwo(CxI32 in_msgID, const Arg0 &in_arg0, const Arg1 &in_arg1)
			: CxMsg(in_msgID), m_arg0(in_arg0), m_arg1(in_arg1) {}

		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator==(const CxMsgOne &in_src) const {
			return (m_msgID == in_src.m_msgID && in_arg0 == in_src.in_arg0 &&
					  in_arg1 == in_src.in_arg1);
		}

		/** @brief Overloaded equality operator (tests id) */
		CX_FORCE_INLINE CxBool operator!=(const CxMsg &in_src) const {
			return (m_msgID != in_src.m_msgID || in_arg0 != in_src.in_arg0 ||
					  in_arg1 != in_src.in_arg1);
		}
		
		/** @return The value of Arg1 for the message */
		CX_FORCE_INLINE Arg1 & arg1() { return m_arg1; }
		
	  protected:
		Arg1 m_arg1;
	};
	
} // namespace cat

#endif // CX_CORE_COMMON_CXMSG_H
