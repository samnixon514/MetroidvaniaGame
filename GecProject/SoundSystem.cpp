#include "SoundSystem.h"
#include <iostream>

SoundSystem::SoundSystem()
{
    if (m_rainMusic.openFromFile("Data/Sound/Rain.mp3"))
    {
        m_rainMusic.setVolume(25.f);
        m_rainMusic.setLooping(true);
        m_rainMusic.play();
    }
    if (!m_crateBreakBuffer.loadFromFile("Data/Sound/CrateBreak.mp3"))
    {
        std::cout << "Crate break sound failed to load" << std::endl;
    }
    m_crateBreakSound.setVolume(5.f);

    if (!m_enemyHitBuffer.loadFromFile("Data/Sound/EnemyHit.WAV"))
    {
        std::cout << "Enemy hit sound failed to load" << std::endl;
    }
    m_enemyHitSound.setVolume(5.f);

    if (!m_spikeHitBuffer.loadFromFile("Data/Sound/SpikeHit.WAV"))
    {
        std::cout << "SpikeHit sound failed to load" << std::endl;
    }
    m_spikeHitSound.setVolume(5.f);

    if (m_attackSoundBuffer.loadFromFile("Data/Sound/AttackStartSound.WAV"))
    {
        m_attackSound.setVolume(3.f);
        m_attackSound.setPitch(1.3f);
    }
}
