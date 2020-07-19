//
// Created by dareb on 7/19/2020.
//

#include <Graphics/Message.h>
#include "Assembly.h"

BASIC_INJECT("messageMaker", 0x8001792c, "addi r3, r30, 280");

// BIG thanks to ;-; | Eon / fracture for helping figure this out

#define __nw_srHeapType ((void * (*)(int size, HEAPTYPE type)) 0x8000c8b8)

#define _constructor_ms_message ((Message * (*)(void * location, int unknown, HEAPTYPE type)) 0x800696b4)
#define _destructor_ms_message ((void (*)(void * location, HEAPTYPE type)) 0x800696b4)

#define _clearMsgBuff_ms_message ((void (*)(Message msg)) 0x8006b674)
#define _setMessageLocation_ms_message ((void (*)(Message msg, double x1, double x2, double y1, double y2)) 0x80069AF0)

// face values:
// 2: victory screen font
// 4: generic/pictochat font
#define _setFace_ms_message ((void (*)(Message msg, int face)) 0x8006A550)

#define _setFixedWidth_ms_message ((void (*)(Message msg, double width)) 0x8006A600)
#define _setColor_ms_message ((void (*)(Message msg, int color)) 0x8006A600)
#define _setScale_ms_message ((void (*)(Message msg, double xScale, double yScale)) 0x8006A018)
#define _setCursorX_ms_message ((void (*)(Message msg, double cursorX)) 0x80069970)
#define _setCursorY_ms_message ((void (*)(Message msg, double cursorY)) 0x80069A30)
#define _printf_ms_message ((void (*)(Message msg, char* text, ...)) 0x80069D40)

#define static_std2DView_ms_message ((void (*)()) 0x8006B360)
#define _printMsgBuf_ms_message ((void (*)(Message msg, int unknown)) 0x8006AB48)
#define _allocMsgBuffer_ms_message ((void (*)(Message * msg, int bufferSize, int unk1, HEAPTYPE heaptype)) 0x8006b400)

void Message::allocMsgBuffer(int bufferSize, int unk1, HEAPTYPE heaptype) {
    _allocMsgBuffer_ms_message(this, bufferSize, unk1, heaptype);
};

void* freeLoc = __nw_srHeapType(0x200, HEAPTYPE::StageInstance);
Message* msg = _constructor_ms_message(freeLoc, 10, HEAPTYPE::StageInstance);

char MESSAGE[] = "hello world";

extern "C" void messageMaker() {
    if (msg->_messageBuffer = 0) {
        msg->allocMsgBuffer(512, 1, HEAPTYPE::StageInstance);
    }
    _setMessageLocation_ms_message(*msg, 0, 0, 200, 200);
    _setFace_ms_message(*msg, 2);
    _setColor_ms_message(*msg, 0xffffffff);
    _setCursorX_ms_message(*msg, 0);
    _setCursorY_ms_message(*msg, 0);
    _setScale_ms_message(*msg, 1, 1);
    _printf_ms_message(*msg, MESSAGE);
    static_std2DView_ms_message();

    _printMsgBuf_ms_message(*msg, 9);

    _clearMsgBuff_ms_message(*msg);
}