/**
 * @file Game.cpp
 * @brief Main Game class implementation with enhanced UI
 */

#include "../include/Game.h"
#include "../include/Utils.h"
#include <cmath>
#include <iostream>

// --------------------- Constructor & Destructor ---------------------

Game::Game(int screenWidth, int screenHeight)
    : m_screenWidth(screenWidth),
      m_screenHeight(screenHeight),
      m_currentState(GameState::MAIN_MENU),
      m_previousState(GameState::MAIN_MENU),
      m_difficulty(Difficulty::EASY),
      m_selectedMenuItem(0),
      m_selectedDifficulty(0),
      m_totalMoves(0),
      m_matchesFound(0),
    m_gameWon(false),
    m_soundEnabled(true),
      m_gameBoard(nullptr),
      m_audioManager(nullptr),
      m_scoreManager(nullptr),
      m_gameStartTime(0.0f),
      m_currentTime(0.0f),
      m_pausedTime(0.0f)
{
    loadResources();
    
    // Test audio device
    if (IsAudioDeviceReady()) {
        Utils::logInfo("Audio device is ready and working!");
    } else {
        Utils::logWarning("Audio device not ready!");
    }
}

Game::~Game() {
    unloadResources();
}

// --------------------- Resource Management ---------------------

void Game::loadResources() {
    // Load fonts (use default if files don't exist)
    if (FileExists("assets/fonts/arial.ttf")) {
        m_titleFont = LoadFont("assets/fonts/arial.ttf");
        m_uiFont = LoadFont("assets/fonts/arial.ttf");
    } else {
        m_titleFont = GetFontDefault();
        m_uiFont = GetFontDefault();
    }

    // Load background (optional)
    if (FileExists("assets/textures/background.png")) {
        m_backgroundTexture = LoadTexture("assets/textures/background.png");
    }

    Utils::logInfo("Resources loaded successfully.");
}

void Game::unloadResources() {
    if (m_titleFont.texture.id != GetFontDefault().texture.id) {
        UnloadFont(m_titleFont);
    }
    if (m_uiFont.texture.id != GetFontDefault().texture.id) {
        UnloadFont(m_uiFont);
    }
    if (m_backgroundTexture.id != 0) {
        UnloadTexture(m_backgroundTexture);
    }

    Utils::logInfo("Resources unloaded successfully.");
}

// --------------------- Game State Updates ---------------------

void Game::update() {
    switch (m_currentState) {
        case GameState::MAIN_MENU: updateMainMenu(); break;
        case GameState::DIFFICULTY: updateDifficultySelection(); break;
        case GameState::PLAYING: updatePlaying(); break;
        case GameState::PAUSED: updatePaused(); break;
        case GameState::GAME_OVER: updateGameOver(); break;
        case GameState::SETTINGS: updateSettings(); break;
        case GameState::HIGH_SCORES: handleHighScoresInput(); break;
    }
}

void Game::updateMainMenu() {
    handleMainMenuInput();
}

void Game::updateDifficultySelection() {
    handleDifficultySelectionInput();
}

void Game::updatePlaying() {
    handlePlayingInput();

    if (m_gameBoard)
        m_gameBoard->update(GetFrameTime());

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

// --------------------- Game Rendering ---------------------

void Game::draw() {
    // Draw animated gradient background
    drawGradientBackground();
    
    switch (m_currentState) {
        case GameState::MAIN_MENU: drawMainMenu(); break;
        case GameState::DIFFICULTY: drawDifficultySelection(); break;
        case GameState::PLAYING: drawPlaying(); break;
        case GameState::PAUSED: drawPaused(); break;
        case GameState::GAME_OVER: drawGameOver(); break;
        case GameState::SETTINGS: drawSettings(); break;
        case GameState::HIGH_SCORES: drawHighScores(); break;
    }
}

void Game::drawGradientBackground() {
    // Animated gradient background
    float time = GetTime();
    Color topColor = Utils::colorFromHSV(fmod(time * 20, 360), 0.6f, 0.4f);
    Color bottomColor = Utils::colorFromHSV(fmod(time * 20 + 180, 360), 0.6f, 0.2f);
    
    DrawRectangleGradientV(0, 0, m_screenWidth, m_screenHeight, topColor, bottomColor);
    
    // Draw floating particles
    for (int i = 0; i < 20; ++i) {
        float x = fmod(i * 73.5f + time * 20, m_screenWidth);
        float y = fmod(i * 127.3f + time * 15, m_screenHeight);
        float size = 2 + sin(time + i) * 2;
        DrawCircle(static_cast<int>(x), static_cast<int>(y), size, ColorAlpha(WHITE, 0.3f));
    }
}

void Game::drawMainMenu() {
    // Draw title with shadow and glow
    const char* title = "MEMORY CARD GAME";
    int titleSize = 50;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = m_screenWidth / 2 - titleWidth / 2;
    int titleY = 80;
    
    // Shadow
    DrawText(title, titleX + 4, titleY + 4, titleSize, ColorAlpha(BLACK, 0.5f));
    // Glow effect
    DrawText(title, titleX, titleY, titleSize, ColorAlpha(GOLD, 0.3f));
    // Main text
    DrawText(title, titleX, titleY, titleSize, GOLD);

    // Draw menu buttons with enhanced style
    for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
        Rectangle buttonRect = { 
            m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
            220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
            BUTTON_WIDTH, 
            BUTTON_HEIGHT 
        };
        bool isSelected = m_selectedMenuItem == (int)i;
        drawEnhancedButton(m_mainMenuItems[i], buttonRect, isSelected);
    }
    
    // Draw credits at bottom
    const char* credits = "Created by Yash Gangwani with love for - MSTC DA-IICT";
    int creditsWidth = MeasureText(credits, 16);
    DrawText(credits, m_screenWidth / 2 - creditsWidth / 2, m_screenHeight - 40, 16, ColorAlpha(WHITE, 0.6f));
}

void Game::drawDifficultySelection() {
    // Draw title
    const char* title = "SELECT DIFFICULTY";
    int titleSize = 40;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 123, titleSize, ColorAlpha(BLACK, 0.5f));
    DrawText(title, m_screenWidth / 2 - titleWidth / 2, 120, titleSize, GOLD);

    // Draw difficulty buttons
    for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
        Rectangle buttonRect = { 
            m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
            250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
            BUTTON_WIDTH, 
            BUTTON_HEIGHT 
        };
        bool isSelected = m_selectedDifficulty == (int)i;
        drawEnhancedButton(m_difficultyNames[i], buttonRect, isSelected);
    }
    
    // Draw back hint with icon
    DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
}

void Game::drawPlaying() {
    // Draw game board
    if (m_gameBoard)
        m_gameBoard->draw();

    // Draw enhanced HUD
    drawEnhancedHUD();
}

void Game::drawEnhancedHUD() {
    // Top bar background with transparency
    DrawRectangle(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.7f));
    DrawRectangleGradientV(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.5f), ColorAlpha(BLACK, 0.2f));
    
    // Stats panel on left
    int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
    int totalPairs = (static_cast<int>(m_difficulty) / 2);
    
    // Moves counter
    DrawRectangleRounded({15, 15, 150, 50}, 0.3f, 8, ColorAlpha(DARKBLUE, 0.8f));
    DrawRectangleRoundedLines({15, 15, 150, 50}, 0.3f, 8, 2, SKYBLUE);
    std::string movesStr = "MOVES: " + std::to_string(m_totalMoves);
    DrawText(movesStr.c_str(), 30, 30, 24, WHITE);
    
    // Matches counter with progress
    DrawRectangleRounded({180, 15, 180, 50}, 0.3f, 8, ColorAlpha(DARKGREEN, 0.8f));
    DrawRectangleRoundedLines({180, 15, 180, 50}, 0.3f, 8, 2, LIME);
    std::string matchesStr = "PAIRS: " + std::to_string(matches) + "/" + std::to_string(totalPairs);
    DrawText(matchesStr.c_str(), 195, 30, 24, WHITE);

    // Score display
    int score = m_scoreManager ? m_scoreManager->getScore() : 0;
    std::string scoreStr = "SCORE: " + std::to_string(score);
    DrawText(scoreStr.c_str(), 380, 30, 24, GOLD);
    
    // Progress bar
    float progress = totalPairs > 0 ? (float)matches / totalPairs : 0;
    DrawRectangleRounded({195, 53, 150 * progress, 8}, 0.5f, 8, LIME);
    
    // Timer on right
    float elapsed = getElapsedTime();
    std::string timerText = Utils::formatTime(elapsed);
    
    DrawRectangleRounded({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, ColorAlpha(MAROON, 0.8f));
    DrawRectangleRoundedLines({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, 2, RED);
    DrawText("TIME", m_screenWidth - 155, 22, 16, LIGHTGRAY);
    DrawText(timerText.c_str(), m_screenWidth - 155, 38, 28, GOLD);
    
    // Bottom hint
    DrawText("P - Pause", m_screenWidth - 150, m_screenHeight - 35, 18, ColorAlpha(WHITE, 0.6f));
}

void Game::drawPaused() {
    // Draw game board dimmed
    if (m_gameBoard)
        m_gameBoard->draw();
    
    // Draw overlay
    DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.8f));
    
    // Pause panel
    Rectangle panel = {m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 200, 500, 400};
    DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.95f));
    DrawRectangleRoundedLines(panel, 0.1f, 16, 4, SKYBLUE);
    
    // Pause text
    const char* pauseText = "PAUSED";
    int textSize = 60;
    int textWidth = MeasureText(pauseText, textSize);
    DrawText(pauseText, m_screenWidth / 2 - textWidth / 2 + 3, m_screenHeight / 2 - 123, textSize, ColorAlpha(BLACK, 0.5f));
    DrawText(pauseText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 120, textSize, GOLD);
    
    // Instructions with icons
    const char* resume = "SPACE - Resume";
    const char* menu = "M - Main Menu";
    int resumeWidth = MeasureText(resume, 24);
    int menuWidth = MeasureText(menu, 24);
    
    DrawText(resume, m_screenWidth / 2 - resumeWidth / 2, m_screenHeight / 2, 24, WHITE);
    DrawText(menu, m_screenWidth / 2 - menuWidth / 2, m_screenHeight / 2 + 40, 24, LIGHTGRAY);
}

void Game::drawGameOver() {
    // Draw game board in background
    if (m_gameBoard)
        m_gameBoard->draw();
    
    // Victory overlay
    DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.85f));
    
    // Victory panel
    Rectangle panel = {m_screenWidth / 2.0f - 300, m_screenHeight / 2.0f - 250, 600, 500};
    DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKGREEN, 0.95f));
    DrawRectangleRoundedLines(panel, 0.1f, 16, 4, LIME);
    
    // Animated stars/particles
    float time = GetTime();
    for (int i = 0; i < 30; ++i) {
        float angle = (i * 12.0f + time * 50) * DEG2RAD;
        float dist = 150 + sin(time * 2 + i) * 30;
        float x = m_screenWidth / 2 + cos(angle) * dist;
        float y = m_screenHeight / 2 - 100 + sin(angle) * dist;
        float size = 3 + sin(time * 3 + i) * 2;
        DrawCircle(static_cast<int>(x), static_cast<int>(y), size, ColorAlpha(GOLD, 0.8f));
    }
    
    // Victory text with glow
    const char* winText = "VICTORY!";
    int textSize = 70;
    int textWidth = MeasureText(winText, textSize);
    DrawText(winText, m_screenWidth / 2 - textWidth / 2 + 4, m_screenHeight / 2 - 164, textSize, ColorAlpha(BLACK, 0.5f));
    DrawText(winText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 160, textSize, GOLD);
    
    // Stats boxes
    float elapsedTime = getElapsedTime();
    std::string timeStr = Utils::formatTime(elapsedTime);
    std::string movesStr = std::to_string(m_totalMoves);
    
    // Time stat
    DrawRectangleRounded({m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(MAROON, 0.8f));
    DrawText("TIME", m_screenWidth / 2 - 220, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
    DrawText(timeStr.c_str(), m_screenWidth / 2 - 220, m_screenHeight / 2 - 5, 32, GOLD);
    
    // Moves stat
    DrawRectangleRounded({m_screenWidth / 2.0f - 20, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(DARKBLUE, 0.8f));
    DrawText("MOVES", m_screenWidth / 2 + 10, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
    DrawText(movesStr.c_str(), m_screenWidth / 2 + 10, m_screenHeight / 2 - 5, 32, SKYBLUE);
    
    // Action buttons with hover detection
    Vector2 mousePos = GetMousePosition();
    Rectangle restartBtn = {m_screenWidth / 2.0f - 230, m_screenHeight / 2.0f + 80, 200, 60};
    Rectangle menuBtn = {m_screenWidth / 2.0f + 30, m_screenHeight / 2.0f + 80, 200, 60};
    
    bool restartHovered = CheckCollisionPointRec(mousePos, restartBtn);
    bool menuHovered = CheckCollisionPointRec(mousePos, menuBtn);
    
    drawEnhancedButton("RESTART (R)", restartBtn, restartHovered, DARKGREEN);
    drawEnhancedButton("MENU (ESC)", menuBtn, menuHovered, DARKBLUE);
}

void Game::drawSettings() {
    // Draw title
    const char* title = "SETTINGS";
    int titleSize = 40;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 103, titleSize, ColorAlpha(BLACK, 0.5f));
    DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, titleSize, GOLD);
    
    // Settings panel
    Rectangle panel = {m_screenWidth / 2.0f - 300, 200, 600, 400};
    DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.8f));
    DrawRectangleRoundedLines(panel, 0.1f, 16, 3, SKYBLUE);
    
    DrawText("Settings", m_screenWidth / 2 - 40, m_screenHeight / 2 - 80, 28, WHITE);

    // Sound toggle
    Rectangle toggleRect = { m_screenWidth / 2.0f - 100, m_screenHeight / 2.0f - 10, 200, 60 };
    Vector2 mousePos = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mousePos, toggleRect);
    Color baseColor = m_soundEnabled ? DARKGREEN : DARKGRAY;
    drawEnhancedButton(std::string("Sound: ") + (m_soundEnabled ? "ON" : "OFF"), toggleRect, hovered, baseColor);

    DrawText("Click the button or press 'S' to toggle sound", m_screenWidth / 2 - 220, m_screenHeight / 2 + 80, 18, LIGHTGRAY);
    
    // Draw back hint
    DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
}

void Game::drawHighScores() {
    const char* title = "HIGH SCORES";
    int titleSize = 48;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 80, titleSize, ColorAlpha(BLACK, 0.5f));
    DrawText(title, m_screenWidth / 2 - titleWidth / 2, 76, titleSize, GOLD);

    // Draw panel
    Rectangle panel = { m_screenWidth / 2.0f - 300, 180, 600, 420 };
    DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.9f));
    DrawRectangleRoundedLines(panel, 0.1f, 16, 3, SKYBLUE);

    // Show high score
    int high = m_scoreManager ? m_scoreManager->getHighScore() : 0;
    std::string hs = "High Score: " + std::to_string(high);
    int y = m_screenHeight / 2 - 10;
    DrawText(hs.c_str(), m_screenWidth / 2 - MeasureText(hs.c_str(), 28) / 2, y, 28, GOLD);

    DrawText("Press <ESC> to return", 20, m_screenHeight - 40, 20, ColorAlpha(WHITE, 0.8f));
}



void Game::drawEnhancedButton(const std::string& text, Rectangle bounds, bool isSelected, Color baseColor) {
    // Button animation
    float scale = isSelected ? 1.05f : 1.0f;
    Rectangle animBounds = {
        bounds.x - (bounds.width * scale - bounds.width) / 2,
        bounds.y - (bounds.height * scale - bounds.height) / 2,
        bounds.width * scale,
        bounds.height * scale
    };
    
    // Shadow
    DrawRectangleRounded({animBounds.x + 6, animBounds.y + 6, animBounds.width, animBounds.height}, 
                         0.2f, 16, ColorAlpha(BLACK, 0.5f));
    
    // Button background with gradient effect
    Color bgColor = isSelected ? Utils::adjustBrightness(baseColor, 1.3f) : baseColor;
    DrawRectangleRounded(animBounds, 0.2f, 16, bgColor);
    
    // Glow effect for selected
    if (isSelected) {
        DrawRectangleRoundedLines(animBounds, 0.2f, 16, 3, GOLD);
        DrawRectangleRoundedLines({animBounds.x - 2, animBounds.y - 2, animBounds.width + 4, animBounds.height + 4}, 
                                  0.2f, 16, 1, ColorAlpha(GOLD, 0.5f));
    } else {
        DrawRectangleRoundedLines(animBounds, 0.2f, 16, 2, Utils::adjustBrightness(baseColor, 1.5f));
    }
    
    // Text with shadow
    int textSize = 24;
    int textWidth = MeasureText(text.c_str(), textSize);
    int textX = static_cast<int>(animBounds.x + (animBounds.width - textWidth) / 2);
    int textY = static_cast<int>(animBounds.y + (animBounds.height - textSize) / 2);
    
    DrawText(text.c_str(), textX + 2, textY + 2, textSize, ColorAlpha(BLACK, 0.6f));
    DrawText(text.c_str(), textX, textY, textSize, isSelected ? GOLD : WHITE);
}

// --------------------- State Transitions ---------------------

void Game::changeState(GameState newState) {
    m_previousState = m_currentState;
    m_currentState = newState;
}

void Game::startNewGame(Difficulty difficulty) {
    m_difficulty = difficulty;

    int numCards = static_cast<int>(difficulty);
    int gridSize = static_cast<int>(sqrt(numCards));
    Vector2 cardSize = Utils::calculateOptimalCardSize(
        gridSize, gridSize,
        Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 200},
        15.0f
    );

    // Create game board first
    m_gameBoard = std::make_unique<GameBoard>(
        gridSize, gridSize, cardSize, 15.0f,
        Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 150}
    );

    // Create audio manager
    m_audioManager = std::make_unique<AudioManager>();
    // Apply current settings (mute) to the newly created AudioManager
    if (m_audioManager) m_audioManager->setMuted(!m_soundEnabled);
    Utils::logInfo("AudioManager created");
    
    // Create score manager
    m_scoreManager = std::make_unique<ScoreManager>();
    // Reset score for new game and connect to board
    m_scoreManager->resetScore();
    if (m_gameBoard) {
        m_gameBoard->setScoreManager(m_scoreManager.get());
    }
    
    // Connect AudioManager to GameBoard
    if (m_audioManager && m_gameBoard) {
        m_gameBoard->setAudioManager(m_audioManager.get());
        Utils::logInfo("AudioManager connected to GameBoard");
    }

    m_totalMoves = 0;
    m_matchesFound = 0;
    m_gameWon = false;
    m_gameStartTime = GetTime();
    
    Utils::logInfo("New game started");
}

void Game::pauseGame() {
    m_pausedTime = GetTime();
    changeState(GameState::PAUSED);
}

void Game::resumeGame() {
    float pausedDuration = GetTime() - m_pausedTime;
    m_gameStartTime += pausedDuration;
    changeState(GameState::PLAYING);
}

void Game::restartGame() {
    startNewGame(m_difficulty);
}

void Game::returnToMainMenu() {
    changeState(GameState::MAIN_MENU);
}

// --------------------- Helpers ---------------------

void Game::checkWinCondition() {
    if (m_gameBoard && m_gameBoard->allMatched() && !m_gameWon) {
        m_gameWon = true;
        m_pausedTime = GetTime();
        m_matchesFound = m_gameBoard->getMatchesFound();
        // Save high score if applicable
        if (m_scoreManager) m_scoreManager->trySaveHighScore();
        changeState(GameState::GAME_OVER);
    }
}

float Game::getElapsedTime() const {
    if (m_gameWon) {
        return m_pausedTime - m_gameStartTime;
    }
    return GetTime() - m_gameStartTime;
}

void Game::drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color) {
    drawEnhancedButton(text, bounds, isSelected, color);
}

void Game::drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font) {
    int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
    DrawTextEx(font, text.c_str(), {m_screenWidth / 2.0f - textWidth / 2, (float)y}, fontSize, 1, color);
}

void Game::drawGameStats() {
    drawEnhancedHUD();
}

void Game::drawTimer() {
    // Handled in drawEnhancedHUD
}

// --------------------- Input Handling ---------------------

void Game::handleMainMenuInput() {
    Vector2 mousePos = GetMousePosition();
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
            Rectangle buttonRect = {
                m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
                220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
                BUTTON_WIDTH,
                BUTTON_HEIGHT
            };
            
            if (CheckCollisionPointRec(mousePos, buttonRect)) {
                switch (i) {
                    case 0: changeState(GameState::DIFFICULTY); break;
                    case 1: changeState(GameState::SETTINGS); break;
                    case 2: changeState(GameState::HIGH_SCORES); break;
                    case 3: CloseWindow(); break;
                }
            }
        }
    }
    
    m_selectedMenuItem = -1;
    for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
        Rectangle buttonRect = {
            m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
            220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
            BUTTON_WIDTH,
            BUTTON_HEIGHT
        };
        
        if (CheckCollisionPointRec(mousePos, buttonRect)) {
            m_selectedMenuItem = static_cast<int>(i);
        }
    }
    
    if (IsKeyPressed(KEY_DOWN)) {
        m_selectedMenuItem = (m_selectedMenuItem + 1) % m_mainMenuItems.size();
    }
    if (IsKeyPressed(KEY_UP)) {
        m_selectedMenuItem = (m_selectedMenuItem - 1 + m_mainMenuItems.size()) % m_mainMenuItems.size();
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (m_selectedMenuItem >= 0) {
            switch (m_selectedMenuItem) {
                case 0: changeState(GameState::DIFFICULTY); break;
                case 1: changeState(GameState::SETTINGS); break;
                case 2: Utils::logInfo("High Scores clicked"); break;
                case 3: CloseWindow(); break;
            }
        }
    }
}

