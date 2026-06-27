#pragma once
#include "raylib.h"
#include "player.h"

#define TITLE "2026 - Kurai Sanzogni (1945)"

typedef struct UiState {
    bool showSplash;
    bool showGameOver;
    int menuIndex; // 0=Play, 1=Quit
} UiState;

void UiInit(UiState* ui);
void UiUpdate(UiState* ui);
void UiDrawHud(const UiState* ui, const Player* p);
void UiDrawSplash(const UiState* ui);
void UiDrawGameOver(const UiState* ui, int score);
