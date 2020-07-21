//
// Created by dareb on 7/20/2020.
//

#include "TextPrinter.h"

TextPrinter printer;

//Must be called before doing anything after graphics settings were
//modified. Otherwise the game will likely hang and crash.
void TextPrinter::setup() {
    //save font values
    auto savedFont1 = *_FONT_THING1;
    auto savedFont2 = *_FONT_THING2;
    auto savedFontScaleX = message.fontScaleX;
    auto savedFontScaleY = message.fontScaleY;

    //clear font values
    *_FONT_THING1 = nullptr;
    *_FONT_THING2 = nullptr;

    //clear 2D flag
    is2D = false;
    startBoundingBox();

    //setDefaultEnv uses some values from the message buffer
    //We don't need these values to print, but ignoring it will causes crashes
    void* fakeMessageBuffer[0x40] = {};
    fakeMessageBuffer[0x4C / 4] = &fakeMessageBuffer;
    message._messageBuffer = &fakeMessageBuffer;

    _setDefaultEnv_Message(&message, 0, 9);

    _GXSetCullMode(GXCullMode::GX_CULL_NONE);

    message.font = MELEE_FONT;

    //restore font values
    *_FONT_THING1 = savedFont1;
    *_FONT_THING2 = savedFont2;
    message.fontScaleX = savedFontScaleX;
    message.fontScaleY = savedFontScaleY;
}

void TextPrinter::start2D() {
    is2D = true;

    start2DDraw();
}

void TextPrinter::startNormal() {
    is2D = false;

    startNormalDraw();
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
            newLine(true);
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

void TextPrinter::newLine(bool fromPrintFn) {
    if (lineStart + maxWidth < message.xPos) {
        maxWidth = message.xPos - lineStart;
    }
    message.xPos = lineStart;
    lastPadLocation = lineStart;
    message.yPos += (fromPrintFn && is2D) ? -lineHeight : lineHeight;
}

void TextPrinter::startBoundingBox() {
    startY = message.yPos;
    lineStart = message.xPos;
    lastPadLocation = message.xPos;
    maxWidth = 0;
}

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

void TextPrinter::drawBoundingBox(GXColor color, float boxPadding) {
    if (lineStart + maxWidth < message.xPos) {
        maxWidth = message.xPos - lineStart;
    }

    int multiplier = (is2D) ? 1 : -1;
    // I THINK the zAxis thing has something to do with this line
    _GXSetZMode(true, GXCompare::GX_EQUAL, true);
    draw2DRectangle(
            color,
            (startY - boxPadding) * multiplier,
            (message.yPos + lineHeight + boxPadding) * multiplier,
            lineStart - boxPadding,
            lineStart + maxWidth + boxPadding,
            message.zPos + 1);
    _GXSetZMode(true, GXCompare::GX_EQUAL, false);
    setup();
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