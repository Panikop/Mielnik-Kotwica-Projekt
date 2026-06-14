#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Mine : public GameObject{
public:
    sf::Sprite shape;
    sf::Sprite blastShape;
    float timer;
    float maxTimer;
    float blastRadius;
    float damage;
    bool exploded;

    float explosionLinger;

    Mine(sf::Vector2f pos, float dmg, float radius);
    void update(float dt, sf::Vector2f playerPos = sf::Vector2f(0.f, 0.f)) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    sf::Vector2f getPosition() const override { return shape.getPosition(); }
};
