#include <cstring>
#include "core/memory/memorymanager.h"
#include "core/memory/poolmemoryallocator.h"
#include "core/memory/stackmemoryallocator.h"
#include "core/memory/chunkmemoryallocator.h"
#include "core/memory/dynamicchunkmemoryallocator.h"

namespace Cat {

	/**
	 * Initializes the array of MemoryAllocator*'s to 1 + 32 because we don't use the 
	 * first element since an OID shouldn't be zero, so we can return zero as an error.
	 */
	MemoryManager::MemoryManager() {
		// Initialize to 32+1 because we don't use the first element of the array.
		allocator_list_ = new MemoryAllocator*[32+1];
		list_size_ = 32+1;
		// Initialize list to all NIL pointers
		memset(allocator_list_, NIL, sizeof(MemoryAllocator*)*list_size_);
		length_ = 0;	
	}

	/**
	 * Initializes the array of MemoryAllocator*'s to 1 + max_allocators because we don't use the 
	 * first element since an OID shouldn't be zero, so we can return zero as an error.
	 */

	MemoryManager::MemoryManager(U32 max_allocators) {
		// Initialize to max_allocators+1 because can never have an OID of 0.
		allocator_list_ = new MemoryAllocator*[max_allocators+1];
		list_size_ = max_allocators+1;
		// Initialize list to all NIL pointers
		memset(allocator_list_, NIL, sizeof(MemoryAllocator*)*list_size_);
		length_ = 0;
	}

	/**
	 * Deletes all the non-NIL entries in the allocator list and the list itself.
	 */
	MemoryManager::~MemoryManager() {
		for(U32 i = 0; i < list_size_; i++) {
			if (allocator_list_[i]) {
				delete allocator_list_[i];
				allocator_list_[i] = NIL;
			}
		}
		delete allocator_list_;
		allocator_list_ = NIL;

	}

	/**
	 * Creates the singleton instance with the specified number of max_allocators. 
	 * @param max_allocators Defaults to 32
	 */
	void MemoryManager::initializeMemoryManagerInstance(U32 max_allocators) {
		if (!singleton_instance_) {
			singleton_instance_ = new MemoryManager(max_allocators);
		} else {
			DWARN("Cannot intialize singleton MemoryManager class more than once!");
		}
	}

	/**
	 * Destroys the one singleton instance of the MemoryManager.
	 */
	void MemoryManager::destroyMemoryManagerInstance() {
		if (singleton_instance_) {
			delete singleton_instance_;
			singleton_instance_ = NIL;
		} else {
			DWARN("Cannot destroy already destroyed MemoryManager singleton class!");
		}
	}

	/**
	 * Creates a new PoolAllocator and returns the OID for it.  The OID is zero only on error.
	 */
	OID MemoryManager::createPoolAllocator(U32 block_size, U32 number_of_blocks, U32 alignment) {
		if (length_ >= list_size_-1) {
			DWARN("To Many Allocators already created (" << length_ << ").  Try increasing the number of max_allocators?");
			return 0;
		}
		OID id = getNextFreeObjectId();
		if (id == 0) {
			DERR("getNextFreeObjectId() returned 0, but should have room (length = " << length_ << ").  Something not good.");
			return 0;
		}

		allocator_list_[id] = new PoolMemoryAllocator(block_size, number_of_blocks, alignment, id);
		length_++;
		return id;
	}

	/**
	 * Creates a new StackAllocator and returns the OID for it.  The OID is zero only on error.
	 */
	OID MemoryManager::createStackAllocator(U32 stack_size) {
		if (length_ >= list_size_-1) {
			DWARN("To Many Allocators already created (" << length_ << ").  Try increasing the number of max_allocators?");
			return 0;
		}
		OID id = getNextFreeObjectId();
		if (id == 0) {
			DERR("getNextFreeObjectId() returned 0, but should have room (length = " << length_ << ").  Something not good.");
			return 0;
		}

		allocator_list_[id] = new StackMemoryAllocator(stack_size, id);
		length_++;
		return id;
	}

	OID MemoryManager::createChunkAllocator(U32 chunk_size, U32 number_of_chunks) {
		if (length_ >= list_size_-1) {
			DWARN("To Many Allocators already created (" << length_ << ").  Try increasing the number of max_allocators?");
			return 0;
		}
		OID id = getNextFreeObjectId();
		if (id == 0) {
			DERR("getNextFreeObjectId() returned 0, but should have room (length = " << length_ << ").  Something not good.");
			return 0;
		}

		allocator_list_[id] = new ChunkMemoryAllocator(chunk_size, number_of_chunks, id);
		length_++;
		return id;

	}

	OID MemoryManager::createDynamicChunkAllocator(U32 default_number_of_chunks) {
		if (length_ >= list_size_-1) {
			DWARN("To Many Allocators already created (" << length_ << ").  Try increasing the number of max_allocators?");
			return 0;
		}
		OID id = getNextFreeObjectId();
		if (id == 0) {
			DERR("getNextFreeObjectId() returned 0, but should have room (length = " << length_ << ").  Something not good.");
			return 0;
		}

		allocator_list_[id] = new DynamicChunkMemoryAllocator(default_number_of_chunks, id);
		length_++;
		return id;

	}


	/**
	 * Deletes the allocator corresponding to the OID passed in. 
	 */
	void MemoryManager::free(OID allocator_id) {
		if (allocator_id < 1 || allocator_id > list_size_) {
			DERR("MemoryAllocator ID (" << allocator_id << ") is invalid!");
			return;
		}

		if (!allocator_list_[allocator_id]) {
			DWARN("Allocator " << allocator_id << " has already been free'd!");
			return;
		}

		delete allocator_list_[allocator_id];
		allocator_list_[allocator_id] = NIL;
		length_--;
	}

	/**
	 * Deletes the allocator passed in. 
	 */
	void MemoryManager::free(MemoryAllocator* allocator) {
		free(allocator->getOID());
	}


	U32 MemoryManager::getMaxAllocators() const {
		return list_size_-1;
	}


	/**
	 * Finds the next available object id to return.
	 * @return The next available object id spot.
	 */
	OID MemoryManager::getNextFreeObjectId() {
		for(OID id = 1; id < list_size_; id++) {
			if (!allocator_list_[id]) {
				return id;
			}
		}
		return 0;
	}

	MemoryManager* MemoryManager::singleton_instance_ = NIL; 


} // namespace Cat
