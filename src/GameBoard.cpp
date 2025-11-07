// // // // /**
// // // //  * @file GameBoard.cpp
// // // //  * @brief GameBoard class implementation
// // // //  */

// // // // #include "../include/GameBoard.h"
// // // // #include <iostream>

// // // // GameBoard::GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds)
// // // //     : m_rows(rows), 
// // // //       m_cols(cols), 
// // // //       m_cardSize(cardSize), 
// // // //       m_padding(padding), 
// // // //       m_screenBounds(screenBounds),
// // // //       m_firstFlippedCard(nullptr),
// // // //       m_secondFlippedCard(nullptr),
// // // //       m_flipBackTimer(0.0f),
// // // //       m_isProcessingMatch(false)
// // // // {
// // // //     createCards();
// // // // }

// // // // void GameBoard::createCards() {
// // // //     int numPairs = (m_rows * m_cols) / 2;
// // // //     std::vector<int> ids = Utils::createCardPairs(numPairs);
// // // //     Utils::shuffle(ids);

// // // //     m_cards.clear();
// // // //     int index = 0;
// // // //     for (int y = 0; y < m_rows; ++y) {
// // // //         for (int x = 0; x < m_cols; ++x) {
// // // //             Vector2 pos = { 
// // // //                 m_screenBounds.x + x * (m_cardSize.x + m_padding),
// // // //                 m_screenBounds.y + y * (m_cardSize.y + m_padding) 
// // // //             };
// // // //             m_cards.push_back(std::make_unique<Card>(ids[index++], "assets/textures/card.png", pos, m_cardSize));
// // // //         }
// // // //     }
// // // // }

// // // // void GameBoard::update(float deltaTime) {
// // // //     // Update all cards
// // // //     for (auto& card : m_cards) {
// // // //         card->update(deltaTime);
// // // //     }
    
// // // //     // Handle flip-back timer for non-matching cards
// // // //     if (m_isProcessingMatch && m_flipBackTimer > 0.0f) {
// // // //         m_flipBackTimer -= deltaTime;
        
// // // //         if (m_flipBackTimer <= 0.0f) {
// // // //             // Time's up - flip non-matching cards back
// // // //             if (m_firstFlippedCard && m_secondFlippedCard) {
// // // //                 if (m_firstFlippedCard->getId() != m_secondFlippedCard->getId()) {
// // // //                     m_firstFlippedCard->flipDown();
// // // //                     m_secondFlippedCard->flipDown();
// // // //                 }
// // // //             }
// // // //             resetFlippedCards();
// // // //         }
// // // //     }
// // // // }

// // // // void GameBoard::draw() const {
// // // //     for (auto& card : m_cards)
// // // //         card->draw();
// // // // }

// // // // void GameBoard::handleClick(Vector2 mousePos) {
// // // //     // Don't allow clicks while processing a match
// // // //     if (m_isProcessingMatch) {
// // // //         return;
// // // //     }
    
// // // //     // Find clicked card
// // // //     for (auto& card : m_cards) {
// // // //         if (card->containsPoint(mousePos)) {
// // // //             // Can only click face-down cards
// // // //             if (card->getState() == CardState::FACE_DOWN) {
// // // //                 card->flipUp();
                
// // // //                 // Track flipped cards
// // // //                 if (!m_firstFlippedCard) {
// // // //                     m_firstFlippedCard = card.get();
// // // //                     Utils::logDebug("First card flipped: ID " + Utils::toString(card->getId()));
// // // //                 } else if (!m_secondFlippedCard && card.get() != m_firstFlippedCard) {
// // // //                     m_secondFlippedCard = card.get();
// // // //                     Utils::logDebug("Second card flipped: ID " + Utils::toString(card->getId()));
                    
// // // //                     // Check for match after second card is flipped
// // // //                     checkMatch();
// // // //                 }
// // // //             }
// // // //             break; // Only handle one card click at a time
// // // //         }
// // // //     }
// // // // }

// // // // void GameBoard::checkMatch() {
// // // //     if (!m_firstFlippedCard || !m_secondFlippedCard) {
// // // //         return;
// // // //     }
    
// // // //     m_isProcessingMatch = true;
    
// // // //     // Check if the two cards match
// // // //     if (m_firstFlippedCard->getId() == m_secondFlippedCard->getId()) {
// // // //         // Match found!
// // // //         Utils::logInfo("Match found! Card ID: " + Utils::toString(m_firstFlippedCard->getId()));
        
// // // //         m_firstFlippedCard->setMatched();
// // // //         m_secondFlippedCard->setMatched();
        
// // // //         // Reset immediately for matched cards
// // // //         resetFlippedCards();
// // // //     } else {
// // // //         // No match - start timer to flip back
// // // //         Utils::logDebug("No match. Cards will flip back.");
// // // //         m_flipBackTimer = FLIP_BACK_DELAY;
// // // //     }
// // // // }

