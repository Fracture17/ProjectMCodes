//
// Created by dareb on 7/20/2020.
//

#include "TextPrinter.h"

TextPrinter printer;

void TextPrinter::setup() {
    //save font values
    auto savedFont1 = *_FONT_THING1;
    auto savedFont2 = *_FONT_THING2;

    //clear font values
    *_FONT_THING1 = nullptr;
    *_FONT_THING2 = nullptr;

    //clear 2D flag
    is2D = false;

    //setDefaultEnv uses some values from the message buffer
    //We don't need these values to print, but ignoring it will causes crashes
    void* fakeMessageBuffer[0x40] = {};
    fakeMessageBuffer[0x4C / 4] = &fakeMessageBuffer;
    message._messageBuffer = &fakeMessageBuffer;

    _setDefaultEnv_Message(&message, 0, 9);

    startNormalDraw();

    _GXSetCullMode(GXCullMode::GX_CULL_NONE);

    message.font = MELEE_FONT;

    //restore font values
    *_FONT_THING1 = savedFont1;
    *_FONT_THING2 = savedFont2;
}

void TextPrinter::start2D() {
    is2D = true;

    _GXSetZMode(true, GXCompare::GX_EQUAL, false);
    _GXSetCullMode(GXCullMode::GX_CULL_NONE);
    _GXSetAlphaCompare(GXCompare::GX_ALWAYS, 0, 1, GXCompare::GX_ALWAYS, 0);
    _gfDrawSetupCoord2D();
}

void TextPrinter::printLine(const char *chars) {
    print(chars);
    newLine();
}

void TextPrinter::print(const char *chars) {
    if (is2D) {
        message.yPos *= -1;
        message.fontScaleY *= -1;
    }

    for (; *chars; chars++) {
        if (*chars == '\n') {
            message.xPos = lineStart;
            lastPadLocation = lineStart;
            message.yPos += (is2D) ? -lineHeight : lineHeight;
        }
        else {
            message.printChar(*chars);
        }
    }

    if (is2D) {
        message.yPos *= -1;
        message.fontScaleY *= -1;
    }
}

void TextPrinter::newLine() {
    message.xPos = lineStart;
    lastPadLocation = lineStart;
    message.yPos += lineHeight;
}

#define _printf ((char* (*)(char* format, ...)) 0x803f861c)

void TextPrinter::printInBox(const char *chars, float boxPadding) {

}

void TextPrinter::padToWidth(float width) {
    float newLocation = width + lastPadLocation;

    if (message.xPos < newLocation) {
        message.xPos = newLocation;
        lastPadLocation = newLocation;
    }
    else {
        lastPadLocation = message.xPos;
    }
}

// PrintMessageBox