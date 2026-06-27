#include "input.h"

Vector2 InputMoveDirection(void) {
    Vector2 direction = {0};

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) direction.x += 1.0f;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) direction.y -= 1.0f;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) direction.y += 1.0f;

    return direction;
}

bool InputShootDown(void) {
    return IsKeyDown(KEY_SPACE);
}

bool InputMenuUpPressed(void) {
    return IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
}

bool InputMenuDownPressed(void) {
    return IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
}

bool InputConfirmPressed(void) {
    return IsKeyPressed(KEY_ENTER);
}

bool InputWindowCloseRequested(void) {
    return WindowShouldClose();
}
