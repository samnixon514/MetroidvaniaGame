#include "ParticleSystem.h"
#include <random>

static float randomFloat(float min, float max)
{
	static std::mt19937 rng{ std::random_device{}() };
	return std::uniform_real_distribution<float>(min, max)(rng);
}

void ParticleSystem::spawnHitParticle(sf::Vector2f position, int count, sf::Color colour)
{
	for (int i = 0; i < count; i++)
	{
		particle p;

		p.shape.setPosition(position);
		p.shape.setFillColor(sf::Color(colour));
		p.velocity = sf::Vector2f({ randomFloat(-150.f, 150.f), randomFloat(-150.f, 150.f) });
		p.lifetime = randomFloat(0.3f, 0.8f);
		m_particles.push_back(p);
	}
}

void ParticleSystem::update(sf::Time dt)
{
	for (auto& p : m_particles)
	{
		p.shape.move(sf::Vector2f({ p.velocity.x * dt.asSeconds(), p.velocity.y * dt.asSeconds() }));
		p.lifetime -= dt.asSeconds();
	}

	std::erase_if(m_particles, [](const particle& p)
		{
			return p.lifetime <= 0.f;
		});
}

void ParticleSystem::draw(sf::RenderWindow* window)
{
	for (const auto& p : m_particles)
	{
		window->draw(p.shape);
	}
}
