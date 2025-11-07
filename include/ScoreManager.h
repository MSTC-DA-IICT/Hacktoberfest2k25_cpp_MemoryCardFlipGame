#pragma once
#include <string>

class ScoreManager {
public:
    ScoreManager();
    void addMove();
    void addMatch(int comboMultiplier = 1);
    void addMismatch();
    void resetScore();

    int getMoves() const;
    int getMatches() const;
    int getScore() const;
    int getHighScore() const;
    // Load high score from disk (if any) and save when updated
    void loadHighScore();
    void trySaveHighScore();

private:
    int m_moves;
    int m_matches;
    int m_score;
    int m_highScore;
};
