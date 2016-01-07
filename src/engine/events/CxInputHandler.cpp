#include "engine/events/CxInputHandler.h"

namespace cat {

	CxInputHandler::CxInputHandler()
		: mp_next(0) {}

	CxInputHandler::~CxInputHandler() {
		mp_next = 0;
	}

	CxInputHandler * CxInputHandler::push(CxInputHandler *in_top) {
		mp_next = in_top;
		return this;
	}
	

	
} // namespace cat
