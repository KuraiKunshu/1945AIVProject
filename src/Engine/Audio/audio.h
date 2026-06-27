#pragma once
#include <stdbool.h>
#include "raylib.h"

typedef struct AudioSys {
    bool ok;
    Music music;
    Sound sfxShoot;
    Sound sfxExplosions[2];
    bool hasMusic;
    bool hasShoot;
    int explosionCount;
} AudioSys;

void AudioInit(AudioSys* a);
void AudioUpdate(AudioSys* a);
void AudioShutdown(AudioSys* a);

void AudioPlayShoot(AudioSys* a);
void AudioPlayExplosion(AudioSys* a);
