/**
 * @file Card.cpp
 * @brief Implementation of the Card class
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#include "Card.h"
#include "Utils.h"

// Initialize static members
Texture2D Card::s_defaultBackTexture = {0};
bool Card::s_defaultTexturesLoaded = false;

Card::Card(int id, const std::string& texturePath, Vector2 position, Vector2 size)
    : m_id(id)
    , m_position(position)
    , m_size(size)
    , m_state(CardState::FACE_DOWN)
    , m_texturePath(texturePath)
    , m_animationProgress(0.0f)
    , m_animationSpeed(FLIP_ANIMATION_SPEED)
    , m_scaleX(1.0f)
    , m_tint(WHITE)
    , m_rotation(0.0f)
    , m_isHovered(false)
{
    loadTextures();
}

Card::Card(const Card& other)
    : m_id(other.m_id)
    , m_position(other.m_position)
    , m_size(other.m_size)
    , m_state(other.m_state)
    , m_texturePath(other.m_texturePath)
    , m_animationProgress(other.m_animationProgress)
    , m_animationSpeed(other.m_animationSpeed)
    , m_scaleX(other.m_scaleX)
    , m_tint(other.m_tint)
    , m_rotation(other.m_rotation)
    , m_isHovered(other.m_isHovered)
{
    loadTextures();
}

Card& Card::operator=(const Card& other) {
    if (this != &other) {
        unloadTextures();
        
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
        
        loadTextures();
    }
    return *this;
}

Card::Card(Card&& other) noexcept
    : m_id(other.m_id)
    , m_position(other.m_position)
    , m_size(other.m_size)
    , m_state(other.m_state)
    , m_frontTexture(other.m_frontTexture)
    , m_backTexture(other.m_backTexture)
    , m_texturePath(std::move(other.m_texturePath))
    , m_animationProgress(other.m_animationProgress)
    , m_animationSpeed(other.m_animationSpeed)
    , m_scaleX(other.m_scaleX)
    , m_tint(other.m_tint)
    , m_rotation(other.m_rotation)
    , m_isHovered(other.m_isHovered)
{
    other.m_frontTexture = {0};
    other.m_backTexture = {0};
}

Card& Card::operator=(Card&& other) noexcept {
    if (this != &other) {
        unloadTextures();
        
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
        
        other.m_frontTexture = {0};
        other.m_backTexture = {0};
    }
    return *this;
}

Card::~Card() {
    unloadTextures();
}

void Card::update(float deltaTime) {
    updateAnimation(deltaTime);
    
    // Check if mouse is hovering (for future use)
    Vector2 mousePos = GetMousePosition();
    m_isHovered = containsPoint(mousePos);
}

void Card::draw() const {
    // Determine which side to draw based on animation
    if (m_state == CardState::FACE_DOWN || m_state == CardState::FLIPPING_DOWN) {
        if (m_scaleX > 0.0f) {
            drawCardBack();
        } else {
            drawCardFront();
        }
    } else {
        if (m_scaleX > 0.0f) {
            drawCardFront();
        } else {
            drawCardBack();
        }
    }
    
    drawCardBorder();
    
    if (m_isHovered && m_state == CardState::FACE_DOWN) {
        drawHoverEffect();
    }
    
    if (m_state == CardState::MATCHED) {
        drawMatchedEffect();
    }
}

void Card::flipUp() {
    if (m_state == CardState::FACE_DOWN) {
        m_state = CardState::FLIPPING_UP;
        m_animationProgress = 0.0f;
    }
}

void Card::flipDown() {
    if (m_state == CardState::FACE_UP) {
        m_state = CardState::FLIPPING_DOWN;
        m_animationProgress = 0.0f;
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

bool Card::containsPoint(Vector2 point) const {
    Rectangle bounds = getBounds();
    return CheckCollisionPointRec(point, bounds);
}

Rectangle Card::getBounds() const {
    return Rectangle{m_position.x, m_position.y, m_size.x, m_size.y};
}

void Card::setPosition(Vector2 position) {
    m_position = position;
}

void Card::setSize(Vector2 size) {
    m_size = size;
}

// === PRIVATE METHODS ===

void Card::loadTextures() {
    if (!s_defaultTexturesLoaded) {
        loadDefaultTextures();
    }
    
    // Use default back texture
    m_backTexture = s_defaultBackTexture;
    
    // Try to load front texture (use placeholder for now)
    // m_frontTexture = LoadTexture(m_texturePath.c_str());
    // For now, we'll use a generated texture
    m_frontTexture = {0};
}

void Card::unloadTextures() {
    // Don't unload front texture if it's valid (texture management can be improved)
    if (m_frontTexture.id != 0) {
        // UnloadTexture(m_frontTexture);
        m_frontTexture = {0};
    }
}

void Card::updateAnimation(float deltaTime) {
    if (isAnimating()) {
        m_animationProgress += deltaTime * m_animationSpeed;
        
        if (m_animationProgress >= 1.0f) {
            m_animationProgress = 1.0f;
            
            // Finish animation
            if (m_state == CardState::FLIPPING_UP) {
                m_state = CardState::FACE_UP;
            } else if (m_state == CardState::FLIPPING_DOWN) {
                m_state = CardState::FACE_DOWN;
            }
        }
        
        // Calculate scale for flip animation
        float halfProgress = m_animationProgress * 2.0f;
        if (halfProgress <= 1.0f) {
            m_scaleX = 1.0f - halfProgress;
        } else {
            m_scaleX = halfProgress - 1.0f;
        }
    } else {
        m_scaleX = 1.0f;
    }
}

void Card::drawCardFront() const {
    Rectangle dest = {
        m_position.x + m_size.x / 2.0f,
        m_position.y + m_size.y / 2.0f,
        m_size.x * std::abs(m_scaleX),
        m_size.y
    };
    
    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};
    
    // Draw card background
    DrawRectanglePro(dest, origin, m_rotation, WHITE);
    
    // Draw card ID as text (placeholder)
    std::string idText = Utils::toString(m_id);
    int fontSize = static_cast<int>(m_size.y * 0.4f);
    int textWidth = MeasureText(idText.c_str(), fontSize);
    DrawText(idText.c_str(), 
             dest.x - textWidth / 2, 
             dest.y - fontSize / 2, 
             fontSize, BLACK);
}

void Card::drawCardBack() const {
    Rectangle dest = {
        m_position.x + m_size.x / 2.0f,
        m_position.y + m_size.y / 2.0f,
        m_size.x * std::abs(m_scaleX),
        m_size.y
    };
    
    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};
    
    // Draw card back with a pattern
    DrawRectanglePro(dest, origin, m_rotation, BLUE);
    
    // Draw a simple pattern
    DrawCircle(dest.x, dest.y, m_size.x * 0.2f * std::abs(m_scaleX), DARKBLUE);
}

void Card::drawCardBorder() const {
    Rectangle bounds = getBounds();
    DrawRectangleLinesEx(bounds, BORDER_THICKNESS, BORDER_COLOR);
}

void Card::drawHoverEffect() const {
    Rectangle bounds = getBounds();
    DrawRectangleRec(bounds, HOVER_COLOR);
}

void Card::drawMatchedEffect() const {
    Rectangle bounds = getBounds();
    DrawRectangleRec(bounds, MATCHED_COLOR);
}

void Card::loadDefaultTextures() {
    // Create a simple default back texture (placeholder)
    s_defaultBackTexture = {0};
    s_defaultTexturesLoaded = true;
}

void Card::unloadDefaultTextures() {
    if (s_defaultTexturesLoaded && s_defaultBackTexture.id != 0) {
        UnloadTexture(s_defaultBackTexture);
        s_defaultBackTexture = {0};
        s_defaultTexturesLoaded = false;
    }
}
