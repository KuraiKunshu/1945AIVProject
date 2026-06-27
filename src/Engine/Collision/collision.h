#pragma once
#include "raylib.h"
#include <stdbool.h>

bool RectsOverlap(Rectangle a, Rectangle b);
Rectangle RectCentered(Vector2 center, float w, float h);
