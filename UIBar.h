#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIBar {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    
    sf::RectangleShape m_bgRect;
    sf::RectangleShape m_borderRect;
    sf::RectangleShape m_lagRect;
    sf::RectangleShape m_fillRect;
    sf::RectangleShape m_sheenRect;
    
    sf::Color m_baseColor;
    sf::Color m_lagColor;
    
    float m_currentValue;
    float m_lagValue;
    float m_maxValue;
    
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

class Ship;

class UIUpgradeMenu {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    
    sf::RectangleShape m_bgRect;
    sf::RectangleShape m_borderRect;
    
    sf::Text m_titleText;
    sf::Text m_scrapsText;
    sf::Text m_upgradesText[4];
    sf::Text m_instructionsText;
    
    sf::Font m_font;

    void centerTextX(sf::Text& text, float y);

public:
    UIUpgradeMenu();
    UIUpgradeMenu(sf::Vector2f resolution, const sf::Font& font);
    
    void update(Ship& ship);
    void draw(sf::RenderWindow& window);
};
