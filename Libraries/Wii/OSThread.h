#pragma once

#include "stddef.h"
#include "OSTime.h"
#include "OSContext.h"

#define OS_THREAD_SPECIFIC_MAX  2

//0 highest, 31 lowest
typedef s32 OSPriority;

struct OSThread;
struct OSThreadQueue;
struct OSMutex;

//global functions
#define OSGetCurrentThread ((OSThread* (*)()) 0x801e0714)
#define OSDisableScheduler ((int (*)()) 0x801e0760)
#define OSEnableScheduler ((int (*)()) 0x801e079c)

//ThreadQueue functions
#define OSSleepThread ((void (*)(OSThreadQueue* threadQueue)) 0x801e1790)
#define OSInitThreadQueue ((void (*)(OSThreadQueue* threadQueue)) 0x801e0704)
#define OSWakeupThread ((void (*)(OSThreadQueue* threadQueue)) 0x801e187c)

//Thread functions
#define OSCreateThread ((bool (*)(OSThread* thread, void* (*func)(void*), void* param, \
	void* stack, u32 stackSize, OSPriority priority, bool isDetached)) 0x801e0cfc)
#define OSIsThreadSuspended ((bool (*)(OSThread* thread)) 0x801e0720)
#define OSIsThreadTerminated ((bool (*)(OSThread* thread)) 0x801e0734)
#define OSCancelThread ((void (*)(OSThread* thread)) 0x801e104c)
#define OSJoinThread ((bool (*)(OSThread* thread, void** val)) 0x801e1224)
#define OSResumeThread ((s32 (*)(OSThread* thread)) 0x801e1364)
#define OSSuspendThread ((s32 (*)(OSThread* thread)) 0x801e15fc)
#define OSYieldThread ((void (*)()) 0x801e0cc0)
#define OSSleepTicks ((void (*)(OSTime ticks)) 0x801e1a80)

struct OSThreadQueue {
	void init();
	//sleeps the current thread and places it in the queue
	void sleep();
	//wakes all threads in queue
	void wake();

	OSThread* head;
	OSThread* tail;
};

struct OSThreadLink {
	OSThread* next;
	OSThread* prev;
};

struct OSMutexQueue {
	OSMutex* head;
	OSMutex* tail;
};

struct OSMutexLink {
	OSMutex* next;
	OSMutex* prev;
};

//Must be global or dynamically allocated
//It will cause problems if it goes out of scope
struct OSThread {
	OSThread(void* (*func)(void*), void* param, u32 stackSize, OSPriority priority, bool isDetached = false);
	OSThread(void* (*func)(), u32 stackSize, OSPriority priority, bool isDetached = false);

	bool isSuspended();
	bool isTerminated();
	void cancel();
	//The address of the result of the joined thread is written at the
	//address pointed to by result
	bool join(void** result = nullptr);
	//The suspend counter is decremented.
	//If the counter is 0 the thread will be schedualed
	s32 resume();
	//increments suspend counter, making the thread unable to be schedualed
	s32 suspend();
	//sleeps the current thread for a time
	static void sleepTicks(OSTime ticks);
	//yields current running thread
	static void yield();

	OSContext context;

	u16 state;
	u16 attr;       // OS_THREAD_ATTR_*
	s32 suspendCounter;    // suspended if the count is greater than zero
	OSPriority effective;
	OSPriority base;
	void* exitValue;

	OSThreadQueue* queue;      // queue thread is on
	OSThreadLink link;       // queue link

	OSThreadQueue queueJoin;  // list of threads waiting for termination (join)

	OSMutex* mutex;      // mutex trying to lock
	OSMutexQueue queueMutex; // list of mutexes owned

	OSThreadLink linkActive; // link of all threads for debugging

	u8* stackBase;  // the thread's designated stack (high address)
	u32* stackEnd;   // last word of stack (low address)

	s32 error;
	void* specific[OS_THREAD_SPECIFIC_MAX];   // thread specific data
};

enum OS_THREAD_STATE {
	OS_THREAD_STATE_READY = 1,
	OS_THREAD_STATE_RUNNING = 2,
	OS_THREAD_STATE_WAITING = 4,
	OS_THREAD_STATE_MORIBUND = 8
};

//Thread attributes
#define OS_THREAD_ATTR_DETACH 0x0001u //is currently detatched

//Placed at the top of the stack when a thread is created.
//Can be used to identify stack overflows
#define OS_THREAD_STACK_MAGIC 0xDEADBABE