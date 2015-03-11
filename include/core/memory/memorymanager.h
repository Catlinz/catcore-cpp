#ifndef CAT_CORE_MEMORY_MEMORYMANAGER_H
#define CAT_CORE_MEMORY_MEMORYMANAGER_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * memorymanager.h: Contains the Memory Management classes.
 *
 * The MemoryManager class allows for custom memory managment by creating 
 * and storing new MemoryAllocator's which contain methods to allocate and 
 * deallocate objects.
 *
 * Author: Catlin Zilinski
 * Date: May 19, 2013
 */
#include "core/memory/memoryallocator.h"

namespace Cat {
	
	/**
	 * The MemoryManager class is a singleton that contains methods to create and store different 
	 * types of MemoryAllocator's, which allow for memory allocation without 
	 * repeated calls to new or free.
	 *
	 * The MemoryManager should be one of the first classes initialized.
	 */
	class MemoryManager {
		public:
			/**
			 * Initializes the MemoryManager with 32 max allocators.
			 */
			MemoryManager();
			/**
			 * Initializes the MemoryManager with the specified number of max_allocators.
			 * @param max_allocators The max number of allocators to have.
			 */
			MemoryManager(U32 max_allocators);
			/**
			 * Destroys all the allocators.
			 */
			~MemoryManager();

			// Initializes and stores the singleton instance.
			static void initializeMemoryManagerInstance(U32 max_allocators = 32);
			// Destroys the singleton instance of the class.
			static void destroyMemoryManagerInstance();
			
			// Gets the one instance of the class
			static inline MemoryManager* getInstance();


			/**
			 * Creates a new PoolMemoryAllocator to use in allocating new objects.
			 * @param block_size The size of each block for the allocator to return
			 * @param number_of_blocks The number of blocks able to allocate
			 * @param alignment The memory Alignment factor for the blocks
			 * @return A nonzero ID for the allocator to use with the get() instance method.
			 */
			OID createPoolAllocator(U32 block_size, U32 number_of_blocks, U32 alignment);

			/**
			 * Creates a new StackMemoryAllocator to use in allocating new objects.
			 * @param stack_size The amount of memory in bytes to allocate for the stack.
			 * @return A nonzero ID for the allocator to use with the get() instance method.
			 */
			OID createStackAllocator(U32 stack_size);

			/**
			 * Creates a new ChunkMemoryAllocator to use in allocating new objects.
			 * @param chunk_size The size of each block of memory
			 * @param number_of_chunks The total number of blocks available.
			 * @return A nonzero ID for the allocator to use with the get() instance method.
			 */
			OID createChunkAllocator(U32 chunk_size, U32 number_of_chunks);

			/**
			 * Creates a new DynamicChunkMemoryAllocator to use in allocating new objects.
			 * @param default_number_of_chunks The default number of blocks to allocate.
			 * @return A nonzero ID for the allocator to use with the get() instance method.
			 */
			OID createDynamicChunkAllocator(U32 default_number_of_chunks = 32);

			/**
			 * Frees the memory used by the allocator and removes it from the list.
			 * @param allocator_id The OID of the allocator to remove
			 */
			void free(OID allocator_id);

			/**
			 * Frees the memory used by the allocator and removes it from the list.
			 * @param allocator The MemoryAllocator to remove.
			 */
			void free(MemoryAllocator* allocator);

			/**
			 * Retreives the MemoryAllocator with the specified Object Id.
			 * @param object_id The Id of the Allocator to get.
			 * @return A pointer to the MemoryAllocator object or NIL on error.
			 */
			inline MemoryAllocator* get(OID object_id);

			/**
			 * Returns the maximum number of memory allocators that can be allocated.
			 * @return Max number of allocators.
			 */
			U32 getMaxAllocators() const;


			
			friend inline U32 len(MemoryManager* manager);

		private:
			static MemoryManager* singleton_instance_;

			MemoryAllocator** allocator_list_; 
			U32 list_size_;	// Max allocators + 1 (0 not used)
			U32 length_;		// Total number of active allocators

			OID getNextFreeObjectId();
			

	};

	inline MemoryManager* MemoryManager::getInstance() {
		#if defined (DEBUG)
		if (!singleton_instance_) {
			DWARN("Getting singleton instance of UNINITIALIZED MemoryManager!");
		}
		#endif
		return singleton_instance_;
	}

	inline MemoryAllocator* MemoryManager::get(OID object_id) {
		if (object_id < 1 || object_id >= list_size_) {
			DWARN("Object ID " << object_id << " out of bounds!  Must be between 1 and " << list_size_ - 1 << "!");
			return NIL;
		}
		return allocator_list_[object_id];
	}

	inline U32 len(MemoryManager* manager) {
		return manager->length_;
	}

}


#endif // CAT_CORE_MEMORY_MEMORYMANAGER_H

