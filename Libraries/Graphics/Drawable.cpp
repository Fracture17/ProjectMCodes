//
// Created by dareb on 8/8/2020.
//

#include "Drawable.h"
#include "Draw.h"

Point::Point(int color, float x1, float y1, int thickness) {
    this->color = color;
    this->x = x1;
    this->y = y1;
    this->thickness = thickness;
}

void Point::draw() {
    startNormalDraw();
    draw2DPoint(color, x, y, thickness);
}

Line::Line(int color, float x1, float y1, float x2, float y2, int thickness) {
    this->color = color;
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    this->thickness = thickness;
}

void Line::draw() {
    startNormalDraw();
    draw2DLine(color, x1, y1, x2, y2, thickness);
}