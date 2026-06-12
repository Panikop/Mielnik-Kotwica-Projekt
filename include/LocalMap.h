#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "LocalNPC.h"
#include "LocalResource.h"
#include "Ship.h"
#include "QuestSystem.h"

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

    void updateCollisions(sf::Sprite& playerSprite, Ship& ship, QuestSystem& qs);
    bool checkExit(sf::Sprite& playerSprite);
    void draw(sf::RenderWindow& window);

    void buildPlayerShipInterior(sf::Vector2f startPos, const sf::Font& font);
    void checkNPCInteractions(sf::Sprite& playerSprite, sf::RenderWindow& window, const sf::Font& font, QuestSystem& qs, int currentMapID, Ship& ship, std::vector<Fraction>& fractions, std::vector<Enemy>& globalEnemies, bool& openUpgrades);
    void generateAsteroidMap(const sf::Font& font);
};
