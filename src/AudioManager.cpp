#include "../include/AudioManager.h"
#include "../include/Utils.h"

AudioManager::AudioManager() {
    Utils::logInfo("AudioManager constructor START");
    
    // Initialize audio device only if not already ready. Track ownership so
    // we don't close an audio device initialized elsewhere (e.g., in main).
    if (!IsAudioDeviceReady()) {
        Utils::logInfo("Audio device not ready - calling InitAudioDevice()");
        InitAudioDevice();
        m_ownsAudioDevice = true;
        if (IsAudioDeviceReady()) {
            Utils::logInfo("Audio device initialized by AudioManager");
        } else {
            Utils::logWarning("InitAudioDevice() called but device still not ready");
        }
    } else {
        Utils::logInfo("Audio device already ready (AudioManager will not Init/Close it)");
        m_ownsAudioDevice = false;
    }

    // Set master volume to maximum when device is ready
    if (IsAudioDeviceReady()) {
        SetMasterVolume(1.0f);
        Utils::logInfo("Master volume set to 1.0");
    }
    
    // Now load sounds only if audio device is ready
    // Initialize sounds to safe defaults first
    flipSound = {0};
    matchSound = {0};

    if (!IsAudioDeviceReady()) {
        Utils::logWarning("Audio device not ready - skipping loading of sound assets");
    } else {
        if (FileExists("assets/sounds/flip.wav")) {
            flipSound = LoadSound("assets/sounds/flip.wav");
            SetSoundVolume(flipSound, 0.8f);
            Utils::logInfo("Flip sound loaded, volume set to 0.8, frameCount: " + Utils::toString(static_cast<int>(flipSound.frameCount)));
        } else {
            Utils::logWarning("Flip sound not found: assets/sounds/flip.wav");
        }

        if (FileExists("assets/sounds/match.wav")) {
            matchSound = LoadSound("assets/sounds/match.wav");
            SetSoundVolume(matchSound, 1.0f);
            Utils::logInfo("Match sound loaded, volume set to 1.0, frameCount: " + Utils::toString(static_cast<int>(matchSound.frameCount)));
        } else {
            Utils::logWarning("Match sound not found: assets/sounds/match.wav");
        }
    }
    
    Utils::logInfo("AudioManager constructor END");
}

AudioManager::~AudioManager() {
    // Unload sounds only if they were actually loaded
    if (flipSound.frameCount > 0) {
        UnloadSound(flipSound);
        Utils::logInfo("Flip sound unloaded");
    }
    if (matchSound.frameCount > 0) {
        UnloadSound(matchSound);
        Utils::logInfo("Match sound unloaded");
    }

    // Close audio device only if this object opened it
    if (m_ownsAudioDevice) {
        if (IsAudioDeviceReady()) {
            CloseAudioDevice();
            Utils::logInfo("Audio device closed by AudioManager");
        } else {
            Utils::logWarning("Audio device already not ready when AudioManager destructor ran");
        }
    } else {
        Utils::logInfo("Audio device owned by another part of program; not closing");
    }
}

void AudioManager::playFlip() {
    Utils::logInfo("=== playFlip() called ===");
    if (m_muted) {
        Utils::logInfo("Audio muted - skipping playFlip");
        return;
    }

    if (!IsAudioDeviceReady()) {
        Utils::logWarning("Audio device not ready - cannot play flip sound");
        return;
    }

    if (flipSound.frameCount > 0) {
        Utils::logInfo("Flip sound is loaded, attempting to play...");
        PlaySound(flipSound);
        Utils::logInfo("PlaySound(flipSound) executed");
    } else {
        Utils::logWarning("Flip sound not loaded, frameCount = " + Utils::toString(static_cast<int>(flipSound.frameCount)));
    }
}

void AudioManager::playMatch() {
    Utils::logInfo("=== playMatch() called ===");
    if (m_muted) {
        Utils::logInfo("Audio muted - skipping playMatch");
        return;
    }

    if (!IsAudioDeviceReady()) {
        Utils::logWarning("Audio device not ready - cannot play match sound");
        return;
    }

    if (matchSound.frameCount > 0) {
        Utils::logInfo("Match sound is loaded, attempting to play...");
        PlaySound(matchSound);
        Utils::logInfo("PlaySound(matchSound) executed");
    } else {
        Utils::logWarning("Match sound not loaded, frameCount = " + Utils::toString(static_cast<int>(matchSound.frameCount)));
    }
}

