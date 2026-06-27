#include "level.h"
#include "utils.h"
#include <string.h>

static void LevelRemoveActiveIslandAt(Level* l, size_t index) {
    if (!l || index >= l->activeIslands.count) return;
    for (size_t i = index + 1; i < l->activeIslands.count; ++i) {
        l->activeIslands.items[i - 1] = l->activeIslands.items[i];
    }
    l->activeIslands.count--;
}

static void LevelCompactIslands(Level* l) {
    if (!l) return;
    size_t i = 0;
    while (i < l->activeIslands.count) {
        Island* island = aiv_vector_at(&l->activeIslands, i);
        if (island == NULL || !island->active) LevelRemoveActiveIslandAt(l, i);
        else ++i;
    }
}

void LevelReset(Level* l) {
    if (!l) return;
    memset(l->islands, 0, sizeof(l->islands));
    if (l->activeIslands.items == NULL) l->activeIslands = aiv_vector_new();
    else l->activeIslands.count = 0;
    l->scrollY = 0.0f;
    l->scrollSpeed = 80.0f;
    l->spawnCooldown = 0.0f;
}

void LevelShutdown(Level* l) {
    if (!l) return;
    aiv_vector_destroy(&l->activeIslands);
    memset(l, 0, sizeof(*l));
}

static void IslandSpawn(Level* l, int screenW) {
    LevelCompactIslands(l);
    for (int i = 0; i < ISLAND_MAX; ++i) {
        Island* is = &l->islands[i];
        if (!is->active) {
            is->active = true;
            is->pos.x = RandF(40.0f, screenW - 40.0f);
            is->pos.y = -40.0f;
            is->r = RandF(18.0f, 42.0f);
            is->spriteIndex = RandI(0, 2);
            aiv_vector_add(&l->activeIslands, is);
            return;
        }
    }
}

void LevelUpdate(Level* l, float dt, int screenW, int screenH) {
    if (!l) return;

    l->scrollY += l->scrollSpeed * dt;

    l->spawnCooldown -= dt;
    if (l->spawnCooldown <= 0.0f) {
        IslandSpawn(l, screenW);
        l->spawnCooldown = RandF(0.35f, 0.9f);
    }

    size_t i = 0;
    while (i < l->activeIslands.count) {
        Island* is = aiv_vector_at(&l->activeIslands, i);
        if (is == NULL || !is->active) {
            LevelRemoveActiveIslandAt(l, i);
            continue;
        }
        is->pos.y += l->scrollSpeed * dt;
        if (is->pos.y > screenH + 60) {
            is->active = false;
            LevelRemoveActiveIslandAt(l, i);
            continue;
        }
        ++i;
    }
}

void LevelDraw(
    const Level* l,
    Texture2D waterTexture,
    const Texture2D* islandTextures,
    size_t islandTextureCount,
    int screenW,
    int screenH
) {
    DrawRectangle(0, 0, screenW, screenH, (Color){20, 80, 140, 255});

    int tileWidth = waterTexture.width > 0 ? waterTexture.width : 32;
    int tileHeight = waterTexture.height > 0 ? waterTexture.height : 32;
    int offsetY = l != NULL ? (int)l->scrollY % tileHeight : 0;
    for (int y = -tileHeight + offsetY; y < screenH; y += tileHeight) {
        for (int x = 0; x < screenW; x += tileWidth) {
            DrawTexture(waterTexture, x, y, WHITE);
        }
    }

    if (!l || !islandTextures || islandTextureCount == 0) return;

    for (size_t i = 0; i < l->activeIslands.count; ++i) {
        const Island* is = l->activeIslands.items[i];
        if (is == NULL || !is->active) continue;

        Texture2D texture = islandTextures[(size_t)is->spriteIndex % islandTextureCount];
        Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        float size = is->r * 2.0f;
        Rectangle destination = {is->pos.x, is->pos.y, size, size};
        Vector2 origin = {size * 0.5f, size * 0.5f};
        DrawTexturePro(texture, source, destination, origin, 0.0f, WHITE);
    }
}
