/**
 * @file Game.cpp
 * @brief Implementation of the main Game class
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#include "Game.h"
#include <iostream>

Game::Game(int screenWidth, int screenHeight)
    : m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
    , m_currentState(GameState::MAIN_MENU)
    , m_previousState(GameState::MAIN_MENU)
    , m_difficulty(Difficulty::EASY)
    , m_gameStartTime(0.0f)
    , m_currentTime(0.0f)
    , m_pausedTime(0.0f)
    , m_selectedMenuItem(0)
    , m_selectedDifficulty(0)
    , m_totalMoves(0)
    , m_matchesFound(0)
    , m_gameWon(false)
{
    Utils::logInfo("Initializing Game...");
    loadResources();
}

Game::~Game() {
    Utils::logInfo("Cleaning up Game resources...");
    unloadResources();
}

void Game::update() {
    switch (m_currentState) {
        case GameState::MAIN_MENU:
            updateMainMenu();
            break;
        case GameState::DIFFICULTY:
            updateDifficultySelection();
            break;
        case GameState::PLAYING:
            updatePlaying();
            break;
        case GameState::PAUSED:
            updatePaused();
            break;
        case GameState::GAME_OVER:
            updateGameOver();
            break;
        case GameState::SETTINGS:
            updateSettings();
            break;
    }
}

void Game::draw() {
    switch (m_currentState) {
        case GameState::MAIN_MENU:
            drawMainMenu();
            break;
        case GameState::DIFFICULTY:
            drawDifficultySelection();
            break;
        case GameState::PLAYING:
            drawPlaying();
            break;
        case GameState::PAUSED:
            drawPaused();
            break;
        case GameState::GAME_OVER:
            drawGameOver();
            break;
        case GameState::SETTINGS:
            drawSettings();
            break;
    }
}

// === STATE UPDATE METHODS ===

void Game::updateMainMenu() {
    handleMainMenuInput();
}

void Game::updateDifficultySelection() {
    handleDifficultySelectionInput();
}

void Game::updatePlaying() {
    // Update current time for timer
    m_currentTime = GetTime();
    
    handlePlayingInput();
    
    // Update game board if it exists
    if (m_gameBoard) {
        // m_gameBoard->update(GetFrameTime());
    }
    
    checkWinCondition();
}

void Game::updatePaused() {
    handlePausedInput();
}

void Game::updateGameOver() {
    handleGameOverInput();
}

void Game::updateSettings() {
    handleSettingsInput();
}

// === STATE DRAW METHODS ===

void Game::drawMainMenu() {
    ClearBackground(DARKBLUE);
    
    // Draw title
    drawCenteredText("MEMORY CARD GAME", 100, 60, YELLOW, m_titleFont);
    drawCenteredText("Hacktoberfest 2025", 170, 30, LIGHTGRAY, m_uiFont);
    
    // Draw menu items
    float startY = m_screenHeight / 2.0f - (MAIN_MENU_ITEMS * (BUTTON_HEIGHT + BUTTON_SPACING)) / 2.0f;
    
    for (int i = 0; i < MAIN_MENU_ITEMS; i++) {
        Rectangle buttonBounds = {
            m_screenWidth / 2.0f - BUTTON_WIDTH / 2.0f,
            startY + i * (BUTTON_HEIGHT + BUTTON_SPACING),
            BUTTON_WIDTH,
            BUTTON_HEIGHT
        };
        
        Color buttonColor = (i == m_selectedMenuItem) ? SKYBLUE : LIGHTGRAY;
        drawButton(m_mainMenuItems[i], buttonBounds, i == m_selectedMenuItem, buttonColor);
    }
    
    // Draw footer
    drawCenteredText("Use Arrow Keys and Enter to navigate", m_screenHeight - 50, 20, GRAY, m_uiFont);
}

void Game::drawDifficultySelection() {
    ClearBackground(DARKBLUE);
    
    // Draw title
    drawCenteredText("SELECT DIFFICULTY", 100, 50, YELLOW, m_titleFont);
    
    // Draw difficulty options
    float startY = m_screenHeight / 2.0f - (DIFFICULTY_OPTIONS * (BUTTON_HEIGHT + BUTTON_SPACING)) / 2.0f;
    
    for (int i = 0; i < DIFFICULTY_OPTIONS; i++) {
        Rectangle buttonBounds = {
            m_screenWidth / 2.0f - BUTTON_WIDTH / 2.0f,
            startY + i * (BUTTON_HEIGHT + BUTTON_SPACING),
            BUTTON_WIDTH,
            BUTTON_HEIGHT
        };
        
        Color buttonColor = (i == m_selectedDifficulty) ? SKYBLUE : LIGHTGRAY;
        drawButton(m_difficultyNames[i], buttonBounds, i == m_selectedDifficulty, buttonColor);
    }
    
    // Draw footer
    drawCenteredText("Press ESC to go back", m_screenHeight - 50, 20, GRAY, m_uiFont);
}

void Game::drawPlaying() {
    ClearBackground(DARKGREEN);
    
    // Draw game board
    if (m_gameBoard) {
        // m_gameBoard->draw();
    }
    
    // Draw game UI elements
    drawGameStats();
    drawTimer();
    
    // Draw instruction text
    const char* instructions = "Click cards to flip them | ESC to pause";
    int textWidth = MeasureText(instructions, 20);
    DrawText(instructions, m_screenWidth / 2 - textWidth / 2, m_screenHeight - 40, 20, WHITE);
}

void Game::drawPaused() {
    // Draw the game state behind the pause overlay
    drawPlaying();
    
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, m_screenWidth, m_screenHeight, Color{0, 0, 0, 150});
    
    // Draw pause text
    drawCenteredText("PAUSED", m_screenHeight / 2 - 50, 60, YELLOW, m_titleFont);
    
    // Draw resume instruction
    drawCenteredText("Press ESC to resume", m_screenHeight / 2 + 30, 30, WHITE, m_uiFont);
    drawCenteredText("Press R to restart", m_screenHeight / 2 + 70, 30, WHITE, m_uiFont);
    drawCenteredText("Press Q to quit to menu", m_screenHeight / 2 + 110, 30, WHITE, m_uiFont);
}

void Game::drawGameOver() {
    ClearBackground(DARKBLUE);
    
    // Draw congratulations message
    drawCenteredText("CONGRATULATIONS!", 150, 60, GOLD, m_titleFont);
    drawCenteredText("You Won!", 220, 40, YELLOW, m_uiFont);
    
    // Draw game statistics
    float centerX = m_screenWidth / 2.0f;
    float startY = 300;
    float lineSpacing = 50;
    
    std::string timeStr = "Time: " + Utils::formatTime(getElapsedTime());
    std::string movesStr = "Total Moves: " + Utils::toString(m_totalMoves);
    std::string matchesStr = "Matches Found: " + Utils::toString(m_matchesFound);
    
    drawCenteredText(timeStr, startY, 35, WHITE, m_uiFont);
    drawCenteredText(movesStr, startY + lineSpacing, 35, WHITE, m_uiFont);
    drawCenteredText(matchesStr, startY + lineSpacing * 2, 35, WHITE, m_uiFont);
    
    // Draw options
    drawCenteredText("Press R to play again", m_screenHeight - 100, 25, LIGHTGRAY, m_uiFont);
    drawCenteredText("Press Q to return to menu", m_screenHeight - 60, 25, LIGHTGRAY, m_uiFont);
}

void Game::drawSettings() {
    ClearBackground(DARKBLUE);
    
    drawCenteredText("SETTINGS", 100, 50, YELLOW, m_titleFont);
    drawCenteredText("Coming Soon!", m_screenHeight / 2, 40, WHITE, m_uiFont);
    drawCenteredText("Press ESC to go back", m_screenHeight - 50, 20, GRAY, m_uiFont);
}

// === TIMER METHODS ===

void Game::drawTimer() {
    float elapsedTime = getElapsedTime();
    std::string timeStr = "Time: " + Utils::formatTime(elapsedTime);
    
    // Draw timer at top right
    int fontSize = 30;
    int textWidth = MeasureText(timeStr.c_str(), fontSize);
    int padding = 20;
    
    // Draw background rectangle
    DrawRectangle(m_screenWidth - textWidth - padding * 2, padding - 5, 
                  textWidth + padding * 2, fontSize + 10, Color{0, 0, 0, 150});
    
    // Draw timer text
    DrawText(timeStr.c_str(), m_screenWidth - textWidth - padding, padding, fontSize, YELLOW);
}

void Game::drawGameStats() {
    int fontSize = 25;
    int padding = 20;
    int yOffset = 20;
    
    // Draw moves counter
    std::string movesStr = "Moves: " + Utils::toString(m_totalMoves);
    DrawText(movesStr.c_str(), padding, yOffset, fontSize, WHITE);
    
    // Draw matches counter
    std::string matchesStr = "Matches: " + Utils::toString(m_matchesFound);
    DrawText(matchesStr.c_str(), padding, yOffset + fontSize + 10, fontSize, WHITE);
}

float Game::getElapsedTime() const {
    if (m_gameStartTime == 0.0f) {
        return 0.0f;
    }
    
    // If game is paused, return the time at pause
    if (m_currentState == GameState::PAUSED) {
        return m_pausedTime - m_gameStartTime;
    }
    
    // If game is over, use current time (which was set when game ended)
    if (m_currentState == GameState::GAME_OVER) {
        return m_currentTime - m_gameStartTime;
    }
    
    // Otherwise return current elapsed time
    return m_currentTime - m_gameStartTime;
}

// === STATE TRANSITION METHODS ===

void Game::changeState(GameState newState) {
    m_previousState = m_currentState;
    m_currentState = newState;
    Utils::logInfo("State changed to: " + Utils::toString(static_cast<int>(newState)));
}

void Game::startNewGame(Difficulty difficulty) {
    m_difficulty = difficulty;
    m_totalMoves = 0;
    m_matchesFound = 0;
    m_gameWon = false;
    
    // Initialize timer
    m_gameStartTime = GetTime();
    m_currentTime = m_gameStartTime;
    m_pausedTime = 0.0f;
    
    Utils::logInfo("Starting new game with difficulty: " + Utils::toString(static_cast<int>(difficulty)));
    
    // TODO: Initialize game board
    // m_gameBoard = std::make_unique<GameBoard>(difficulty);
    
    changeState(GameState::PLAYING);
}

void Game::pauseGame() {
    if (m_currentState == GameState::PLAYING) {
        m_pausedTime = GetTime();
        changeState(GameState::PAUSED);
        Utils::logInfo("Game paused");
    }
}

void Game::resumeGame() {
    if (m_currentState == GameState::PAUSED) {
        // Adjust game start time to account for paused duration
        float pauseDuration = GetTime() - m_pausedTime;
        m_gameStartTime += pauseDuration;
        
        changeState(GameState::PLAYING);
        Utils::logInfo("Game resumed");
    }
}

void Game::restartGame() {
    startNewGame(m_difficulty);
}

void Game::returnToMainMenu() {
    changeState(GameState::MAIN_MENU);
}

// === INPUT HANDLING ===

void Game::handleMainMenuInput() {
    // Navigate menu with arrow keys
    if (IsKeyPressed(KEY_DOWN)) {
        m_selectedMenuItem = (m_selectedMenuItem + 1) % MAIN_MENU_ITEMS;
    }
    if (IsKeyPressed(KEY_UP)) {
        m_selectedMenuItem = (m_selectedMenuItem - 1 + MAIN_MENU_ITEMS) % MAIN_MENU_ITEMS;
    }
    
    // Select with Enter
    if (IsKeyPressed(KEY_ENTER)) {
        switch (m_selectedMenuItem) {
            case 0: // Start Game
                changeState(GameState::DIFFICULTY);
                break;
            case 1: // Settings
                changeState(GameState::SETTINGS);
                break;
            case 2: // High Scores
                Utils::logInfo("High Scores not implemented yet");
                break;
            case 3: // Exit
                // Window will close
                break;
        }
    }
}

void Game::handleDifficultySelectionInput() {
    // Navigate with arrow keys
    if (IsKeyPressed(KEY_DOWN)) {
        m_selectedDifficulty = (m_selectedDifficulty + 1) % DIFFICULTY_OPTIONS;
    }
    if (IsKeyPressed(KEY_UP)) {
        m_selectedDifficulty = (m_selectedDifficulty - 1 + DIFFICULTY_OPTIONS) % DIFFICULTY_OPTIONS;
    }
    
    // Select difficulty with Enter
    if (IsKeyPressed(KEY_ENTER)) {
        Difficulty selectedDiff;
        switch (m_selectedDifficulty) {
            case 0:
                selectedDiff = Difficulty::EASY;
                break;
            case 1:
                selectedDiff = Difficulty::MEDIUM;
                break;
            case 2:
                selectedDiff = Difficulty::HARD;
                break;
            default:
                selectedDiff = Difficulty::EASY;
        }
        startNewGame(selectedDiff);
    }
    
    // Go back with ESC
    if (IsKeyPressed(KEY_ESCAPE)) {
        changeState(GameState::MAIN_MENU);
    }
}

void Game::handlePlayingInput() {
    // Pause with ESC
    if (IsKeyPressed(KEY_ESCAPE)) {
        pauseGame();
    }
    
    // TODO: Handle card clicking
    // if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    //     Vector2 mousePos = GetMousePosition();
    //     m_gameBoard->handleClick(mousePos);
    //     m_totalMoves++;
    // }
}

void Game::handlePausedInput() {
    // Resume with ESC
    if (IsKeyPressed(KEY_ESCAPE)) {
        resumeGame();
    }
    
    // Restart with R
    if (IsKeyPressed(KEY_R)) {
        restartGame();
    }
    
    // Quit to menu with Q
    if (IsKeyPressed(KEY_Q)) {
        returnToMainMenu();
    }
}

void Game::handleGameOverInput() {
    // Play again with R
    if (IsKeyPressed(KEY_R)) {
        restartGame();
    }
    
    // Return to menu with Q
    if (IsKeyPressed(KEY_Q)) {
        returnToMainMenu();
    }
}

void Game::handleSettingsInput() {
    // Go back with ESC
    if (IsKeyPressed(KEY_ESCAPE)) {
        changeState(GameState::MAIN_MENU);
    }
}

// === UI HELPER METHODS ===

void Game::drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color) {
    // Draw button background
    Color bgColor = isSelected ? color : Color{color.r / 2, color.g / 2, color.b / 2, 255};
    DrawRectangleRec(bounds, bgColor);
    
    // Draw button border
    Color borderColor = isSelected ? YELLOW : GRAY;
    DrawRectangleLinesEx(bounds, 3, borderColor);
    
    // Draw button text
    int fontSize = 30;
    int textWidth = MeasureText(text.c_str(), fontSize);
    int textX = bounds.x + (bounds.width - textWidth) / 2;
    int textY = bounds.y + (bounds.height - fontSize) / 2;
    
    DrawText(text.c_str(), textX, textY, fontSize, BLACK);
}

void Game::drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font) {
    int textWidth = MeasureText(text.c_str(), fontSize);
    int x = m_screenWidth / 2 - textWidth / 2;
    DrawText(text.c_str(), x, y, fontSize, color);
}

// === UTILITY METHODS ===

void Game::loadResources() {
    Utils::logInfo("Loading game resources...");
    
    // Load fonts (using default fonts for now)
    m_titleFont = GetFontDefault();
    m_uiFont = GetFontDefault();
    
    // TODO: Load textures, sounds, etc.
    // m_backgroundTexture = LoadTexture("assets/textures/background.png");
    
    Utils::logInfo("Resources loaded successfully!");
}

void Game::unloadResources() {
    // TODO: Unload textures, sounds, etc.
    // UnloadTexture(m_backgroundTexture);
}

void Game::checkWinCondition() {
    // TODO: Check if all cards are matched
    // if (m_gameBoard && m_gameBoard->allCardsMatched()) {
    //     m_gameWon = true;
    //     changeState(GameState::GAME_OVER);
    //     Utils::logInfo("Game won! Time: " + Utils::formatTime(getElapsedTime()));
    // }
}
