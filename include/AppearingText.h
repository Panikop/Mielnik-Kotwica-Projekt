#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct TextElement {
    sf::Text text;
    float lifeTime;
    float maxLifeTime;
};

class AppearingText {
private:
    std::vector<TextElement> texts;
    const sf::Font* font;

public:
    AppearingText();
    void setFont(const sf::Font& f);
    void addText(const std::string& message, sf::Color color = sf::Color::White, float duration = 1.0f);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
};
