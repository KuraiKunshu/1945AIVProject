#include "collision.h"

bool RectsOverlap(Rectangle a, Rectangle b) {
    return (a.x < (b.x + b.width)) &&
           ((a.x + a.width) > b.x) &&
           (a.y < (b.y + b.height)) &&
           ((a.y + a.height) > b.y);
}

Rectangle RectCentered(Vector2 c, float w, float h) {
    Rectangle r = { c.x - w*0.5f, c.y - h*0.5f, w, h };
    return r;
}