void Game::handleDifficultySelectionInput() {
    Vector2 mousePos = GetMousePosition();
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
            Rectangle buttonRect = {
                m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
                250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
                BUTTON_WIDTH,
                BUTTON_HEIGHT
            };
            
            if (CheckCollisionPointRec(mousePos, buttonRect)) {
                switch (i) {
                    case 0: m_difficulty = Difficulty::EASY; break;
                    case 1: m_difficulty = Difficulty::MEDIUM; break;
                    case 2: m_difficulty = Difficulty::HARD; break;
                }
                startNewGame(m_difficulty);
                changeState(GameState::PLAYING);
            }
        }
    }
    
    m_selectedDifficulty = -1;
    for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
        Rectangle buttonRect = {
            m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
            250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
            BUTTON_WIDTH,
            BUTTON_HEIGHT
        };
        
        if (CheckCollisionPointRec(mousePos, buttonRect)) {
            m_selectedDifficulty = static_cast<int>(i);
        }
    }
    
    if (IsKeyPressed(KEY_DOWN)) {
        m_selectedDifficulty = (m_selectedDifficulty + 1) % m_difficultyNames.size();
    }
    if (IsKeyPressed(KEY_UP)) {
        m_selectedDifficulty = (m_selectedDifficulty - 1 + m_difficultyNames.size()) % m_difficultyNames.size();
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (m_selectedDifficulty >= 0) {
            switch (m_selectedDifficulty) {
                case 0: m_difficulty = Difficulty::EASY; break;
                case 1: m_difficulty = Difficulty::MEDIUM; break;
                case 2: m_difficulty = Difficulty::HARD; break;
            }
            startNewGame(m_difficulty);
            changeState(GameState::PLAYING);
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        changeState(GameState::MAIN_MENU);
    }
}

void Game::handlePlayingInput() {
    // Pause game with P key only (not ESC to avoid accidental exits)
    if (IsKeyPressed(KEY_P)) {
        pauseGame();
        return;
    }
    
    // Handle card clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_gameBoard) {
        Vector2 mousePos = GetMousePosition();
        m_gameBoard->handleClick(mousePos);
        m_totalMoves++;
    }
}

void Game::handlePausedInput() {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)) {
        resumeGame();
    }
    
    if (IsKeyPressed(KEY_M)) {
        returnToMainMenu();
    }
}

void Game::handleGameOverInput() {
    // Handle keyboard input
    if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
        restartGame();
        changeState(GameState::PLAYING);
    }
    
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) {
        returnToMainMenu();
    }
    
    // Handle mouse clicks on buttons
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Rectangle restartBtn = {m_screenWidth / 2.0f - 230, m_screenHeight / 2.0f + 80, 200, 60};
        Rectangle menuBtn = {m_screenWidth / 2.0f + 30, m_screenHeight / 2.0f + 80, 200, 60};
        
        if (CheckCollisionPointRec(mousePos, restartBtn)) {
            restartGame();
            changeState(GameState::PLAYING);
        } else if (CheckCollisionPointRec(mousePos, menuBtn)) {
            returnToMainMenu();
        }
    }
}

void Game::handleSettingsInput() {
    // Toggle sound with 'S'
    if (IsKeyPressed(KEY_S)) {
        m_soundEnabled = !m_soundEnabled;
        Utils::logInfo(std::string("Settings: soundEnabled = ") + (m_soundEnabled ? "true" : "false"));
        if (m_audioManager) m_audioManager->setMuted(!m_soundEnabled);
    }

    // Toggle via mouse click on the toggle button area
    Vector2 mousePos = GetMousePosition();
    Rectangle toggleRect = { m_screenWidth / 2.0f - 100, m_screenHeight / 2.0f - 10, 200, 60 };
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, toggleRect)) {
        m_soundEnabled = !m_soundEnabled;
        Utils::logInfo(std::string("Settings (mouse): soundEnabled = ") + (m_soundEnabled ? "true" : "false"));
        if (m_audioManager) m_audioManager->setMuted(!m_soundEnabled);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        // Return to main menu instead of exiting the program (Exit key disabled in main)
        changeState(GameState::MAIN_MENU);
    }
}

void Game::handleHighScoresInput() {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        changeState(GameState::MAIN_MENU);
    }
}

// /**
//  * @file Game.cpp
//  * @brief Main Game class implementation with enhanced UI
//  */

// #include "../include/Game.h"
// #include "../include/Utils.h"
// #include <cmath>
// #include <iostream>

// // --------------------- Constructor & Destructor ---------------------

// Game::Game(int screenWidth, int screenHeight)
//     : m_screenWidth(screenWidth),
//       m_screenHeight(screenHeight),
//       m_currentState(GameState::MAIN_MENU),
//       m_previousState(GameState::MAIN_MENU),
//       m_difficulty(Difficulty::EASY),
//       m_selectedMenuItem(0),
//       m_selectedDifficulty(0),
//       m_totalMoves(0),
//       m_matchesFound(0),
//       m_gameWon(false),
//       m_gameBoard(nullptr),
//       m_audioManager(nullptr),
//       m_scoreManager(nullptr),
//       m_gameStartTime(0.0f),
//       m_currentTime(0.0f),
//       m_pausedTime(0.0f)
// {
//     loadResources();
// }

// Game::~Game() {
//     unloadResources();
// }

// // --------------------- Resource Management ---------------------

// void Game::loadResources() {
//     // Load fonts (use default if files don't exist)
//     if (FileExists("assets/fonts/arial.ttf")) {
//         m_titleFont = LoadFont("assets/fonts/arial.ttf");
//         m_uiFont = LoadFont("assets/fonts/arial.ttf");
//     } else {
//         m_titleFont = GetFontDefault();
//         m_uiFont = GetFontDefault();
//     }

//     // Load background (optional)
//     if (FileExists("assets/textures/background.png")) {
//         m_backgroundTexture = LoadTexture("assets/textures/background.png");
//     }

//     Utils::logInfo("Resources loaded successfully.");
// }

// void Game::unloadResources() {
//     if (m_titleFont.texture.id != GetFontDefault().texture.id) {
//         UnloadFont(m_titleFont);
//     }
//     if (m_uiFont.texture.id != GetFontDefault().texture.id) {
//         UnloadFont(m_uiFont);
//     }
//     if (m_backgroundTexture.id != 0) {
//         UnloadTexture(m_backgroundTexture);
//     }

//     Utils::logInfo("Resources unloaded successfully.");
// }

// // --------------------- Game State Updates ---------------------

// void Game::update() {
//     switch (m_currentState) {
//         case GameState::MAIN_MENU: updateMainMenu(); break;
//         case GameState::DIFFICULTY: updateDifficultySelection(); break;
//         case GameState::PLAYING: updatePlaying(); break;
//         case GameState::PAUSED: updatePaused(); break;
//         case GameState::GAME_OVER: updateGameOver(); break;
//         case GameState::SETTINGS: updateSettings(); break;
//     }
// }

// void Game::updateMainMenu() {
//     handleMainMenuInput();
// }

// void Game::updateDifficultySelection() {
//     handleDifficultySelectionInput();
// }

// void Game::updatePlaying() {
//     handlePlayingInput();

//     if (m_gameBoard)
//         m_gameBoard->update(GetFrameTime());

//     checkWinCondition();
// }

// void Game::updatePaused() {
//     handlePausedInput();
// }

// void Game::updateGameOver() {
//     handleGameOverInput();
// }

// void Game::updateSettings() {
//     handleSettingsInput();
// }

// // --------------------- Game Rendering ---------------------

// void Game::draw() {
//     // Draw animated gradient background
//     drawGradientBackground();
    
//     switch (m_currentState) {
//         case GameState::MAIN_MENU: drawMainMenu(); break;
//         case GameState::DIFFICULTY: drawDifficultySelection(); break;
//         case GameState::PLAYING: drawPlaying(); break;
//         case GameState::PAUSED: drawPaused(); break;
//         case GameState::GAME_OVER: drawGameOver(); break;
//         case GameState::SETTINGS: drawSettings(); break;
//     }
// }

// void Game::drawGradientBackground() {
//     // Animated gradient background
//     float time = GetTime();
//     Color topColor = Utils::colorFromHSV(fmod(time * 20, 360), 0.6f, 0.4f);
//     Color bottomColor = Utils::colorFromHSV(fmod(time * 20 + 180, 360), 0.6f, 0.2f);
    
//     DrawRectangleGradientV(0, 0, m_screenWidth, m_screenHeight, topColor, bottomColor);
    
//     // Draw floating particles
//     for (int i = 0; i < 20; ++i) {
//         float x = fmod(i * 73.5f + time * 20, m_screenWidth);
//         float y = fmod(i * 127.3f + time * 15, m_screenHeight);
//         float size = 2 + sin(time + i) * 2;
//         DrawCircle(x, y, size, ColorAlpha(WHITE, 0.3f));
//     }
// }

// void Game::drawMainMenu() {
//     // Draw title with shadow and glow
//     const char* title = "MEMORY CARD GAME";
//     int titleSize = 50;
//     int titleWidth = MeasureText(title, titleSize);
//     int titleX = m_screenWidth / 2 - titleWidth / 2;
//     int titleY = 80;
    
//     // Shadow
//     DrawText(title, titleX + 4, titleY + 4, titleSize, ColorAlpha(BLACK, 0.5f));
//     // Glow effect
//     DrawText(title, titleX, titleY, titleSize, ColorAlpha(GOLD, 0.3f));
//     // Main text
//     DrawText(title, titleX, titleY, titleSize, GOLD);

//     // Draw menu buttons with enhanced style
//     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
//         Rectangle buttonRect = { 
//             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
//             220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
//             BUTTON_WIDTH, 
//             BUTTON_HEIGHT 
//         };
//         bool isSelected = m_selectedMenuItem == (int)i;
//         drawEnhancedButton(m_mainMenuItems[i], buttonRect, isSelected);
//     }
    
//     // Draw credits at bottom
//     const char* credits = "Created with Raylib - MSTC DA-IICT";
//     int creditsWidth = MeasureText(credits, 16);
//     DrawText(credits, m_screenWidth / 2 - creditsWidth / 2, m_screenHeight - 40, 16, ColorAlpha(WHITE, 0.6f));
// }

// void Game::drawDifficultySelection() {
//     // Draw title
//     const char* title = "SELECT DIFFICULTY";
//     int titleSize = 40;
//     int titleWidth = MeasureText(title, titleSize);
//     DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 123, titleSize, ColorAlpha(BLACK, 0.5f));
//     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 120, titleSize, GOLD);

//     // Draw difficulty buttons
//     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
//         Rectangle buttonRect = { 
//             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
//             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
//             BUTTON_WIDTH, 
//             BUTTON_HEIGHT 
//         };
//         bool isSelected = m_selectedDifficulty == (int)i;
//         drawEnhancedButton(m_difficultyNames[i], buttonRect, isSelected);
//     }
    
//     // Draw back hint with icon
//     DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
// }

// void Game::drawPlaying() {
//     // Draw game board
//     if (m_gameBoard)
//         m_gameBoard->draw();

//     // Draw enhanced HUD
//     drawEnhancedHUD();
// }

// void Game::drawEnhancedHUD() {
//     // Top bar background with transparency
//     DrawRectangle(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.7f));
//     DrawRectangleGradientV(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.5f), ColorAlpha(BLACK, 0.2f));
    
//     // Stats panel on left
//     int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
//     int totalPairs = (static_cast<int>(m_difficulty) / 2);
    
//     // Moves counter
//     DrawRectangleRounded({15, 15, 150, 50}, 0.3f, 8, ColorAlpha(DARKBLUE, 0.8f));
//     DrawRectangleRoundedLines({15, 15, 150, 50}, 0.3f, 8, 2, SKYBLUE);
//     std::string movesStr = "MOVES: " + std::to_string(m_totalMoves);
//     DrawText(movesStr.c_str(), 30, 30, 24, WHITE);
    
//     // Matches counter with progress
//     DrawRectangleRounded({180, 15, 180, 50}, 0.3f, 8, ColorAlpha(DARKGREEN, 0.8f));
//     DrawRectangleRoundedLines({180, 15, 180, 50}, 0.3f, 8, 2, LIME);
//     std::string matchesStr = "PAIRS: " + std::to_string(matches) + "/" + std::to_string(totalPairs);
//     DrawText(matchesStr.c_str(), 195, 30, 24, WHITE);
    
//     // Progress bar
//     float progress = totalPairs > 0 ? (float)matches / totalPairs : 0;
//     DrawRectangleRounded({195, 53, 150 * progress, 8}, 0.5f, 8, LIME);
    
//     // Timer on right
//     float elapsed = getElapsedTime();
//     std::string timerText = Utils::formatTime(elapsed);
//     int timerWidth = MeasureText(timerText.c_str(), 28);
    
//     DrawRectangleRounded({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, ColorAlpha(MAROON, 0.8f));
//     DrawRectangleRoundedLines({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, 2, RED);
//     DrawText("TIME", m_screenWidth - 155, 22, 16, LIGHTGRAY);
//     DrawText(timerText.c_str(), m_screenWidth - 155, 38, 28, GOLD);
    
//     // Bottom hint
//     DrawText("P - Pause", m_screenWidth - 150, m_screenHeight - 35, 18, ColorAlpha(WHITE, 0.6f));
// }

// void Game::drawPaused() {
//     // Draw game board dimmed
//     if (m_gameBoard)
//         m_gameBoard->draw();
    
//     // Draw overlay
//     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.8f));
    
//     // Pause panel
//     Rectangle panel = {m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 200, 500, 400};
//     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.95f));
//     DrawRectangleRoundedLines(panel, 0.1f, 16, 4, SKYBLUE);
    
//     // Pause text
//     const char* pauseText = "PAUSED";
//     int textSize = 60;
//     int textWidth = MeasureText(pauseText, textSize);
//     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2 + 3, m_screenHeight / 2 - 123, textSize, ColorAlpha(BLACK, 0.5f));
//     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 120, textSize, GOLD);
    
//     // Instructions with icons
//     const char* resume = "SPACE - Resume";
//     const char* menu = "M - Main Menu";
//     int resumeWidth = MeasureText(resume, 24);
//     int menuWidth = MeasureText(menu, 24);
    
//     DrawText(resume, m_screenWidth / 2 - resumeWidth / 2, m_screenHeight / 2, 24, WHITE);
//     DrawText(menu, m_screenWidth / 2 - menuWidth / 2, m_screenHeight / 2 + 40, 24, LIGHTGRAY);
// }

// void Game::drawGameOver() {
//     // Draw game board in background
//     if (m_gameBoard)
//         m_gameBoard->draw();
    
//     // Victory overlay
//     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.85f));
    
//     // Victory panel
//     Rectangle panel = {m_screenWidth / 2.0f - 300, m_screenHeight / 2.0f - 250, 600, 500};
//     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKGREEN, 0.95f));
//     DrawRectangleRoundedLines(panel, 0.1f, 16, 4, LIME);
    
//     // Animated stars/particles
//     float time = GetTime();
//     for (int i = 0; i < 30; ++i) {
//         float angle = (i * 12.0f + time * 50) * DEG2RAD;
//         float dist = 150 + sin(time * 2 + i) * 30;
//         float x = m_screenWidth / 2 + cos(angle) * dist;
//         float y = m_screenHeight / 2 - 100 + sin(angle) * dist;
//         float size = 3 + sin(time * 3 + i) * 2;
//         DrawCircle(x, y, size, ColorAlpha(GOLD, 0.8f));
//     }
    
//     // Victory text with glow
//     const char* winText = "VICTORY!";
//     int textSize = 70;
//     int textWidth = MeasureText(winText, textSize);
//     DrawText(winText, m_screenWidth / 2 - textWidth / 2 + 4, m_screenHeight / 2 - 164, textSize, ColorAlpha(BLACK, 0.5f));
//     DrawText(winText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 160, textSize, GOLD);
    
//     // Stats boxes
//     float elapsedTime = getElapsedTime();
//     std::string timeStr = Utils::formatTime(elapsedTime);
//     std::string movesStr = std::to_string(m_totalMoves);
//     int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
//     std::string matchesStr = std::to_string(matches);
    
//     // Time stat
//     DrawRectangleRounded({m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(MAROON, 0.8f));
//     DrawText("TIME", m_screenWidth / 2 - 220, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
//     DrawText(timeStr.c_str(), m_screenWidth / 2 - 220, m_screenHeight / 2 - 5, 32, GOLD);
    
//     // Moves stat
//     DrawRectangleRounded({m_screenWidth / 2.0f - 20, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(DARKBLUE, 0.8f));
//     DrawText("MOVES", m_screenWidth / 2 + 10, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
//     DrawText(movesStr.c_str(), m_screenWidth / 2 + 10, m_screenHeight / 2 - 5, 32, SKYBLUE);
    
//     // Action buttons with hover detection
//     Vector2 mousePos = GetMousePosition();
//     Rectangle restartBtn = {m_screenWidth / 2.0f - 230, m_screenHeight / 2.0f + 80, 200, 60};
//     Rectangle menuBtn = {m_screenWidth / 2.0f + 30, m_screenHeight / 2.0f + 80, 200, 60};
    
//     bool restartHovered = CheckCollisionPointRec(mousePos, restartBtn);
//     bool menuHovered = CheckCollisionPointRec(mousePos, menuBtn);
    
//     drawEnhancedButton("RESTART (R)", restartBtn, restartHovered, DARKGREEN);
//     drawEnhancedButton("MENU (ESC)", menuBtn, menuHovered, DARKBLUE);
// }

// void Game::drawSettings() {
//     // Draw title
//     const char* title = "SETTINGS";
//     int titleSize = 40;
//     int titleWidth = MeasureText(title, titleSize);
//     DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 103, titleSize, ColorAlpha(BLACK, 0.5f));
//     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, titleSize, GOLD);
    
//     // Settings panel
//     Rectangle panel = {m_screenWidth / 2.0f - 300, 200, 600, 400};
//     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.8f));
//     DrawRectangleRoundedLines(panel, 0.1f, 16, 3, SKYBLUE);
    
//     DrawText("Settings coming soon...", m_screenWidth / 2 - 120, m_screenHeight / 2, 24, LIGHTGRAY);
//     DrawText("Volume Controls", m_screenWidth / 2 - 250, m_screenHeight / 2 + 60, 20, WHITE);
//     DrawText("Sound Effects", m_screenWidth / 2 - 250, m_screenHeight / 2 + 100, 20, WHITE);
//     DrawText("Music", m_screenWidth / 2 - 250, m_screenHeight / 2 + 140, 20, WHITE);
    
//     // Draw back hint
//     DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
// }

// void Game::drawEnhancedButton(const std::string& text, Rectangle bounds, bool isSelected, Color baseColor) {
//     // Button animation
//     float scale = isSelected ? 1.05f : 1.0f;
//     Rectangle animBounds = {
//         bounds.x - (bounds.width * scale - bounds.width) / 2,
//         bounds.y - (bounds.height * scale - bounds.height) / 2,
//         bounds.width * scale,
//         bounds.height * scale
//     };
    
//     // Shadow
//     DrawRectangleRounded({animBounds.x + 6, animBounds.y + 6, animBounds.width, animBounds.height}, 
//                          0.2f, 16, ColorAlpha(BLACK, 0.5f));
    
//     // Button background with gradient effect
//     Color bgColor = isSelected ? Utils::adjustBrightness(baseColor, 1.3f) : baseColor;
//     DrawRectangleRounded(animBounds, 0.2f, 16, bgColor);
    
//     // Glow effect for selected
//     if (isSelected) {
//         DrawRectangleRoundedLines(animBounds, 0.2f, 16, 3, GOLD);
//         DrawRectangleRoundedLines({animBounds.x - 2, animBounds.y - 2, animBounds.width + 4, animBounds.height + 4}, 
//                                   0.2f, 16, 1, ColorAlpha(GOLD, 0.5f));
//     } else {
//         DrawRectangleRoundedLines(animBounds, 0.2f, 16, 2, Utils::adjustBrightness(baseColor, 1.5f));
//     }
    
//     // Text with shadow
//     int textSize = 24;
//     int textWidth = MeasureText(text.c_str(), textSize);
//     int textX = animBounds.x + (animBounds.width - textWidth) / 2;
//     int textY = animBounds.y + (animBounds.height - textSize) / 2;
    
//     DrawText(text.c_str(), textX + 2, textY + 2, textSize, ColorAlpha(BLACK, 0.6f));
//     DrawText(text.c_str(), textX, textY, textSize, isSelected ? GOLD : WHITE);
// }

// // --------------------- State Transitions ---------------------

// void Game::changeState(GameState newState) {
//     m_previousState = m_currentState;
//     m_currentState = newState;
// }

// void Game::startNewGame(Difficulty difficulty) {
//     m_difficulty = difficulty;

//     int numCards = static_cast<int>(difficulty);
//     int gridSize = static_cast<int>(sqrt(numCards));
//     Vector2 cardSize = Utils::calculateOptimalCardSize(
//         gridSize, gridSize,
//         Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 200},
//         15.0f
//     );

//     // Create game board first
//     m_gameBoard = std::make_unique<GameBoard>(
//         gridSize, gridSize, cardSize, 15.0f,
//         Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 150}
//     );

//     // Create managers
//     try {
//         m_audioManager = std::make_unique<AudioManager>();
//         Utils::logInfo("AudioManager created successfully");
//     } catch (const std::exception& e) {
//         Utils::logError("Failed to create AudioManager: " + std::string(e.what()));
//         m_audioManager = nullptr;
//     }
    
//     m_scoreManager = std::make_unique<ScoreManager>();
    
