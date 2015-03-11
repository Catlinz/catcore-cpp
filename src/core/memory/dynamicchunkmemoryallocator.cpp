#include "core/memory/dynamicchunkmemoryallocator.h"
#include "core/memory/chunkmemoryallocator.h"

namespace Cat {

	DynamicChunkMemoryAllocator::DynamicChunkMemoryAllocator(U32 default_number_of_chunks, OID id) {
		number_of_allocators_ = 0;
		default_number_of_blocks_ = default_number_of_chunks;
		id_ = id;
	}

	DynamicChunkMemoryAllocator::~DynamicChunkMemoryAllocator() {
		DOUT("Calling DYNAMICCHUNKMemoryAllocator DESTRUCTOR\n");
		free();		
	}

	VPtr DynamicChunkMemoryAllocator::alloc() {
		DERR("Cannot call DynamicChunkMemoryAllocator::alloc() with no arguments!");
		return NIL;
	}

	VPtr DynamicChunkMemoryAllocator::alloc(U32 block_size, U32 alignment) {
		// Check to see if there are no allocators, if not, create one.
		if (allocators_ == NIL) {
			addChunk(block_size, default_number_of_blocks_);
			last_accessed_ = allocators_->allocator;
		}
		// Check the most recently used allocator.
		if (last_accessed_->getBlockSize() > block_size && last_accessed_->getBlockSize() < block_size*2) {
			return last_accessed_->alloc(block_size, alignment);
		}

		// Otherwise, check for an appropriate allocator.
		DCAllocatorNode* ptr = allocators_;
		while (ptr) {
			if (ptr->allocator->getBlockSize() >= block_size) {
				if (ptr->allocator->getBlockSize() < block_size*2) {
					return ptr->allocator->alloc(block_size, alignment);
				}
				break;
			}
			ptr = ptr->next;
		}

		ChunkMemoryAllocator* allocator = addChunk(block_size, default_number_of_blocks_);
		if (allocator) {
			last_accessed_ = allocator;
			return allocator->alloc(block_size, alignment);		
		}
		DERR("Could not find correctly sized ChunkMemoryAllocator and could not create a new one!");
		return NIL;

	}

	void DynamicChunkMemoryAllocator::dealloc(VPtr memory_block) {
		// Check to see if it was from the most recently accessed.
		if (last_accessed_->doesOwnMemory(memory_block)) {
			last_accessed_->dealloc(memory_block);
			return;
		}

		// Find where it belongs otherwise.
		DCAllocatorNode* ptr = allocators_;
		while (ptr) {
			if (ptr->allocator->doesOwnMemory(memory_block)) {
				ptr->allocator->dealloc(memory_block);
				return;
			}
			ptr = ptr->next;
		}

		DERR("Could not find ChunkMemoryAllocator that owns the memory block!");
	}

	void DynamicChunkMemoryAllocator::dealloc() {
		DERR("Is an error to call DynamicChunkMemoryAllocator::dealloc() with no arguments!");
	}

	void DynamicChunkMemoryAllocator::reset() {
		DCAllocatorNode* ptr = allocators_;
		while (ptr) {
			ptr->allocator->reset();
			ptr = ptr->next;
		}
	}

	void DynamicChunkMemoryAllocator::free() {
		DCAllocatorNode* ptr = allocators_;
		while (ptr) {
			allocators_ = ptr->next;
			delete ptr->allocator;
			delete ptr;
			ptr = allocators_;
		}
		number_of_allocators_ = 0;
		last_accessed_ = NIL;
		allocators_ = NIL;
	}

	OID DynamicChunkMemoryAllocator::getOID() {
		return id_;
	}

	ChunkMemoryAllocator* DynamicChunkMemoryAllocator::addChunk(U32 chunk_size, U32 number_of_chunks) {
		if (chunk_size < sizeof(VPtr)) {
			DERR("Cannot allocate a chunk smaller than sizeof(VPtr) = " << sizeof(VPtr) << "!");
			return NIL;
		}

		chunk_size = getNextHighestPowerOfTwo(chunk_size);

		DCAllocatorNode* node = new DCAllocatorNode();
		node->allocator = ::new ChunkMemoryAllocator(chunk_size, number_of_chunks);

		// If there are no allocators yet, then make it the first one.
		if (allocators_ == NIL) {
			allocators_ = node;
			node->next = node->prev = NIL;
			last_accessed_ = node->allocator;
			number_of_allocators_++;
			return node->allocator;
		}

		// Else, put it in the list, in order.
		DCAllocatorNode* ptr = allocators_;
		DCAllocatorNode* prev = NIL;
		while(ptr) {
			if (ptr->allocator->getBlockSize() > chunk_size) {
				break;
			}
			prev = ptr;
			ptr = ptr->next;
		}
		if (prev == NIL) { /* Is the smallest allocator yet, so goes on the front of the list */
			node->prev = NIL;
			node->next = allocators_;
			allocators_->prev = node;
			allocators_ = node;
		} else if (ptr == NIL) { /* Largest yet, at end of the list */
			node->next = NIL;
			node->prev = prev;
			prev->next = node;
		} else {
			node->prev = ptr->prev;
			node->next = ptr;
			prev->next = node;
			ptr->prev = node;
		}
		number_of_allocators_++;
		return node->allocator;
	}

	void DynamicChunkMemoryAllocator::freeChunk(U32 chunk_size) {
		chunk_size = getNextHighestPowerOfTwo(chunk_size);
		DCAllocatorNode* ptr = allocators_;
		while (ptr) {
			if (ptr->allocator->getBlockSize() == chunk_size) {
				break;
			}
			ptr = ptr->next;
		}
		if (!ptr) {
			DERR("Allocator with chunk_size = " << chunk_size << " not found!");
			return;
		}
		
		if (ptr->next) {
			ptr->next->prev = ptr->prev;
		}
		if (ptr->prev) {
			ptr->prev->next = ptr->next;
		}

		if (allocators_->allocator->getBlockSize() == ptr->allocator->getBlockSize()) {
			allocators_ = ptr->next;
		}

		if (last_accessed_->getBlockSize() == ptr->allocator->getBlockSize()) {
			if (allocators_) {
				last_accessed_ = allocators_->allocator;
			} else {
				last_accessed_ = NIL;
			}
		}

		delete ptr->allocator;
		delete ptr;
		number_of_allocators_--;
	}

	ChunkMemoryAllocator* DynamicChunkMemoryAllocator::getChunk(U32 chunk_size) {
		chunk_size = getNextHighestPowerOfTwo(chunk_size);
		DCAllocatorNode* ptr = allocators_;
		while (ptr) {
			if (ptr->allocator->getBlockSize() == chunk_size) {
				break;
			}
			ptr = ptr->next;
		}
		if (!ptr) { return NIL; }
		return ptr->allocator;
	}

	Boolean DynamicChunkMemoryAllocator::canFit(U32 block_size) const {
		DCAllocatorNode* ptr = allocators_;	
		while (ptr) { 
			if (ptr->allocator->getBlockSize() >= block_size) { 
				return true; 
			} 
			ptr = ptr->next; 
		}
		return false;
	}


} // namespace Cat
