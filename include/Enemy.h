#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Projectile.h"
#include "GameObject.h"
#include "Mine.h"

enum class EnemyType {
    BASIC_SHIELDED, // Stojacy z odnawiajaca sie tarcza
    SHOOTER,        // Stojacy, strzelajacy przed siebie
    MINER,          // Latajacy w kolko, zrzucajacy miny
    CHASER          // Latajacy za graczem i strzelajacy
};

class Enemy : public GameObject {
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

    void update(float dt, sf::Vector2f playerPos = sf::Vector2f(0.f, 0.f)) override;
    void draw(sf::RenderWindow& window) override;
    bool takeDamage(float damage);
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    sf::Vector2f getPosition() const override { return shape.getPosition(); }

    bool isQuestTarget = false;
};
