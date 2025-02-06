#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<stdbool.h>
#include<stdlib.h>

#define HEAP_CAPACITY 1000000
#define ALLOCATED_CHUNKS_CAPACITY 1000000
#define FREED_CHUNKS_CAPACITY 1000000

typedef struct {
  void* start;
  size_t size;
}CHUNK;

void* heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

CHUNK chunks[ALLOCATED_CHUNKS_CAPACITY] = {0};
size_t allocated_chunks_size = 0;
int allocated_or_freed_mapping[ALLOCATED_CHUNKS_CAPACITY] = {0};   //allocated=1, freed=0


void* my_alloc(size_t size){
  // Check if the size of the heap exceeds the maximum size
  if(heap_size + size > HEAP_CAPACITY) {
    // If there is no space at the end, go through allocated bitmap and look for a freed block with required size
    for(int i = 0; i <= allocated_chunks_size; i++){
      // Check if freed and if chunk size is bigger than required
      if(!allocated_or_freed_mapping[i] && chunks[i].size >= size) {
        void* result;
        // If requirement matched, allocate buffer mark as allocated
        allocated_or_freed_mapping[i] = 1;
        if(chunks[i].size == size) {
          // If no resizing requried, return the pointer to the start
          result = chunks[i].start;
        } else {
          // If required size is lesser, resize the chunk and return only the necessary amount of memory
          result = chunks[i].start;
          const CHUNK allocated_chunk = {
            .start = chunks[i].start + size,
            .size = chunks[i].size - size,
          };
          // New chunk of (total size - required size) added
          chunks[allocated_chunks_size] = allocated_chunk;
          allocated_or_freed_mapping[allocated_chunks_size++] = 0;
          chunks[i].size = size;
        }
        // Return pointer
        return result;
      }
    }
    // If not enough space and no block available, return memory exceeded
    printf("heap memory exceeded!");
    exit(1);
  }

  // If size available at the end, just return end pointer and add a new allocated chunk to allocated buffer
  // Add the new chunk size to total size
  void* result = heap + heap_size;
  heap_size += size;

  // If chunk number limit exceeded return error
  if(allocated_chunks_size > ALLOCATED_CHUNKS_CAPACITY) {
    printf("chunk size exceeded");
    exit(1);
  }

  // New chunk
  const CHUNK allocated_chunk = {
    .start = result,
    .size = size,
  };
  
  // Add the new chunk to the allocated buffer
  chunks[allocated_chunks_size] = allocated_chunk;
  allocated_or_freed_mapping[allocated_chunks_size++] = 1;

  // Return the pointer
  return result;
}

void* my_free(size_t size){
  assert(false && "alloc not yet implemented");
  return NULL;
}

int main() {
  for(int i=0; i <= 25; i++) {
    char* alpha = my_alloc(1);
    *alpha = (char)('A' + i);

    printf("pos: %p, char: %c\n", alpha, *alpha);
  }

  for(int i = 0; i < allocated_chunks_size; i++){
    if(allocated_or_freed_mapping[i]) {
      printf("%p\n", chunks[i].start);
      printf("%zu\n\n", chunks[i].size);
    }
  }

  printf("\n\n\n\n");
  
  for(int i = 0; i < allocated_chunks_size; i++){
    if(!allocated_or_freed_mapping[i]) {
      printf("%p\n", chunks[i].start);
      printf("%zu\n\n", chunks[i].size);
    }
  }

  return 0;
}

