//
// Created by dareb on 8/8/2020.
//

#include "Drawable.h"
#include "Draw.h"

Point::Point(int in_color, float in_x1, float in_y1, int in_thickness) {
    color = in_color;
    x = in_x1;
    y = in_y1;
    thickness = in_thickness;
}

void Point::draw() {
    draw2DPoint(color, x, y, thickness);
}

Line::Line(int in_color, float in_x1, float in_y1, float in_x2, float in_y2, int in_thickness) {
    color = in_color;
    x1 = in_x1;
    x2 = in_x2;
    y1 = in_y1;
    y2 = in_y2;
    thickness = in_thickness;
}

void Line::draw() {
    draw2DLine(color, x1, y1, x2, y2, thickness);
}

void RectOutline::draw() {
    draw2DRectOutline(color, top, bottom, left, right, thickness);
}