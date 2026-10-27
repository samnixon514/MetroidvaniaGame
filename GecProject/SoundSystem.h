#pragma once
#include <SFML/Audio.hpp>

class SoundSystem
{
public:
	SoundSystem();

	sf::SoundBuffer m_crateBreakBuffer;
	sf::Sound m_crateBreakSound{ m_crateBreakBuffer };

	sf::SoundBuffer m_enemyHitBuffer;
	sf::Sound m_enemyHitSound{ m_enemyHitBuffer };

	sf::SoundBuffer m_spikeHitBuffer;
	sf::Sound m_spikeHitSound{ m_spikeHitBuffer };

	sf::SoundBuffer m_attackSoundBuffer;
	sf::Sound m_attackSound{ m_attackSoundBuffer };

	sf::Music m_rainMusic;

private:
};

