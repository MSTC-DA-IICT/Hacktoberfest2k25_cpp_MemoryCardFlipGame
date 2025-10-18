/**
 * @file AudioManager.h
 * @brief AudioManager class for managing game audio
 * 
 * @author MSTC DA-IICT
 * @version 1.0.0
 * @date 2025-10-14
 */

#pragma once

#include <raylib.h>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    void playFlipSound();
    void playMatchSound();
    void playWinSound();
    
private:
    // Sound effects would be loaded here
};
