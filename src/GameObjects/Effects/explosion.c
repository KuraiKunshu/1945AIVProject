#include "explosion.h"

#include <string.h>

static void ExplosionsRemoveActiveAt(ExplosionPool* pool, size_t index)
{
    if (pool == NULL || index >= pool->active.count) return;
    for (size_t i = index + 1; i < pool->active.count; ++i) {
        pool->active.items[i - 1] = pool->active.items[i];
    }
    pool->active.count--;
}

static void ExplosionsCompact(ExplosionPool* pool)
{
    if (pool == NULL) return;
    size_t i = 0;
    while (i < pool->active.count) {
        Explosion* explosion = aiv_vector_at(&pool->active, i);
        if (explosion == NULL || !explosion->active) ExplosionsRemoveActiveAt(pool, i);
        else ++i;
    }
}

void ExplosionsReset(ExplosionPool* pool)
{
    if (pool == NULL) return;
    memset(pool->items, 0, sizeof(pool->items));
    if (pool->active.items == NULL) pool->active = aiv_vector_new();
    else pool->active.count = 0;
}

void ExplosionsShutdown(ExplosionPool* pool)
{
    if (pool == NULL) return;
    aiv_vector_destroy(&pool->active);
    memset(pool, 0, sizeof(*pool));
}

bool ExplosionsSpawn(
    ExplosionPool* pool,
    ExplosionKind kind,
    Vector2 position,
    float size
)
{
    if (pool == NULL) return false;

    ExplosionsCompact(pool);

    for (int i = 0; i < EXPLOSION_POOL_MAX; ++i) {
        Explosion* explosion = &pool->items[i];
        if (explosion->active) continue;

        explosion->active = true;
        explosion->kind = kind;
        explosion->position = position;
        explosion->size = size;
        AnimationInit(&explosion->animation, 18.0f, false);
        aiv_vector_add(&pool->active, explosion);
        return true;
    }
    return false;
}

void ExplosionsUpdate(
    ExplosionPool* pool,
    const SpriteSheet* enemySheet,
    const SpriteSheet* playerSheet,
    const SpriteSheet* playerBulletHitSheet,
    float deltaTime
)
{
    if (pool == NULL || enemySheet == NULL || playerSheet == NULL || playerBulletHitSheet == NULL) return;

    size_t i = 0;
    while (i < pool->active.count) {
        Explosion* explosion = aiv_vector_at(&pool->active, i);
        if (explosion == NULL || !explosion->active) {
            ExplosionsRemoveActiveAt(pool, i);
            continue;
        }

        const SpriteSheet* sheet = enemySheet;
        if (explosion->kind == EXPLOSION_PLAYER) sheet = playerSheet;
        if (explosion->kind == EXPLOSION_PLAYER_BULLET_HIT) sheet = playerBulletHitSheet;
        AnimationUpdate(&explosion->animation, sheet->frameCount, deltaTime);
        if (explosion->animation.finished) {
            explosion->active = false;
            ExplosionsRemoveActiveAt(pool, i);
            continue;
        }
        ++i;
    }
}

void ExplosionsDraw(
    const ExplosionPool* pool,
    const SpriteSheet* enemySheet,
    const SpriteSheet* playerSheet,
    const SpriteSheet* playerBulletHitSheet
)
{
    if (pool == NULL || enemySheet == NULL || playerSheet == NULL || playerBulletHitSheet == NULL) return;

    for (size_t i = 0; i < pool->active.count; ++i) {
        const Explosion* explosion = pool->active.items[i];
        if (explosion == NULL || !explosion->active) continue;

        const SpriteSheet* sheet = enemySheet;
        if (explosion->kind == EXPLOSION_PLAYER) sheet = playerSheet;
        if (explosion->kind == EXPLOSION_PLAYER_BULLET_HIT) sheet = playerBulletHitSheet;
        Rectangle destination = {
            explosion->position.x,
            explosion->position.y,
            explosion->size,
            explosion->size
        };
        AnimationDraw(sheet, &explosion->animation, destination, 0.0f, WHITE);
    }
}
