#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>


using namespace std;


class player
{
private:
    float max_health=100;
    float current_health=max_health;

    float max_stamina = 50;
    float current_stamina = max_stamina;
    float staminaDepleteRate = 5;
    float staminaRechargeRate = 2;
    bool staminaOnCooldown = false;

    float playerSpeed = 100;
    float movementMultiplier = 1;


public:
    void update(float dt, sf::Vector2f mouseWorldPosition);

    void createPlayer();
    sf::RectangleShape sprite;
};
