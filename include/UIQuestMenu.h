#pragma once
#include <SFML/Graphics.hpp>
#include "QuestSystem.h"
#include "fraction.h"
#include <vector>

class UIQuestMenu {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    sf::RectangleShape m_bgRect;
    sf::RectangleShape m_borderRect;

    sf::Text m_titleText;
    sf::Text m_questNameText;
    sf::Text m_questDetailsText;
    sf::Text m_instructionsText;

    sf::Font m_font;

    void centerTextX(sf::Text& text, float y);

public:
    UIQuestMenu();
    UIQuestMenu(sf::Vector2f resolution, const sf::Font& font);

    void update(const QuestSystem& qs, const std::vector<Fraction>& fractions);
    void draw(sf::RenderWindow& window);
};
