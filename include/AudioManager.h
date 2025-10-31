#pragma once
#include <raylib.h>
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void playFlip();
    void playMatch();
    void setMuted(bool muted);
    bool isMuted() const { return m_muted; }
private:
    Sound flipSound;
    Sound matchSound;
    // Whether this AudioManager initialized the audio device and thus
    // is responsible for closing it in the destructor.
    bool m_ownsAudioDevice = false;
    // Whether audio playback is muted via settings
    bool m_muted = false;
};
