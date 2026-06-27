#include "actor.h"
#include "collision.h"

#include <string.h>

void ActorInit(
    Actor* actor,
    Vector2 pos,
    Vector2 vel,
    float width,
    float height,
    float rotation,
    float animationFps,
    bool animationLoop
) {
    if (actor == NULL) return;

    memset(actor, 0, sizeof(*actor));
    actor->pos = pos;
    actor->vel = vel;
    actor->w = width;
    actor->h = height;
    actor->rotation = rotation;
    AnimationInit(&actor->animation, animationFps, animationLoop);
}

void ActorMove(Actor* actor, float dt) {
    if (actor == NULL) return;

    actor->pos.x += actor->vel.x * dt;
    actor->pos.y += actor->vel.y * dt;
}

void ActorUpdateAnimation(Actor* actor, int frameCount, float dt) {
    if (actor == NULL) return;

    AnimationUpdate(&actor->animation, frameCount, dt);
}

Rectangle ActorBounds(const Actor* actor) {
    if (actor == NULL) return (Rectangle){0};

    return RectCentered(actor->pos, actor->w, actor->h);
}

void ActorDraw(const Actor* actor, const SpriteSheet* sheet, Color tint) {
    if (actor == NULL || sheet == NULL) return;

    Rectangle destination = {
        actor->pos.x,
        actor->pos.y,
        actor->w,
        actor->h
    };
    AnimationDraw(sheet, &actor->animation, destination, actor->rotation, tint);
}
