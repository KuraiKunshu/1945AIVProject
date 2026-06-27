#pragma once

#include <stdbool.h>
#include "raylib.h"

typedef struct SpriteSheet {
    Texture2D texture;
    int frameCount;
} SpriteSheet;

typedef struct SpriteAnimation {
    int currentFrame;
    float frameDuration;
    float elapsed;
    bool loop;
    bool finished;
} SpriteAnimation;

void AnimationInit(SpriteAnimation* animation, float framesPerSecond, bool loop);
void AnimationReset(SpriteAnimation* animation);
void AnimationUpdate(SpriteAnimation* animation, int frameCount, float deltaTime);
Rectangle AnimationSource(const SpriteSheet* sheet, int frame);
void AnimationDraw(
    const SpriteSheet* sheet,
    const SpriteAnimation* animation,
    Rectangle destination,
    float rotation,
    Color tint
);