// // // // void GameBoard::resetFlippedCards() {
// // // //     m_firstFlippedCard = nullptr;
// // // //     m_secondFlippedCard = nullptr;
// // // //     m_flipBackTimer = 0.0f;
// // // //     m_isProcessingMatch = false;
// // // // }

// // // // bool GameBoard::allMatched() const {
// // // //     for (auto& card : m_cards)
// // // //         if (!card->isMatched()) return false;
// // // //     return true;
// // // // }

// // // /**
// // //  * @file GameBoard.cpp
// // //  * @brief GameBoard class implementation
// // //  */

// // // #include "../include/GameBoard.h"
// // // #include <iostream>

// // // GameBoard::GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds)
// // //     : m_rows(rows), 
// // //       m_cols(cols), 
// // //       m_cardSize(cardSize), 
// // //       m_padding(padding), 
// // //       m_screenBounds(screenBounds),
// // //       m_firstFlippedCard(nullptr),
// // //       m_secondFlippedCard(nullptr),
// // //       m_flipBackTimer(0.0f),
// // //       m_isProcessingMatch(false),
// // //       m_matchesFound(0)
// // // {
// // //     createCards();
// // // }

// // // void GameBoard::createCards() {
// // //     int numPairs = (m_rows * m_cols) / 2;
// // //     std::vector<int> ids = Utils::createCardPairs(numPairs);
// // //     Utils::shuffle(ids);

// // //     m_cards.clear();
// // //     int index = 0;
// // //     for (int y = 0; y < m_rows; ++y) {
// // //         for (int x = 0; x < m_cols; ++x) {
// // //             Vector2 pos = { 
// // //                 m_screenBounds.x + x * (m_cardSize.x + m_padding),
// // //                 m_screenBounds.y + y * (m_cardSize.y + m_padding) 
// // //             };
// // //             m_cards.push_back(std::make_unique<Card>(ids[index++], "assets/textures/card.png", pos, m_cardSize));
// // //         }
// // //     }
// // // }

// // // void GameBoard::update(float deltaTime) {
// // //     // Update all cards
// // //     for (auto& card : m_cards) {
// // //         card->update(deltaTime);
// // //     }
    
// // //     // Handle flip-back timer for non-matching cards
// // //     if (m_isProcessingMatch && m_flipBackTimer > 0.0f) {
// // //         m_flipBackTimer -= deltaTime;
        
// // //         if (m_flipBackTimer <= 0.0f) {
// // //             // Time's up - flip non-matching cards back
// // //             if (m_firstFlippedCard && m_secondFlippedCard) {
// // //                 if (m_firstFlippedCard->getId() != m_secondFlippedCard->getId()) {
// // //                     m_firstFlippedCard->flipDown();
// // //                     m_secondFlippedCard->flipDown();
// // //                 }
// // //             }
// // //             resetFlippedCards();
// // //         }
// // //     }
// // // }

// // // void GameBoard::draw() const {
// // //     for (auto& card : m_cards)
// // //         card->draw();
// // // }

// // // void GameBoard::handleClick(Vector2 mousePos) {
// // //     // Don't allow clicks while processing a match
// // //     if (m_isProcessingMatch) {
// // //         return;
// // //     }
    
// // //     // Find clicked card
// // //     for (auto& card : m_cards) {
// // //         if (card->containsPoint(mousePos)) {
// // //             // Can only click face-down cards
// // //             if (card->getState() == CardState::FACE_DOWN) {
// // //                 card->flipUp();
                
// // //                 // Track flipped cards
// // //                 if (!m_firstFlippedCard) {
// // //                     m_firstFlippedCard = card.get();
// // //                     Utils::logDebug("First card flipped: ID " + Utils::toString(card->getId()));
// // //                 } else if (!m_secondFlippedCard && card.get() != m_firstFlippedCard) {
// // //                     m_secondFlippedCard = card.get();
// // //                     Utils::logDebug("Second card flipped: ID " + Utils::toString(card->getId()));
                    
// // //                     // Check for match after second card is flipped
// // //                     checkMatch();
// // //                 }
// // //             }
// // //             break; // Only handle one card click at a time
// // //         }
// // //     }
// // // }

// // // void GameBoard::checkMatch() {
// // //     if (!m_firstFlippedCard || !m_secondFlippedCard) {
// // //         return;
// // //     }
    
// // //     m_isProcessingMatch = true;
    
// // //     // Check if the two cards match
// // //     if (m_firstFlippedCard->getId() == m_secondFlippedCard->getId()) {
// // //         // Match found!
// // //         m_matchesFound++;
// // //         Utils::logInfo("Match found! Card ID: " + Utils::toString(m_firstFlippedCard->getId()));
        
// // //         m_firstFlippedCard->setMatched();
// // //         m_secondFlippedCard->setMatched();
        
// // //         // Reset immediately for matched cards
// // //         resetFlippedCards();
// // //     } else {
// // //         // No match - start timer to flip back
// // //         Utils::logDebug("No match. Cards will flip back.");
// // //         m_flipBackTimer = FLIP_BACK_DELAY;
// // //     }
// // // }

