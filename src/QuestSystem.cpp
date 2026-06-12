#include "../include/QuestSystem.h"
#include <cstdlib>

QuestSystem::QuestSystem() {
    wreckShape.setRadius(40.f);
    wreckShape.setOrigin(40.f, 40.f);
    wreckShape.setFillColor(sf::Color(100, 100, 120));
    wreckShape.setOutlineThickness(3.f);

    asteroidShape.setRadius(80.f);
    asteroidShape.setOrigin(80.f, 80.f);
    asteroidShape.setFillColor(sf::Color(130, 120, 110));
}

void QuestSystem::generateAvailableQuests(const std::vector<Fraction>& fractions) {
    availableQuests.clear();

    for(int i = 0; i < 3; i++) {
        Quest q;
        q.giverMapID = -1;
        q.giverNPC = "Zleceniodawca";
        q.type = static_cast<QuestType>(rand() % 5);

        q.destMapID = rand() % fractions.size();
        q.destNPC = "Gubernator";

        q.reward = static_cast<RewardType>(rand() % 3);
        q.rewardAmt = 50 + (rand() % 100);
        q.repReward = 5 + (rand() % 10);

        q.targetFractionID = (q.type == QuestType::CARGO || q.type == QuestType::VIP) ? q.destMapID : (rand() % fractions.size());

        float randX = (rand() % 4000) - 2000.f;
        float randY = (rand() % 4000) - 2000.f;
        q.targetPos = sf::Vector2f(randX, randY);
        q.searchArea = q.targetPos + sf::Vector2f((rand()%600)-300.f, (rand()%600)-300.f);

        switch (q.type) {
            case QuestType::CARGO: q.title = "Dostawa Towaru"; q.cargoRequired = 25; break;
            case QuestType::VIP: q.title = "Transport VIPa"; q.objectiveComplete = true; break;
            case QuestType::KILL: q.title = "Eliminacja Piratow"; q.requiredKills = 3; break;
            case QuestType::ASTEROID: q.title = "Wydobycie z Asteroidy"; break;
            case QuestType::INVESTIGATE: q.title = "Zbadanie Wraku"; break;
        }
        availableQuests.push_back(q);
    }
}

void QuestSystem::acceptQuest(int index, Ship& ship, std::vector<Enemy>& globalEnemies) {
    if (index < 0 || index >= availableQuests.size() || currentQuest.active) return;

    Quest chosen = availableQuests[index];
    if (chosen.type == QuestType::CARGO) {
        if (ship.getCurrentScrap() + chosen.cargoRequired > ship.getMaxScrap()) return;
        ship.addScrap(chosen.cargoRequired);
    }

    currentQuest = chosen;
    currentQuest.active = true;

    if (currentQuest.type == QuestType::ASTEROID) {
        isAsteroidSpawned = true;
        asteroidShape.setPosition(currentQuest.targetPos);
    } else if (currentQuest.type == QuestType::INVESTIGATE) {
        isWreckSpawned = true;
        wreckShape.setPosition(currentQuest.targetPos);
    } else if (currentQuest.type == QuestType::KILL) {
        for (int i = 0; i < currentQuest.requiredKills; ++i) {
            Enemy e(EnemyType::SHOOTER, currentQuest.targetPos + sf::Vector2f(i * 100.f, i * 100.f));
            e.isQuestTarget = true;
            globalEnemies.push_back(e);
        }
    }
}

