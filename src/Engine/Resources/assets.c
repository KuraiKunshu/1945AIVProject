#include "assets.h"

#include <string.h>

static Texture2D MakeFallbackTexture(Color color, int width, int height)
{
    Image image = GenImageColor(width, height, color);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

static Texture2D LoadTextureOrFallback(
    const char* path,
    Color fallbackColor,
    int fallbackWidth,
    int fallbackHeight
)
{
    if (FileExists(path)) {
        Texture2D texture = LoadTexture(path);
        if (texture.id != 0) return texture;
    }
    return MakeFallbackTexture(fallbackColor, fallbackWidth, fallbackHeight);
}

static SpriteSheet LoadSheetOrFallback(
    const char* path,
    int frameCount,
    Color fallbackColor,
    int fallbackWidth,
    int fallbackHeight
)
{
    SpriteSheet sheet = {0};
    if (FileExists(path)) {
        sheet.texture = LoadTexture(path);
        if (sheet.texture.id != 0) {
            sheet.frameCount = frameCount;
            return sheet;
        }
    }

    sheet.texture = MakeFallbackTexture(
        fallbackColor,
        fallbackWidth,
        fallbackHeight
    );
    sheet.frameCount = 1;
    return sheet;
}

static void AssetsUnloadTexture(Texture2D* texture)
{
    if (texture != NULL && texture->id != 0) {
        UnloadTexture(*texture);
        *texture = (Texture2D){0};
    }
}

static void AssetsUnloadSheet(SpriteSheet* sheet)
{
    if (sheet == NULL) return;
    AssetsUnloadTexture(&sheet->texture);
    sheet->frameCount = 0;
}

bool AssetsLoad(Assets* assets)
{
    if (assets == NULL) return false;
    memset(assets, 0, sizeof(*assets));

    assets->player = LoadSheetOrFallback(
        "assets/player/myplane_strip3.png", 3, BLUE, 65, 65
    );

    const char* enemyPaths[ENEMY_SPRITE_COUNT] = {
        "assets/enemy/enemy1_strip3.png",
        "assets/enemy/enemy2_strip3.png",
        "assets/enemy/enemy3_strip3.png",
        "assets/enemy/enemy4_strip3.png"
    };
    const Color enemyColors[ENEMY_SPRITE_COUNT] = {RED, ORANGE, LIME, SKYBLUE};
    for (int i = 0; i < ENEMY_SPRITE_COUNT; ++i) {
        assets->enemies[i] = LoadSheetOrFallback(
            enemyPaths[i], 3, enemyColors[i], 32, 32
        );
    }

    assets->enemyExplosion = LoadSheetOrFallback(
        "assets/enemy/explosion1_strip6.png", 6, ORANGE, 32, 32
    );
    assets->playerExplosion = LoadSheetOrFallback(
        "assets/player/explosion2_strip7.png", 7, YELLOW, 65, 65
    );

    assets->playerBullet = LoadSheetOrFallback(
        "assets/player/player_projectile.png", 10, YELLOW, 48, 32
    );
    assets->playerBulletHit = LoadSheetOrFallback(
        "assets/player/Ice VFX 1 Hit.png", 12, SKYBLUE, 32, 32
    );
    assets->enemyBullet = LoadTextureOrFallback(
        "assets/enemy/enemybullet1.png", RED, 10, 10
    );
    assets->menuBackground = LoadTextureOrFallback(
        "assets/map/Title.png", BLACK, 640, 480
    );
    assets->water = LoadTextureOrFallback(
        "assets/map/water.png", (Color){20, 80, 140, 255}, 32, 32
    );

    const char* islandPaths[ISLAND_TEXTURE_COUNT] = {
        "assets/map/island1.png",
        "assets/map/island2.png",
        "assets/map/island3.png"
    };
    for (int i = 0; i < ISLAND_TEXTURE_COUNT; ++i) {
        assets->islands[i] = LoadTextureOrFallback(
            islandPaths[i], DARKGREEN, 48, 48
        );
    }

    assets->loaded = true;
    return true;
}

void AssetsUnload(Assets* assets)
{
    if (assets == NULL) return;

    AssetsUnloadSheet(&assets->player);
    for (int i = 0; i < ENEMY_SPRITE_COUNT; ++i) {
        AssetsUnloadSheet(&assets->enemies[i]);
    }
    AssetsUnloadSheet(&assets->enemyExplosion);
    AssetsUnloadSheet(&assets->playerExplosion);
    AssetsUnloadSheet(&assets->playerBullet);
    AssetsUnloadSheet(&assets->playerBulletHit);

    AssetsUnloadTexture(&assets->enemyBullet);
    AssetsUnloadTexture(&assets->menuBackground);
    AssetsUnloadTexture(&assets->water);
    for (int i = 0; i < ISLAND_TEXTURE_COUNT; ++i) {
        AssetsUnloadTexture(&assets->islands[i]);
    }

    memset(assets, 0, sizeof(*assets));
}