//     // Connect AudioManager to GameBoard if available
//     if (m_audioManager && m_gameBoard) {
//         m_gameBoard->setAudioManager(m_audioManager.get());
//         Utils::logInfo("AudioManager connected to GameBoard");
//     }

//     m_totalMoves = 0;
//     m_matchesFound = 0;
//     m_gameWon = false;
//     m_gameStartTime = GetTime();
    
//     Utils::logInfo("New game started with difficulty: " + std::to_string(static_cast<int>(difficulty)) + " cards");
// }

// void Game::pauseGame() {
//     m_pausedTime = GetTime();
//     changeState(GameState::PAUSED);
// }

// void Game::resumeGame() {
//     float pausedDuration = GetTime() - m_pausedTime;
//     m_gameStartTime += pausedDuration;
//     changeState(GameState::PLAYING);
// }

// void Game::restartGame() {
//     startNewGame(m_difficulty);
// }

// void Game::returnToMainMenu() {
//     changeState(GameState::MAIN_MENU);
// }

// // --------------------- Helpers ---------------------

// void Game::checkWinCondition() {
//     if (m_gameBoard && m_gameBoard->allMatched() && !m_gameWon) {
//         m_gameWon = true;
//         m_pausedTime = GetTime();
//         m_matchesFound = m_gameBoard->getMatchesFound();
//         changeState(GameState::GAME_OVER);
//     }
// }

// float Game::getElapsedTime() const {
//     if (m_gameWon) {
//         return m_pausedTime - m_gameStartTime;
//     }
//     return GetTime() - m_gameStartTime;
// }

// void Game::drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color) {
//     drawEnhancedButton(text, bounds, isSelected, color);
// }

// void Game::drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font) {
//     int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
//     DrawTextEx(font, text.c_str(), {m_screenWidth / 2.0f - textWidth / 2, (float)y}, fontSize, 1, color);
// }

// void Game::drawGameStats() {
//     drawEnhancedHUD();
// }

// void Game::drawTimer() {
//     // Handled in drawEnhancedHUD
// }

// // --------------------- Input Handling ---------------------

// void Game::handleMainMenuInput() {
//     Vector2 mousePos = GetMousePosition();
    
//     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//         for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
//             Rectangle buttonRect = {
//                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
//                 220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
//                 BUTTON_WIDTH,
//                 BUTTON_HEIGHT
//             };
            
//             if (CheckCollisionPointRec(mousePos, buttonRect)) {
//                 switch (i) {
//                     case 0: changeState(GameState::DIFFICULTY); break;
//                     case 1: changeState(GameState::SETTINGS); break;
//                     case 2: Utils::logInfo("High Scores clicked"); break;
//                     case 3: CloseWindow(); break;
//                 }
//             }
//         }
//     }
    
//     m_selectedMenuItem = -1;
//     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
//         Rectangle buttonRect = {
//             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
//             220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
//             BUTTON_WIDTH,
//             BUTTON_HEIGHT
//         };
        
//         if (CheckCollisionPointRec(mousePos, buttonRect)) {
//             m_selectedMenuItem = static_cast<int>(i);
//         }
//     }
    
//     if (IsKeyPressed(KEY_DOWN)) {
//         m_selectedMenuItem = (m_selectedMenuItem + 1) % m_mainMenuItems.size();
//     }
//     if (IsKeyPressed(KEY_UP)) {
//         m_selectedMenuItem = (m_selectedMenuItem - 1 + m_mainMenuItems.size()) % m_mainMenuItems.size();
//     }
//     if (IsKeyPressed(KEY_ENTER)) {
//         if (m_selectedMenuItem >= 0) {
//             switch (m_selectedMenuItem) {
//                 case 0: changeState(GameState::DIFFICULTY); break;
//                 case 1: changeState(GameState::SETTINGS); break;
//                 case 2: Utils::logInfo("High Scores clicked"); break;
//                 case 3: CloseWindow(); break;
//             }
//         }
//     }
// }

// void Game::handleDifficultySelectionInput() {
//     Vector2 mousePos = GetMousePosition();
    
//     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//         for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
//             Rectangle buttonRect = {
//                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
//                 250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
//                 BUTTON_WIDTH,
//                 BUTTON_HEIGHT
//             };
            
//             if (CheckCollisionPointRec(mousePos, buttonRect)) {
//                 switch (i) {
//                     case 0: m_difficulty = Difficulty::EASY; break;
//                     case 1: m_difficulty = Difficulty::MEDIUM; break;
//                     case 2: m_difficulty = Difficulty::HARD; break;
//                 }
//                 startNewGame(m_difficulty);
//                 changeState(GameState::PLAYING);
//             }
//         }
//     }
    
//     m_selectedDifficulty = -1;
//     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
//         Rectangle buttonRect = {
//             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
//             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
//             BUTTON_WIDTH,
//             BUTTON_HEIGHT
//         };
        
//         if (CheckCollisionPointRec(mousePos, buttonRect)) {
//             m_selectedDifficulty = static_cast<int>(i);
//         }
//     }
    
//     if (IsKeyPressed(KEY_DOWN)) {
//         m_selectedDifficulty = (m_selectedDifficulty + 1) % m_difficultyNames.size();
//     }
//     if (IsKeyPressed(KEY_UP)) {
//         m_selectedDifficulty = (m_selectedDifficulty - 1 + m_difficultyNames.size()) % m_difficultyNames.size();
//     }
//     if (IsKeyPressed(KEY_ENTER)) {
//         if (m_selectedDifficulty >= 0) {
//             switch (m_selectedDifficulty) {
//                 case 0: m_difficulty = Difficulty::EASY; break;
//                 case 1: m_difficulty = Difficulty::MEDIUM; break;
//                 case 2: m_difficulty = Difficulty::HARD; break;
//             }
//             startNewGame(m_difficulty);
//             changeState(GameState::PLAYING);
//         }
//     }
//     if (IsKeyPressed(KEY_ESCAPE)) {
//         changeState(GameState::MAIN_MENU);
//     }
// }

// void Game::handlePlayingInput() {
//     // Pause game with P key only (not ESC to avoid accidental exits)
//     if (IsKeyPressed(KEY_P)) {
//         pauseGame();
//         return;
//     }
    
//     // Handle card clicks
//     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_gameBoard) {
//         Vector2 mousePos = GetMousePosition();
//         m_gameBoard->handleClick(mousePos);
//         m_totalMoves++;
//     }
// }

// void Game::handlePausedInput() {
//     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)) {
//         resumeGame();
//     }
    
//     if (IsKeyPressed(KEY_M)) {
//         returnToMainMenu();
//     }
// }

// void Game::handleGameOverInput() {
//     // Handle keyboard input
//     if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
//         restartGame();
//         changeState(GameState::PLAYING);
//     }
    
//     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) {
//         returnToMainMenu();
//     }
    
//     // Handle mouse clicks on buttons
//     Vector2 mousePos = GetMousePosition();
//     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//         Rectangle restartBtn = {m_screenWidth / 2.0f - 230, m_screenHeight / 2.0f + 80, 200, 60};
//         Rectangle menuBtn = {m_screenWidth / 2.0f + 30, m_screenHeight / 2.0f + 80, 200, 60};
        
//         if (CheckCollisionPointRec(mousePos, restartBtn)) {
//             restartGame();
//             changeState(GameState::PLAYING);
//         } else if (CheckCollisionPointRec(mousePos, menuBtn)) {
//             returnToMainMenu();
//         }
//     }
// }

// void Game::handleSettingsInput() {
//     if (IsKeyPressed(KEY_ESCAPE)) {
//         changeState(GameState::MAIN_MENU);
//     }
// }

// // /**
// //  * @file Game.cpp
// //  * @brief Main Game class implementation with enhanced UI
// //  */

// // #include "../include/Game.h"
// // #include "../include/Utils.h"
// // #include <cmath>
// // #include <iostream>

// // // --------------------- Constructor & Destructor ---------------------

// // Game::Game(int screenWidth, int screenHeight)
// //     : m_screenWidth(screenWidth),
// //       m_screenHeight(screenHeight),
// //       m_currentState(GameState::MAIN_MENU),
// //       m_previousState(GameState::MAIN_MENU),
// //       m_difficulty(Difficulty::EASY),
// //       m_selectedMenuItem(0),
// //       m_selectedDifficulty(0),
// //       m_totalMoves(0),
// //       m_matchesFound(0),
// //       m_gameWon(false),
// //       m_gameBoard(nullptr),
// //       m_audioManager(nullptr),
// //       m_scoreManager(nullptr),
// //       m_gameStartTime(0.0f),
// //       m_currentTime(0.0f),
// //       m_pausedTime(0.0f)
// // {
// //     loadResources();
// // }

// // Game::~Game() {
// //     unloadResources();
// // }

// // // --------------------- Resource Management ---------------------

// // void Game::loadResources() {
// //     // Load fonts (use default if files don't exist)
// //     if (FileExists("assets/fonts/arial.ttf")) {
// //         m_titleFont = LoadFont("assets/fonts/arial.ttf");
// //         m_uiFont = LoadFont("assets/fonts/arial.ttf");
// //     } else {
// //         m_titleFont = GetFontDefault();
// //         m_uiFont = GetFontDefault();
// //     }

// //     // Load background (optional)
// //     if (FileExists("assets/textures/background.png")) {
// //         m_backgroundTexture = LoadTexture("assets/textures/background.png");
// //     }

// //     Utils::logInfo("Resources loaded successfully.");
// // }

// // void Game::unloadResources() {
// //     if (m_titleFont.texture.id != GetFontDefault().texture.id) {
// //         UnloadFont(m_titleFont);
// //     }
// //     if (m_uiFont.texture.id != GetFontDefault().texture.id) {
// //         UnloadFont(m_uiFont);
// //     }
// //     if (m_backgroundTexture.id != 0) {
// //         UnloadTexture(m_backgroundTexture);
// //     }

// //     Utils::logInfo("Resources unloaded successfully.");
// // }

// // // --------------------- Game State Updates ---------------------

// // void Game::update() {
// //     switch (m_currentState) {
// //         case GameState::MAIN_MENU: updateMainMenu(); break;
// //         case GameState::DIFFICULTY: updateDifficultySelection(); break;
// //         case GameState::PLAYING: updatePlaying(); break;
// //         case GameState::PAUSED: updatePaused(); break;
// //         case GameState::GAME_OVER: updateGameOver(); break;
// //         case GameState::SETTINGS: updateSettings(); break;
// //     }
// // }

// // void Game::updateMainMenu() {
// //     handleMainMenuInput();
// // }

// // void Game::updateDifficultySelection() {
// //     handleDifficultySelectionInput();
// // }

// // void Game::updatePlaying() {
// //     handlePlayingInput();

// //     if (m_gameBoard)
// //         m_gameBoard->update(GetFrameTime());

// //     checkWinCondition();
// // }

// // void Game::updatePaused() {
// //     handlePausedInput();
// // }

// // void Game::updateGameOver() {
// //     handleGameOverInput();
// // }

// // void Game::updateSettings() {
// //     handleSettingsInput();
// // }

// // // --------------------- Game Rendering ---------------------

// // void Game::draw() {
// //     // Draw animated gradient background
// //     drawGradientBackground();
    
// //     switch (m_currentState) {
// //         case GameState::MAIN_MENU: drawMainMenu(); break;
// //         case GameState::DIFFICULTY: drawDifficultySelection(); break;
// //         case GameState::PLAYING: drawPlaying(); break;
// //         case GameState::PAUSED: drawPaused(); break;
// //         case GameState::GAME_OVER: drawGameOver(); break;
// //         case GameState::SETTINGS: drawSettings(); break;
// //     }
// // }

// // void Game::drawGradientBackground() {
// //     // Animated gradient background
// //     float time = GetTime();
// //     Color topColor = Utils::colorFromHSV(fmod(time * 20, 360), 0.6f, 0.4f);
// //     Color bottomColor = Utils::colorFromHSV(fmod(time * 20 + 180, 360), 0.6f, 0.2f);
    
// //     DrawRectangleGradientV(0, 0, m_screenWidth, m_screenHeight, topColor, bottomColor);
    
// //     // Draw floating particles
// //     for (int i = 0; i < 20; ++i) {
// //         float x = fmod(i * 73.5f + time * 20, m_screenWidth);
// //         float y = fmod(i * 127.3f + time * 15, m_screenHeight);
// //         float size = 2 + sin(time + i) * 2;
// //         DrawCircle(x, y, size, ColorAlpha(WHITE, 0.3f));
// //     }
// // }

// // void Game::drawMainMenu() {
// //     // Draw title with shadow and glow
// //     const char* title = "MEMORY CARD GAME";
// //     int titleSize = 50;
// //     int titleWidth = MeasureText(title, titleSize);
// //     int titleX = m_screenWidth / 2 - titleWidth / 2;
// //     int titleY = 80;
    
// //     // Shadow
// //     DrawText(title, titleX + 4, titleY + 4, titleSize, ColorAlpha(BLACK, 0.5f));
// //     // Glow effect
// //     DrawText(title, titleX, titleY, titleSize, ColorAlpha(GOLD, 0.3f));
// //     // Main text
// //     DrawText(title, titleX, titleY, titleSize, GOLD);

// //     // Draw menu buttons with enhanced style
// //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// //         Rectangle buttonRect = { 
// //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// //             220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// //             BUTTON_WIDTH, 
// //             BUTTON_HEIGHT 
// //         };
// //         bool isSelected = m_selectedMenuItem == (int)i;
// //         drawEnhancedButton(m_mainMenuItems[i], buttonRect, isSelected);
// //     }
    
// //     // Draw credits at bottom
// //     const char* credits = "Created with Raylib - MSTC DA-IICT";
// //     int creditsWidth = MeasureText(credits, 16);
// //     DrawText(credits, m_screenWidth / 2 - creditsWidth / 2, m_screenHeight - 40, 16, ColorAlpha(WHITE, 0.6f));
// // }

// // void Game::drawDifficultySelection() {
// //     // Draw title
// //     const char* title = "SELECT DIFFICULTY";
// //     int titleSize = 40;
// //     int titleWidth = MeasureText(title, titleSize);
// //     DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 123, titleSize, ColorAlpha(BLACK, 0.5f));
// //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 120, titleSize, GOLD);

// //     // Draw difficulty buttons
// //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// //         Rectangle buttonRect = { 
// //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// //             BUTTON_WIDTH, 
// //             BUTTON_HEIGHT 
// //         };
// //         bool isSelected = m_selectedDifficulty == (int)i;
// //         drawEnhancedButton(m_difficultyNames[i], buttonRect, isSelected);
// //     }
    
// //     // Draw back hint with icon
// //     DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
// // }

// // void Game::drawPlaying() {
// //     // Draw game board
// //     if (m_gameBoard)
// //         m_gameBoard->draw();

// //     // Draw enhanced HUD
// //     drawEnhancedHUD();
// // }

// // void Game::drawEnhancedHUD() {
// //     // Top bar background with transparency
// //     DrawRectangle(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.7f));
// //     DrawRectangleGradientV(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.5f), ColorAlpha(BLACK, 0.2f));
    
// //     // Stats panel on left
// //     int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
// //     int totalPairs = (static_cast<int>(m_difficulty) / 2);
    
// //     // Moves counter
// //     DrawRectangleRounded({15, 15, 150, 50}, 0.3f, 8, ColorAlpha(DARKBLUE, 0.8f));
// //     DrawRectangleRoundedLines({15, 15, 150, 50}, 0.3f, 8, 2, SKYBLUE);
// //     std::string movesStr = "MOVES: " + std::to_string(m_totalMoves);
// //     DrawText(movesStr.c_str(), 30, 30, 24, WHITE);
    
// //     // Matches counter with progress
// //     DrawRectangleRounded({180, 15, 180, 50}, 0.3f, 8, ColorAlpha(DARKGREEN, 0.8f));
// //     DrawRectangleRoundedLines({180, 15, 180, 50}, 0.3f, 8, 2, LIME);
// //     std::string matchesStr = "PAIRS: " + std::to_string(matches) + "/" + std::to_string(totalPairs);
// //     DrawText(matchesStr.c_str(), 195, 30, 24, WHITE);
    
// //     // Progress bar
// //     float progress = totalPairs > 0 ? (float)matches / totalPairs : 0;
// //     DrawRectangleRounded({195, 53, 150 * progress, 8}, 0.5f, 8, LIME);
    
// //     // Timer on right
// //     float elapsed = getElapsedTime();
// //     std::string timerText = Utils::formatTime(elapsed);
// //     int timerWidth = MeasureText(timerText.c_str(), 28);
    
// //     DrawRectangleRounded({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, ColorAlpha(MAROON, 0.8f));
// //     DrawRectangleRoundedLines({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, 2, RED);
// //     DrawText("TIME", m_screenWidth - 155, 22, 16, LIGHTGRAY);
// //     DrawText(timerText.c_str(), m_screenWidth - 155, 38, 28, GOLD);
    
// //     // Bottom hint
// //     DrawText("P - Pause", m_screenWidth - 150, m_screenHeight - 35, 18, ColorAlpha(WHITE, 0.6f));
// // }

// // void Game::drawPaused() {
// //     // Draw game board dimmed
// //     if (m_gameBoard)
// //         m_gameBoard->draw();
    
// //     // Draw overlay
// //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.8f));
    
// //     // Pause panel
// //     Rectangle panel = {m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 200, 500, 400};
// //     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.95f));
// //     DrawRectangleRoundedLines(panel, 0.1f, 16, 4, SKYBLUE);
    
// //     // Pause text
// //     const char* pauseText = "PAUSED";
// //     int textSize = 60;
// //     int textWidth = MeasureText(pauseText, textSize);
// //     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2 + 3, m_screenHeight / 2 - 123, textSize, ColorAlpha(BLACK, 0.5f));
// //     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 120, textSize, GOLD);
    
// //     // Instructions with icons
// //     const char* resume = "SPACE - Resume";
// //     const char* menu = "M - Main Menu";
// //     int resumeWidth = MeasureText(resume, 24);
// //     int menuWidth = MeasureText(menu, 24);
    
// //     DrawText(resume, m_screenWidth / 2 - resumeWidth / 2, m_screenHeight / 2, 24, WHITE);
// //     DrawText(menu, m_screenWidth / 2 - menuWidth / 2, m_screenHeight / 2 + 40, 24, LIGHTGRAY);
// // }

// // void Game::drawGameOver() {
// //     // Draw game board in background
// //     if (m_gameBoard)
// //         m_gameBoard->draw();
    
// //     // Victory overlay
// //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.85f));
    
// //     // Victory panel
// //     Rectangle panel = {m_screenWidth / 2.0f - 300, m_screenHeight / 2.0f - 250, 600, 500};
// //     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKGREEN, 0.95f));
// //     DrawRectangleRoundedLines(panel, 0.1f, 16, 4, LIME);
    
// //     // Animated stars/particles
// //     float time = GetTime();
// //     for (int i = 0; i < 30; ++i) {
// //         float angle = (i * 12.0f + time * 50) * DEG2RAD;
// //         float dist = 150 + sin(time * 2 + i) * 30;
// //         float x = m_screenWidth / 2 + cos(angle) * dist;
// //         float y = m_screenHeight / 2 - 100 + sin(angle) * dist;
// //         float size = 3 + sin(time * 3 + i) * 2;
// //         DrawCircle(x, y, size, ColorAlpha(GOLD, 0.8f));
// //     }
    
// //     // Victory text with glow
// //     const char* winText = "VICTORY!";
// //     int textSize = 70;
// //     int textWidth = MeasureText(winText, textSize);
// //     DrawText(winText, m_screenWidth / 2 - textWidth / 2 + 4, m_screenHeight / 2 - 164, textSize, ColorAlpha(BLACK, 0.5f));
// //     DrawText(winText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 160, textSize, GOLD);
    
// //     // Stats boxes
// //     float elapsedTime = getElapsedTime();
// //     std::string timeStr = Utils::formatTime(elapsedTime);
// //     std::string movesStr = std::to_string(m_totalMoves);
// //     int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
// //     std::string matchesStr = std::to_string(matches);
    
// //     // Time stat
// //     DrawRectangleRounded({m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(MAROON, 0.8f));
// //     DrawText("TIME", m_screenWidth / 2 - 220, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
// //     DrawText(timeStr.c_str(), m_screenWidth / 2 - 220, m_screenHeight / 2 - 5, 32, GOLD);
    
// //     // Moves stat
// //     DrawRectangleRounded({m_screenWidth / 2.0f - 20, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(DARKBLUE, 0.8f));
// //     DrawText("MOVES", m_screenWidth / 2 + 10, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
// //     DrawText(movesStr.c_str(), m_screenWidth / 2 + 10, m_screenHeight / 2 - 5, 32, SKYBLUE);
    
// //     // Action buttons with hover detection
// //     Vector2 mousePos = GetMousePosition();
// //     Rectangle restartBtn = {m_screenWidth / 2.0f - 230, m_screenHeight / 2.0f + 80, 200, 60};
// //     Rectangle menuBtn = {m_screenWidth / 2.0f + 30, m_screenHeight / 2.0f + 80, 200, 60};
    
// //     bool restartHovered = CheckCollisionPointRec(mousePos, restartBtn);
// //     bool menuHovered = CheckCollisionPointRec(mousePos, menuBtn);
    
// //     drawEnhancedButton("RESTART (R)", restartBtn, restartHovered, DARKGREEN);
// //     drawEnhancedButton("MENU (ESC)", menuBtn, menuHovered, DARKBLUE);
// // }

// // void Game::drawSettings() {
// //     // Draw title
// //     const char* title = "SETTINGS";
// //     int titleSize = 40;
// //     int titleWidth = MeasureText(title, titleSize);
// //     DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 103, titleSize, ColorAlpha(BLACK, 0.5f));
// //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, titleSize, GOLD);
    
