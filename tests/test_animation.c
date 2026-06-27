#define CLOVE_SUITE_NAME AnimationSuite
#include "clove-unit.h"

#include "animation.h"
#include "explosion.h"

CLOVE_TEST(LoopingAnimationWrapsToFirstFrame)
{
    SpriteAnimation animation;
    AnimationInit(&animation, 10.0f, true);

    AnimationUpdate(&animation, 3, 0.31f);

    CLOVE_INT_EQ(0, animation.currentFrame);
    CLOVE_IS_FALSE(animation.finished);
}

CLOVE_TEST(OneShotAnimationStopsOnLastFrame)
{
    SpriteAnimation animation;
    AnimationInit(&animation, 10.0f, false);

    AnimationUpdate(&animation, 3, 1.0f);

    CLOVE_INT_EQ(2, animation.currentFrame);
    CLOVE_IS_TRUE(animation.finished);
}

CLOVE_TEST(SpriteSheetSourceSelectsCorrectFrame)
{
    SpriteSheet sheet = {{0}};
    sheet.texture.width = 192;
    sheet.texture.height = 32;
    sheet.frameCount = 6;

    Rectangle source = AnimationSource(&sheet, 4);

    CLOVE_FLOAT_EQ(128.0f, source.x);
    CLOVE_FLOAT_EQ(32.0f, source.width);
    CLOVE_FLOAT_EQ(32.0f, source.height);
}

CLOVE_TEST(ExplosionPoolRecyclesFinishedExplosion)
{
    ExplosionPool pool = {0};
    SpriteSheet enemySheet = {{0}};
    SpriteSheet playerSheet = {{0}};
    SpriteSheet playerBulletHitSheet = {{0}};
    enemySheet.frameCount = 6;
    playerSheet.frameCount = 7;
    playerBulletHitSheet.frameCount = 12;

    ExplosionsReset(&pool);
    CLOVE_IS_TRUE(ExplosionsSpawn(
        &pool,
        EXPLOSION_ENEMY,
        (Vector2){10.0f, 20.0f},
        32.0f
    ));
    CLOVE_IS_TRUE(pool.items[0].active);

    ExplosionsUpdate(&pool, &enemySheet, &playerSheet, &playerBulletHitSheet, 1.0f);
    CLOVE_IS_FALSE(pool.items[0].active);
    CLOVE_SIZET_EQ(0, pool.active.count);

    ExplosionsShutdown(&pool);
}
