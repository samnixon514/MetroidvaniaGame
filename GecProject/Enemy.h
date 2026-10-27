#pragma once
#include <SFML/Graphics.hpp>
#include "HittableObject.h"
class Enemy : public HittableObject
{
public:
	Enemy(sf::Vector2f position, sf::Vector2f size);

	void draw(sf::RenderWindow* window) override;

	sf::FloatRect getBounds() const override;

	void update(sf::Time dt);

	void takeDamage(int amount);

	bool isDead() const;

	void reset();

private:

	sf::RectangleShape m_enemyShape;
	sf::Vector2f m_startPosition;
	int m_health = 2;
	float m_speed = 100.f;
	float m_direction = 1.f;
	float m_leftMaxDistance = 0.f;
	float m_rightMaxDistance = 0.f;
	sf::Texture m_texture;
	int m_currentFrame = 0;
	float m_animationTimer = 0.f;
	float m_frameDuration = 0.1f;
};

