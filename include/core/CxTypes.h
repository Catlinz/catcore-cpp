#ifndef CX_CORE_CXTYPES_H
#define CX_CORE_CXTYPES_H

/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxTypes.h
 * @brief Contains definitions of basic types used through the libraries
 *
 * @author Catlin Zilinski
 * @date May 8, 2015
 */

#include "core/CxPreproc.h"

namespace cat {

	/* Define the basic types */
	
#if defined(CX_WINDOWS) || defined(CX_XBOXONE) || defined(CX_XBOX360)
	typedef unsigned __int64 CxU64;
	typedef signed __int64 CxI64;
#else
	typedef unsigned long long CxU64;
	typedef signed long long CxI64;
#endif
	
	typedef double CxF64;
	
	typedef unsigned int CxU32;
	typedef signed int CxI32;
	typedef float CxF32;

	typedef unsigned short CxU16;
	typedef signed short CxI16;

	typedef char CxChar;
	typedef unsigned char CxU8;
	typedef signed char CxI8;

	typedef bool CxBool;
		
	/* Make sure the sizes are correct */
	CX_STATIC_ASSERT(sizeof(CxI8)==1,  assert_sizeof_CxI8);
	CX_STATIC_ASSERT(sizeof(CxU8)==1,  assert_sizeof_CxU8);
	CX_STATIC_ASSERT(sizeof(CxI16)==2, assert_sizeof_CxI16);
	CX_STATIC_ASSERT(sizeof(CxU16)==2, assert_sizeof_CxU16);
	CX_STATIC_ASSERT(sizeof(CxI32)==4, assert_sizeof_CxI32);
	CX_STATIC_ASSERT(sizeof(CxU32)==4, assert_sizeof_CxU32);
	CX_STATIC_ASSERT(sizeof(CxI64)==8, assert_sizeof_CxI64);
	CX_STATIC_ASSERT(sizeof(CxU64)==8, assert_sizeof_CxU64);

	/* Define the ranges of the types */
#define CX_MAX_U64 18446744073709551615
#define CX_MIN_U64 0
#define CX_MAX_I64 9223372036854775807
#define CX_MIN_I64 (-9223372036854775807)
#define CX_MAX_F64 DBL_MAX
#define CX_MIN_F64 -DBL_MAX
	
#define CX_MAX_U32 4294967295
#define CX_MIN_U32 0
#define CX_MAX_I32 2147483647
#define CX_MIN_I32 (-2147483648)
#define CX_MAX_F32 FLT_MAX
#define CX_MIN_F32 -FLT_MAX

#define CX_MAX_U16 65535U
#define CX_MIN_U16 0
#define CX_MAX_I16 32767
#define CX_MIN_I16 (-32768)

#define CX_MAX_U8 255U
#define CX_MIN_U8 0
#define CX_MAX_I8 127
#define CX_MIN_I8 (-128)

	/* Define the size of a Real value */
#if !defined(CX_REAL64)
	typedef CxF32 CxReal;
#  define CX_MAX_REAL CX_MAX_F32
#  define CX_MIN_REAL CX_MIN_F32
#  define CX_EPSILON CxReal(1e-20f)
#else
	typedef CxF64 CxReal;
#  define CX_MAX_REAL CX_MAX_F64
#  define CX_MIN_REAL CX_MIN_F64
#  define CX_EPSILON CxReal(1e-180)
#endif

	/**
	 * @brief Check to see if a floating point value is equal to another.
	 * @param in_val1 The value to check.
	 * @param in_val2 The value to check to see if val1 equals.
	 * @param in_error The amount of error to allow for in equality.
	 * @return True if the values are approximatly equal.
	 */
	//CX_FORCE_INLINE CxBool approx(CxReal in_val1, CxReal in_val2, CxReal in_error = CX_EPSILON) {
	//	return (fabs(in_val1 - in_val2) <= CX_EPSILON);
	//}
	
	/* Types to store / manipulate memory addresses */
#if defined(CX_32BIT)
	typedef CxU32 CxAddr;
#elif defined(CX_64BIT)
	typedef CxU64 CxAddr;
#endif	

	union CxMemAddr {
		void *ptr;
		CxAddr addr;
	};
	
	CX_STATIC_ASSERT(sizeof(CxAddr) >= sizeof(void *), assert_sizeof_CxAddr);
	
	/* Some more descriptive aliases of the basic types */
	typedef CxI64 CxBitField64;
	typedef CxI32 CxBitField32;
	typedef CxI16 CxBitField16;
	typedef CxI8  CxBitField8;
	
	typedef CxI8 CxByte;
	typedef CxU8 CxUByte;

	/* A union that can hold / convert any core type */
	union CxCoreType {
		CxU64  u64;
		CxI64  i64;
		CxF64  f64;
		void  *ptr;
		CxU32  u32;
		CxI32  i32;
		CxF32  f32;
		CxU16  u16;
		CxI16  i16;
		CxU8   u8;
		CxI8   i8;
		CxBool boolean;
	};

	/** @return True if the two values are equal. */
	CX_FORCE_INLINE CxBool operator==(const CxCoreType &in_v1,
												 const CxCoreType &in_v2) {
		return in_v1.u64 == in_v2.u64;
	}
	/** @return True if the two values are NOT equal. */
	CX_FORCE_INLINE CxBool operator!=(const CxCoreType &in_v1,
												 const CxCoreType &in_v2) {
		return in_v1.u64 != in_v2.u64;
	}
	/** @return True if the v1 > v2 */
	CX_FORCE_INLINE CxBool operator>(const CxCoreType &in_v1,
												const CxCoreType &in_v2) {
		return in_v1.u64 > in_v2.u64;
	}
	/** @return True if the v1 >= v2 */
	CX_FORCE_INLINE CxBool operator>=(const CxCoreType &in_v1,
												const CxCoreType &in_v2) {
		return in_v1.u64 >= in_v2.u64;
	}
	/** @return True if the v1 < v2 */
	CX_FORCE_INLINE CxBool operator<(const CxCoreType &in_v1,
												const CxCoreType &in_v2) {
		return in_v1.u64 < in_v2.u64;
	}
	/** @return True if the v1 <= v2 */
	CX_FORCE_INLINE CxBool operator<=(const CxCoreType &in_v1,
												const CxCoreType &in_v2) {
		return in_v1.u64 <= in_v2.u64;
	}
	
} // namespace cat

#endif // CX_CORE_CXTYPES_H
