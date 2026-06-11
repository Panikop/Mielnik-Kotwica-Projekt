#pragma once
#include <SFML/Graphics.hpp>

struct Explosion {
    sf::Sprite sprite;
    float lifetime;
    float maxLifetime;
    bool active;

    Explosion(sf::Vector2f pos);
    void update(float dt);
    void draw(sf::RenderWindow& window);
};
