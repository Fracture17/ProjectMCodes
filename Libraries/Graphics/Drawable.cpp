//
// Created by dareb on 8/8/2020.
//

#include "Drawable.h"
#include "Draw.h"
#include "Containers/vector.h"
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

Renderables renderables;

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

void Point::_draw() {
    draw2DPoint(this->color, this->x, this->y, this->thickness);
}

void Line::_draw() {
    draw2DLine(color, x1, y1, x2, y2, thickness);
}

void RectOutline::_draw() {
    draw2DRectOutline(color, top, bottom, left, right, thickness);
}

void Rect::_draw() {
//    OSReport("addr: %08x\n", this);
//    OSReport("rect draw: (col, t, b, l, r): %08x, %.3f, %.3f, %.3f, %.3f\n", color.value, top, bottom, left, right);
    draw2DRectangle(color, top, bottom, left, right);
}

// this is the only way I could get it to work for now
void Drawable::draw() {
    setupDrawPrimitives();
//    OSReport("(delay, lifetime): %u, %u\n", delay, lifeTime);
    if (delay == 0) {
        if (is2D) { start2DDraw(); }
        else { startNormalDraw(); }
        switch(drawKind) {
            case 0:
                (*(Point*)this)._draw();
                break;
            case 1:
                (*(Line*)this)._draw();
                break;
            case 2:
                (*(RectOutline*)this)._draw();
                break;
            case 3:
                (*(Rect*)this)._draw();
                break;
        }
        if (autoTimer) { lifeTime --; }
    } else { if (autoTimer) { delay --; } }
}

void Renderables::renderAll() {

//    for (int i = 0; i < 8; i++) {
//        vector<Drawable> * d;
//        if (i == 0) d = (vector<Drawable>*) &points.frame;
//        else if (i == 1) d = (vector<Drawable>*) &points.tick;
//        else if (i == 2) d = (vector<Drawable>*) &lines.frame;
//        else if (i == 3) d = (vector<Drawable>*) &lines.tick;
//        else if (i == 4) d = (vector<Drawable>*) &rectOutlines.frame;
//        else if (i == 5) d = (vector<Drawable>*) &rectOutlines.tick;
//        else if (i == 6) d = (vector<Drawable>*) &rects.frame;
//        else if (i == 7) d = (vector<Drawable>*) &rects.tick;
//
//        for (int j = 0; j < (*d).size(); j++) {
//            if ((*d)[j].delay == 0) {
//                (*d)[j].lifeTime --;
//            } else {
//                (*d)[j].delay --;
//            }
//            if ((*d)[j].lifeTime == 0) {
//                (*d).erase(j);
//            }
//        }
//    }
//    for (int i = 0; i < points.frame.size(); i++) {
//        points.frame[i].draw();
//        if (points.frame[i].lifeTime == 0) {
//            points.frame.erase(i);
//        }
//    }
    for (int i = 0; i < points.tick.size(); i++) { points.tick[i].draw(); if (points.tick[i].lifeTime == 0) points.tick.erase(i); }
    for (int i = 0; i < points.frame.size(); i++) { points.frame[i].draw(); if (points.frame[i].lifeTime == 0) points.frame.erase(i); }
    for (int i = 0; i < lines.tick.size(); i++) { lines.tick[i].draw(); if (lines.tick[i].lifeTime == 0) lines.tick.erase(i); }
    for (int i = 0; i < lines.frame.size(); i++) { lines.frame[i].draw(); if (lines.frame[i].lifeTime == 0) lines.frame.erase(i); }
    for (int i = 0; i < rectOutlines.tick.size(); i++) { rectOutlines.tick[i].draw(); if (rectOutlines.tick[i].lifeTime == 0) rectOutlines.tick.erase(i); }
    for (int i = 0; i < rectOutlines.frame.size(); i++) { rectOutlines.frame[i].draw(); if (rectOutlines.frame[i].lifeTime == 0) rectOutlines.frame.erase(i); }
    for (int i = 0; i < rects.tick.size(); i++) { rects.tick[i].draw(); if (rects.tick[i].lifeTime == 0) rects.tick.erase(i); }
    for (int i = 0; i < rects.frame.size(); i++) { rects.frame[i].draw(); if (rects.frame[i].lifeTime == 0) rects.frame.erase(i); }
}

// FIXME: temporary solution - need to be fixed for general purpose use
void Renderables::updateTick() {
    for (int i = 0; i < points.tick.size(); i++) {
        if (points.tick[i].delay == 0) {
            points.tick[i].lifeTime --;
            if (points.tick[i].lifeTime == 0) { points.tick.erase(i); }
        } else {
            points.tick[i].delay --;
        }
    }
    for (int i = 0; i < lines.tick.size(); i++) {
        if (lines.tick[i].delay == 0) {
            lines.tick[i].lifeTime --;
            if (lines.tick[i].lifeTime == 0) { lines.tick.erase(i); }
        } else {
            lines.tick[i].delay --;
        }
    }
    for (int i = 0; i < rectOutlines.tick.size(); i++) {
        if (rectOutlines.tick[i].delay == 0) {
            rectOutlines.tick[i].lifeTime --;
            if (rectOutlines.tick[i].lifeTime == 0) { rectOutlines.tick.erase(i); }
        } else {
            rectOutlines.tick[i].delay --;
        }
    }
    for (int i = 0; i < rects.tick.size(); i++) {
        if (rects.tick[i].delay == 0) {
            rects.tick[i].lifeTime --;
            if (rects.tick[i].lifeTime == 0) { rects.tick.erase(i); }
        } else {
            rects.tick[i].delay --;
        }
    }
}