//
// Created by dareb on 7/20/2020.
//

#include "TextPrinter.h"
#include "Drawable.h"

TextPrinter printer;

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

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
//    message.setObjZCompare(GXCompare::GX_EQUAL, true);

    // message.font = MELEE_FONT;
    // other fonts include: 
    // - font_latin12.arc
    // - font_latin1.arc
    // - font_watch.arc

    // // BLANK (0) (enc: 3; fmt: )
    // message.font = (FontData*) 0x80497d2c;
    // OSReport("0 enc: %08x\n", message.font->GetGlyph());
    // // MELEE STANDARD (1) / DEFAULT (enc: 0; fmt: )
    // message.font = (FontData*) 0x80497d54;
    // OSReport("1 enc: %08x\n", message.font->GetGlyph());
    // // Hiranga Font (2) (starts @ 0x32) (USA Tags) (enc: 0; fmt: )
    // message.font = (FontData*) 0x80497da4;
    // OSReport("2 enc: %08x\n", message.font->GetGlyph());
    // // Ending Font (3) (enc: 0; fmt: )
    // message.font = (FontData*) 0x80497dcc;
    // OSReport("3 enc: %08x\n", message.font->GetGlyph());
    // // Melee Font (4) (MELEE MONO) (enc: 0; fmt: )
    // message.font = (FontData*) 0x80497e44;
    // OSReport("4 enc: %08x\n", message.font->GetGlyph());
    // // Resant Font (5) (enc: 0; fmt: )
    // message.font = (FontData*) 0x80497df4;
    // OSReport("5 enc: %08x\n", message.font->GetGlyph());
    // // Fox Font (6) (enc: 0; fmt: )
    // message.font = (FontData*) 0x80497e1c;
    // OSReport("6 enc: %08x\n", message.font->GetGlyph());
    // // Alert Font(?) (7) (enc: 0; fmt: )
    // message.font = (FontData*) 0x80497e6c;
    // OSReport("7 enc: %08x\n", message.font->GetGlyph());
    // // USA MAIN MENU (8) (enc: 0; fmt: )
    // message.font = (FontData*) 0x80497d7c;
    // OSReport("8 enc: %08x\n", message.font->GetGlyph());

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

// deprecated
void TextPrinter::drawBoundingBoxes(int id) {
//    for (int i = 0; i < bboxVecs.size(); i++) {
//        if (bboxVecs[i].id == id) {
//            vector<RectBounds> * boxes = &bboxVecs[i].rectBounds;
//            size_t size = boxes->size();
//            setupDrawPrimitives();
//
//
//            for (int j = 0; j < size; j++) {
//                if ((*boxes)[j].is2D) {
//                    start2DDraw();
//                } else {
//                    startNormalDraw();
//                }
//                draw2DRectangle(
//                    (*boxes)[j].color,
//                    (*boxes)[j].top,
//                    (*boxes)[j].bottom,
//                    (*boxes)[j].left,
//                    (*boxes)[j].right,
//                    (*boxes)[j].zPos);
//            }
//            boxes->clear();
//            setup();
//            break;
//        }
//    }
}

void TextPrinter::setTextColor(GXColor color) {
    this->message.textColor.color1 = color;
    this->message.textColor.color2 = color;
    this->message.textColor.color3 = color;
    this->message.textColor.color4 = color;
//    appears to be unused?
//    this->message.textColor.color5 = color;
}

void TextPrinter::print(const char *chars) {
//    _GXSetZMode(true, GXCompare::GX_EQUAL, true);
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
            // message.printChar(0xFF00 | *chars);
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
    if (renderPre) bboxIdx = renderables.items.preFrame.size();
    else bboxIdx = renderables.items.frame.size();
}

void TextPrinter::saveBoundingBox(int id, GXColor color, float boxPadding) {
    if (lineStart + maxWidth < message.xPos) {
        maxWidth = message.xPos - lineStart;
    }

    int multiplier = (is2D) ? 1 : -1;
    Rect * r = new Rect{
            0,
            1,
            color,
            (startY - boxPadding) * multiplier,
            (message.yPos + lineHeight + boxPadding) * multiplier,
            lineStart - boxPadding,
            lineStart + maxWidth + boxPadding,
            is2D
    };
    // OSReport("Rect in (t, b, l, r): %.3f, %.3f, %.3f, %.3f\n", r.top, r.bottom, r.left, r.right);
    if (renderPre) renderables.items.preFrame.insert(r, bboxIdx);
    else renderables.items.frame.insert(r, bboxIdx);
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