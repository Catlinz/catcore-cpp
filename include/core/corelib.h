#ifndef CAT_CORE_CORELIB_H
#define CAT_CORE_CORELIB_H

/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * corelib.h: Standard header to include in all the library files
 *
 * Author: Catlin Zilinski
 * Date: May 19, 2013
 */

// Define statements to print out information to the console if the program is 
// running with debug information on
#if defined (DEBUG)
#include <iostream>
#include <iomanip>
#include <cassert>
#include <new>
#include <cstdlib>
#include <cfloat>

#define DCRASH(msg)									\
	(std::cerr << "#------> ERROR ('" << __FILE__ << "' : [" << std::dec << __LINE__ << "]): "  << msg << std::endl << std::flush); \
	assert(false)
#define DERR(msg) \
	(std::cerr << "#------> ERROR ('" << __FILE__ << "' : [" << std::dec << __LINE__ << "]): "  << msg << std::endl << std::flush)
#define DWARN(msg) \
	(std::cout << "#----> Warning ('" << __FILE__ << "' : [" << std::dec << __LINE__ << "]): "  << msg << std::endl << std::flush)
#define DMSG(msg) \
	(std::cout << "#--> Message ('" << __FILE__ << "' : [" << std::dec << __LINE__ << "]): "  << msg << std::endl << std::flush)
#define DOUT(msg) (std::cout << msg)
#define D(exp) exp

#define D_CONDERR(cond,msg)							\
	if ((cond)) { (std::cerr << "#------> ERROR ('" << __FILE__ << "' : [" << std::dec << __LINE__ << "]): "  << msg << std::endl << std::flush); }
#define D_CONDWARN(cond,msg)							\
	if ((cond)) { (std::cerr << "#----> Warning ('" << __FILE__ << "' : [" << std::dec << __LINE__ << "]): "  << msg << std::endl << std::flush); }
#define D_CONDMSG(cond,msg)							\
	if ((cond)) { (std::cerr << "#--> Message ('" << __FILE__ << "' : [" << std::dec << __LINE__ << "]): "  << msg << std::endl << std::flush); }

/**
 * Definition for debug macros to check for infinite or NaN values.
 */
/* Check for NaN */
#define D_ISNUMBER(x) assert((x) == (x))
/* Check for infinite float */
#define D_ISFINITE_FLT(x) assert((x) <= FLT_MAX && (x) >= -FLT_MAX)
#define D_ISFINITE_DBL(x) assert((x) <= DBL_MAX && (x) >= -DBL_MAX)
/* Check for NaN or infinite */
#define D_ISVALID_FLT(x)								\
	assert((x) == (x));									\
	assert((x) <= FLT_MAX && (x) >= -FLT_MAX)
#define D_ISVALID_DBL(x)								\
	assert((x) == (x));									\
   assert((x) <= DBL_MAX && (x) >= -DBL_MAX)
	
#else
#define DCRASH(msg)
#define DERR(msg)
#define DWARN(msg)
#define DMSG(msg)
#define DOUT(msg)
#define D(exp)

#define D_CONDERR(cond,msg)
#define D_CONDWARN(cond,msg)
#define D_CONDMSG(cond,msg)

#define D_ISNUMBER(x)
#define D_ISFINITE_FLT(x)
#define D_ISFINITE_DBL(x)
#define D_ISVALID_FLT(x)
#define D_ISVALID_DBL(x)
#endif

/**
 * Definition for safe delete macro for object pointers.
 */
#define CC_SAFEDELETE(var)								\
	if (var) {												\
		delete var;											\
		var = NIL;											\
	}															\

/**
 * Definition for safe delete macro for arrays.
 */
#define CC_SAFEDELETE_ARRAY(var)						\
	if (var) {												\
		delete[] var;										\
		var = NIL;											\
	}															\

/**
 * Definition for safe delete of reference counted pointers.
 */
#define CC_SAFEDELETE_ISPTR(var)						\
	if (var && var->release()) {						\
		delete var;											\
		var = NIL;											\
	}															\
	
	

/**
 * Definintion for a macro to supress unused variable warnings.
 */
#define CC_UNUSED(unused_var_name) (void)(unused_var_name)


	

// Check for, and remember, if 64bit or 32bit
// Check windows
#if defined(_WIN64) || defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(__MINGW32__) || defined(__MINGW64__)
#define OS_WINDOWS 1
#if defined(_WIN64) || defined(WIN64) || defined(__MINGW64)
#define ENV_IS_64
#else
#define ENV_IS_32
#endif
#endif

// Check for apple
#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__) || defined(__OSX__)
#define OS_APPLE 1
#endif

