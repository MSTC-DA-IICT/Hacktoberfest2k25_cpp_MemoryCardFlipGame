#pragma once
#include <vector>
#include <memory>
#include "Card.h"
#include "Utils.h"

// Forward declaration
class AudioManager;

class GameBoard {
public:
    GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds);
    void update(float deltaTime);
    void draw() const;
    void handleClick(Vector2 mousePos);
    bool allMatched() const;
    int getMatchesFound() const { return m_matchesFound; }
    int getComboCount() const { return m_comboCount; }
    float getComboDisplayTime() const { return m_comboDisplayTime; }
    bool isHintActive() const { return m_hintDisplayTime > 0.0f; }
    
    // Shuffle animation control
    void startShuffle(float durationSeconds);
    bool isShuffling() const { return m_isShuffling; }
    float getShuffleDuration() const { return m_shuffleDuration; }
    void setAudioManager(AudioManager* audioManager) { m_audioManager = audioManager; }
    void setScoreManager(class ScoreManager* scoreManager) { m_scoreManager = scoreManager; }
    
    // Hint system
    void showHint();
    bool canUseHint() const { return m_hintsRemaining > 0 && m_hintCooldown <= 0.0f; }
    int getHintsRemaining() const { return m_hintsRemaining; }
    float getHintCooldown() const { return m_hintCooldown; }

private:
    int m_rows;
    int m_cols;
    Vector2 m_cardSize;
    float m_padding;
    Rectangle m_screenBounds;
    std::vector<std::unique_ptr<Card>> m_cards;
    
    Card* m_firstFlippedCard;
    Card* m_secondFlippedCard;
    float m_flipBackTimer;
    bool m_isProcessingMatch;
    int m_matchesFound;
    AudioManager* m_audioManager;
    ScoreManager* m_scoreManager;
    
    // Combo system
    int m_comboCount;
    float m_comboDisplayTime;
    
    // Hint system
    int m_hintsRemaining;
    float m_hintCooldown;
    Card* m_hintCard1;
    Card* m_hintCard2;
    float m_hintDisplayTime;
    bool m_hintAutoFlipBack;
    
    static constexpr float FLIP_BACK_DELAY = 1.0f;
    static constexpr int MAX_HINTS = 3;
    static constexpr float HINT_COOLDOWN = 15.0f; // seconds
    static constexpr float HINT_DISPLAY_DURATION = 3.0f; // seconds
    static constexpr float COMBO_DISPLAY_DURATION = 2.0f; // seconds

    void createCards();
    void checkMatch();
    void resetFlippedCards();
    void findHintPair();
    
    // Shuffle animation state
    bool m_isShuffling = false;
    float m_shuffleDuration = 0.0f;
    float m_shuffleTimer = 0.0f;
    // Movement-based shuffle scheduling
    std::vector<int> m_shuffleOrder; // final order: for each slot index, which original card index will land there
    std::vector<Vector2> m_shuffleTargets; // target positions per original card index
    int m_nextShuffleStartIndex = 0; // next index in m_shuffleOrder to begin moving
    float m_shuffleStartInterval = 0.02f; // stagger between starting each card move
    float m_shuffleMoveDuration = 0.45f; // duration for each card move
};

// #pragma once
// #include <vector>
// #include <memory>
// #include "Card.h"
// #include "Utils.h"

// // Forward declaration
// class AudioManager;

// class GameBoard {
// public:
//     GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds);
//     void update(float deltaTime);
//     void draw() const;
//     void handleClick(Vector2 mousePos);
//     bool allMatched() const;
//     int getMatchesFound() const { return m_matchesFound; }
//     void setAudioManager(AudioManager* audioManager) { m_audioManager = audioManager; } // ADD THIS

// private:
//     int m_rows;
//     int m_cols;
//     Vector2 m_cardSize;
//     float m_padding;
//     Rectangle m_screenBounds;
//     std::vector<std::unique_ptr<Card>> m_cards;
    
