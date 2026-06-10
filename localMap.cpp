#include "localMap.h"
#include "Ship.h"
#include <cmath>

LocalMap::LocalMap() {
    mapName = "Unknown";
}

void LocalMap::generateStationMap(const sf::Font& font) {
    mapName = "Stacja Orbitalna Alpha";

    exitZone.setSize(sf::Vector2f(40.f, 80.f));
    exitZone.setPosition(100.f, 360.f);
    exitZone.setFillColor(sf::Color(0, 150, 255, 100));

    exitText.setFont(font);
    exitText.setString("[SPACJA] - aby wyjsc");
    exitText.setCharacterSize(14);
    exitText.setFillColor(sf::Color::Cyan);
    exitText.setPosition(80.f, 330.f);


    sf::RectangleShape topWall(sf::Vector2f(800.f, 20.f)); topWall.setPosition(100.f, 200.f); topWall.setFillColor(sf::Color(80, 85, 95));
    sf::RectangleShape bottomWall(sf::Vector2f(800.f, 20.f)); bottomWall.setPosition(100.f, 600.f); bottomWall.setFillColor(sf::Color(80, 85, 95));
    sf::RectangleShape leftWall1(sf::Vector2f(20.f, 160.f)); leftWall1.setPosition(100.f, 200.f); leftWall1.setFillColor(sf::Color(80, 85, 95));
    sf::RectangleShape leftWall2(sf::Vector2f(20.f, 160.f)); leftWall2.setPosition(100.f, 440.f); leftWall2.setFillColor(sf::Color(80, 85, 95));
    sf::RectangleShape rightWall(sf::Vector2f(20.f, 420.f)); rightWall.setPosition(900.f, 200.f); rightWall.setFillColor(sf::Color(80, 85, 95));


    sf::RectangleShape pillar(sf::Vector2f(100.f, 100.f)); pillar.setPosition(450.f, 350.f); pillar.setFillColor(sf::Color(60, 65, 75));

    walls.push_back(topWall);
    walls.push_back(bottomWall);
    walls.push_back(leftWall1);
    walls.push_back(leftWall2);
    walls.push_back(rightWall);
    walls.push_back(pillar);


    npcs.push_back(LocalNPC(sf::Vector2f(550.f, 300.f), "Kolega", "Dobry den", font));
    npcs.push_back(LocalNPC(sf::Vector2f(700.f, 500.f), "Dziwny", "jol", font));


    resources.push_back(LocalResource(sf::Vector2f(300.f, 250.f), 15));
    resources.push_back(LocalResource(sf::Vector2f(800.f, 550.f), 25));
}

void LocalMap::generatePlanetMap(std::string name, sf::Color groundColor, const sf::Font& font) {
    mapName = name;


    exitZone.setSize(sf::Vector2f(60.f, 60.f));
    exitZone.setPosition(500.f, 400.f);
    exitZone.setFillColor(sf::Color(0, 200, 100, 100));

    exitText.setFont(font);
    exitText.setString("[SPACJA] - aby wyjsc");
    exitText.setCharacterSize(14);
    exitText.setFillColor(sf::Color::Green);
    exitText.setPosition(430.f, 370.f);


    sf::RectangleShape rock1(sf::Vector2f(200.f, 50.f)); rock1.setPosition(200.f, 200.f); rock1.setFillColor(groundColor);
    sf::RectangleShape rock2(sf::Vector2f(50.f, 300.f)); rock2.setPosition(800.f, 300.f); rock2.setFillColor(groundColor);

    walls.push_back(rock1);
    walls.push_back(rock2);


    resources.push_back(LocalResource(sf::Vector2f(250.f, 300.f), 40));
    resources.push_back(LocalResource(sf::Vector2f(750.f, 250.f), 30));
    resources.push_back(LocalResource(sf::Vector2f(400.f, 600.f), 50));
}

void LocalMap::updateCollisions(sf::Sprite& playerSprite, Ship& ship) {
    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();

    for (const auto& wall : walls) {
        sf::FloatRect wallBounds = wall.getGlobalBounds();
        sf::FloatRect intersection;

        if (playerBounds.intersects(wallBounds, intersection)) {
            if (intersection.width < intersection.height) {
                if (playerSprite.getPosition().x < wall.getPosition().x + wall.getSize().x / 2.f)
                    playerSprite.move(-intersection.width, 0.f);
                else
                    playerSprite.move(intersection.width, 0.f);
            } else {
                if (playerSprite.getPosition().y < wall.getPosition().y + wall.getSize().y / 2.f)
                    playerSprite.move(0.f, -intersection.height);
                else
                    playerSprite.move(0.f, intersection.height);
            }
            playerBounds = playerSprite.getGlobalBounds();
        }
    }

    for (auto& res : resources) {
        if (!res.collected) {
            if (playerBounds.intersects(res.shape.getGlobalBounds())) {

                if (ship.getCurrentStorage() + res.amount <= ship.getMaxStorage()) {
                    ship.addStorage(res.amount);
                    res.collected = true;
                } else {
                    float freeSpace = ship.getMaxStorage() - ship.getCurrentStorage();
                    ship.addStorage(freeSpace);
                    res.amount -= freeSpace;
                    if (freeSpace > 0) res.collected = true;
                }
            }
        }
    }
}

bool LocalMap::checkExit(sf::Sprite& playerSprite) {
    return playerSprite.getGlobalBounds().intersects(exitZone.getGlobalBounds());
}

void LocalMap::draw(sf::RenderWindow& window) {

    window.draw(exitZone);
    window.draw(exitText);

    for (auto& wall : walls) window.draw(wall);

    for (auto& res : resources) {
        if (!res.collected) window.draw(res.shape);
    }

    for (auto& npc : npcs) {
        window.draw(npc.shape);
        window.draw(npc.nameText);
    }
}
