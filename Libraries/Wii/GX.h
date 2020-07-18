//
// Created by johno on 7/18/2020.
//

#ifndef PROJECTMCODES_GX_H
#define PROJECTMCODES_GX_H

#include "stddef.h"

//Alpha is ignored if not used
union GXColor {
    GXColor() = default;
    GXColor(u32 value): value(value) {};
    u32 value = 0;
    struct {
        u8 red;
        u8 green;
        u8 blue;
        u8 alpha;
    }__attribute__((packed, aligned(4)));
}__attribute__((packed, aligned(4)));

union WGPipe {
    vu8 U8;
    vs8 S8;
    vu16 U16;
    vs16 S16;
    vu32 U32;
    vs32 S32;
    vf32 F32;
};

enum GXPrimitive {
    //Draws a series of points. Each vertex is a single point.
    GX_POINTS = 0xB8,
    //Draws a series of unconnected line segments. Each pair of vertices makes a line.
    GX_LINES = 0xA8,
    //Draws a series of lines. Each vertex (besides the first) makes a line between it and the previous.
    GX_LINESTRIP = 0xB0,
    //Draws a series of unconnected triangles. Three vertices make a single triangle.
    GX_TRIANGLES = 0x90,
    //Draws a series of triangles. Each triangle (besides the first) shares a side with the previous triangle.
    //Each vertex (besides the first two) completes a triangle.
    GX_TRIANGLESTRIP = 0x98,
    //Draws a single triangle fan. The first vertex is the "centerpoint". The second and third vertex complete
    //the first triangle. Each subsequent vertex completes another triangle which shares a side with the previous
    //triangle (except the first triangle) and has the centerpoint vertex as one of the vertices.
    GX_TRIANGLEFAN = 0xA0,
    //Draws a series of unconnected quads. Every four vertices completes a quad. Internally, each quad is
    //translated into a pair of triangles.
    GX_QUADS = 0x80
};

//A primitive is forward facing if its vertices are clockwise to the viewer
enum GXCullMode {
    //Don't cull
    GX_CULL_NONE = 0,
    //Cull front facing primitives
    GX_CULL_FRONT = 1,
    //Cull back facing primitives
    GX_CULL_BACK = 2,
    //Cull all primitives
    GX_CULL_ALL = 3
};

//compare function to use
enum GXCompare {
    //Never passes
    GX_NEVER = 0,
    GX_LESS = 1,
    GX_LEQUAL = 2,
    GX_EQUAL = 3,
    GX_GREATER = 4,
    GX_NEQUAL = 5,
    GX_GEQUAL = 6,
    //Always passes
    GX_ALWAYS = 7
};

//Write to this to draw stuff
#define WG_PIPE ((WGPipe*) 0xCC008000)

#define _GXSetZMode ((void (*)(bool compareEnabled, GXCompare compareType, bool updateEnabled)) 0x801f4774)
#define _GXSetAlphaCompare ((void (*)(GXCompare comp0, u8 ref0, u32 op, GXCompare comp1, u8 ref1)) 0x801f3fd8)
//Width is in pixels / 6
//just set textOffsets to 0
#define _GXSetLineWidth ((void (*)(u8 width, int texOffsets)) 0x801f12ac)
#define _GXSetCullMode ((void (*)(GXCullMode cullMode)) 0x801f136c)

#define _GXBegin ((void (*)(GXPrimitive type, int format, u16 numVerts)) 0x801f1088)

#endif //PROJECTMCODES_GX_H
