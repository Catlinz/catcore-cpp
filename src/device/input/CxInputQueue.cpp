#include "device/input/CxInputQueue.h"

namespace cat {

	CxInputQueue::CxInputQueue()
		: CxDualQueue<CxInputEvent, CxSpinlock>() {}

	CxInputQueue::CxInputQueue(CxI32 in_capacity)
		: CxDualQueue<CxInputEvent, CxSpinlock>(in_capacity) {}

	CxInputQueue::CxInputQueue(const CxInputQueue &in_src)
		: CxDualQueue<CxInputEvent, CxSpinlock>(in_src) {}

	CxInputQueue::CxInputQueue(CxInputQueue &&in_src)
		: CxDualQueue<CxInputEvent, CxSpinlock>(in_src) {}

   CxInputQueue & CxInputQueue::operator=(const CxInputQueue &in_src) {
		CxDualQueue<CxInputEvent, CxSpinlock>::operator=(in_src);
		return *this;
	}

	CxInputQueue & CxInputQueue::operator=(CxInputQueue &&in_src) {
		CxDualQueue<CxInputEvent, CxSpinlock>::operator=(in_src);
		return *this;
	}

} // namespace cat
