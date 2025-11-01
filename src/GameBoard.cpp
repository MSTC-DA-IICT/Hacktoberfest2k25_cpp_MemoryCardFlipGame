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
#include <iostream>
#include <algorithm>

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
            m_scoreManager(nullptr)
{
    Utils::logInfo("GameBoard constructor called");
    createCards();
}

void GameBoard::startShuffle(float durationSeconds) {
    if (m_cards.empty()) return;
    m_isShuffling = true;
    m_shuffleDuration = durationSeconds;
    m_shuffleTimer = 0.0f;
    m_nextShuffleStartIndex = 0;

    // Compute grid slot positions
    int n = static_cast<int>(m_cards.size());
    std::vector<Vector2> slotPositions; slotPositions.reserve(n);
    for (int y = 0; y < m_rows; ++y) {
        for (int x = 0; x < m_cols; ++x) {
            Vector2 pos = { 
                m_screenBounds.x + x * (m_cardSize.x + m_padding),
                m_screenBounds.y + y * (m_cardSize.y + m_padding)
            };
            slotPositions.push_back(pos);
        }
    }

    // Create a shuffled list of slot positions and assign targets per current card index
    std::vector<int> shuffledSlots(n);
    for (int i = 0; i < n; ++i) shuffledSlots[i] = i;
    Utils::shuffle(shuffledSlots);

    m_shuffleTargets.clear();
    m_shuffleTargets.resize(n);
    for (int i = 0; i < n; ++i) {
        int slotIdx = shuffledSlots[i];
        m_shuffleTargets[i] = slotPositions[slotIdx];
    }

    // Ensure all non-matched cards are face-down before moving
    for (auto& card : m_cards) {
        if (!card->isMatched() && card->getState() == CardState::FACE_UP) {
            card->flipDown();
        }
    }

    Utils::logInfo("Position shuffle started: duration=" + Utils::toString(m_shuffleDuration) + " cards=" + Utils::toString(n));
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

    // Handle position-based shuffle animation
    if (m_isShuffling) {
        m_shuffleTimer += deltaTime;

        int n = static_cast<int>(m_cards.size());

        // Start moves in a staggered fashion based on start interval
        while (m_nextShuffleStartIndex < n && m_shuffleTimer >= m_nextShuffleStartIndex * m_shuffleStartInterval) {
            int idx = m_nextShuffleStartIndex;
            if (idx >= 0 && idx < n) {
                if (!m_cards[idx]->isMatched()) {
                    m_cards[idx]->moveTo(m_shuffleTargets[idx], m_shuffleMoveDuration);
                }
            }
            m_nextShuffleStartIndex++;
        }

        // Check if all moves have been started and completed
        if (m_nextShuffleStartIndex >= n) {
            bool anyMoving = false;
            for (auto& card : m_cards) {
                if (card->isMoving()) {
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
}

void GameBoard::handleClick(Vector2 mousePos) {
    // Don't allow clicks while processing a match
    if (m_isProcessingMatch) {
        Utils::logDebug("Click ignored - processing match");
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
        
        // Play match sound - DEBUG VERSION
        if (m_audioManager) {
            Utils::logInfo("Match found! Audio manager exists, calling playMatch()");
            m_audioManager->playMatch();
        } else {
            Utils::logError("Audio manager is NULL! Cannot play match sound.");
        }
        
        Utils::logInfo("Match found! Card ID: " + Utils::toString(m_firstFlippedCard->getId()) + 
                      " | Total matches: " + Utils::toString(m_matchesFound));
        
        m_firstFlippedCard->setMatched();
        m_secondFlippedCard->setMatched();
        // Update score manager
        if (m_scoreManager) {
            m_scoreManager->addMatch();
        }
        
        // Reset immediately for matched cards
        resetFlippedCards();
    } else {
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