// Else assume linux/unix
#if ! defined (OS_APPLE) && ! defined (OS_WINDOWS)
#define OS_UNIX 1
#endif


// Check GCC
#if defined (__GNUC__)
#if defined (__x86_64__) || defined (__ppc64__) || defined (__amd64) || defined (__x86_64) || defined(__amd64__) 
#define ENV_IS_64
#else
#define ENV_IS_32
#endif
#endif

#include "core/types.h"

#if defined (OS_APPLE) && !defined (USE_XLIB)
#include "core/osx/corelib.h"
#define IF_APPLE(x) x
#else
#define IF_APPLE(x)
#endif

#if defined (OS_WINDOWS)
#include "core/win32/corelib.h"
#define IF_WINDOWS(x) x
#else
#define IF_WINDOWS(x)
#endif

#if defined (OS_UNIX) || defined (USE_XLIB)
#include "core/unix/corelib.h"
#define IF_UNIX(x) x
#else
#define IF_UNIX(x)
#endif

#if defined (OS_UNIX) || defined (OS_APPLE)
#define IF_UNIX_OR_APPLE(x) x
#else
#define IF_UNIX_OR_APPLE(x)
#endif

// Some core functions that are used throughout

namespace Cat {
	/**
	 * @brief Checks if a floating point value is roughly equal to another.
	 * @param value The value to check for equality
	 * @param equals What the value should be equal to
	 * @param error The OPTIONAL error range to use
	 * @return true if the value is roughly equal.
	 */
	inline Boolean approx(Real value, Real equals, Real error = REAL(0.0001)) {
		if (value > equals) return (value - equals < error);
		return (equals - value < error);
	}	

	/**
	 * @brief Calculates the CRC32 hash of a string and returns the hash as a 32 bit integer.
	 * @param str The string to hash.
	 * @return A 32 bit unsigned integer represetning the CRC32 hash of the string.
	 */
	OID crc32(const Char* str);

	/**
	 * @brief Copies the value of a string and returns a pointer to the newly allocated copy.
	 * @param str The string to make a copy of.
	 * @return A pointer to the newly allocated string copy.
	 */
	Char* copy(const Char* str);

	/**
	 * @brief Tests to see if two strings are equal.
	 * @param str1 The first string to test.
	 * @param str2 The second string to test.
	 * @return True if the strings are equal, false otherwise.
	 */
	Boolean eq(const Char* str1, const Char* str2);

	/**
	 * @brief Tests to see if a field in a bitfield is set.
	 * @param field The bitfield to test
	 * @param bit The bit to test.
	 */
	inline Boolean testBit(BitField32 field, U32 bit) {
		return (field & (1<<bit));
	}
	
	/**
	 * @brief Sets a bit in a bitfield.
	 * @param field The bitfield to test
	 * @param bit The bit to test.
	 */
	inline BitField32 setBit(BitField32 field, U32 bit) {
		return (field | (1<<bit));
	}

	/**
	 * @brief Clears a bit in a bitfield.
	 * @param field The bitfield to test
	 * @param bit The bit to test.
	 */
	inline BitField32 clearBit(BitField32 field, U32 bit) {
		return (field & (~(1<<bit)));
	}

	namespace ReturnCode {
		/**
		 * @brief Check to see if the return code is an error code.
		 * @param returnCode the ReturnCode to check.
		 * @return True if the ReturnCode is an error.
		 */
		inline Boolean isError(I32 returnCode) {
			return (returnCode & kRCError);
		}

		/**
		 * @brief Check to see if the return code is a warning code.
		 * @param returnCode the ReturnCode to check.
		 * @return True if the ReturnCode is a warning.
		 */
		inline Boolean isWarning(I32 returnCode) {
			return (returnCode & kRCWarning);
		}

		/**
		 * @brief Check to see if the return code is a success code.
		 * @param returnCode the ReturnCode to check.
		 * @return True if the ReturnCode is a successfull code.
		 */
		inline Boolean isSuccess(I32 returnCode) {
			return !(returnCode & (kRCWarning | kRCError));
		}

		/**
		 * @brief Check to see if the return code is NOT a success code.
		 * @param returnCode the ReturnCode to check.
		 * @return True if the ReturnCode is NOT a successful code.
		 */
		inline Boolean isNotSuccess(I32 returnCode) {
			return (returnCode & (kRCWarning | kRCError));
		}
		
		const Char* toString(I32 returnCode);		
	}

	namespace Warning {
		const Char* toString(I32 warning);
	}	
	
	namespace Error {
		const Char* toString(I32 error);
	}
	

}




#endif // CAT_CORE_CORELIB_H

