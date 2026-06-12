#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Ship.h"
#include "fraction.h"
#include "Enemy.h"

enum class QuestType { CARGO, VIP, KILL, ASTEROID, INVESTIGATE };
enum class RewardType { SCRAP, ELECTRONICS, RARE_METALS };

struct Quest {
    bool active = false;
    QuestType type;
    std::string title;
    std::string description;

    int giverMapID;
    std::string giverNPC;

    int destMapID;
    std::string destNPC;

    sf::Vector2f targetPos;
    sf::Vector2f searchArea;

    RewardType reward;
    int rewardAmt;

    bool objectiveComplete = false;
    int cargoRequired = 0;
    int requiredKills = 0;
    int currentKills = 0;

    int targetFractionID;
    int repReward;
};

class QuestSystem {
public:
    std::vector<Quest> availableQuests;
    Quest currentQuest;

    sf::CircleShape wreckShape;
    bool isWreckSpawned = false;

    sf::CircleShape asteroidShape;
    bool isAsteroidSpawned = false;

    QuestSystem();
    void generateAvailableQuests(const std::vector<Fraction>& fractions);

    void acceptQuest(int index, Ship& ship, std::vector<Enemy>& globalEnemies);
    std::string tryInteract(const std::string& npcName, int mapID, Ship& ship, std::vector<Fraction>& fractions, std::vector<Enemy>& globalEnemies);

    void update(float dt);
    void drawSpaceObjects(sf::RenderWindow& window);
    void drawMapMarkers(sf::RenderWindow& window, const sf::Font& font, const std::vector<Fraction>& fractions);

    void registerKill();
    void registerItemCollected();
};
