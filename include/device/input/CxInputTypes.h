#ifndef CX_DEVICE_INPUT_CXINPUTTYPES_H
#define CX_DEVICE_INPUT_CXINPUTTYPES_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxInputTypes.h
 * Definitions of various types and constants used for input event processing.
 *
 * @author Catlin Zilinski
 * @date Jan 2, 2016
 */

#include "core/Cx.h"

namespace cat {
	
	namespace CxInput {
		
		/** @brief The types of actions a button can perform. */
		enum ButtonAction {
			kRelease = 0x0,
			kPress = 0x1,
			kRepeat = 0x2,
		};
		
	} // namespace CxInput

} // namespace cat
#endif // CX_DEVICE_INPUT_CXINPUTTYPES_H
