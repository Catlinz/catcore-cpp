#ifndef CAT_CORE_MEMORY_STACKMEMORYALLOCATOR_H
#define CAT_CORE_MEMORY_STACKMEMORYALLOCATOR_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * stackmemoryallocator.h: Contains the StackMemoryAllocator class, used to quickly 
 * allocated memory blocks of different sizes in a stack based manor.
 *
 * Author: Catlin Zilinski
 * Date: Sept 20, 2013
 */

#include "core/memory/memoryallocator.h"

namespace Cat {

	/**
	 * The StackMemoryAllocator class is used when allocating and deallocating blocks of different 
	 * sizes.  When deallocating, can only deallocate to a set marker. 
	 */
	class StackMemoryAllocator : public MemoryAllocator {
		public:
			/**
			 * Creates a new MemoryAllocator of a certain size to create a stack from.
			 */
			StackMemoryAllocator(U32 stack_size, OID id = 0);
			~StackMemoryAllocator();

			/**
			 * Allocates a block of memory with the specified alignment
			 * @block_size The size of memory block to allocate
			 * @alignment The memory alignment for the block
			 */
			VPtr alloc(U32 block_size, U32 alignment);
			/**
			 * Is an error to call alloc with no paramaters for StackMemoryAllocator
			 */
			VPtr alloc();

			void dealloc();
			/**
			 * Is an error to call dealloc with memory block for StackMemoryAllocator
			 */
			void dealloc(VPtr memory_block);

			/**
			 * Resets the MemoryAllocator to its initial state.
			 * The list of memory blocks is rebuilt, has the same effect as if every allocated
			 * block of memory was free'd.
			 */
			void reset();
			/**
			 * Frees all the memory associated with this MemoryAllocator.  
			 * The MemoryAllocator can no longer be used after free() is called.
			 */
			void free();

			/**
			 * Marks a point to where the next dealloc call will deallocate to.
			 */
			void mark();

			/**
			 * @see MemoryAllocator::getOID()
			 */
			OID getOID();

			inline U32 getStackSize() const;
			inline MemAddr getNextBlock() const;
		private:
			MemAddr	next_block_;	
			MemAddr	marker_;
			MemAddr	unaligned_memory_block_;
			U32		stack_size_;
			OID 		id_;

	};

	inline U32 StackMemoryAllocator::getStackSize() const { 
		return stack_size_;
	}
	inline MemAddr StackMemoryAllocator::getNextBlock() const {
		return next_block_;
	}

} // namespace Cat

#endif // CAT_CORE_MEMORY_STACKMEMORYALLOCATOR_H
	


