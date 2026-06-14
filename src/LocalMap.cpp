#include "../include/LocalMap.h"
#include "../include/Textures.h"
#include <iostream>
#include <fstream>
#include <cmath>

LocalMap::LocalMap() {
    mapName = "Unknown";
}

void LocalMap::buildPlayerShipInterior(sf::Vector2f startPos, const sf::Font& font) {

    sf::Color hullColor(100, 110, 120);


    sf::RectangleShape cockpitLeft(sf::Vector2f(20.f, 100.f)); cockpitLeft.setPosition(startPos.x - 60.f, startPos.y - 150.f); cockpitLeft.setFillColor(hullColor);
    sf::RectangleShape cockpitRight(sf::Vector2f(20.f, 100.f)); cockpitRight.setPosition(startPos.x + 40.f, startPos.y - 150.f); cockpitRight.setFillColor(hullColor);
    sf::RectangleShape cockpitTop(sf::Vector2f(120.f, 20.f)); cockpitTop.setPosition(startPos.x - 60.f, startPos.y - 170.f); cockpitTop.setFillColor(hullColor);


    sf::RectangleShape mainLeftTop(sf::Vector2f(60.f, 20.f)); mainLeftTop.setPosition(startPos.x - 120.f, startPos.y - 50.f); mainLeftTop.setFillColor(hullColor);
    sf::RectangleShape mainRightTop(sf::Vector2f(60.f, 20.f)); mainRightTop.setPosition(startPos.x + 60.f, startPos.y - 50.f); mainRightTop.setFillColor(hullColor);

    sf::RectangleShape mainLeft(sf::Vector2f(20.f, 120.f)); mainLeft.setPosition(startPos.x - 120.f, startPos.y - 30.f); mainLeft.setFillColor(hullColor);


    sf::RectangleShape mainRightTopPiece(sf::Vector2f(20.f, 30.f)); mainRightTopPiece.setPosition(startPos.x + 100.f, startPos.y - 30.f); mainRightTopPiece.setFillColor(hullColor);
    sf::RectangleShape mainRightBottomPiece(sf::Vector2f(20.f, 40.f)); mainRightBottomPiece.setPosition(startPos.x + 100.f, startPos.y + 50.f); mainRightBottomPiece.setFillColor(hullColor);


    sf::RectangleShape engineLeftTop(sf::Vector2f(80.f, 20.f)); engineLeftTop.setPosition(startPos.x - 200.f, startPos.y + 90.f); engineLeftTop.setFillColor(hullColor);
    sf::RectangleShape engineRightTop(sf::Vector2f(80.f, 20.f)); engineRightTop.setPosition(startPos.x + 120.f, startPos.y + 90.f); engineRightTop.setFillColor(hullColor);

    sf::RectangleShape engineLeft(sf::Vector2f(20.f, 80.f)); engineLeft.setPosition(startPos.x - 200.f, startPos.y + 110.f); engineLeft.setFillColor(hullColor);
    sf::RectangleShape engineRight(sf::Vector2f(20.f, 80.f)); engineRight.setPosition(startPos.x + 180.f, startPos.y + 110.f); engineRight.setFillColor(hullColor);

    sf::RectangleShape engineBottom(sf::Vector2f(400.f, 20.f)); engineBottom.setPosition(startPos.x - 200.f, startPos.y + 190.f); engineBottom.setFillColor(hullColor);


    walls.push_back(cockpitLeft); walls.push_back(cockpitRight); walls.push_back(cockpitTop);
    walls.push_back(mainLeftTop); walls.push_back(mainRightTop); walls.push_back(mainLeft);
    walls.push_back(mainRightTopPiece); walls.push_back(mainRightBottomPiece);
    walls.push_back(engineLeftTop); walls.push_back(engineRightTop); walls.push_back(engineLeft); walls.push_back(engineRight); walls.push_back(engineBottom);

    for (auto& w : walls) w.setTexture(&Textures::wallTex);

    // strefa wyjscia ze statku z powrotem do kosmosu
    exitZone.setSize(sf::Vector2f(100.f, 120.f));
    exitZone.setPosition(startPos.x - 50.f, startPos.y - 160.f);
    exitZone.setFillColor(sf::Color(0, 255, 100, 100));

    exitText.setFont(font);
    exitText.setString("[SPACJA] - Usiadz za stery");
    exitText.setCharacterSize(14);
    exitText.setFillColor(sf::Color::Green);
    exitText.setPosition(startPos.x - 70.f, startPos.y - 170.f);
}

