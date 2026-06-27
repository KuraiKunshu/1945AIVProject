#include "raylib.h"
#include "game.h"
#include "input.h"
#include <time.h>
#include <stdlib.h>



int main(void) {
    const int screenW = 640;
    const int screenH = 480;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenW, screenH, TITLE);
    SetTargetFPS(60);

    ChangeDirectory(GetApplicationDirectory());

    srand((unsigned int)time(NULL));

    Game game;
    GameInit(&game, screenW, screenH);

    while (!InputWindowCloseRequested() && !game.exitRequested) {
        float dt = GetFrameTime();
        GameUpdate(&game, dt);
        if (game.exitRequested) break;
        GameDraw(&game);
    }

    GameShutdown(&game);

    CloseWindow();
    return 0;
}
