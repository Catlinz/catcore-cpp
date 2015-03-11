#include <cstdlib>
#include "core/memory/poolmemoryallocator.h"

namespace Cat {

	/**
 	 * The PoolMemoryAllocator constructor creates a new Linked List of 
 	 * free memory blocks, using the blocks themselves to store the pointers 
 	 * for the linked list.
 	 */
	PoolMemoryAllocator::PoolMemoryAllocator(U32 block_size, U32 number_of_blocks, U32 block_alignment, OID id) {
		next_block_.ptr = NIL;
		unaligned_memory_block_.ptr = NIL;
		id_ = id;
		// Make sure the block_size is >= the size of a pointer
		if (block_size < sizeof(VPtr)) {
			DERR("block_size must be >= sizeof(VPtr).");
			return;
		}

		block_size_ = block_size;
		number_of_blocks_ = number_of_blocks;

		// First, allocate the total amount of memory we need, plus the alignment size
		// to allow us to align the memory properly
		U32 memory_to_allocate = (block_size * number_of_blocks) + block_alignment;
		unaligned_memory_block_.ptr = malloc(memory_to_allocate);
		if (!unaligned_memory_block_.ptr) {
			DERR("Failed to get memory for PoolMemoryAllocator!");
			return;
		}

		// Calculate the adjusted block address
		aligned_memory_block_ = next_block_ =
			MemoryAllocator::getAlignedMemoryAddress(unaligned_memory_block_, block_alignment);
		
		// Initialize the linked list of memory blocks.
		DMSG("Created new PoolMemoryAllocator at block: " << std::hex << aligned_memory_block_.addr); 
		reset();

	}

	/*
	 * The destructor simply calls the destroy() function, if it gets called.
	 */
	PoolMemoryAllocator::~PoolMemoryAllocator() {
		DOUT("Calling POOLMemoryAllocator DESTRUCTOR\n");
		free();
	}

	/*
	 * The alloc() method returns a new free block of memory for use.  The size of the 
	 * block is fixed.  If there are no free blocks, null is returned
	 */
	VPtr PoolMemoryAllocator::alloc() {
		if (next_block_.ptr == NIL) {
			DWARN("No more free blocks to give!");
			return NIL;
		}

		VPtr memory_block = next_block_.ptr;
		next_block_.addr = (*((Addr*)next_block_.ptr));
		return memory_block;
	}

	VPtr PoolMemoryAllocator::alloc(U32 block_size, U32 alignment) {
		CC_UNUSED(block_size);
		CC_UNUSED(alignment);
		return alloc();
	}

	void PoolMemoryAllocator::dealloc(VPtr memory_block) {
		MemAddr current_block_address = next_block_;
		next_block_.ptr = memory_block;
		
		MemAddr* block_ptr = (MemAddr*)memory_block;
		*block_ptr = current_block_address;
	}

	void PoolMemoryAllocator::dealloc() {
		DERR("Dealloc() not implemented for PoolMemoryAllocator!");
	}

	/**
	 * Use the stored block information to reset the linked list of 
	 * memory blocks to how it should be before any allocations take place.
	 */
	void PoolMemoryAllocator::reset() {
		// Make sure the MemoryAllocator hasn't been free'd yet
		if (unaligned_memory_block_.ptr == NIL)  {
			DWARN("PoolMemoryAllocator has been free'd, can't reset!");
			return;
		}
	
		// Create the linked list out of the memory blocks, putting the location of the next memory block in the 
		// first sizeof(VPtr) bytes of the current memory block
		MemAddr current_block_address = aligned_memory_block_;
		Addr max_address = (aligned_memory_block_.addr) + (block_size_*(number_of_blocks_-1));
		Addr* block_ptr = NIL;
		while (current_block_address.addr < max_address) {
			block_ptr = (Addr*)current_block_address.ptr;
			current_block_address.addr += block_size_;
			*block_ptr = current_block_address.addr;	
		}

		// Set the last block's pointer to NIL
		block_ptr = (Addr*)current_block_address.ptr;
		*block_ptr = NIL;

		next_block_ = aligned_memory_block_;
	}

	void PoolMemoryAllocator::free() {
		// Make sure the MemoryAllocator hasn't been free'd yet
		if (unaligned_memory_block_.ptr == NIL) {
			DWARN("PoolMemoryAllocator has already been free'd!");
			return;
		}

		::free(unaligned_memory_block_.ptr);
		unaligned_memory_block_.ptr = next_block_.ptr = aligned_memory_block_.ptr = NIL;
	}

	OID PoolMemoryAllocator::getOID() {
		return id_;
	}


} // namespace Cat
