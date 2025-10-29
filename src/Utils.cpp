#include "../include/Utils.h"

// Initialize static members
std::mt19937 Utils::s_rng;
bool Utils::s_rngInitialized = false;
std::chrono::high_resolution_clock::time_point Utils::s_startTime;
bool Utils::s_startTimeInitialized = false;

// === Logging ===
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

// === Math ===
int Utils::randomInt(int min, int max) {
    if (!s_rngInitialized) {
        s_rng.seed(std::random_device{}());
        s_rngInitialized = true;
    }
    std::uniform_int_distribution<int> dist(min, max);
    return dist(s_rng);
}

float Utils::randomFloat(float min, float max) {
    if (!s_rngInitialized) {
        s_rng.seed(std::random_device{}());
        s_rngInitialized = true;
    }
    std::uniform_real_distribution<float> dist(min, max);
    return dist(s_rng);
}

float Utils::lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float Utils::clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float Utils::distance(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

float Utils::distanceSquared(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return dx * dx + dy * dy;
}

// === Vector2 helpers ===
Vector2 Utils::vector2(float value) {
    return Vector2{value, value};
}

Vector2 Utils::vector2Add(Vector2 a, Vector2 b) {
    return Vector2{a.x + b.x, a.y + b.y};
}

Vector2 Utils::vector2Subtract(Vector2 a, Vector2 b) {
    return Vector2{a.x - b.x, a.y - b.y};
}

Vector2 Utils::vector2Scale(Vector2 vector, float scalar) {
    return Vector2{vector.x * scalar, vector.y * scalar};
}

// === String ===
std::string Utils::toString(int value) {
    return std::to_string(value);
}

std::string Utils::toString(float value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

std::string Utils::formatTime(float seconds) {
    int mins = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    std::ostringstream oss;
    oss << (mins < 10 ? "0" : "") << mins << ":"
        << (secs < 10 ? "0" : "") << secs;
    return oss.str();
}

std::string Utils::toUpper(const std::string& str) {
    std::string result = str;
    for (auto &c : result) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return result;
}

std::string Utils::toLower(const std::string& str) {
    std::string result = str;
    for (auto &c : result) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

// === File utilities ===
bool Utils::fileExists(const std::string& filename) {
    std::ifstream f(filename);
    return f.good();
}

std::string Utils::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    return content;
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
    if (pos != std::string::npos) {
        return filepath.substr(0, pos);
    }
    return "";
}

std::string Utils::getFilename(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos != std::string::npos) {
        return filepath.substr(pos + 1);
    }
    return filepath;
}

// === Array/Vector utilities ===
std::vector<int> Utils::range(int start, int end) {
    std::vector<int> result;
    for (int i = start; i < end; ++i) {
        result.push_back(i);
    }
    return result;
}

std::vector<int> Utils::createCardPairs(int numPairs) {
    std::vector<int> cards;
    for (int i = 0; i < numPairs; ++i) {
        cards.push_back(i);
        cards.push_back(i);
    }
    shuffle(cards);
    return cards;
}

// === Time utilities ===
long long Utils::getCurrentTimestamp() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

float Utils::getElapsedTime() {
    if (!s_startTimeInitialized) {
        s_startTime = std::chrono::high_resolution_clock::now();
        s_startTimeInitialized = true;
    }
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - s_startTime;
    return elapsed.count();
}

// === Color utilities ===
Color Utils::colorLerp(Color color1, Color color2, float t) {
    Color result;
    result.r = static_cast<unsigned char>(lerp(static_cast<float>(color1.r), static_cast<float>(color2.r), t));
    result.g = static_cast<unsigned char>(lerp(static_cast<float>(color1.g), static_cast<float>(color2.g), t));
    result.b = static_cast<unsigned char>(lerp(static_cast<float>(color1.b), static_cast<float>(color2.b), t));
    result.a = static_cast<unsigned char>(lerp(static_cast<float>(color1.a), static_cast<float>(color2.a), t));
    return result;
}

Color Utils::colorFromHSV(float hue, float saturation, float value) {
    float h = hue / 60.0f;
    int i = static_cast<int>(h);
    float f = h - static_cast<float>(i);
    float p = value * (1.0f - saturation);
    float q = value * (1.0f - saturation * f);
    float t = value * (1.0f - saturation * (1.0f - f));

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    
    switch (i % 6) {
        case 0: r = value; g = t; b = p; break;
        case 1: r = q; g = value; b = p; break;
        case 2: r = p; g = value; b = t; break;
        case 3: r = p; g = q; b = value; break;
        case 4: r = t; g = p; b = value; break;
        case 5: r = value; g = p; b = q; break;
    }

    Color result;
    result.r = static_cast<unsigned char>(r * 255.0f);
    result.g = static_cast<unsigned char>(g * 255.0f);
    result.b = static_cast<unsigned char>(b * 255.0f);
    result.a = 255;
    return result;
}

Color Utils::adjustBrightness(Color color, float factor) {
    Color result;
    result.r = static_cast<unsigned char>(clamp(static_cast<float>(color.r) * factor, 0.0f, 255.0f));
    result.g = static_cast<unsigned char>(clamp(static_cast<float>(color.g) * factor, 0.0f, 255.0f));
    result.b = static_cast<unsigned char>(clamp(static_cast<float>(color.b) * factor, 0.0f, 255.0f));
    result.a = color.a;
    return result;
}

// === Game-specific utilities ===
Vector2 Utils::calculateGridDimensions(int numCards) {
    int cols = static_cast<int>(std::ceil(std::sqrt(static_cast<float>(numCards))));
    int rows = static_cast<int>(std::ceil(static_cast<float>(numCards) / static_cast<float>(cols)));
    return Vector2{static_cast<float>(cols), static_cast<float>(rows)};
}

std::vector<Vector2> Utils::calculateCardPositions(int gridWidth, int gridHeight,
                                                     Vector2 cardSize, float padding,
                                                     Rectangle screenBounds) {
    std::vector<Vector2> positions;
    float totalWidth = static_cast<float>(gridWidth) * cardSize.x + static_cast<float>(gridWidth - 1) * padding;
    float totalHeight = static_cast<float>(gridHeight) * cardSize.y + static_cast<float>(gridHeight - 1) * padding;
    
    float startX = screenBounds.x + (screenBounds.width - totalWidth) / 2.0f;
    float startY = screenBounds.y + (screenBounds.height - totalHeight) / 2.0f;
    
    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            Vector2 pos;
            pos.x = startX + static_cast<float>(col) * (cardSize.x + padding);
            pos.y = startY + static_cast<float>(row) * (cardSize.y + padding);
            positions.push_back(pos);
        }
    }
    
    return positions;
}

Vector2 Utils::calculateOptimalCardSize(int gridWidth, int gridHeight,
                                         Rectangle screenBounds, float padding) {
    float availableWidth = screenBounds.width - padding * static_cast<float>(gridWidth + 1);
    float availableHeight = screenBounds.height - padding * static_cast<float>(gridHeight + 1);
    
    float cardWidth = availableWidth / static_cast<float>(gridWidth);
    float cardHeight = availableHeight / static_cast<float>(gridHeight);
    
    float aspectRatio = 2.0f / 3.0f;
    if (cardWidth / cardHeight > aspectRatio) {
        cardWidth = cardHeight * aspectRatio;
    } else {
        cardHeight = cardWidth / aspectRatio;
    }
    
    return Vector2{cardWidth, cardHeight};
}