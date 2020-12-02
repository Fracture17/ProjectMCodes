//
// Created by johno on 8/7/2020.
//


#include "Wii/EXI/EXI.h"
#include "Memory.h"
#include "Assembly.h"
#include "Wii/PAD/PADStatus.h"
#include "Brawl/GF/GameFrame.h"








INJECTION("sendInput", 0x8002959c, R"(
    addi r5, sp, 8
    SAVE_REGS
    mr r3, r5
    bl sendInput
    RESTORE_REGS
    li r0, 2
)");

bool on = false;
bool on2 = false;
bool on3 = false;

extern "C" void sendInput(PADStatus* pad) {
    if(on && on2) {
        char buffer[0x30] = {};
        buffer[0] = 0xB0;
        *((int*) (buffer + 1)) = GAME_FRAME->frameCounter;
        buffer[5] = 2;
        memcpy(buffer + 6, pad, 0xC);

        writeEXI(buffer, 1 + 4 + 1 + 0xC, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);
    }
}


BASIC_INJECT("setMatchSelections", 0x806c92d8, "lwz r0, 0x74(sp)");

extern "C" void setMatchSelections() {
    char buffer[0x30] = {};

    buffer[0] = 0xB5;
    buffer[1] = 9;
    buffer[2] = 0;
    buffer[3] = 1;
    buffer[6] = 3;

    writeEXI(buffer, 7, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);

    on3 = true;
}


BASIC_INJECT("startConnection", 0x806b5c54, "li r0, 1");

extern "C" void startConnection() {
    char buffer[0x30] = {};

    buffer[0] = 0xB4;
    buffer[1] = 1;

    writeEXI(buffer, 20, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);

    on2 = true;
}


BASIC_INJECT("getMatchState", 0x80017470, "lwz r0, 0xF8(r22)");

extern "C" void getMatchState() {
    if(on3) {
        char buffer[0x300] = {};

        buffer[0] = 0xB3;

        writeEXI(buffer, 1, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);

        readEXI(buffer, 0x290, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);
    }
}