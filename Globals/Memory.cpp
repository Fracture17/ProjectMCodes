#include "Memory.h"

MEMHeapHandle mainHeap;

//STARTUP("initMemory");

extern "C" void initMemory() {
	mainHeap = createExpHeap(MAIN_HEAP_ADDRESS, MAIN_HEAP_SIZE);
}


extern "C" void* memmove(void* dest, const void* source, size_t size) {
	return _memmove(dest, source, size);
}

extern "C" void* memcpy(void* dest, const void* source, size_t size) {
	return _memmove(dest, source, size);
}

extern "C" void* memset(void* data, int value, size_t size) {
	return _memset(data, value, size);
}

extern "C" int memcmp(const void* a, const void* b, size_t size) {
    return _memcmp(a, b, size);
}

extern "C" void* malloc(size_t size, int alignment) {
	return allocFromExpHeap(mainHeap, size, alignment);
}

extern "C" void free(void* ptr) {
	freeToExpHeap(mainHeap, ptr);
}

void* operator new(size_t n) {
	return malloc(n);
}

void operator delete(void* p) {
	free(p);
}

void operator delete(void* p, size_t size) {
    free(p);
}

void* operator new[](size_t s) {
	return malloc(s);
}

void operator delete[](void* p) {
	free(p);
}

void operator delete[](void* p, size_t size) {
	delete[]((char*)p);
}