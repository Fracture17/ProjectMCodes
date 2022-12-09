//
// Created by johno on 7/14/2020.
//

#include <Graphics/TextPrinter.h>
#include <Wii/PAD/PADStatus.h>
#include <Wii/MATH.h>
#include "Assembly.h"
#include "Memory.h"
#include "Graphics/Drawable.h"
#include "Containers/vector.h"
#include "Containers/ArrayVector.h"

#include "Brawl/IP/IpHuman.h"
#include "Brawl/sndSystem.h"

#include "Graphics/Draw.h"
#include "Brawl/Message.h"

#define _checkTransition_soStatusModule ((void (*)(soStatusModuleImpl* self, soModuleAccessor* accesser)) 0x80780098)

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define snprintf ((int (*)(char* buffer, size_t maxSize, const char* format, ...)) 0x803f8924)
#define strcat ((int (*)(char* destination, const char* source)) 0x803fa384)
#define strcmp ((int (*)(const char* str1, const char* str2)) 0x803fa3fc)
#define atof ((float (*)(const char* buffer)) 0x803fbbf8)

INJECTION("forceVisMemPool", 0x80025dc8, R"(
    cmpwi r3, 69
)");

bool updateFrame = false;
INJECTION("frameUpdate", 0x8001792c, R"(
    bl updatePreFrame
    addi r3, r30, 280
)");

// INJECTION("UPDATE_POST_FRAME", 0x800177b0, R"(
//     SAVE_REGS
//     bl updatePreFrame
//     RESTORE_REGS
//     or r0, r0, r3
// )");

