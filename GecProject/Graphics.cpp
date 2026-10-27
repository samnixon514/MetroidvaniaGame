#include "Graphics.h"
#include <random>
#include <iostream> 

//Randomfloat used for random number generating
float randomFloat(float min, float max)
{
    static std::mt19937 rng{ std::random_device{}() };
    return std::uniform_real_distribution<float>(min, max)(rng);
}

Graphics::Graphics()
    : m_backgroundSprite(m_backgroundTexture)
{
    for (int i = 0; i < 150; i++)
    {
        raindrops.push_back(makeDrop(randomFloat(0.f, 600.f)));
    }

    //Floating Platforms
    if (m_platformTexture.loadFromFile("Data/Tiles/FloatingPlatform.png"))
    {
        rectangles.push_back(MyRectangle(sf::Vector2f({ 100.f, 480.f }), sf::Vector2f({ 120.f, 29.f })));
        rectangles.push_back(MyRectangle(sf::Vector2f({ 300.f, 390.f }), sf::Vector2f({ 120.f, 29.f })));
        rectangles.push_back(MyRectangle(sf::Vector2f({ 520.f, 300.f }), sf::Vector2f({ 120.f, 29.f })));

        for (auto& rect : rectangles)
        {
            rect.setTexture(m_platformTexture);
        }
    }

    //Initialising objects in the world
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Tree.png", sf::Vector2f({ 80.f, 277.f }), false, false, 1.0f));
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Crate.png", sf::Vector2f({ 735.f, 524.f }), true, true, 0.6f));
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Tree.png", sf::Vector2f({ 560.f, 248.f }), false, false, 1.1f));
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Crate.png", sf::Vector2f({ 130.f, 434.f }), true, true, 0.6f));
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Crate.png", sf::Vector2f({ 330.f, 344.f }), true, true, 0.6f));
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Crate.png", sf::Vector2f({ 550.f, 254.f }), true, true, 0.6f));
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Bush.png", sf::Vector2f({ 525.f, 500.f }), false, false, 1.5f));
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Crate.png", sf::Vector2f({ 535.f, 524.f }), true, true, 0.6f));
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Treestump.png", sf::Vector2f({ 20.f, 535.f }), false, false, 1.0f));
    m_hitObjects.push_back(std::make_unique<HitObject>("Data/Object/Stone.png", sf::Vector2f({ 430.f, 520.f }), false, false, 1.0f));
    m_hazards.push_back(std::make_unique<HitObject>("Data/Object/Spikes.png", sf::Vector2f({ 300.f, 560.f }), false, false, 0.3f));
    
    //Background
    if (!m_backgroundTexture.loadFromFile("Data/BG/BG.png"))
    {
        return;
    }
    m_backgroundSprite.setTexture(m_backgroundTexture, true);
    m_backgroundSprite.setScale(sf::Vector2f({ 0.8f, 0.8f }));

    //Ground Texture
    if (m_groundTexture.loadFromFile("Data/Tiles/2.png"))
    {
        m_groundTexture.setRepeated(true);
        m_ground.setSize(sf::Vector2f({ 800.f, 30.f }));
        m_ground.setPosition(sf::Vector2f({ 0.f, 570.f }));
        m_ground.setTexture(&m_groundTexture);
        m_ground.setTextureRect(sf::IntRect({ 0, 0 }, { 3413, 128 }));
    }
}

