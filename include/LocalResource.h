#pragma once
#include <SFML/Graphics.hpp>

struct LocalResource {
    sf::RectangleShape shape;
    int amount;
    bool collected;

    LocalResource(sf::Vector2f pos, int amt) {
        amount = amt;
        collected = false;

        shape.setSize(sf::Vector2f(16.f, 16.f));
        shape.setOrigin(8.f, 8.f);
        shape.setPosition(pos);
        shape.setFillColor(sf::Color(218, 165, 32));
    }
};