// // // void GameBoard::resetFlippedCards() {
// // //     m_firstFlippedCard = nullptr;
// // //     m_secondFlippedCard = nullptr;
// // //     m_flipBackTimer = 0.0f;
// // //     m_isProcessingMatch = false;
// // // }

// // // bool GameBoard::allMatched() const {
// // //     for (auto& card : m_cards)
// // //         if (!card->isMatched()) return false;
// // //     return true;
// // // }

// // /**
// //  * @file GameBoard.cpp
// //  * @brief GameBoard class implementation
// //  */

// // #include "../include/GameBoard.h"
// // #include "../include/AudioManager.h"
// // #include <iostream>

// // GameBoard::GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds)
// //     : m_rows(rows), 
// //       m_cols(cols), 
// //       m_cardSize(cardSize), 
// //       m_padding(padding), 
// //       m_screenBounds(screenBounds),
// //       m_firstFlippedCard(nullptr),
// //       m_secondFlippedCard(nullptr),
// //       m_flipBackTimer(0.0f),
// //       m_isProcessingMatch(false),
// //       m_matchesFound(0),
// //       m_audioManager(nullptr)  // ADD THIS
// // {
// //     createCards();
// // }

// // void GameBoard::createCards() {
// //     int numPairs = (m_rows * m_cols) / 2;
// //     std::vector<int> ids = Utils::createCardPairs(numPairs);
// //     Utils::shuffle(ids);

// //     m_cards.clear();
// //     int index = 0;
// //     for (int y = 0; y < m_rows; ++y) {
// //         for (int x = 0; x < m_cols; ++x) {
// //             Vector2 pos = { 
// //                 m_screenBounds.x + x * (m_cardSize.x + m_padding),
// //                 m_screenBounds.y + y * (m_cardSize.y + m_padding) 
// //             };
// //             m_cards.push_back(std::make_unique<Card>(ids[index++], "assets/textures/card.png", pos, m_cardSize));
// //         }
// //     }
// // }

// // void GameBoard::update(float deltaTime) {
// //     // Update all cards
// //     for (auto& card : m_cards) {
// //         card->update(deltaTime);
// //     }
    
// //     // Handle flip-back timer for non-matching cards
// //     if (m_isProcessingMatch && m_flipBackTimer > 0.0f) {
// //         m_flipBackTimer -= deltaTime;
        
// //         if (m_flipBackTimer <= 0.0f) {
// //             // Time's up - flip non-matching cards back
// //             if (m_firstFlippedCard && m_secondFlippedCard) {
// //                 if (m_firstFlippedCard->getId() != m_secondFlippedCard->getId()) {
// //                     m_firstFlippedCard->flipDown();
// //                     m_secondFlippedCard->flipDown();
// //                 }
// //             }
// //             resetFlippedCards();
// //         }
// //     }
// // }

// // void GameBoard::draw() const {
// //     for (auto& card : m_cards)
// //         card->draw();
// // }

// // void GameBoard::handleClick(Vector2 mousePos) {
// //     // Don't allow clicks while processing a match
// //     if (m_isProcessingMatch) {
// //         return;
// //     }
    
// //     // Find clicked card
// //     for (auto& card : m_cards) {
// //         if (card->containsPoint(mousePos)) {
// //             // Can only click face-down cards
// //             if (card->getState() == CardState::FACE_DOWN) {
// //                 card->flipUp();
                
// //                 // Play flip sound
// //                 if (m_audioManager) {
// //                     m_audioManager->playFlip();
// //                 }
                
// //                 // Track flipped cards
// //                 if (!m_firstFlippedCard) {
// //                     m_firstFlippedCard = card.get();
// //                     Utils::logDebug("First card flipped: ID " + Utils::toString(card->getId()));
// //                 } else if (!m_secondFlippedCard && card.get() != m_firstFlippedCard) {
// //                     m_secondFlippedCard = card.get();
// //                     Utils::logDebug("Second card flipped: ID " + Utils::toString(card->getId()));
                    
// //                     // Check for match after second card is flipped
// //                     checkMatch();
// //                 }
// //             }
// //             break; // Only handle one card click at a time
// //         }
// //     }
// // }

// // void GameBoard::checkMatch() {
// //     if (!m_firstFlippedCard || !m_secondFlippedCard) {
// //         return;
// //     }
    
// //     m_isProcessingMatch = true;
    
// //     // Check if the two cards match
// //     if (m_firstFlippedCard->getId() == m_secondFlippedCard->getId()) {
// //         // Match found!
// //         m_matchesFound++;
        
// //         // Play match sound
// //         if (m_audioManager) {
// //             m_audioManager->playMatch();
// //         }
        
// //         Utils::logInfo("Match found! Card ID: " + Utils::toString(m_firstFlippedCard->getId()));
        
// //         m_firstFlippedCard->setMatched();
// //         m_secondFlippedCard->setMatched();
        
