#include "Enemy.h"
#include "Textures.h"

Enemy::Enemy(EnemyType t, sf::Vector2f pos) {
  type = t;
  active = true;
  shape.setTexture(Textures::ufoTex);
  shape.setOrigin(32.f, 32.f); // 64x64 sprite
  shape.setScale(0.625f, 0.625f); // Make it 40x40
  shape.setPosition(pos);

  shield = 0;
  maxShield = 0;
  shieldRechargeRate = 0;
  shootTimer = 0;
  shootCooldown = 0;
  mineTimer = 0;
  mineCooldown = 0;
  angle = 0;

  if (type == EnemyType::BASIC_SHIELDED) {
    hp = 100.f;
    maxHp = 100.f;
    shape.setColor(sf::Color(255, 100, 0)); // Pomaranczowy

    maxShield = 150.f;
    shield = maxShield;
    shieldRechargeRate = 10.f;
    shieldShape.setRadius(35.f);
    shieldShape.setOrigin(35.f, 35.f);
    shieldShape.setOutlineThickness(2.f);
  } else if (type == EnemyType::SHOOTER) {
    hp = 80.f;
    maxHp = 80.f;
    shape.setColor(sf::Color(200, 0, 200)); // Fioletowy

    aimDir = sf::Vector2f(-1.f, 0.f);
    shootCooldown = 1.0f;
    shootTimer = shootCooldown;
  } else if (type == EnemyType::MINER) {
    hp = 120.f;
    maxHp = 120.f;
    shape.setColor(sf::Color(200, 200, 0)); // Zolty

    centerPoint = pos;
    circleRadius = 150.f;
    flySpeed = 1.5f;
    mineCooldown = 2.5f;
    mineTimer = mineCooldown;
  }
}

void Enemy::update(float dt, std::vector<Projectile> &outProjectiles,
                   std::vector<Mine> &outMines) {
  if (!active)
    return;

  if (type == EnemyType::BASIC_SHIELDED) {
    if (shield < maxShield) {
      shield += shieldRechargeRate * dt;
      if (shield > maxShield)
        shield = maxShield;
    }
    shieldShape.setPosition(shape.getPosition());
    shieldShape.setFillColor(sf::Color(
        255, 100, 0, static_cast<sf::Uint8>(10 * (shield / maxShield))));
    shieldShape.setOutlineColor(sf::Color(
        255, 150, 0, static_cast<sf::Uint8>(100 * (shield / maxShield))));
  } else if (type == EnemyType::SHOOTER) {
    shootTimer -= dt;
    if (shootTimer <= 0) {
      shootTimer = shootCooldown;
      outProjectiles.push_back(Projectile(shape.getPosition(), aimDir, 400.f,
                                          10.f, sf::Color::Yellow));
    }
  } else if (type == EnemyType::MINER) {
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

bool Enemy::takeDamage(float damage) {
  if (hp <= 0) return false;
  if (shield > 0) {
    shield -= damage;
    if (shield < 0) {
      hp += shield;
      shield = 0;
    }
  } else {
    hp -= damage;
  }
  if (hp <= 0) {
    active = false;
    return true;
  }
  return false;
}

void Enemy::draw(sf::RenderWindow &window) {
  if (!active)
    return;

  window.draw(shape);
  if (type == EnemyType::BASIC_SHIELDED && shield > 0) {
    window.draw(shieldShape);
  }
}
