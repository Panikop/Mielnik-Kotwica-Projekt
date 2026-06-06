#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Fraction {
private:
    sf::Vector2f location;
    int trustLevel;
    std::string name;

public:
    Fraction();
    Fraction(const std::string& name, const sf::Vector2f& loc, int trust);
    Fraction(const std::string& name, float x, float y, int trust);

    // Planet visual elements
    sf::CircleShape planetShape;
    sf::CircleShape interactionArea;
    sf::Text nameText;

    void createPlanet(const sf::Font& font, float planetRadius = 80.f, float interactionRadius = 150.f, sf::Color planetColor = sf::Color(100, 149, 237));

    // Getters and Setters
    sf::Vector2f getLocation() const;
    void setLocation(const sf::Vector2f& loc);
    void setLocation(float x, float y);

    int getTrustLevel() const;
    void setTrustLevel(int trust);
    void adjustTrustLevel(int change);

    std::string getName() const;
    void setName(const std::string& name);
};