// //         // Reset immediately for matched cards
// //         resetFlippedCards();
// //     } else {
// //         // No match - start timer to flip back
// //         Utils::logDebug("No match. Cards will flip back.");
// //         m_flipBackTimer = FLIP_BACK_DELAY;
// //     }
// // }

// // void GameBoard::resetFlippedCards() {
// //     m_firstFlippedCard = nullptr;
// //     m_secondFlippedCard = nullptr;
// //     m_flipBackTimer = 0.0f;
// //     m_isProcessingMatch = false;
// // }

// // bool GameBoard::allMatched() const {
// //     for (auto& card : m_cards)
// //         if (!card->isMatched()) return false;
// //     return true;
// // }

// /**
//  * @file GameBoard.cpp
//  * @brief GameBoard class implementation
//  */

// // #include "../include/GameBoard.h"
// // #include "../include/AudioManager.h"
// // #include <iostream>

// // GameBoard::GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds)
// //     : m_rows(rows), 
// //       m_cols(cols), 
// //       m_cardSize(cardSize), 
// //       m_padding(padding), 
// //       m_screenBounds(screenBounds),
// //       m_firstFlippedCard(nullptr),
// //       m_secondFlippedCard(nullptr),
// //       m_flipBackTimer(0.0f),
// //       m_isProcessingMatch(false),
// //       m_matchesFound(0),
// //       m_audioManager(nullptr)
// // {
// //     createCards();
// // }

// // void GameBoard::createCards() {
// //     int numPairs = (m_rows * m_cols) / 2;
// //     std::vector<int> ids = Utils::createCardPairs(numPairs);
// //     Utils::shuffle(ids);

// //     m_cards.clear();
// //     int index = 0;
// //     for (int y = 0; y < m_rows; ++y) {
// //         for (int x = 0; x < m_cols; ++x) {
// //             Vector2 pos = { 
// //                 m_screenBounds.x + x * (m_cardSize.x + m_padding),
// //                 m_screenBounds.y + y * (m_cardSize.y + m_padding) 
// //             };
// //             m_cards.push_back(std::make_unique<Card>(ids[index++], "assets/textures/card.png", pos, m_cardSize));
// //         }
// //     }
// // }

// // void GameBoard::update(float deltaTime) {
// //     // Update all cards
// //     for (auto& card : m_cards) {
// //         card->update(deltaTime);
// //     }
    
// //     // Handle flip-back timer for non-matching cards
// //     if (m_isProcessingMatch && m_flipBackTimer > 0.0f) {
// //         m_flipBackTimer -= deltaTime;
        
// //         if (m_flipBackTimer <= 0.0f) {
// //             // Time's up - flip non-matching cards back
// //             if (m_firstFlippedCard && m_secondFlippedCard) {
// //                 if (m_firstFlippedCard->getId() != m_secondFlippedCard->getId()) {
// //                     m_firstFlippedCard->flipDown();
// //                     m_secondFlippedCard->flipDown();
// //                 }
// //             }
// //             resetFlippedCards();
// //         }
// //     }
// // }

// // void GameBoard::draw() const {
// //     for (auto& card : m_cards)
// //         card->draw();
// // }

// // void GameBoard::handleClick(Vector2 mousePos) {
// //     // Don't allow clicks while processing a match
// //     if (m_isProcessingMatch) {
// //         return;
// //     }
    
// //     // Find clicked card
// //     for (auto& card : m_cards) {
// //         if (card->containsPoint(mousePos)) {
// //             // Can only click face-down cards
// //             if (card->getState() == CardState::FACE_DOWN) {
// //                 card->flipUp();
                
// //                 // Play flip sound
// //                 if (m_audioManager) {
// //                     m_audioManager->playFlip();
// //                 }
                
// //                 // Track flipped cards
// //                 if (!m_firstFlippedCard) {
// //                     m_firstFlippedCard = card.get();
// //                     Utils::logDebug("First card flipped: ID " + Utils::toString(card->getId()));
// //                 } else if (!m_secondFlippedCard && card.get() != m_firstFlippedCard) {
// //                     m_secondFlippedCard = card.get();
// //                     Utils::logDebug("Second card flipped: ID " + Utils::toString(card->getId()));
                    
// //                     // Check for match after second card is flipped
// //                     checkMatch();
// //                 }
// //             }
// //             break; // Only handle one card click at a time
// //         }
// //     }
// // }

// // void GameBoard::checkMatch() {
// //     if (!m_firstFlippedCard || !m_secondFlippedCard) {
// //         return;
// //     }
    
// //     m_isProcessingMatch = true;
    
// //     // Check if the two cards match
// //     if (m_firstFlippedCard->getId() == m_secondFlippedCard->getId()) {
// //         // Match found!
// //         m_matchesFound++;
        
// //         // Play match sound
// //         if (m_audioManager) {
// //             m_audioManager->playMatch();
// //         }
        
// //         Utils::logInfo("Match found! Card ID: " + Utils::toString(m_firstFlippedCard->getId()) + 
// //                       " | Total matches: " + Utils::toString(m_matchesFound));
        
