#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Textures.h"

struct LocalNPC {
    sf::Sprite shape;
    sf::Text nameText;
    std::string name;
    std::string dialogue;

    LocalNPC(sf::Vector2f pos, std::string n, std::string dial, const sf::Font& font) {
        name = n;
        dialogue = dial;

        shape.setTexture(Textures::npcTex);
        shape.setOrigin(10.f, 16.f); // 20x32
        shape.setPosition(pos);
        shape.setColor(sf::Color(50, 200, 50));

        nameText.setFont(font);
        nameText.setString(name);
        nameText.setCharacterSize(14);
        nameText.setFillColor(sf::Color::White);
        sf::FloatRect bounds = nameText.getLocalBounds();
        nameText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height);
        nameText.setPosition(pos.x, pos.y - 18.f);
    }
};