void LocalMap::generateStationMap(const sf::Font& font) {
    mapName = "Stacja Orbitalna Alpha";


    buildPlayerShipInterior(sf::Vector2f(400.f, 400.f), font);

    sf::RectangleShape topWall(sf::Vector2f(1200.f, 20.f)); topWall.setPosition(0.f, 0.f); topWall.setFillColor(sf::Color(80, 85, 95));
    sf::RectangleShape bottomWall(sf::Vector2f(1200.f, 20.f)); bottomWall.setPosition(0.f, 800.f); bottomWall.setFillColor(sf::Color(80, 85, 95));
    sf::RectangleShape leftWall(sf::Vector2f(20.f, 820.f)); leftWall.setPosition(0.f, 0.f); leftWall.setFillColor(sf::Color(80, 85, 95));
    sf::RectangleShape rightWall(sf::Vector2f(20.f, 820.f)); rightWall.setPosition(1180.f, 0.f); rightWall.setFillColor(sf::Color(80, 85, 95));


    walls.push_back(topWall); walls.push_back(bottomWall); walls.push_back(leftWall); walls.push_back(rightWall);

    for (auto& w : walls) w.setTexture(&Textures::wallTex);

    floorShape.setSize(sf::Vector2f(2000.f, 2000.f));
    floorShape.setPosition(-400.f, -400.f);
    floorShape.setTexture(&Textures::floorTex);
    floorShape.setTextureRect(sf::IntRect(0, 0, 2000, 2000));

    //npc
    npcs.push_back(LocalNPC(sf::Vector2f(800.f, 200.f), "Mechanik", "Zrobie ci maszyne.", font));
    npcs.push_back(LocalNPC(sf::Vector2f(600.f, 500.f), "Zleceniodawca", "Mamy robote.", font));

}

void LocalMap::generatePlanetMap(std::string name, sf::Color groundColor, const sf::Font& font) {
    mapName = name;

    buildPlayerShipInterior(sf::Vector2f(500.f, 500.f), font);

    sf::RectangleShape rock1(sf::Vector2f(200.f, 50.f)); rock1.setPosition(200.f, 200.f); rock1.setFillColor(groundColor);
    sf::RectangleShape rock2(sf::Vector2f(50.f, 300.f)); rock2.setPosition(800.f, 300.f); rock2.setFillColor(groundColor);

    sf::RectangleShape topWall(sf::Vector2f(1200.f, 20.f)); topWall.setPosition(0.f, 0.f); topWall.setFillColor(groundColor);
    sf::RectangleShape bottomWall(sf::Vector2f(1200.f, 20.f)); bottomWall.setPosition(0.f, 800.f); bottomWall.setFillColor(groundColor);
    sf::RectangleShape leftWall(sf::Vector2f(20.f, 820.f)); leftWall.setPosition(0.f, 0.f); leftWall.setFillColor(groundColor);
    sf::RectangleShape rightWall(sf::Vector2f(20.f, 820.f)); rightWall.setPosition(1180.f, 0.f); rightWall.setFillColor(groundColor);

    walls.push_back(rock1);
    walls.push_back(rock2);
    walls.push_back(topWall); walls.push_back(bottomWall); walls.push_back(leftWall); walls.push_back(rightWall);

    for (auto& w : walls) w.setTexture(&Textures::wallTex);

    floorShape.setSize(sf::Vector2f(2000.f, 2000.f));
    floorShape.setPosition(-500.f, -500.f);
    floorShape.setTexture(&Textures::floorTex);
    floorShape.setTextureRect(sf::IntRect(0, 0, 2000, 2000));
    floorShape.setFillColor(groundColor); // Blend with ground color

    npcs.push_back(LocalNPC(sf::Vector2f(500.f, 200.f), "Gubernator", "Witaj u nas.", font));
    npcs.push_back(LocalNPC(sf::Vector2f(700.f, 400.f), "Mieszkaniec", "Spokojnie tutaj dzis.", font));

}


