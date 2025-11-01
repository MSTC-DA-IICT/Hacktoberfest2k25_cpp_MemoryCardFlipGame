#include "../include/Card.h"
#include "../include/Utils.h"

Texture2D Card::s_defaultBackTexture{};
bool Card::s_defaultTexturesLoaded = false;

Card::Card(int id, const std::string& texturePath, Vector2 position, Vector2 size)
    : m_id(id), m_position(position), m_size(size), m_state(CardState::FACE_DOWN),
      m_animationProgress(0.0f), m_animationSpeed(FLIP_ANIMATION_SPEED),
    m_scaleX(1.0f), m_tint(WHITE), m_rotation(0.0f), m_isHovered(false),
      m_texturePath(texturePath)
{
    if (!s_defaultTexturesLoaded) {
        loadDefaultTextures();
    }
    
    // Try to load texture from file. If loading fails, fall back to generated texture.
    if (FileExists(texturePath.c_str())) {
        Texture2D tmp = LoadTexture(texturePath.c_str());
        if (tmp.width > 0 && tmp.height > 0) {
            m_frontTexture = tmp;
        } else {
            // Loading failed - log and generate fallback
            Utils::logError(std::string("Failed to load front texture: ") + texturePath + ". Using generated color texture.");
            // ensure any invalid texture is unloaded
            if (tmp.id != 0) UnloadTexture(tmp);
            // fall through to generate
        }
    }

    // If front texture not loaded from file, generate a unique colored texture for this card ID
    if (m_frontTexture.id == 0) {
        // Generate a unique colored texture for this card ID
        Color cardColor = Utils::colorFromHSV(m_id * 30.0f, 0.8f, 0.9f);
        Image frontImg = GenImageColor(static_cast<int>(size.x), static_cast<int>(size.y), cardColor);
        
        // Draw card ID number on it
        ImageDrawRectangle(&frontImg, 10, 10, static_cast<int>(size.x) - 20, static_cast<int>(size.y) - 20, WHITE);
        ImageDrawRectangle(&frontImg, 15, 15, static_cast<int>(size.x) - 30, static_cast<int>(size.y) - 30, cardColor);
        
        m_frontTexture = LoadTextureFromImage(frontImg);
        UnloadImage(frontImg);
    }
    
    m_backTexture = s_defaultBackTexture;
}

Card::~Card() {
    UnloadTexture(m_frontTexture);
}

void Card::flipUp() {
    if (m_state == CardState::FACE_DOWN) {
        m_state = CardState::FLIPPING_UP;
    }
}

void Card::flipDown() {
    if (m_state == CardState::FACE_UP) {
        m_state = CardState::FLIPPING_DOWN;
    }
}

void Card::setMatched() {
    m_state = CardState::MATCHED;
}

bool Card::isRevealed() const {
    return m_state == CardState::FACE_UP || m_state == CardState::MATCHED;
}

bool Card::isAnimating() const {
    return m_state == CardState::FLIPPING_UP || m_state == CardState::FLIPPING_DOWN;
}

bool Card::isMatched() const {
    return m_state == CardState::MATCHED;
}

Rectangle Card::getBounds() const {
    return {m_position.x, m_position.y, m_size.x, m_size.y};
}

void Card::setPosition(Vector2 pos) {
    m_position = pos;
}

void Card::setSize(Vector2 size) {
    m_size = size;
}

bool Card::containsPoint(Vector2 point) const {
    Rectangle r = getBounds();
    return point.x >= r.x && point.x <= r.x + r.width && 
           point.y >= r.y && point.y <= r.y + r.height;
}

