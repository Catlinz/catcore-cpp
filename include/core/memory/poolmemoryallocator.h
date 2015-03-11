#ifndef CAT_CORE_MEMORY_POOLMEMORYALLOCATOR_H
#define CAT_CORE_MEMORY_POOLMEMORYALLOCATOR_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * poolmemoryallocator.h: Contains the PoolMemoryAllocator class, used to quickly 
 * allocated memory blocks of the same size.
 *
 * Author: Catlin Zilinski
 * Date: Sept 20, 2013
 */

#include "core/memory/memoryallocator.h"

namespace Cat {

		/**
	 * The PoolMemoryAllocator class is used when allocating and deallocating blocks of the same 
	 * size (e.g., all the same type of object).  
	 * BLOCK SIZE MUST BE GREATER THAN OR EQUAL TO SIZE OF A POINTER
	 */
	class PoolMemoryAllocator : public MemoryAllocator {
		public:
			/**
			 * Creates a new MemoryAllocator that has a certain number of fixed size blocks available.
			 * Each block will be aligned.
			 * @param block_size The size each block will be, in bytes.
			 * @param number_of_blocks The number of blocks to allocate.
			 * @param block_alignment The alignment we need to make sure each block is aligned in memory. (Must be Power of 2)
			 */
			PoolMemoryAllocator(U32 block_size, U32 number_of_blocks, U32 block_alignment, OID id = 0);
			~PoolMemoryAllocator();

			/**
			 * Allocates a block of memory from the list of free blocks.
			 * @return A pointer to the block of memory, or null on error
			 */
			VPtr alloc();
			/**
			 * block_size parameter is ignored.
			 * @see allocate();
			 */
			VPtr alloc(U32 block_size, U32 alignment);

			void dealloc(VPtr memory_block);
			void dealloc();
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
			 * @see MemoryAllocator::getOID()
			 */
			OID getOID();


			inline U32 getBlockSize() const;
			inline U32 getNumberOfBlocks() const;
			inline MemAddr getNextBlock() const;
			inline MemAddr getAlignedMemoryBlock() const;
			inline MemAddr getUnalignedMemoryBlock() const;
		private:
			MemAddr	next_block_;	
			MemAddr	aligned_memory_block_;
			MemAddr	unaligned_memory_block_;
			U32		block_size_;
			U32		number_of_blocks_;
			OID		id_;
	};

	inline U32 PoolMemoryAllocator::getBlockSize() const {
		return block_size_; 
	}
	inline U32 PoolMemoryAllocator::getNumberOfBlocks() const { 
		return number_of_blocks_;
	}
	inline MemAddr PoolMemoryAllocator::getNextBlock() const {
		return next_block_; 
	}
	inline MemAddr PoolMemoryAllocator::getAlignedMemoryBlock() const { 
		return aligned_memory_block_;
	}
	inline MemAddr PoolMemoryAllocator::getUnalignedMemoryBlock() const { 
		return unaligned_memory_block_; 
	}



} // namespace Cat

#endif // CAT_CORE_MEMORY_POOLMEMORYALLOCATOR_H
	

