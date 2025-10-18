/**
 * @file GameBoard.cpp
 * @brief Implementation of GameBoard class
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#include "GameBoard.h"

GameBoard::GameBoard(int numCards)
    : m_numCards(numCards)
{
    // TODO: Initialize cards
}

GameBoard::~GameBoard() {
}

void GameBoard::update(float deltaTime) {
    for (auto& card : m_cards) {
        card->update(deltaTime);
    }
}

void GameBoard::draw() {
    for (const auto& card : m_cards) {
        card->draw();
    }
}

bool GameBoard::allCardsMatched() const {
    for (const auto& card : m_cards) {
        if (!card->isMatched()) {
            return false;
        }
    }
    return true;
}