// //         m_firstFlippedCard->setMatched();
// //         m_secondFlippedCard->setMatched();
        
// //         // Reset immediately for matched cards
// //         resetFlippedCards();
// //     } else {
// //         // No match - start timer to flip back
// //         Utils::logDebug("No match. Cards will flip back.");
// //         m_flipBackTimer = FLIP_BACK_DELAY;
// //     }
// // }

// // void GameBoard::resetFlippedCards() {
// //     m_firstFlippedCard = nullptr;
// //     m_secondFlippedCard = nullptr;
// //     m_flipBackTimer = 0.0f;
// //     m_isProcessingMatch = false;
// // }

// // bool GameBoard::allMatched() const {
// //     for (auto& card : m_cards)
// //         if (!card->isMatched()) return false;
// //     return true;
// // }

// /**
//  * @file GameBoard.cpp
//  * @brief GameBoard class implementation
//  */

// #include "../include/GameBoard.h"
// #include "../include/AudioManager.h"
// #include <iostream>

// GameBoard::GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds)
//     : m_rows(rows), 
//       m_cols(cols), 
//       m_cardSize(cardSize), 
//       m_padding(padding), 
//       m_screenBounds(screenBounds),
//       m_firstFlippedCard(nullptr),
//       m_secondFlippedCard(nullptr),
//       m_flipBackTimer(0.0f),
//       m_isProcessingMatch(false),
//       m_matchesFound(0),
//       m_audioManager(nullptr)
// {
//     createCards();
// }

// void GameBoard::createCards() {
//     int numPairs = (m_rows * m_cols) / 2;
//     std::vector<int> ids = Utils::createCardPairs(numPairs);
//     Utils::shuffle(ids);

//     m_cards.clear();
//     int index = 0;
//     for (int y = 0; y < m_rows; ++y) {
//         for (int x = 0; x < m_cols; ++x) {
//             Vector2 pos = { 
//                 m_screenBounds.x + x * (m_cardSize.x + m_padding),
//                 m_screenBounds.y + y * (m_cardSize.y + m_padding) 
//             };
//             m_cards.push_back(std::make_unique<Card>(ids[index++], "assets/textures/card.png", pos, m_cardSize));
//         }
//     }
// }

// void GameBoard::update(float deltaTime) {
//     // Update all cards
//     for (auto& card : m_cards) {
//         card->update(deltaTime);
//     }
    
//     // Handle flip-back timer for non-matching cards
//     if (m_isProcessingMatch && m_flipBackTimer > 0.0f) {
//         m_flipBackTimer -= deltaTime;
        
//         if (m_flipBackTimer <= 0.0f) {
//             // Time's up - flip non-matching cards back
//             if (m_firstFlippedCard && m_secondFlippedCard) {
//                 if (m_firstFlippedCard->getId() != m_secondFlippedCard->getId()) {
//                     m_firstFlippedCard->flipDown();
//                     m_secondFlippedCard->flipDown();
//                 }
//             }
//             resetFlippedCards();
//         }
//     }
// }

// void GameBoard::draw() const {
//     for (auto& card : m_cards)
//         card->draw();
// }

// void GameBoard::handleClick(Vector2 mousePos) {
//     // Don't allow clicks while processing a match
//     if (m_isProcessingMatch) {
//         return;
//     }
    
//     // Find clicked card
//     for (auto& card : m_cards) {
//         if (card->containsPoint(mousePos)) {
//             // Can only click face-down cards
//             if (card->getState() == CardState::FACE_DOWN) {
//                 card->flipUp();
                
//                 // Play flip sound
//                 if (m_audioManager) {
//                     m_audioManager->playFlip();
//                 }
                
//                 // Track flipped cards
//                 if (!m_firstFlippedCard) {
//                     m_firstFlippedCard = card.get();
//                     Utils::logDebug("First card flipped: ID " + Utils::toString(card->getId()));
//                 } else if (!m_secondFlippedCard && card.get() != m_firstFlippedCard) {
//                     m_secondFlippedCard = card.get();
//                     Utils::logDebug("Second card flipped: ID " + Utils::toString(card->getId()));
                    
//                     // Check for match after second card is flipped
//                     checkMatch();
//                 }
//             }
//             break; // Only handle one card click at a time
//         }
//     }
// }

// void GameBoard::checkMatch() {
//     if (!m_firstFlippedCard || !m_secondFlippedCard) {
//         return;
//     }
    
//     m_isProcessingMatch = true;
    
//     // Check if the two cards match
//     if (m_firstFlippedCard->getId() == m_secondFlippedCard->getId()) {
//         // Match found!
//         m_matchesFound++;
        
//         // Play match sound
//         if (m_audioManager) {
//             m_audioManager->playMatch();
//         }
        
//         Utils::logInfo("Match found! Card ID: " + Utils::toString(m_firstFlippedCard->getId()) + 
//                       " | Total matches: " + Utils::toString(m_matchesFound));
        
