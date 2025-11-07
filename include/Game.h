/**
 * @file Game.h
 * @brief Main Game class that manages the overall game state and flow
 * 
 * This class handles the main game loop, scene management, and coordinates
 * between different game components like GameBoard, AudioManager, etc.
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#pragma once

#include <raylib.h>
#include <memory>
#include <vector>
#include <string>

#include "GameBoard.h"
#include "AudioManager.h"
#include "ScoreManager.h"
#include "Utils.h"

/**
 * @brief Enumeration of different game states
 */
enum class GameState {
    MAIN_MENU,      ///< Main menu screen
    DIFFICULTY,     ///< Difficulty selection screen
    PLAYING,        ///< Main gameplay
    PAUSED,         ///< Game paused
    GAME_OVER,      ///< Game completed successfully
    SETTINGS,       ///< Settings/options screen
    HIGH_SCORES     ///< High scores screen
};

/**
 * @brief Enumeration of difficulty levels
 */
enum class Difficulty {
    EASY = 16,      ///< 4x4 grid (16 cards, 8 pairs)
    MEDIUM = 36,    ///< 6x6 grid (36 cards, 18 pairs)
    HARD = 64       ///< 8x8 grid (64 cards, 32 pairs)
};

/**
 * @brief Main Game class that manages the entire game
 * 
 * The Game class is responsible for:
 * - Managing game states and transitions
 * - Handling user input
 * - Coordinating between different game components
 * - Managing the game timer and scoring
 * - Rendering the UI and delegating drawing to appropriate components
 */
class Game {
public:
    /**
     * @brief Constructor for the Game class
     * @param screenWidth Width of the game window
     * @param screenHeight Height of the game window
     */
    Game(int screenWidth, int screenHeight);
    
    /**
     * @brief Destructor - cleans up resources
     */
    ~Game();
    
    /**
     * @brief Updates the game logic
     * 
     * This method is called once per frame and handles:
     * - State-specific updates
     * - Input processing
     * - Game timer updates
     * - Victory condition checking
     */
    void update();
    
    /**
     * @brief Renders the game
     * 
     * This method is called once per frame and handles:
     * - State-specific rendering
     * - UI drawing
     * - Delegating drawing to appropriate components
     */
    void draw();
    
    /**
     * @brief Gets the current game state
     * @return Current GameState
     */
    GameState getCurrentState() const { return m_currentState; }
    
    /**
     * @brief Gets the current difficulty level
     * @return Current Difficulty
     */
    Difficulty getDifficulty() const { return m_difficulty; }
    
private:
    // Screen dimensions
    int m_screenWidth;
    int m_screenHeight;
    
    // Game state management
    GameState m_currentState;
    GameState m_previousState;
    Difficulty m_difficulty;
    
    // Game components
    std::unique_ptr<GameBoard> m_gameBoard;
    std::unique_ptr<AudioManager> m_audioManager;
    std::unique_ptr<ScoreManager> m_scoreManager;
    
    // Game timing
    float m_gameStartTime;
    float m_currentTime;
    float m_pausedTime;
    
    // UI elements
    Font m_titleFont;
    Font m_uiFont;
    Texture2D m_backgroundTexture;
    
    // Menu selection
    int m_selectedMenuItem;
    int m_selectedDifficulty;
    
    // Game statistics
    int m_totalMoves;
    int m_matchesFound;
    bool m_gameWon;
    // Settings
    bool m_soundEnabled;
    int m_shufflesUsed;
    float m_shuffleCooldownTimer;
    static constexpr float SHUFFLE_COOLDOWN_SECONDS = 25.0f;
    static constexpr float SHUFFLE_INITIAL_DELAY_SECONDS = 3.0f;
    
    // Private methods for different game states
    void updateMainMenu();
    void updateDifficultySelection();
    void updatePlaying();
    void updatePaused();
    void updateGameOver();
    void updateSettings();
    
    void drawMainMenu();
    void drawDifficultySelection();
    void drawPlaying();
    void drawPaused();
    void drawGameOver();
    void drawSettings();
    void drawHighScores();
    
    // State transition methods
    void changeState(GameState newState);
    void startNewGame(Difficulty difficulty);
    void pauseGame();
    void resumeGame();
    void restartGame();
    void returnToMainMenu();
    
    // UI helper methods
    void drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color = LIGHTGRAY);
    void drawEnhancedButton(const std::string& text, Rectangle bounds, bool isSelected, Color baseColor = LIGHTGRAY);
    void drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font);
    void drawGameStats();
    void drawTimer();
    void drawGradientBackground();
    void drawEnhancedHUD();
    
    // Input handling
    void handleMainMenuInput();
    void handleDifficultySelectionInput();
    void handlePlayingInput();
    void handlePausedInput();
    void handleGameOverInput();
    void handleSettingsInput();
    void handleHighScoresInput();
    
    // Utility methods
    void loadResources();
    void unloadResources();
    void checkWinCondition();
    float getElapsedTime() const;
    bool canTriggerShuffle() const;
    void triggerShuffle();
    
    // Menu configuration
    static constexpr int MAIN_MENU_ITEMS = 4;
    static constexpr int DIFFICULTY_OPTIONS = 3;
    static constexpr float BUTTON_HEIGHT = 60.0f;
    static constexpr float BUTTON_WIDTH = 300.0f;
    static constexpr float BUTTON_SPACING = 20.0f;
    
    // Menu item names
    const std::vector<std::string> m_mainMenuItems = {
        "Start Game",
        "Settings", 
        "High Scores",
        "Exit"
    };
    
    const std::vector<std::string> m_difficultyNames = {
        "Easy (4x4)",
        "Medium (6x6)", 
        "Hard (8x8)"
    };
};

