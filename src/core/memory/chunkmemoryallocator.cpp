#include <cstdlib>
#include "core/memory/chunkmemoryallocator.h"

namespace Cat {

	/**
 	 * The ChunkMemoryAllocator constructor creates a new Linked List of 
 	 * free memory blocks, using the blocks themselves to store the pointers 
 	 * for the linked list.
 	 */
	ChunkMemoryAllocator::ChunkMemoryAllocator(U32 chunk_size, U32 number_of_chunks, OID id) {
		next_block_.ptr = NIL;
		unaligned_memory_block_.ptr = NIL;
		id_ = id;
		// Make sure the block_size is >= the size of a pointer
		if (chunk_size < sizeof(VPtr)) {
			DERR("chunk_size (" << chunk_size << ") must be >= sizeof(VPtr).");
			return;
		}
		// Make sure the chunk_size is a power of two using special quick modulus
		// that only works for powers of two.
		if ((chunk_size & (chunk_size - 1)) != 0) {
			DERR("chunk_size ( " << chunk_size << " ) must be a power of 2!");
			return;
		}

		block_size_ = chunk_size;
		number_of_blocks_ = number_of_chunks;

		// First, allocate the total amount of memory we need, plus the alignment size
		// to allow us to align the memory properly
		U32 memory_to_allocate = (block_size_ * number_of_blocks_) + block_size_;
		unaligned_memory_block_.ptr = malloc(memory_to_allocate);
		if (!unaligned_memory_block_.ptr) {
			DERR("Failed to get memory for ChunkMemoryAllocator!");
			return;
		}

		// Calculate the adjusted block address
		aligned_memory_block_ = next_block_ =
			MemoryAllocator::getAlignedMemoryAddress(unaligned_memory_block_, block_size_);
		
		// Initialize the linked list of memory blocks.
		DMSG("Created new ChunkMemoryAllocator at block: " << std::hex << aligned_memory_block_.addr << " with block size " << block_size_); 
		reset();

	}

	/*
	 * The destructor simply calls the destroy() function, if it gets called.
	 */
	ChunkMemoryAllocator::~ChunkMemoryAllocator() {
		DOUT("Calling CHUNKMemoryAllocator DESTRUCTOR\n");
		free();
	}

	VPtr ChunkMemoryAllocator::alloc() {
		DERR("Cannot call ChunkMemoryAllocator::alloc() with no arguments!");
		return NIL;
	}

	VPtr ChunkMemoryAllocator::alloc(U32 block_size, U32 alignment) {
		CC_UNUSED(block_size);
		CC_UNUSED(alignment);
		if (next_block_.ptr == NIL) {
			DWARN("No more free blocks to give!");
			return NIL;
		}
		
		MemAddr memory_block = MemoryAllocator::getAlignedMemoryAddress(next_block_, alignment);
		next_block_.addr = (*((Addr*)next_block_.ptr));

		return memory_block.ptr;

	}

	void ChunkMemoryAllocator::dealloc(VPtr memory_block) {

		MemAddr current_block_address = next_block_;
		next_block_.ptr = memory_block;
		// go back down to the aligned memory address using modulus
		next_block_.addr -= (next_block_.addr & (block_size_ - 1)); // Works ONLY because block_size_ is power of 2.
		
		*((MemAddr*)next_block_.ptr) = current_block_address;
	}

	void ChunkMemoryAllocator::dealloc() {
		DERR("Dealloc() not implemented for ChunkMemoryAllocator!");
	}

	/**
	 * Use the stored block information to reset the linked list of 
	 * memory blocks to how it should be before any allocations take place.
	 */
	void ChunkMemoryAllocator::reset() {
		// Make sure the MemoryAllocator hasn't been free'd yet
		if (unaligned_memory_block_.ptr == NIL)  {
			DWARN("ChunkMemoryAllocator has been free'd, can't reset!");
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

	void ChunkMemoryAllocator::free() {
		// Make sure the MemoryAllocator hasn't been free'd yet
		if (unaligned_memory_block_.ptr == NIL) {
			DWARN("ChunkMemoryAllocator has already been free'd!");
			return;
		}

		::free(unaligned_memory_block_.ptr);
		unaligned_memory_block_.ptr = next_block_.ptr = aligned_memory_block_.ptr = NIL;
	}

	OID ChunkMemoryAllocator::getOID() {
		return id_;
	}


} // namespace Cat
