#ifndef CX_CORE_EVENT_CXSLOT_H
#define CX_CORE_EVENT_CXSLOT_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *	
 *	@file CxSlot.h
 *	@brief The Slot class to hold a function and receiver object for a signal.
 *
 * @author Catlin Zilinski
 * @date Sept 5, 2015
 */

#include "core/Cx.h"
#include "core/common/CxArgs.h"

namespace cat {

	typedef void (*CxSlotFunc)(void *, const CxArgs &);

	/**
	 * @class CxSlot CxSlot.h "core/event/CxSlot.h"
	 *	@brief A class for holding a signal to be connected to a slot.
	 *
	 * @author Catlin Zilinski
	 * @version 4
	 * @since Apr 30, 2014
	 */
	class CxSlot {		
	  public:
		/** @brief Create empty slot */
	   CX_FORCE_INLINE CxSlot() : mp_slot(0), mp_recv(0) {}

		/**
		 * @brief Create a slot from a function and receiver object.
		 * @param in_slot The function to be called in response to a signal.
		 * @param in_recv The object to pass as the first arg of the slot.
		 */
		CX_FORCE_INLINE CxSlot(CxSlotFunc in_slot, void *in_recv)
			: mp_slot(in_slot), mp_recv(in_recv) {}

		/** @return True if the slot and receivers are the same. */
		CX_FORCE_INLINE CxBool operator==(const CxSlot &in_slot) const {
			return (mp_slot == in_slot.mp_slot &&
					  mp_recv == in_slot.mp_recv);
		}

		/** @return True if either the slot or receiver is different. */
		CX_FORCE_INLINE CxBool operator!=(const CxSlot &in_slot) const {
			return (mp_slot != in_slot.mp_slot ||
					  mp_recv != in_slot.mp_recv);
		}

		/**
		 * @brief Call the slot with the specified arguments.
		 * @param in_args The arguments to pass to the slot.
		 */
		CX_FORCE_INLINE void call(const CxArgs &in_args) const {
			mp_slot(mp_recv, in_args);
		}

		/** @return A pointer to the receiving object */
		CX_FORCE_INLINE void * recv() const { return mp_recv; }

		/** @return A pointer to the slot function */
		CX_FORCE_INLINE CxSlotFunc slot() const { return mp_slot; }

	  private:
		CxSlotFunc mp_slot;
		void * mp_recv;
	};
	
} // namepsace cat

#endif // CX_CORE_EVENT_CXSLOT_H