void Card::update(float deltaTime) {
    // Update movement first (position lerp)
    if (m_isMoving) {
        m_moveTimer += deltaTime;
        float t = m_moveDuration > 0.0f ? (m_moveTimer / m_moveDuration) : 1.0f;
        if (t >= 1.0f) {
            m_position = m_moveTarget;
            m_isMoving = false;
            m_moveTimer = 0.0f;
            m_moveDuration = 0.0f;
        } else {
            // ease in-out quad
            float tt = t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
            m_position.x = m_moveStart.x + (m_moveTarget.x - m_moveStart.x) * tt;
            m_position.y = m_moveStart.y + (m_moveTarget.y - m_moveStart.y) * tt;
        }
    }

    // Flip animation: two-phase (shrink -> swap -> expand)
    if (m_state == CardState::FLIPPING_UP || m_state == CardState::FLIPPING_DOWN) {
        m_animationProgress += deltaTime * m_animationSpeed;
        // clamp progress
        if (m_animationProgress > 1.0f) m_animationProgress = 1.0f;

        // Compute scaleX as shrinking to 0 at progress=0.5 then expanding back to 1
        float p = m_animationProgress; // 0..1
        if (p < 0.5f) {
            // first half: shrink from 1 -> 0
            m_scaleX = 1.0f - (p / 0.5f);
        } else {
            // second half: expand from 0 -> 1
            m_scaleX = (p - 0.5f) / 0.5f;
        }

        // Finish animation when progress reaches 1.0
        if (m_animationProgress >= 1.0f) {
            if (m_state == CardState::FLIPPING_UP) {
                m_state = CardState::FACE_UP;
            } else if (m_state == CardState::FLIPPING_DOWN) {
                m_state = CardState::FACE_DOWN;
            }
            m_animationProgress = 0.0f;
            m_scaleX = 1.0f;
        }
    }
}

void Card::moveTo(Vector2 target, float duration) {
    m_moveStart = m_position;
    m_moveTarget = target;
    m_moveDuration = duration;
    m_moveTimer = 0.0f;
    m_isMoving = true;
}

bool Card::isMoving() const {
    return m_isMoving;
}

void Card::draw() const {
    Rectangle rect = getBounds();
    // If the card is animating a flip, we draw a scaled version (scaleX) and swap
    // the texture at the midpoint to create a smooth flip illusion.
    if (m_state == CardState::FLIPPING_UP || m_state == CardState::FLIPPING_DOWN) {
        float p = m_animationProgress; // 0..1
        bool showFront = false;
        if (m_state == CardState::FLIPPING_UP) {
            showFront = (p >= 0.5f);
        } else { // FLIPPING_DOWN
            showFront = (p < 0.5f);
        }

        // Compute destination rect centered with scaled width
        float drawWidth = rect.width * std::max(0.001f, m_scaleX);
        float drawX = rect.x + (rect.width - drawWidth) * 0.5f;
        Rectangle sourceRect = {0, 0, (float)(showFront ? m_frontTexture.width : m_backTexture.width), (float)(showFront ? m_frontTexture.height : m_backTexture.height)};
        Rectangle destRect = { drawX, rect.y, drawWidth, rect.height };
        if (showFront) {
            DrawTexturePro(m_frontTexture, sourceRect, destRect, {0,0}, 0.0f, WHITE);
            // draw id text when front is visible and scale is reasonably large
            if (m_scaleX > 0.35f) {
                std::string idText = std::to_string(m_id);
                int fontSize = static_cast<int>(rect.height * 0.4f);
                int textWidth = MeasureText(idText.c_str(), fontSize);
                DrawText(idText.c_str(), 
                         static_cast<int>(rect.x + rect.width / 2 - textWidth / 2), 
                         static_cast<int>(rect.y + rect.height / 2 - fontSize / 2), 
                         fontSize, BLACK);
            }
        } else {
            DrawTexturePro(m_backTexture, sourceRect, destRect, {0,0}, 0.0f, WHITE);
        }
    } else {
        // Not animating: draw normally depending on revealed state
        if (isRevealed()) {
            Rectangle sourceRect = {0, 0, (float)m_frontTexture.width, (float)m_frontTexture.height};
            Rectangle destRect = {rect.x, rect.y, rect.width, rect.height};
            DrawTexturePro(m_frontTexture, sourceRect, destRect, {0, 0}, 0.0f, WHITE);

            // Draw card ID number on front
            std::string idText = std::to_string(m_id);
            int fontSize = static_cast<int>(rect.height * 0.4f); // Scale font with card size
            int textWidth = MeasureText(idText.c_str(), fontSize);
            DrawText(idText.c_str(), 
                     static_cast<int>(rect.x + rect.width / 2 - textWidth / 2), 
                     static_cast<int>(rect.y + rect.height / 2 - fontSize / 2), 
                     fontSize, BLACK);
        } else {
            Rectangle sourceRect = {0, 0, (float)m_backTexture.width, (float)m_backTexture.height};
            Rectangle destRect = {rect.x, rect.y, rect.width, rect.height};
            DrawTexturePro(m_backTexture, sourceRect, destRect, {0, 0}, 0.0f, WHITE);
        }
    }

    // Draw border
    DrawRectangleLinesEx(rect, 2.0f, BORDER_COLOR);
    
    // Draw glow effect for matched cards
    if (isMatched()) {
        DrawRectangleLinesEx(rect, 4.0f, GREEN);
    }
}

