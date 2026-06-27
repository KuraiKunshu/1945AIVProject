#pragma once

#include <stdbool.h>
#include "aiv_vector.h"
#include "animation.h"

#define EXPLOSION_POOL_MAX 64

typedef enum ExplosionKind {
    EXPLOSION_ENEMY = 0,
    EXPLOSION_PLAYER,
    EXPLOSION_PLAYER_BULLET_HIT
} ExplosionKind;

typedef struct Explosion {
    bool active;
    ExplosionKind kind;
    Vector2 position;
    float size;
    SpriteAnimation animation;
} Explosion;

typedef struct ExplosionPool {
    Explosion items[EXPLOSION_POOL_MAX];
    aiv_vector_t active;
} ExplosionPool;

void ExplosionsReset(ExplosionPool* pool);
void ExplosionsShutdown(ExplosionPool* pool);
bool ExplosionsSpawn(
    ExplosionPool* pool,
    ExplosionKind kind,
    Vector2 position,
    float size
);
void ExplosionsUpdate(
    ExplosionPool* pool,
    const SpriteSheet* enemySheet,
    const SpriteSheet* playerSheet,
    const SpriteSheet* playerBulletHitSheet,
    float deltaTime
);
void ExplosionsDraw(
    const ExplosionPool* pool,
    const SpriteSheet* enemySheet,
    const SpriteSheet* playerSheet,
    const SpriteSheet* playerBulletHitSheet
);
