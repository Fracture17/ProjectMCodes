//
// Created by johno on 7/18/2020.
//

#ifndef PROJECTMCODES_DRAW_H
#define PROJECTMCODES_DRAW_H

#include "Brawl/gfCameraManager.h"
#include "Wii/GX.h"
#include "Brawl/gfDraw.h"

void setup2DDraw();
void setupDrawPrimitives();

void drawTriangleFan(GXColor color, Position3D* vertices, u32 numVertices);
void draw2DQuad(GXColor color, float top, float bottom, float left, float right);

#endif //PROJECTMCODES_DRAW_H
