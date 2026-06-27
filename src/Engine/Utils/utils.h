#pragma once
#include "raylib.h"
#include <stdbool.h>

float ClampF(float v, float lo, float hi);
float RandF(float lo, float hi);
int RandI(int lo, int hi);

typedef struct Timer {
    float t;
    float limit;
    bool enabled;
} Timer;

void TimerStart(Timer* tm, float limit);
bool TimerTick(Timer* tm, float dt);
void DrawTextureFitCentered(Texture2D tex, Vector2 center, float w, float h, float rotationDeg, Color tint);