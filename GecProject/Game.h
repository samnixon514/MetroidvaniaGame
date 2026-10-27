#pragma once
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "Graphics.h"
#include "ExternalHeaders.h"
class Game
{
public:
	Game();

	void running();

private:

	sf::RenderWindow m_window{ sf::VideoMode({ 800, 600 }), "GEC Start Project" };

	sf::Clock m_uiDeltaClock;

	sf::Clock m_updateClock;

	AnimatedSprite m_animSprite{ "Data/RobotRun.png", "Data/RobotIdle.png", "Data/RobotAttack.png", "Data/RobotJump.png", 569, 558, 569, 558, 569, 558, 569, 558 };

	Graphics m_graphics;

	sf::Clock m_fpsClock;
	int m_frameCount = 0;
	float m_fps = 0.f;
};

