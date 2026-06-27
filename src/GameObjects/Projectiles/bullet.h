#pragma once
#include <stdbool.h>
#include "raylib.h"
#include "aiv_vector.h"
#include "animation.h"

typedef enum BulletOwner {
    BULLET_PLAYER = 0,
    BULLET_ENEMY  = 1
} BulletOwner;

typedef enum BulletType {
    BULLET_TYPE_PLAYER_BASIC = 0,
    BULLET_TYPE_ENEMY_BASIC = 1,
    BULLET_TYPE_COUNT
} BulletType;

typedef struct Bullet {
    bool active;
    BulletOwner owner;
    BulletType type;
    Vector2 pos;
    Vector2 vel;
    float w, h;
    float ttl;
    SpriteAnimation animation;
} Bullet;

#define BULLET_POOL_MAX 128

typedef struct BulletPool {
    Bullet bullets[BULLET_POOL_MAX];
    aiv_vector_t active;
} BulletPool;

void BulletsReset(BulletPool* p);
Bullet* BulletsSpawn(BulletPool* p, BulletOwner owner, Vector2 pos, Vector2 vel, float ttl);
Bullet* BulletsSpawnType(BulletPool* p, BulletType type, Vector2 pos, Vector2 vel, float ttl);
void BulletsUpdate(BulletPool* p, float dt, int screenW, int screenH);
void BulletsDraw(
    const BulletPool* p,
    const SpriteSheet* playerSheet,
    Texture2D enemyTexture
);
void BulletsShutdown(BulletPool* p);

Rectangle BulletBounds(const Bullet* b);
