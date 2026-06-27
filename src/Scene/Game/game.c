#include "game.h"
#include "collision.h"
#include "input.h"
#include "utils.h"
#include "raymath.h"
#include <string.h>

static const float startSpawnTime = 0.90f;
static const float minSpawnTime = 0.35f;
static const float difficultyGrowth = 0.0008f;

static const float ENERGY_REGEN_MULTIPLIER = 0.5f;

void GameInit(Game* g, int screenW, int screenH) {
    if (!g) return;
    memset(g, 0, sizeof(*g));
    g->screenW = screenW;
    g->screenH = screenH;
    g->state = GAME_SPLASH;

    UiInit(&g->ui);
    AudioInit(&g->audio);

    LevelReset(&g->level);
    BulletsReset(&g->bullets);
    EnemiesReset(&g->enemies);
    PlayerReset(&g->player, screenW, screenH);
    ExplosionsReset(&g->explosions);
    AssetsLoad(&g->assets);
}

void GameResetPlay(Game* g) {
    if (!g) return;
    LevelReset(&g->level);
    BulletsReset(&g->bullets);
    EnemiesReset(&g->enemies);
    PlayerReset(&g->player, g->screenW, g->screenH);
    ExplosionsReset(&g->explosions);
    g->state = GAME_PLAYING;
    g->ui.showSplash = false;
    g->ui.showGameOver = false;
}

static void SpawnPlayerBulletHit(Game* g, Vector2 position) {
    ExplosionsSpawn(
        &g->explosions,
        EXPLOSION_PLAYER_BULLET_HIT,
        position,
        48.0f
    );
}

