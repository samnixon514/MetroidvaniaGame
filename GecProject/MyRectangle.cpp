#include "MyRectangle.h"
#include <iostream>


MyRectangle::MyRectangle(sf::Vector2f position, sf::Vector2f size)
{
	m_myRectShape.setSize(size);

	m_myRectShape.setPosition(position);

	m_myRectShape.setFillColor(sf::Color::Magenta);

}

void MyRectangle::draw(sf::RenderWindow* window)
{
	window->draw(m_myRectShape);
}

sf::FloatRect MyRectangle::getBounds() const
{
	sf::FloatRect box = m_myRectShape.getGlobalBounds();
	box.size.y = 12.f;
	return box;
}

void MyRectangle::collide(AnimatedSprite& player)
{
	auto overlap = player.getPlayerBounds().findIntersection(getBounds());

	if (overlap)
	{
		sf::FloatRect playerBox = player.getPlayerBounds();
		sf::FloatRect rectangleBox = getBounds();

		if (overlap->size.x < overlap->size.y)
		{
			float playerCentre = playerBox.position.x + playerBox.size.x / 2.f;
			float rectangleCentre = rectangleBox.position.x + rectangleBox.size.x / 2.f;

			if (playerCentre < rectangleCentre)
			{
				player.movePlayer(-overlap->size.x, 0.f);
			}
			else
			{
				player.movePlayer(overlap->size.x, 0.f);
			}
		}
		else
		{
			float playerCentre = playerBox.position.y + playerBox.size.y / 2.f;
			float rectangleCentre = rectangleBox.position.y + rectangleBox.size.y / 2.f;

			if (playerCentre < rectangleCentre)
			{
				player.movePlayer(0.f, -overlap->size.y);
				player.land();
			}
			else
			{
				player.movePlayer(0.f, overlap->size.y);
			}
		}
	}
}

void MyRectangle::setRotationAngle(float degrees)
{
	m_myRectShape.setRotation(sf::degrees(degrees));
}

void MyRectangle::setTexture(sf::Texture& texture)
{
	m_myRectShape.setTexture(&texture);
	m_myRectShape.setFillColor(sf::Color::White);
}
