#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIBar {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Color m_baseColor;
    sf::Color m_lagColor;

    float m_currentValue;
    float m_lagValue;
    float m_maxValue;

    sf::RectangleShape m_bgRect;
    sf::RectangleShape m_borderRect;
    sf::RectangleShape m_fillRect;
    sf::RectangleShape m_lagRect;
    sf::RectangleShape m_sheenRect;

    sf::Text m_labelText;
    sf::Text m_valueText;

    bool m_isCritical;
    bool m_isOnCooldown;
    float m_pulseTime;

public:
    UIBar();
    UIBar(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor, const std::string& label, const sf::Font& font);
    void update(float dt, float currentValue, float maxValue, bool isOnCooldown = false);
    void draw(sf::RenderWindow& window);
};
