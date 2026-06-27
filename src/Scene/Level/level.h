#pragma once
#include "aiv_vector.h"
#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct Island {
    Vector2 pos;
    float r;
    int spriteIndex;
    bool active;
} Island;

#define ISLAND_MAX 24

typedef struct Level {
    float scrollY;
    float scrollSpeed;
    Island islands[ISLAND_MAX];
    aiv_vector_t activeIslands;
    float spawnCooldown;
} Level;

void LevelReset(Level* l);
void LevelShutdown(Level* l);
void LevelUpdate(Level* l, float dt, int screenW, int screenH);
void LevelDraw(
    const Level* l,
    Texture2D waterTexture,
    const Texture2D* islandTextures,
    size_t islandTextureCount,
    int screenW,
    int screenH
);
