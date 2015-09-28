#ifndef CX_CORE_EVENT_CXSIGNAL_H
#define CX_CORE_EVENT_CXSIGNAL_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file CxSignal.h
 *	@brief The CxSignalEmitter base class for objects to emit signals.
 *
 * @author Catlin Zilinski
 * @date Sept 4, 2015
 */

#include "core/Cx.h"
#include "core/common/CxPODVector.h"
#include "core/common/CxHashMap.h"
#include "core/event/CxSlot.h"

namespace cat {

	/**
	 * @class CxSignalEmitter CxSignal.h "core/event/CxSignal.h"
	 *	@brief The base class for any class that wants to emit signals.
	 *
	 * @author Catlin Zilinski
	 * @version 3
	 * @since Apr 30, 2014
	 */
	class CxSignalEmitter {		
	  public:
		/**
		 * @brief Create a new signal emitter.
		 * @param in_capacity The initial number of signals to be stored.
		 */
		CX_FORCE_INLINE CxSignalEmitter(CxI32 in_capacity = 8) {
			m_slots = CxHashMap<CxChar *, CxPODVector<CxSlot> >(in_capacity, 0.8f);
		}

		/** @brief Destroys the map. */
	   virtual ~CxSignalEmitter();

		/**
		 * @brief Connect a signal of this object to the specified handler.
		 * @param in_signal The name of the signal to attach the slot to.
		 * @param in_slot The slot to attach to the Signal.
		 * @return True if the slot was successfully attached.
		 */
		CxBool connect(const CxChar * in_signal, const CxSlot &in_slot);
		
		/**
		 * @brief Connect a signal of this object to the specified handler.
		 * @param name The hashed name of the signal to attach the handler to.
		 * @param func The static slot method on the object to connect to.
		 * @param obj The object to call the slot on.
		 * @return True if the SignalHandler was successfully attached.
		 */
		CX_FORCE_INLINE CxBool connect(const CxChar * in_signal, CxSlotFunc in_slot, void* in_recv) {
			return connect(in_signal, CxSlot(in_slot, in_recv));
		}

		/**
		 * @brief disconnect a signal of this object from the specified handler.
		 * @param in_signal The name of the signal to detach the handler from.
		 * @param in_slot The SignalHandler to detach from the Signal.
		 * @return True if the SignalHandler was successfully detached.
		 */
		CxBool disconnect(const CxChar *in_signal, const CxSlot &in_slot);
		
		/**
		 * @brief disconnect a signal of this object from the specified handler.
		 * @see disconnect(const Char*, SignalHandler*)
		 * @param in_signal The name of the signal to detach the handler from.
		 * @param in_slot The static slot method on the object to disconnect.
		 * @param in_recv The object to disconnect the slot.
		 * @return True if the SignalHandler was successfully detached.
		 */
		CX_FORCE_INLINE CxBool disconnect(const CxChar *in_signal, CxSlotFunc in_slot,
													 void *in_recv) {
			return disconnect(in_signal, CxSlot(in_slot, in_recv));
		}

		/**
		 * @brief disconnect all signal handlers from the specified signal.
		 * @param in_signal The name of the signal to detach the handler from.
		 * @return True if there are no handlers attached anymore.
		 */
		CxBool disconnectAll(const CxChar *in_signal);

	  protected:
		/**
		 * @brief Emit the specified signal.
		 * @param signal The signal to emit.
		 * @param data The SignalData to send with the signal.
		 */
		void emit(CxU32 in_signal, const CxArgs &in_args);
		
		CX_FORCE_INLINE void emit(const CxChar *in_signal, const CxArgs &in_args) {
			emit(CxHash(in_signal), in_args);
		}

		CX_FORCE_INLINE void emit(CxU32 in_signal) { emit(in_signal, CxArgs()); }
		CX_FORCE_INLINE void emit(const CxChar *in_signal) {
			emit(CxHash(in_signal), CxArgs());
		}
		
		CxHashMap<CxChar *, CxPODVector<CxSlot> > m_slots;
	};
	
} // namepsace cat

#endif // CX_CORE_EVENT_CXSIGNAL_H