// //     // Settings panel
// //     Rectangle panel = {m_screenWidth / 2.0f - 300, 200, 600, 400};
// //     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.8f));
// //     DrawRectangleRoundedLines(panel, 0.1f, 16, 3, SKYBLUE);
    
// //     DrawText("Settings coming soon...", m_screenWidth / 2 - 120, m_screenHeight / 2, 24, LIGHTGRAY);
// //     DrawText("Volume Controls", m_screenWidth / 2 - 250, m_screenHeight / 2 + 60, 20, WHITE);
// //     DrawText("Sound Effects", m_screenWidth / 2 - 250, m_screenHeight / 2 + 100, 20, WHITE);
// //     DrawText("Music", m_screenWidth / 2 - 250, m_screenHeight / 2 + 140, 20, WHITE);
    
// //     // Draw back hint
// //     DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
// // }

// // void Game::drawEnhancedButton(const std::string& text, Rectangle bounds, bool isSelected, Color baseColor) {
// //     // Button animation
// //     float scale = isSelected ? 1.05f : 1.0f;
// //     Rectangle animBounds = {
// //         bounds.x - (bounds.width * scale - bounds.width) / 2,
// //         bounds.y - (bounds.height * scale - bounds.height) / 2,
// //         bounds.width * scale,
// //         bounds.height * scale
// //     };
    
// //     // Shadow
// //     DrawRectangleRounded({animBounds.x + 6, animBounds.y + 6, animBounds.width, animBounds.height}, 
// //                          0.2f, 16, ColorAlpha(BLACK, 0.5f));
    
// //     // Button background with gradient effect
// //     Color bgColor = isSelected ? Utils::adjustBrightness(baseColor, 1.3f) : baseColor;
// //     DrawRectangleRounded(animBounds, 0.2f, 16, bgColor);
    
// //     // Glow effect for selected
// //     if (isSelected) {
// //         DrawRectangleRoundedLines(animBounds, 0.2f, 16, 3, GOLD);
// //         DrawRectangleRoundedLines({animBounds.x - 2, animBounds.y - 2, animBounds.width + 4, animBounds.height + 4}, 
// //                                   0.2f, 16, 1, ColorAlpha(GOLD, 0.5f));
// //     } else {
// //         DrawRectangleRoundedLines(animBounds, 0.2f, 16, 2, Utils::adjustBrightness(baseColor, 1.5f));
// //     }
    
// //     // Text with shadow
// //     int textSize = 24;
// //     int textWidth = MeasureText(text.c_str(), textSize);
// //     int textX = animBounds.x + (animBounds.width - textWidth) / 2;
// //     int textY = animBounds.y + (animBounds.height - textSize) / 2;
    
// //     DrawText(text.c_str(), textX + 2, textY + 2, textSize, ColorAlpha(BLACK, 0.6f));
// //     DrawText(text.c_str(), textX, textY, textSize, isSelected ? GOLD : WHITE);
// // }

// // // --------------------- State Transitions ---------------------

// // void Game::changeState(GameState newState) {
// //     m_previousState = m_currentState;
// //     m_currentState = newState;
// // }

// // void Game::startNewGame(Difficulty difficulty) {
// //     m_difficulty = difficulty;

// //     int numCards = static_cast<int>(difficulty);
// //     int gridSize = static_cast<int>(sqrt(numCards));
// //     Vector2 cardSize = Utils::calculateOptimalCardSize(
// //         gridSize, gridSize,
// //         Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 200},
// //         15.0f
// //     );

// //     m_gameBoard = std::make_unique<GameBoard>(
// //         gridSize, gridSize, cardSize, 15.0f,
// //         Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 150}
// //     );

// //     m_audioManager = std::make_unique<AudioManager>();
// //     m_scoreManager = std::make_unique<ScoreManager>();
    
// //     // Connect AudioManager to GameBoard
// //     m_gameBoard->setAudioManager(m_audioManager.get());

// //     m_totalMoves = 0;
// //     m_matchesFound = 0;
// //     m_gameWon = false;
// //     m_gameStartTime = GetTime();
// // }

// // void Game::pauseGame() {
// //     m_pausedTime = GetTime();
// //     changeState(GameState::PAUSED);
// // }

// // void Game::resumeGame() {
// //     float pausedDuration = GetTime() - m_pausedTime;
// //     m_gameStartTime += pausedDuration;
// //     changeState(GameState::PLAYING);
// // }

// // void Game::restartGame() {
// //     startNewGame(m_difficulty);
// // }

// // void Game::returnToMainMenu() {
// //     changeState(GameState::MAIN_MENU);
// // }

// // // --------------------- Helpers ---------------------

// // void Game::checkWinCondition() {
// //     if (m_gameBoard && m_gameBoard->allMatched() && !m_gameWon) {
// //         m_gameWon = true;
// //         m_pausedTime = GetTime();
// //         m_matchesFound = m_gameBoard->getMatchesFound();
// //         changeState(GameState::GAME_OVER);
// //     }
// // }

// // float Game::getElapsedTime() const {
// //     if (m_gameWon) {
// //         return m_pausedTime - m_gameStartTime;
// //     }
// //     return GetTime() - m_gameStartTime;
// // }

// // void Game::drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color) {
// //     drawEnhancedButton(text, bounds, isSelected, color);
// // }

// // void Game::drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font) {
// //     int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
// //     DrawTextEx(font, text.c_str(), {m_screenWidth / 2.0f - textWidth / 2, (float)y}, fontSize, 1, color);
// // }

// // void Game::drawGameStats() {
// //     drawEnhancedHUD();
// // }

// // void Game::drawTimer() {
// //     // Handled in drawEnhancedHUD
// // }

// // // --------------------- Input Handling ---------------------

// // void Game::handleMainMenuInput() {
// //     Vector2 mousePos = GetMousePosition();
    
// //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// //         for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// //             Rectangle buttonRect = {
// //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// //                 220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// //                 BUTTON_WIDTH,
// //                 BUTTON_HEIGHT
// //             };
            
// //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// //                 switch (i) {
// //                     case 0: changeState(GameState::DIFFICULTY); break;
// //                     case 1: changeState(GameState::SETTINGS); break;
// //                     case 2: Utils::logInfo("High Scores clicked"); break;
// //                     case 3: CloseWindow(); break;
// //                 }
// //             }
// //         }
// //     }
    
// //     m_selectedMenuItem = -1;
// //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// //         Rectangle buttonRect = {
// //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// //             220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// //             BUTTON_WIDTH,
// //             BUTTON_HEIGHT
// //         };
        
// //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// //             m_selectedMenuItem = static_cast<int>(i);
// //         }
// //     }
    
// //     if (IsKeyPressed(KEY_DOWN)) {
// //         m_selectedMenuItem = (m_selectedMenuItem + 1) % m_mainMenuItems.size();
// //     }
// //     if (IsKeyPressed(KEY_UP)) {
// //         m_selectedMenuItem = (m_selectedMenuItem - 1 + m_mainMenuItems.size()) % m_mainMenuItems.size();
// //     }
// //     if (IsKeyPressed(KEY_ENTER)) {
// //         if (m_selectedMenuItem >= 0) {
// //             switch (m_selectedMenuItem) {
// //                 case 0: changeState(GameState::DIFFICULTY); break;
// //                 case 1: changeState(GameState::SETTINGS); break;
// //                 case 2: Utils::logInfo("High Scores clicked"); break;
// //                 case 3: CloseWindow(); break;
// //             }
// //         }
// //     }
// // }

// // void Game::handleDifficultySelectionInput() {
// //     Vector2 mousePos = GetMousePosition();
    
// //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// //         for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// //             Rectangle buttonRect = {
// //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// //                 250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// //                 BUTTON_WIDTH,
// //                 BUTTON_HEIGHT
// //             };
            
// //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// //                 switch (i) {
// //                     case 0: m_difficulty = Difficulty::EASY; break;
// //                     case 1: m_difficulty = Difficulty::MEDIUM; break;
// //                     case 2: m_difficulty = Difficulty::HARD; break;
// //                 }
// //                 startNewGame(m_difficulty);
// //                 changeState(GameState::PLAYING);
// //             }
// //         }
// //     }
    
// //     m_selectedDifficulty = -1;
// //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// //         Rectangle buttonRect = {
// //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// //             BUTTON_WIDTH,
// //             BUTTON_HEIGHT
// //         };
        
// //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// //             m_selectedDifficulty = static_cast<int>(i);
// //         }
// //     }
    
// //     if (IsKeyPressed(KEY_DOWN)) {
// //         m_selectedDifficulty = (m_selectedDifficulty + 1) % m_difficultyNames.size();
// //     }
// //     if (IsKeyPressed(KEY_UP)) {
// //         m_selectedDifficulty = (m_selectedDifficulty - 1 + m_difficultyNames.size()) % m_difficultyNames.size();
// //     }
// //     if (IsKeyPressed(KEY_ENTER)) {
// //         if (m_selectedDifficulty >= 0) {
// //             switch (m_selectedDifficulty) {
// //                 case 0: m_difficulty = Difficulty::EASY; break;
// //                 case 1: m_difficulty = Difficulty::MEDIUM; break;
// //                 case 2: m_difficulty = Difficulty::HARD; break;
// //             }
// //             startNewGame(m_difficulty);
// //             changeState(GameState::PLAYING);
// //         }
// //     }
// //     if (IsKeyPressed(KEY_ESCAPE)) {
// //         changeState(GameState::MAIN_MENU);
// //     }
// // }

// // void Game::handlePlayingInput() {
// //     // Pause game with P key only (not ESC to avoid accidental exits)
// //     if (IsKeyPressed(KEY_P)) {
// //         pauseGame();
// //         return;
// //     }
    
// //     // Handle card clicks
// //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_gameBoard) {
// //         Vector2 mousePos = GetMousePosition();
// //         m_gameBoard->handleClick(mousePos);
// //         m_totalMoves++;
// //     }
// // }

// // void Game::handlePausedInput() {
// //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)) {
// //         resumeGame();
// //     }
    
// //     if (IsKeyPressed(KEY_M)) {
// //         returnToMainMenu();
// //     }
// // }

// // void Game::handleGameOverInput() {
// //     // Handle keyboard input
// //     if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
// //         restartGame();
// //         changeState(GameState::PLAYING);
// //     }
    
// //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) {
// //         returnToMainMenu();
// //     }
    
// //     // Handle mouse clicks on buttons
// //     Vector2 mousePos = GetMousePosition();
// //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// //         Rectangle restartBtn = {m_screenWidth / 2.0f - 230, m_screenHeight / 2.0f + 80, 200, 60};
// //         Rectangle menuBtn = {m_screenWidth / 2.0f + 30, m_screenHeight / 2.0f + 80, 200, 60};
        
// //         if (CheckCollisionPointRec(mousePos, restartBtn)) {
// //             restartGame();
// //             changeState(GameState::PLAYING);
// //         } else if (CheckCollisionPointRec(mousePos, menuBtn)) {
// //             returnToMainMenu();
// //         }
// //     }
// // }

// // void Game::handleSettingsInput() {
// //     if (IsKeyPressed(KEY_ESCAPE)) {
// //         changeState(GameState::MAIN_MENU);
// //     }
// // }

// // // /**
// // //  * @file Game.cpp
// // //  * @brief Main Game class implementation with enhanced UI
// // //  */

// // // #include "../include/Game.h"
// // // #include "../include/Utils.h"
// // // #include <cmath>
// // // #include <iostream>

// // // // --------------------- Constructor & Destructor ---------------------

// // // Game::Game(int screenWidth, int screenHeight)
// // //     : m_screenWidth(screenWidth),
// // //       m_screenHeight(screenHeight),
// // //       m_currentState(GameState::MAIN_MENU),
// // //       m_previousState(GameState::MAIN_MENU),
// // //       m_difficulty(Difficulty::EASY),
// // //       m_selectedMenuItem(0),
// // //       m_selectedDifficulty(0),
// // //       m_totalMoves(0),
// // //       m_matchesFound(0),
// // //       m_gameWon(false),
// // //       m_gameBoard(nullptr),
// // //       m_audioManager(nullptr),
// // //       m_scoreManager(nullptr),
// // //       m_gameStartTime(0.0f),
// // //       m_currentTime(0.0f),
// // //       m_pausedTime(0.0f)
// // // {
// // //     loadResources();
// // // }

// // // Game::~Game() {
// // //     unloadResources();
// // // }

// // // // --------------------- Resource Management ---------------------

// // // void Game::loadResources() {
// // //     // Load fonts (use default if files don't exist)
// // //     if (FileExists("assets/fonts/arial.ttf")) {
// // //         m_titleFont = LoadFont("assets/fonts/arial.ttf");
// // //         m_uiFont = LoadFont("assets/fonts/arial.ttf");
// // //     } else {
// // //         m_titleFont = GetFontDefault();
// // //         m_uiFont = GetFontDefault();
// // //     }

// // //     // Load background (optional)
// // //     if (FileExists("assets/textures/background.png")) {
// // //         m_backgroundTexture = LoadTexture("assets/textures/background.png");
// // //     }

// // //     Utils::logInfo("Resources loaded successfully.");
// // // }

// // // void Game::unloadResources() {
// // //     if (m_titleFont.texture.id != GetFontDefault().texture.id) {
// // //         UnloadFont(m_titleFont);
// // //     }
// // //     if (m_uiFont.texture.id != GetFontDefault().texture.id) {
// // //         UnloadFont(m_uiFont);
// // //     }
// // //     if (m_backgroundTexture.id != 0) {
// // //         UnloadTexture(m_backgroundTexture);
// // //     }

// // //     Utils::logInfo("Resources unloaded successfully.");
// // // }

// // // // --------------------- Game State Updates ---------------------

// // // void Game::update() {
// // //     switch (m_currentState) {
// // //         case GameState::MAIN_MENU: updateMainMenu(); break;
// // //         case GameState::DIFFICULTY: updateDifficultySelection(); break;
// // //         case GameState::PLAYING: updatePlaying(); break;
// // //         case GameState::PAUSED: updatePaused(); break;
// // //         case GameState::GAME_OVER: updateGameOver(); break;
// // //         case GameState::SETTINGS: updateSettings(); break;
// // //     }
// // // }

// // // void Game::updateMainMenu() {
// // //     handleMainMenuInput();
// // // }

// // // void Game::updateDifficultySelection() {
// // //     handleDifficultySelectionInput();
// // // }

// // // void Game::updatePlaying() {
// // //     handlePlayingInput();

// // //     if (m_gameBoard)
// // //         m_gameBoard->update(GetFrameTime());

// // //     checkWinCondition();
// // // }

// // // void Game::updatePaused() {
// // //     handlePausedInput();
// // // }

// // // void Game::updateGameOver() {
// // //     handleGameOverInput();
// // // }

// // // void Game::updateSettings() {
// // //     handleSettingsInput();
// // // }

// // // // --------------------- Game Rendering ---------------------

// // // void Game::draw() {
// // //     // Draw animated gradient background
// // //     drawGradientBackground();
    
// // //     switch (m_currentState) {
// // //         case GameState::MAIN_MENU: drawMainMenu(); break;
// // //         case GameState::DIFFICULTY: drawDifficultySelection(); break;
// // //         case GameState::PLAYING: drawPlaying(); break;
// // //         case GameState::PAUSED: drawPaused(); break;
// // //         case GameState::GAME_OVER: drawGameOver(); break;
// // //         case GameState::SETTINGS: drawSettings(); break;
// // //     }
// // // }

// // // void Game::drawGradientBackground() {
// // //     // Animated gradient background
// // //     float time = GetTime();
// // //     Color topColor = Utils::colorFromHSV(fmod(time * 20, 360), 0.6f, 0.4f);
// // //     Color bottomColor = Utils::colorFromHSV(fmod(time * 20 + 180, 360), 0.6f, 0.2f);
    
// // //     DrawRectangleGradientV(0, 0, m_screenWidth, m_screenHeight, topColor, bottomColor);
    
// // //     // Draw floating particles
// // //     for (int i = 0; i < 20; ++i) {
// // //         float x = fmod(i * 73.5f + time * 20, m_screenWidth);
// // //         float y = fmod(i * 127.3f + time * 15, m_screenHeight);
// // //         float size = 2 + sin(time + i) * 2;
// // //         DrawCircle(x, y, size, ColorAlpha(WHITE, 0.3f));
// // //     }
// // // }

// // // void Game::drawMainMenu() {
// // //     // Draw title with shadow and glow
// // //     const char* title = "MEMORY CARD GAME";
// // //     int titleSize = 50;
// // //     int titleWidth = MeasureText(title, titleSize);
// // //     int titleX = m_screenWidth / 2 - titleWidth / 2;
// // //     int titleY = 80;
    
// // //     // Shadow
// // //     DrawText(title, titleX + 4, titleY + 4, titleSize, ColorAlpha(BLACK, 0.5f));
// // //     // Glow effect
// // //     DrawText(title, titleX, titleY, titleSize, ColorAlpha(GOLD, 0.3f));
// // //     // Main text
// // //     DrawText(title, titleX, titleY, titleSize, GOLD);

// // //     // Draw menu buttons with enhanced style
// // //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // //         Rectangle buttonRect = { 
// // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // //             220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // //             BUTTON_WIDTH, 
// // //             BUTTON_HEIGHT 
// // //         };
// // //         bool isSelected = m_selectedMenuItem == (int)i;
// // //         drawEnhancedButton(m_mainMenuItems[i], buttonRect, isSelected);
// // //     }
    
// // //     // Draw credits at bottom
// // //     const char* credits = "Created with Raylib - MSTC DA-IICT";
// // //     int creditsWidth = MeasureText(credits, 16);
// // //     DrawText(credits, m_screenWidth / 2 - creditsWidth / 2, m_screenHeight - 40, 16, ColorAlpha(WHITE, 0.6f));
// // // }

// // // void Game::drawDifficultySelection() {
// // //     // Draw title
// // //     const char* title = "SELECT DIFFICULTY";
// // //     int titleSize = 40;
// // //     int titleWidth = MeasureText(title, titleSize);
// // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 123, titleSize, ColorAlpha(BLACK, 0.5f));
// // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 120, titleSize, GOLD);

// // //     // Draw difficulty buttons
// // //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // //         Rectangle buttonRect = { 
// // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // //             BUTTON_WIDTH, 
// // //             BUTTON_HEIGHT 
// // //         };
// // //         bool isSelected = m_selectedDifficulty == (int)i;
// // //         drawEnhancedButton(m_difficultyNames[i], buttonRect, isSelected);
// // //     }
    
// // //     // Draw back hint with icon
// // //     DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
// // // }

// // // void Game::drawPlaying() {
// // //     // Draw game board
// // //     if (m_gameBoard)
// // //         m_gameBoard->draw();

// // //     // Draw enhanced HUD
// // //     drawEnhancedHUD();
// // // }

// // // void Game::drawEnhancedHUD() {
// // //     // Top bar background with transparency
// // //     DrawRectangle(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.7f));
// // //     DrawRectangleGradientV(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.5f), ColorAlpha(BLACK, 0.2f));
    
// // //     // Stats panel on left
// // //     int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
// // //     int totalPairs = (static_cast<int>(m_difficulty) / 2);
    
// // //     // Moves counter
// // //     DrawRectangleRounded({15, 15, 150, 50}, 0.3f, 8, ColorAlpha(DARKBLUE, 0.8f));
// // //     DrawRectangleRoundedLines({15, 15, 150, 50}, 0.3f, 8, 2, SKYBLUE);
// // //     std::string movesStr = "MOVES: " + std::to_string(m_totalMoves);
// // //     DrawText(movesStr.c_str(), 30, 30, 24, WHITE);
    
// // //     // Matches counter with progress
// // //     DrawRectangleRounded({180, 15, 180, 50}, 0.3f, 8, ColorAlpha(DARKGREEN, 0.8f));
// // //     DrawRectangleRoundedLines({180, 15, 180, 50}, 0.3f, 8, 2, LIME);
// // //     std::string matchesStr = "PAIRS: " + std::to_string(matches) + "/" + std::to_string(totalPairs);
// // //     DrawText(matchesStr.c_str(), 195, 30, 24, WHITE);
    
// // //     // Progress bar
// // //     float progress = totalPairs > 0 ? (float)matches / totalPairs : 0;
// // //     DrawRectangleRounded({195, 53, 150 * progress, 8}, 0.5f, 8, LIME);
    
// // //     // Timer on right
// // //     float elapsed = getElapsedTime();
// // //     std::string timerText = Utils::formatTime(elapsed);
// // //     int timerWidth = MeasureText(timerText.c_str(), 28);
    
// // //     DrawRectangleRounded({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, ColorAlpha(MAROON, 0.8f));
// // //     DrawRectangleRoundedLines({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, 2, RED);
// // //     DrawText("TIME", m_screenWidth - 155, 22, 16, LIGHTGRAY);
// // //     DrawText(timerText.c_str(), m_screenWidth - 155, 38, 28, GOLD);
    
// // //     // Bottom hint
// // //     DrawText("P - Pause", m_screenWidth - 150, m_screenHeight - 35, 18, ColorAlpha(WHITE, 0.6f));
// // // }

// // // void Game::drawPaused() {
// // //     // Draw game board dimmed
// // //     if (m_gameBoard)
// // //         m_gameBoard->draw();
    
// // //     // Draw overlay
// // //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.8f));
    
// // //     // Pause panel
// // //     Rectangle panel = {m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 200, 500, 400};
// // //     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.95f));
// // //     DrawRectangleRoundedLines(panel, 0.1f, 16, 4, SKYBLUE);
    