// Static method implementations
void Card::loadDefaultTextures() {
    if (!s_defaultTexturesLoaded) {
        // Prefer using a provided image for the card back if available.
        const char* preferredPaths[] = {
            "assets/textures/card_back.png",
            "assets/textures/card1.png",
            "assets/textures/back.png",
            nullptr
        };

        bool loaded = false;
        for (int i = 0; preferredPaths[i] != nullptr; ++i) {
            const char* p = preferredPaths[i];
            if (FileExists(p)) {
                Texture2D tmp = LoadTexture(p);
                if (tmp.width > 0 && tmp.height > 0) {
                    s_defaultBackTexture = tmp;
                    loaded = true;
                    Utils::logInfo(std::string("Loaded card back texture: ") + p);
                    break;
                } else {
                    Utils::logError(std::string("Failed to load back texture: ") + p + " - trying next option");
                    if (tmp.id != 0) UnloadTexture(tmp);
                }
            }
        }

        if (!loaded) {
            // Fallback: create a small default back texture (will be scaled)
            int texSize = 100;
            Image backImg = GenImageColor(texSize, texSize, BLUE);
            // Draw a pattern on the back (optional decoration)
            ImageDrawRectangle(&backImg, texSize/10, texSize/10, texSize*8/10, texSize*8/10, DARKBLUE);
            ImageDrawRectangle(&backImg, texSize/5, texSize/5, texSize*3/5, texSize*3/5, BLUE);
            s_defaultBackTexture = LoadTextureFromImage(backImg);
            UnloadImage(backImg);
            Utils::logInfo("Generated default card back texture (fallback)");
        }

        s_defaultTexturesLoaded = true;
    }
}

void Card::unloadDefaultTextures() {
    if (s_defaultTexturesLoaded) {
        UnloadTexture(s_defaultBackTexture);
        s_defaultTexturesLoaded = false;
        Utils::logInfo("Default card textures unloaded");
    }
}

// Copy constructor
Card::Card(const Card& other)
    : m_id(other.m_id),
      m_position(other.m_position),
      m_size(other.m_size),
      m_state(other.m_state),
      m_texturePath(other.m_texturePath),
      m_animationProgress(other.m_animationProgress),
      m_animationSpeed(other.m_animationSpeed),
      m_scaleX(other.m_scaleX),
      m_tint(other.m_tint),
      m_rotation(other.m_rotation),
      m_isHovered(other.m_isHovered)
{
    // Load textures for the new card
    if (!other.m_texturePath.empty()) {
        Texture2D tmp = LoadTexture(other.m_texturePath.c_str());
        if (tmp.width > 0 && tmp.height > 0) {
            m_frontTexture = tmp;
        } else {
            Utils::logError(std::string("Copy ctor: failed to load front texture: ") + other.m_texturePath + ". Generating fallback.");
            if (tmp.id != 0) UnloadTexture(tmp);
        }
    }
    m_backTexture = s_defaultBackTexture;
}

