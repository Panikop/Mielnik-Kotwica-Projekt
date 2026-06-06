#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace std;

enum class state{
STATEK,
LUDZIK,
ANIMACJA,
};




class Ship{

private:

    float max_health=100;
    float current_health=max_health;

    float max_nitro = 50;
    float current_nitro = max_nitro;
    float nitroDepleteRate = 10;
    float nitroRechargeRate = 1;
    bool nitroOnCooldown = false;

    float max_storage = 100;
    float current_storage = 50;

    float shipSpeed = 200;
    float movementMultiplier = 1;

    int healthUpgradeLevel = 0;
    int nitroUpgradeLevel = 0;
    int storageUpgradeLevel = 0;
    int speedUpgradeLevel = 0;

    sf::Vector2f velocity;



public:
    void update(float dt, sf::Vector2f mouseWorldPosition, state activeState);

    sf::Vector2f getPosition();
    float calculateMovementMultiplier();

    void createShip();
    sf::RectangleShape sprite;



    float getMaxHealth();
    float getCurrentHealth();

    float getMaxNitro();
    float getCurrentNitro();
    bool getNitroOnCooldown();

    float getMaxStorage();
    float getCurrentStorage();

    void collisionMove(float x, float y, float multiplier);

    int getHealthUpgradeLevel() const;
    int getNitroUpgradeLevel() const;
    int getStorageUpgradeLevel() const;
    int getSpeedUpgradeLevel() const;

    float getHealthUpgradeCost() const;
    float getNitroUpgradeCost() const;
    float getStorageUpgradeCost() const;
    float getSpeedUpgradeCost() const;

    float getShipSpeed() const;

    bool upgradeMaxHealth();
    bool upgradeMaxNitro();
    bool upgradeMaxStorage();
    bool upgradeShipSpeed();
};
