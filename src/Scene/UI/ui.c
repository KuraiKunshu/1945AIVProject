#include "ui.h"
#include "input.h"
#include "utils.h"
#include <string.h>

void UiInit(UiState* ui) {
    if (!ui) return;
    memset(ui, 0, sizeof(*ui));
    ui->showSplash = true;
    ui->showGameOver = false;
    ui->menuIndex = 0;
}

void UiUpdate(UiState* ui) {
    if (!ui) return;
    if (!ui->showSplash) return;
    if (InputMenuUpPressed()) ui->menuIndex = 0;
    if (InputMenuDownPressed()) ui->menuIndex = 1;
}

void UiDrawHud(const UiState* ui, const Player* p) {
    (void)ui;
    if (!p) return;

    DrawText(TextFormat("LIVES: %d", p->lives), 10, 8, 16, WHITE);
    DrawText(TextFormat("SCORE: %d", p->score), 10, 28, 16, WHITE);

    int barX = 10, barY = 50, barW = 150, barH = 10;
    DrawRectangle(barX, barY, barW, barH, (Color){255,255,255,40});
    int fill = (int)(barW * ClampF(p->energy/100.0f, 0.0f, 1.0f));
    DrawRectangle(barX, barY, fill, barH, GREEN);
    DrawRectangleLines(barX, barY, barW, barH, WHITE);
    DrawText("ENERGY", barX + barW + 8, barY - 3, 14, WHITE);
}

void UiDrawSplash(const UiState* ui) {
    if (!ui) return;

    const char* title = TITLE;
    int tw = MeasureText(title, 28);
    DrawText(title, 280 - tw/2, 10, 28, GOLD);

    Color c0 = (ui->menuIndex == 0) ? MAROON : GOLD;
    Color c1 = (ui->menuIndex == 1) ? MAROON : GOLD;
    DrawText("PLAY", 240, 240, 20, c0);
    DrawText("QUIT", 240, 280, 20, c1);
    DrawText("Press ENTER to confirm", 170, 320, 16, GOLD);

    DrawText("Controls: WASD/Arrows move, SPACE shoot", 280, 450, 16, GOLD);
}

void UiDrawGameOver(const UiState* ui, int score) {
    (void)ui;
    const char* t = "GAME OVER";
    int tw = MeasureText(t, 36);
    DrawText(t, 320 - tw/2, 150, 36, RED);
    DrawText(TextFormat("Final Score: %d", score), 240, 210, 20, WHITE);
    DrawText("Press ENTER to return to menu", 170, 270, 18, WHITE);
}
