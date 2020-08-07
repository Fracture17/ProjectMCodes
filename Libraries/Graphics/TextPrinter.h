//
// Created by dareb on 7/20/2020.
//

#ifndef PROJECTMCODES_TEXTPRINTER_H
#define PROJECTMCODES_TEXTPRINTER_H


#include <Brawl/Message.h>
#include <Containers/vector.h>

struct RectBounds {
    GXColor color;
    float top;
    float bottom;
    float left;
    float right;
    float zPos;
    bool is2D;
};

struct vecRef {
    int id;
    vector<RectBounds> rectBounds;
};

struct TextPrinter {
    void print(const char* characters);
    void printLine(const char* characters);
    void padToWidth(float width);
    void newLine(bool fromPrintFn = false);
    void startBoundingBox();
    void saveBoundingBox(int id, GXColor color, float boxPadding = 0);
    void drawBoundingBoxes(int id);
    void setTextColor(GXColor color);
    void start2D();
    void startNormal();
    void setup();

    Message message;
    float lineHeight = 20;
    float maxWidth;
    float lineStart;
    float startY;
    float lastPadLocation;
    vector<vecRef> bboxVecs;
    bool is2D = false;
};

extern TextPrinter printer;



#endif //PROJECTMCODES_TEXTPRINTER_H
