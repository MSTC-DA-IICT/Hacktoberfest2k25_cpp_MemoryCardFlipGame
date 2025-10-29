#include "../include/ScoreManager.h"
#include "../include/Utils.h"
#include <fstream>

static constexpr const char* HIGH_SCORE_FILE = "assets/highscore.txt";

ScoreManager::ScoreManager()
	: m_moves(0), m_matches(0), m_score(0), m_highScore(0) {
	loadHighScore();
}

void ScoreManager::addMove() { m_moves++; }

void ScoreManager::addMatch() {
	m_matches++;
	m_score += 10; // +10 for a match
}

void ScoreManager::addMismatch() {
	m_score -= 4; // -4 for mismatch (allow negative scores)
}

void ScoreManager::resetScore() {
	m_moves = 0;
	m_matches = 0;
	m_score = 0;
}

int ScoreManager::getMoves() const { return m_moves; }
int ScoreManager::getMatches() const { return m_matches; }
int ScoreManager::getScore() const { return m_score; }
int ScoreManager::getHighScore() const { return m_highScore; }

void ScoreManager::loadHighScore() {
	// Try to read high score from file
	std::ifstream in(HIGH_SCORE_FILE);
	if (!in.is_open()) {
		Utils::logInfo("High score file not found, starting at 0");
		m_highScore = 0;
		return;
	}
	int v = 0;
	in >> v;
	if (in.fail()) {
		Utils::logWarning("Failed to parse high score file, resetting to 0");
		m_highScore = 0;
	} else {
		m_highScore = v;
	}
}

void ScoreManager::trySaveHighScore() {
	if (m_score > m_highScore) {
		m_highScore = m_score;
		// Attempt to write to file
		std::ofstream out(HIGH_SCORE_FILE);
		if (!out.is_open()) {
			Utils::logError("Failed to open high score file for writing: " + std::string(HIGH_SCORE_FILE));
			return;
		}
		out << m_highScore;
		Utils::logInfo("New high score saved: " + Utils::toString(m_highScore));
	}
}
