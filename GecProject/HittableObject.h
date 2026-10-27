#pragma once
#include <SFML/Graphics.hpp>

class HittableObject
{
public:
    virtual ~HittableObject() = default;

    virtual void draw(sf::RenderWindow* window) = 0;
    virtual sf::FloatRect getBounds() const = 0;
};
