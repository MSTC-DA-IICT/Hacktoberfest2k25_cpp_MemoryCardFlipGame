/**
 * @file Utils.cpp
 * @brief Implementation of utility functions for the Memory Card Game
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>

// Initialize static members
std::mt19937 Utils::s_rng;
bool Utils::s_rngInitialized = false;
std::chrono::high_resolution_clock::time_point Utils::s_startTime;
bool Utils::s_startTimeInitialized = false;

// === LOGGING FUNCTIONS ===

void Utils::logInfo(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

void Utils::logWarning(const std::string& message) {
    std::cout << "[WARNING] " << message << std::endl;
}

void Utils::logError(const std::string& message) {
    std::cerr << "[ERROR] " << message << std::endl;
}

void Utils::logDebug(const std::string& message) {
    #ifdef DEBUG
    std::cout << "[DEBUG] " << message << std::endl;
    #endif
}

// === MATH UTILITIES ===

void Utils::initializeRNG() {
    std::random_device rd;
    s_rng.seed(rd());
    s_rngInitialized = true;
}

int Utils::randomInt(int min, int max) {
    if (!s_rngInitialized) {
        initializeRNG();
    }
    std::uniform_int_distribution<int> dist(min, max);
    return dist(s_rng);
}

float Utils::randomFloat(float min, float max) {
    if (!s_rngInitialized) {
        initializeRNG();
    }
    std::uniform_real_distribution<float> dist(min, max);
    return dist(s_rng);
}

float Utils::lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float Utils::clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float Utils::distance(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

float Utils::distanceSquared(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return dx * dx + dy * dy;
}

// === VECTOR UTILITIES ===

Vector2 Utils::vector2(float value) {
    return {value, value};
}

Vector2 Utils::vector2Add(Vector2 a, Vector2 b) {
    return {a.x + b.x, a.y + b.y};
}

Vector2 Utils::vector2Subtract(Vector2 a, Vector2 b) {
    return {a.x - b.x, a.y - b.y};
}

Vector2 Utils::vector2Scale(Vector2 vector, float scalar) {
    return {vector.x * scalar, vector.y * scalar};
}

// === STRING UTILITIES ===

std::string Utils::toString(int value) {
    return std::to_string(value);
}

std::string Utils::toString(float value, int precision) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

std::string Utils::formatTime(float seconds) {
    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds / 60;
    int secs = totalSeconds % 60;
    
    std::ostringstream out;
    out << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << secs;
    return out.str();
}

std::string Utils::toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string Utils::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// === FILE UTILITIES ===

bool Utils::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

std::string Utils::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool Utils::writeFile(const std::string& filename, const std::string& contents) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << contents;
    return true;
}

std::string Utils::getDirectory(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return "";
    }
    return filepath.substr(0, pos);
}

std::string Utils::getFilename(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos + 1);
}

// === ARRAY/VECTOR UTILITIES ===

std::vector<int> Utils::range(int start, int end) {
    std::vector<int> result;
    for (int i = start; i < end; i++) {
        result.push_back(i);
    }
    return result;
}

std::vector<int> Utils::createCardPairs(int numPairs) {
    std::vector<int> pairs;
    for (int i = 0; i < numPairs; i++) {
        pairs.push_back(i);
        pairs.push_back(i);
    }
    shuffle(pairs);
    return pairs;
}

// === TIME UTILITIES ===

void Utils::initializeStartTime() {
    s_startTime = std::chrono::high_resolution_clock::now();
    s_startTimeInitialized = true;
}

long long Utils::getCurrentTimestamp() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return duration.count();
}

float Utils::getElapsedTime() {
    if (!s_startTimeInitialized) {
        initializeStartTime();
    }
    
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_startTime);
    return duration.count() / 1000.0f;
}

// === COLOR UTILITIES ===

Color Utils::colorLerp(Color color1, Color color2, float t) {
    t = clamp(t, 0.0f, 1.0f);
    return Color{
        static_cast<unsigned char>(lerp(color1.r, color2.r, t)),
        static_cast<unsigned char>(lerp(color1.g, color2.g, t)),
        static_cast<unsigned char>(lerp(color1.b, color2.b, t)),
        static_cast<unsigned char>(lerp(color1.a, color2.a, t))
    };
}

Color Utils::colorFromHSV(float hue, float saturation, float value) {
    float h = hue / 60.0f;
    float c = value * saturation;
    float x = c * (1.0f - std::abs(std::fmod(h, 2.0f) - 1.0f));
    float m = value - c;
    
    float r, g, b;
    if (h >= 0 && h < 1) {
        r = c; g = x; b = 0;
    } else if (h >= 1 && h < 2) {
        r = x; g = c; b = 0;
    } else if (h >= 2 && h < 3) {
        r = 0; g = c; b = x;
    } else if (h >= 3 && h < 4) {
        r = 0; g = x; b = c;
    } else if (h >= 4 && h < 5) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }
    
    return Color{
        static_cast<unsigned char>((r + m) * 255),
        static_cast<unsigned char>((g + m) * 255),
        static_cast<unsigned char>((b + m) * 255),
        255
    };
}

Color Utils::adjustBrightness(Color color, float factor) {
    return Color{
        static_cast<unsigned char>(clamp(color.r * factor, 0.0f, 255.0f)),
        static_cast<unsigned char>(clamp(color.g * factor, 0.0f, 255.0f)),
        static_cast<unsigned char>(clamp(color.b * factor, 0.0f, 255.0f)),
        color.a
    };
}

// === GAME-SPECIFIC UTILITIES ===

Vector2 Utils::calculateGridDimensions(int numCards) {
    // Calculate square root to get grid dimensions
    int gridSize = static_cast<int>(std::sqrt(numCards));
    
    // Ensure even grid
    if (gridSize * gridSize < numCards) {
        gridSize++;
    }
    
    return {static_cast<float>(gridSize), static_cast<float>(gridSize)};
}

std::vector<Vector2> Utils::calculateCardPositions(int gridWidth, int gridHeight,
                                                   Vector2 cardSize, float padding,
                                                   Rectangle screenBounds) {
    std::vector<Vector2> positions;
    
    float totalWidth = gridWidth * cardSize.x + (gridWidth - 1) * padding;
    float totalHeight = gridHeight * cardSize.y + (gridHeight - 1) * padding;
    
    float startX = screenBounds.x + (screenBounds.width - totalWidth) / 2.0f;
    float startY = screenBounds.y + (screenBounds.height - totalHeight) / 2.0f;
    
    for (int row = 0; row < gridHeight; row++) {
        for (int col = 0; col < gridWidth; col++) {
            float x = startX + col * (cardSize.x + padding);
            float y = startY + row * (cardSize.y + padding);
            positions.push_back({x, y});
        }
    }
    
    return positions;
}

Vector2 Utils::calculateOptimalCardSize(int gridWidth, int gridHeight,
                                       Rectangle screenBounds, float padding) {
    float availableWidth = screenBounds.width - (gridWidth + 1) * padding;
    float availableHeight = screenBounds.height - (gridHeight + 1) * padding;
    
    float cardWidth = availableWidth / gridWidth;
    float cardHeight = availableHeight / gridHeight;
    
    // Use the smaller dimension to ensure cards fit
    float cardSize = (cardWidth < cardHeight) ? cardWidth : cardHeight;
    
    return {cardSize, cardSize};
}
