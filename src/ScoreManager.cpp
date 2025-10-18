/**
 * @file ScoreManager.cpp
 * @brief Implementation of ScoreManager class
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#include "ScoreManager.h"

ScoreManager::ScoreManager()
    : m_currentScore(0)
    , m_highScore(0)
{
    // TODO: Load high score from file
}

ScoreManager::~ScoreManager() {
}

void ScoreManager::calculateScore(int moves, float time) {
    // Simple scoring formula: base score minus moves and time penalties
    int baseScore = 10000;
    int movePenalty = moves * 10;
    int timePenalty = static_cast<int>(time) * 5;
    
    m_currentScore = baseScore - movePenalty - timePenalty;
    if (m_currentScore < 0) {
        m_currentScore = 0;
    }
}

void ScoreManager::saveHighScore(int score, float time) {
    if (score > m_highScore) {
        m_highScore = score;
        // TODO: Save to file
    }
}
