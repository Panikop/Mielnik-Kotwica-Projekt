#pragma once
#include <string>
#include <vector>
#include "Ship.h"
#include "fraction.h"
#include "Enemy.h"

class SaveSystem {
public:
    // Zapisuje wszystkie podane parametry gry do pliku tekstowego
    static bool saveGame(const std::string& filename, Ship& ship, std::vector<Fraction>& fractions);

    // Wczytuje wszystkie parametry z pliku tekstowego i zastepuje obecny stan
    static bool loadGame(const std::string& filename, Ship& ship, std::vector<Fraction>& fractions);
};