// // //     // Pause text
// // //     const char* pauseText = "PAUSED";
// // //     int textSize = 60;
// // //     int textWidth = MeasureText(pauseText, textSize);
// // //     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2 + 3, m_screenHeight / 2 - 123, textSize, ColorAlpha(BLACK, 0.5f));
// // //     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 120, textSize, GOLD);
    
// // //     // Instructions with icons
// // //     const char* resume = "SPACE - Resume";
// // //     const char* menu = "M - Main Menu";
// // //     int resumeWidth = MeasureText(resume, 24);
// // //     int menuWidth = MeasureText(menu, 24);
    
// // //     DrawText(resume, m_screenWidth / 2 - resumeWidth / 2, m_screenHeight / 2, 24, WHITE);
// // //     DrawText(menu, m_screenWidth / 2 - menuWidth / 2, m_screenHeight / 2 + 40, 24, LIGHTGRAY);
// // // }

// // // void Game::drawGameOver() {
// // //     // Draw game board in background
// // //     if (m_gameBoard)
// // //         m_gameBoard->draw();
    
// // //     // Victory overlay
// // //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.85f));
    
// // //     // Victory panel
// // //     Rectangle panel = {m_screenWidth / 2.0f - 300, m_screenHeight / 2.0f - 250, 600, 500};
// // //     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKGREEN, 0.95f));
// // //     DrawRectangleRoundedLines(panel, 0.1f, 16, 4, LIME);
    
// // //     // Animated stars/particles
// // //     float time = GetTime();
// // //     for (int i = 0; i < 30; ++i) {
// // //         float angle = (i * 12.0f + time * 50) * DEG2RAD;
// // //         float dist = 150 + sin(time * 2 + i) * 30;
// // //         float x = m_screenWidth / 2 + cos(angle) * dist;
// // //         float y = m_screenHeight / 2 - 100 + sin(angle) * dist;
// // //         float size = 3 + sin(time * 3 + i) * 2;
// // //         DrawCircle(x, y, size, ColorAlpha(GOLD, 0.8f));
// // //     }
    
// // //     // Victory text with glow
// // //     const char* winText = "VICTORY!";
// // //     int textSize = 70;
// // //     int textWidth = MeasureText(winText, textSize);
// // //     DrawText(winText, m_screenWidth / 2 - textWidth / 2 + 4, m_screenHeight / 2 - 164, textSize, ColorAlpha(BLACK, 0.5f));
// // //     DrawText(winText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 160, textSize, GOLD);
    
// // //     // Stats boxes
// // //     float elapsedTime = getElapsedTime();
// // //     std::string timeStr = Utils::formatTime(elapsedTime);
// // //     std::string movesStr = std::to_string(m_totalMoves);
// // //     int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
// // //     std::string matchesStr = std::to_string(matches);
    
// // //     // Time stat
// // //     DrawRectangleRounded({m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(MAROON, 0.8f));
// // //     DrawText("TIME", m_screenWidth / 2 - 220, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
// // //     DrawText(timeStr.c_str(), m_screenWidth / 2 - 220, m_screenHeight / 2 - 5, 32, GOLD);
    
// // //     // Moves stat
// // //     DrawRectangleRounded({m_screenWidth / 2.0f - 20, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(DARKBLUE, 0.8f));
// // //     DrawText("MOVES", m_screenWidth / 2 + 10, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
// // //     DrawText(movesStr.c_str(), m_screenWidth / 2 + 10, m_screenHeight / 2 - 5, 32, SKYBLUE);
    
// // //     // Action buttons with hover detection
// // //     Vector2 mousePos = GetMousePosition();
// // //     Rectangle restartBtn = {m_screenWidth / 2.0f - 230, m_screenHeight / 2.0f + 80, 200, 60};
// // //     Rectangle menuBtn = {m_screenWidth / 2.0f + 30, m_screenHeight / 2.0f + 80, 200, 60};
    
// // //     bool restartHovered = CheckCollisionPointRec(mousePos, restartBtn);
// // //     bool menuHovered = CheckCollisionPointRec(mousePos, menuBtn);
    
// // //     drawEnhancedButton("RESTART (R)", restartBtn, restartHovered, DARKGREEN);
// // //     drawEnhancedButton("MENU (ESC)", menuBtn, menuHovered, DARKBLUE);
// // // }

// // // void Game::drawSettings() {
// // //     // Draw title
// // //     const char* title = "SETTINGS";
// // //     int titleSize = 40;
// // //     int titleWidth = MeasureText(title, titleSize);
// // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 103, titleSize, ColorAlpha(BLACK, 0.5f));
// // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, titleSize, GOLD);
    
// // //     // Settings panel
// // //     Rectangle panel = {m_screenWidth / 2.0f - 300, 200, 600, 400};
// // //     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.8f));
// // //     DrawRectangleRoundedLines(panel, 0.1f, 16, 3, SKYBLUE);
    
// // //     DrawText("Settings coming soon...", m_screenWidth / 2 - 120, m_screenHeight / 2, 24, LIGHTGRAY);
// // //     DrawText("Volume Controls", m_screenWidth / 2 - 250, m_screenHeight / 2 + 60, 20, WHITE);
// // //     DrawText("Sound Effects", m_screenWidth / 2 - 250, m_screenHeight / 2 + 100, 20, WHITE);
// // //     DrawText("Music", m_screenWidth / 2 - 250, m_screenHeight / 2 + 140, 20, WHITE);
    
// // //     // Draw back hint
// // //     DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
// // // }

// // // void Game::drawEnhancedButton(const std::string& text, Rectangle bounds, bool isSelected, Color baseColor) {
// // //     // Button animation
// // //     float scale = isSelected ? 1.05f : 1.0f;
// // //     Rectangle animBounds = {
// // //         bounds.x - (bounds.width * scale - bounds.width) / 2,
// // //         bounds.y - (bounds.height * scale - bounds.height) / 2,
// // //         bounds.width * scale,
// // //         bounds.height * scale
// // //     };
    
// // //     // Shadow
// // //     DrawRectangleRounded({animBounds.x + 6, animBounds.y + 6, animBounds.width, animBounds.height}, 
// // //                          0.2f, 16, ColorAlpha(BLACK, 0.5f));
    
// // //     // Button background with gradient effect
// // //     Color bgColor = isSelected ? Utils::adjustBrightness(baseColor, 1.3f) : baseColor;
// // //     DrawRectangleRounded(animBounds, 0.2f, 16, bgColor);
    
// // //     // Glow effect for selected
// // //     if (isSelected) {
// // //         DrawRectangleRoundedLines(animBounds, 0.2f, 16, 3, GOLD);
// // //         DrawRectangleRoundedLines({animBounds.x - 2, animBounds.y - 2, animBounds.width + 4, animBounds.height + 4}, 
// // //                                   0.2f, 16, 1, ColorAlpha(GOLD, 0.5f));
// // //     } else {
// // //         DrawRectangleRoundedLines(animBounds, 0.2f, 16, 2, Utils::adjustBrightness(baseColor, 1.5f));
// // //     }
    
// // //     // Text with shadow
// // //     int textSize = 24;
// // //     int textWidth = MeasureText(text.c_str(), textSize);
// // //     int textX = animBounds.x + (animBounds.width - textWidth) / 2;
// // //     int textY = animBounds.y + (animBounds.height - textSize) / 2;
    
// // //     DrawText(text.c_str(), textX + 2, textY + 2, textSize, ColorAlpha(BLACK, 0.6f));
// // //     DrawText(text.c_str(), textX, textY, textSize, isSelected ? GOLD : WHITE);
// // // }

// // // // --------------------- State Transitions ---------------------

// // // void Game::changeState(GameState newState) {
// // //     m_previousState = m_currentState;
// // //     m_currentState = newState;
// // // }

// // // void Game::startNewGame(Difficulty difficulty) {
// // //     m_difficulty = difficulty;

// // //     int numCards = static_cast<int>(difficulty);
// // //     int gridSize = static_cast<int>(sqrt(numCards));
// // //     Vector2 cardSize = Utils::calculateOptimalCardSize(
// // //         gridSize, gridSize,
// // //         Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 200},
// // //         15.0f
// // //     );

// // //     m_gameBoard = std::make_unique<GameBoard>(
// // //         gridSize, gridSize, cardSize, 15.0f,
// // //         Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 150}
// // //     );

// // //     m_audioManager = std::make_unique<AudioManager>();
// // //     m_scoreManager = std::make_unique<ScoreManager>();

// // //     m_totalMoves = 0;
// // //     m_matchesFound = 0;
// // //     m_gameWon = false;
// // //     m_gameStartTime = GetTime();
// // // }

// // // void Game::pauseGame() {
// // //     m_pausedTime = GetTime();
// // //     changeState(GameState::PAUSED);
// // // }

// // // void Game::resumeGame() {
// // //     float pausedDuration = GetTime() - m_pausedTime;
// // //     m_gameStartTime += pausedDuration;
// // //     changeState(GameState::PLAYING);
// // // }

// // // void Game::restartGame() {
// // //     startNewGame(m_difficulty);
// // // }

// // // void Game::returnToMainMenu() {
// // //     changeState(GameState::MAIN_MENU);
// // // }

// // // // --------------------- Helpers ---------------------

// // // void Game::checkWinCondition() {
// // //     if (m_gameBoard && m_gameBoard->allMatched() && !m_gameWon) {
// // //         m_gameWon = true;
// // //         m_pausedTime = GetTime();
// // //         m_matchesFound = m_gameBoard->getMatchesFound();
// // //         changeState(GameState::GAME_OVER);
// // //     }
// // // }

// // // float Game::getElapsedTime() const {
// // //     if (m_gameWon) {
// // //         return m_pausedTime - m_gameStartTime;
// // //     }
// // //     return GetTime() - m_gameStartTime;
// // // }

// // // void Game::drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color) {
// // //     drawEnhancedButton(text, bounds, isSelected, color);
// // // }

// // // void Game::drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font) {
// // //     int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
// // //     DrawTextEx(font, text.c_str(), {m_screenWidth / 2.0f - textWidth / 2, (float)y}, fontSize, 1, color);
// // // }

// // // void Game::drawGameStats() {
// // //     drawEnhancedHUD();
// // // }

// // // void Game::drawTimer() {
// // //     // Handled in drawEnhancedHUD
// // // }

// // // // --------------------- Input Handling ---------------------

// // // void Game::handleMainMenuInput() {
// // //     Vector2 mousePos = GetMousePosition();
    
// // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// // //         for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // //             Rectangle buttonRect = {
// // //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // //                 220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // //                 BUTTON_WIDTH,
// // //                 BUTTON_HEIGHT
// // //             };
            
// // //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // //                 switch (i) {
// // //                     case 0: changeState(GameState::DIFFICULTY); break;
// // //                     case 1: changeState(GameState::SETTINGS); break;
// // //                     case 2: Utils::logInfo("High Scores clicked"); break;
// // //                     case 3: CloseWindow(); break;
// // //                 }
// // //             }
// // //         }
// // //     }
    
// // //     m_selectedMenuItem = -1;
// // //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // //         Rectangle buttonRect = {
// // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // //             220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // //             BUTTON_WIDTH,
// // //             BUTTON_HEIGHT
// // //         };
        
// // //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // //             m_selectedMenuItem = static_cast<int>(i);
// // //         }
// // //     }
    
// // //     if (IsKeyPressed(KEY_DOWN)) {
// // //         m_selectedMenuItem = (m_selectedMenuItem + 1) % m_mainMenuItems.size();
// // //     }
// // //     if (IsKeyPressed(KEY_UP)) {
// // //         m_selectedMenuItem = (m_selectedMenuItem - 1 + m_mainMenuItems.size()) % m_mainMenuItems.size();
// // //     }
// // //     if (IsKeyPressed(KEY_ENTER)) {
// // //         if (m_selectedMenuItem >= 0) {
// // //             switch (m_selectedMenuItem) {
// // //                 case 0: changeState(GameState::DIFFICULTY); break;
// // //                 case 1: changeState(GameState::SETTINGS); break;
// // //                 case 2: Utils::logInfo("High Scores clicked"); break;
// // //                 case 3: CloseWindow(); break;
// // //             }
// // //         }
// // //     }
// // // }

// // // void Game::handleDifficultySelectionInput() {
// // //     Vector2 mousePos = GetMousePosition();
    
// // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// // //         for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // //             Rectangle buttonRect = {
// // //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // //                 250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // //                 BUTTON_WIDTH,
// // //                 BUTTON_HEIGHT
// // //             };
            
// // //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // //                 switch (i) {
// // //                     case 0: m_difficulty = Difficulty::EASY; break;
// // //                     case 1: m_difficulty = Difficulty::MEDIUM; break;
// // //                     case 2: m_difficulty = Difficulty::HARD; break;
// // //                 }
// // //                 startNewGame(m_difficulty);
// // //                 changeState(GameState::PLAYING);
// // //             }
// // //         }
// // //     }
    
// // //     m_selectedDifficulty = -1;
// // //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // //         Rectangle buttonRect = {
// // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // //             BUTTON_WIDTH,
// // //             BUTTON_HEIGHT
// // //         };
        
// // //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // //             m_selectedDifficulty = static_cast<int>(i);
// // //         }
// // //     }
    
// // //     if (IsKeyPressed(KEY_DOWN)) {
// // //         m_selectedDifficulty = (m_selectedDifficulty + 1) % m_difficultyNames.size();
// // //     }
// // //     if (IsKeyPressed(KEY_UP)) {
// // //         m_selectedDifficulty = (m_selectedDifficulty - 1 + m_difficultyNames.size()) % m_difficultyNames.size();
// // //     }
// // //     if (IsKeyPressed(KEY_ENTER)) {
// // //         if (m_selectedDifficulty >= 0) {
// // //             switch (m_selectedDifficulty) {
// // //                 case 0: m_difficulty = Difficulty::EASY; break;
// // //                 case 1: m_difficulty = Difficulty::MEDIUM; break;
// // //                 case 2: m_difficulty = Difficulty::HARD; break;
// // //             }
// // //             startNewGame(m_difficulty);
// // //             changeState(GameState::PLAYING);
// // //         }
// // //     }
// // //     if (IsKeyPressed(KEY_ESCAPE)) {
// // //         changeState(GameState::MAIN_MENU);
// // //     }
// // // }

// // // void Game::handlePlayingInput() {
// // //     // Pause game with P key only (not ESC to avoid accidental exits)
// // //     if (IsKeyPressed(KEY_P)) {
// // //         pauseGame();
// // //         return;
// // //     }
    
// // //     // Handle card clicks
// // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_gameBoard) {
// // //         Vector2 mousePos = GetMousePosition();
// // //         m_gameBoard->handleClick(mousePos);
// // //         m_totalMoves++;
// // //     }
// // // }

// // // void Game::handlePausedInput() {
// // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)) {
// // //         resumeGame();
// // //     }
    
// // //     if (IsKeyPressed(KEY_M)) {
// // //         returnToMainMenu();
// // //     }
// // // }

// // // void Game::handleGameOverInput() {
// // //     // Handle keyboard input
// // //     if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
// // //         restartGame();
// // //         changeState(GameState::PLAYING);
// // //     }
    
// // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) {
// // //         returnToMainMenu();
// // //     }
    
// // //     // Handle mouse clicks on buttons
// // //     Vector2 mousePos = GetMousePosition();
// // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// // //         Rectangle restartBtn = {m_screenWidth / 2.0f - 230, m_screenHeight / 2.0f + 80, 200, 60};
// // //         Rectangle menuBtn = {m_screenWidth / 2.0f + 30, m_screenHeight / 2.0f + 80, 200, 60};
        
// // //         if (CheckCollisionPointRec(mousePos, restartBtn)) {
// // //             restartGame();
// // //             changeState(GameState::PLAYING);
// // //         } else if (CheckCollisionPointRec(mousePos, menuBtn)) {
// // //             returnToMainMenu();
// // //         }
// // //     }
// // // }

// // // void Game::handleSettingsInput() {
// // //     if (IsKeyPressed(KEY_ESCAPE)) {
// // //         changeState(GameState::MAIN_MENU);
// // //     }
// // // }

// // // // /**
// // // //  * @file Game.cpp
// // // //  * @brief Main Game class implementation with enhanced UI
// // // //  */

// // // // #include "../include/Game.h"
// // // // #include "../include/Utils.h"
// // // // #include <cmath>
// // // // #include <iostream>

// // // // // --------------------- Constructor & Destructor ---------------------

// // // // Game::Game(int screenWidth, int screenHeight)
// // // //     : m_screenWidth(screenWidth),
// // // //       m_screenHeight(screenHeight),
// // // //       m_currentState(GameState::MAIN_MENU),
// // // //       m_previousState(GameState::MAIN_MENU),
// // // //       m_difficulty(Difficulty::EASY),
// // // //       m_selectedMenuItem(0),
// // // //       m_selectedDifficulty(0),
// // // //       m_totalMoves(0),
// // // //       m_matchesFound(0),
// // // //       m_gameWon(false),
// // // //       m_gameBoard(nullptr),
// // // //       m_audioManager(nullptr),
// // // //       m_scoreManager(nullptr),
// // // //       m_gameStartTime(0.0f),
// // // //       m_currentTime(0.0f),
// // // //       m_pausedTime(0.0f)
// // // // {
// // // //     loadResources();
// // // // }

// // // // Game::~Game() {
// // // //     unloadResources();
// // // // }

// // // // // --------------------- Resource Management ---------------------

// // // // void Game::loadResources() {
// // // //     // Load fonts (use default if files don't exist)
// // // //     if (FileExists("assets/fonts/arial.ttf")) {
// // // //         m_titleFont = LoadFont("assets/fonts/arial.ttf");
// // // //         m_uiFont = LoadFont("assets/fonts/arial.ttf");
// // // //     } else {
// // // //         m_titleFont = GetFontDefault();
// // // //         m_uiFont = GetFontDefault();
// // // //     }

// // // //     // Load background (optional)
// // // //     if (FileExists("assets/textures/background.png")) {
// // // //         m_backgroundTexture = LoadTexture("assets/textures/background.png");
// // // //     }

// // // //     Utils::logInfo("Resources loaded successfully.");
// // // // }

// // // // void Game::unloadResources() {
// // // //     if (m_titleFont.texture.id != GetFontDefault().texture.id) {
// // // //         UnloadFont(m_titleFont);
// // // //     }
// // // //     if (m_uiFont.texture.id != GetFontDefault().texture.id) {
// // // //         UnloadFont(m_uiFont);
// // // //     }
// // // //     if (m_backgroundTexture.id != 0) {
// // // //         UnloadTexture(m_backgroundTexture);
// // // //     }

// // // //     Utils::logInfo("Resources unloaded successfully.");
// // // // }

// // // // // --------------------- Game State Updates ---------------------

// // // // void Game::update() {
// // // //     switch (m_currentState) {
// // // //         case GameState::MAIN_MENU: updateMainMenu(); break;
// // // //         case GameState::DIFFICULTY: updateDifficultySelection(); break;
// // // //         case GameState::PLAYING: updatePlaying(); break;
// // // //         case GameState::PAUSED: updatePaused(); break;
// // // //         case GameState::GAME_OVER: updateGameOver(); break;
// // // //         case GameState::SETTINGS: updateSettings(); break;
// // // //     }
// // // // }

// // // // void Game::updateMainMenu() {
// // // //     handleMainMenuInput();
// // // // }

// // // // void Game::updateDifficultySelection() {
// // // //     handleDifficultySelectionInput();
// // // // }

// // // // void Game::updatePlaying() {
// // // //     handlePlayingInput();

// // // //     if (m_gameBoard)
// // // //         m_gameBoard->update(GetFrameTime());

// // // //     checkWinCondition();
// // // // }

// // // // void Game::updatePaused() {
// // // //     handlePausedInput();
// // // // }

// // // // void Game::updateGameOver() {
// // // //     handleGameOverInput();
// // // // }

// // // // void Game::updateSettings() {
// // // //     handleSettingsInput();
// // // // }

// // // // // --------------------- Game Rendering ---------------------

// // // // void Game::draw() {
// // // //     // Draw animated gradient background
// // // //     drawGradientBackground();
    
// // // //     switch (m_currentState) {
// // // //         case GameState::MAIN_MENU: drawMainMenu(); break;
// // // //         case GameState::DIFFICULTY: drawDifficultySelection(); break;
// // // //         case GameState::PLAYING: drawPlaying(); break;
// // // //         case GameState::PAUSED: drawPaused(); break;
// // // //         case GameState::GAME_OVER: drawGameOver(); break;
// // // //         case GameState::SETTINGS: drawSettings(); break;
// // // //     }
// // // // }

// // // // void Game::drawGradientBackground() {
// // // //     // Animated gradient background
// // // //     float time = GetTime();
// // // //     Color topColor = Utils::colorFromHSV(fmod(time * 20, 360), 0.6f, 0.4f);
// // // //     Color bottomColor = Utils::colorFromHSV(fmod(time * 20 + 180, 360), 0.6f, 0.2f);
    
// // // //     DrawRectangleGradientV(0, 0, m_screenWidth, m_screenHeight, topColor, bottomColor);
    
// // // //     // Draw floating particles
// // // //     for (int i = 0; i < 20; ++i) {
// // // //         float x = fmod(i * 73.5f + time * 20, m_screenWidth);
// // // //         float y = fmod(i * 127.3f + time * 15, m_screenHeight);
// // // //         float size = 2 + sin(time + i) * 2;
// // // //         DrawCircle(x, y, size, ColorAlpha(WHITE, 0.3f));
// // // //     }
// // // // }

