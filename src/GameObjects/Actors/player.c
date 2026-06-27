#include "player.h"
#include "bullet.h"
#include "input.h"
#include "utils.h"
#include <string.h>

#define PLAYER_SPEED 220.0f
#define PLAYER_BULLET_SPEED 320.0f
#define PLAYER_BULLET_TTL 1.4f
#define PLAYER_SHOOT_DELAY 0.14f
#define PLAYER_SHOOT_ENERGY_COST 0.8f

void PlayerReset(Player* p, int screenW, int screenH) {
    if (!p) return;
    memset(p, 0, sizeof(*p));
    ActorInit(
        &p->actor,
        (Vector2){ screenW * 0.5f, screenH - 70.0f },
        (Vector2){0},
        80.0f,
        80.0f,
        0.0f,
        10.0f,
        true
    );
    p->lives = 3;
    p->energy = 100.0f;
    p->score = 0;
    p->controlEnabled = true;
}

static bool PlayerTryShoot(Player* p, BulletPool* bullets, float dt) {
    if (p == NULL || bullets == NULL) return false;

    p->shootCooldown -= dt;
    if (p->shootCooldown < 0.0f) p->shootCooldown = 0.0f;
    if (!p->controlEnabled) return false;
    if (!InputShootDown()) return false;
    if (p->shootCooldown > 0.0f) return false;

    Vector2 pos = {
        p->actor.pos.x,
        p->actor.pos.y - 18.0f
    };
    Vector2 vel = {
        0.0f,
        -PLAYER_BULLET_SPEED
    };

    if (!BulletsSpawnType(bullets, BULLET_TYPE_PLAYER_BASIC, pos, vel, PLAYER_BULLET_TTL)) {
        return false;
    }

    p->shootCooldown = PLAYER_SHOOT_DELAY;
    p->energy = ClampF(p->energy - PLAYER_SHOOT_ENERGY_COST, 0.0f, 100.0f);
    return true;
}

bool PlayerUpdate(Player* p, BulletPool* bullets, float dt, int screenW, int screenH) {
    if (!p) return false;

    if (p->invulnerable) {
        p->invulnTimeLeft -= dt;
        p->blinkAccum += dt;
        if (p->invulnTimeLeft <= 0.0f) {
            p->invulnerable = false;
            p->controlEnabled = true;
            p->blinkAccum = 0.0f;
        }
    }

    bool didShoot = PlayerTryShoot(p, bullets, dt);

    if (p->controlEnabled) {
        Vector2 d = InputMoveDirection();
        p->actor.vel = (Vector2){ d.x * PLAYER_SPEED, d.y * PLAYER_SPEED };
    } else {
        p->actor.vel = (Vector2){0};
    }

    ActorMove(&p->actor, dt);

    float halfW = p->actor.w * 0.5f;
    float halfH = p->actor.h * 0.5f;
    p->actor.pos.x = ClampF(p->actor.pos.x, halfW, screenW - halfW);
    p->actor.pos.y = ClampF(p->actor.pos.y, halfH, screenH - halfH);
    ActorUpdateAnimation(&p->actor, 3, dt);
    return didShoot;
}

void PlayerOnHit(Player* p) {
    if (!p) return;
    if (p->invulnerable) return;

    p->lives -= 1;
    p->energy = 100.0f;

    p->controlEnabled = false;
    p->invulnerable = true;
    p->invulnTimeLeft = 3.0f;
    p->blinkAccum = 0.0f;
}

Rectangle PlayerBounds(const Player* p) {
    if (!p) return (Rectangle){0};
    return ActorBounds(&p->actor);
}

void PlayerDraw(const Player* p, const SpriteSheet* sheet) {
    if (!p || !sheet) return;

    if (p->invulnerable) {
        int phase = (int)(p->blinkAccum / 0.12f);
        if (phase % 2 == 0) return;
    }

    ActorDraw(&p->actor, sheet, WHITE);
}
