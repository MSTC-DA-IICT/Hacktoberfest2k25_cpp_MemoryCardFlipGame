git /**
 * @file main.cpp
 * @brief Entry point for the single player Memory Card Flip Game.
 */

#include <cstdlib>
#include <exception>
#include <memory>
#include <string>

#include <raylib.h>

#include "../include/Game.h"
#include "../include/Utils.h"

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;
constexpr int TARGET_FPS = 60;
constexpr const char* WINDOW_TITLE = "Memory Card Flip Game";

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(TARGET_FPS);

    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        Utils::logError("Audio device failed to initialise. Continuing without sound.");
    }

    try {
        auto game = std::make_unique<Game>(SCREEN_WIDTH, SCREEN_HEIGHT);
        Utils::logInfo("Memory Card Game initialised.");

        while (!WindowShouldClose()) {
            game->update();

            BeginDrawing();
            ClearBackground(DARKBLUE);
            game->draw();
#ifdef DEBUG
            DrawFPS(10, 10);
#endif
            EndDrawing();
        }

        Utils::logInfo("Game loop exited cleanly.");

    } catch (const std::exception& ex) {
        Utils::logError(std::string("Uncaught exception: ") + ex.what());

        BeginDrawing();
        ClearBackground(RED);
        DrawText("An unexpected error occurred. Check console for details.",
                 SCREEN_WIDTH / 2 - 260, SCREEN_HEIGHT / 2, 20, WHITE);
        EndDrawing();

        while (!WindowShouldClose()) {
            // Allow the user to read the error message before exiting.
        }

        CloseAudioDevice();
        CloseWindow();
        return EXIT_FAILURE;
    }

    CloseAudioDevice();
    CloseWindow();
    return EXIT_SUCCESS;
}
