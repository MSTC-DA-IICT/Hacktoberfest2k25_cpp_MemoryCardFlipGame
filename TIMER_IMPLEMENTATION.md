# Timer Feature Implementation

## Overview
This document describes the implementation of the game timer feature for the Memory Card Flip Game. The timer tracks how long players take to complete the game, enabling scoring and competitive gameplay.

## Implementation Details

### Timer Variables (Game.h)
The timer functionality uses three key member variables in the `Game` class:

```cpp
float m_gameStartTime;  // Time when the game started
float m_currentTime;    // Current game time
float m_pausedTime;     // Time when game was paused
```

### Core Timer Methods

#### 1. **getElapsedTime()**
Calculates the elapsed time since the game started, accounting for different game states:

- **Playing**: Returns `currentTime - startTime`
- **Paused**: Returns `pausedTime - startTime` (frozen at pause moment)
- **Game Over**: Returns the final time (frozen when game ended)

```cpp
float Game::getElapsedTime() const {
    if (m_gameStartTime == 0.0f) {
        return 0.0f;
    }
    
    if (m_currentState == GameState::PAUSED) {
        return m_pausedTime - m_gameStartTime;
    }
    
    if (m_currentState == GameState::GAME_OVER) {
        return m_currentTime - m_gameStartTime;
    }
    
    return m_currentTime - m_gameStartTime;
}
```

#### 2. **drawTimer()**
Displays the timer in the top-right corner of the screen:

```cpp
void Game::drawTimer() {
    float elapsedTime = getElapsedTime();
    std::string timeStr = "Time: " + Utils::formatTime(elapsedTime);
    
    // Draw with background for readability
    DrawRectangle(...); // Semi-transparent background
    DrawText(timeStr.c_str(), ...); // Timer text
}
```

#### 3. **startNewGame()**
Initializes the timer when a new game begins:

```cpp
void Game::startNewGame(Difficulty difficulty) {
    // Initialize timer
    m_gameStartTime = GetTime();
    m_currentTime = m_gameStartTime;
    m_pausedTime = 0.0f;
    
    // ... other initialization
}
```

#### 4. **updatePlaying()**
Updates the current time each frame during gameplay:

```cpp
void Game::updatePlaying() {
    // Update current time for timer
    m_currentTime = GetTime();
    
    // ... other update logic
}
```

### Pause/Resume Functionality

The timer correctly handles pause and resume operations:

```cpp
void Game::pauseGame() {
    if (m_currentState == GameState::PLAYING) {
        m_pausedTime = GetTime();
        changeState(GameState::PAUSED);
    }
}

void Game::resumeGame() {
    if (m_currentState == GameState::PAUSED) {
        // Adjust game start time to account for paused duration
        float pauseDuration = GetTime() - m_pausedTime;
        m_gameStartTime += pauseDuration;
        
        changeState(GameState::PLAYING);
    }
}
```

This ensures the timer doesn't count time while the game is paused.

### Time Formatting (Utils.cpp)

The `Utils::formatTime()` function formats seconds into MM:SS format:

```cpp
std::string Utils::formatTime(float seconds) {
    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds / 60;
    int secs = totalSeconds % 60;
    
    std::ostringstream out;
    out << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << secs;
    return out.str();
}
```

### Game Over Display

When the game ends, the final time is displayed on the victory screen:

```cpp
void Game::drawGameOver() {
    // ... congratulations message
    
    std::string timeStr = "Time: " + Utils::formatTime(getElapsedTime());
    drawCenteredText(timeStr, startY, 35, WHITE, m_uiFont);
    
    // ... other stats
}
```

## Features

### ✅ Implemented
1. **Accurate Time Tracking**: Timer starts when game begins
2. **Real-time Display**: Timer shown in top-right corner during gameplay
3. **Pause Support**: Timer freezes when game is paused
4. **Resume Accuracy**: Time excluded during pause periods
5. **Final Time Display**: Completion time shown on victory screen
6. **Formatted Output**: Time displayed in MM:SS format
7. **Game State Integration**: Timer respects all game states

### 🎯 Benefits for Gameplay

1. **Scoring System**: 
   - Time is a factor in score calculation (see `ScoreManager::calculateScore()`)
   - Base score reduced by time penalty: `timePenalty = static_cast<int>(time) * 5`

2. **Competitiveness**:
   - Players can compete for fastest completion times
   - High score system can track best times
   - Encourages replay for better performance

3. **Progress Tracking**:
   - Players can see how long they've been playing
   - Helps set personal goals and challenges
   - Adds urgency and excitement to gameplay

## Usage

### For Players
1. Start a new game from the main menu
2. The timer starts automatically when gameplay begins
3. The timer is visible in the top-right corner
4. Press ESC to pause (timer will freeze)
5. Press ESC again to resume (timer continues from pause point)
6. Complete the game to see your final time

### For Developers
To access the timer in code:

```cpp
// Get elapsed time
float elapsedTime = game.getElapsedTime();

// Format for display
std::string timeStr = Utils::formatTime(elapsedTime);

// Use in scoring
scoreManager.calculateScore(moves, elapsedTime);
```

## Testing Recommendations

1. **Start Timer Test**: Verify timer starts at 0:00 when game begins
2. **Accuracy Test**: Confirm timer increments correctly (1 second per real second)
3. **Pause Test**: Check timer freezes when paused
4. **Resume Test**: Verify paused time is excluded from total
5. **Game Over Test**: Confirm final time is displayed correctly
6. **Format Test**: Ensure MM:SS format is correct (e.g., 01:05 for 65 seconds)

## Future Enhancements

Potential improvements for the timer system:

1. **Millisecond Precision**: Display MM:SS.mmm for precise timing
2. **Best Time Tracking**: Save and display personal best times per difficulty
3. **Leaderboards**: Online leaderboard integration
4. **Time Challenges**: Special modes with time limits
5. **Time Bonuses**: Bonus points for completing under certain times
6. **Statistics**: Average time, improvement tracking, etc.

## Integration with Other Systems

The timer integrates with:

- **ScoreManager**: Uses time for score calculation
- **Game States**: Respects PLAYING, PAUSED, and GAME_OVER states
- **UI System**: Displays in HUD and game over screen
- **Utils**: Uses formatting utilities

## Technical Notes

- Uses `GetTime()` from raylib for accurate timing
- Timer resolution is in seconds (can be enhanced for milliseconds)
- All time values stored as `float` for precision
- State-aware design prevents timing errors during pause/resume
- Zero-padded formatting for consistent display width

## Conclusion

The timer feature is fully implemented and integrated into the game. It provides accurate time tracking, supports pause/resume functionality, and enables competitive gameplay through the scoring system. The implementation is robust, state-aware, and ready for use in the Hacktoberfest 2025 Memory Card Flip Game project.
