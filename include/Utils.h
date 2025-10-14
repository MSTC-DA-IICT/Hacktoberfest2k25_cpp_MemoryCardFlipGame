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

/**
 * @brief Utility class containing static helper functions
 */
class Utils {
public:
    // Delete constructor - this is a static utility class
    Utils() = delete;
    
    // === LOGGING FUNCTIONS ===
    
    /**
     * @brief Logs an info message to console
     * @param message Message to log
     */
    static void logInfo(const std::string& message);
    
    /**
     * @brief Logs a warning message to console
     * @param message Warning message to log
     */
    static void logWarning(const std::string& message);
    
    /**
     * @brief Logs an error message to console
     * @param message Error message to log
     */
    static void logError(const std::string& message);
    
    /**
     * @brief Logs a debug message to console (only in debug builds)
     * @param message Debug message to log
     */
    static void logDebug(const std::string& message);
    
    // === MATH UTILITIES ===
    
    /**
     * @brief Generates a random integer between min and max (inclusive)
     * @param min Minimum value
     * @param max Maximum value
     * @return Random integer
     */
    static int randomInt(int min, int max);
    
    /**
     * @brief Generates a random float between min and max
     * @param min Minimum value
     * @param max Maximum value
     * @return Random float
     */
    static float randomFloat(float min, float max);
    
    /**
     * @brief Linear interpolation between two values
     * @param a Start value
     * @param b End value
     * @param t Interpolation factor (0.0 to 1.0)
     * @return Interpolated value
     */
    static float lerp(float a, float b, float t);
    
    /**
     * @brief Clamps a value between min and max
     * @param value Value to clamp
     * @param min Minimum value
     * @param max Maximum value
     * @return Clamped value
     */
    static float clamp(float value, float min, float max);
    
    /**
     * @brief Calculates distance between two points
     * @param a First point
     * @param b Second point
     * @return Distance between points
     */
    static float distance(Vector2 a, Vector2 b);
    
    /**
     * @brief Calculates squared distance between two points (faster than distance)
     * @param a First point
     * @param b Second point
     * @return Squared distance between points
     */
    static float distanceSquared(Vector2 a, Vector2 b);
    
    // === VECTOR UTILITIES ===
    
    /**
     * @brief Creates a Vector2 with the same value for both x and y
     * @param value Value for both components
     * @return Vector2 with equal components
     */
    static Vector2 vector2(float value);
    
    /**
     * @brief Adds two Vector2s
     * @param a First vector
     * @param b Second vector
     * @return Sum of vectors
     */
    static Vector2 vector2Add(Vector2 a, Vector2 b);
    
    /**
     * @brief Subtracts Vector2 b from Vector2 a
     * @param a First vector
     * @param b Second vector
     * @return Difference of vectors
     */
    static Vector2 vector2Subtract(Vector2 a, Vector2 b);
    
    /**
     * @brief Multiplies Vector2 by a scalar
     * @param vector Vector to multiply
     * @param scalar Scalar multiplier
     * @return Scaled vector
     */
    static Vector2 vector2Scale(Vector2 vector, float scalar);
    
    // === STRING UTILITIES ===
    
    /**
     * @brief Converts integer to string
     * @param value Integer value
     * @return String representation
     */
    static std::string toString(int value);
    
    /**
     * @brief Converts float to string with specified precision
     * @param value Float value
     * @param precision Number of decimal places
     * @return String representation
     */
    static std::string toString(float value, int precision = 2);
    
    /**
     * @brief Formats time in seconds to MM:SS format
     * @param seconds Time in seconds
     * @return Formatted time string
     */
    static std::string formatTime(float seconds);
    
    /**
     * @brief Converts string to uppercase
     * @param str String to convert
     * @return Uppercase string
     */
    static std::string toUpper(const std::string& str);
    
    /**
     * @brief Converts string to lowercase
     * @param str String to convert  
     * @return Lowercase string
     */
    static std::string toLower(const std::string& str);
    
    // === FILE UTILITIES ===
    
    /**
     * @brief Checks if a file exists
     * @param filename Path to file
     * @return True if file exists, false otherwise
     */
    static bool fileExists(const std::string& filename);
    
    /**
     * @brief Reads entire file contents to string
     * @param filename Path to file
     * @return File contents as string, empty if file doesn't exist
     */
    static std::string readFile(const std::string& filename);
    
