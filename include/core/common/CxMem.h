#ifndef CX_CORE_COMMON_CXMEM_H
#define CX_CORE_COMMON_CXMEM_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxMem.h
 * @brief Contains basic memory allocation and manipulation functions.
 *
 * @author Catlin Zilinski
 * @date May 19, 2015
 */

#include "core/Cx.h"

namespace cat {
	namespace mem {

		/**
		 * @brief Allocate the specified amount of bytes.
		 * @param in_bytes The number of bytes to allocate.
		 * @return A pointer to the allocated memory.
		 */
		CX_FORCE_INLINE void * alloc(CxU32 in_bytes) {
			return ::malloc(in_bytes);
		}

		/**
		 * @brief Allocate the specified amount of bytes.
		 * This method will either allocate the memory, if the input pointer 
		 * is zero, or reallocate the memory if the pointer is not null.
		 * @param inout_ptr A pointer to store the allocated block of memory.
		 * @param in_bytes The number of bytes to allocate.
		 * @return A pointer to the allocated memory.
		 */
		CX_FORCE_INLINE void * alloc(void *&inout_ptr, CxU32 in_bytes) {
			if (inout_ptr == 0) { inout_ptr = ::malloc(in_bytes); }
			else { inout_ptr = ::realloc(inout_ptr, in_bytes); }
			return inout_ptr;
		}

		/**
		 * @brief Allocate the specified amount of bytes and initialise to zero.
		 * @param in_num The number of elements to allocate memory for.
		 * @param in_size The size, in bytes, of each element.
		 * @return A pointer to the allocated memory.
		 */
		CX_FORCE_INLINE void * allocZero(CxU32 in_num, CxU32 in_size) {
			return ::calloc(in_num, in_size);
		}

		/**
		 * @brief Allocate the specified amount of bytes and initialise to zero.
		 * This method will either allocate the memory, if the input pointer 
		 * is zero, or reallocate the memory if the pointer is not null.
		 * @param inout_ptr A pointer to store the allocated block of memory.
		 * @param in_num The number of elements to allocate memory for.
		 * @param in_size The size, in bytes, of each element.
		 * @return A pointer to the allocated memory.
		 */
		CX_FORCE_INLINE void * allocZero(void *&inout_ptr, CxU32 in_num, CxU32 in_size) {
			if (inout_ptr == 0) { inout_ptr = ::calloc(in_num, in_size); }
			else {
				inout_ptr = ::realloc(inout_ptr, in_num*in_size);
				::memset(inout_ptr, 0, in_num*in_size);
			}
			return inout_ptr;
		}

		/**
		 * @brief Compare two blocks of memory for equality.
		 * This method returns 0 if the blocks are equal, > 0 if
		 * the first non-equal value (treated as an unsigned char) in 
		 * the first block is greater than the second, and < 0 for the opposite.
		 * @param in_ptr1 The first block of memory to test.
		 * @param in_ptr2 The second block of memory to test.
		 * @param in_bytes The number of bytes to test in each block.
		 * @return 0 for equal, >0 for greator, <0 for less than.
		 */
		CX_FORCE_INLINE CxI32 compare(const void *CX_RESTRICT in_ptr1,
												const void *CX_RESTRICT in_ptr2, CxU32 in_bytes) {
			return memcmp(in_ptr1, in_ptr2, in_bytes);
		}

		/**
		 * @brief Copy memory from one location to another.
		 * @param in_dest The destination to copy to.
		 * @param in_src The memory location to copy from.
		 * @param in_bytes The number of bytes to copy.
		 * @return A pointer to the destination parameter.
		 */
		CX_FORCE_INLINE void * copy(void *CX_RESTRICT inout_dest,
											 const void *CX_RESTRICT in_src, CxU32 in_bytes) {
			return memcpy(inout_dest, in_src, in_bytes);
		}

		/**
		 * @brief Test if the two blocks of memory are equal or not.
		 * @param in_ptr1 The first block of memory to test.
		 * @param in_ptr2 The second block of memory to test.
		 * @param in_bytes The number of bytes to test in each block.
		 * @return True if the two blocks of memory have equal contents.
		 */
		CX_FORCE_INLINE CxBool eq(const void *CX_RESTRICT in_ptr1,
										  const void *CX_RESTRICT in_ptr2, CxU32 in_bytes) {
			return memcmp(in_ptr1, in_ptr2, in_bytes) == 0;
		}

