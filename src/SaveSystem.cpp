#include "../include/SaveSystem.h"
#include <fstream>
#include <iostream>

bool SaveSystem::saveGame(const std::string& filename, Ship& ship, std::vector<Fraction>& fractions, std::vector<Enemy>& enemies) {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    // --- SHIP STATE ---
    out << "[SHIP]\n";
    out << ship.getPosition().x << " " << ship.getPosition().y << "\n";
    out << ship.current_health << " " << ship.max_health << " " << ship.healthUpgradeLevel << "\n";
    out << ship.current_nitro << " " << ship.max_nitro << " " << ship.nitroUpgradeLevel << "\n";
    out << ship.current_shield << " " << ship.max_shield << " " << ship.shieldUpgradeLevel << "\n";
    out << ship.shipSpeed << " " << ship.speedUpgradeLevel << "\n";
    out << ship.cannonDamage << " " << ship.cannonUpgradeLevel << "\n";
    out << ship.mineDamage << " " << ship.mineRadius << " " << ship.mineUpgradeLevel << "\n";
    out << ship.current_scrap << " " << ship.max_scrap << " " << ship.storageUpgradeLevel << "\n";
    out << ship.current_electronics << " " << ship.max_electronics << "\n";
    out << ship.current_rare_metals << " " << ship.max_rare_metals << "\n";

    // --- FRACTIONS ---
    out << "[FRACTIONS]\n";
    out << fractions.size() << "\n";
    for (auto& f : fractions) {
        out << f.getTrustLevel() << "\n"; // Zapisujemy tylko poziom zaufania (Trust)
    }

    // --- ENEMIES ---
    out << "[ENEMIES]\n";
    out << enemies.size() << "\n";
    for (auto& e : enemies) {
        // e.type, pozycja X, Y, hp, shield, active
        out << (int)e.type << " " << e.shape.getPosition().x << " " << e.shape.getPosition().y << " "
            << e.hp << " " << e.shield << " " << e.active << "\n";
    }

    out.close();
    std::cout << "Gra zostala poprawnie zapisana do: " << filename << "\n";
    return true;
}

bool SaveSystem::loadGame(const std::string& filename, Ship& ship, std::vector<Fraction>& fractions, std::vector<Enemy>& enemies) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "Brak pliku zapisu: " << filename << "\n";
        return false;
    }

    std::string header;

    // --- SHIP STATE ---
    in >> header; // Oczekiwane [SHIP]
    if (header != "[SHIP]") return false;

    float px, py;
    in >> px >> py;
    ship.sprite.setPosition(px, py);

    in >> ship.current_health >> ship.max_health >> ship.healthUpgradeLevel;
    in >> ship.current_nitro >> ship.max_nitro >> ship.nitroUpgradeLevel;
    in >> ship.current_shield >> ship.max_shield >> ship.shieldUpgradeLevel;
    in >> ship.shipSpeed >> ship.speedUpgradeLevel;
    in >> ship.cannonDamage >> ship.cannonUpgradeLevel;
    in >> ship.mineDamage >> ship.mineRadius >> ship.mineUpgradeLevel;
    in >> ship.current_scrap >> ship.max_scrap >> ship.storageUpgradeLevel;
    in >> ship.current_electronics >> ship.max_electronics;
    in >> ship.current_rare_metals >> ship.max_rare_metals;

    // --- FRACTIONS ---
    in >> header; // Oczekiwane [FRACTIONS]
    if (header == "[FRACTIONS]") {
        size_t fCount;
        in >> fCount;
        for (size_t i = 0; i < fCount; ++i) {
            int trust;
            in >> trust;
            if (i < fractions.size()) {
                fractions[i].setTrustLevel(trust);
            }
        }
    }

    // --- ENEMIES ---
    in >> header; // Oczekiwane [ENEMIES]
    if (header == "[ENEMIES]") {
        size_t eCount;
        in >> eCount;
        enemies.clear();
        for (size_t i = 0; i < eCount; ++i) {
            int typeInt;
            float ex, ey, ehp, eshield;
            bool eactive;
            in >> typeInt >> ex >> ey >> ehp >> eshield >> eactive;

            EnemyType type = static_cast<EnemyType>(typeInt);
            Enemy e(type, sf::Vector2f(ex, ey));
            e.hp = ehp;
            e.shield = eshield;
            e.active = eactive;
            if (!eactive) {
                e.hp = 0;
            } else {
                e.shape.setPosition(ex, ey);
            }
            enemies.push_back(e);
        }
    }

    in.close();
    std::cout << "Gra zostala poprawnie wczytana z pliku: " << filename << "\n";
    return true;
}
