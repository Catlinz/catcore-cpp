#include "core/event/CxSignal.h"

namespace cat {

	CxSignalEmitter::~CxSignalEmitter() { }
	

	CxBool CxSignalEmitter::connect(const CxChar * in_signal, const CxSlot &in_slot) {
		CxPODVector<CxSlot> &slots = m_slots[in_signal];
		if (!slots.contains(in_slot)) {
			slots.append(in_slot);
			return true;
		}
		else {
			CXD_WARN("Cannot add slot '%d' twice.", in_signal);
			return false;
		}
	}

	CxBool CxSignalEmitter::disconnect(const CxChar * in_signal, const CxSlot &in_slot) {
		return m_slots[in_signal].remove(in_slot);
	}

	CxBool CxSignalEmitter::disconnectAll(const CxChar *in_signal) {
		CxPODVector<CxSlot> &slots = m_slots[in_signal];
		if (slots.size() > 0) { slots.clear();  return true; }
		else { return false; }
	}

	void CxSignalEmitter::emit(CxU32 in_signal, const CxArgs &in_args) {
		const CxPODVector<CxSlot> &slots = m_slots.value(in_signal);

		const CxI32 nm_slots = slots.count();
		for (CxI32 i = 0; i < nm_slots; ++i) {
			slots[i].call(in_args);
		}
	}	

} // namespace cat