//         m_firstFlippedCard->setMatched();
//         m_secondFlippedCard->setMatched();
        
//         // Reset immediately for matched cards
//         resetFlippedCards();
//     } else {
//         // No match - start timer to flip back
//         Utils::logDebug("No match. Cards will flip back.");
//         m_flipBackTimer = FLIP_BACK_DELAY;
//     }
// }

// void GameBoard::resetFlippedCards() {
//     m_firstFlippedCard = nullptr;
//     m_secondFlippedCard = nullptr;
//     m_flipBackTimer = 0.0f;
//     m_isProcessingMatch = false;
// }

// bool GameBoard::allMatched() const {
//     for (auto& card : m_cards)
//         if (!card->isMatched()) return false;
//     return true;
// }

/**
 * @file GameBoard.cpp
 * @brief GameBoard class implementation
 */

#include "../include/GameBoard.h"
#include "../include/AudioManager.h"
#include "../include/ScoreManager.h"
#include <algorithm>
#include <cmath>

GameBoard::GameBoard(int rows, int cols, Vector2 cardSize, float padding, Rectangle screenBounds)
    : m_rows(rows), 
      m_cols(cols), 
      m_cardSize(cardSize), 
      m_padding(padding), 
      m_screenBounds(screenBounds),
      m_firstFlippedCard(nullptr),
      m_secondFlippedCard(nullptr),
      m_flipBackTimer(0.0f),
      m_isProcessingMatch(false),
      m_matchesFound(0),
      m_audioManager(nullptr),
      m_scoreManager(nullptr),
      m_comboCount(0),
      m_comboDisplayTime(0.0f),
      m_hintsRemaining(MAX_HINTS),
      m_hintCooldown(0.0f),
      m_hintCard1(nullptr),
      m_hintCard2(nullptr),
      m_hintDisplayTime(0.0f),
      m_hintAutoFlipBack(false)
{
    Utils::logInfo("GameBoard constructor called");
    createCards();
}

void GameBoard::startShuffle(float durationSeconds) {
    if (m_cards.empty()) {
        return;
    }

    std::vector<int> movableIndices;
    movableIndices.reserve(m_cards.size());
    std::vector<Vector2> availablePositions;
    availablePositions.reserve(m_cards.size());

    for (size_t i = 0; i < m_cards.size(); ++i) {
        Card* card = m_cards[i].get();
        if (card->isMatched()) {
            continue;
        }
        movableIndices.push_back(static_cast<int>(i));
        availablePositions.push_back(card->getPosition());
    }

    if (movableIndices.size() <= 1) {
        Utils::logInfo("Shuffle skipped - insufficient unmatched cards");
        return;
    }

    Utils::shuffle(movableIndices);
    Utils::shuffle(availablePositions);

    m_isShuffling = true;
    m_shuffleDuration = durationSeconds;
    m_shuffleTimer = 0.0f;
    m_nextShuffleStartIndex = 0;
    m_shuffleOrder = movableIndices;

    m_shuffleTargets.assign(m_cards.size(), Vector2{});
    for (size_t i = 0; i < m_cards.size(); ++i) {
        m_shuffleTargets[i] = m_cards[i]->getPosition();
    }

    for (size_t i = 0; i < movableIndices.size(); ++i) {
        int cardIdx = movableIndices[i];
        m_shuffleTargets[cardIdx] = availablePositions[i];
    }

    // Reset in-progress selections and combo since layout is changing
    if (m_firstFlippedCard || m_secondFlippedCard) {
        if (m_firstFlippedCard && !m_firstFlippedCard->isMatched() && m_firstFlippedCard->isRevealed()) {
            m_firstFlippedCard->flipDown();
        }
        if (m_secondFlippedCard && !m_secondFlippedCard->isMatched() && m_secondFlippedCard->isRevealed()) {
            m_secondFlippedCard->flipDown();
        }
        resetFlippedCards();
    }

    for (int index : movableIndices) {
        if (!m_cards[index]->isMatched() && m_cards[index]->isRevealed()) {
            m_cards[index]->flipDown();
        }
    }

    // Clear any active hints when reshuffling occurs
    m_hintDisplayTime = 0.0f;
    m_hintCard1 = nullptr;
    m_hintCard2 = nullptr;
    m_hintAutoFlipBack = false;

    m_comboCount = 0;
    m_comboDisplayTime = 0.0f;

    Utils::logInfo("Position shuffle started: duration=" + Utils::toString(m_shuffleDuration) +
                   " cards=" + Utils::toString(static_cast<int>(movableIndices.size())));
}