// /**
//  * @file Game.h
//  * @brief Main Game class that manages the overall game state and flow
//  * 
//  * This class handles the main game loop, scene management, and coordinates
//  * between different game components like GameBoard, AudioManager, etc.
//  * 
//  * @author MSTC DA-IICT
//  * @version 1.0.0
//  * @date 2025-10-14
//  */

// #pragma once

// #include <raylib.h>
// #include <memory>
// #include <vector>
// #include <string>

// #include "GameBoard.h"
// #include "AudioManager.h"
// #include "ScoreManager.h"
// #include "Utils.h"

// /**
//  * @brief Enumeration of different game states
//  */
// enum class GameState {
//     MAIN_MENU,      ///< Main menu screen
//     DIFFICULTY,     ///< Difficulty selection screen
//     PLAYING,        ///< Main gameplay
//     PAUSED,         ///< Game paused
//     GAME_OVER,      ///< Game completed successfully
//     SETTINGS        ///< Settings/options screen
// };

// /**
//  * @brief Enumeration of difficulty levels
//  */
// enum class Difficulty {
//     EASY = 16,      ///< 4x4 grid (16 cards, 8 pairs)
//     MEDIUM = 36,    ///< 6x6 grid (36 cards, 18 pairs)
//     HARD = 64       ///< 8x8 grid (64 cards, 32 pairs)
// };

// /**
//  * @brief Main Game class that manages the entire game
//  * 
//  * The Game class is responsible for:
//  * - Managing game states and transitions
//  * - Handling user input
//  * - Coordinating between different game components
//  * - Managing the game timer and scoring
//  * - Rendering the UI and delegating drawing to appropriate components
//  */
// class Game {
// public:
//     /**
//      * @brief Constructor for the Game class
//      * @param screenWidth Width of the game window
//      * @param screenHeight Height of the game window
//      */
//     Game(int screenWidth, int screenHeight);
    
//     /**
//      * @brief Destructor - cleans up resources
//      */
//     ~Game();
    
//     /**
//      * @brief Updates the game logic
//      * 
//      * This method is called once per frame and handles:
//      * - State-specific updates
//      * - Input processing
//      * - Game timer updates
//      * - Victory condition checking
//      */
//     void update();
    
//     /**
//      * @brief Renders the game
//      * 
//      * This method is called once per frame and handles:
//      * - State-specific rendering
//      * - UI drawing
//      * - Delegating drawing to appropriate components
//      */
//     void draw();
    
//     /**
//      * @brief Gets the current game state
//      * @return Current GameState
//      */
//     GameState getCurrentState() const { return m_currentState; }
    
//     /**
//      * @brief Gets the current difficulty level
//      * @return Current Difficulty
//      */
//     Difficulty getDifficulty() const { return m_difficulty; }
    
// private:
//     // Screen dimensions
//     int m_screenWidth;
//     int m_screenHeight;
    
//     // Game state management
//     GameState m_currentState;
//     GameState m_previousState;
//     Difficulty m_difficulty;
    
//     // Game components
//     std::unique_ptr<GameBoard> m_gameBoard;
//     std::unique_ptr<AudioManager> m_audioManager;
//     std::unique_ptr<ScoreManager> m_scoreManager;
    
//     // Game timing
//     float m_gameStartTime;
//     float m_currentTime;
//     float m_pausedTime;
    
//     // UI elements
//     Font m_titleFont;
//     Font m_uiFont;
//     Texture2D m_backgroundTexture;
    
//     // Menu selection
//     int m_selectedMenuItem;
//     int m_selectedDifficulty;
    
//     // Game statistics
//     int m_totalMoves;
//     int m_matchesFound;
//     bool m_gameWon;
    
//     // Private methods for different game states
//     void updateMainMenu();
//     void updateDifficultySelection();
//     void updatePlaying();
//     void updatePaused();
//     void updateGameOver();
//     void updateSettings();
    
//     void drawMainMenu();
//     void drawDifficultySelection();
//     void drawPlaying();
//     void drawPaused();
//     void drawGameOver();
//     void drawSettings();
    
//     // State transition methods
//     void changeState(GameState newState);
//     void startNewGame(Difficulty difficulty);
//     void pauseGame();
//     void resumeGame();
//     void restartGame();
//     void returnToMainMenu();
    
//     // UI helper methods
//     void drawButton(const std::string& text, Rectangle bounds, bool isSelected, Color color = LIGHTGRAY);
//     void drawCenteredText(const std::string& text, int y, int fontSize, Color color, Font font);
//     void drawGameStats();
//     void drawTimer();
    
//     // Input handling
//     void handleMainMenuInput();
//     void handleDifficultySelectionInput();
//     void handlePlayingInput();
//     void handlePausedInput();
//     void handleGameOverInput();
//     void handleSettingsInput();
    
//     // Utility methods
//     void loadResources();
//     void unloadResources();
//     void checkWinCondition();
//     float getElapsedTime() const;
    
//     // Menu configuration
//     static constexpr int MAIN_MENU_ITEMS = 4;
//     static constexpr int DIFFICULTY_OPTIONS = 3;
//     static constexpr float BUTTON_HEIGHT = 60.0f;
//     static constexpr float BUTTON_WIDTH = 300.0f;
//     static constexpr float BUTTON_SPACING = 20.0f;
    
//     // Menu item names
//     const std::vector<std::string> m_mainMenuItems = {
//         "Start Game",
//         "Settings", 
//         "High Scores",
//         "Exit"
//     };
    
//     const std::vector<std::string> m_difficultyNames = {
//         "Easy (4x4)",
//         "Medium (6x6)", 
//         "Hard (8x8)"
//     };
// };