    /**
     * @brief Writes string to file
     * @param filename Path to file
     * @param contents Contents to write
     * @return True if successful, false otherwise
     */
    static bool writeFile(const std::string& filename, const std::string& contents);
    
    /**
     * @brief Gets the directory path from a full file path
     * @param filepath Full file path
     * @return Directory path
     */
    static std::string getDirectory(const std::string& filepath);
    
    /**
     * @brief Gets the filename (without path) from a full file path
     * @param filepath Full file path
     * @return Filename only
     */
    static std::string getFilename(const std::string& filepath);
    
    // === ARRAY/VECTOR UTILITIES ===
    
    /**
     * @brief Shuffles a vector using Fisher-Yates algorithm
     * @tparam T Type of elements in vector
     * @param vec Vector to shuffle (modified in-place)
     */
    template<typename T>
    static void shuffle(std::vector<T>& vec);
    
    /**
     * @brief Creates a vector of integers from start to end-1
     * @param start Starting value
     * @param end Ending value (exclusive)
     * @return Vector of integers
     */
    static std::vector<int> range(int start, int end);
    
    /**
     * @brief Creates pairs of numbers for memory game
     * @param numPairs Number of pairs to create
     * @return Vector with each number appearing twice
     */
    static std::vector<int> createCardPairs(int numPairs);
    
    // === TIME UTILITIES ===
    
    /**
     * @brief Gets current timestamp in milliseconds
     * @return Current timestamp
     */
    static long long getCurrentTimestamp();
    
    /**
     * @brief Gets elapsed time since program start
     * @return Elapsed time in seconds
     */
    static float getElapsedTime();
    
    // === COLOR UTILITIES ===
    
    /**
     * @brief Interpolates between two colors
     * @param color1 First color
     * @param color2 Second color
     * @param t Interpolation factor (0.0 to 1.0)
     * @return Interpolated color
     */
    static Color colorLerp(Color color1, Color color2, float t);
    
    /**
     * @brief Creates a color from HSV values
     * @param hue Hue (0-360)
     * @param saturation Saturation (0.0-1.0)
     * @param value Value/Brightness (0.0-1.0)
     * @return RGB Color
     */
    static Color colorFromHSV(float hue, float saturation, float value);
    
    /**
     * @brief Adjusts color brightness
     * @param color Original color
     * @param factor Brightness factor (1.0 = no change, >1.0 = brighter, <1.0 = darker)
     * @return Adjusted color
     */
    static Color adjustBrightness(Color color, float factor);
    
    // === GAME-SPECIFIC UTILITIES ===
    
    /**
     * @brief Calculates grid dimensions for given number of cards
     * @param numCards Total number of cards
     * @return Vector2 with width (x) and height (y) of grid
     */
    static Vector2 calculateGridDimensions(int numCards);
    
    /**
     * @brief Calculates card positions for a grid layout
     * @param gridWidth Width of grid (number of cards)
     * @param gridHeight Height of grid (number of cards)
     * @param cardSize Size of each card
     * @param padding Padding between cards
     * @param screenBounds Available screen area
     * @return Vector of card positions
     */
    static std::vector<Vector2> calculateCardPositions(int gridWidth, int gridHeight,
                                                      Vector2 cardSize, float padding,
                                                      Rectangle screenBounds);
    
    /**
     * @brief Calculates optimal card size for given grid and screen size
     * @param gridWidth Width of grid
     * @param gridHeight Height of grid  
     * @param screenBounds Available screen area
     * @param padding Padding between cards
     * @return Optimal card size
     */
    static Vector2 calculateOptimalCardSize(int gridWidth, int gridHeight,
                                           Rectangle screenBounds, float padding);

private:
    // Random number generator
    static std::mt19937 s_rng;
    static bool s_rngInitialized;
    
    // Program start time for elapsed time calculation
    static std::chrono::high_resolution_clock::time_point s_startTime;
    static bool s_startTimeInitialized;
    
    // Helper methods
    static void initializeRNG();
    static void initializeStartTime();
    
    // Constants
    static constexpr float PI = 3.14159265359f;
    static constexpr float DEG_TO_RAD = PI / 180.0f;
    static constexpr float RAD_TO_DEG = 180.0f / PI;
};

// Template implementation for shuffle function
template<typename T>
void Utils::shuffle(std::vector<T>& vec) {
    if (!s_rngInitialized) {
        initializeRNG();
    }
    
    std::shuffle(vec.begin(), vec.end(), s_rng);
}