#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AnimatedSprite.h"
#include "MyRectangle.h"
#include "Enemy.h"
#include "ParticleSystem.h"
#include <memory>
#include "HitObject.h"
#include "SoundSystem.h"


struct rainDrop {
	sf::RectangleShape shape{ sf::Vector2f({1.f, 20.f}) };
	float speed = 0.f;
};

class Graphics
{
public:
	Graphics();

	void update(sf::Time dt);

	void draw(sf::RenderWindow* window);

	void makeRectangle(sf::Vector2f position, sf::Vector2f size);

	void setPlayer(AnimatedSprite& player);


private:
	rainDrop makeDrop(float startY);

	std::vector<rainDrop> raindrops;
	float spawnTimer = 0;
	std::vector<MyRectangle> rectangles;
	AnimatedSprite* m_player = nullptr;

	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite;

	sf::Texture m_groundTexture;
	sf::RectangleShape m_ground;

	ParticleSystem m_particles;

	SoundSystem soundSystem;

	std::vector<std::unique_ptr<HitObject>> m_hitObjects;
	std::vector<std::unique_ptr<HitObject>> m_hazards;

	bool m_attackHitDone = false;
	bool m_wasAttacking = false;

	sf::Texture m_platformTexture;

	sf::RectangleShape m_healthBars{ sf::Vector2f({20.f, 20.f}) };

	float m_damageCooldown = 0.f;

	Enemy m_enemy{ sf::Vector2f({650.f, 495.f}), sf::Vector2f({ 66.f, 80.f }) };
};