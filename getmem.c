#include <assert.h>
#include "mem.h"
#include "mem_impl.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

// Declare insertSorted function
void insertSorted(freeNode** head, freeNode* newBlock);

void* getmem(uintptr_t size) {
    check_heap();
    if (size <= 0) {
        return NULL;
    }

    uintptr_t totalSize = size + NODESIZE;
    uintptr_t remainder = totalSize % 16;
    if (remainder != 0) {
        totalSize += (16 - remainder);
    }

    freeNode* currentNode = freelist;
    freeNode* prevNode = NULL;

    printf("Searching for block of size: %" PRIuPTR "\n", totalSize);

    while (currentNode != NULL) {
        printf("Inspecting block: Address: %" PRIuPTR ", Size: %" PRIuPTR ", Next: %" PRIuPTR "\n",
               (uintptr_t)currentNode, currentNode->size, (uintptr_t)currentNode->next);

        if (currentNode->size >= totalSize) {
            // Found a block large enough
            if (currentNode->size - totalSize > MINCHUNK) {
                // Split the block if it's significantly larger than requested
                freeNode* newBlock = (freeNode*)((uintptr_t)currentNode + totalSize);
                newBlock->size = currentNode->size - totalSize;
                newBlock->next = NULL; // Explicitly set next to NULL
                currentNode->size = totalSize;
                newBlock->next = currentNode->next;

                // Insert the new block in sorted order
                insertSorted(&freelist, newBlock);
            } else {
                // Remove the block from the free list
                if (prevNode == NULL) {
                    freelist = currentNode->next;
                } else {
                    prevNode->next = currentNode->next;
                }
            }

            return (void*)((uintptr_t)currentNode + NODESIZE);
        }

        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    // No suitable block found in the free list,
    // acquire a new block from the system
    uintptr_t chunkSize = (totalSize > BIGCHUNK) ? totalSize : BIGCHUNK;
    if (chunkSize < MINCHUNK) {
        chunkSize = MINCHUNK;
    }

    freeNode* newBlock = (freeNode*)malloc(chunkSize);

    if (newBlock == NULL) {
        // Unable to allocate memory from the system
        return NULL;
    }

    newBlock->size = chunkSize - NODESIZE;
    newBlock->next = freelist;
    freelist = newBlock;

    return (void*)((uintptr_t)newBlock + NODESIZE);
}

// Definition of insertSorted function
void insertSorted(freeNode** head, freeNode* newBlock) {
    freeNode* current = *head;
    freeNode* prev = NULL;

    // Find the correct position to insert the new block
    while (current != NULL && (uintptr_t)current < (uintptr_t)newBlock) {
        prev = current;
        current = current->next;
    }

    // Insert the new block into the free list
    if (prev != NULL) {
        prev->next = newBlock;
    } else {
        *head = newBlock;
    }

    if (current != NULL) {
        newBlock->next = current;
    } else {
        newBlock->next = NULL;
    }
}
