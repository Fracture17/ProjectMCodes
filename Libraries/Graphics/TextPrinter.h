//
// Created by dareb on 7/20/2020.
//

#ifndef PROJECTMCODES_TEXTPRINTER_H
#define PROJECTMCODES_TEXTPRINTER_H


#include <Brawl/Message.h>

struct TextPrinter {
    void print(const char* characters);
    void printf(const char* characters, ...);
    void printLine(const char* characters);
    void padToWidth(float width);
    void newLine();
    void printInBox(const char* characters, float boxPadding);
    void start2D();
    void setup();

    Message message;
    float lineHeight = 20;
    float lineStart;

    float lastPadLocation;
    bool is2D = false;
};

extern TextPrinter printer;



#endif //PROJECTMCODES_TEXTPRINTER_H
