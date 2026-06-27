#pragma once

#include <stdbool.h>
#include "raylib.h"
#include "animation.h"

typedef struct Actor {
    Vector2 pos;
    Vector2 vel;
    float w;
    float h;
    float rotation;
    SpriteAnimation animation;
} Actor;

void ActorInit(
    Actor* actor,
    Vector2 pos,
    Vector2 vel,
    float width,
    float height,
    float rotation,
    float animationFps,
    bool animationLoop
);
void ActorMove(Actor* actor, float dt);
void ActorUpdateAnimation(Actor* actor, int frameCount, float dt);
Rectangle ActorBounds(const Actor* actor);
void ActorDraw(const Actor* actor, const SpriteSheet* sheet, Color tint);