void Graphics::update(sf::Time dt)
{
    //Raindrop Effect in background, gets made first in draw as you want it to appear behind everything else
    spawnTimer += dt.asSeconds();
    while (spawnTimer >= 0.02f)
    {
        spawnTimer -= 0.02f;
        raindrops.push_back(makeDrop(-15.f));
    }
    for (auto& drop : raindrops)
    {
        drop.shape.move(sf::Vector2f({ 100.f * dt.asSeconds(), drop.speed * dt.asSeconds() }));
    }
    std::erase_if(raindrops, [](const rainDrop& drop)
        {
            return drop.shape.getPosition().y > 600.f || drop.shape.getPosition().x > 800.f;
        });

    m_enemy.update(dt);

    //collision
    for (auto& rect : rectangles)
    {
        rect.collide(*m_player);
    }
    m_particles.update(dt);

    //Breakable Objects if attack anim plays and overlapped with the player and object box
    if (m_player)
    {
        if (m_player->isAttacking())
        {
            if (!m_attackHitDone)
            {
                for (auto& object : m_hitObjects)
                {
                    if (object->isHittable() && !object->isBroken())
                    {
                        auto overlap = m_player->getAttackBounds().findIntersection(object->getBounds());

                        if (overlap)
                        {
                            m_particles.spawnHitParticle(overlap->position + overlap->size / 2.f, 12);
                            m_attackHitDone = true;

                            if (object->isBreakable())
                            {
                                object->breakObject();
                                soundSystem.m_crateBreakSound.play();
                            }
                        }
                    }
                    if (!m_attackHitDone && !m_enemy.isDead())
                    {
                        auto enemyOverlap = m_player->getAttackBounds().findIntersection(m_enemy.getBounds());

                        if (enemyOverlap)
                        {
                            m_particles.spawnHitParticle(
                                enemyOverlap->position + enemyOverlap->size / 2.f,
                                12,
                                sf::Color::Red);
                            m_enemy.takeDamage(1);
                            soundSystem.m_enemyHitSound.play();
                            m_attackHitDone = true;
                        }
                    }
                }
            }
        }
        else
        {
            m_attackHitDone = false;
        }

        bool attacking = m_player->isAttacking();

        if (attacking && !m_wasAttacking)
        {
            soundSystem.m_attackSound.play();
        }

        m_wasAttacking = attacking;
    }

    //Damage Cooldown and Hazards hurting player
    if (m_damageCooldown > 0.f)
    {
        m_damageCooldown = m_damageCooldown - dt.asSeconds();
    }
    if (m_player)
    {
        for (auto& hazard : m_hazards)
        {
            if (m_damageCooldown <= 0.f)
            {
                if (m_player->getPlayerBounds().findIntersection(hazard->getBounds()))
                {
                    m_player->takeDamage(1);
                    soundSystem.m_spikeHitSound.play();
                    m_damageCooldown = 1.f;
                }
            }
        }

        if (m_damageCooldown <= 0.f && !m_enemy.isDead() && !m_player->isAttacking())
        {
            if (m_player->getPlayerBounds().findIntersection(m_enemy.getBounds()))
            {
                m_player->takeDamage(1);
                m_damageCooldown = 1.f;
            }
        }

        if (m_player->getHealth() <= 0)
        {
            m_player->respawn();
            m_damageCooldown = 1.f;

            for (auto& object : m_hitObjects)
            {
                object->reset();
            }
            m_enemy.reset();
        }
    }
}

//Drawing everything involved
void Graphics::draw(sf::RenderWindow* window)
{
    //background
    window->draw(m_backgroundSprite);

    //rain
    for (const auto& drop : raindrops)
    {
        window->draw(drop.shape);
    }

    //Background Objects
    for (auto& object : m_hitObjects)
    {
        object->draw(window);
    }

    for (auto& hazard : m_hazards)
    {
        hazard->draw(window);
    }

    //Ground
    window->draw(m_ground);

    //Platforms
    for (auto& rect : rectangles)
    {
        rect.draw(window);
    }

    m_enemy.draw(window);

    //Player
    if (m_player)
    {
        m_player->draw(window);

        for (int i = 0; i < m_player->getHealth(); i++)
        {
            m_healthBars.setPosition(sf::Vector2f({ 10.f + i * 30.f, 10.f }));
            m_healthBars.setFillColor(sf::Color::Red);
            window->draw(m_healthBars);
        }
    }

    //Particle Effect
    m_particles.draw(window);
}

void Graphics::setPlayer(AnimatedSprite& player)
{
    m_player = &player;
}

void Graphics::makeRectangle(sf::Vector2f position, sf::Vector2f size)
{
    rectangles.push_back(MyRectangle(position, size));
}

//Initialising Raindrops and their qualities 
rainDrop Graphics::makeDrop(float startY)
{
    rainDrop drop;
    drop.shape.setPosition(sf::Vector2f({ randomFloat(-200.f, 800.f), startY }));
    drop.speed = randomFloat(300.f, 600.f);
    drop.shape.setFillColor(sf::Color(165, 210, 250, static_cast<std::uint8_t>(randomFloat(50.f, 200.f))));
    drop.shape.setRotation(sf::degrees(-10.f));
    drop.shape.setSize(sf::Vector2f({ randomFloat(1.f,3.f), randomFloat(5.f, 25.f) }));

    return drop;
}