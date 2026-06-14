#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

struct Explosion : public GameObject{
    public:
    sf::Sprite sprite;
    float lifetime;
    float maxLifetime;

    Explosion(sf::Vector2f pos);
    void update(float dt, sf::Vector2f playerPos = sf::Vector2f(0.f, 0.f)) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const override { return sprite.getPosition(); }
};
