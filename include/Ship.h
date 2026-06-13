#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Projectile.h"
#include "Mine.h"

using namespace std;

enum class state{
MENU,
STATEK,
LUDZIK,
ANIMACJA,
DEATH,
WIN
};




class SaveSystem;

class Ship{
    friend class SaveSystem;
private:

    float max_health=100;
    float current_health=max_health;

    float max_nitro = 50;
    float current_nitro = max_nitro;
    float nitroDepleteRate = 10;
    float nitroRechargeRate = 1;
    bool nitroOnCooldown = false;

    float max_scrap = 50;
    float current_scrap = 50;

    float max_electronics = 50;
    float current_electronics = 50;

    float max_rare_metals = 50;
    float current_rare_metals = 50;

    float shipSpeed = 200;
    float movementMultiplier = 1;

    int healthUpgradeLevel = 0;
    int nitroUpgradeLevel = 0;
    int storageUpgradeLevel = 0;
    int speedUpgradeLevel = 0;

    sf::Vector2f velocity;


    // zmienne do walki
    float max_shield = 100;
    float current_shield = max_shield;
    float shieldRechargeRate = 5;

    float cannonDamage = 15.f;
    float cannonCooldown = 0.3f;
    float currentCannonCooldown = 0.f;

    float mineDamage = 50.f;
    float mineRadius = 100.f;
    float mineCooldown = 2.0f;
    float currentMineCooldown = 0.f;

    int shieldUpgradeLevel = 0;
    int cannonUpgradeLevel = 0;
    int mineUpgradeLevel = 0;



public:
    void update(float dt, sf::Vector2f mouseWorldPosition, state activeState);

    sf::Vector2f getPosition();
    float calculateMovementMultiplier();

    void createShip();
    sf::Sprite sprite;



    float getMaxHealth();
    float getCurrentHealth();

    float getMaxNitro();
    float getCurrentNitro();
    bool getNitroOnCooldown();

    float getMaxScrap();
    float getCurrentScrap();

    float getMaxElectronics();
    float getCurrentElectronics();

    float getMaxRareMetals();
    float getCurrentRareMetals();

    void collisionMove(float x, float y, float multiplier);

    int getHealthUpgradeLevel() const;
    int getNitroUpgradeLevel() const;
    int getStorageUpgradeLevel() const;
    int getSpeedUpgradeLevel() const;

    sf::Vector3f getHealthUpgradeCost() const;
    sf::Vector3f getNitroUpgradeCost() const;
    sf::Vector3f getStorageUpgradeCost() const;
    sf::Vector3f getSpeedUpgradeCost() const;

    float getShipSpeed() const;

    bool upgradeMaxHealth();
    bool upgradeMaxNitro();
    bool upgradeMaxStorage();
    bool upgradeShipSpeed();
    bool healShipWithScrap();

    std::vector<Projectile> projectiles;
    std::vector<Mine> mines;
    sf::CircleShape shieldShape;

    void shootCannon(sf::Vector2f targetPos);
    void dropMine();
    bool takeDamage(float amount);

    float getCurrentShield();
    float getMaxShield();

    int getShieldUpgradeLevel() const;
    int getCannonUpgradeLevel() const;
    int getMineUpgradeLevel() const;

    sf::Vector3f getShieldUpgradeCost() const;
    sf::Vector3f getCannonUpgradeCost() const;
    sf::Vector3f getMineUpgradeCost() const;

    bool upgradeMaxShield();
    bool upgradeCannon();
    bool upgradeMines();

    void addScrap(float amount);
    void addElectronics(float amount);
    void addRareMetals(float amount);
};
