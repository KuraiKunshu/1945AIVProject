#pragma once

#include <stdbool.h>
#include "animation.h"

#define ENEMY_SPRITE_COUNT 4
#define ISLAND_TEXTURE_COUNT 3

typedef struct GameAssets {
    SpriteSheet player;
    SpriteSheet enemies[ENEMY_SPRITE_COUNT];
    SpriteSheet enemyExplosion;
    SpriteSheet playerExplosion;
    SpriteSheet playerBullet;
    SpriteSheet playerBulletHit;

    Texture2D enemyBullet;
    Texture2D menuBackground;
    Texture2D water;
    Texture2D islands[ISLAND_TEXTURE_COUNT];

    bool loaded;
} Assets;

bool AssetsLoad(Assets* assets);
void AssetsUnload(Assets* assets);
