#include "../include/Ship.h"
#include "../include/Textures.h"

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

float Ship::getMaxScrap() { return Ship::max_scrap; }
float Ship::getCurrentScrap() { return Ship::current_scrap; }

float Ship::getMaxElectronics() { return Ship::max_electronics; }
float Ship::getCurrentElectronics() { return Ship::current_electronics; }

float Ship::getMaxRareMetals() { return Ship::max_rare_metals; }
float Ship::getCurrentRareMetals() { return Ship::current_rare_metals; }

int Ship::getHealthUpgradeLevel() const { return healthUpgradeLevel; }
int Ship::getNitroUpgradeLevel() const { return nitroUpgradeLevel; }
int Ship::getStorageUpgradeLevel() const { return storageUpgradeLevel; }
int Ship::getSpeedUpgradeLevel() const { return speedUpgradeLevel; }

sf::Vector3f Ship::getHealthUpgradeCost() const {
  return sf::Vector3f(20.f + healthUpgradeLevel * 15.f, 10.f + healthUpgradeLevel * 5.f, 5.f + healthUpgradeLevel * 2.f);
}
sf::Vector3f Ship::getNitroUpgradeCost() const {
  return sf::Vector3f(25.f + nitroUpgradeLevel * 15.f, 15.f + nitroUpgradeLevel * 10.f, 5.f + nitroUpgradeLevel * 5.f);
}
sf::Vector3f Ship::getStorageUpgradeCost() const {
  return sf::Vector3f(30.f + storageUpgradeLevel * 20.f, 5.f + storageUpgradeLevel * 5.f, 0.f + storageUpgradeLevel * 2.f);
}
sf::Vector3f Ship::getSpeedUpgradeCost() const {
  return sf::Vector3f(40.f + speedUpgradeLevel * 25.f, 20.f + speedUpgradeLevel * 15.f, 10.f + speedUpgradeLevel * 5.f);
}

float Ship::getShipSpeed() const { return shipSpeed; }

bool Ship::upgradeMaxHealth() {
  sf::Vector3f cost = getHealthUpgradeCost();
  if (current_scrap >= cost.x && current_electronics >= cost.y && current_rare_metals >= cost.z) {
    current_scrap -= cost.x;
    current_electronics -= cost.y;
    current_rare_metals -= cost.z;
    healthUpgradeLevel++;
    max_health += 25.f;
    current_health += 25.f;
    return true;
  }
  return false;
}

bool Ship::upgradeMaxNitro() {
  sf::Vector3f cost = getNitroUpgradeCost();
  if (current_scrap >= cost.x && current_electronics >= cost.y && current_rare_metals >= cost.z) {
    current_scrap -= cost.x;
    current_electronics -= cost.y;
    current_rare_metals -= cost.z;
    nitroUpgradeLevel++;
    max_nitro += 15.f;
    current_nitro += 15.f;
    return true;
  }
  return false;
}

bool Ship::upgradeMaxStorage() {
  sf::Vector3f cost = getStorageUpgradeCost();
  if (current_scrap >= cost.x && current_electronics >= cost.y && current_rare_metals >= cost.z) {
    current_scrap -= cost.x;
    current_electronics -= cost.y;
    current_rare_metals -= cost.z;
    storageUpgradeLevel++;
    max_scrap += 50.f;
    max_electronics += 50.f;
    max_rare_metals += 50.f;
    return true;
  }
  return false;
}

bool Ship::upgradeShipSpeed() {
  sf::Vector3f cost = getSpeedUpgradeCost();
  if (current_scrap >= cost.x && current_electronics >= cost.y && current_rare_metals >= cost.z) {
    current_scrap -= cost.x;
    current_electronics -= cost.y;
    current_rare_metals -= cost.z;
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

sf::Vector3f Ship::getShieldUpgradeCost() const {
  return sf::Vector3f(30.f + shieldUpgradeLevel * 20.f, 25.f + shieldUpgradeLevel * 10.f, 15.f + shieldUpgradeLevel * 5.f);
}
sf::Vector3f Ship::getCannonUpgradeCost() const {
  return sf::Vector3f(40.f + cannonUpgradeLevel * 25.f, 30.f + cannonUpgradeLevel * 15.f, 20.f + cannonUpgradeLevel * 10.f);
}
sf::Vector3f Ship::getMineUpgradeCost() const {
  return sf::Vector3f(35.f + mineUpgradeLevel * 20.f, 20.f + mineUpgradeLevel * 15.f, 10.f + mineUpgradeLevel * 5.f);
}

bool Ship::upgradeMaxShield() {
  sf::Vector3f cost = getShieldUpgradeCost();
  if (current_scrap >= cost.x && current_electronics >= cost.y && current_rare_metals >= cost.z) {
    current_scrap -= cost.x;
    current_electronics -= cost.y;
    current_rare_metals -= cost.z;
    shieldUpgradeLevel++;
    max_shield += 25.f;
    current_shield += 25.f;
    return true;
  }
  return false;
}

bool Ship::upgradeCannon() {
  sf::Vector3f cost = getCannonUpgradeCost();
  if (current_scrap >= cost.x && current_electronics >= cost.y && current_rare_metals >= cost.z) {
    current_scrap -= cost.x;
    current_electronics -= cost.y;
    current_rare_metals -= cost.z;
    cannonUpgradeLevel++;
    cannonDamage += 10.f;
    return true;
  }
  return false;
}

bool Ship::upgradeMines() {
  sf::Vector3f cost = getMineUpgradeCost();
  if (current_scrap >= cost.x && current_electronics >= cost.y && current_rare_metals >= cost.z) {
    current_scrap -= cost.x;
    current_electronics -= cost.y;
    current_rare_metals -= cost.z;
    mineUpgradeLevel++;
    mineDamage += 25.f;
    mineRadius += 15.f;
    return true;
  }
  return false;
}

void Ship::addScrap(float amount) {
  current_scrap += amount;
  if (current_scrap > max_scrap) {
    current_scrap = max_scrap;
  }
}

void Ship::addElectronics(float amount) {
  current_electronics += amount;
  if (current_electronics > max_electronics) {
    current_electronics = max_electronics;
  }
}

void Ship::addRareMetals(float amount) {
  current_rare_metals += amount;
  if (current_rare_metals > max_rare_metals) {
    current_rare_metals = max_rare_metals;
  }
}
