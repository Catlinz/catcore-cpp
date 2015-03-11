#ifndef CAT_CORE_MEMORY_DYNAMICCHUNKMEMORYALLOCATOR_H
#define CAT_CORE_MEMORY_DYNAMICCHUNKMEMORYALLOCATOR_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * dynamicchunkmemoryallocator.h: Contains the DynamicChunkMemoryAllocator class, which basically 
 * contains a list of ChunkMemoryAllocators, and picks the best one based on the size, and if there's not 
 * one big enough, it trys to create a new one.
 *
 * Author: Catlin Zilinski
 * Date: Sept 25, 2013
 */

#include "core/memory/memoryallocator.h"

namespace Cat {

	class ChunkMemoryAllocator;

	struct DCAllocatorNode {
		ChunkMemoryAllocator* 	allocator;
		DCAllocatorNode*			next;
		DCAllocatorNode*			prev;
	};

	/**
	 * The DynamicChunkMemoryAllocator class is used when allocating and deallocating blocks of the same 
	 * size (e.g., all the same type of object).  
	 * BLOCK SIZE MUST BE GREATER THAN OR EQUAL TO SIZE OF A POINTER AND A POWER OF 2
	 */
	class DynamicChunkMemoryAllocator : public MemoryAllocator {
		public:
			/**
			 * Creates a new, empty DynamicChunkMemoryAllocator with no allocators yet.
			 * @param default_number_of_chunks The default number of chunks to give an allocator.
			 * @param id The OID of the allocator (only set by MemoryManager).
			 */
			DynamicChunkMemoryAllocator(U32 default_number_of_chunks = 32, OID id = 0);
			~DynamicChunkMemoryAllocator();

			/**
			 * Is an error to call alloc() with no parameters for DynamicChunkMemoryAllocator.
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
			 * Simply resets the state of all the allocators.
			 */
			void reset();
			/**
			 * Frees all the memory associated with this MemoryAllocator.  
			 * Unlike most MemoryAllocators, this one can be used after it is free'd by adding 
			 * new allocators again.
			 */
			void free();

			/**
			 * @see MemoryAllocator::getOID()
			 */
			OID getOID();

			/**
			 * Add a new ChunkAllocator to the available allocators.
			 * @param chunk_size The size of each block of memory.
			 * @param number_of_chunks The number of memory blocks.
			 * @return True The created ChunkMemoryAllocator if successfull, else, NIL.
			 */
			ChunkMemoryAllocator* addChunk(U32 chunk_size, U32 number_of_chunks);

			/**
			 * Remove a ChunkAllocator from the available allocators.
			 * @param chunk_size: The block size of the allocator we are removing.
			 */
			void freeChunk(U32 chunk_size);

			/**
			 * Gets the ChunkAllocator associated with the block size.
			 */
			ChunkMemoryAllocator* getChunk(U32 chunk_size);

			inline U32 getNumberOfAllocators() const;
			inline U32 getDefaultNumberOfBlocks() const;
			Boolean canFit(U32 block_size) const;
		private:
			static inline U32 getNextHighestPowerOfTwo(U32 val);

			U32							number_of_allocators_;
			U32							default_number_of_blocks_;
			ChunkMemoryAllocator*	last_accessed_;
			DCAllocatorNode*			allocators_;
			OID							id_;
	};

	inline U32 DynamicChunkMemoryAllocator::getNumberOfAllocators() const { 
		return number_of_allocators_;
	}
	inline U32 DynamicChunkMemoryAllocator::getDefaultNumberOfBlocks() const {
		return default_number_of_blocks_;
	}
	
	inline U32 DynamicChunkMemoryAllocator::getNextHighestPowerOfTwo(U32 val) {
		val--;
		val |= val >> 1;
		val |= val >> 2;
		val |= val >> 4;
		val |= val >> 8;
		val |= val >> 16;
		return (val+1);
	}

	
} // namespace Cat

#endif // CAT_CORE_MEMORY_DYNAMICCHUNKMEMORYALLOCATOR_H
	


