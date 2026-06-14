#include "../include/Mine.h"
#include "../include/Textures.h"

Mine::Mine(sf::Vector2f pos, float dmg, float radius) {
  maxTimer = 3.0f;
  timer = maxTimer;
  damage = dmg;
  blastRadius = radius;
  exploded = false;
  active = true;
  explosionLinger = 0.5f;

  shape.setTexture(Textures::mineTex);
  shape.setOrigin(16.f, 16.f); // 32x32 sprite
  shape.setPosition(pos);

  blastShape.setTexture(Textures::explosionTex);
  blastShape.setOrigin(32.f, 32.f); // 64x64 sprite
  blastShape.setPosition(pos);
  float scale = blastRadius / 32.f; // Skalujemy zeby pokrylo zasieg wybuchu
  blastShape.setScale(scale, scale);
}

void Mine::update(float dt, sf::Vector2f playerPos) {
  if (!active)
    return;

  if (!exploded) {
    timer -= dt;

    int blinkRate = static_cast<int>(timer * 10) % 2;
    if (blinkRate == 0)
      shape.setColor(sf::Color::White);
    else
      shape.setColor(sf::Color(255, 100, 100)); // Czerwonawe miganie

    if (timer <= 0) {
      exploded = true;
    }
  } else {
    explosionLinger -= dt;

    // Znika plynnie
    sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (explosionLinger / 0.5f));
    blastShape.setColor(sf::Color(255, 255, 255, alpha));

    if (explosionLinger <= 0)
      active = false;
  }
}

void Mine::draw(sf::RenderWindow& window) {
    if (!active) return;

    if (!exploded) {
        window.draw(shape);
    } else {
        window.draw(blastShape);
    }
}
