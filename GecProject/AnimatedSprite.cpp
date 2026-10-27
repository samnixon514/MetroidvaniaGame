#include "AnimatedSprite.h"

#include <iostream>

AnimatedSprite::AnimatedSprite(std::string filelocation, std::string idleLocation, std::string attackLocation, std::string jumpLocation, int walkWidth, int walkHeight, int idleWidth, int idleHeight, int attackWidth, int attackHeight, int jumpWidth, int jumpHeight)
{
	if (!animTextureWalk.loadFromFile(filelocation) || !animTextureIdle.loadFromFile(idleLocation) || !animTextureAttack.loadFromFile(attackLocation) || !animTextureJump.loadFromFile(jumpLocation))
	{
		// sprite failed to load
	}
	else
	{

		m_walkWidth = walkWidth;

		m_walkHeight = walkHeight; 

		m_idleWidth = idleWidth;

		m_idleHeight = idleHeight;

		m_attackWidth = attackWidth;

		m_attackHeight = attackHeight;

		m_jumpWidth = jumpWidth;

		m_jumpHeight = jumpHeight;

		sf::Vector2u textXY = animTextureWalk.getSize();

		walkMaxFrames = animTextureWalk.getSize().x / m_walkWidth;

		idleMaxFrames = animTextureIdle.getSize().x / m_idleWidth;

		attackMaxFrames = animTextureAttack.getSize().x / m_attackWidth;

		jumpMaxFrames = animTextureJump.getSize().x / m_jumpWidth;

		m_posX = 30.f;
		m_direction = 1.f;

		m_horizontalMovement = m_direction;

		animSprite = new sf::Sprite(animTextureWalk);

		animSprite->setOrigin(sf::Vector2f({ m_walkWidth / 2.f, 0.f }));

		animSprite->setTextureRect(sf::IntRect({currentFrames * m_walkWidth, 0} , { m_walkWidth, m_walkHeight}));

		animSprite->setPosition(sf::Vector2f({ m_posX, m_posY }));

		animSprite->setScale(sf::Vector2f({ m_scale, m_scale }));

		m_groundY = 576.f - m_walkHeight * m_scale;
		m_posY = m_groundY;
	}
}

//Update the animation, if walking is true then finished walking is false, walk animation plays when moving is true and sets direction. Sets velocity and gravity aspect with time.
void AnimatedSprite::update(sf::Time dt)
{
	m_onGround = false;
	m_velocityY += m_gravity * dt.asSeconds();
	m_posY += m_velocityY * dt.asSeconds();

	if (m_posY >= m_groundY)
	{
		m_posY = m_groundY;
		m_velocityY = 0.f;
		m_onGround = true;

		if (m_isJumping)
		{
			m_isJumping = false;
			m_finishedmoving = !m_moving;
		}
	}

	m_posX += 200.f * m_horizontalMovement * dt.asSeconds();

	animSprite->setPosition(sf::Vector2f({ m_posX, m_posY }));

	float halfWidth = m_walkWidth * m_scale / 2.f;

	if (m_posX < halfWidth) m_posX = halfWidth;
	if (m_posX > 800.f - halfWidth) m_posX = 800.f - halfWidth;

	if (!m_isAttacking && !m_isJumping && m_moving != m_finishedmoving)
	{
		m_finishedmoving = m_moving;
		currentFrames = 0;

		if (m_moving)
		{
			animSprite->setTexture(animTextureWalk, false);
			animSprite->setTextureRect(sf::IntRect({ 0, 0 }, { m_walkWidth, m_walkHeight }));
		}
		else
		{
			animSprite->setTexture(animTextureIdle, false);
			animSprite->setTextureRect(sf::IntRect({ 0, 0 }, { m_idleWidth, m_idleHeight }));
		}

	}
	timer += dt;

	sf::Time animationSpeed = sf::seconds(0.05f);

	if (timer >= animationSpeed)
	{
		timer -= animationSpeed;

		nextFrame();
	}
}

//Draws the sprite
void AnimatedSprite::draw(sf::RenderWindow* window)
{
	window->draw(*animSprite);
}

//Sets directional movement
void AnimatedSprite::setMovement(float dirX, float dirY)
{
	m_horizontalMovement = dirX;

	m_verticalMovement = dirY;

	if (dirX != 0.f)
	{
		m_direction = dirX;
		animSprite->setScale(sf::Vector2f({ dirX * m_scale, m_scale }));
	}

	m_moving = (dirX != 0.f || dirY != 0.f);
}

//Attack animation linked in main when pressing a key on keyboard or controller. 
void AnimatedSprite::startAttack()
{
	if (m_isAttacking)
	{
		return;		
	}

	m_isAttacking = true;
	currentFrames = 0;

	animSprite->setTexture(animTextureAttack, false);
	animSprite->setTextureRect(sf::IntRect({ 0, 0 }, { m_attackWidth, m_attackHeight }));
	
}

//Jumping animation linked to main when pressing a key on keyboard or controller. 
void AnimatedSprite::startJump()
{
	if (m_isJumping ||!m_onGround)
	{
		return;
	}

	m_velocityY = -600.f;
	m_isJumping = true;

	if (!m_isAttacking)
	{
		currentFrames = 0;
		animSprite->setTexture(animTextureJump, false);
		animSprite->setTextureRect(sf::IntRect({ 0, 0 }, { m_jumpWidth, m_jumpHeight }));
	}
}

