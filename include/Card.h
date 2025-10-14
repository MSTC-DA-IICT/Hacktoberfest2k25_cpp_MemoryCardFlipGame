/**
 * @file Card.h
 * @brief Card class representing individual memory game cards
 * 
 * This class handles individual card behavior including rendering,
 * flipping animations, and state management.
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#pragma once

#include <raylib.h>
#include <string>
#include <memory>

/**
 * @brief Enumeration of card states
 */
enum class CardState {
    FACE_DOWN,     ///< Card is face down (hidden)
    FLIPPING_UP,   ///< Card is in the process of flipping up
    FACE_UP,       ///< Card is face up (revealed)
    FLIPPING_DOWN, ///< Card is in the process of flipping down
    MATCHED        ///< Card has been matched and stays revealed
};

/**
 * @brief Card class representing individual memory cards
 * 
 * The Card class is responsible for:
 * - Storing card data (ID, texture, position)
 * - Managing card state and animations
 * - Rendering the card with appropriate visuals
 * - Handling card flip animations
 */
class Card {
public:
    /**
     * @brief Constructor for Card class
     * @param id Unique identifier for this card (used for matching)
     * @param texturePath Path to the card's texture file
     * @param position Position of the card on screen
     * @param size Size of the card
     */
    Card(int id, const std::string& texturePath, Vector2 position, Vector2 size);
    
    /**
     * @brief Copy constructor
     * @param other Card to copy from
     */
    Card(const Card& other);
    
    /**
     * @brief Copy assignment operator
     * @param other Card to copy from
     * @return Reference to this card
     */
    Card& operator=(const Card& other);
    
    /**
     * @brief Move constructor
     * @param other Card to move from
     */
    Card(Card&& other) noexcept;
    
    /**
     * @brief Move assignment operator
     * @param other Card to move from
     * @return Reference to this card
     */
    Card& operator=(Card&& other) noexcept;
    
    /**
     * @brief Destructor - cleans up resources
     */
    ~Card();
    
    /**
     * @brief Updates the card (animations, etc.)
     * @param deltaTime Time elapsed since last frame
     */
    void update(float deltaTime);
    
    /**
     * @brief Draws the card to the screen
     */
    void draw() const;
    
    /**
     * @brief Starts the flip animation to reveal the card
     */
    void flipUp();
    
    /**
     * @brief Starts the flip animation to hide the card
     */
    void flipDown();
    
    /**
     * @brief Marks the card as matched (permanently revealed)
     */
    void setMatched();
    
    /**
     * @brief Checks if the card is currently revealed (face up or matched)
     * @return True if card is revealed, false otherwise
     */
    bool isRevealed() const;
    
    /**
     * @brief Checks if the card is currently animating
     * @return True if card is flipping, false otherwise
     */
    bool isAnimating() const;
    
    /**
     * @brief Checks if the card has been matched
     * @return True if card is matched, false otherwise
     */
    bool isMatched() const;
    
    /**
     * @brief Checks if a point is within the card's bounds
     * @param point Point to check
     * @return True if point is within card bounds, false otherwise
     */
    bool containsPoint(Vector2 point) const;
    
    /**
     * @brief Gets the card's unique ID
     * @return Card ID
     */
    int getId() const { return m_id; }
    
    /**
     * @brief Gets the card's current state
     * @return Current CardState
     */
    CardState getState() const { return m_state; }
    
    /**
     * @brief Gets the card's position
     * @return Card position as Vector2
     */
    Vector2 getPosition() const { return m_position; }
    
    /**
     * @brief Gets the card's size
     * @return Card size as Vector2
     */
    Vector2 getSize() const { return m_size; }
    
    /**
     * @brief Gets the card's bounding rectangle
     * @return Rectangle representing card bounds
     */
    Rectangle getBounds() const;
    
    /**
     * @brief Sets the card's position
     * @param position New position
     */
    void setPosition(Vector2 position);
    
    /**
     * @brief Sets the card's size
     * @param size New size
     */
    void setSize(Vector2 size);

private:
    // Card properties
    int m_id;                    ///< Unique identifier for matching
    Vector2 m_position;          ///< Card position on screen
    Vector2 m_size;              ///< Card dimensions
    CardState m_state;           ///< Current card state
    
    // Textures
    Texture2D m_frontTexture;    ///< Texture when card is face up
    Texture2D m_backTexture;     ///< Texture when card is face down
    std::string m_texturePath;   ///< Path to the front texture
    
    // Animation properties
    float m_animationProgress;   ///< Progress of current animation (0.0 to 1.0)
    float m_animationSpeed;      ///< Speed of flip animation
    float m_scaleX;              ///< Current X scale for flip animation
    
    // Visual properties
    Color m_tint;                ///< Color tint for the card
    float m_rotation;            ///< Card rotation angle
    bool m_isHovered;            ///< Whether mouse is over the card
    
    // Static textures (shared by all cards)
    static Texture2D s_defaultBackTexture;
    static bool s_defaultTexturesLoaded;
    
    // Helper methods
    void loadTextures();
    void unloadTextures();
    void updateAnimation(float deltaTime);
    void drawCardFront() const;
    void drawCardBack() const;
    void drawCardBorder() const;
    void drawHoverEffect() const;
    void drawMatchedEffect() const;
    
    // Static methods for managing shared resources
    static void loadDefaultTextures();
    static void unloadDefaultTextures();
    
    // Animation constants
    static constexpr float FLIP_ANIMATION_SPEED = 8.0f;
    static constexpr float HOVER_SCALE_FACTOR = 1.05f;
    static constexpr float MATCHED_GLOW_ALPHA = 0.7f;
    static constexpr Color HOVER_COLOR = {255, 255, 255, 50};
    static constexpr Color MATCHED_COLOR = {0, 255, 0, 100};
    static constexpr Color BORDER_COLOR = {200, 200, 200, 255};
    static constexpr float BORDER_THICKNESS = 2.0f;
    
    // Allow Card factory functions to access private members
    friend std::unique_ptr<Card> createCard(int id, const std::string& texturePath, 
                                           Vector2 position, Vector2 size);
};