// // // // void Game::drawMainMenu() {
// // // //     // Draw title with shadow and glow
// // // //     const char* title = "MEMORY CARD GAME";
// // // //     int titleSize = 50;
// // // //     int titleWidth = MeasureText(title, titleSize);
// // // //     int titleX = m_screenWidth / 2 - titleWidth / 2;
// // // //     int titleY = 80;
    
// // // //     // Shadow
// // // //     DrawText(title, titleX + 4, titleY + 4, titleSize, ColorAlpha(BLACK, 0.5f));
// // // //     // Glow effect
// // // //     DrawText(title, titleX, titleY, titleSize, ColorAlpha(GOLD, 0.3f));
// // // //     // Main text
// // // //     DrawText(title, titleX, titleY, titleSize, GOLD);

// // // //     // Draw menu buttons with enhanced style
// // // //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // // //         Rectangle buttonRect = { 
// // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // //             220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // //             BUTTON_WIDTH, 
// // // //             BUTTON_HEIGHT 
// // // //         };
// // // //         bool isSelected = m_selectedMenuItem == (int)i;
// // // //         drawEnhancedButton(m_mainMenuItems[i], buttonRect, isSelected);
// // // //     }
    
// // // //     // Draw credits at bottom
// // // //     const char* credits = "Created with Raylib - MSTC DA-IICT";
// // // //     int creditsWidth = MeasureText(credits, 16);
// // // //     DrawText(credits, m_screenWidth / 2 - creditsWidth / 2, m_screenHeight - 40, 16, ColorAlpha(WHITE, 0.6f));
// // // // }

// // // // void Game::drawDifficultySelection() {
// // // //     // Draw title
// // // //     const char* title = "SELECT DIFFICULTY";
// // // //     int titleSize = 40;
// // // //     int titleWidth = MeasureText(title, titleSize);
// // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 123, titleSize, ColorAlpha(BLACK, 0.5f));
// // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 120, titleSize, GOLD);

// // // //     // Draw difficulty buttons
// // // //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // // //         Rectangle buttonRect = { 
// // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // //             BUTTON_WIDTH, 
// // // //             BUTTON_HEIGHT 
// // // //         };
// // // //         bool isSelected = m_selectedDifficulty == (int)i;
// // // //         drawEnhancedButton(m_difficultyNames[i], buttonRect, isSelected);
// // // //     }
    
// // // //     // Draw back hint with icon
// // // //     DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
// // // // }

// // // // void Game::drawPlaying() {
// // // //     // Draw game board
// // // //     if (m_gameBoard)
// // // //         m_gameBoard->draw();

// // // //     // Draw enhanced HUD
// // // //     drawEnhancedHUD();
// // // // }

// // // // void Game::drawEnhancedHUD() {
// // // //     // Top bar background with transparency
// // // //     DrawRectangle(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.7f));
// // // //     DrawRectangleGradientV(0, 0, m_screenWidth, 80, ColorAlpha(BLACK, 0.5f), ColorAlpha(BLACK, 0.2f));
    
// // // //     // Stats panel on left
// // // //     int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
// // // //     int totalPairs = (static_cast<int>(m_difficulty) / 2);
    
// // // //     // Moves counter
// // // //     DrawRectangleRounded({15, 15, 150, 50}, 0.3f, 8, ColorAlpha(DARKBLUE, 0.8f));
// // // //     DrawRectangleRoundedLines({15, 15, 150, 50}, 0.3f, 8, 2, SKYBLUE);
// // // //     std::string movesStr = "MOVES: " + std::to_string(m_totalMoves);
// // // //     DrawText(movesStr.c_str(), 30, 30, 24, WHITE);
    
// // // //     // Matches counter with progress
// // // //     DrawRectangleRounded({180, 15, 180, 50}, 0.3f, 8, ColorAlpha(DARKGREEN, 0.8f));
// // // //     DrawRectangleRoundedLines({180, 15, 180, 50}, 0.3f, 8, 2, LIME);
// // // //     std::string matchesStr = "PAIRS: " + std::to_string(matches) + "/" + std::to_string(totalPairs);
// // // //     DrawText(matchesStr.c_str(), 195, 30, 24, WHITE);
    
// // // //     // Progress bar
// // // //     float progress = totalPairs > 0 ? (float)matches / totalPairs : 0;
// // // //     DrawRectangleRounded({195, 53, 150 * progress, 8}, 0.5f, 8, LIME);
    
// // // //     // Timer on right
// // // //     float elapsed = getElapsedTime();
// // // //     std::string timerText = Utils::formatTime(elapsed);
// // // //     int timerWidth = MeasureText(timerText.c_str(), 28);
    
// // // //     DrawRectangleRounded({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, ColorAlpha(MAROON, 0.8f));
// // // //     DrawRectangleRoundedLines({m_screenWidth - 170.0f, 15, 155, 50}, 0.3f, 8, 2, RED);
// // // //     DrawText("TIME", m_screenWidth - 155, 22, 16, LIGHTGRAY);
// // // //     DrawText(timerText.c_str(), m_screenWidth - 155, 38, 28, GOLD);
    
// // // //     // Bottom hint
// // // //     DrawText("ESC - Pause", m_screenWidth - 150, m_screenHeight - 35, 18, ColorAlpha(WHITE, 0.6f));
// // // // }

// // // // void Game::drawPaused() {
// // // //     // Draw game board dimmed
// // // //     if (m_gameBoard)
// // // //         m_gameBoard->draw();
    
// // // //     // Draw overlay
// // // //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.8f));
    
// // // //     // Pause panel
// // // //     Rectangle panel = {m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 200, 500, 400};
// // // //     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.95f));
// // // //     DrawRectangleRoundedLines(panel, 0.1f, 16, 4, SKYBLUE);
    
// // // //     // Pause text
// // // //     const char* pauseText = "PAUSED";
// // // //     int textSize = 60;
// // // //     int textWidth = MeasureText(pauseText, textSize);
// // // //     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2 + 3, m_screenHeight / 2 - 123, textSize, ColorAlpha(BLACK, 0.5f));
// // // //     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 120, textSize, GOLD);
    
// // // //     // Instructions with icons
// // // //     const char* resume = "SPACE - Resume";
// // // //     const char* menu = "M - Main Menu";
// // // //     int resumeWidth = MeasureText(resume, 24);
// // // //     int menuWidth = MeasureText(menu, 24);
    
// // // //     DrawText(resume, m_screenWidth / 2 - resumeWidth / 2, m_screenHeight / 2, 24, WHITE);
// // // //     DrawText(menu, m_screenWidth / 2 - menuWidth / 2, m_screenHeight / 2 + 40, 24, LIGHTGRAY);
// // // // }

// // // // void Game::drawGameOver() {
// // // //     // Draw game board in background
// // // //     if (m_gameBoard)
// // // //         m_gameBoard->draw();
    
// // // //     // Victory overlay
// // // //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.85f));
    
// // // //     // Victory panel
// // // //     Rectangle panel = {m_screenWidth / 2.0f - 300, m_screenHeight / 2.0f - 250, 600, 500};
// // // //     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKGREEN, 0.95f));
// // // //     DrawRectangleRoundedLines(panel, 0.1f, 16, 4, LIME);
    
// // // //     // Animated stars/particles
// // // //     float time = GetTime();
// // // //     for (int i = 0; i < 30; ++i) {
// // // //         float angle = (i * 12.0f + time * 50) * DEG2RAD;
// // // //         float dist = 150 + sin(time * 2 + i) * 30;
// // // //         float x = m_screenWidth / 2 + cos(angle) * dist;
// // // //         float y = m_screenHeight / 2 - 100 + sin(angle) * dist;
// // // //         float size = 3 + sin(time * 3 + i) * 2;
// // // //         DrawCircle(x, y, size, ColorAlpha(GOLD, 0.8f));
// // // //     }
    
// // // //     // Victory text with glow
// // // //     const char* winText = "VICTORY!";
// // // //     int textSize = 70;
// // // //     int textWidth = MeasureText(winText, textSize);
// // // //     DrawText(winText, m_screenWidth / 2 - textWidth / 2 + 4, m_screenHeight / 2 - 164, textSize, ColorAlpha(BLACK, 0.5f));
// // // //     DrawText(winText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 160, textSize, GOLD);
    
// // // //     // Stats boxes
// // // //     float elapsedTime = getElapsedTime();
// // // //     std::string timeStr = Utils::formatTime(elapsedTime);
// // // //     std::string movesStr = std::to_string(m_totalMoves);
// // // //     int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
// // // //     std::string matchesStr = std::to_string(matches);
    
// // // //     // Time stat
// // // //     DrawRectangleRounded({m_screenWidth / 2.0f - 250, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(MAROON, 0.8f));
// // // //     DrawText("TIME", m_screenWidth / 2 - 220, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
// // // //     DrawText(timeStr.c_str(), m_screenWidth / 2 - 220, m_screenHeight / 2 - 5, 32, GOLD);
    
// // // //     // Moves stat
// // // //     DrawRectangleRounded({m_screenWidth / 2.0f - 20, m_screenHeight / 2.0f - 50, 200, 80}, 0.2f, 8, ColorAlpha(DARKBLUE, 0.8f));
// // // //     DrawText("MOVES", m_screenWidth / 2 + 10, m_screenHeight / 2 - 35, 20, LIGHTGRAY);
// // // //     DrawText(movesStr.c_str(), m_screenWidth / 2 + 10, m_screenHeight / 2 - 5, 32, SKYBLUE);
    
// // // //     // Action buttons
// // // //     Rectangle restartBtn = {m_screenWidth / 2.0f - 230, m_screenHeight / 2.0f + 80, 200, 60};
// // // //     Rectangle menuBtn = {m_screenWidth / 2.0f + 30, m_screenHeight / 2.0f + 80, 200, 60};
    
// // // //     drawEnhancedButton("RESTART (R)", restartBtn, false, DARKGREEN);
// // // //     drawEnhancedButton("MENU (ESC)", menuBtn, false, DARKBLUE);
// // // // }

// // // // void Game::drawSettings() {
// // // //     // Draw title
// // // //     const char* title = "SETTINGS";
// // // //     int titleSize = 40;
// // // //     int titleWidth = MeasureText(title, titleSize);
// // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2 + 3, 103, titleSize, ColorAlpha(BLACK, 0.5f));
// // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, titleSize, GOLD);
    
// // // //     // Settings panel
// // // //     Rectangle panel = {m_screenWidth / 2.0f - 300, 200, 600, 400};
// // // //     DrawRectangleRounded(panel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.8f));
// // // //     DrawRectangleRoundedLines(panel, 0.1f, 16, 3, SKYBLUE);
    
// // // //     DrawText("Settings coming soon...", m_screenWidth / 2 - 120, m_screenHeight / 2, 24, LIGHTGRAY);
// // // //     DrawText("Volume Controls", m_screenWidth / 2 - 250, m_screenHeight / 2 + 60, 20, WHITE);
// // // //     DrawText("Sound Effects", m_screenWidth / 2 - 250, m_screenHeight / 2 + 100, 20, WHITE);
// // // //     DrawText("Music", m_screenWidth / 2 - 250, m_screenHeight / 2 + 140, 20, WHITE);
    
// // // //     // Draw back hint
// // // //     DrawText("< ESC", 20, m_screenHeight - 40, 24, ColorAlpha(WHITE, 0.8f));
// // // // }

// // // // void Game::drawEnhancedButton(const std::string& text, Rectangle bounds, bool isSelected, Color baseColor) {
// // // //     // Button animation
// // // //     float scale = isSelected ? 1.05f : 1.0f;
// // // //     Rectangle animBounds = {
// // // //         bounds.x - (bounds.width * scale - bounds.width) / 2,
// // // //         bounds.y - (bounds.height * scale - bounds.height) / 2,
// // // //         bounds.width * scale,
// // // //         bounds.height * scale
// // // //     };
    
// // // //     // Shadow
// // // //     DrawRectangleRounded({animBounds.x + 6, animBounds.y + 6, animBounds.width, animBounds.height}, 
// // // //                          0.2f, 16, ColorAlpha(BLACK, 0.5f));
    
// // // //     // Button background with gradient effect
// // // //     Color bgColor = isSelected ? Utils::adjustBrightness(baseColor, 1.3f) : baseColor;
// // // //     DrawRectangleRounded(animBounds, 0.2f, 16, bgColor);
    
// // // //     // Glow effect for selected
// // // //     if (isSelected) {
// // // //         DrawRectangleRoundedLines(animBounds, 0.2f, 16, 3, GOLD);
// // // //         DrawRectangleRoundedLines({animBounds.x - 2, animBounds.y - 2, animBounds.width + 4, animBounds.height + 4}, 
// // // //                                   0.2f, 16, 1, ColorAlpha(GOLD, 0.5f));
// // // //     } else {
// // // //         DrawRectangleRoundedLines(animBounds, 0.2f, 16, 2, Utils::adjustBrightness(baseColor, 1.5f));
// // // //     }
    
// // // //     // Text with shadow
// // // //     int textSize = 24;
// // // //     int textWidth = MeasureText(text.c_str(), textSize);
// // // //     int textX = animBounds.x + (animBounds.width - textWidth) / 2;
// // // //     int textY = animBounds.y + (animBounds.height - textSize) / 2;
    
// // // //     DrawText(text.c_str(), textX + 2, textY + 2, textSize, ColorAlpha(BLACK, 0.6f));
// // // //     DrawText(text.c_str(), textX, textY, textSize, isSelected ? GOLD : WHITE);
// // // // }

// // // // // --------------------- State Transitions ---------------------

// // // // void Game::changeState(GameState newState) {
// // // //     m_previousState = m_currentState;
// // // //     m_currentState = newState;
// // // // }

// // // // void Game::startNewGame(Difficulty difficulty) {
// // // //     m_difficulty = difficulty;

// // // //     int numCards = static_cast<int>(difficulty);
// // // //     int gridSize = static_cast<int>(sqrt(numCards));
// // // //     Vector2 cardSize = Utils::calculateOptimalCardSize(
// // // //         gridSize, gridSize,
// // // //         Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 200},
// // // //         15.0f
// // // //     );

// // // //     m_gameBoard = std::make_unique<GameBoard>(
// // // //         gridSize, gridSize, cardSize, 15.0f,
// // // //         Rectangle{0, 100, (float)m_screenWidth, (float)m_screenHeight - 150}
// // // //     );

// // // //     m_audioManager = std::make_unique<AudioManager>();
// // // //     m_scoreManager = std::make_unique<ScoreManager>();

// // // //     m_totalMoves = 0;
// // // //     m_matchesFound = 0;
// // // //     m_gameWon = false;
// // // //     m_gameStartTime = GetTime();
// // // // }

// // // // void Game::pauseGame() {
// // // //     m_pausedTime = GetTime();
// // // //     changeState(GameState::PAUSED);
// // // // }

// // // // void Game::resumeGame() {
// // // //     float pausedDuration = GetTime() - m_pausedTime;
// // // //     m_gameStartTime += pausedDuration;
// // // //     changeState(GameState::PLAYING);
// // // // }

// // // // void Game::restartGame() {
// // // //     startNewGame(m_difficulty);
// // // // }

// // // // void Game::returnToMainMenu() {
// // // //     changeState(GameState::MAIN_MENU);
// // // // }

// // // // // --------------------- Helpers ---------------------

// // // // void Game::checkWinCondition() {
// // // //     if (m_gameBoard && m_gameBoard->allMatched() && !m_gameWon) {
// // // //         m_gameWon = true;
// // // //         m_pausedTime = GetTime();
// // // //         m_matchesFound = m_gameBoard->getMatchesFound();
// // // //         changeState(GameState::GAME_OVER);
// // // //     }
// // // // }

// // // // float Game::getElapsedTime() const {
// // // //     if (m_gameWon) {
// // // //         return m_pausedTime - m_gameStartTime;
// // // //     }
// // // //     return GetTime() - m_gameStartTime;
// // // // }

// // // // void Game::drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color) {
// // // //     drawEnhancedButton(text, bounds, isSelected, color);
// // // // }

// // // // void Game::drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font) {
// // // //     int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
// // // //     DrawTextEx(font, text.c_str(), {m_screenWidth / 2.0f - textWidth / 2, (float)y}, fontSize, 1, color);
// // // // }

// // // // void Game::drawGameStats() {
// // // //     drawEnhancedHUD();
// // // // }

// // // // void Game::drawTimer() {
// // // //     // Handled in drawEnhancedHUD
// // // // }

// // // // // --------------------- Input Handling ---------------------

// // // // void Game::handleMainMenuInput() {
// // // //     Vector2 mousePos = GetMousePosition();
    
// // // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// // // //         for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // // //             Rectangle buttonRect = {
// // // //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // //                 220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // //                 BUTTON_WIDTH,
// // // //                 BUTTON_HEIGHT
// // // //             };
            
// // // //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // //                 switch (i) {
// // // //                     case 0: changeState(GameState::DIFFICULTY); break;
// // // //                     case 1: changeState(GameState::SETTINGS); break;
// // // //                     case 2: Utils::logInfo("High Scores clicked"); break;
// // // //                     case 3: CloseWindow(); break;
// // // //                 }
// // // //             }
// // // //         }
// // // //     }
    
// // // //     m_selectedMenuItem = -1;
// // // //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // // //         Rectangle buttonRect = {
// // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // //             220.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // //             BUTTON_WIDTH,
// // // //             BUTTON_HEIGHT
// // // //         };
        
// // // //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // //             m_selectedMenuItem = static_cast<int>(i);
// // // //         }
// // // //     }
    
// // // //     if (IsKeyPressed(KEY_DOWN)) {
// // // //         m_selectedMenuItem = (m_selectedMenuItem + 1) % m_mainMenuItems.size();
// // // //     }
// // // //     if (IsKeyPressed(KEY_UP)) {
// // // //         m_selectedMenuItem = (m_selectedMenuItem - 1 + m_mainMenuItems.size()) % m_mainMenuItems.size();
// // // //     }
// // // //     if (IsKeyPressed(KEY_ENTER)) {
// // // //         if (m_selectedMenuItem >= 0) {
// // // //             switch (m_selectedMenuItem) {
// // // //                 case 0: changeState(GameState::DIFFICULTY); break;
// // // //                 case 1: changeState(GameState::SETTINGS); break;
// // // //                 case 2: Utils::logInfo("High Scores clicked"); break;
// // // //                 case 3: CloseWindow(); break;
// // // //             }
// // // //         }
// // // //     }
// // // // }

// // // // void Game::handleDifficultySelectionInput() {
// // // //     Vector2 mousePos = GetMousePosition();
    
// // // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// // // //         for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // // //             Rectangle buttonRect = {
// // // //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // //                 250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // //                 BUTTON_WIDTH,
// // // //                 BUTTON_HEIGHT
// // // //             };
            
// // // //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // //                 switch (i) {
// // // //                     case 0: m_difficulty = Difficulty::EASY; break;
// // // //                     case 1: m_difficulty = Difficulty::MEDIUM; break;
// // // //                     case 2: m_difficulty = Difficulty::HARD; break;
// // // //                 }
// // // //                 startNewGame(m_difficulty);
// // // //                 changeState(GameState::PLAYING);
// // // //             }
// // // //         }
// // // //     }
    
// // // //     m_selectedDifficulty = -1;
// // // //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // // //         Rectangle buttonRect = {
// // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // //             BUTTON_WIDTH,
// // // //             BUTTON_HEIGHT
// // // //         };
        
// // // //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // //             m_selectedDifficulty = static_cast<int>(i);
// // // //         }
// // // //     }
    
// // // //     if (IsKeyPressed(KEY_DOWN)) {
// // // //         m_selectedDifficulty = (m_selectedDifficulty + 1) % m_difficultyNames.size();
// // // //     }
// // // //     if (IsKeyPressed(KEY_UP)) {
// // // //         m_selectedDifficulty = (m_selectedDifficulty - 1 + m_difficultyNames.size()) % m_difficultyNames.size();
// // // //     }
// // // //     if (IsKeyPressed(KEY_ENTER)) {
// // // //         if (m_selectedDifficulty >= 0) {
// // // //             switch (m_selectedDifficulty) {
// // // //                 case 0: m_difficulty = Difficulty::EASY; break;
// // // //                 case 1: m_difficulty = Difficulty::MEDIUM; break;
// // // //                 case 2: m_difficulty = Difficulty::HARD; break;
// // // //             }
// // // //             startNewGame(m_difficulty);
// // // //             changeState(GameState::PLAYING);
// // // //         }
// // // //     }
// // // //     if (IsKeyPressed(KEY_ESCAPE)) {
// // // //         changeState(GameState::MAIN_MENU);
// // // //     }
// // // // }

// // // // void Game::handlePlayingInput() {
// // // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
// // // //         pauseGame();
// // // //     }
    
// // // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_gameBoard) {
// // // //         Vector2 mousePos = GetMousePosition();
// // // //         m_gameBoard->handleClick(mousePos);
// // // //         m_totalMoves++;
// // // //     }
// // // // }

// // // // void Game::handlePausedInput() {
// // // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)) {
// // // //         resumeGame();
// // // //     }
    
// // // //     if (IsKeyPressed(KEY_M)) {
// // // //         returnToMainMenu();
// // // //     }
// // // // }

// // // // void Game::handleGameOverInput() {
// // // //     if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
// // // //         restartGame();
// // // //         changeState(GameState::PLAYING);
// // // //     }
    
// // // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) {
// // // //         returnToMainMenu();
// // // //     }
// // // // }

// // // // void Game::handleSettingsInput() {
// // // //     if (IsKeyPressed(KEY_ESCAPE)) {
// // // //         changeState(GameState::MAIN_MENU);
// // // //     }
// // // // }

// // // // // /**
// // // // //  * @file Game.cpp
// // // // //  * @brief Main Game class implementation
// // // // //  */

// // // // // #include "../include/Game.h"
// // // // // #include "../include/Utils.h"
// // // // // #include <cmath>
// // // // // #include <iostream>

