//
// Created by johno on 7/13/2020.
//

#ifndef PROJECTMCODES_MESSAGE_H
#define PROJECTMCODES_MESSAGE_H


#include "stddef.h"
#include "Brawl/gfCameraManager.h"
#include "Memory.h"
#include "Assembly.h"


//Alpha is ignored if not used
struct GXColor {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
};


//each of these colors a portion of the text, not sure which yet
//not sure if the fifth is nescassary
struct CharWriterTextColor {
    GXColor color1;
    GXColor color2;
    GXColor color3;
    GXColor color4;
    GXColor color5;
};

//FontData may not be the real name, and may not be used completly correctly
struct FontData {

};


typedef u16 UTF16;

typedef f32	Mtx[3][4];


//This is called a ton of things
//Anything with the ms prefix that has to do with printing
//This might be a derived class, but I'm going to treat them all as the same for now
//Total size is 0x200 bytes
struct Message {
    //run each frame before printing
    void setup();

    static void std2DView();


    //null terminated
    void printString(const char* characters);
    //null terminated
    void printStringUTF(const UTF16* characters);
    void printChar(const UTF16 character);




    //0
    //seems to be a boolean flag for GX color mapping
    //takes up 4 bytes, should be set to 0 unless we find out what it means
    int _colorMap = 0;


    //0x4
    //seems to always be 0xFFFFFFFF
    //possibly used with GX color mapping
    u32 _mightAlsoBeColor = 0xFFFFFFFF;

    //0x8
    CharWriterTextColor textColor;

    //0x1C
    //seems to always be 0xFFFFFFFF
    u32 _mightAlsoBeColor2 = 0xFFFFFFFF;

    char _spacer[0x24 - 0x1C - 4];

    //0x24
    float fontScaleX = 1;

    //0x28
    float fontScaleY = 1;

    //0x2c
    float xPos;

    //0x30
    float yPos;

    //0x34
    float zPos;

    //0x38
    //don't know what this is, but notes say to set it to 1 and leave it
    int _thingEqualToOne1 = 1;

    //0x3C
    //don't know what this is, but notes say to set it to 1 and leave it
    int _thingEqualToOne2 = 1;

    char _spacer2[0x44 - 0x3C - 4];

    //0x44
    float fixedWidth;

    //0x48
    FontData* font;

    //0x4C
    //might be to get it to actually print or something
    //always seems to be 1, and notes have it set to 1
    bool _flagThing = true;

    char _spacer3[0x50 - 0x4C - 1];

    //0x50
    //use this in general to scale font
    //also changes scale of positions
    float fontSize = 0.0625;

    char _spacer4[0x58 - 0x50 - 4];

    //0x58
    //always seems to be some value close or equal to 1
    //seems to affect text width
    float _fontWidthThing = 1;

    //0x5C
    //Set by SetEdge[ms10Char]
    float _edge = 0;

    //0x60
    //Set by SetEdge, each byte set individually
    char _edgeBytes[4];


    char _spacer5[0x68 - 0x60 - 4];


    //0x68
    //something to do with font
    //set to 1 in SetFont, but could be set to other values based on rtoc stuff
    float _fontThing = 1;

    char _spacer6[0x74 - 0x68 - 4];


    //0x74
    //values are in pixels
    //default is 0, 0, 640, 480
    //not sure if used to position message on screen or to clip message or what
    //seems to be relative to parent or something
    float screenStartX = 0;
    float screenStartY = 0;
    float screenEndX = 640;
    float screenEndY = 480;


    char _spacer7[0x8C - 0x74 - 4 * 4];


    //0x8C
    //not sure if this is the space between lines for newline or something else
    float _lineSpace = 0;

    char _spacer8[0x94 - 0x8C - 4];


    //0x94
    //might be a bool, but is 4 bytes
    //Seems to affect GX color mapping
    //Set to 0 by default using setDrawFlag[Message]
    int _drawFlag = 0;

    char _spacer9[0x1B8 - 0x94 - 4];


    //0x1B8
    //set to 0.5 in setDefaultEnv
    float _unknown1;

    //0x1BC
    //set to 0.95 in setDefaultEnv
    float _unknown2;

    char _spacer11[0x1C4 - 0x1BC - 4];

    //0x1C4
    //seems to be something set from a value from the messageBuffer
    bool _messageByte;

    char _spacer12[0x1D0 - 0x1C4 - 1];

    //0x1D0
    //seems to point to message buffer object
    //Has more stuff in it than just message, but not needed if manually printing characters, so leaving it void*
    void* _messageBuffer;

    char _spacer10[0x200 - 0x1D0 - 4];
}__attribute__((packed, aligned(4)));;


extern Message message;


//not 100% sure what the flags do, but color flag should be 0, and font flag should be 9
#define _setDefaultEnv_Message ((void (*)(Message* message, int _textColorFlagThing, int _fontFlagThing)) 0x8006a964)
#define _Print_Message ((void (*)(Message* message, const UTF16 character)) 0x8006fe50)
#define _std2DView ((void (*)()) 0x8006b360)


//These seem to be pointers to some currently loaded fonts
//These must be cleared and restored durign setup, or can crash
//crashes occur in between certain loads, like when a game ends
#define _FONT_THING1 ((void**) 0x805a0174)
#define _FONT_THING2 ((void**) 0x805a0178)

//pretty sure this is melee font, which is always loaded
#define MELEE_FONT ((FontData*) 0x80497e44)


#define _GXLoadPosMtxImm ((void (*)(const Mtx* matrix, u32 id)) 0x801f51dc)

#endif //PROJECTMCODES_MESSAGE_H
