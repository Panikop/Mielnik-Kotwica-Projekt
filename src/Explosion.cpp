#include "../include/Explosion.h"
#include "../include/Textures.h"

Explosion::Explosion(sf::Vector2f pos) {
  maxLifetime = 0.5f;
  lifetime = maxLifetime;
  active = true;

  sprite.setTexture(Textures::explosionTex);
  sprite.setOrigin(32.f, 32.f); // Zakladajac rozmiar 64x64
  sprite.setPosition(pos);
  sprite.setScale(1.f, 1.f);
}

void Explosion::update(float dt) {
  if (!active) return;
  lifetime -= dt;

  // Efekt zanikania i powiekszania
  float progress = 1.0f - (lifetime / maxLifetime);
  if (progress > 1.0f) progress = 1.0f;

  float scale = 1.0f + progress * 1.5f; // Rozrasta sie
  sprite.setScale(scale, scale);

  sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (lifetime / maxLifetime));
  sprite.setColor(sf::Color(255, 255, 255, alpha));

  if (lifetime <= 0) {
    active = false;
  }
}

void Explosion::draw(sf::RenderWindow& window) {
  if (active) window.draw(sprite);
}
