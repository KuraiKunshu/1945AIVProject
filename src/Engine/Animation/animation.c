#include "animation.h"

void AnimationInit(SpriteAnimation* animation, float framesPerSecond, bool loop)
{
    if (animation == NULL) return;

    animation->currentFrame = 0;
    animation->frameDuration = framesPerSecond > 0.0f
        ? 1.0f / framesPerSecond
        : 0.0f;
    animation->elapsed = 0.0f;
    animation->loop = loop;
    animation->finished = false;
}

void AnimationReset(SpriteAnimation* animation)
{
    if (animation == NULL) return;
    animation->currentFrame = 0;
    animation->elapsed = 0.0f;
    animation->finished = false;
}

void AnimationUpdate(SpriteAnimation* animation, int frameCount, float deltaTime)
{
    if (animation == NULL || animation->finished || frameCount <= 1 ||
        animation->frameDuration <= 0.0f) return;

    animation->elapsed += deltaTime;
    while (animation->elapsed >= animation->frameDuration) {
        animation->elapsed -= animation->frameDuration;
        animation->currentFrame++;

        if (animation->currentFrame < frameCount) continue;

        if (animation->loop) animation->currentFrame = 0;
        else {
            animation->currentFrame = frameCount - 1;
            animation->finished = true;
            animation->elapsed = 0.0f;
            break;
        }
    }
}

Rectangle AnimationSource(const SpriteSheet* sheet, int frame)
{
    if (sheet == NULL || sheet->frameCount <= 0) return (Rectangle){0};

    int safeFrame = frame;
    if (safeFrame < 0) safeFrame = 0;
    if (safeFrame >= sheet->frameCount) safeFrame = sheet->frameCount - 1;
    float frameWidth = (float)sheet->texture.width / (float)sheet->frameCount;
    return (Rectangle){
        frameWidth * (float)safeFrame,
        0.0f,
        frameWidth,
        (float)sheet->texture.height
    };
}

void AnimationDraw(
    const SpriteSheet* sheet,
    const SpriteAnimation* animation,
    Rectangle destination,
    float rotation,
    Color tint
)
{
    if (sheet == NULL || animation == NULL || sheet->texture.id == 0) return;

    Rectangle source = AnimationSource(sheet, animation->currentFrame);
    Vector2 origin = {destination.width * 0.5f, destination.height * 0.5f};
    DrawTexturePro(sheet->texture, source, destination, origin, rotation, tint);
}
