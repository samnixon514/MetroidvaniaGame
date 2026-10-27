#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "HittableObject.h"
class HitObject : public HittableObject
{
public:
    HitObject(std::string filename, sf::Vector2f position, bool hittable, bool breakable, float scale);

    void draw(sf::RenderWindow* window) override;
    sf::FloatRect getBounds() const override;

    bool isBreakable() const;
    bool isBroken() const;
    void breakObject();
    bool isHittable() const;
    void reset();

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    bool m_breakable = false;
    bool m_broken = false;
    bool m_hittable = true;
};

