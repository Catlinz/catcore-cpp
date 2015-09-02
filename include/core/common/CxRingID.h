#ifndef CX_CORE_COMMON_CXRINGID_H
#define CX_CORE_COMMON_CXRINGID_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxRingID.h: 
 * @brief Contains a class for generating short term unique ID's.
 *
 * @author Catlin Zilinski
 * @date Sept 2, 2015
 */

#include "core/Cx.h"
#include "core/threading/CxSpinlock.h"

namespace cat {

	/**
	 * @class CxRingID CxRingID.h "core/common/CxRingID.h"
	 * The CxRingID class is used to generate unique ID's for 
	 * short lived objects (short enough that an ID is out of use before
	 * it is reused) such as projectiles from weapons.
	 * 
	 * @since Sept 2, 2015
	 * @version 1
	 * @author Catlin Zilinski
	 */
	class CxRingID {
	  public:
		/** @brief Create an empty ID generator initialised to zero */
		CxRingID();

		/** 
		 * @brief Create a new ID generator with the given start and range.
		 * @param in_start The value the ID's will start at.
		 * @param in_range The number of unique ID's that can be generated.
		 */
		CX_FORCE_INLINE CxRingID(CxU32 in_start, CxU32 in_range)
			: m_start(in_start), m_range(in_range), m_cur(0) {}

		/** @return An ID and increment next ID. */
		CX_FORCE_INLINE CxU32 generate() {
			const CxU32 s = m_start;
			const CxU32 r = m_range;
			m_lock.lock();
			const CxU32 c = m_cur;
			CxU32 id = c + s;
			m_cur = (c + 1) % r;
			m_lock.unlock();
			return id;
		}
		
	  private:
		CxSpinLock m_lock;
		CxU32 m_start, m_range;
		CxU32 m_cur;
	};

} // namespace cat

#endif // CX_CORE_COMMON_CXRINGID_H