// Copy assignment operator
Card& Card::operator=(const Card& other) {
    if (this != &other) {
        // Unload old texture
        UnloadTexture(m_frontTexture);
        
        // Copy data
        m_id = other.m_id;
        m_position = other.m_position;
        m_size = other.m_size;
        m_state = other.m_state;
        m_texturePath = other.m_texturePath;
        m_animationProgress = other.m_animationProgress;
        m_animationSpeed = other.m_animationSpeed;
        m_scaleX = other.m_scaleX;
        m_tint = other.m_tint;
        m_rotation = other.m_rotation;
        m_isHovered = other.m_isHovered;
        
        // Load new texture
        if (!other.m_texturePath.empty()) {
            Texture2D tmp = LoadTexture(other.m_texturePath.c_str());
            if (tmp.width > 0 && tmp.height > 0) {
                m_frontTexture = tmp;
            } else {
                Utils::logError(std::string("Copy assign: failed to load front texture: ") + other.m_texturePath + ". Generating fallback.");
                if (tmp.id != 0) UnloadTexture(tmp);
            }
        }
        m_backTexture = s_defaultBackTexture;
    }
    return *this;
}

// Move constructor
Card::Card(Card&& other) noexcept
    : m_id(other.m_id),
      m_position(other.m_position),
      m_size(other.m_size),
      m_state(other.m_state),
      m_frontTexture(other.m_frontTexture),
      m_backTexture(other.m_backTexture),
      m_texturePath(std::move(other.m_texturePath)),
      m_animationProgress(other.m_animationProgress),
      m_animationSpeed(other.m_animationSpeed),
      m_scaleX(other.m_scaleX),
      m_tint(other.m_tint),
      m_rotation(other.m_rotation),
      m_isHovered(other.m_isHovered)
{
    // Invalidate the moved-from object's texture
    other.m_frontTexture = Texture2D{};
    other.m_backTexture = Texture2D{};
}

// Move assignment operator
Card& Card::operator=(Card&& other) noexcept {
    if (this != &other) {
        // Unload old texture
        UnloadTexture(m_frontTexture);
        
        // Move data
        m_id = other.m_id;
        m_position = other.m_position;
        m_size = other.m_size;
        m_state = other.m_state;
        m_frontTexture = other.m_frontTexture;
        m_backTexture = other.m_backTexture;
        m_texturePath = std::move(other.m_texturePath);
        m_animationProgress = other.m_animationProgress;
        m_animationSpeed = other.m_animationSpeed;
        m_scaleX = other.m_scaleX;
        m_tint = other.m_tint;
        m_rotation = other.m_rotation;
        m_isHovered = other.m_isHovered;
        
        // Invalidate the moved-from object's texture
        other.m_frontTexture = Texture2D{};
        other.m_backTexture = Texture2D{};
    }
    return *this;
}

// #include "../include/Card.h"
// #include "../include/Utils.h"

// Texture2D Card::s_defaultBackTexture{};
// bool Card::s_defaultTexturesLoaded = false;

// Card::Card(int id, const std::string& texturePath, Vector2 position, Vector2 size)
//     : m_id(id), m_position(position), m_size(size), m_state(CardState::FACE_DOWN),
//       m_animationProgress(0.0f), m_animationSpeed(FLIP_ANIMATION_SPEED),
//       m_scaleX(1.0f), m_tint(WHITE), m_rotation(0.0f), m_isHovered(false),
//       m_texturePath(texturePath)
// {
//     if (!s_defaultTexturesLoaded) {
//         loadDefaultTextures();
//     }
    
//     // Try to load texture from file
//     if (FileExists(texturePath.c_str())) {
//         m_frontTexture = LoadTexture(texturePath.c_str());
//     } else {
//         // Generate a unique colored texture for this card ID
//         Color cardColor = Utils::colorFromHSV(m_id * 30.0f, 0.8f, 0.9f);
//         Image frontImg = GenImageColor(static_cast<int>(size.x), static_cast<int>(size.y), cardColor);
        
//         // Draw card ID number on it
//         ImageDrawRectangle(&frontImg, 10, 10, static_cast<int>(size.x) - 20, static_cast<int>(size.y) - 20, WHITE);
//         ImageDrawRectangle(&frontImg, 15, 15, static_cast<int>(size.x) - 30, static_cast<int>(size.y) - 30, cardColor);
        
