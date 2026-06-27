#pragma once
#include <stdbool.h>
#include "raylib.h"
#include "aiv_vector.h"
#include "actor.h"

typedef struct BulletPool BulletPool;

typedef enum EnemyMove {
    ENEMY_STRAIGHT = 0,
    ENEMY_UPSIDE = 1,
    ENEMY_DIAGONAL = 2,
    ENEMY_DIAGONAL_UPSIDE = 3
} EnemyMove;

typedef struct Enemy {
    bool active;
    Actor actor;
    int hp;
    EnemyMove move;
    float shootCooldown;
    int spriteIndex;
} Enemy;

#define ENEMY_POOL_MAX 48

typedef struct EnemyPool {
    Enemy enemies[ENEMY_POOL_MAX];
    aiv_vector_t active;
    float spawnTimer;
    float spawnInterval;
} EnemyPool;

void EnemiesReset(EnemyPool* e);
Enemy* EnemiesSpawn(EnemyPool* e, Vector2 pos, Vector2 vel, EnemyMove move, int score);
void EnemiesUpdate(
    EnemyPool* e,
    BulletPool* bullets,
    Vector2 target,
    int score,
    float dt,
    int screenW,
    int screenH
);
void EnemiesDraw(const EnemyPool* e, const SpriteSheet* sheets, size_t sheetCount);
void EnemiesShutdown(EnemyPool* e);

Rectangle EnemyBounds(const Enemy* e);
