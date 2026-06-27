#include "utils.h"
#include "raylib.h"
#include <stdlib.h>

float ClampF(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

float RandF(float lo, float hi) {
    float t = (float)rand() / (float)RAND_MAX;
    return lo + (hi - lo) * t;
}

int RandI(int lo, int hi) {
    if (hi <= lo) return lo;
    int span = (hi - lo) + 1;
    return lo + (rand() % span);
}

void TimerStart(Timer* tm, float limit) {
    if (!tm) return;
    tm->t = 0.0f;
    tm->limit = limit;
    tm->enabled = true;
}

bool TimerTick(Timer* tm, float dt) {
    if (!tm || !tm->enabled) return false;
    tm->t += dt;
    if (tm->t >= tm->limit) {
        tm->enabled = false;
        return true;
    }
    return false;
}

void DrawTextureFitCentered(Texture2D tex, Vector2 center, float w, float h, float rotationDeg, Color tint)
{
    Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
    Rectangle dst = { center.x, center.y, w, h };
    Vector2 origin = { w * 0.5f, h * 0.5f };
    DrawTexturePro(tex, src, dst, origin, rotationDeg, tint);
}
