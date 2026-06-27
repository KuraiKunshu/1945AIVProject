#include "bullet.h"
#include "collision.h"
#include "raymath.h"
#include <math.h>
#include "utils.h"
#include <string.h>

typedef struct BulletTypeDef {
    BulletOwner owner;
    float width;
    float height;
    int frameCount;
    float animationFps;
    bool animated;
    bool swapDrawSize;
    float rotationOffset;
} BulletTypeDef;

static const BulletTypeDef bulletTypeDefs[BULLET_TYPE_COUNT] = {
    {
        BULLET_PLAYER,
        16.0f,
        32.0f,
        10,
        24.0f,
        true,
        true,
        0.0f
    },
    {
        BULLET_ENEMY,
        30.0f,
        15.0f,
        1,
        0.0f,
        false,
        false,
        90.0f
    }
};

static const BulletTypeDef* BulletTypeDefFor(BulletType type) {
    if (type < 0 || type >= BULLET_TYPE_COUNT) return &bulletTypeDefs[BULLET_TYPE_ENEMY_BASIC];
    return &bulletTypeDefs[type];
}

static BulletType BulletDefaultTypeForOwner(BulletOwner owner) {
    return owner == BULLET_PLAYER ? BULLET_TYPE_PLAYER_BASIC : BULLET_TYPE_ENEMY_BASIC;
}

static void BulletsRemoveActiveAt(BulletPool* p, size_t index) {
    if (!p || index >= p->active.count) return;
    for (size_t i = index + 1; i < p->active.count; ++i) {
        p->active.items[i - 1] = p->active.items[i];
    }
    p->active.count--;
}

void BulletsReset(BulletPool* p) {
    if (!p) return;
    memset(p->bullets, 0, sizeof(p->bullets));
    if (p->active.items == NULL) p->active = aiv_vector_new();
    else p->active.count = 0;
}

static void BulletsCompact(BulletPool* p) {
    size_t i = 0;
    while (i < p->active.count) {
        Bullet* b = aiv_vector_at(&p->active, i);
        if (b == NULL || !b->active) BulletsRemoveActiveAt(p, i);
        else ++i;
    }
}

Bullet* BulletsSpawnType(BulletPool* p, BulletType type, Vector2 pos, Vector2 vel, float ttl) {
    if (!p) return NULL;
    const BulletTypeDef* def = BulletTypeDefFor(type);
    BulletsCompact(p);
    for (int i = 0; i < BULLET_POOL_MAX; ++i) {
        Bullet* b = &p->bullets[i];
        if (!b->active) {
            b->active = true;
            b->owner = def->owner;
            b->type = type;
            b->pos = pos;
            b->vel = vel;
            b->ttl = ttl;
            b->w = def->width;
            b->h = def->height;
            AnimationInit(&b->animation, def->animationFps, true);
            aiv_vector_add(&p->active, b);
            return b;
        }
    }
    return NULL;
}

Bullet* BulletsSpawn(BulletPool* p, BulletOwner owner, Vector2 pos, Vector2 vel, float ttl) {
    return BulletsSpawnType(p, BulletDefaultTypeForOwner(owner), pos, vel, ttl);
}

void BulletsUpdate(BulletPool* p, float dt, int screenW, int screenH) {
    if (!p) return;
    size_t i = 0;
    while (i < p->active.count) {
        Bullet* b = aiv_vector_at(&p->active, i);
        if (b == NULL || !b->active) {
            BulletsRemoveActiveAt(p, i);
            continue;
        }
        b->pos.x += b->vel.x * dt;
        b->pos.y += b->vel.y * dt;
        b->ttl -= dt;
        const BulletTypeDef* def = BulletTypeDefFor(b->type);
        if (def->animated) {
            AnimationUpdate(&b->animation, def->frameCount, dt);
        }

        if (b->ttl <= 0.0f ||
            b->pos.x < -20 || b->pos.x > screenW + 20 ||
            b->pos.y < -40 || b->pos.y > screenH + 40) {
            b->active = false;
            BulletsRemoveActiveAt(p, i);
            continue;
        }
        ++i;
    }
}

Rectangle BulletBounds(const Bullet* b) {
    if (!b) return (Rectangle){0};
    return RectCentered(b->pos, b->w, b->h);
}

void BulletsDraw(
    const BulletPool* p,
    const SpriteSheet* playerSheet,
    Texture2D enemyTexture
) {
    if (!p) return;
    for (size_t i = 0; i < p->active.count; ++i) {
        const Bullet* b = p->active.items[i];
        if (!b->active) continue;

        const BulletTypeDef* def = BulletTypeDefFor(b->type);
        float angleRad = atan2f(b->vel.y, b->vel.x);
        float rotation = angleRad * (180.0f / PI) + def->rotationOffset;
        if (b->type == BULLET_TYPE_PLAYER_BASIC && playerSheet != NULL && playerSheet->texture.id != 0) {
            Rectangle destination = {
                b->pos.x,
                b->pos.y,
                def->swapDrawSize ? b->h : b->w,
                def->swapDrawSize ? b->w : b->h
            };
            AnimationDraw(playerSheet, &b->animation, destination, rotation, WHITE);
        } else {
            DrawTextureFitCentered(enemyTexture, b->pos, b->w, b->h, rotation, WHITE);
        }
    }
}

void BulletsShutdown(BulletPool* p) {
    if (!p) return;
    aiv_vector_destroy(&p->active);
    memset(p, 0, sizeof(*p));
}
