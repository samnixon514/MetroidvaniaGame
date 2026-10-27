#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct particle
{
	sf::RectangleShape shape{ sf::Vector2f({4.f, 4.f}) };
	sf::Vector2f velocity;
	float lifetime = 0.f;
};
class ParticleSystem
{
public:
	void spawnHitParticle(sf::Vector2f position, int count, sf::Color colour = sf::Color(255,200,50));
	void update(sf::Time dt);
	void draw(sf::RenderWindow* window);

private:
	std::vector<particle> m_particles;
};

