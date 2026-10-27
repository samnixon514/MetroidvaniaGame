#pragma once
#include <SFML/Graphics.hpp>

class AnimatedSprite
{
public:

	AnimatedSprite(std::string filelocation, std::string idleLocation, std::string attackLocation, std::string jumpLocation, int walkWidth, int walkHeight, int idleWidth, int idleHeight, int attackWidth, int attackHeight, int jumpWidth, int jumphHeight);

	void update(sf::Time dt);

	void draw(sf::RenderWindow* window);

	void setMovement(float dirX, float dirY);

	void startAttack();

	void startJump();

	void endJump();

	sf::FloatRect getPlayerBounds() const;

	sf::FloatRect getAttackBounds() const;

	void movePlayer(float dx, float dy);

	void land();

	void handleInput();

	bool isAttacking() const { return m_isAttacking; }

	int getHealth() const { return m_health; }

	void takeDamage(int amount);

	void respawn();

private:

	void nextFrame();

	sf::Sprite* animSprite = nullptr;

	sf::Texture animTextureWalk = sf::Texture();

	sf::Texture animTextureIdle = sf::Texture();

	sf::Texture animTextureAttack = sf::Texture();

	sf::Texture animTextureJump = sf::Texture();

	int currentFrames = 0;
	int walkMaxFrames = 0;
	int m_walkHeight = 0;
	int m_walkWidth = 0;
	sf::Clock dt;
	int m_idleWidth = 0;
	int m_idleHeight = 0;
	int idleMaxFrames = 0;
	int attackMaxFrames = 0;
	int m_attackHeight = 0;
	int m_attackWidth = 0;
	int jumpMaxFrames = 0;
	int m_jumpWidth = 0;
	int m_jumpHeight = 0;
	sf::Time timer{ sf::Time::Zero };
	float m_posX = 400.f;
	float m_posY = 0.f;
	float m_direction = 1.f;
	bool m_moving = true;
	bool m_finishedmoving = true;
	float m_verticalMovement = 0.f;
	float m_horizontalMovement = 0.f;
	float m_scale = 0.15f;
	float m_velocityY = 0.f;
	float m_gravity = 1500.f;
	float m_groundY = 0.f;
	bool m_isAttacking = false;
	bool m_isJumping = false;
	bool m_onGround = false;
	bool m_attackWasPressed = false;
	bool m_attackButtonWasPressed = false;
	bool m_spaceWasPressed = false;
	bool m_jumpWasPressed = false;
	int m_health = 3;
};

