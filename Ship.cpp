#include "Ship.h"
#include "Textures.h"

void Ship::createShip() {
  sprite.setTexture(Textures::shipTex);
  sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
  sprite.setOrigin(32.f, 32.f);
  sprite.setScale(0.8f, 0.8f); // 64 * 0.8 = ~51 size
  velocity.x = 0;
  velocity.y = 0;

  shieldShape.setRadius(35.f);
  shieldShape.setOrigin(35.f, 35.f);
  shieldShape.setFillColor(
      sf::Color(50, 100, 255, 100)); // PĂłĹ‚przezroczysty niebieski
  shieldShape.setOutlineColor(sf::Color(50, 150, 255, 200));
  shieldShape.setOutlineThickness(2.f);
}

void Ship::update(float dt, sf::Vector2f mouseWorldPosition,
                  state activeState) {

  sf::Vector2f shipPosition = Ship::getPosition();
  sf::Vector2f direction(mouseWorldPosition.x - shipPosition.x,
                         mouseWorldPosition.y - shipPosition.y);

  float dirlength = sqrt(direction.x * direction.x +
                         direction.y * direction.y); // normalizacja wektora

  if (dirlength != 0) {
    direction /= dirlength;
  } else {
    direction.x = 0;
    direction.y = 0;
  }

  sf::Vector2f right(-direction.y, direction.x);

  // KIERUNEK I MNOĹ»NIK
  movementMultiplier = Ship::calculateMovementMultiplier();
  sf::Vector2f movement(0, 0);
  if (activeState == state::STATEK) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      movement.x += direction.x;
      movement.y += direction.y;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      movement.x -= direction.x;
      movement.y -= direction.y;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      movement.x -= right.x;
      movement.y -= right.y;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      movement.x += right.x;
      movement.y += right.y;
    }

    // BOOST

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && current_nitro > 0 &&
        !nitroOnCooldown && (movement.x != 0 || movement.y != 0)) {
      current_nitro -= nitroDepleteRate * dt;
      movementMultiplier += 1;
      if (current_nitro < 0) {
        current_nitro = 0;
        nitroOnCooldown = true;
      }
    } else {
      if (current_nitro < max_nitro) {
        if (nitroOnCooldown && current_nitro > 25) {
          nitroOnCooldown = false;
        }

        current_nitro += nitroRechargeRate * dt;
      }
      if (current_nitro > max_nitro) {
        current_nitro = max_nitro;
      }
    }
  }

  // RUCH
  velocity.x *= 0.99;
  velocity.y *= 0.99;

  float length = sqrt(movement.x * movement.x + movement.y * movement.y);
  if (length != 0) {
    movement /= length;
    sprite.move(movement * shipSpeed * movementMultiplier * dt);
    sprite.setTextureRect(sf::IntRect(64, 0, 64, 64));
  } else {
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
  }
  sprite.move(velocity);

  // ROTACJA
  if (activeState == state::STATEK) {
    float angleRadians = atan2(direction.y, direction.x);
    float angleDegrees = angleRadians * 180 / M_PI;
    sprite.setRotation(angleDegrees + 90.f);
  }

  if (activeState == state::STATEK) {
    // Regeneracja tarczy
    if (current_shield < max_shield) {
      current_shield += shieldRechargeRate * dt;
      if (current_shield > max_shield)
        current_shield = max_shield;
    }
    shieldShape.setPosition(sprite.getPosition());
    // IntensywnoĹ›Ä‡ koloru tarczy zaleĹĽy od jej stanu
    shieldShape.setFillColor(
        sf::Color(50, 100, 255,
                  static_cast<sf::Uint8>(100 * (current_shield / max_shield))));

    // Cooldowny broni
    if (currentCannonCooldown > 0)
      currentCannonCooldown -= dt;
    if (currentMineCooldown > 0)
      currentMineCooldown -= dt;

    // Strzelanie (Lewy Przycisk Myszy)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      shootCannon(mouseWorldPosition);
    }
    // Miny (Prawy Przycisk Myszy)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
      dropMine();
    }
  }

  // Aktualizacja pociskĂłw
  for (auto &p : projectiles)
    p.update(dt);
  projectiles.erase(
      std::remove_if(projectiles.begin(), projectiles.end(),
                     [](const Projectile &p) { return !p.active; }),
      projectiles.end());

  // Aktualizacja min
  for (auto &m : mines)
    m.update(dt);
  mines.erase(std::remove_if(mines.begin(), mines.end(),
                             [](const Mine &m) { return !m.active; }),
              mines.end());
}

sf::Vector2f Ship::getPosition() { return sprite.getPosition(); }

float Ship::calculateMovementMultiplier() {
  Ship::movementMultiplier = 1;
  return Ship::movementMultiplier;
}

float Ship::getCurrentHealth() { return Ship::current_health; }

float Ship::getCurrentNitro() { return Ship::current_nitro; }

float Ship::getMaxHealth() { return Ship::max_health; }

float Ship::getMaxNitro() { return Ship::max_nitro; }

bool Ship::getNitroOnCooldown() { return Ship::nitroOnCooldown; }

