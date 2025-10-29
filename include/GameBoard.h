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
    void setAudioManager(AudioManager* audioManager) { m_audioManager = audioManager; }
    void setScoreManager(class ScoreManager* scoreManager) { m_scoreManager = scoreManager; }

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
    
    static constexpr float FLIP_BACK_DELAY = 1.0f;

    void createCards();
    void checkMatch();
    void resetFlippedCards();
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