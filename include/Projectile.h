#pragma once
#include <SFML/Graphics.hpp>

struct Projectile {
    sf::Sprite shape;
    sf::Vector2f velocity;
    float damage;
    float lifetime;
    bool active;

    Projectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, float dmg, sf::Color color);
    void update(float dt);
};