void AudioManager::setMuted(bool muted) {
    m_muted = muted;
    Utils::logInfo(std::string("AudioManager mute set to: ") + (m_muted ? "ON" : "OFF"));
    // Optionally set master volume to 0 when muted, restore when unmuted
    if (IsAudioDeviceReady()) {
        SetMasterVolume(m_muted ? 0.0f : 1.0f);
    }
}
// #include "../include/AudioManager.h"
// #include "../include/Utils.h"

// AudioManager::AudioManager() {
//     // Initialize audio device FIRST
//     InitAudioDevice();
    
//     // Set master volume
//     SetMasterVolume(1.0f);  // Full volume
    
//     Utils::logInfo("Audio device initialized");
    
//     // Now load sounds
//     if (FileExists("assets/sounds/flip.wav")) {
//         flipSound = LoadSound("assets/sounds/flip.wav");
//         SetSoundVolume(flipSound, 0.5f);  // 50% volume for flip
//         Utils::logInfo("Flip sound loaded");
//     } else {
//         Utils::logWarning("Flip sound not found: assets/sounds/flip.wav");
//         flipSound.frameCount = 0;
//         flipSound.stream.buffer = nullptr;
//     }
    
//     if (FileExists("assets/sounds/match.wav")) {
//         matchSound = LoadSound("assets/sounds/match.wav");
//         SetSoundVolume(matchSound, 0.7f);  // 70% volume for match
//         Utils::logInfo("Match sound loaded");
//     } else {
//         Utils::logWarning("Match sound not found: assets/sounds/match.wav");
//         matchSound.frameCount = 0;
//         matchSound.stream.buffer = nullptr;
//     }
// }

// AudioManager::~AudioManager() {
//     // Only unload if sounds were actually loaded
//     if (flipSound.frameCount > 0) {
//         UnloadSound(flipSound);
//     }
//     if (matchSound.frameCount > 0) {
//         UnloadSound(matchSound);
//     }
//     CloseAudioDevice();
//     Utils::logInfo("Audio device closed");
// }

// void AudioManager::playFlip() {
//     Utils::logInfo("playFlip() called!");
//     if (flipSound.frameCount > 0) {
//         Utils::logInfo("Playing flip sound NOW");
//         PlaySound(flipSound);
//     } else {
//         Utils::logWarning("Flip sound not loaded, frameCount = 0");
//     }
// }

// void AudioManager::playMatch() {
//     Utils::logInfo("playMatch() called!");
//     if (matchSound.frameCount > 0) {
//         Utils::logInfo("Playing match sound NOW");
//         PlaySound(matchSound);
//     } else {
//         Utils::logWarning("Match sound not loaded, frameCount = 0");
//     }
// }
// // #include "../include/AudioManager.h"
// // #include "../include/Utils.h"

// // AudioManager::AudioManager() {
// //     // Initialize audio device FIRST
// //     InitAudioDevice();
// //     Utils::logInfo("Audio device initialized");
    
// //     // Now load sounds
// //     if (FileExists("assets/sounds/flip.wav")) {
// //         flipSound = LoadSound("assets/sounds/flip.wav");
// //         Utils::logInfo("Flip sound loaded");
// //     } else {
// //         Utils::logWarning("Flip sound not found: assets/sounds/flip.wav");
// //         // Create empty sound to avoid issues
// //         flipSound.frameCount = 0;
// //         flipSound.stream.buffer = nullptr;
// //     }
    
// //     if (FileExists("assets/sounds/match.wav")) {
// //         matchSound = LoadSound("assets/sounds/match.wav");
// //         Utils::logInfo("Match sound loaded");
// //     } else {
// //         Utils::logWarning("Match sound not found: assets/sounds/match.wav");
// //         // Create empty sound to avoid issues
// //         matchSound.frameCount = 0;
// //         matchSound.stream.buffer = nullptr;
// //     }
// // }

// // AudioManager::~AudioManager() {
// //     // Only unload if sounds were actually loaded
// //     if (flipSound.frameCount > 0) {
// //         UnloadSound(flipSound);
// //     }
// //     if (matchSound.frameCount > 0) {
// //         UnloadSound(matchSound);
// //     }
// //     CloseAudioDevice();
// //     Utils::logInfo("Audio device closed");
// // }

// // void AudioManager::playFlip() {
// //     if (flipSound.frameCount > 0) {
// //         PlaySound(flipSound);
// //     }
// // }

