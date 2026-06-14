#include "../include/Projectile.h"
#include "../include/Textures.h"
#include <cmath>

Projectile::Projectile(sf::Vector2f startPos, sf::Vector2f direction,
                       float speed, float dmg, sf::Color color) {
  shape.setTexture(Textures::bulletTex);
  shape.setOrigin(5.5f, 16.f);
  shape.setPosition(startPos);
  shape.setColor(color);
  float angleRadians = atan2(direction.y, direction.x);
  shape.setRotation(angleRadians * 180 / M_PI + 90); // rotate bullet
  velocity = direction * speed;
  damage = dmg;
  lifetime = 3.0f;
  active = true;
}

void Projectile::update(float dt, sf::Vector2f playerPos) {
  shape.move(velocity * dt);
  lifetime -= dt;
  if (lifetime <= 0)
    active = false;
}

void Projectile::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(shape);
    }
}
