#include "fraction.h"

Fraction::Fraction() 
    : name("Unknown"), location(0.f, 0.f), trustLevel(0) {}

Fraction::Fraction(const std::string& n, const sf::Vector2f& loc, int trust) 
    : name(n), location(loc), trustLevel(trust) {}

Fraction::Fraction(const std::string& n, float x, float y, int trust) 
    : name(n), location(x, y), trustLevel(trust) {}

void Fraction::createPlanet(const sf::Font& font, float planetRadius, float interactionRadius, sf::Color planetColor) {
    planetShape.setRadius(planetRadius);
    planetShape.setOrigin(planetRadius, planetRadius);
    planetShape.setFillColor(planetColor);
    planetShape.setPosition(location);

    interactionArea.setRadius(interactionRadius);
    interactionArea.setOrigin(interactionRadius, interactionRadius);
    interactionArea.setFillColor(sf::Color::Transparent);
    interactionArea.setOutlineColor(sf::Color::Yellow);
    interactionArea.setOutlineThickness(2.f);
    interactionArea.setPosition(location);

    // Konfiguracja tekstu
    nameText.setFont(font);
    nameText.setString(name);
    nameText.setCharacterSize(24);
    nameText.setFillColor(sf::Color::White);
    nameText.setStyle(sf::Text::Bold);

    // Wyrodkowanie tekstu nad planeta
    sf::FloatRect textBounds = nameText.getLocalBounds();
    nameText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height);
    nameText.setPosition(location.x, location.y - planetRadius - 15.f);
}

sf::Vector2f Fraction::getLocation() const {
    return location;
}

void Fraction::setLocation(const sf::Vector2f& loc) {
    location = loc;
    planetShape.setPosition(location);
    interactionArea.setPosition(location);
    float planetRadius = planetShape.getRadius();
    nameText.setPosition(location.x, location.y - planetRadius - 15.f);
}

void Fraction::setLocation(float x, float y) {
    location.x = x;
    location.y = y;
    planetShape.setPosition(location);
    interactionArea.setPosition(location);
    float planetRadius = planetShape.getRadius();
    nameText.setPosition(location.x, location.y - planetRadius - 15.f);
}

int Fraction::getTrustLevel() const {
    return trustLevel;
}

void Fraction::setTrustLevel(int trust) {
    trustLevel = trust;
}

void Fraction::adjustTrustLevel(int change) {
    trustLevel += change;
}

std::string Fraction::getName() const {
    return name;
}

void Fraction::setName(const std::string& newName) {
    name = newName;
    nameText.setString(name);
    sf::FloatRect textBounds = nameText.getLocalBounds();
    nameText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height);
    float planetRadius = planetShape.getRadius();
    nameText.setPosition(location.x, location.y - planetRadius - 15.f);
}
