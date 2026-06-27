#include "audio.h"

#include <math.h>
#include <string.h>

#define SHOOT_SAMPLE_RATE 22050
#define SHOOT_DURATION_SECONDS 0.09f

static Sound MakeShootSound(void) {
    unsigned int frameCount = (unsigned int)(
        SHOOT_SAMPLE_RATE * SHOOT_DURATION_SECONDS
    );
    short* samples = MemAlloc(frameCount * sizeof(short));
    if (samples == NULL) return (Sound){0};

    for (unsigned int i = 0; i < frameCount; ++i) {
        float progress = (float)i / (float)frameCount;
        float time = (float)i / (float)SHOOT_SAMPLE_RATE;
        float frequency = 920.0f - 420.0f * progress;
        float envelope = 1.0f - progress;
        samples[i] = (short)(sinf(2.0f * PI * frequency * time) * envelope * 9000.0f);
    }

    Wave wave = {
        frameCount,
        SHOOT_SAMPLE_RATE,
        16,
        1,
        samples
    };
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

void AudioInit(AudioSys* a) {
    if (!a) return;
    memset(a, 0, sizeof(*a));

    InitAudioDevice();
    a->ok = IsAudioDeviceReady();
    if (!a->ok) return;

    if (FileExists("assets/audio/background.mp3")) {
        a->music = LoadMusicStream("assets/audio/background.mp3");
        a->hasMusic = IsMusicValid(a->music);
        a->music.looping = true;
    }
    if (a->hasMusic) {
        PlayMusicStream(a->music);
        SetMusicVolume(a->music, 0.35f);
    }

    a->sfxShoot = MakeShootSound();
    a->hasShoot = IsSoundValid(a->sfxShoot);
    if (a->hasShoot) {
        SetSoundVolume(a->sfxShoot, 0.5f);
    }

    const char* explosionPaths[2] = {
        "assets/audio/snd_explosion1.wav",
        "assets/audio/snd_explosion2.wav"
    };
    for (int i = 0; i < 2; ++i) {
        if (!FileExists(explosionPaths[i])) continue;
        Sound sound = LoadSound(explosionPaths[i]);
        if (!IsSoundValid(sound)) continue;
        SetSoundVolume(sound, 0.6f);
        a->sfxExplosions[a->explosionCount++] = sound;
    }
}

void AudioUpdate(AudioSys* a) {
    if (!a || !a->ok) return;
    if (a->hasMusic) UpdateMusicStream(a->music);
}

void AudioShutdown(AudioSys* a) {
    if (!a) return;
    if (a->ok) {
        if (a->hasMusic) {
            StopMusicStream(a->music);
            UnloadMusicStream(a->music);
        }
        if (a->hasShoot) UnloadSound(a->sfxShoot);
        for (int i = 0; i < a->explosionCount; ++i) {
            UnloadSound(a->sfxExplosions[i]);
        }
        CloseAudioDevice();
    }
    memset(a, 0, sizeof(*a));
}

void AudioPlayShoot(AudioSys* a) {
    if (!a || !a->ok || !a->hasShoot) return;
    PlaySound(a->sfxShoot);
}

void AudioPlayExplosion(AudioSys* a) {
    if (!a || !a->ok || a->explosionCount <= 0) return;
    int index = GetRandomValue(0, a->explosionCount - 1);
    PlaySound(a->sfxExplosions[index]);
}
