/**
 * Memory Card Flip Game
 * 
 * A classic memory card matching game built with C++ and raylib.
 * This project is part of Hacktoberfest 2025 by MSTC DA-IICT.
 * 
 * @file main.cpp
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 * 
 * @copyright Copyright (c) 2025 MSTC DA-IICT. All rights reserved.
 * @license MIT License
 */

#include <raylib.h>
#include <iostream>
#include <memory>

#include "Game.h"
#include "Utils.h"

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;
constexpr int TARGET_FPS = 60;
constexpr const char* WINDOW_TITLE = "Memory Card Flip Game - MSTC DA-IICT";

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    // Disable default ESC behavior (raylib closes window on ESC by default)
    // We'll handle ESC manually in the game's state handlers so settings can
    // return to the main menu instead of exiting the application.
    SetExitKey(KEY_NULL);
    SetTargetFPS(TARGET_FPS);
    
    InitAudioDevice();
    
    if (!IsAudioDeviceReady()) {
        Utils::logError("Failed to initialize audio device!");
    }
    
    // Set window icon (if available)
    // Image icon = LoadImage("assets/textures/icon.png");
    // SetWindowIcon(icon);
    // UnloadImage(icon);
    
    try {
        // Create the game instance
        auto game = std::make_unique<Game>(SCREEN_WIDTH, SCREEN_HEIGHT);
        
        Utils::logInfo("Memory Card Game initialized successfully!");
        Utils::logInfo("Screen resolution: " + std::to_string(SCREEN_WIDTH) + "x" + std::to_string(SCREEN_HEIGHT));
        Utils::logInfo("Target FPS: " + std::to_string(TARGET_FPS));
        
        // Main game loop
        while (!WindowShouldClose()) {
            // Update
            game->update();
            
            // Draw
            BeginDrawing();
            {
                ClearBackground(DARKBLUE);
                game->draw();
                
                // Draw FPS counter in debug mode
                #ifdef DEBUG
                DrawFPS(10, 10);
                #endif
            }
            EndDrawing();
        }
        
        Utils::logInfo("Game loop ended normally.");
        
    } catch (const std::exception& e) {
        Utils::logError("Game error: " + std::string(e.what()));
        
        // Show error message to user
        BeginDrawing();
        ClearBackground(RED);
        DrawText("GAME ERROR - Check console for details", 
                 SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2, 20, WHITE);
        EndDrawing();
        
        // Wait for user to close window
        while (!WindowShouldClose()) {
            // Just wait for window close
        }
        
        CloseAudioDevice();
        CloseWindow();
        return EXIT_FAILURE;
    }
    
    // Cleanup
    Utils::logInfo("Cleaning up resources...");
    CloseAudioDevice();
    CloseWindow();
    
    Utils::logInfo("Thanks for playing Memory Card Game!");
    Utils::logInfo("Made with ❤️ by MSTC DA-IICT for Hacktoberfest 2025");
    
    return EXIT_SUCCESS;
}