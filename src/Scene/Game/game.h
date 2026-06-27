#pragma once
#include <stdbool.h>
#include "raylib.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "level.h"
#include "ui.h"
#include "audio.h"
#include "assets.h"
#include "explosion.h"

typedef enum GameState {
    GAME_SPLASH = 0,
    GAME_PLAYING,
    GAME_GAMEOVER
} GameState;

typedef struct Game {
    int screenW, screenH;
    GameState state;
    bool exitRequested;

    Player player;
    EnemyPool enemies;
    BulletPool bullets;

    Level level;
    UiState ui;
    AudioSys audio;
    Assets assets;
    ExplosionPool explosions;
} Game;

void GameInit(Game* g, int screenW, int screenH);
void GameResetPlay(Game* g);
void GameUpdate(Game* g, float dt);
void GameDraw(Game* g);
void GameShutdown(Game* g);
