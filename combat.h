#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>


struct Projectile {
    sf::Sprite shape;
    sf::Vector2f velocity;
    float damage;
    float lifetime;
    bool active;

    Projectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, float dmg, sf::Color color);
    void update(float dt);
};

class Mine {
public:
    sf::Sprite shape;
    sf::Sprite blastShape;
    float timer;
    float maxTimer;
    float blastRadius;
    float damage;
    bool exploded;
    bool active;
    float explosionLinger;

    Mine(sf::Vector2f pos, float dmg, float radius);
    void update(float dt);
};

struct Explosion {
    sf::Sprite sprite;
    float lifetime;
    float maxLifetime;
    bool active;

    Explosion(sf::Vector2f pos);
    void update(float dt);
    void draw(sf::RenderWindow& window);
};

enum class EnemyType {
    BASIC_SHIELDED, // Stojacy z odnawiajaca sie tarcza
    SHOOTER,        // Stojacy, strzelajacy przed siebie
    MINER           // Latajacy w kolko, zrzucajacy miny
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
    void update(float dt, std::vector<Projectile>& outProjectiles, std::vector<Mine>& outMines);
    bool takeDamage(float damage);
    void draw(sf::RenderWindow& window);
};
