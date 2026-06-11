#pragma once
#include <SFML/Graphics.hpp>
#include "Ship.h"

class UIInventoryBar {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    sf::RectangleShape m_bgRect;
    sf::RectangleShape m_borderRect;

    sf::RectangleShape m_scrapFill;
    sf::RectangleShape m_elecFill;
    sf::RectangleShape m_rareFill;

    sf::Text m_scrapText;
    sf::Text m_elecText;
    sf::Text m_rareText;

    float m_scrapCur, m_scrapMax;
    float m_elecCur, m_elecMax;
    float m_rareCur, m_rareMax;

public:
    UIInventoryBar();
    UIInventoryBar(sf::Vector2f position, sf::Vector2f size, const sf::Font& font);

    void update(float dt, Ship& ship);
    void draw(sf::RenderWindow& window);
};
