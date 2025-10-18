/**
 * @file GameBoard.h
 * @brief GameBoard class for managing the card grid
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#pragma once

#include <raylib.h>
#include <vector>
#include <memory>
#include "Card.h"

class GameBoard {
public:
    GameBoard(int numCards);
    ~GameBoard();
    
    void update(float deltaTime);
    void draw();
    
    bool allCardsMatched() const;
    
private:
    std::vector<std::unique_ptr<Card>> m_cards;
    int m_numCards;
};
