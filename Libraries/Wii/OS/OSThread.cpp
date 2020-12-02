#include "OSThread.h"

//include OSMutex here so mutex type is defined, but avoids circular dependancies
#include "OSMutex.h"
#include "Memory.h"


//Insert debug info into threads

//0 will be reserved for doesn't exist or something
unsigned int nextThreadID = 1;

char mainThreadName[] = "Main (GX)";

//r31 is thread*
//can use r3 and r11
INJECTION("setThreadID", 0x801e0f4c, R"(
    lis r11, nextThreadID@ha
    lwzu r3, nextThreadID@l(r11)

    #store thread ID in thread specific slot 1
    #cheating a bit, since it's supposed to be a pointer to the number but whatever
    stw r3, 0x310(r31)

    #increment ID
    addi r3, r3, 1
    stw r3, 0(r11)

    #replace instruction
    li r3, 1
)");

//set first thread id, since it's not made with OSCreateThread
STARTUP("setMainThreadInfo");

extern "C" void setMainThreadInfo() {
    auto mainThread = *CURRENT_THREAD;
    mainThread->specific[0] = (void*) nextThreadID;
    nextThreadID++;
    mainThread->specific[1] = mainThreadName;
}


//set known thread names



OSThread* getCurrentThread() {
	return OSGetCurrentThread();
}

int disableScheduler() {
	return OSDisableScheduler();
}

int enableScheduler() {
	return OSEnableScheduler();
}

void OSThreadQueue::init() {
	OSInitThreadQueue(this);
}

void OSThreadQueue::sleep() {
	OSSleepThread(this);
}

void OSThreadQueue::wake() {
	OSWakeupThread(this);
}

OSThread::OSThread(void* (*func)(void*), void* param, u32 stackSize, OSPriority priority, bool isDetached) {
	void* stack = malloc(stackSize);
	stack = (char*)stack + stackSize;
	OSCreateThread(this, func, param, stack, stackSize, priority, isDetached);
}

OSThread::OSThread(void* (*func)(), u32 stackSize, OSPriority priority, bool isDetached) {
	void* stack = malloc(stackSize);
	stack = (char*)stack + stackSize;
	OSCreateThread(this, (void* (*)(void*))func, nullptr, stack, stackSize, priority, isDetached);
}

bool OSThread::isSuspended() {
	return OSIsThreadSuspended(this);
}

bool OSThread::isTerminated() {
	return OSIsThreadTerminated(this);
}

void OSThread::cancel() {
	OSCancelThread(this);
}

bool OSThread::join(void** result) {
	return OSJoinThread(this, result);
}

s32 OSThread::resume() {
	return OSResumeThread(this);
}

s32 OSThread::suspend() {
	return OSSuspendThread(this);
}

void OSThread::sleepTicks(OSTime ticks) {
	OSSleepTicks(ticks);
}

void OSThread::yield() {
	OSYieldThread();
}