#include "../include/player.h"
#include "../include/Textures.h"

void player::createPlayer()
{
    sprite.setTexture(Textures::playerAnimTex);
    sprite.setTextureRect(sf::IntRect(0, 0, 221, 328));
    sprite.setOrigin(110.f, 164.f); 
    sprite.setScale(0.12f, 0.12f); // Skalowanie obrazka (ok 40px wysokosci)
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
        
        walkTimer += dt * 8.f; // 8 FPS animacji
        if (walkTimer >= 1.f) {
            walkTimer -= 1.f;
            currentFrame = (currentFrame + 1) % 4;
            sprite.setTextureRect(sf::IntRect(currentFrame * 221, 0, 221, 328));
        }

        // Odbicie lustrzane jezeli idzie w lewo
        if (movement.x < 0) {
            sprite.setScale(-0.12f, 0.12f);
        } else if (movement.x > 0) {
            sprite.setScale(0.12f, 0.12f);
        }
        
    } else {
        walkTimer = 0.f;
        currentFrame = 0;
        sprite.setTextureRect(sf::IntRect(0, 0, 221, 328));
    }

    // sf::Vector2f playerPos = sprite.getPosition();
    // sf::Vector2f dir = mouseWorldPosition - playerPos;
    // float angleRadians = std::atan2(dir.y, dir.x);
    // float angleDegrees = angleRadians * 180.f / 3.14159265f;
    // sprite.setRotation(angleDegrees);
}
