#ifndef CAT_CORE_UTIL_INTERNALMESSAGE_H
#define CAT_CORE_UTIL_INTERNALMESSAGE_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file internalmessage.h: 
 * @brief Contains the definition for the InternalMessage classes.
 *
 * The InternalMessage Params classes are simple container classes 
 * to facilitate passing messages internally to perform actions.
 *
 * @author Catlin Zilinski
 * @date Mar 17, 2014
 */

#include "core/corelib.h"

namespace Cat {

	class InternalMessage {
	  public:
		I32 typeID;

		InternalMessage()
			: typeID(0) {}
		
		InternalMessage(I32 pTypeID)
			: typeID(pTypeID) {}
		
		inline Boolean operator==(const InternalMessage& other) const {
			return typeID == other.typeID;			
		}
		inline Boolean operator!=(const InternalMessage& other) const {
			return typeID != other.typeID;
		}
	};

	template<typename Arg1>
	class InternalMessage1Arg {
	  public:
		I32 typeID;
		Char numArgs;
		Arg1 arg1;

		InternalMessage1Arg()
			: typeID(0), numArgs(0) {}
		
		InternalMessage1Arg(I32 pTypeID)
			: typeID(pTypeID), numArgs(0) {}
		
		InternalMessage1Arg(I32 pTypeID, Arg1 pArg1)
			: typeID(pTypeID), numArgs(1), arg1(pArg1) {}
		
		inline Boolean operator==(const InternalMessage1Arg<Arg1>& other) const {
			return (
				(typeID == other.typeID && numArgs == other.numArgs) &&
				((numArgs == 0) || (arg1 == other.arg1))
				);
		}
		inline Boolean operator!=(const InternalMessage1Arg<Arg1>& other) const {
			return (
				(typeID != other.typeID || numArgs != other.numArgs) ||
				((numArgs != 0) && (arg1 != other.arg1))
				);
		}
	};

	template<typename Arg1, typename Arg2>
	class InternalMessage2Args {
	  public:
		I32 typeID;
		Char numArgs;
		Arg1 arg1;
		Arg2 arg2;		

		InternalMessage2Args()
			: typeID(0), numArgs(0) {}
		
		InternalMessage2Args(I32 pTypeID)
			: typeID(pTypeID), numArgs(0) {}
		
		InternalMessage2Args(I32 pTypeID, Arg1 pArg1)
			: typeID(pTypeID), numArgs(1), arg1(pArg1) {}

		InternalMessage2Args(I32 pTypeID, Arg1 pArg1, Arg2 pArg2)
			: typeID(pTypeID), numArgs(1), arg1(pArg1), arg2(pArg2) {}
		
		inline Boolean operator==(const InternalMessage2Args<Arg1, Arg2>& other) const {
			return (
				(typeID == other.typeID && numArgs == other.numArgs) &&
				(
					numArgs == 0 || (numArgs == 1 && arg1 == other.arg1) ||
					(arg1 == other.arg1 && arg2 == other.arg2)
					)
				);
		}
		inline Boolean operator!=(const InternalMessage2Args<Arg1, Arg2>& other) const {
			return (
				(typeID != other.typeID || numArgs != other.numArgs) ||
				(
					(numArgs == 1 && (arg1 != other.arg1)) ||
					(numArgs == 2 && (arg1 != other.arg1 || arg2 != other.arg2))
					)
				);
		}
	};
} // namespace Cat

#endif // CAT_CORE_UTIL_INTERNALMESSAGE_H