void Ship::collisionMove(float x, float y, float multiplier) {
  Ship::sprite.move(x, y);
  velocity.x = x * multiplier;
  velocity.y = y * multiplier;
}

float Ship::getMaxStorage() { return Ship::max_storage; }

float Ship::getCurrentStorage() { return Ship::current_storage; }

int Ship::getHealthUpgradeLevel() const { return healthUpgradeLevel; }
int Ship::getNitroUpgradeLevel() const { return nitroUpgradeLevel; }
int Ship::getStorageUpgradeLevel() const { return storageUpgradeLevel; }
int Ship::getSpeedUpgradeLevel() const { return speedUpgradeLevel; }

float Ship::getHealthUpgradeCost() const {
  return 20.f + healthUpgradeLevel * 15.f;
}
float Ship::getNitroUpgradeCost() const {
  return 25.f + nitroUpgradeLevel * 15.f;
}
float Ship::getStorageUpgradeCost() const {
  return 30.f + storageUpgradeLevel * 20.f;
}
float Ship::getSpeedUpgradeCost() const {
  return 40.f + speedUpgradeLevel * 25.f;
}

float Ship::getShipSpeed() const { return shipSpeed; }

bool Ship::upgradeMaxHealth() {
  float cost = getHealthUpgradeCost();
  if (current_storage >= cost) {
    current_storage -= cost;
    healthUpgradeLevel++;
    max_health += 25.f;
    current_health += 25.f;
    return true;
  }
  return false;
}

bool Ship::upgradeMaxNitro() {
  float cost = getNitroUpgradeCost();
  if (current_storage >= cost) {
    current_storage -= cost;
    nitroUpgradeLevel++;
    max_nitro += 15.f;
    current_nitro += 15.f;
    return true;
  }
  return false;
}

bool Ship::upgradeMaxStorage() {
  float cost = getStorageUpgradeCost();
  if (current_storage >= cost) {
    current_storage -= cost;
    storageUpgradeLevel++;
    max_storage += 50.f;
    return true;
  }
  return false;
}

bool Ship::upgradeShipSpeed() {
  float cost = getSpeedUpgradeCost();
  if (current_storage >= cost) {
    current_storage -= cost;
    speedUpgradeLevel++;
    shipSpeed += 30.f;
    return true;
  }
  return false;
}

void Ship::shootCannon(sf::Vector2f targetPos) {
  if (currentCannonCooldown <= 0) {
    sf::Vector2f shipPos = getPosition();
    sf::Vector2f dir = targetPos - shipPos;
    float len = sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0)
      dir /= len;

    // Zielone pociski gracza
    projectiles.push_back(
        Projectile(shipPos, dir, 500.f, cannonDamage, sf::Color::Green));
    currentCannonCooldown = cannonCooldown;
  }
}

void Ship::dropMine() {
  if (currentMineCooldown <= 0) {
    mines.push_back(Mine(getPosition(), mineDamage, mineRadius));
    currentMineCooldown = mineCooldown;
  }
}

bool Ship::takeDamage(float amount) {
  if (current_health <= 0) return false;
  if (current_shield > 0) {
    current_shield -= amount;
    if (current_shield < 0) {
      current_health += current_shield; // przerzucenie obrazen z tarczy na hp
      current_shield = 0;
    }
  } else {
    current_health -= amount;
  }
  if (current_health <= 0) {
    current_health = 0;
    return true;
  }
  return false;
}

float Ship::getCurrentShield() { return current_shield; }
float Ship::getMaxShield() { return max_shield; }

// --- Ulepszenia ---
int Ship::getShieldUpgradeLevel() const { return shieldUpgradeLevel; }
int Ship::getCannonUpgradeLevel() const { return cannonUpgradeLevel; }
int Ship::getMineUpgradeLevel() const { return mineUpgradeLevel; }

float Ship::getShieldUpgradeCost() const {
  return 30.f + shieldUpgradeLevel * 20.f;
}
float Ship::getCannonUpgradeCost() const {
  return 40.f + cannonUpgradeLevel * 25.f;
}
float Ship::getMineUpgradeCost() const {
  return 35.f + mineUpgradeLevel * 20.f;
}

bool Ship::upgradeMaxShield() {
  float cost = getShieldUpgradeCost();
  if (current_storage >= cost) {
    current_storage -= cost;
    shieldUpgradeLevel++;
    max_shield += 25.f;
    current_shield += 25.f;
    return true;
  }
  return false;
}

bool Ship::upgradeCannon() {
  float cost = getCannonUpgradeCost();
  if (current_storage >= cost) {
    current_storage -= cost;
    cannonUpgradeLevel++;
    cannonDamage += 10.f;
    return true;
  }
  return false;
}

bool Ship::upgradeMines() {
  float cost = getMineUpgradeCost();
  if (current_storage >= cost) {
    current_storage -= cost;
    mineUpgradeLevel++;
    mineDamage += 25.f;
    mineRadius += 15.f;
    return true;
  }
  return false;
}

void Ship::addStorage(float amount) {
  current_storage += amount;
  if (current_storage > max_storage) {
    current_storage = max_storage;
  }
}
