#include "Enemy.h"

Enemy::Enemy(sf::Vector2f position, sf::Vector2f size)
{
	m_enemyShape.setPosition(position);

	m_enemyShape.setSize(size);

	m_enemyShape.setFillColor(sf::Color::Cyan);

	m_leftMaxDistance = position.x - 150.f;
	m_rightMaxDistance = position.x + 100.f;

	m_startPosition = position;

	if (m_texture.loadFromFile("Data/Enemy/EnemyWalk.png"))
	{
		m_enemyShape.setTexture(&m_texture);
		m_enemyShape.setTextureRect(sf::IntRect({ 0, 0 }, { 587, 707 }));
		m_enemyShape.setFillColor(sf::Color::White);
	}
}

//Drawing but returning if enemy is dead
void Enemy::draw(sf::RenderWindow* window)
{
	if (isDead())
	{
		return;
	}

	window->draw(m_enemyShape);
}

//Getting the box of the enemy
sf::FloatRect Enemy::getBounds() const
{
	return m_enemyShape.getGlobalBounds();
}

//Updating
void Enemy::update(sf::Time dt)
{
	if (isDead())
	{
		return;
	}

	m_enemyShape.move(sf::Vector2f({ m_speed * m_direction * dt.asSeconds(), 0.f }));

	float x = m_enemyShape.getPosition().x;
	if (x < m_leftMaxDistance)
	{
		m_direction = 1.f;
	}
	if (x > m_rightMaxDistance)
	{
		m_direction = -1.f;
	}

	m_animationTimer += dt.asSeconds();

	if (m_animationTimer >= m_frameDuration)
	{
		m_animationTimer = 0.f;
		m_currentFrame++;

		if (m_currentFrame >= 10)
		{
			m_currentFrame = 0;
		}
	}
	int frameX = m_currentFrame * 587;

	if (m_direction > 0.f)
	{
		m_enemyShape.setTextureRect(
			sf::IntRect({ frameX, 0 }, { 587, 707 })
		);
	}
	else
	{
		m_enemyShape.setTextureRect(
			sf::IntRect({ frameX + 587, 0 }, { -587, 707 })
		);
	}
}

//Take Damage with healthbar
void Enemy::takeDamage(int amount)
{
	m_health = m_health - amount;
}


//If health is 0 then enemy is dead
bool Enemy::isDead() const
{
	return m_health <= 0;
}

//Resets on player respawn
void Enemy::reset()
{
	m_health = 2;
	m_enemyShape.setPosition(m_startPosition);
	m_direction = 1.f;
}
