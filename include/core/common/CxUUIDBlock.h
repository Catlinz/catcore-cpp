#ifndef CX_CORE_COMMON_CXUUIDBLOCK_H
#define CX_CORE_COMMON_CXUUIDBLOCK_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxUUIDBlock.h: 
 * @brief Contains a class for generating universally unique ID's.
 *
 * @author Catlin Zilinski
 * @date Sept 2, 2015
 */

#include "core/Cx.h"
#include "core/threading/CxSpinlock.h"
#include "core/threading/CxMutex.h"
#include "core/threading/CxConditionVariable.h"

namespace cat {

	/**
	 * @class CxUUIDBlock CxUUIDBlock.h "core/common/CxUUIDBlock.h"
	 * The CxUUIDBlock class is used to generate universally unique ID's 
	 * from a given block of available ID's.  An example usage is a server that
	 * distributes blocks of UUID's to clients to use instead of asking the server 
	 * for a new UUID each time the client needs one.
	 * 
	 * @since Sept 2, 2015
	 * @version 1
	 * @author Catlin Zilinski
	 */
	class CxUUIDBlock {
	  public:
		/** @brief Create an empty CxUUIDBlock */
		CxUUIDBlock();

		/**
		 * @brief Generate and return a new ID, and set whether or not the generator is valid.
		 * If the last ID has been generated, the generator will be set to invalid and 
		 * need to be refreshed with a new block of UUID's to generate.
		 * @return The ID (greater than 0) or 0 if no valid ID can be generated.
		 */
		CX_FORCE_INLINE CxU64 generate() {
			m_lock.lock();
		   CxU64 id = m_next++;
			if (m_next > m_last) {
				m_isValid = false;
				if (id > m_last) { id = 0; }
			}
			m_lock.unlock();
			return id;
		}

		/** 
		 * @brief Set the first and last ID's the generator can generate.
		 * @param in_start The first ID to generate.
		 * @param in_end The last ID to generate.
		 */
		void set(CxU64 in_start, CxU64 in_end);

		/**
		 * @brief Sets to be fetching a new block if no-one else has done so already.
		 * @return True if we should fetch the new block, false if someone already doing so.
		 */
		CxBool shouldFetchNewIDBlock();

		/** @brief Signals the calling thread to wait until the next block of ID's is fetched. */
		void waitForNewIDBlock();
		
		
	  private:
		CxU64 m_next;
		CxU64 m_last;

		CxSpinlock m_lock;
		CxMutex m_fetchLock;
		CxConditionVariable m_fetchWait;

		CxBool m_isValid, m_isFetching;
		
	};
} // namespace cat

#endif // CX_CORE_COMMON_CXUUIDBLOCK_H
