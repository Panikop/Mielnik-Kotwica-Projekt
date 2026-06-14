#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class GameObject {
public:
    bool active = true;

    virtual ~GameObject() = default;

    virtual void update(float dt, sf::Vector2f playerPos = sf::Vector2f(0.f, 0.f)) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual sf::FloatRect getBounds() const { return sf::FloatRect(); }
    virtual sf::Vector2f getPosition() const { return sf::Vector2f(0.f, 0.f); }
};

extern std::vector<std::unique_ptr<GameObject>> pendingObjects;
