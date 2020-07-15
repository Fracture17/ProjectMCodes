//
// Created by johno on 7/14/2020.
//

#include "Graphics/Message.h"
#include "Assembly.h"


BASIC_INJECT("testPrint", 0x8001792c, "addi r3, r30, 280");


char M[] = "Wow! So cool!";

extern "C" void testPrint() {
    message.setup();
    message.xPos = 100;
    message.yPos = 100;
    message.printChar('x');
    message.printChar('z');
    message.printString(M);
}