// // void AudioManager::playMatch() {
// //     if (matchSound.frameCount > 0) {
// //         PlaySound(matchSound);
// //     }
// // }

// // // /**
// // //  * @file AudioManager.cpp
// // //  * @brief Audio management implementation
// // //  */

// // // #include "../include/AudioManager.h"
// // // #include "../include/Utils.h"

// // // AudioManager::AudioManager() {
// // //     InitAudioDevice();
    
// // //     Utils::logInfo("Audio device initialized");
    
// // //     // Initialize sounds as empty/invalid
// // //     flipSound = {0};
// // //     matchSound = {0};
    
// // //     // Try to load flip sound
// // //     if (FileExists("assets/sounds/flip.wav")) {
// // //         flipSound = LoadSound("assets/sounds/flip.wav");
// // //         Utils::logInfo("Flip sound loaded successfully");
// // //     } else {
// // //         Utils::logWarning("Flip sound not found, will play silently");
// // //     }
    
// // //     // Try to load match sound
// // //     if (FileExists("assets/sounds/match.wav")) {
// // //         matchSound = LoadSound("assets/sounds/match.wav");
// // //         Utils::logInfo("Match sound loaded successfully");
// // //     } else {
// // //         Utils::logWarning("Match sound not found, will play silently");
// // //     }
// // // }

// // // AudioManager::~AudioManager() {
// // //     // Safely unload sounds if they were loaded
// // //     if (flipSound.stream.buffer != nullptr) {
// // //         UnloadSound(flipSound);
// // //     }
// // //     if (matchSound.stream.buffer != nullptr) {
// // //         UnloadSound(matchSound);
// // //     }
    
// // //     CloseAudioDevice();
// // //     Utils::logInfo("Audio device closed");
// // // }

// // // void AudioManager::playFlip() {
// // //     if (flipSound.stream.buffer != nullptr && IsSoundReady(flipSound)) {
// // //         PlaySound(flipSound);
// // //     }
// // // }

// // // void AudioManager::playMatch() {
// // //     if (matchSound.stream.buffer != nullptr && IsSoundReady(matchSound)) {
// // //         PlaySound(matchSound);
// // //     }
// // // }
// // // // #include "../include/AudioManager.h"
// // // // #include "../include/Utils.h"

// // // // AudioManager::AudioManager() {
// // // //     InitAudioDevice();
    
// // // //     // Try to load sounds, use defaults if not found
// // // //     if (FileExists("assets/sounds/flip.wav")) {
// // // //         flipSound = LoadSound("assets/sounds/flip.wav");
// // // //         Utils::logInfo("Flip sound loaded");
// // // //     } else {
// // // //         Utils::logWarning("Flip sound not found, will play silently");
// // // //         flipSound = {0}; // Empty sound
// // // //     }
    
// // // //     if (FileExists("assets/sounds/match.wav")) {
// // // //         matchSound = LoadSound("assets/sounds/match.wav");
// // // //         Utils::logInfo("Match sound loaded");
// // // //     } else {
// // // //         Utils::logWarning("Match sound not found, will play silently");
// // // //         matchSound = {0}; // Empty sound
// // // //     }
// // // // }

// // // // AudioManager::~AudioManager() {
// // // //     if (flipSound.frameCount > 0) UnloadSound(flipSound);
// // // //     if (matchSound.frameCount > 0) UnloadSound(matchSound);
// // // //     CloseAudioDevice();
// // // // }

// // // // void AudioManager::playFlip() {
// // // //     if (flipSound.frameCount > 0) {
// // // //         PlaySound(flipSound);
// // // //     }
// // // // }

// // // // void AudioManager::playMatch() {
// // // //     if (matchSound.frameCount > 0) {
// // // //         PlaySound(matchSound);
// // // //     }
// // // // }

// // // // #include "../include/AudioManager.h"
// // // // #include "../include/Utils.h"

// // // // AudioManager::AudioManager() {
// // // //     flipSound = LoadSound("assets/sounds/flip.wav");
// // // //     matchSound = LoadSound("assets/sounds/match.wav");
// // // // }

// // // // AudioManager::~AudioManager() {
// // // //     UnloadSound(flipSound);
// // // //     UnloadSound(matchSound);
// // // // }

// // // // void AudioManager::playFlip() {
// // // //     PlaySound(flipSound);
// // // // }

// // // // void AudioManager::playMatch() {
// // // //     PlaySound(matchSound);
// // // // }
