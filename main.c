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
  if(heap_size + size > HEAP_CAPACITY) {
    for(int i = 0; i <= allocated_chunks_size; i++){
      if(!allocated_or_freed_mapping[i] && chunks[i].size >= size) {
        void* result;
        allocated_or_freed_mapping[i] = 1;
        if(chunks[i].size == size) {
          result = chunks[i].start;
        } else {
          result = chunks[i].start;
          const CHUNK allocated_chunk = {
            .start = chunks[i].start + size,
            .size = chunks[i].size - size,
          };
          chunks[allocated_chunks_size] = allocated_chunk;
          allocated_or_freed_mapping[allocated_chunks_size++] = 0;
          chunks[i].size = size;
        }
      return result;
      }
    }
    printf("heap memory exceeded!");
    exit(1);
  }
  void* result = heap + heap_size;
  heap_size += size;

  if(allocated_chunks_size > ALLOCATED_CHUNKS_CAPACITY) {
    printf("chunk size exceeded");
    exit(1);
  }
  const CHUNK allocated_chunk = {
    .start = result,
    .size = size,
  };
  
  chunks[allocated_chunks_size] = allocated_chunk;
  allocated_or_freed_mapping[allocated_chunks_size++] = 1;

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

