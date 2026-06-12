#pragma once
#include <SFML/Graphics.hpp>

struct LocalResource {
    sf::RectangleShape shape;
    int amount;
    bool collected;
    bool isQuestItem;

    LocalResource(sf::Vector2f pos, int amt, bool questItem = false) {
        amount = amt;
        collected = false;
        isQuestItem = questItem;

        shape.setSize(sf::Vector2f(16.f, 16.f));
        shape.setOrigin(8.f, 8.f);
        shape.setPosition(pos);

        if (isQuestItem) shape.setFillColor(sf::Color(255, 50, 255));
        else shape.setFillColor(sf::Color(218, 165, 32));
    }
};