void GameBoard::createCards() {
    int numPairs = (m_rows * m_cols) / 2;
    std::vector<int> ids = Utils::createCardPairs(numPairs);
    Utils::shuffle(ids);

    m_cards.clear();
    int index = 0;
    for (int y = 0; y < m_rows; ++y) {
        for (int x = 0; x < m_cols; ++x) {
            Vector2 pos = { 
                m_screenBounds.x + x * (m_cardSize.x + m_padding),
                m_screenBounds.y + y * (m_cardSize.y + m_padding) 
            };
            m_cards.push_back(std::make_unique<Card>(ids[index++], "assets/textures/card.png", pos, m_cardSize));
        }
    }
    Utils::logInfo("Created " + Utils::toString(m_rows * m_cols) + " cards");
}

void GameBoard::update(float deltaTime) {
    // Update all cards
    for (auto& card : m_cards) {
        card->update(deltaTime);
    }
    
    // Update combo display timer
    if (m_comboDisplayTime > 0.0f) {
        m_comboDisplayTime -= deltaTime;
        if (m_comboDisplayTime <= 0.0f) {
            m_comboDisplayTime = 0.0f;
        }
    }
    
    // Update hint cooldown
    if (m_hintCooldown > 0.0f) {
        m_hintCooldown -= deltaTime;
        if (m_hintCooldown < 0.0f) {
            m_hintCooldown = 0.0f;
        }
    }
    
    // Update hint display timer
    if (m_hintDisplayTime > 0.0f) {
        m_hintDisplayTime -= deltaTime;
        if (m_hintDisplayTime <= 0.0f) {
            m_hintDisplayTime = 0.0f;
            if (m_hintAutoFlipBack) {
                if (m_hintCard1 && !m_hintCard1->isMatched() && m_hintCard1->isRevealed()) {
                    m_hintCard1->flipDown();
                }
                if (m_hintCard2 && !m_hintCard2->isMatched() && m_hintCard2->isRevealed()) {
                    m_hintCard2->flipDown();
                }
            }
            m_hintCard1 = nullptr;
            m_hintCard2 = nullptr;
            m_hintAutoFlipBack = false;
        }
    }

    // Handle position-based shuffle animation
    if (m_isShuffling) {
        m_shuffleTimer += deltaTime;

        const int totalToShuffle = static_cast<int>(m_shuffleOrder.size());
        const int cardCount = static_cast<int>(m_cards.size());

        // Start moves in a staggered fashion based on start interval
        while (m_nextShuffleStartIndex < totalToShuffle &&
               m_shuffleTimer >= m_nextShuffleStartIndex * m_shuffleStartInterval) {
            int cardIndex = m_shuffleOrder[m_nextShuffleStartIndex];
            if (cardIndex >= 0 && cardIndex < cardCount) {
                m_cards[cardIndex]->moveTo(m_shuffleTargets[cardIndex], m_shuffleMoveDuration);
            }
            m_nextShuffleStartIndex++;
        }

        // Check if all moves have been started and completed
        if (m_nextShuffleStartIndex >= totalToShuffle) {
            bool anyMoving = false;
            for (auto& card : m_cards) {
                if (!card->isMatched() && card->isMoving()) {
                    anyMoving = true;
                    break;
                }
            }

            // End shuffle when no cards are moving
            if (!anyMoving) {
                m_isShuffling = false;
                m_shuffleTimer = 0.0f;
                m_nextShuffleStartIndex = 0;
                m_shuffleTargets.clear();
                m_shuffleOrder.clear();
                Utils::logInfo("Position shuffle completed");
            }
        }

        // While shuffling, do not process match flipback logic or accept clicks
        return;
    }
    
    // Handle flip-back timer for non-matching cards
    if (m_isProcessingMatch && m_flipBackTimer > 0.0f) {
        m_flipBackTimer -= deltaTime;
        
        if (m_flipBackTimer <= 0.0f) {
            // Time's up - flip non-matching cards back
            if (m_firstFlippedCard && m_secondFlippedCard) {
                if (m_firstFlippedCard->getId() != m_secondFlippedCard->getId()) {
                    m_firstFlippedCard->flipDown();
                    m_secondFlippedCard->flipDown();
                }
            }
            resetFlippedCards();
        }
    }
}

void GameBoard::draw() const {
    for (auto& card : m_cards)
        card->draw();
    
    // Draw hint highlighting
    if (m_hintDisplayTime > 0.0f && m_hintCard1 && m_hintCard2) {
        Rectangle r1 = m_hintCard1->getBounds();
        Rectangle r2 = m_hintCard2->getBounds();
        float alpha = 0.5f + 0.3f * sin(GetTime() * 5.0f); // Pulsing effect
        Color hintColor = ColorAlpha(YELLOW, alpha);
        DrawRectangleLinesEx(r1, 4.0f, hintColor);
        DrawRectangleLinesEx(r2, 4.0f, hintColor);
    }
}

