#pragma once
#include <SFML/Graphics.hpp>

class Mine {
public:
    sf::Sprite shape;
    sf::Sprite blastShape;
    float timer;
    float maxTimer;
    float blastRadius;
    float damage;
    bool exploded;
    bool active;
    float explosionLinger;

    Mine(sf::Vector2f pos, float dmg, float radius);
    void update(float dt);
};
