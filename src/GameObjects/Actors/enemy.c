#include "enemy.h"
#include "bullet.h"
#include "utils.h"
#include <math.h>
#include <string.h>

#define ENEMY_BULLET_SPEED 160.0f
#define ENEMY_BULLET_TTL 3.0f

static void EnemiesRemoveActiveAt(EnemyPool* p, size_t index) {
    if (!p || index >= p->active.count) return;
    for (size_t i = index + 1; i < p->active.count; ++i) {
        p->active.items[i - 1] = p->active.items[i];
    }
    p->active.count--;
}

void EnemiesReset(EnemyPool* p) {
    if (!p) return;
    memset(p->enemies, 0, sizeof(p->enemies));
    if (p->active.items == NULL) p->active = aiv_vector_new();
    else p->active.count = 0;
    p->spawnTimer = 0.0f;
    p->spawnInterval = 0.85f;
}

static void EnemiesCompact(EnemyPool* p) {
    size_t i = 0;
    while (i < p->active.count) {
        Enemy* e = aiv_vector_at(&p->active, i);
        if (e == NULL || !e->active) EnemiesRemoveActiveAt(p, i);
        else ++i;
    }
}

static float EnemyRotationFromVelocity(Vector2 vel) {
    if (vel.x == 0.0f && vel.y == 0.0f) return 0.0f;
    return atan2f(vel.y, vel.x) * (180.0f / PI) - 90.0f;
}

static float EnemyNextShootDelay(int score) {
    float scoreFactor = (float)score;
    float minDelay = ClampF(0.90f - scoreFactor * 0.0009f, 0.45f, 0.90f);
    float maxDelay = ClampF(1.60f - scoreFactor * 0.0012f, 0.85f, 1.60f);
    return RandF(minDelay, maxDelay);
}

static void EnemyTryShoot(Enemy* e, BulletPool* bullets, Vector2 target, int score) {
    if (e == NULL || bullets == NULL || !e->active) return;
    if (e->shootCooldown > 0.0f) return;

    e->shootCooldown = EnemyNextShootDelay(score);

    Vector2 toTarget = {
        target.x - e->actor.pos.x,
        target.y - e->actor.pos.y
    };
    float length = sqrtf(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
    if (length <= 0.0001f) return;

    Vector2 velocity = {
        (toTarget.x / length) * ENEMY_BULLET_SPEED,
        (toTarget.y / length) * ENEMY_BULLET_SPEED
    };
    BulletsSpawnType(
        bullets,
        BULLET_TYPE_ENEMY_BASIC,
        e->actor.pos,
        velocity,
        ENEMY_BULLET_TTL
    );
}

Enemy* EnemiesSpawn(EnemyPool* p, Vector2 pos, Vector2 vel, EnemyMove move, int score) {
    if (!p) return NULL;
    EnemiesCompact(p);
    for (int i = 0; i < ENEMY_POOL_MAX; ++i) {
        Enemy* e = &p->enemies[i];
        if (!e->active) {
            e->active = true;
            ActorInit(
                &e->actor,
                pos,
                vel,
                40.0f,
                40.0f,
                EnemyRotationFromVelocity(vel),
                8.0f,
                true
            );
            e->hp = 2;
            e->move = move;
            e->shootCooldown = EnemyNextShootDelay(score);
            e->spriteIndex = RandI(0, 3);
            aiv_vector_add(&p->active, e);
            return e;
        }
    }
    return NULL;
}



void EnemiesUpdate(
    EnemyPool* p,
    BulletPool* bullets,
    Vector2 target,
    int score,
    float dt,
    int screenW,
    int screenH
) {
    if (!p) return;
    size_t i = 0;
    while (i < p->active.count) {
        Enemy* e = aiv_vector_at(&p->active, i);
        if (e == NULL || !e->active) {
            EnemiesRemoveActiveAt(p, i);
            continue;
        }
        
        ActorMove(&e->actor, dt); 
        e->shootCooldown -= dt;
        EnemyTryShoot(e, bullets, target, score);
        ActorUpdateAnimation(&e->actor, 3, dt);

        if (e->actor.pos.y > screenH + 80 ||
            e->actor.pos.y < -80 ||
            e->actor.pos.x < -80 ||
            e->actor.pos.x > screenW + 80) {
            e->active = false;
            EnemiesRemoveActiveAt(p, i);
            continue;
        }
        ++i;
    }
}

Rectangle EnemyBounds(const Enemy* e) {
    if (!e) return (Rectangle){0};
    return ActorBounds(&e->actor);
}

void EnemiesDraw(const EnemyPool* p, const SpriteSheet* sheets, size_t sheetCount) {
    if (!p || !sheets || sheetCount == 0) return;
    for (size_t i = 0; i < p->active.count; ++i) {
        const Enemy* e = p->active.items[i];
        if (!e->active) continue;

        size_t sheetIndex = (size_t)e->spriteIndex % sheetCount;
        ActorDraw(&e->actor, &sheets[sheetIndex], WHITE);
    }
}

void EnemiesShutdown(EnemyPool* p) {
    if (!p) return;
    aiv_vector_destroy(&p->active);
    memset(p, 0, sizeof(*p));
}
