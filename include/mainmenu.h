#pragma once
#include <SFML/Graphics.hpp>
#include "Ship.h"

class MainMenu {
public:
    MainMenu(sf::Vector2f resolution, sf::Font& font);
    int update(sf::Vector2f mousePos, bool mouseClicked, state& activeState);
    void draw(sf::RenderWindow& window);

private:
    sf::Font& m_font;
    sf::Text m_titleText;
    sf::Text m_newGameText;
    sf::Text m_loadSlot1;
    sf::Text m_loadSlot2;
    sf::Text m_loadSlot3;
    sf::Vector2f m_resolution;
    sf::Texture m_backgroundTexture;
    sf::RectangleShape m_backgroundShape;
};
