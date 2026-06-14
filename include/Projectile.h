#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

struct Projectile : public GameObject {
public:
    sf::Sprite shape;
    sf::Vector2f velocity;
    float damage;
    float lifetime;

    Projectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, float dmg, sf::Color color);
    void update(float dt, sf::Vector2f playerPos = sf::Vector2f(0.f, 0.f)) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    sf::Vector2f getPosition() const override { return shape.getPosition(); }
};
