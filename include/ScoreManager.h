/**
 * @file ScoreManager.h
 * @brief ScoreManager class for managing game scoring
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#pragma once

#include <string>

class ScoreManager {
public:
    ScoreManager();
    ~ScoreManager();
    
    void calculateScore(int moves, float time);
    int getScore() const { return m_currentScore; }
    
    void saveHighScore(int score, float time);
    int getHighScore() const { return m_highScore; }
    
private:
    int m_currentScore;
    int m_highScore;
};