//     Card* m_firstFlippedCard;
//     Card* m_secondFlippedCard;
//     float m_flipBackTimer;
//     bool m_isProcessingMatch;
//     int m_matchesFound;
//     AudioManager* m_audioManager;  // ADD THIS
    
//     static constexpr float FLIP_BACK_DELAY = 1.0f;

//     void createCards();
//     void checkMatch();
//     void resetFlippedCards();
// };

// // #pragma once
// // #include <vector>
// // #include <memory>
// // #include "Card.h"
// // #include "Utils.h"

// // class GameBoard {
// // public:
// //     GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds);
// //     void update(float deltaTime);
// //     void draw() const;
// //     void handleClick(Vector2 mousePos);
// //     bool allMatched() const;
// //     int getMatchesFound() const { return m_matchesFound; }  // <-- ADD THIS

// // private:
// //     int m_rows;
// //     int m_cols;
// //     Vector2 m_cardSize;
// //     float m_padding;
// //     Rectangle m_screenBounds;
// //     std::vector<std::unique_ptr<Card>> m_cards;
    
// //     Card* m_firstFlippedCard;
// //     Card* m_secondFlippedCard;
// //     float m_flipBackTimer;
// //     bool m_isProcessingMatch;
// //     int m_matchesFound;  // <-- ADD THIS
    
// //     static constexpr float FLIP_BACK_DELAY = 1.0f;

// //     void createCards();
// //     void checkMatch();
// //     void resetFlippedCards();
// // };

// // // // /**
// // // //  * @file GameBoard.h
// // // //  * @brief GameBoard class managing the grid of cards
// // // //  */

// // // // #pragma once
// // // // #include <vector>
// // // // #include <memory>
// // // // #include "Card.h"
// // // // #include "Utils.h"

// // // // class GameBoard {
// // // // public:
// // // //     GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds);
    
// // // //     void update(float deltaTime);
// // // //     void draw() const;
// // // //     void handleClick(Vector2 mousePos);
// // // //     bool allMatched() const;

// // // // private:
// // // //     int m_rows;
// // // //     int m_cols;
// // // //     Vector2 m_cardSize;
// // // //     float m_padding;
// // // //     Rectangle m_screenBounds;
// // // //     std::vector<std::unique_ptr<Card>> m_cards;
    
// // // //     // Game state for card matching
// // // //     Card* m_firstFlippedCard;
// // // //     Card* m_secondFlippedCard;
// // // //     float m_flipBackTimer;
// // // //     bool m_isProcessingMatch;
    
// // // //     static constexpr float FLIP_BACK_DELAY = 1.0f;

// // // //     void createCards();
// // // //     void checkMatch();
// // // //     void resetFlippedCards();
// // // // };

// // // #pragma once
// // // #include <vector>
// // // #include <memory>
// // // #include "Card.h"
// // // #include "Utils.h"

// // // class GameBoard {
// // // public:
// // //     GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds);
// // //     void update(float deltaTime);
// // //     void draw() const;
// // //     void handleClick(Vector2 mousePos);
// // //     bool allMatched() const;
// // //     int getMatchesFound() const { return m_matchesFound; }  // ADD THIS LINE

// // // private:
// // //     int m_rows;
// // //     int m_cols;
// // //     Vector2 m_cardSize;
// // //     float m_padding;
// // //     Rectangle m_screenBounds;
// // //     std::vector<std::unique_ptr<Card>> m_cards;
    
// // //     Card* m_firstFlippedCard;
// // //     Card* m_secondFlippedCard;
// // //     float m_flipBackTimer;
// // //     bool m_isProcessingMatch;
// // //     int m_matchesFound;  // ADD THIS LINE
    
// // //     static constexpr float FLIP_BACK_DELAY = 1.0f;

// // //     void createCards();
// // //     void checkMatch();
// // //     void resetFlippedCards();
// // // };