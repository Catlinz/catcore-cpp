#ifndef CAT_CORE_TYPES_H
#define CAT_CORE_TYPES_H

/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * types.h: Contains definitions of basic types used through the libraries
 *
 * Author: Catlin Zilinski
 * Date: May 19, 2013
 */

// Define some basic constants

#include <cstddef>

#define NIL 0
#define CC_TRUE true
#define CC_FALSE false
#define CC_YES true
#define CC_NO false
#define CC_SUCCESS true
#define CC_FAILURE false


namespace Cat {

	enum PtrFreeMethod {
		PFM_DELETE, PFM_DELETE_OBJ, PFM_FREE, PFM_DELETE_ARRAY, PFM_DELETE_OBJ_ARRAY
	};

	namespace ReturnCode {
		enum Enum {
			kRCSuccess = 0x0,
			kRCTrue = 0x1,
			kRCFalse = 0x2,
			kRCExists = 0x3,
			kRCAlreadyDone = 0x4,
			kRCWarning = 0x10,
			kRCError = 0x40,
		};
	} // namespace ReturnCode

	namespace Warning {
		enum Enum {
			kWNoWarning = 0x0,
			kWUnknownWarning = 0x10,
			kWNullValue = 0x11,
			kWInvalidValue = 0x12,
			kWInvalidState = 0x13,
			kWInvalidArgument = 0x14,
			kWInvalidFormat = 0x15,
			kWAllocFailed = 0x16,
			kWConnectionFailed = 0x17,
			kWBadRequest = 0x18,
			kWAccessDenied = 0x19,
			kWIOFailed = 0x1a,
			kWFileNotFound = 0x1b,
			kWInvalidPath = 0x1c,
			kWInvalidFileName = 0x1d,
			kWInvalidFile = 0x1e,
			kWMemoryWarning = 0x1f,
			kWFailedOp = 0x20,
			kWIncompleteOp = 0x21,
			kWExists = 0x22,
			kWAlreadyDone = 0x23,
		};
	} // namespace Warning

	namespace Error {
		enum ErrEnum {
			kENoError = 0,
			kEUnknownError = 0x40,
			kEInvalidValue = 0x41,
			kEInvalidState = 0x42,
			kEInvalidArgument = 0x43,
			kEInvalidFormat = 0x44,
			kEBadAddress = 0x45,
			kEAllocError = 0x46,
			kEConnectionError = 0x47,
			kEIlegalOperation = 0x48,
			kEBadRequest = 0x49,
			kEAccessDenied = 0x4a,
			kEIOError = 0x4b,
			kEFileNotFound = 0x4c,
			kEInvalidPath = 0x4d,
			kEInvalidFileName = 0x4e,
			kEInvalidFile = 0x5f,
			kENullValue = 0x50,
			kEInsufficientMemory = 0x51,
			kEExists = 0x22,
			kEAlreadyDone = 0x23,
		};
	} // namespace Error
	
	
	typedef unsigned long long U64;
	typedef signed long long I64;

	typedef unsigned int U32;
	typedef signed int I32;

	typedef unsigned short U16;
	typedef signed short I16;

	typedef unsigned char U8;
	typedef signed char I8;

	typedef size_t Size;

	typedef float F32;
	typedef double F64;

	// So can easily adjust precision of all math objects.
	typedef F32 Real;
#define REAL(x) x##f

	typedef unsigned char UByte;
	typedef unsigned char Byte;
	
	typedef void* VPtr;
	typedef unsigned char* ByteArray;

	typedef bool Boolean;

	typedef U32 OID;

	typedef char Char;

	typedef Char* CString;
	typedef Char* CStr;

	typedef U64 BitField64;
	typedef U32 BitField32;
	typedef U16 BitField16;
	typedef unsigned char BitField8;	

	union Number32 {	
		U32 u32;
		I32 i32;
		F32 f32;
		U16 u16;
		I16 i16;
		U8 u8;
		I8 i8;
		UByte uByte;
		Byte byte;		
	};
	

	union Number64 {
		U64 u64;
		I64 i64;
		F64 f64;
		Size size;		
		U32 u32;
		I32 i32;
		F32 f32;
		U16 u16;
		I16 i16;
		U8 u8;
		I8 i8;
		UByte uByte;
		Byte byte;
		Boolean boolean;		
	};

	union IntegralType {
		U64 u64;
		I64 i64;
		F64 f64;
		VPtr vPtr;		
		Size size;
		Real real;		
		U32 u32;
		I32 i32;
		F32 f32;
		U16 u16;
		I16 i16;
		U8 u8;
		I8 i8;
		UByte uByte;
		Byte byte;
		Boolean boolean;
	};
	
	inline Boolean operator==(const Number64& n1, const Number64& n2) { return n1.u64 == n2.u64; }	
	inline Boolean operator!=(const Number64& n1, const Number64& n2) { return n1.u64 != n2.u64; }
	inline Boolean operator==(const Number32& n1, const Number32& n2) { return n1.u32 == n2.u32; }	
	inline Boolean operator!=(const Number32& n1, const Number32& n2) { return n1.u32 != n2.u32; }
	inline Boolean operator==(const IntegralType& n1, const IntegralType& n2) { return n1.u64 == n2.u64; }	
	inline Boolean operator!=(const IntegralType& n1, const IntegralType& n2) { return n1.u64 != n2.u64; }
	
	
#ifdef ENV_IS_64
	typedef U64 Addr;
	union MemAddr {
		VPtr ptr;
		Addr addr;
	};
#else
	typedef U32 Addr;
	union MemAddr {
		VPtr ptr;
		Addr addr;
	};
#endif // ENV_IS_64

} // namespace Cat
#endif // CAT_CORE_TYPES_H