		/**
		 * @brief Find the first occurance of a value in the memory block.
		 * @param in_ptr The pointer to the memory to search through.
		 * @param in_value The value to look for in the memory block.
		 * @param in_bytes THe number of bytes to search.
		 * @return A pointer to the first occurance of the value, or null if not found.
		 */
		CX_FORCE_INLINE void * find(void *CX_RESTRICT in_ptr, CxUByte in_value, CxU32 in_bytes) {
			return memchr(in_ptr, in_value, in_bytes);
		}

		/**
		 * @brief const version of mem::find(...).
		 * @see mem::find(void*, CxUByte, CxU32).
		 */
		CX_FORCE_INLINE const void * find(const void *CX_RESTRICT in_ptr, CxUByte in_value, CxU32 in_bytes) {
			return memchr(in_ptr, in_value, in_bytes);
		}

		/**
		 * @brief Free the specified memory.
		 * This method can handle null pointers.
		 * @param in_ptr The pointer to the allocated memory.
		 */
		CX_FORCE_INLINE void free(void *&in_ptr) {
			if (in_ptr) { ::free(in_ptr); in_ptr = 0; }
		}

		/**
		 * @brief Test two blocks of memory for inequality.
		 * This method tests each block of memory as an array of unsigned char's, and 
		 * returns whether or not the first none-equal block (if there is one) in ptr1 
		 * evaluates to greater than the block in ptr2 as unsigned char's.
		 * @param in_ptr1 The first block of memory to test.
		 * @param in_ptr2 The second block of memory to test.
		 * @param in_bytes The number of bytes to test in each block.
		 * @return True if the first block is 'greater' than the second block.
		 */
		CX_FORCE_INLINE CxBool greater(const void *CX_RESTRICT in_ptr1,
												 const void *CX_RESTRICT in_ptr2, CxU32 in_bytes) {
			return memcmp(in_ptr1, in_ptr2, in_bytes) > 0;
		}

		/**
		 * @brief Test two blocks of memory for inequality.
		 * This method tests each block of memory as an array of unsigned char's, and 
		 * returns whether or not the first none-equal block (if there is one) in ptr1 
		 * evaluates to less than the block in ptr2 as unsigned char's.
		 * @param in_ptr1 The first block of memory to test.
		 * @param in_ptr2 The second block of memory to test.
		 * @param in_bytes The number of bytes to test in each block.
		 * @return True if the first block is 'less' than the second block.
		 */
		CX_FORCE_INLINE CxBool less(const void *CX_RESTRICT in_ptr1,
												 const void *CX_RESTRICT in_ptr2, CxU32 in_bytes) {
			return memcmp(in_ptr1, in_ptr2, in_bytes) < 0;
		}

		/**
		 * @brief Copies memory from src to dest.  
		 * This method uses an intermediate buffer to copy the memory to so that
		 * if the src and destination overlap, the copy still works.
		 * @param inout_dest The location to copy the memory to.
		 * @param in_src The location to copy from.
		 * @param in_bytes The number of bytes to copy.
		 * @return Returns the destination pointer.
		 */
		CX_FORCE_INLINE void * move(void *inout_dest, const void *in_src, CxU32 in_bytes) {
			return memmove(inout_dest, in_src, in_bytes);
		}

		/**
		 * @brief Resize previously allocated memory.
		 * @param inout_ptr The pointer to the memory to reallocate.
		 * @param in_bytes The new size of the memory to allocate.
		 * @return Returns the pointer inout_ptr.
		 */
		CX_FORCE_INLINE void * resize(void *&inout_dest, CxU32 in_bytes) {
			inout_dest = ::realloc(inout_dest, in_bytes);
			return inout_dest;
		}

		/**
		 * @brief Set the memory to the the specified integer value.
		 * @param in_ptr The pointer to the memory to set.
		 * @param in_value The value to set the memory to.
		 * @param in_bytes The number of bytes to set.
		 * @return Returns the pointer to the memory, in_ptr.
		 */
		CX_FORCE_INLINE void set(void *CX_RESTRICT inout_ptr, CxI32 in_value, CxU32 in_bytes) {
			return memset(inout_ptr, in_value, in_bytes);
		}

		/**
		 * @brief Sets the memory to all zeros.
		 * @param inout_ptr The pointer to the memory to set to zeros.
		 * @param in_bytes The number of bytes to set to zeros.
		 * @return Returns the input pointer.
		 */
		CX_FORCE_INLINE void zero(void *CX_RESTRICT inout_ptr, CxU32 in_bytes) {
			return memset(inout_ptr, 0, in_bytes);
		}
		
	} // namespace mem
} // namespace cat


#endif // CX_CORE_COMMON_CXMEM_H
