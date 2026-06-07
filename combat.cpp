#include "combat.h"


Projectile::Projectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, float dmg, sf::Color color) {
    shape.setRadius(4.f);
    shape.setOrigin(4.f, 4.f);
    shape.setPosition(startPos);
    shape.setFillColor(color);
    velocity = direction * speed;
    damage = dmg;
    lifetime = 3.0f;
    active = true;
}

void Projectile::update(float dt) {
    shape.move(velocity * dt);
    lifetime -= dt;
    if (lifetime <= 0) active = false;
}


Mine::Mine(sf::Vector2f pos, float dmg, float radius) {
    maxTimer = 3.0f;
    timer = maxTimer;
    damage = dmg;
    blastRadius = radius;
    exploded = false;
    active = true;
    explosionLinger = 0.5f;

    shape.setRadius(8.f);
    shape.setOrigin(8.f, 8.f);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Red);
    shape.setOutlineColor(sf::Color::Yellow);
    shape.setOutlineThickness(1.f);

    blastShape.setRadius(blastRadius);
    blastShape.setOrigin(blastRadius, blastRadius);
    blastShape.setPosition(pos);
    blastShape.setFillColor(sf::Color::Transparent);
    blastShape.setOutlineColor(sf::Color::Red);
    blastShape.setOutlineThickness(2.f);
}

void Mine::update(float dt) {
    if (!active) return;

    if (!exploded) {
        timer -= dt;

        int blinkRate = static_cast<int>(timer * 10) % 2;
        if (blinkRate == 0) shape.setFillColor(sf::Color::White);
        else shape.setFillColor(sf::Color::Red);

        if (timer <= 0) {
            exploded = true;
        }
    } else {

        explosionLinger -= dt;
        if (explosionLinger <= 0) active = false;
    }
}

Enemy::Enemy(EnemyType t, sf::Vector2f pos) {
    type = t;
    active = true;
    shape.setSize(sf::Vector2f(40.f, 40.f));
    shape.setOrigin(20.f, 20.f);
    shape.setPosition(pos);

    shield = 0; maxShield = 0; shieldRechargeRate = 0;
    shootTimer = 0; shootCooldown = 0;
    mineTimer = 0; mineCooldown = 0;
    angle = 0;

    if (type == EnemyType::BASIC_SHIELDED) {
        hp = 100.f; maxHp = 100.f;
        shape.setFillColor(sf::Color(255, 100, 0)); // Pomaranczowy

        maxShield = 150.f;
        shield = maxShield;
        shieldRechargeRate = 10.f;
        shieldShape.setRadius(35.f);
        shieldShape.setOrigin(35.f, 35.f);
        shieldShape.setOutlineThickness(2.f);
    }
    else if (type == EnemyType::SHOOTER) {
        hp = 80.f; maxHp = 80.f;
        shape.setFillColor(sf::Color(200, 0, 200)); // Fioletowy

        aimDir = sf::Vector2f(-1.f, 0.f);
        shootCooldown = 1.0f;
        shootTimer = shootCooldown;
    }
    else if (type == EnemyType::MINER) {
        hp = 120.f; maxHp = 120.f;
        shape.setFillColor(sf::Color(200, 200, 0)); // Zolty

        centerPoint = pos;
        circleRadius = 150.f;
        flySpeed = 1.5f;
        mineCooldown = 2.5f;
        mineTimer = mineCooldown;
    }
}

void Enemy::update(float dt, std::vector<Projectile>& outProjectiles, std::vector<Mine>& outMines) {
    if (!active) return;

    if (type == EnemyType::BASIC_SHIELDED) {
        if (shield < maxShield) {
            shield += shieldRechargeRate * dt;
            if (shield > maxShield) shield = maxShield;
        }
        shieldShape.setPosition(shape.getPosition());
        shieldShape.setFillColor(sf::Color(255, 100, 0, static_cast<sf::Uint8>(100 * (shield / maxShield))));
        shieldShape.setOutlineColor(sf::Color(255, 150, 0, static_cast<sf::Uint8>(200 * (shield / maxShield))));
    }
    else if (type == EnemyType::SHOOTER) {
        shootTimer -= dt;
        if (shootTimer <= 0) {
            shootTimer = shootCooldown;
            outProjectiles.push_back(Projectile(shape.getPosition(), aimDir, 400.f, 10.f, sf::Color::Yellow));
        }
    }
    else if (type == EnemyType::MINER) {
        angle += flySpeed * dt;
        float x = centerPoint.x + cos(angle) * circleRadius;
        float y = centerPoint.y + sin(angle) * circleRadius;
        shape.setPosition(x, y);

        mineTimer -= dt;
        if (mineTimer <= 0) {
            mineTimer = mineCooldown;
            outMines.push_back(Mine(shape.getPosition(), 30.f, 80.f));
        }
    }
}

void Enemy::takeDamage(float damage) {
    if (shield > 0) {
        shield -= damage;
        if (shield < 0) {
            hp += shield;
            shield = 0;
        }
    } else {
        hp -= damage;
    }
    if (hp <= 0) active = false;
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!active) return;

    window.draw(shape);
    if (type == EnemyType::BASIC_SHIELDED && shield > 0) {
        window.draw(shieldShape);
    }
}
