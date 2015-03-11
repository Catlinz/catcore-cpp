#include <cstdlib>
#include "core/memory/stackmemoryallocator.h"

namespace Cat {

	/**
	 * Creates a new StackMemoryAllocator with a set size.  Sets the initial 
	 * memory block to start at, and the initial memory block to store the markers at.  
	 * Essentially works as a doubly ended stack, with the markers going down from the top.
	 * @param stack_size The size of the stack in bytes
	 */
	StackMemoryAllocator::StackMemoryAllocator(U32 stack_size, OID id) {
		id_ = id;
		unaligned_memory_block_.ptr = next_block_.ptr = malloc(stack_size);
		if (!unaligned_memory_block_.ptr) {
			DERR("Failed to get memory for StackMemoryAllocator!");
			return;
		}
		stack_size_ = stack_size;
	
		// Initialize the memory block for the first marker
		DMSG("Created new StackMemoryAllocator at block: " << std::hex << unaligned_memory_block_.addr); 
		reset();
		
	}

	StackMemoryAllocator::~StackMemoryAllocator() {
		DOUT("Calling STACKMemoryAllocator DESTRUCTOR\n");
		free();
	}

	/**
	 * Allocates a block of memory from the stack of a specified size.  Makes sure to 
	 * align the allocation to the specified memory alignment.
	 * @param block_size The size, in bytes, of memory to allocate
	 * @param alignment The memory alignment the block should have
	 * @return a VPtr to the block of memory, or NIL if error
	 */
	VPtr StackMemoryAllocator::alloc(U32 block_size, U32 alignment) {
		// Calculate the aligned memory block from the next_block_
		MemAddr aligned_memory_addr = MemoryAllocator::getAlignedMemoryAddress(next_block_, alignment);

		// Make sure we don't collide with the markers
		if (aligned_memory_addr.addr + block_size > marker_.addr) {
			DERR("Cannot allocate block of size " << block_size << ", collides with top of stack!");
			return NIL;
		}

		next_block_.addr = (aligned_memory_addr.addr + block_size);
		return aligned_memory_addr.ptr;


	}

	/**
	 * alloc() with no arguments prints an error and returns NIL 
	 */
	VPtr StackMemoryAllocator::alloc() {
		DERR("Cannot call alloc() with no arguments for StackMemoryAllocator");
		return NIL;
	}

	/**
	 * Deallocates at least one block of memory.  It deallocates back to where the 
	 * last marker was placed.  Make sure to call the destructors of the objects 
	 * allocated after the last marker was placed before calling dealloc()
	 */
	void StackMemoryAllocator::dealloc() {
		if (next_block_.addr == unaligned_memory_block_.addr) // Already deallocated to beginning
			return;

		MemAddr mark_addr;
		mark_addr = *((MemAddr*)marker_.ptr);
		if (mark_addr.ptr == 0) { 
			// No markers, deallocate to beginning
			next_block_ = unaligned_memory_block_;
		} else {
			next_block_.ptr = mark_addr.ptr;
			marker_.addr = (marker_.addr + sizeof(VPtr));
		}
	}
	
	/**
	 * dealloc(memory_block) with a specific memory_block prints an error.
	 */
	void StackMemoryAllocator::dealloc(VPtr memory_block) {
		CC_UNUSED(memory_block);
		DERR("Cannot dealloc() a specific memory block, must use markers!");
	}

	/**
	 * Resets the StackMemoryAllocator to the initial state, removes all markers.
	 */
	void StackMemoryAllocator::reset() {
		// Find the address for the first (NIL) marker
		MemAddr marker_addr;
		marker_addr.addr = unaligned_memory_block_.addr + stack_size_ - sizeof(VPtr);
		marker_ = MemoryAllocator::getAlignedMemoryAddress(marker_addr, sizeof(VPtr));
		*((Addr*)(marker_.ptr)) = NIL;

		// Reset the next_block_ to the original address
		next_block_ = unaligned_memory_block_;
	}

	/**
	 * Frees the memory block associated with the StackMemoryAllocator.  
	 * The StackMemoryAllocator is unusable after this call.
	 */
	void StackMemoryAllocator::free() {
		// Make sure the MemoryAllocator hasn't been free'd yet
		if (unaligned_memory_block_.ptr == NIL) {
			DWARN("StackMemoryAllocator has already been free'd!");
			return;
		}

		::free(unaligned_memory_block_.ptr);
		unaligned_memory_block_.ptr = next_block_.ptr = marker_.ptr = NIL;

	}

	/**
	 * Marks a position in the StackMemoryAllocator to where the next dealloc() 
	 * call will deallocate to.
	 */
	void StackMemoryAllocator::mark() {
		MemAddr marker_addr = marker_;
		marker_addr.addr -= sizeof(VPtr);
		
		MemAddr* marker = (MemAddr*)marker_addr.ptr;
		*marker = next_block_;

		marker_ = marker_addr;
	}

	OID StackMemoryAllocator::getOID() {
		return id_;
	}



} // namespace Cat
