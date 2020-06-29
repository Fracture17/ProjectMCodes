#pragma once

#include "Assembly.h"
#include "stddef.h"
#include "Wii/expHeap.h"

#define MAIN_HEAP_ADDRESS ((void*) 0x935D0000)
#define MAIN_HEAP_SIZE (0x10000)
extern MEMHeapHandle mainHeap;

//creates the memory buffer used by malloc
//Called first thing after codes are loaded
extern "C" void initMemory();


void* operator new(size_t n);
void operator delete(void* p);
//this is the version that gets called.
//Gives size of buffer for debuggin purposes
//Can just call normal version
void operator delete(void* p, size_t size);
void* operator new[](size_t s);
void operator delete[](void* p);
//this is the version that gets called.
//Gives size of buffer for debuggin purposes
//Can just call normal version
//TODO: Test this.  Destructors may not be called correctly
void operator delete[](void* p, size_t size);

//These replace the calls the compiler generates automatically
extern "C" void* memmove(void* dest, const void* source, size_t size);
extern "C" void* memcpy(void* dest, const void* source, size_t size);
extern "C" void* memset(void* data, int value, size_t size);


#define _memmove ((void* const (*)(void* dest, const void* source, size_t size)) 0x803f602c)
#define _memset ((void* (*)(void* data, int value, size_t size)) 0x8000443c)

//returns heap manager address from a given heap index
//extern void* const (*getMemAllocator)(int heap);

//uses the heap manager to alloacte new memory. adds 0x20 for managing info
//extern void* const (*allocate)(void* allocator, int size);

//compiler can generate calls to memmove, memcpy, memset, and memcmp
//these functions are wrappers to call the Brawl versions


extern "C" void* malloc(size_t size, int alignment = 4);
extern "C" void free(void* ptr);


