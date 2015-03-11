#ifndef CAT_CORE_MEMORY_MEMORYALLOCATOR_H
#define CAT_CORE_MEMORY_MEMORYALLOCATOR_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * memoryallocator.h: Contains the MemoryAllocator interface and the various
 * MemoryAllocator class types.
 *
 * The MemoryAllocator classes are used to be able to quickly allocate memory without 
 * repeated use of the new and delete operators.
 * Author: Catlin Zilinski
 * Date: May 19, 2013
 */

#include <new>
#include "core/corelib.h"

namespace Cat {
	
	/**
	 * The MemoryAllocator interface defines the basic methods required for a 
	 * MemoryAllocator to implement, namly, allocate, deallocate.
	 */
	class MemoryAllocator {
		public:

			virtual ~MemoryAllocator() {}
			/**
			 * Allocates a new block of memory from the MemoryAllocator.
			 * Can be an error to simply call it if the MemoryAllocator does not suport 
			 * default or fixed size blocks.
			 * @return A pointer to the block of memory, or null on error.
			 */
			virtual VPtr alloc() = 0;
			
			/**
			 * Allocates a new block of memory of a specified size.
			 * @param block_size The size of the block in bytes to allocate.
			 * @return A pointer to the block of memory, or null on error.
			 */
			virtual VPtr alloc(U32 block_size, U32 alignment) = 0;
			
			/**
			 * Marks previously allocated memory as free'd
			 */
			virtual void dealloc(VPtr memory_block) = 0;
			virtual void dealloc() = 0;
			/**
			 * Resets the MemoryAllocator to its default state (as if all blocks were free'd)
			 */
			virtual void reset() = 0;
			/**
			 * Frees all memory associated with the MemoryAllocator
			 */
			virtual void free() = 0;

			/**
			 * Gets the id associated with this allocator if created by a MemoryManager.
			 * @return the OID of the allocator or 0 if none.
			 */
			virtual OID getOID() = 0;

			/**
			 * Returns a aligned memory address
			 * @param address The Address to align
			 * @param alignment The memory alignment to use
			 * @return The aligned memory address.
			 */
			static inline MemAddr getAlignedMemoryAddress(MemAddr addr, U32 alignment);
			
	};

	inline MemAddr MemoryAllocator::getAlignedMemoryAddress(MemAddr addr, U32 alignment)  {
		// If is already aligned, return it
		D(assert(alignment > 0));
		if (addr.addr % alignment == 0) return addr;
		// Calculate and return the adjusted block address
		addr.addr += (alignment - ((alignment - 1) & addr.addr));
		return addr;

	}

}

/**
 * The overloaded positional new operator to allocate memory from a 
 * MemoryAllocator
 */
inline void* operator new(size_t nbytes, cc::MemoryAllocator& allocator) {
	return allocator.alloc(nbytes, 0);
}
inline void* operator new(size_t nbytes, cc::MemoryAllocator& allocator, cc::U32 alignment) {
	return allocator.alloc(nbytes, alignment);
}


#endif // CAT_CORE_MEMORY_MEMORYALLOCATOR_H

