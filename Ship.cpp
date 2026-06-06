#include "Ship.h"

void Ship::createShip()
{
    sprite.setSize(sf::Vector2f(30, 30));
    sprite.setFillColor(sf::Color::Red);
    sprite.setOrigin(15, 15);
    velocity.x=0;
    velocity.y=0;
}



void Ship::update(float dt, sf::Vector2f mouseWorldPosition, state activeState)
{



    sf::Vector2f shipPosition = Ship::getPosition();
    sf::Vector2f direction(mouseWorldPosition.x - shipPosition.x, mouseWorldPosition.y - shipPosition.y);


    float dirlength = sqrt(direction.x*direction.x + direction.y * direction.y); //normalizacja wektora


    if(dirlength != 0)
    {
        direction /= dirlength;
    }
    else
    {
        direction.x = 0;
        direction.y = 0;
    }

    sf::Vector2f right(-direction.y, direction.x);


    //KIERUNEK I MNOŻNIK
    movementMultiplier = Ship::calculateMovementMultiplier();
    sf::Vector2f movement(0, 0);
    if(activeState == state::STATEK)
    {

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


    //BOOST

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && current_nitro > 0 && !nitroOnCooldown && (movement.x != 0 || movement.y != 0))
    {
        current_nitro -= nitroDepleteRate * dt;
        movementMultiplier += 1;
        if (current_nitro < 0)
        {
            current_nitro = 0;
            nitroOnCooldown = true;
        }
    }
    else
    {
        if(current_nitro < max_nitro)
        {
            if(nitroOnCooldown && current_nitro > 25)
            {
                nitroOnCooldown = false;
            }

            current_nitro += nitroRechargeRate * dt;
        }
        if(current_nitro > max_nitro)
        {
            current_nitro = max_nitro;
        }
    }

    }


    //RUCH
    velocity.x *= 0.99;
    velocity.y *= 0.99;

    float length = sqrt(movement.x * movement.x + movement.y*movement.y);
    if(length !=0)
    {
        movement /= length;
        sprite.move(movement*shipSpeed*movementMultiplier*dt);
        sf::Vector2f temp = Ship::getPosition();

        cout << "Ship position " << temp.x << " " << temp.y << endl;
    }
    sprite.move(velocity);

    //ROTACJA
    if(activeState == state::STATEK)
    {
    float angleRadians = atan2(direction.y, direction.x);
    float angleDegrees = angleRadians * 180/ M_PI;
    sprite.setRotation(angleDegrees);
    }

}

sf::Vector2f Ship::getPosition()
{
    return sprite.getPosition();
}

float Ship::calculateMovementMultiplier()
{
    Ship::movementMultiplier=1;
    return Ship::movementMultiplier;
}

float Ship::getCurrentHealth()
{
    return Ship::current_health;
}

float Ship::getCurrentNitro()
{
    return Ship::current_nitro;
}

float Ship::getMaxHealth()
{
    return Ship::max_health;
}

float Ship::getMaxNitro()
{
    return Ship::max_nitro;
}

bool Ship::getNitroOnCooldown()
{
    return Ship::nitroOnCooldown;
}

void Ship::collisionMove(float x, float y, float multiplier)
{
    Ship::sprite.move(x,y);
    velocity.x = x*multiplier;
    velocity.y = y*multiplier;
}

float Ship::getMaxStorage()
{
    return Ship::max_storage;
}

float Ship::getCurrentStorage()
{
    return Ship::current_storage;
}

int Ship::getHealthUpgradeLevel() const { return healthUpgradeLevel; }
int Ship::getNitroUpgradeLevel() const { return nitroUpgradeLevel; }
int Ship::getStorageUpgradeLevel() const { return storageUpgradeLevel; }
int Ship::getSpeedUpgradeLevel() const { return speedUpgradeLevel; }

float Ship::getHealthUpgradeCost() const { return 20.f + healthUpgradeLevel * 15.f; }
float Ship::getNitroUpgradeCost() const { return 25.f + nitroUpgradeLevel * 15.f; }
float Ship::getStorageUpgradeCost() const { return 30.f + storageUpgradeLevel * 20.f; }
float Ship::getSpeedUpgradeCost() const { return 40.f + speedUpgradeLevel * 25.f; }

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