void GameBoard::handleClick(Vector2 mousePos) {
    // Don't allow clicks while processing a match
    if (m_isProcessingMatch || m_isShuffling || (m_hintDisplayTime > 0.0f && m_hintAutoFlipBack)) {
        Utils::logDebug("Click ignored - board temporarily locked");
        return;
    }
    
    // Find clicked card
    for (auto& card : m_cards) {
        if (card->containsPoint(mousePos)) {
            // Can only click face-down cards
            if (card->getState() == CardState::FACE_DOWN) {
                card->flipUp();
                
                // Play flip sound - DEBUG VERSION
                if (m_audioManager) {
                    Utils::logInfo("Audio manager exists, calling playFlip()");
                    m_audioManager->playFlip();
                } else {
                    Utils::logError("Audio manager is NULL! Cannot play sound.");
                }
                
                // Track flipped cards
                if (!m_firstFlippedCard) {
                    m_firstFlippedCard = card.get();
                    Utils::logDebug("First card flipped: ID " + Utils::toString(card->getId()));
                } else if (!m_secondFlippedCard && card.get() != m_firstFlippedCard) {
                    m_secondFlippedCard = card.get();
                    Utils::logDebug("Second card flipped: ID " + Utils::toString(card->getId()));
                    
                    // Check for match after second card is flipped
                    checkMatch();
                }
            }
            break; // Only handle one card click at a time
        }
    }
}

void GameBoard::checkMatch() {
    if (!m_firstFlippedCard || !m_secondFlippedCard) {
        return;
    }
    
    m_isProcessingMatch = true;
    
    // Check if the two cards match
    if (m_firstFlippedCard->getId() == m_secondFlippedCard->getId()) {
        // Match found!
        m_matchesFound++;
        
        // Increment combo
        m_comboCount++;
        m_comboDisplayTime = COMBO_DISPLAY_DURATION;
        
        // Calculate combo multiplier (1x, 2x, 3x, etc., max 5x)
        int comboMultiplier = std::min(m_comboCount, 5);
        
        // Play match sound - DEBUG VERSION
        if (m_audioManager) {
            Utils::logInfo("Match found! Audio manager exists, calling playMatch()");
            m_audioManager->playMatch();
        } else {
            Utils::logError("Audio manager is NULL! Cannot play match sound.");
        }
        
        Utils::logInfo("Match found! Card ID: " + Utils::toString(m_firstFlippedCard->getId()) + 
                      " | Total matches: " + Utils::toString(m_matchesFound) +
                      " | Combo: " + Utils::toString(m_comboCount) + "x");
        
        m_firstFlippedCard->setMatched();
        m_secondFlippedCard->setMatched();
        // Update score manager with combo multiplier
        if (m_scoreManager) {
            m_scoreManager->addMatch(comboMultiplier);
        }
        
        // Reset immediately for matched cards
        resetFlippedCards();
    } else {
        // No match - reset combo
        m_comboCount = 0;
        m_comboDisplayTime = 0.0f;
        
        // No match - start timer to flip back
        Utils::logDebug("No match. Cards will flip back.");
        m_flipBackTimer = FLIP_BACK_DELAY;
        // Penalty for mismatch
        if (m_scoreManager) {
            m_scoreManager->addMismatch();
        }
    }
}

void GameBoard::resetFlippedCards() {
    m_firstFlippedCard = nullptr;
    m_secondFlippedCard = nullptr;
    m_flipBackTimer = 0.0f;
    m_isProcessingMatch = false;
}

bool GameBoard::allMatched() const {
    for (auto& card : m_cards)
        if (!card->isMatched()) return false;
    return true;
}

void GameBoard::findHintPair() {
    // Find two face-down cards with matching IDs
    m_hintCard1 = nullptr;
    m_hintCard2 = nullptr;
    
    for (size_t i = 0; i < m_cards.size(); ++i) {
        if (m_cards[i]->isMatched() || m_cards[i]->isRevealed()) continue;
        
        int id = m_cards[i]->getId();
        
        // Look for a matching card
        for (size_t j = i + 1; j < m_cards.size(); ++j) {
            if (m_cards[j]->isMatched() || m_cards[j]->isRevealed()) continue;
            
            if (m_cards[j]->getId() == id) {
                m_hintCard1 = m_cards[i].get();
                m_hintCard2 = m_cards[j].get();
                return;
            }
        }
    }
}

void GameBoard::showHint() {
    if (!canUseHint()) return;
    if (m_isShuffling || m_isProcessingMatch || m_firstFlippedCard || m_secondFlippedCard) return;
    
    findHintPair();
    
    if (m_hintCard1 && m_hintCard2) {
        m_hintDisplayTime = HINT_DISPLAY_DURATION;
        m_hintsRemaining--;
        m_hintCooldown = HINT_COOLDOWN;
        m_hintAutoFlipBack = true;
        
        // Penalty for using hint
        if (m_scoreManager) {
            m_scoreManager->addMismatch(); // Deduct points for using hint
        }
        m_comboCount = 0;
        m_comboDisplayTime = 0.0f;

        if (!m_hintCard1->isRevealed()) {
            m_hintCard1->flipUp();
        }
        if (!m_hintCard2->isRevealed()) {
            m_hintCard2->flipUp();
        }

        Utils::logInfo("Hint shown! Remaining hints: " + Utils::toString(m_hintsRemaining));
    }
}