static void DrawTextureFullscreen(Texture2D texture, int screenW, int screenH) {
    if (texture.id == 0) return;

    Rectangle source = {
        0.0f,
        0.0f,
        (float)texture.width,
        (float)texture.height
    };
    Rectangle destination = {
        0.0f,
        0.0f,
        (float)screenW,
        (float)screenH
    };
    DrawTexturePro(texture, source, destination, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
}

static void SpawnEnemyWave(Game* g) {
    float scoreFactor = (float)g->player.score;

    float minSpeed = ClampF(80.0f + scoreFactor * 0.03f, 80.0f, 160.0f);
    float maxSpeed = ClampF(120.0f + scoreFactor * 0.04f, 120.0f, 220.0f);
    float speed = RandF(minSpeed, maxSpeed);
    
    float x = RandF(40.0f, g->screenW - 40.0f);
    float y = RandF(40.0f, g->screenH - 40.0f);
    Vector2 pos, vel;
    EnemyMove mv = (EnemyMove)RandI(0, 3);
    switch (mv)
    {
        case ENEMY_UPSIDE:
            pos = (Vector2){ x, g->screenH+20.0f };
            vel = (Vector2){ 0.0f, -speed };
            break;
        case ENEMY_DIAGONAL:
            pos = (Vector2){ -40, y };
            vel = Vector2Scale(Vector2Normalize((Vector2){ 1.0f, 0.65f }), speed);
            break;
        case ENEMY_DIAGONAL_UPSIDE:
            pos = (Vector2){ g->screenW+40, y };
            vel = Vector2Scale(Vector2Normalize((Vector2){ -1.0f, -0.65f }), speed);
            break;
        case ENEMY_STRAIGHT:
            pos = (Vector2){ x, -20.0f };
            vel = (Vector2){ 0.0f, speed };
            break;
    }
    
    EnemiesSpawn(&g->enemies, pos, vel, mv, g->player.score);
}

static void ExplodeEnemy(Game* g, Vector2 position, float size) {
    ExplosionsSpawn(&g->explosions, EXPLOSION_ENEMY, position, size);
    AudioPlayExplosion(&g->audio);
}

static bool DamagePlayer(Game* g) {
    int previousLives = g->player.lives;
    PlayerOnHit(&g->player);
    if (g->player.lives == previousLives) return false;

    ExplosionsSpawn(
        &g->explosions,
        EXPLOSION_PLAYER,
        g->player.actor.pos,
        g->player.actor.w
    );
    AudioPlayExplosion(&g->audio);
    return true;
}

static void ResolveCollisions(Game* g) {
    Rectangle pr = ActorBounds(&g->player.actor);

    for (size_t i = 0; i < g->enemies.active.count; ++i) {
        Enemy* e = aiv_vector_at(&g->enemies.active, i);
        if (!e->active) continue;
        if (RectsOverlap(pr, EnemyBounds(e))) {
            e->active = false;
            ExplodeEnemy(g, e->actor.pos, e->actor.w * 1.5f);
            DamagePlayer(g);
            if (g->player.lives <= 0) return;
        }
    }

    for (size_t i = 0; i < g->bullets.active.count; ++i) {
        Bullet* b = aiv_vector_at(&g->bullets.active, i);
        if (!b->active) continue;

        Rectangle br = BulletBounds(b);

        if (b->owner == BULLET_ENEMY) {
            if (RectsOverlap(br, pr)) {
                b->active = false;
                DamagePlayer(g);
                if (g->player.lives <= 0) return;
            }
            continue;
        }

        for (size_t j = 0; j < g->enemies.active.count; ++j) {
            Enemy* e = aiv_vector_at(&g->enemies.active, j);
            if (!e->active) continue;
            if (RectsOverlap(br, EnemyBounds(e))) {
                SpawnPlayerBulletHit(g, b->pos);
                b->active = false;
                e->hp -= 1;
                if (e->hp <= 0) {
                    e->active = false;
                    ExplodeEnemy(g, e->actor.pos, e->actor.w * 1.5f);
                    g->player.score += 10;
                    g->player.energy = ClampF(g->player.energy + 6.0f, 0.0f, 100.0f);
                }
                break;
            }
        }
    }

    for (size_t i = 0; i < g->bullets.active.count; ++i) {
        Bullet* a = aiv_vector_at(&g->bullets.active, i);
        if (!a->active) continue;
        if (a->owner != BULLET_PLAYER) continue;
        Rectangle ar = BulletBounds(a);

        for (size_t j = 0; j < g->bullets.active.count; ++j) {
            Bullet* b = aiv_vector_at(&g->bullets.active, j);
            if (!b->active) continue;
            if (b->owner != BULLET_ENEMY) continue;
            if (RectsOverlap(ar, BulletBounds(b))) {
                Vector2 impact = {
                    (a->pos.x + b->pos.x) * 0.5f,
                    (a->pos.y + b->pos.y) * 0.5f
                };
                a->active = false;
                b->active = false;
                ExplosionsSpawn(
                    &g->explosions,
                    EXPLOSION_ENEMY,
                    impact,
                    24.0f
                );
                break;
            }
        }
    }
}

void GameUpdate(Game* g, float dt) {
    if (!g) return;

    AudioUpdate(&g->audio);
    ExplosionsUpdate(
        &g->explosions,
        &g->assets.enemyExplosion,
        &g->assets.playerExplosion,
        &g->assets.playerBulletHit,
        dt
    );

    if (g->state == GAME_SPLASH) {
        UiUpdate(&g->ui);
        if (InputConfirmPressed()) {
            if (g->ui.menuIndex == 0) GameResetPlay(g);
            else g->exitRequested = true;
        }
        return;
    }

    if (g->state == GAME_GAMEOVER) {
        if (InputConfirmPressed()) {
            g->ui.showSplash = true;
            g->ui.menuIndex = 0;
            g->ui.showGameOver = false;
            g->state = GAME_SPLASH;
        }
        return;
    }

    LevelUpdate(&g->level, dt, g->screenW, g->screenH);

    float scoreFactor = (float)g->player.score;

    g->enemies.spawnInterval = ClampF(
        startSpawnTime - scoreFactor * difficultyGrowth,
        minSpawnTime,
        startSpawnTime
    );

    g->enemies.spawnTimer += dt;
    if (g->enemies.spawnTimer >= g->enemies.spawnInterval) {
        g->enemies.spawnTimer = 0.0f;
        SpawnEnemyWave(g);
    }

    if (PlayerUpdate(&g->player, &g->bullets, dt, g->screenW, g->screenH)) {
        AudioPlayShoot(&g->audio);
    }
    EnemiesUpdate(
        &g->enemies,
        &g->bullets,
        g->player.actor.pos,
        g->player.score,
        dt,
        g->screenW,
        g->screenH
    );
    BulletsUpdate(&g->bullets, dt, g->screenW, g->screenH);

    ResolveCollisions(g);

    g->player.energy = ClampF(g->player.energy + ENERGY_REGEN_MULTIPLIER * dt, 0.0f, 100.0f);

    if (g->player.lives <= 0) {
        g->state = GAME_GAMEOVER;
        g->ui.showGameOver = true;
    }
}

void GameDraw(Game* g) {
    if (!g) return;

    BeginDrawing();
    ClearBackground(BLACK);

    if (g->state == GAME_SPLASH) {
        DrawTextureFullscreen(g->assets.menuBackground, g->screenW, g->screenH);
        UiDrawSplash(&g->ui);
        EndDrawing();
        return;
    }

    if (g->state == GAME_GAMEOVER) {
        LevelDraw(
            &g->level,
            g->assets.water,
            g->assets.islands,
            ISLAND_TEXTURE_COUNT,
            g->screenW,
            g->screenH
        );
        ExplosionsDraw(
            &g->explosions,
            &g->assets.enemyExplosion,
            &g->assets.playerExplosion,
            &g->assets.playerBulletHit
        );
        UiDrawGameOver(&g->ui, g->player.score);
        EndDrawing();
        return;
    }

    LevelDraw(
        &g->level,
        g->assets.water,
        g->assets.islands,
        ISLAND_TEXTURE_COUNT,
        g->screenW,
        g->screenH
    );
    EnemiesDraw(&g->enemies, g->assets.enemies, ENEMY_SPRITE_COUNT);
    BulletsDraw(
        &g->bullets,
        &g->assets.playerBullet,
        g->assets.enemyBullet
    );
    PlayerDraw(&g->player, &g->assets.player);
    ExplosionsDraw(
        &g->explosions,
        &g->assets.enemyExplosion,
        &g->assets.playerExplosion,
        &g->assets.playerBulletHit
    );

    UiDrawHud(&g->ui, &g->player);
    EndDrawing();
}

void GameShutdown(Game* g) {
    if (!g) return;

    AudioShutdown(&g->audio);
    AssetsUnload(&g->assets);
    ExplosionsShutdown(&g->explosions);
    EnemiesShutdown(&g->enemies);
    BulletsShutdown(&g->bullets);
    LevelShutdown(&g->level);
    memset(g, 0, sizeof(*g));
}
