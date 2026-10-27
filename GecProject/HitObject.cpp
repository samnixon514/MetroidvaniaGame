#include "HitObject.h"

HitObject::HitObject(std::string filename, sf::Vector2f position, bool hittable, bool breakable, float scale)
    : m_sprite(m_texture)
{
    m_hittable = hittable;
    m_breakable = breakable;
   
    if (!m_texture.loadFromFile(filename))
    {
        return;
    }

    m_sprite.setTexture(m_texture, true);
    m_sprite.setScale(sf::Vector2f({ scale, scale }));
    m_sprite.setPosition(position);
}

void HitObject::draw(sf::RenderWindow* window)
{
    if (!m_broken)
    {
        window->draw(m_sprite);
    }
}

sf::FloatRect HitObject::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

bool HitObject::isBreakable() const
{
    return m_breakable;
}

bool HitObject::isBroken() const
{
    return m_broken;
}

void HitObject::breakObject()
{
    m_broken = true;
}

bool HitObject::isHittable() const
{
    return m_hittable;
}

void HitObject::reset()
{
    m_broken = false;
}
