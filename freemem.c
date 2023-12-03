/* freemem.c
   implements freemem (free) for memory system
   CSE 374 HW6
   Copyright 2023 Devina Tavathia & Mia Onodera
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"
#include <stdio.h>

static void combine(freeNode* firstBlock, freeNode* secondBlock);

/* Your other functions... */

void freemem(void* p) {
  print_heap(stdout);
  check_heap();

  if (p == NULL) {
    return;
  }


  freeNode* block = ((freeNode*)(uintptr_t)p - NODESIZE);
  printf("free block\n");
  fprintf(stdout, "%" PRIuPTR "\n", (uintptr_t)block);
  fprintf(stdout, "%" PRIuPTR "\n", block->size);
  freeNode* currentNode = freelist;
  freeNode* prevNode = NULL;

  while (currentNode != NULL && currentNode < block) {
    prevNode = currentNode;
    currentNode = currentNode->next;
  }

  if (prevNode == NULL) {
    block->next = freelist;
    freelist = block;
  } else {
    prevNode->next = block;
    block->next = currentNode;
  }

  combine(prevNode, block);
  combine(block, block->next);
  print_heap(stdout);
  check_heap();
}

static void combine(freeNode* firstBlock, freeNode* secondBlock) {
  if (firstBlock != NULL && secondBlock != NULL) {
    uintptr_t endFirst = (uintptr_t)firstBlock + NODESIZE + firstBlock->size;
    uintptr_t startSecond = (uintptr_t)secondBlock;

    if (endFirst == startSecond) {
      firstBlock->size += NODESIZE + secondBlock->size;
      firstBlock->next = secondBlock->next;
      free(secondBlock);
    }
  }
}