#define RENDER_X_SPACING 80
#define RENDER_SCALE_X 0.5;
#define RENDER_SCALE_Y 0.5;
#define TOP_PADDING 69
#define LEFT_PADDING 20
extern "C" void updatePreFrame() {
    renderables.items.preFrame.push(new Rect (
        GXColor(0x00000088),
        10,
        480 / 2,
        10,
        640 - 10,
        true
    ));
    
    printer.setup();
    printer.drawBoundingBoxes(0);

    startNormalDraw();

    printer.setup();

    Message * message = &printer.message;
    message->fontScaleX = 0.1;
    message->fontScaleY = 0.1;
    printer.lineHeight = 20 * message->fontScaleY;

    char buffer[200] = {};
    char aiInputBuffer[100] = {};

    message->xPos = 1;
    message->yPos = 1;
    message->zPos = 0;

    setupDrawPrimitives();
    renderables.renderPre();
    
    printer.setup();
    printer.renderPre = true;
    printer.start2D();

    for (int i = 0; i < 4; i++) {
        auto xPos = (i + 0.5) * (640 / 4);
        auto yPos = 75;
        
        #define IP_DISPLAY_SCALE 3
        auto& ip = PREVIOUS_PADS[i];
        auto& ipbtn = ip.button;
        

        message->fontScaleY = RENDER_SCALE_Y;
        message->fontScaleX = RENDER_SCALE_X;
        printer.lineHeight = 20 * message->fontScaleY;

        message->xPos = xPos;
        message->yPos = yPos + 20 + 5;
        printer.startBoundingBox();
        sprintf(buffer, "jx: %d", ip.stickX);
        printer.printLine(buffer);
        sprintf(buffer, "jy: %d\n", ip.stickY);
        printer.printLine(buffer);
        sprintf(buffer, "cx: %d", ip.substickX);
        printer.printLine(buffer);
        sprintf(buffer, "cy: %d\n", ip.substickY);
        printer.printLine(buffer);
        sprintf(buffer, "L: %d", ip.triggerLeft);
        printer.printLine(buffer);
        sprintf(buffer, "R: %d", ip.triggerRight);
        printer.printLine(buffer);
        printer.saveBoundingBox(i, 0x000000AA, 2);

        renderables.items.frame.push(new Rect (
            xPos + (2) * IP_DISPLAY_SCALE,
            yPos - (7.5) * IP_DISPLAY_SCALE,
            (30) * IP_DISPLAY_SCALE,
            (20) * IP_DISPLAY_SCALE,
            true,
            GXColor(0x000000AA)
        ));
        renderables.items.frame.push(new Rect (
            xPos + (5) * IP_DISPLAY_SCALE,
            yPos - (10) * IP_DISPLAY_SCALE,
            (5) * IP_DISPLAY_SCALE,
            (5) * IP_DISPLAY_SCALE,
            true,
            GXColor((ipbtn.A) ? 0x00FF00FF : 0x007700FF)
        ));
        renderables.items.frame.push(new Rect (
            xPos + (0) * IP_DISPLAY_SCALE,
            yPos - (8) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            true,
            GXColor((ipbtn.B) ? 0xFF0000FF : 0x770000FF)
        ));
        renderables.items.frame.push(new Rect (
            xPos + (5) * IP_DISPLAY_SCALE,
            yPos - (15) * IP_DISPLAY_SCALE,
            (5) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            true,
            GXColor((ipbtn.Y) ? 0xFFFFFFFF : 0x777777FF)
        ));
        
        renderables.items.frame.push(new Rect (
            xPos + (10) * IP_DISPLAY_SCALE,
            yPos - (10) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            (5) * IP_DISPLAY_SCALE,
            true,
            GXColor((ipbtn.X) ? 0xFFFFFFFF : 0x777777FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos + (10) * IP_DISPLAY_SCALE,
            yPos - (15) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            true,
            GXColor((ipbtn.Z) ? 0x8800FFFF : 0x770077FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos - (5) * IP_DISPLAY_SCALE,
            yPos - (10) * IP_DISPLAY_SCALE,
            (5) * IP_DISPLAY_SCALE,
            (5) * IP_DISPLAY_SCALE,
            true,
            GXColor(0x777777FF)
        ));

        // for effect
        renderables.items.frame.push(new Rect (
            xPos - (5 - 2 * (float)ip.stickX / 255.0) * IP_DISPLAY_SCALE,
            yPos - (10 + 2 * (float)ip.stickY / 255.0) * IP_DISPLAY_SCALE,
            (1.2) * IP_DISPLAY_SCALE,
            (1.2) * IP_DISPLAY_SCALE,
            true,
            GXColor(0x444444FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos - (5 - 2 * (float)ip.stickX / 100.0) * IP_DISPLAY_SCALE,
            yPos - (10 + 2 * (float)ip.stickY / 100.0) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            true,
            GXColor(0xFFFFFFFF)
        ));

        renderables.items.frame.push(new Rect (
            xPos + (5) * IP_DISPLAY_SCALE,
            yPos - (2) * IP_DISPLAY_SCALE,
            (3) * IP_DISPLAY_SCALE,
            (3) * IP_DISPLAY_SCALE,
            true,
            GXColor(0x777700FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos + (5 + 2 * (float)ip.substickX / 150.0) * IP_DISPLAY_SCALE,
            yPos - (2 + 2 * (float)ip.substickY / 150.0) * IP_DISPLAY_SCALE,
            (1.5) * IP_DISPLAY_SCALE,
            (1.5) * IP_DISPLAY_SCALE,
            true,
            GXColor(0xFFDD00FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos - (10) * IP_DISPLAY_SCALE,
            yPos - (12) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            (9) * IP_DISPLAY_SCALE,
            true,
            GXColor(0x333333FF)
        ));
        float LTrigger = (float) ip.triggerLeft / 200;
        renderables.items.frame.push(new Rect (
            xPos - (10) * IP_DISPLAY_SCALE,
            yPos - ((ipbtn.L) ? 12 : (7.5 + 4 * LTrigger)) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            (8 * LTrigger) * IP_DISPLAY_SCALE,
            true,
            GXColor(ipbtn.L ? 0xFFFFFFFF : 0x888888FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos + (13.5) * IP_DISPLAY_SCALE,
            yPos - (12) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            (9) * IP_DISPLAY_SCALE,
            true,
            GXColor(0x333333FF)
        ));

        float RTrigger = (float) ip.triggerRight / 200;
        renderables.items.frame.push(new Rect (
            xPos + (13.5) * IP_DISPLAY_SCALE,
            yPos - ((ipbtn.R) ? 12 : (7.5 + 4 * RTrigger)) * IP_DISPLAY_SCALE,
            (2.5) * IP_DISPLAY_SCALE,
            (8 * RTrigger) * IP_DISPLAY_SCALE,
            true,
            GXColor(ipbtn.R ? 0xFFFFFFFF : 0x888888FF)
        ));
        
        // dpad center
        renderables.items.frame.push(new Rect (
            xPos - (5) * IP_DISPLAY_SCALE,
            yPos - (2) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            true,
            GXColor(0x777777FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos - (7) * IP_DISPLAY_SCALE,
            yPos - (2) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            true,
            GXColor(ipbtn.LeftDPad ? 0xFFFFFFFF : 0x777777FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos - (3) * IP_DISPLAY_SCALE,
            yPos - (2) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            true,
            GXColor(ipbtn.RightDPad ? 0xFFFFFFFF : 0x777777FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos - (5) * IP_DISPLAY_SCALE,
            yPos - (4) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            true,
            GXColor(ipbtn.UpDPad ? 0xFFFFFFFF : 0x777777FF)
        ));

        renderables.items.frame.push(new Rect (
            xPos - (5) * IP_DISPLAY_SCALE,
            yPos - (0) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            (2) * IP_DISPLAY_SCALE,
            true,
            GXColor(ipbtn.DownDPad ? 0xFFFFFFFF : 0x777777FF)
        ));
    }

    renderables.renderAll();
    startNormalDraw();
}