// // // // // // --------------------- Constructor & Destructor ---------------------

// // // // // Game::Game(int screenWidth, int screenHeight)
// // // // //     : m_screenWidth(screenWidth),
// // // // //       m_screenHeight(screenHeight),
// // // // //       m_currentState(GameState::MAIN_MENU),
// // // // //       m_previousState(GameState::MAIN_MENU),
// // // // //       m_difficulty(Difficulty::EASY),
// // // // //       m_selectedMenuItem(0),
// // // // //       m_selectedDifficulty(0),
// // // // //       m_totalMoves(0),
// // // // //       m_matchesFound(0),
// // // // //       m_gameWon(false),
// // // // //       m_gameBoard(nullptr),
// // // // //       m_audioManager(nullptr),
// // // // //       m_scoreManager(nullptr),
// // // // //       m_gameStartTime(0.0f),
// // // // //       m_currentTime(0.0f),
// // // // //       m_pausedTime(0.0f)
// // // // // {
// // // // //     loadResources();
// // // // // }

// // // // // Game::~Game() {
// // // // //     unloadResources();
// // // // // }

// // // // // // --------------------- Resource Management ---------------------

// // // // // void Game::loadResources() {
// // // // //     // Load fonts (use default if files don't exist)
// // // // //     if (FileExists("assets/fonts/arial.ttf")) {
// // // // //         m_titleFont = LoadFont("assets/fonts/arial.ttf");
// // // // //         m_uiFont = LoadFont("assets/fonts/arial.ttf");
// // // // //     } else {
// // // // //         m_titleFont = GetFontDefault();
// // // // //         m_uiFont = GetFontDefault();
// // // // //     }

// // // // //     // Load background (optional)
// // // // //     if (FileExists("assets/textures/background.png")) {
// // // // //         m_backgroundTexture = LoadTexture("assets/textures/background.png");
// // // // //     }

// // // // //     Utils::logInfo("Resources loaded successfully.");
// // // // // }

// // // // // void Game::unloadResources() {
// // // // //     if (m_titleFont.texture.id != GetFontDefault().texture.id) {
// // // // //         UnloadFont(m_titleFont);
// // // // //     }
// // // // //     if (m_uiFont.texture.id != GetFontDefault().texture.id) {
// // // // //         UnloadFont(m_uiFont);
// // // // //     }
// // // // //     if (m_backgroundTexture.id != 0) {
// // // // //         UnloadTexture(m_backgroundTexture);
// // // // //     }

// // // // //     Utils::logInfo("Resources unloaded successfully.");
// // // // // }

// // // // // // --------------------- Game State Updates ---------------------

// // // // // void Game::update() {
// // // // //     switch (m_currentState) {
// // // // //         case GameState::MAIN_MENU: updateMainMenu(); break;
// // // // //         case GameState::DIFFICULTY: updateDifficultySelection(); break;
// // // // //         case GameState::PLAYING: updatePlaying(); break;
// // // // //         case GameState::PAUSED: updatePaused(); break;
// // // // //         case GameState::GAME_OVER: updateGameOver(); break;
// // // // //         case GameState::SETTINGS: updateSettings(); break;
// // // // //     }
// // // // // }

// // // // // void Game::updateMainMenu() {
// // // // //     handleMainMenuInput();
// // // // // }

// // // // // void Game::updateDifficultySelection() {
// // // // //     handleDifficultySelectionInput();
// // // // // }

// // // // // void Game::updatePlaying() {
// // // // //     handlePlayingInput();

// // // // //     if (m_gameBoard)
// // // // //         m_gameBoard->update(GetFrameTime());

// // // // //     checkWinCondition();
// // // // // }

// // // // // void Game::updatePaused() {
// // // // //     handlePausedInput();
// // // // // }

// // // // // void Game::updateGameOver() {
// // // // //     handleGameOverInput();
// // // // // }

// // // // // void Game::updateSettings() {
// // // // //     handleSettingsInput();
// // // // // }

// // // // // // --------------------- Game Rendering ---------------------

// // // // // void Game::draw() {
// // // // //     // Draw background
// // // // //     ClearBackground(DARKBLUE);
    
// // // // //     switch (m_currentState) {
// // // // //         case GameState::MAIN_MENU: drawMainMenu(); break;
// // // // //         case GameState::DIFFICULTY: drawDifficultySelection(); break;
// // // // //         case GameState::PLAYING: drawPlaying(); break;
// // // // //         case GameState::PAUSED: drawPaused(); break;
// // // // //         case GameState::GAME_OVER: drawGameOver(); break;
// // // // //         case GameState::SETTINGS: drawSettings(); break;
// // // // //     }
// // // // // }

// // // // // void Game::drawMainMenu() {
// // // // //     // Draw title
// // // // //     const char* title = "Memory Card Flip Game";
// // // // //     int titleSize = 40;
// // // // //     int titleWidth = MeasureText(title, titleSize);
// // // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, titleSize, WHITE);

// // // // //     // Draw menu buttons
// // // // //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // // // //         Rectangle buttonRect = { 
// // // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // //             200.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // //             BUTTON_WIDTH, 
// // // // //             BUTTON_HEIGHT 
// // // // //         };
// // // // //         drawButton(m_mainMenuItems[i], buttonRect, m_selectedMenuItem == (int)i);
// // // // //     }
// // // // // }

// // // // // void Game::drawDifficultySelection() {
// // // // //     // Draw title
// // // // //     const char* title = "Select Difficulty";
// // // // //     int titleSize = 35;
// // // // //     int titleWidth = MeasureText(title, titleSize);
// // // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 150, titleSize, WHITE);

// // // // //     // Draw difficulty buttons
// // // // //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // // // //         Rectangle buttonRect = { 
// // // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // //             BUTTON_WIDTH, 
// // // // //             BUTTON_HEIGHT 
// // // // //         };
// // // // //         drawButton(m_difficultyNames[i], buttonRect, m_selectedDifficulty == (int)i);
// // // // //     }
    
// // // // //     // Draw back hint
// // // // //     DrawText("Press ESC to go back", 10, m_screenHeight - 30, 20, LIGHTGRAY);
// // // // // }

// // // // // void Game::drawPlaying() {
// // // // //     if (m_gameBoard)
// // // // //         m_gameBoard->draw();

// // // // //     drawTimer();
// // // // //     drawGameStats();
    
// // // // //     // Draw pause hint
// // // // //     DrawText("Press ESC to pause", m_screenWidth - 200, m_screenHeight - 30, 20, LIGHTGRAY);
// // // // // }

// // // // // void Game::drawPaused() {
// // // // //     // Draw semi-transparent overlay
// // // // //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.7f));
    
// // // // //     // Draw pause text
// // // // //     const char* pauseText = "PAUSED";
// // // // //     int textSize = 60;
// // // // //     int textWidth = MeasureText(pauseText, textSize);
// // // // //     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 100, textSize, YELLOW);
    
// // // // //     // Draw instructions
// // // // //     const char* resumeText = "Press SPACE to resume";
// // // // //     int resumeWidth = MeasureText(resumeText, 20);
// // // // //     DrawText(resumeText, m_screenWidth / 2 - resumeWidth / 2, m_screenHeight / 2, 20, WHITE);
    
// // // // //     const char* menuText = "Press M to return to menu";
// // // // //     int menuWidth = MeasureText(menuText, 20);
// // // // //     DrawText(menuText, m_screenWidth / 2 - menuWidth / 2, m_screenHeight / 2 + 40, 20, WHITE);
// // // // // }

// // // // // void Game::drawGameOver() {
// // // // //     // Draw semi-transparent overlay
// // // // //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.7f));
    
// // // // //     // Draw victory text
// // // // //     const char* winText = "YOU WIN!";
// // // // //     int textSize = 60;
// // // // //     int textWidth = MeasureText(winText, textSize);
// // // // //     DrawText(winText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 150, textSize, GREEN);
    
// // // // //     // Draw stats
// // // // //     float elapsedTime = getElapsedTime();
// // // // //     std::string timeStr = "Time: " + Utils::formatTime(elapsedTime);
// // // // //     std::string movesStr = "Moves: " + std::to_string(m_totalMoves);
    
// // // // //     int timeWidth = MeasureText(timeStr.c_str(), 25);
// // // // //     int movesWidth = MeasureText(movesStr.c_str(), 25);
    
// // // // //     DrawText(timeStr.c_str(), m_screenWidth / 2 - timeWidth / 2, m_screenHeight / 2 - 50, 25, WHITE);
// // // // //     DrawText(movesStr.c_str(), m_screenWidth / 2 - movesWidth / 2, m_screenHeight / 2, 25, WHITE);
    
// // // // //     // Draw instructions
// // // // //     const char* restartText = "Press R to restart";
// // // // //     int restartWidth = MeasureText(restartText, 20);
// // // // //     DrawText(restartText, m_screenWidth / 2 - restartWidth / 2, m_screenHeight / 2 + 80, 20, LIGHTGRAY);
    
// // // // //     const char* menuText = "Press ESC for menu";
// // // // //     int menuWidth = MeasureText(menuText, 20);
// // // // //     DrawText(menuText, m_screenWidth / 2 - menuWidth / 2, m_screenHeight / 2 + 110, 20, LIGHTGRAY);
// // // // // }

// // // // // void Game::drawSettings() {
// // // // //     // Draw title
// // // // //     const char* title = "Settings";
// // // // //     int titleSize = 40;
// // // // //     int titleWidth = MeasureText(title, titleSize);
// // // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, titleSize, WHITE);
    
// // // // //     // TODO: Add actual settings options here
// // // // //     DrawText("Settings coming soon...", m_screenWidth / 2 - 100, m_screenHeight / 2, 20, LIGHTGRAY);
    
// // // // //     // Draw back hint
// // // // //     DrawText("Press ESC to go back", 10, m_screenHeight - 30, 20, LIGHTGRAY);
// // // // // }

// // // // // // --------------------- State Transitions ---------------------

// // // // // void Game::changeState(GameState newState) {
// // // // //     m_previousState = m_currentState;
// // // // //     m_currentState = newState;
// // // // // }

// // // // // void Game::startNewGame(Difficulty difficulty) {
// // // // //     m_difficulty = difficulty;

// // // // //     int numCards = static_cast<int>(difficulty);
// // // // //     int gridSize = static_cast<int>(sqrt(numCards));
// // // // //     Vector2 cardSize = Utils::calculateOptimalCardSize(
// // // // //         gridSize, gridSize,
// // // // //         Rectangle{0, 0, (float)m_screenWidth, (float)m_screenHeight - 100},
// // // // //         10.0f
// // // // //     );

// // // // //     // Create new GameBoard
// // // // //     m_gameBoard = std::make_unique<GameBoard>(
// // // // //         gridSize, gridSize, cardSize, 10.0f,
// // // // //         Rectangle{0, 50, (float)m_screenWidth, (float)m_screenHeight - 150}
// // // // //     );

// // // // //     // Create AudioManager & ScoreManager
// // // // //     m_audioManager = std::make_unique<AudioManager>();
// // // // //     m_scoreManager = std::make_unique<ScoreManager>();

// // // // //     m_totalMoves = 0;
// // // // //     m_matchesFound = 0;
// // // // //     m_gameWon = false;
// // // // //     m_gameStartTime = GetTime();
// // // // // }

// // // // // void Game::pauseGame() {
// // // // //     m_pausedTime = GetTime();
// // // // //     changeState(GameState::PAUSED);
// // // // // }

// // // // // void Game::resumeGame() {
// // // // //     float pausedDuration = GetTime() - m_pausedTime;
// // // // //     m_gameStartTime += pausedDuration;
// // // // //     changeState(GameState::PLAYING);
// // // // // }

// // // // // void Game::restartGame() {
// // // // //     startNewGame(m_difficulty);
// // // // // }

// // // // // void Game::returnToMainMenu() {
// // // // //     changeState(GameState::MAIN_MENU);
// // // // // }

// // // // // // --------------------- Helpers ---------------------

// // // // // void Game::checkWinCondition() {
// // // // //     if (m_gameBoard && m_gameBoard->allMatched() && !m_gameWon) {
// // // // //         m_gameWon = true;
// // // // //         m_pausedTime = GetTime(); // Save the winning time
// // // // //         // Calculate matches found
// // // // //         m_matchesFound = m_gameBoard->getMatchesFound();
// // // // //         changeState(GameState::GAME_OVER);
// // // // //     }
// // // // // }

// // // // // float Game::getElapsedTime() const {
// // // // //     if (m_gameWon) {
// // // // //         // Return the time when game was won (frozen)
// // // // //         return m_pausedTime - m_gameStartTime;
// // // // //     }
// // // // //     return GetTime() - m_gameStartTime;
// // // // // }

// // // // // void Game::drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color) {
// // // // //     // Draw button background
// // // // //     Color bgColor = isSelected ? GRAY : color;
// // // // //     DrawRectangleRec(bounds, bgColor);
    
// // // // //     // Draw button border
// // // // //     DrawRectangleLinesEx(bounds, 2.0f, isSelected ? YELLOW : DARKGRAY);
    
// // // // //     // Draw text centered
// // // // //     int textSize = 20;
// // // // //     int textWidth = MeasureText(text.c_str(), textSize);
// // // // //     int textX = bounds.x + (bounds.width - textWidth) / 2;
// // // // //     int textY = bounds.y + (bounds.height - textSize) / 2;
// // // // //     DrawText(text.c_str(), textX, textY, textSize, isSelected ? RED : BLACK);
// // // // // }

// // // // // void Game::drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font) {
// // // // //     int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
// // // // //     DrawTextEx(font, text.c_str(), {m_screenWidth / 2.0f - textWidth / 2, (float)y}, fontSize, 1, color);
// // // // // }

// // // // // void Game::drawGameStats() {
// // // // //     std::string movesStr = "Moves: " + std::to_string(m_totalMoves);
// // // // //     int matches = m_gameBoard ? m_gameBoard->getMatchesFound() : 0;
// // // // //     std::string matchesStr = "Matches: " + std::to_string(matches);
// // // // //     DrawText(movesStr.c_str(), 10, 10, 20, WHITE);
// // // // //     DrawText(matchesStr.c_str(), 10, 40, 20, WHITE);
// // // // // }

// // // // // void Game::drawTimer() {
// // // // //     float elapsed = getElapsedTime();
// // // // //     std::string timerText = "Time: " + Utils::formatTime(elapsed);
// // // // //     DrawText(timerText.c_str(), m_screenWidth - 150, 10, 20, WHITE);
// // // // // }

// // // // // // --------------------- Input Handling ---------------------

// // // // // void Game::handleMainMenuInput() {
// // // // //     Vector2 mousePos = GetMousePosition();
    
// // // // //     // Check mouse clicks on buttons
// // // // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// // // // //         for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // // // //             Rectangle buttonRect = {
// // // // //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // //                 200.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // //                 BUTTON_WIDTH,
// // // // //                 BUTTON_HEIGHT
// // // // //             };
            
// // // // //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // // //                 // Handle button click based on index
// // // // //                 switch (i) {
// // // // //                     case 0: // Start Game
// // // // //                         changeState(GameState::DIFFICULTY);
// // // // //                         break;
// // // // //                     case 1: // Settings
// // // // //                         changeState(GameState::SETTINGS);
// // // // //                         break;
// // // // //                     case 2: // High Scores
// // // // //                         Utils::logInfo("High Scores clicked");
// // // // //                         break;
// // // // //                     case 3: // Exit
// // // // //                         CloseWindow();
// // // // //                         break;
// // // // //                 }
// // // // //             }
// // // // //         }
// // // // //     }
    
// // // // //     // Update hover state for visual feedback
// // // // //     m_selectedMenuItem = -1; // Reset selection
// // // // //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // // // //         Rectangle buttonRect = {
// // // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // //             200.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // //             BUTTON_WIDTH,
// // // // //             BUTTON_HEIGHT
// // // // //         };
        
// // // // //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // // //             m_selectedMenuItem = static_cast<int>(i);
// // // // //         }
// // // // //     }
    
// // // // //     // Keyboard navigation
// // // // //     if (IsKeyPressed(KEY_DOWN)) {
// // // // //         m_selectedMenuItem = (m_selectedMenuItem + 1) % m_mainMenuItems.size();
// // // // //     }
// // // // //     if (IsKeyPressed(KEY_UP)) {
// // // // //         m_selectedMenuItem = (m_selectedMenuItem - 1 + m_mainMenuItems.size()) % m_mainMenuItems.size();
// // // // //     }
// // // // //     if (IsKeyPressed(KEY_ENTER)) {
// // // // //         if (m_selectedMenuItem >= 0) {
// // // // //             switch (m_selectedMenuItem) {
// // // // //                 case 0: changeState(GameState::DIFFICULTY); break;
// // // // //                 case 1: changeState(GameState::SETTINGS); break;
// // // // //                 case 2: Utils::logInfo("High Scores clicked"); break;
// // // // //                 case 3: CloseWindow(); break;
// // // // //             }
// // // // //         }
// // // // //     }
// // // // // }

// // // // // void Game::handleDifficultySelectionInput() {
// // // // //     Vector2 mousePos = GetMousePosition();
    
// // // // //     // Check mouse clicks on difficulty buttons
// // // // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// // // // //         for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // // // //             Rectangle buttonRect = {
// // // // //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // //                 250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // //                 BUTTON_WIDTH,
// // // // //                 BUTTON_HEIGHT
// // // // //             };
            
// // // // //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // // //                 // Set difficulty and start game
// // // // //                 switch (i) {
// // // // //                     case 0: m_difficulty = Difficulty::EASY; break;
// // // // //                     case 1: m_difficulty = Difficulty::MEDIUM; break;
// // // // //                     case 2: m_difficulty = Difficulty::HARD; break;
// // // // //                 }
// // // // //                 startNewGame(m_difficulty);
// // // // //                 changeState(GameState::PLAYING);
// // // // //             }
// // // // //         }
// // // // //     }
    
// // // // //     // Update hover state
// // // // //     m_selectedDifficulty = -1; // Reset selection
// // // // //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // // // //         Rectangle buttonRect = {
// // // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // //             BUTTON_WIDTH,
// // // // //             BUTTON_HEIGHT
// // // // //         };
        
// // // // //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // // //             m_selectedDifficulty = static_cast<int>(i);
// // // // //         }
// // // // //     }
    
// // // // //     // Keyboard navigation
// // // // //     if (IsKeyPressed(KEY_DOWN)) {
// // // // //         m_selectedDifficulty = (m_selectedDifficulty + 1) % m_difficultyNames.size();
// // // // //     }
// // // // //     if (IsKeyPressed(KEY_UP)) {
// // // // //         m_selectedDifficulty = (m_selectedDifficulty - 1 + m_difficultyNames.size()) % m_difficultyNames.size();
// // // // //     }
// // // // //     if (IsKeyPressed(KEY_ENTER)) {
// // // // //         if (m_selectedDifficulty >= 0) {
// // // // //             switch (m_selectedDifficulty) {
// // // // //                 case 0: m_difficulty = Difficulty::EASY; break;
// // // // //                 case 1: m_difficulty = Difficulty::MEDIUM; break;
// // // // //                 case 2: m_difficulty = Difficulty::HARD; break;
// // // // //             }
// // // // //             startNewGame(m_difficulty);
// // // // //             changeState(GameState::PLAYING);
// // // // //         }
// // // // //     }
// // // // //     if (IsKeyPressed(KEY_ESCAPE)) {
// // // // //         changeState(GameState::MAIN_MENU);
// // // // //     }
// // // // // }

// // // // // void Game::handlePlayingInput() {
// // // // //     // Pause game with ESC or P
// // // // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
// // // // //         pauseGame();
// // // // //     }
    
// // // // //     // Handle card clicks
// // // // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_gameBoard) {
// // // // //         Vector2 mousePos = GetMousePosition();
// // // // //         m_gameBoard->handleClick(mousePos);
// // // // //         m_totalMoves++;
// // // // //     }
// // // // // }

// // // // // void Game::handlePausedInput() {
// // // // //     // Resume with ESC, P, or SPACE
// // // // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)) {
// // // // //         resumeGame();
// // // // //     }
    
// // // // //     // Return to menu with M
// // // // //     if (IsKeyPressed(KEY_M)) {
// // // // //         returnToMainMenu();
// // // // //     }
// // // // // }

// // // // // void Game::handleGameOverInput() {
// // // // //     // Restart with R or ENTER
// // // // //     if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
// // // // //         restartGame();
// // // // //         changeState(GameState::PLAYING);
// // // // //     }
    
// // // // //     // Return to menu with ESC or M
// // // // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) {
// // // // //         returnToMainMenu();
// // // // //     }
// // // // // }

// // // // // void Game::handleSettingsInput() {
// // // // //     // Return to menu with ESC
// // // // //     if (IsKeyPressed(KEY_ESCAPE)) {
// // // // //         changeState(GameState::MAIN_MENU);
// // // // //     }
// // // // // }

// // // // // // /**
// // // // // //  * @file Game.cpp
// // // // // //  * @brief Main Game class implementation
// // // // // //  */

// // // // // // #include "../include/Game.h"
// // // // // // #include "../include/Utils.h"
// // // // // // #include <cmath>
// // // // // // #include <iostream>

// // // // // // // --------------------- Constructor & Destructor ---------------------