void LocalMap::checkNPCInteractions(sf::Sprite& playerSprite, sf::RenderWindow& window, const sf::Font& font, QuestSystem& qs, int currentMapID, Ship& ship, std::vector<Fraction>& fractions, std::vector<std::unique_ptr<GameObject>>& globalEnemies, bool& openUpgrades) {
    sf::Vector2f playerPos = playerSprite.getPosition();

    for (auto& npc : npcs) {
        sf::Vector2f npcPos = npc.shape.getPosition();
        float dist = std::sqrt(std::pow(playerPos.x - npcPos.x, 2) + std::pow(playerPos.y - npcPos.y, 2));

        if (dist < 80.f) {
            sf::Text interactText;
            interactText.setFont(font);
            interactText.setCharacterSize(16);
            interactText.setOutlineColor(sf::Color::Black);
            interactText.setOutlineThickness(1.f);

            if (npc.name == "Mechanik") {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                    openUpgrades = true;
                    interactText.setFillColor(sf::Color::White);
                    interactText.setString("Mechanik: \"Otwieram panel ulepszen...\"");
                } else {
                    interactText.setFillColor(sf::Color::Yellow);
                    interactText.setString("[E] Ulepsz statek");
                }
            }
            else if (npc.name == "Zleceniodawca" && !qs.currentQuest.active) {
                interactText.setFillColor(sf::Color::Cyan);
                interactText.setString("Dostepne zlecenia:\n[1] " + qs.availableQuests[0].title + "\n[2] " + qs.availableQuests[1].title + "\n[3] " + qs.availableQuests[2].title);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) qs.acceptQuest(0, ship, globalEnemies);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) qs.acceptQuest(1, ship, globalEnemies);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) qs.acceptQuest(2, ship, globalEnemies);
            }
            else {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                    std::string response = qs.tryInteract(npc.name, currentMapID, ship, fractions, globalEnemies);
                    if (response == "") response = npc.dialogue;

                    interactText.setFillColor(sf::Color::White);
                    interactText.setString(npc.name + ": \"" + response + "\"");
                } else {
                    bool canHandIn = (qs.currentQuest.active && (npc.name == qs.currentQuest.giverNPC || npc.name == qs.currentQuest.destNPC));
                    interactText.setFillColor(sf::Color::Yellow);
                    interactText.setString(canHandIn ? "[E] Oddaj zadanie" : "[E] Porozmawiaj");
                }
            }

            sf::FloatRect bounds = interactText.getLocalBounds();
            interactText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height);
            interactText.setPosition(npcPos.x, npcPos.y - 40.f);
            window.draw(interactText);
        }
    }
}


void LocalMap::updateCollisions(sf::Sprite& playerSprite, Ship& ship, QuestSystem& qs) {
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
                if (res.isQuestItem) {
                    qs.registerItemCollected();
                    res.collected = true;
                } else {
                    if (ship.getCurrentScrap() + res.amount <= ship.getMaxScrap()) {
                        ship.addScrap(res.amount);
                        res.collected = true;
                    } else {
                        float freeSpace = ship.getMaxScrap() - ship.getCurrentScrap();
                        ship.addScrap(freeSpace);
                        res.amount -= freeSpace;
                        if (freeSpace > 0) res.collected = true;
                    }
                }
            }
        }
    }
}


//wyjscie
bool LocalMap::checkExit(sf::Sprite& playerSprite) {
    return playerSprite.getGlobalBounds().intersects(exitZone.getGlobalBounds());
}


//rysowanie
void LocalMap::draw(sf::RenderWindow& window) {
    window.draw(floorShape);

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

//asteroida do zadania
void LocalMap::generateAsteroidMap(const sf::Font& font) {
    mapName = "Zbadana Asteroida";
    buildPlayerShipInterior(sf::Vector2f(500.f, 500.f), font);

    sf::RectangleShape rock(sf::Vector2f(200.f, 200.f)); rock.setPosition(200.f, 200.f); rock.setFillColor(sf::Color(100, 100, 100));

    sf::RectangleShape topWall(sf::Vector2f(1200.f, 20.f)); topWall.setPosition(0.f, 0.f); topWall.setFillColor(sf::Color(100, 100, 100));
    sf::RectangleShape bottomWall(sf::Vector2f(1200.f, 20.f)); bottomWall.setPosition(0.f, 800.f); bottomWall.setFillColor(sf::Color(100, 100, 100));
    sf::RectangleShape leftWall(sf::Vector2f(20.f, 820.f)); leftWall.setPosition(0.f, 0.f); leftWall.setFillColor(sf::Color(100, 100, 100));
    sf::RectangleShape rightWall(sf::Vector2f(20.f, 820.f)); rightWall.setPosition(1180.f, 0.f); rightWall.setFillColor(sf::Color(100, 100, 100));

    walls.push_back(rock);
    walls.push_back(topWall); walls.push_back(bottomWall); walls.push_back(leftWall); walls.push_back(rightWall);

    for (auto& w : walls) w.setTexture(&Textures::wallTex);

    floorShape.setSize(sf::Vector2f(2000.f, 2000.f));
    floorShape.setPosition(-500.f, -500.f);
    floorShape.setTexture(&Textures::floorTex);
    floorShape.setTextureRect(sf::IntRect(0, 0, 2000, 2000));
    floorShape.setFillColor(sf::Color(100, 100, 100));

    resources.push_back(LocalResource(sf::Vector2f(800.f, 300.f), 1, true));
}
