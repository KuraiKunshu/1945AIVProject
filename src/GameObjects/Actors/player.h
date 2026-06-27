#pragma once
#include <stdbool.h>
#include "raylib.h"
#include "actor.h"

typedef struct BulletPool BulletPool;

typedef struct Player {
    Actor actor;

    int lives;
    float energy;  // 0..100
    int score;

    bool controlEnabled;

    bool invulnerable;
    float invulnTimeLeft;
    float blinkAccum;

    float shootCooldown;
} Player;

void PlayerReset(Player* p, int screenW, int screenH);
bool PlayerUpdate(Player* p, BulletPool* bullets, float dt, int screenW, int screenH);
void PlayerDraw(const Player* p, const SpriteSheet* sheet);

void PlayerOnHit(Player* p);
Rectangle PlayerBounds(const Player* p);