// // // // // // Game::Game(int screenWidth, int screenHeight)
// // // // // //     : m_screenWidth(screenWidth),
// // // // // //       m_screenHeight(screenHeight),
// // // // // //       m_currentState(GameState::MAIN_MENU),
// // // // // //       m_previousState(GameState::MAIN_MENU),
// // // // // //       m_difficulty(Difficulty::EASY),
// // // // // //       m_selectedMenuItem(0),
// // // // // //       m_selectedDifficulty(0),
// // // // // //       m_totalMoves(0),
// // // // // //       m_matchesFound(0),
// // // // // //       m_gameWon(false),
// // // // // //       m_gameBoard(nullptr),
// // // // // //       m_audioManager(nullptr),
// // // // // //       m_scoreManager(nullptr),
// // // // // //       m_gameStartTime(0.0f),
// // // // // //       m_currentTime(0.0f),
// // // // // //       m_pausedTime(0.0f)
// // // // // // {
// // // // // //     loadResources();
// // // // // // }

// // // // // // Game::~Game() {
// // // // // //     unloadResources();
// // // // // // }

// // // // // // // --------------------- Resource Management ---------------------

// // // // // // void Game::loadResources() {
// // // // // //     // Load fonts (use default if files don't exist)
// // // // // //     if (FileExists("assets/fonts/arial.ttf")) {
// // // // // //         m_titleFont = LoadFont("assets/fonts/arial.ttf");
// // // // // //         m_uiFont = LoadFont("assets/fonts/arial.ttf");
// // // // // //     } else {
// // // // // //         m_titleFont = GetFontDefault();
// // // // // //         m_uiFont = GetFontDefault();
// // // // // //     }

// // // // // //     // Load background (optional)
// // // // // //     if (FileExists("assets/textures/background.png")) {
// // // // // //         m_backgroundTexture = LoadTexture("assets/textures/background.png");
// // // // // //     }

// // // // // //     Utils::logInfo("Resources loaded successfully.");
// // // // // // }

// // // // // // void Game::unloadResources() {
// // // // // //     if (m_titleFont.texture.id != GetFontDefault().texture.id) {
// // // // // //         UnloadFont(m_titleFont);
// // // // // //     }
// // // // // //     if (m_uiFont.texture.id != GetFontDefault().texture.id) {
// // // // // //         UnloadFont(m_uiFont);
// // // // // //     }
// // // // // //     if (m_backgroundTexture.id != 0) {
// // // // // //         UnloadTexture(m_backgroundTexture);
// // // // // //     }

// // // // // //     Utils::logInfo("Resources unloaded successfully.");
// // // // // // }

// // // // // // // --------------------- Game State Updates ---------------------

// // // // // // void Game::update() {
// // // // // //     switch (m_currentState) {
// // // // // //         case GameState::MAIN_MENU: updateMainMenu(); break;
// // // // // //         case GameState::DIFFICULTY: updateDifficultySelection(); break;
// // // // // //         case GameState::PLAYING: updatePlaying(); break;
// // // // // //         case GameState::PAUSED: updatePaused(); break;
// // // // // //         case GameState::GAME_OVER: updateGameOver(); break;
// // // // // //         case GameState::SETTINGS: updateSettings(); break;
// // // // // //     }
// // // // // // }

// // // // // // void Game::updateMainMenu() {
// // // // // //     handleMainMenuInput();
// // // // // // }

// // // // // // void Game::updateDifficultySelection() {
// // // // // //     handleDifficultySelectionInput();
// // // // // // }

// // // // // // void Game::updatePlaying() {
// // // // // //     handlePlayingInput();

// // // // // //     if (m_gameBoard)
// // // // // //         m_gameBoard->update(GetFrameTime());

// // // // // //     checkWinCondition();
// // // // // // }

// // // // // // void Game::updatePaused() {
// // // // // //     handlePausedInput();
// // // // // // }

// // // // // // void Game::updateGameOver() {
// // // // // //     handleGameOverInput();
// // // // // // }

// // // // // // void Game::updateSettings() {
// // // // // //     handleSettingsInput();
// // // // // // }

// // // // // // // --------------------- Game Rendering ---------------------

// // // // // // void Game::draw() {
// // // // // //     // Draw background
// // // // // //     ClearBackground(DARKBLUE);
    
// // // // // //     switch (m_currentState) {
// // // // // //         case GameState::MAIN_MENU: drawMainMenu(); break;
// // // // // //         case GameState::DIFFICULTY: drawDifficultySelection(); break;
// // // // // //         case GameState::PLAYING: drawPlaying(); break;
// // // // // //         case GameState::PAUSED: drawPaused(); break;
// // // // // //         case GameState::GAME_OVER: drawGameOver(); break;
// // // // // //         case GameState::SETTINGS: drawSettings(); break;
// // // // // //     }
// // // // // // }

// // // // // // void Game::drawMainMenu() {
// // // // // //     // Draw title
// // // // // //     const char* title = "Memory Card Flip Game";
// // // // // //     int titleSize = 40;
// // // // // //     int titleWidth = MeasureText(title, titleSize);
// // // // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, titleSize, WHITE);

// // // // // //     // Draw menu buttons
// // // // // //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // // // // //         Rectangle buttonRect = { 
// // // // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // // //             200.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // // //             BUTTON_WIDTH, 
// // // // // //             BUTTON_HEIGHT 
// // // // // //         };
// // // // // //         drawButton(m_mainMenuItems[i], buttonRect, m_selectedMenuItem == (int)i);
// // // // // //     }
// // // // // // }

// // // // // // void Game::drawDifficultySelection() {
// // // // // //     // Draw title
// // // // // //     const char* title = "Select Difficulty";
// // // // // //     int titleSize = 35;
// // // // // //     int titleWidth = MeasureText(title, titleSize);
// // // // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 150, titleSize, WHITE);

// // // // // //     // Draw difficulty buttons
// // // // // //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // // // // //         Rectangle buttonRect = { 
// // // // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // // //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // // //             BUTTON_WIDTH, 
// // // // // //             BUTTON_HEIGHT 
// // // // // //         };
// // // // // //         drawButton(m_difficultyNames[i], buttonRect, m_selectedDifficulty == (int)i);
// // // // // //     }
    
// // // // // //     // Draw back hint
// // // // // //     DrawText("Press ESC to go back", 10, m_screenHeight - 30, 20, LIGHTGRAY);
// // // // // // }

// // // // // // void Game::drawPlaying() {
// // // // // //     if (m_gameBoard)
// // // // // //         m_gameBoard->draw();

// // // // // //     drawTimer();
// // // // // //     drawGameStats();
    
// // // // // //     // Draw pause hint
// // // // // //     DrawText("Press ESC to pause", m_screenWidth - 200, m_screenHeight - 30, 20, LIGHTGRAY);
// // // // // // }

// // // // // // void Game::drawPaused() {
// // // // // //     // Draw semi-transparent overlay
// // // // // //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.7f));
    
// // // // // //     // Draw pause text
// // // // // //     const char* pauseText = "PAUSED";
// // // // // //     int textSize = 60;
// // // // // //     int textWidth = MeasureText(pauseText, textSize);
// // // // // //     DrawText(pauseText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 100, textSize, YELLOW);
    
// // // // // //     // Draw instructions
// // // // // //     const char* resumeText = "Press SPACE to resume";
// // // // // //     int resumeWidth = MeasureText(resumeText, 20);
// // // // // //     DrawText(resumeText, m_screenWidth / 2 - resumeWidth / 2, m_screenHeight / 2, 20, WHITE);
    
// // // // // //     const char* menuText = "Press M to return to menu";
// // // // // //     int menuWidth = MeasureText(menuText, 20);
// // // // // //     DrawText(menuText, m_screenWidth / 2 - menuWidth / 2, m_screenHeight / 2 + 40, 20, WHITE);
// // // // // // }

// // // // // // void Game::drawGameOver() {
// // // // // //     // Draw semi-transparent overlay
// // // // // //     DrawRectangle(0, 0, m_screenWidth, m_screenHeight, ColorAlpha(BLACK, 0.7f));
    
// // // // // //     // Draw victory text
// // // // // //     const char* winText = "YOU WIN!";
// // // // // //     int textSize = 60;
// // // // // //     int textWidth = MeasureText(winText, textSize);
// // // // // //     DrawText(winText, m_screenWidth / 2 - textWidth / 2, m_screenHeight / 2 - 150, textSize, GREEN);
    
// // // // // //     // Draw stats
// // // // // //     float elapsedTime = getElapsedTime();
// // // // // //     std::string timeStr = "Time: " + Utils::formatTime(elapsedTime);
// // // // // //     std::string movesStr = "Moves: " + std::to_string(m_totalMoves);
    
// // // // // //     int timeWidth = MeasureText(timeStr.c_str(), 25);
// // // // // //     int movesWidth = MeasureText(movesStr.c_str(), 25);
    
// // // // // //     DrawText(timeStr.c_str(), m_screenWidth / 2 - timeWidth / 2, m_screenHeight / 2 - 50, 25, WHITE);
// // // // // //     DrawText(movesStr.c_str(), m_screenWidth / 2 - movesWidth / 2, m_screenHeight / 2, 25, WHITE);
    
// // // // // //     // Draw instructions
// // // // // //     const char* restartText = "Press R to restart";
// // // // // //     int restartWidth = MeasureText(restartText, 20);
// // // // // //     DrawText(restartText, m_screenWidth / 2 - restartWidth / 2, m_screenHeight / 2 + 80, 20, LIGHTGRAY);
    
// // // // // //     const char* menuText = "Press ESC for menu";
// // // // // //     int menuWidth = MeasureText(menuText, 20);
// // // // // //     DrawText(menuText, m_screenWidth / 2 - menuWidth / 2, m_screenHeight / 2 + 110, 20, LIGHTGRAY);
// // // // // // }

// // // // // // void Game::drawSettings() {
// // // // // //     // Draw title
// // // // // //     const char* title = "Settings";
// // // // // //     int titleSize = 40;
// // // // // //     int titleWidth = MeasureText(title, titleSize);
// // // // // //     DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, titleSize, WHITE);
    
// // // // // //     // TODO: Add actual settings options here
// // // // // //     DrawText("Settings coming soon...", m_screenWidth / 2 - 100, m_screenHeight / 2, 20, LIGHTGRAY);
    
// // // // // //     // Draw back hint
// // // // // //     DrawText("Press ESC to go back", 10, m_screenHeight - 30, 20, LIGHTGRAY);
// // // // // // }

// // // // // // // --------------------- State Transitions ---------------------

// // // // // // void Game::changeState(GameState newState) {
// // // // // //     m_previousState = m_currentState;
// // // // // //     m_currentState = newState;
// // // // // // }

// // // // // // void Game::startNewGame(Difficulty difficulty) {
// // // // // //     m_difficulty = difficulty;

// // // // // //     int numCards = static_cast<int>(difficulty);
// // // // // //     int gridSize = static_cast<int>(sqrt(numCards));
// // // // // //     Vector2 cardSize = Utils::calculateOptimalCardSize(
// // // // // //         gridSize, gridSize,
// // // // // //         Rectangle{0, 0, (float)m_screenWidth, (float)m_screenHeight - 100},
// // // // // //         10.0f
// // // // // //     );

// // // // // //     // Create new GameBoard
// // // // // //     m_gameBoard = std::make_unique<GameBoard>(
// // // // // //         gridSize, gridSize, cardSize, 10.0f,
// // // // // //         Rectangle{0, 50, (float)m_screenWidth, (float)m_screenHeight - 150}
// // // // // //     );

// // // // // //     // Create AudioManager & ScoreManager
// // // // // //     m_audioManager = std::make_unique<AudioManager>();
// // // // // //     m_scoreManager = std::make_unique<ScoreManager>();

// // // // // //     m_totalMoves = 0;
// // // // // //     m_matchesFound = 0;
// // // // // //     m_gameWon = false;
// // // // // //     m_gameStartTime = GetTime();
// // // // // // }

// // // // // // void Game::pauseGame() {
// // // // // //     m_pausedTime = GetTime();
// // // // // //     changeState(GameState::PAUSED);
// // // // // // }

// // // // // // void Game::resumeGame() {
// // // // // //     float pausedDuration = GetTime() - m_pausedTime;
// // // // // //     m_gameStartTime += pausedDuration;
// // // // // //     changeState(GameState::PLAYING);
// // // // // // }

// // // // // // void Game::restartGame() {
// // // // // //     startNewGame(m_difficulty);
// // // // // // }

// // // // // // void Game::returnToMainMenu() {
// // // // // //     changeState(GameState::MAIN_MENU);
// // // // // // }

// // // // // // // --------------------- Helpers ---------------------

// // // // // // void Game::checkWinCondition() {
// // // // // //     if (m_gameBoard && m_gameBoard->allMatched() && !m_gameWon) {
// // // // // //         m_gameWon = true;
// // // // // //         changeState(GameState::GAME_OVER);
// // // // // //     }
// // // // // // }

// // // // // // float Game::getElapsedTime() const {
// // // // // //     return GetTime() - m_gameStartTime;
// // // // // // }

// // // // // // void Game::drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color) {
// // // // // //     // Draw button background
// // // // // //     Color bgColor = isSelected ? GRAY : color;
// // // // // //     DrawRectangleRec(bounds, bgColor);
    
// // // // // //     // Draw button border
// // // // // //     DrawRectangleLinesEx(bounds, 2.0f, isSelected ? YELLOW : DARKGRAY);
    
// // // // // //     // Draw text centered
// // // // // //     int textSize = 20;
// // // // // //     int textWidth = MeasureText(text.c_str(), textSize);
// // // // // //     int textX = bounds.x + (bounds.width - textWidth) / 2;
// // // // // //     int textY = bounds.y + (bounds.height - textSize) / 2;
// // // // // //     DrawText(text.c_str(), textX, textY, textSize, isSelected ? RED : BLACK);
// // // // // // }

// // // // // // void Game::drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font) {
// // // // // //     int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
// // // // // //     DrawTextEx(font, text.c_str(), {m_screenWidth / 2.0f - textWidth / 2, (float)y}, fontSize, 1, color);
// // // // // // }

// // // // // // void Game::drawGameStats() {
// // // // // //     std::string movesStr = "Moves: " + std::to_string(m_totalMoves);
// // // // // //     std::string matchesStr = "Matches: " + std::to_string(m_matchesFound);
// // // // // //     DrawText(movesStr.c_str(), 10, 10, 20, WHITE);
// // // // // //     DrawText(matchesStr.c_str(), 10, 40, 20, WHITE);
// // // // // // }

// // // // // // void Game::drawTimer() {
// // // // // //     float elapsed = getElapsedTime();
// // // // // //     std::string timerText = "Time: " + Utils::formatTime(elapsed);
// // // // // //     DrawText(timerText.c_str(), m_screenWidth - 150, 10, 20, WHITE);
// // // // // // }

// // // // // // // --------------------- Input Handling ---------------------

// // // // // // void Game::handleMainMenuInput() {
// // // // // //     Vector2 mousePos = GetMousePosition();
    
// // // // // //     // Check mouse clicks on buttons
// // // // // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// // // // // //         for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // // // // //             Rectangle buttonRect = {
// // // // // //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // // //                 200.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // // //                 BUTTON_WIDTH,
// // // // // //                 BUTTON_HEIGHT
// // // // // //             };
            
// // // // // //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // // // //                 // Handle button click based on index
// // // // // //                 switch (i) {
// // // // // //                     case 0: // Start Game
// // // // // //                         changeState(GameState::DIFFICULTY);
// // // // // //                         break;
// // // // // //                     case 1: // Settings
// // // // // //                         changeState(GameState::SETTINGS);
// // // // // //                         break;
// // // // // //                     case 2: // High Scores
// // // // // //                         Utils::logInfo("High Scores clicked");
// // // // // //                         break;
// // // // // //                     case 3: // Exit
// // // // // //                         CloseWindow();
// // // // // //                         break;
// // // // // //                 }
// // // // // //             }
// // // // // //         }
// // // // // //     }
    
// // // // // //     // Update hover state for visual feedback
// // // // // //     m_selectedMenuItem = -1; // Reset selection
// // // // // //     for (size_t i = 0; i < m_mainMenuItems.size(); ++i) {
// // // // // //         Rectangle buttonRect = {
// // // // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // // //             200.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // // //             BUTTON_WIDTH,
// // // // // //             BUTTON_HEIGHT
// // // // // //         };
        
// // // // // //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // // // //             m_selectedMenuItem = static_cast<int>(i);
// // // // // //         }
// // // // // //     }
    
// // // // // //     // Keyboard navigation
// // // // // //     if (IsKeyPressed(KEY_DOWN)) {
// // // // // //         m_selectedMenuItem = (m_selectedMenuItem + 1) % m_mainMenuItems.size();
// // // // // //     }
// // // // // //     if (IsKeyPressed(KEY_UP)) {
// // // // // //         m_selectedMenuItem = (m_selectedMenuItem - 1 + m_mainMenuItems.size()) % m_mainMenuItems.size();
// // // // // //     }
// // // // // //     if (IsKeyPressed(KEY_ENTER)) {
// // // // // //         if (m_selectedMenuItem >= 0) {
// // // // // //             switch (m_selectedMenuItem) {
// // // // // //                 case 0: changeState(GameState::DIFFICULTY); break;
// // // // // //                 case 1: changeState(GameState::SETTINGS); break;
// // // // // //                 case 2: Utils::logInfo("High Scores clicked"); break;
// // // // // //                 case 3: CloseWindow(); break;
// // // // // //             }
// // // // // //         }
// // // // // //     }
// // // // // // }

// // // // // // void Game::handleDifficultySelectionInput() {
// // // // // //     Vector2 mousePos = GetMousePosition();
    
// // // // // //     // Check mouse clicks on difficulty buttons
// // // // // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
// // // // // //         for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // // // // //             Rectangle buttonRect = {
// // // // // //                 m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // // //                 250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // // //                 BUTTON_WIDTH,
// // // // // //                 BUTTON_HEIGHT
// // // // // //             };
            
// // // // // //             if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // // // //                 // Set difficulty and start game
// // // // // //                 switch (i) {
// // // // // //                     case 0: m_difficulty = Difficulty::EASY; break;
// // // // // //                     case 1: m_difficulty = Difficulty::MEDIUM; break;
// // // // // //                     case 2: m_difficulty = Difficulty::HARD; break;
// // // // // //                 }
// // // // // //                 startNewGame(m_difficulty);
// // // // // //                 changeState(GameState::PLAYING);
// // // // // //             }
// // // // // //         }
// // // // // //     }
    
// // // // // //     // Update hover state
// // // // // //     m_selectedDifficulty = -1; // Reset selection
// // // // // //     for (size_t i = 0; i < m_difficultyNames.size(); ++i) {
// // // // // //         Rectangle buttonRect = {
// // // // // //             m_screenWidth / 2.0f - BUTTON_WIDTH / 2,
// // // // // //             250.0f + i * (BUTTON_HEIGHT + BUTTON_SPACING),
// // // // // //             BUTTON_WIDTH,
// // // // // //             BUTTON_HEIGHT
// // // // // //         };
        
// // // // // //         if (CheckCollisionPointRec(mousePos, buttonRect)) {
// // // // // //             m_selectedDifficulty = static_cast<int>(i);
// // // // // //         }
// // // // // //     }
    
// // // // // //     // Keyboard navigation
// // // // // //     if (IsKeyPressed(KEY_DOWN)) {
// // // // // //         m_selectedDifficulty = (m_selectedDifficulty + 1) % m_difficultyNames.size();
// // // // // //     }
// // // // // //     if (IsKeyPressed(KEY_UP)) {
// // // // // //         m_selectedDifficulty = (m_selectedDifficulty - 1 + m_difficultyNames.size()) % m_difficultyNames.size();
// // // // // //     }
// // // // // //     if (IsKeyPressed(KEY_ENTER)) {
// // // // // //         if (m_selectedDifficulty >= 0) {
// // // // // //             switch (m_selectedDifficulty) {
// // // // // //                 case 0: m_difficulty = Difficulty::EASY; break;
// // // // // //                 case 1: m_difficulty = Difficulty::MEDIUM; break;
// // // // // //                 case 2: m_difficulty = Difficulty::HARD; break;
// // // // // //             }
// // // // // //             startNewGame(m_difficulty);
// // // // // //             changeState(GameState::PLAYING);
// // // // // //         }
// // // // // //     }
// // // // // //     if (IsKeyPressed(KEY_ESCAPE)) {
// // // // // //         changeState(GameState::MAIN_MENU);
// // // // // //     }
// // // // // // }

// // // // // // void Game::handlePlayingInput() {
// // // // // //     // Pause game with ESC or P
// // // // // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
// // // // // //         pauseGame();
// // // // // //     }
    
// // // // // //     // Handle card clicks
// // // // // //     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_gameBoard) {
// // // // // //         Vector2 mousePos = GetMousePosition();
// // // // // //         m_gameBoard->handleClick(mousePos);
// // // // // //         m_totalMoves++;
// // // // // //     }
// // // // // // }

// // // // // // void Game::handlePausedInput() {
// // // // // //     // Resume with ESC, P, or SPACE
// // // // // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)) {
// // // // // //         resumeGame();
// // // // // //     }
    
// // // // // //     // Return to menu with M
// // // // // //     if (IsKeyPressed(KEY_M)) {
// // // // // //         returnToMainMenu();
// // // // // //     }
// // // // // // }

// // // // // // void Game::handleGameOverInput() {
// // // // // //     // Restart with R or ENTER
// // // // // //     if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
// // // // // //         restartGame();
// // // // // //         changeState(GameState::PLAYING);
// // // // // //     }
    
// // // // // //     // Return to menu with ESC or M
// // // // // //     if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) {
// // // // // //         returnToMainMenu();
// // // // // //     }
// // // // // // }

// // // // // // void Game::handleSettingsInput() {
// // // // // //     // Return to menu with ESC
// // // // // //     if (IsKeyPressed(KEY_ESCAPE)) {
// // // // // //         changeState(GameState::MAIN_MENU);
// // // // // //     }
// // // // // // }