std::string QuestSystem::tryInteract(const std::string& npcName, int mapID, Ship& ship, std::vector<Fraction>& fractions, std::vector<Enemy>& globalEnemies) {
    if (currentQuest.active) {
        bool isDestNPC = (npcName == currentQuest.destNPC && mapID == currentQuest.destMapID);
        bool isGiverNPC = (npcName == currentQuest.giverNPC && mapID == currentQuest.giverMapID);

        if ((currentQuest.type == QuestType::CARGO || currentQuest.type == QuestType::VIP) && isDestNPC) {
            if (currentQuest.type == QuestType::CARGO) ship.addScrap(-currentQuest.cargoRequired);

            if (currentQuest.reward == RewardType::SCRAP) ship.addScrap(currentQuest.rewardAmt);
            else if (currentQuest.reward == RewardType::ELECTRONICS) ship.addElectronics(currentQuest.rewardAmt);
            else if (currentQuest.reward == RewardType::RARE_METALS) ship.addRareMetals(currentQuest.rewardAmt);

            fractions[currentQuest.targetFractionID].adjustTrustLevel(currentQuest.repReward);
            currentQuest.active = false;
            generateAvailableQuests(fractions);
            return "Dziekuje! Towar dotarl, a twoja reputacja rosnie.";
        }
        else if ((currentQuest.type == QuestType::KILL || currentQuest.type == QuestType::ASTEROID || currentQuest.type == QuestType::INVESTIGATE) && isGiverNPC) {
            if (currentQuest.objectiveComplete) {
                if (currentQuest.reward == RewardType::SCRAP) ship.addScrap(currentQuest.rewardAmt);
                else if (currentQuest.reward == RewardType::ELECTRONICS) ship.addElectronics(currentQuest.rewardAmt);
                else if (currentQuest.reward == RewardType::RARE_METALS) ship.addRareMetals(currentQuest.rewardAmt);

                fractions[currentQuest.targetFractionID].adjustTrustLevel(currentQuest.repReward);
                currentQuest.active = false;
                isAsteroidSpawned = false;
                isWreckSpawned = false;
                generateAvailableQuests(fractions);
                return "Dobra robota! Wynagrodzenie i reputacja przyznane.";
            } else {
                return "Jeszcze nie wykonales zadania!";
            }
        }
    }
    return "";
}

void QuestSystem::update(float dt) {
    if (currentQuest.active && currentQuest.type == QuestType::INVESTIGATE) {
        wreckShape.setOutlineColor(currentQuest.objectiveComplete ? sf::Color::Green : sf::Color::Red);
    }
}

void QuestSystem::drawSpaceObjects(sf::RenderWindow& window) {
    if (isAsteroidSpawned) window.draw(asteroidShape);
    if (isWreckSpawned) window.draw(wreckShape);
}

void QuestSystem::drawMapMarkers(sf::RenderWindow& window, const sf::Font& font, const std::vector<Fraction>& fractions) {
    if (!currentQuest.active) return;

    sf::Text markerText;
    markerText.setFont(font);
    markerText.setCharacterSize(100);
    markerText.setStyle(sf::Text::Bold);
    markerText.setFillColor(sf::Color::Magenta);

    if (currentQuest.type == QuestType::CARGO || currentQuest.type == QuestType::VIP) {
        markerText.setString("[CEL DOSTAWY]");
        sf::Vector2f pos = fractions[currentQuest.destMapID].getLocation();
        markerText.setPosition(pos.x, pos.y - 200.f);
        window.draw(markerText);
    }
    else {
        sf::CircleShape searchCircle(600.f);
        searchCircle.setOrigin(600.f, 600.f);
        searchCircle.setPosition(currentQuest.searchArea);
        searchCircle.setFillColor(sf::Color(255, 0, 255, 50));
        searchCircle.setOutlineColor(sf::Color::Magenta);
        searchCircle.setOutlineThickness(10.f);
        window.draw(searchCircle);

        markerText.setString("[STREFA ZADANIA]");
        markerText.setPosition(currentQuest.searchArea.x - 400.f, currentQuest.searchArea.y - 700.f);
        window.draw(markerText);
    }
}

void QuestSystem::registerKill() {
    if (currentQuest.active && currentQuest.type == QuestType::KILL) {
        currentQuest.currentKills++;
        if (currentQuest.currentKills >= currentQuest.requiredKills) currentQuest.objectiveComplete = true;
    }
}

void QuestSystem::registerItemCollected() {
    if (currentQuest.active && currentQuest.type == QuestType::ASTEROID) currentQuest.objectiveComplete = true;
}