//Can let go of jump input early to do a little jump or hold for a longer jump set above. 
void AnimatedSprite::endJump()
{
	if (m_velocityY < -200.f)
	{
		m_velocityY = -200.f;
	}
}

//Shortens the outer box of the character so collision seems nicer
sf::FloatRect AnimatedSprite::getPlayerBounds() const
{
	sf::FloatRect box = animSprite->getGlobalBounds();

	box.position.x += box.size.x * 0.2f;
	box.size.x *= 0.65f;

	box.position.y += box.size.y * 0.03f;
	box.size.y *= 0.9f;

	return box;
}

//Attack Hitbox
sf::FloatRect AnimatedSprite::getAttackBounds() const
{
	sf::FloatRect box = getPlayerBounds();

	box.position.x -= 15.f;
	box.size.x += 30.f;

	return box;
}

void AnimatedSprite::movePlayer(float dx, float dy)
{
	m_posX += dx;
	m_posY += dy;

	animSprite->setPosition(sf::Vector2f({ m_posX, m_posY }));
}


//character landing on the ground resets animations and stops jumping
void AnimatedSprite::land()
{
	m_onGround = true;
	m_velocityY = 0.f;
	
	if (m_isJumping)
	{
		m_isJumping = false;
		m_finishedmoving = !m_moving;
	}
}

void AnimatedSprite::handleInput()
{
	float moveDirection = 0.f;
	float moveVertical = 0.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) moveDirection = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) moveDirection = 1.f;

	if (sf::Joystick::isConnected(0))
	{

		float joystickX = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
		float joystickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

		if (moveDirection == 0.f)
		{
			if (joystickX < -30.f) moveDirection = -1.f;
			else if (joystickX > 30.f) moveDirection = 1.f;
		}
	}

	bool pressedAttackKey = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	if (pressedAttackKey && !m_attackWasPressed)
	{
		startAttack();
	}
	m_attackWasPressed = pressedAttackKey;

	bool attackButtonpressed = sf::Joystick::isButtonPressed(0, 2);

	if (attackButtonpressed && !m_attackButtonWasPressed)
	{
		startAttack();
	}
	m_attackButtonWasPressed = attackButtonpressed;

	bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

	if (spacePressed && !m_spaceWasPressed)
	{
		startJump();
	}
	if (!spacePressed && m_spaceWasPressed)
	{
		endJump();
	}

	m_spaceWasPressed = spacePressed;

	bool jumpButtonPressed = sf::Joystick::isButtonPressed(0, 0);

	if (jumpButtonPressed && !m_jumpWasPressed)
	{
		startJump();
	}

	if (!jumpButtonPressed && m_jumpWasPressed)
	{
		endJump();
	}

	m_jumpWasPressed = jumpButtonPressed;

	setMovement(moveDirection, moveVertical);
}

void AnimatedSprite::takeDamage(int amount)
{
	m_health = m_health - amount;

	if (m_health < 0)
	{
		m_health = 0;
	}
}

void AnimatedSprite::respawn()
{
	m_isAttacking = false;
	m_isJumping = false;
	m_health = 3;

	m_posX = 30.f;
	m_direction = 1.f;
	m_horizontalMovement = m_direction;

	m_posY = m_groundY;

	animSprite->setPosition(sf::Vector2f({ m_posX, m_posY }));
}

//Plays animation out and resets the frame when it has been completed. Plays both the walking and idle animation. Also has screen position logic to reset the character into the screen.
void AnimatedSprite::nextFrame()
{ 
	currentFrames++;

	 if (m_isAttacking)
	{
		if (currentFrames >= attackMaxFrames)
		{
			currentFrames = 0;
			m_isAttacking = false;
			
			if (m_isJumping)
			{
				animSprite->setTexture(animTextureJump, false);
			}

			m_finishedmoving = !m_moving;
		}

		animSprite->setTextureRect(sf::IntRect({ currentFrames * m_attackWidth, 0 }, { m_attackWidth, m_attackHeight }));
	}

	 else if (m_isJumping)
	 {
		 if (currentFrames >= jumpMaxFrames)
		 {
			 currentFrames = 0;
			 m_isJumping = false;
			 m_finishedmoving = !m_moving;
		 }
		 animSprite->setTextureRect(sf::IntRect({ currentFrames * m_jumpWidth, 0 }, { m_jumpWidth, m_jumpHeight }));
	 }

	else if (m_moving)
	{
		if (currentFrames >= walkMaxFrames)
		{
			currentFrames = 0;
		}
		animSprite->setTextureRect(sf::IntRect({ currentFrames * m_walkWidth, 0 }, { m_walkWidth, m_walkHeight }));
	}

		else
		{
			if (currentFrames >= idleMaxFrames)
			{
				currentFrames = 0;
			}
				animSprite->setTextureRect(sf::IntRect({ currentFrames * m_idleWidth, 0 }, { m_idleWidth, m_idleHeight }));
		}

	animSprite->setPosition(sf::Vector2f({ m_posX, m_posY }));
}


