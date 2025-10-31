/**
 * @file Utils.h
 * @brief Utility functions and helpers for the Memory Card Game
 * 
 * This file contains various utility functions used throughout the game
 * including logging, math helpers, file operations, and other common utilities.
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>

class Utils {
private:
    // Constants (use names that won't collide with macros)
    inline static constexpr float kPI = 3.14159265359f;
    inline static constexpr float kDEG_TO_RAD = kPI / 180.0f;
    inline static constexpr float kRAD_TO_DEG = 180.0f / kPI;

public:
    Utils() = delete; // Static class, no constructor

    // === LOGGING FUNCTIONS ===
    static void logInfo(const std::string& message);
    static void logWarning(const std::string& message);
    static void logError(const std::string& message);
    static void logDebug(const std::string& message);

    // === MATH UTILITIES ===
    static int randomInt(int min, int max);
    static float randomFloat(float min, float max);
    static float lerp(float a, float b, float t);
    static float clamp(float value, float min, float max);
    static float distance(Vector2 a, Vector2 b);
    static float distanceSquared(Vector2 a, Vector2 b);

    // === VECTOR UTILITIES ===
    static Vector2 vector2(float value);
    static Vector2 vector2Add(Vector2 a, Vector2 b);
    static Vector2 vector2Subtract(Vector2 a, Vector2 b);
    static Vector2 vector2Scale(Vector2 vector, float scalar);

    // === STRING UTILITIES ===
    static std::string toString(int value);
    static std::string toString(float value, int precision = 2);
    static std::string formatTime(float seconds);
    static std::string toUpper(const std::string& str);
    static std::string toLower(const std::string& str);

    // === FILE UTILITIES ===
    static bool fileExists(const std::string& filename);
    static std::string readFile(const std::string& filename);
    static bool writeFile(const std::string& filename, const std::string& contents);
    static std::string getDirectory(const std::string& filepath);
    static std::string getFilename(const std::string& filepath);

    // === ARRAY/VECTOR UTILITIES ===
    template<typename T>
    static void shuffle(std::vector<T>& vec) {
        if (!s_rngInitialized) {
            s_rng.seed(std::random_device{}());
            s_rngInitialized = true;
        }
        std::shuffle(vec.begin(), vec.end(), s_rng);
    }
    static std::vector<int> range(int start, int end);
    static std::vector<int> createCardPairs(int numPairs);

    // === TIME UTILITIES ===
    static long long getCurrentTimestamp();
    static float getElapsedTime();

    // === COLOR UTILITIES ===
    static Color colorLerp(Color color1, Color color2, float t);
    static Color colorFromHSV(float hue, float saturation, float value);
    static Color adjustBrightness(Color color, float factor);

    // === GAME-SPECIFIC UTILITIES ===
    static Vector2 calculateGridDimensions(int numCards);
    static std::vector<Vector2> calculateCardPositions(int gridWidth, int gridHeight,
                                                        Vector2 cardSize, float padding,
                                                        Rectangle screenBounds);
    static Vector2 calculateOptimalCardSize(int gridWidth, int gridHeight,
                                             Rectangle screenBounds, float padding);

private:
    static std::mt19937 s_rng;
    static bool s_rngInitialized;
    static std::chrono::high_resolution_clock::time_point s_startTime;
    static bool s_startTimeInitialized;
};