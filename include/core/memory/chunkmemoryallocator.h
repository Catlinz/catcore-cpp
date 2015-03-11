#ifndef CAT_CORE_MEMORY_CHUNKMEMORYALLOCATOR_H
#define CAT_CORE_MEMORY_CHUNKMEMORYALLOCATOR_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * chunkmemoryallocator.h: Contains the ChunkMemoryAllocator class, which is similiar to 
 * the ChunkMemoryAllocator class, except the size data being stored can be smaller than the 
 * blocks, and they (may) have to be aligned for each object.
 *
 * Author: Catlin Zilinski
 * Date: Sept 23, 2013
 */

#include "core/memory/memoryallocator.h"

namespace Cat {

		/**
	 * The ChunkMemoryAllocator class is used when allocating and deallocating blocks of the same 
	 * size (e.g., all the same type of object).  
	 * BLOCK SIZE MUST BE GREATER THAN OR EQUAL TO SIZE OF A POINTER AND A POWER OF 2
	 */
	class ChunkMemoryAllocator : public MemoryAllocator {
		public:
			/**
			 * Creates a new MemoryAllocator that has a certain number of fixed size blocks available.
			 * Each block will be aligned to the size of the block.
			 * @param block_size The size each block will be, in bytes (must be a power of two).
			 * @param number_of_blocks The number of blocks to allocate.
			 */
			ChunkMemoryAllocator(U32 chunk_size, U32 number_of_chunks, OID id = 0);
			~ChunkMemoryAllocator();

			/**
			 * Is an error to call alloc() with no parameters for ChunkMemoryAllocator.
			 */
			VPtr alloc();
			/**
			 * Allocate a new object in a block of memory.  The block_size must be 
			 * smaller than the size of each chunk.  Also, if the blocksize + calculated alignment offset 
			 * must be <= the chunk size.
			 * @param block_size The size of the object being allocated.
			 * @param alignment The alignment required for the object.
			 * @return A pointer to the memory for the object.
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

			/**
			 * checks to see whether or not a specified piece of memory lies within this 
			 * allocators control.
			 * @param block The block of memory to test.
			 * @return True if the memory belongs to this allocator.
			 */
			inline Boolean doesOwnMemory(VPtr block) const;


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

	inline U32 ChunkMemoryAllocator::getBlockSize() const {
		return block_size_; 
	}
	inline U32 ChunkMemoryAllocator::getNumberOfBlocks() const { 
		return number_of_blocks_;
	}
	inline MemAddr ChunkMemoryAllocator::getNextBlock() const {
		return next_block_; 
	}
	inline MemAddr ChunkMemoryAllocator::getAlignedMemoryBlock() const { 
		return aligned_memory_block_;
	}
	inline MemAddr ChunkMemoryAllocator::getUnalignedMemoryBlock() const { 
		return unaligned_memory_block_; 
	}

	inline Boolean ChunkMemoryAllocator::doesOwnMemory(VPtr block) const {
		return ((Addr)block >= aligned_memory_block_.addr && (Addr)block < (aligned_memory_block_.addr + (block_size_*number_of_blocks_)));	
	}



} // namespace Cat

#endif // CAT_CORE_MEMORY_CHUNKMEMORYALLOCATOR_H
	

