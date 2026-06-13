#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Projectile.h"
#include "Mine.h"

enum class EnemyType {
    BASIC_SHIELDED, // Stojacy z odnawiajaca sie tarcza
    SHOOTER,        // Stojacy, strzelajacy przed siebie
    MINER,          // Latajacy w kolko, zrzucajacy miny
    CHASER          // Latajacy za graczem i strzelajacy
};

class Enemy {
public:
    EnemyType type;
    sf::Sprite shape;
    float hp;
    float maxHp;
    bool active;

    float shield;
    float maxShield;
    float shieldRechargeRate;
    sf::CircleShape shieldShape;

    float shootTimer;
    float shootCooldown;
    sf::Vector2f aimDir;

    sf::Vector2f centerPoint;
    float angle;
    float circleRadius;
    float flySpeed;
    float mineTimer;
    float mineCooldown;

    Enemy(EnemyType t, sf::Vector2f pos);
    void update(float dt, sf::Vector2f playerPos, std::vector<Projectile>& outProjectiles, std::vector<Mine>& outMines);
    bool takeDamage(float damage);
    void draw(sf::RenderWindow& window);

    bool isQuestTarget = false;
};