//         m_frontTexture = LoadTextureFromImage(frontImg);
//         UnloadImage(frontImg);
//     }
    
//     m_backTexture = s_defaultBackTexture;
// }

// Card::~Card() {
//     UnloadTexture(m_frontTexture);
// }

// void Card::flipUp() {
//     if (m_state == CardState::FACE_DOWN) {
//         m_state = CardState::FLIPPING_UP;
//     }
// }

// void Card::flipDown() {
//     if (m_state == CardState::FACE_UP) {
//         m_state = CardState::FLIPPING_DOWN;
//     }
// }

// void Card::setMatched() {
//     m_state = CardState::MATCHED;
// }

// bool Card::isRevealed() const {
//     return m_state == CardState::FACE_UP || m_state == CardState::MATCHED;
// }

// bool Card::isAnimating() const {
//     return m_state == CardState::FLIPPING_UP || m_state == CardState::FLIPPING_DOWN;
// }

// bool Card::isMatched() const {
//     return m_state == CardState::MATCHED;
// }

// Rectangle Card::getBounds() const {
//     return {m_position.x, m_position.y, m_size.x, m_size.y};
// }

// void Card::setPosition(Vector2 pos) {
//     m_position = pos;
// }

// void Card::setSize(Vector2 size) {
//     m_size = size;
// }

// bool Card::containsPoint(Vector2 point) const {
//     Rectangle r = getBounds();
//     return point.x >= r.x && point.x <= r.x + r.width && 
//            point.y >= r.y && point.y <= r.y + r.height;
// }

// void Card::update(float deltaTime) {
//     if (m_state == CardState::FLIPPING_UP) {
//         m_animationProgress += deltaTime * m_animationSpeed;
//         m_scaleX = 1.0f - m_animationProgress;
//         if (m_animationProgress >= 1.0f) {
//             m_state = CardState::FACE_UP;
//             m_animationProgress = 0.0f;
//             m_scaleX = 1.0f;
//         }
//     } else if (m_state == CardState::FLIPPING_DOWN) {
//         m_animationProgress += deltaTime * m_animationSpeed;
//         m_scaleX = 1.0f - m_animationProgress;
//         if (m_animationProgress >= 1.0f) {
//             m_state = CardState::FACE_DOWN;
//             m_animationProgress = 0.0f;
//             m_scaleX = 1.0f;
//         }
//     }
// }

// void Card::draw() const {
//     Rectangle rect = getBounds();
//     Vector2 origin = {rect.width / 2.0f, rect.height / 2.0f};
//     Rectangle drawRect = {rect.x + origin.x, rect.y + origin.y, rect.width, rect.height};

//     if (isRevealed()) {
//         DrawTextureEx(m_frontTexture, m_position, 0.0f, 1.0f, WHITE);
        
//         // Draw card ID number on front
//         std::string idText = std::to_string(m_id);
//         int fontSize = 40;
//         int textWidth = MeasureText(idText.c_str(), fontSize);
//         DrawText(idText.c_str(), 
//                  static_cast<int>(rect.x + rect.width / 2 - textWidth / 2), 
//                  static_cast<int>(rect.y + rect.height / 2 - fontSize / 2), 
//                  fontSize, BLACK);
//     } else {
//         DrawTextureEx(m_backTexture, m_position, 0.0f, 1.0f, WHITE);
//     }

//     // Draw border
//     DrawRectangleLinesEx(rect, 2.0f, BORDER_COLOR);
    
//     // Draw glow effect for matched cards
//     if (isMatched()) {
//         DrawRectangleLinesEx(rect, 4.0f, GREEN);
//     }
// }

// // Static method implementations
// void Card::loadDefaultTextures() {
//     if (!s_defaultTexturesLoaded) {
//         // Create a default back texture (blue card back)
//         Image backImg = GenImageColor(200, 300, BLUE);
        
//         // Draw a pattern on the back (optional decoration)
//         ImageDrawRectangle(&backImg, 10, 10, 180, 280, DARKBLUE);
//         ImageDrawRectangle(&backImg, 20, 20, 160, 260, BLUE);
        
