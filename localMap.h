#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Ship;

struct LocalNPC {
    sf::CircleShape shape;
    sf::Text nameText;
    std::string name;
    std::string dialogue;

    LocalNPC(sf::Vector2f pos, std::string n, std::string dial, const sf::Font& font) {
        name = n;
        dialogue = dial;

        shape.setRadius(12.f);
        shape.setOrigin(12.f, 12.f);
        shape.setPosition(pos);
        shape.setFillColor(sf::Color(50, 200, 50));

        nameText.setFont(font);
        nameText.setString(name);
        nameText.setCharacterSize(14);
        nameText.setFillColor(sf::Color::White);
        sf::FloatRect bounds = nameText.getLocalBounds();
        nameText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height);
        nameText.setPosition(pos.x, pos.y - 18.f);
    }
};


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

class LocalMap {
public:
    std::string mapName;
    std::vector<sf::RectangleShape> walls;
    std::vector<LocalNPC> npcs;
    std::vector<LocalResource> resources;

    sf::RectangleShape exitZone;
    sf::Text exitText;

    LocalMap();
    void generateStationMap(const sf::Font& font);
    void generatePlanetMap(std::string name, sf::Color groundColor, const sf::Font& font);

    void updateCollisions(sf::RectangleShape& playerSprite, Ship& ship);
    bool checkExit(sf::RectangleShape& playerSprite);
    void draw(sf::RenderWindow& window);
};
