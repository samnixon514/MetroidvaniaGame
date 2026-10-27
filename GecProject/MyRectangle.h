#pragma once
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"

class MyRectangle
{
public:

	MyRectangle(sf::Vector2f position, sf::Vector2f size);

	void draw(sf::RenderWindow* window);
	
	sf::FloatRect getBounds() const;

	void collide(AnimatedSprite&);

	void setRotationAngle(float degrees);

	void setTexture(sf::Texture& texture);

private:

	sf::RectangleShape m_myRectShape;
};