//         s_defaultBackTexture = LoadTextureFromImage(backImg);
//         UnloadImage(backImg);
        
//         s_defaultTexturesLoaded = true;
//         Utils::logInfo("Default card textures loaded successfully");
//     }
// }

// void Card::unloadDefaultTextures() {
//     if (s_defaultTexturesLoaded) {
//         UnloadTexture(s_defaultBackTexture);
//         s_defaultTexturesLoaded = false;
//         Utils::logInfo("Default card textures unloaded");
//     }
// }

// // Copy constructor
// Card::Card(const Card& other)
//     : m_id(other.m_id),
//       m_position(other.m_position),
//       m_size(other.m_size),
//       m_state(other.m_state),
//       m_texturePath(other.m_texturePath),
//       m_animationProgress(other.m_animationProgress),
//       m_animationSpeed(other.m_animationSpeed),
//       m_scaleX(other.m_scaleX),
//       m_tint(other.m_tint),
//       m_rotation(other.m_rotation),
//       m_isHovered(other.m_isHovered)
// {
//     // Load textures for the new card
//     if (!other.m_texturePath.empty()) {
//         m_frontTexture = LoadTexture(other.m_texturePath.c_str());
//     }
//     m_backTexture = s_defaultBackTexture;
// }

// // Copy assignment operator
// Card& Card::operator=(const Card& other) {
//     if (this != &other) {
//         // Unload old texture
//         UnloadTexture(m_frontTexture);
        
//         // Copy data
//         m_id = other.m_id;
//         m_position = other.m_position;
//         m_size = other.m_size;
//         m_state = other.m_state;
//         m_texturePath = other.m_texturePath;
//         m_animationProgress = other.m_animationProgress;
//         m_animationSpeed = other.m_animationSpeed;
//         m_scaleX = other.m_scaleX;
//         m_tint = other.m_tint;
//         m_rotation = other.m_rotation;
//         m_isHovered = other.m_isHovered;
        
//         // Load new texture
//         if (!other.m_texturePath.empty()) {
//             m_frontTexture = LoadTexture(other.m_texturePath.c_str());
//         }
//         m_backTexture = s_defaultBackTexture;
//     }
//     return *this;
// }

// // Move constructor
// Card::Card(Card&& other) noexcept
//     : m_id(other.m_id),
//       m_position(other.m_position),
//       m_size(other.m_size),
//       m_state(other.m_state),
//       m_frontTexture(other.m_frontTexture),
//       m_backTexture(other.m_backTexture),
//       m_texturePath(std::move(other.m_texturePath)),
//       m_animationProgress(other.m_animationProgress),
//       m_animationSpeed(other.m_animationSpeed),
//       m_scaleX(other.m_scaleX),
//       m_tint(other.m_tint),
//       m_rotation(other.m_rotation),
//       m_isHovered(other.m_isHovered)
// {
//     // Invalidate the moved-from object's texture
//     other.m_frontTexture = Texture2D{};
//     other.m_backTexture = Texture2D{};
// }

// // Move assignment operator
// Card& Card::operator=(Card&& other) noexcept {
//     if (this != &other) {
//         // Unload old texture
//         UnloadTexture(m_frontTexture);
        
//         // Move data
//         m_id = other.m_id;
//         m_position = other.m_position;
//         m_size = other.m_size;
//         m_state = other.m_state;
//         m_frontTexture = other.m_frontTexture;
//         m_backTexture = other.m_backTexture;
//         m_texturePath = std::move(other.m_texturePath);
//         m_animationProgress = other.m_animationProgress;
//         m_animationSpeed = other.m_animationSpeed;
//         m_scaleX = other.m_scaleX;
//         m_tint = other.m_tint;
//         m_rotation = other.m_rotation;
//         m_isHovered = other.m_isHovered;
        
//         // Invalidate the moved-from object's texture
//         other.m_frontTexture = Texture2D{};
//         other.m_backTexture = Texture2D{};
//     }
//     return *this;
// }