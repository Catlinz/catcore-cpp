#ifndef CAT_CORE_DEFER_MESSAGETYPES_H
#define CAT_CORE_DEFER_MESSAGETYPES_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file messagetypes.h
 *	@brief Contains all the different types of messages.
 *
 * @author Catlin Zilinski
 * @date Mar 16, 2014
 */

#define CAT_NUM_MESSAGE_TYPES 4

namespace Cat {
	namespace MessageType {
		enum CoreMessage {
			kMTNoMessage = 0,
			kMTWindowResized = 1,
			kMTWindowMinimized = 2,
			kMTWindowRestored = 3,
		};
	} // namespace MessageType
} // namespace Cat

#endif // CAT_CORE_DEFER_MESSAGETYPES_H
