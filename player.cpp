#include "player.h"

#include "player.h"

void player::createPlayer()
{
    sprite.setSize(sf::Vector2f(24, 24));
    sprite.setFillColor(sf::Color::Yellow);
    sprite.setOrigin(12, 12);
    sprite.setPosition(200.f, 400.f);
}

void player::update(float dt, sf::Vector2f mouseWorldPosition)
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.f;

    float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (length != 0.f) {
        movement /= length;
        sprite.move(movement * playerSpeed * dt);
    }

    sf::Vector2f playerPos = sprite.getPosition();
    sf::Vector2f dir = mouseWorldPosition - playerPos;
    float angleRadians = std::atan2(dir.y, dir.x);
    float angleDegrees = angleRadians * 180.f / 3.14159265f;
    sprite.setRotation(angleDegrees);
}
