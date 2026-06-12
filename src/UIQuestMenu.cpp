#include "../include/UIQuestMenu.h"
#include <sstream>

UIQuestMenu::UIQuestMenu() : m_position(0.f, 0.f), m_size(0.f, 0.f) {}

UIQuestMenu::UIQuestMenu(sf::Vector2f resolution, const sf::Font &font) : m_font(font) {
    m_size = sf::Vector2f(500.f, 400.f);
    m_position = sf::Vector2f((resolution.x - m_size.x) / 2.f, (resolution.y - m_size.y) / 2.f);

    m_bgRect.setPosition(m_position);
    m_bgRect.setSize(m_size);
    m_bgRect.setFillColor(sf::Color(15, 20, 25, 240));

    m_borderRect.setPosition(m_position);
    m_borderRect.setSize(m_size);
    m_borderRect.setFillColor(sf::Color::Transparent);
    m_borderRect.setOutlineThickness(2.0f);
    m_borderRect.setOutlineColor(sf::Color(255, 165, 0, 180));

    m_titleText.setFont(m_font);
    m_titleText.setString("DZIENNIK ZADAN");
    m_titleText.setCharacterSize(24);
    m_titleText.setFillColor(sf::Color(255, 165, 0));
    m_titleText.setStyle(sf::Text::Bold);
    m_titleText.setOutlineColor(sf::Color::Black);
    m_titleText.setOutlineThickness(1.5f);
    centerTextX(m_titleText, m_position.y + 30.f);

    m_questNameText.setFont(m_font);
    m_questNameText.setCharacterSize(20);
    m_questNameText.setFillColor(sf::Color::White);
    m_questNameText.setStyle(sf::Text::Bold);

    m_questDetailsText.setFont(m_font);
    m_questDetailsText.setCharacterSize(18);
    m_questDetailsText.setFillColor(sf::Color(200, 200, 200));

    m_instructionsText.setFont(m_font);
    m_instructionsText.setString("Nacisnij [J] lub [Esc] aby zamknac");
    m_instructionsText.setCharacterSize(14);
    m_instructionsText.setFillColor(sf::Color(150, 160, 175));
    centerTextX(m_instructionsText, m_position.y + m_size.y - 30.f);
}

void UIQuestMenu::centerTextX(sf::Text &text, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(m_position.x + m_size.x / 2.0f, y);
}

void UIQuestMenu::update(const QuestSystem& qs, const std::vector<Fraction>& fractions) {
    if (!qs.currentQuest.active) {
        m_questNameText.setString("Brak aktywnych zlecen.");
        m_questNameText.setFillColor(sf::Color(150, 150, 150));
        centerTextX(m_questNameText, m_position.y + 100.f);

        m_questDetailsText.setString("Odwiedz stacje orbitalna i porozmawiaj\nz Mechanikiem, aby znalezc robote.");
        m_questDetailsText.setOrigin(0, 0);
        sf::FloatRect textBounds = m_questDetailsText.getLocalBounds();
        m_questDetailsText.setPosition(m_position.x + (m_size.x / 2.0f) - (textBounds.width / 2.0f), m_position.y + 160.f);
    } else {
        m_questNameText.setString(qs.currentQuest.title);
        m_questNameText.setFillColor(sf::Color::White);
        centerTextX(m_questNameText, m_position.y + 80.f);

        std::stringstream ss;

        if (qs.currentQuest.type == QuestType::CARGO) {
            ss << "Zadanie: Dostarcz towar (" << qs.currentQuest.cargoRequired << " t)\n";
            ss << "Cel: " << qs.currentQuest.destNPC << " na planecie \n(Zlokalizuj znacznik na mapie)\n";
        } else if (qs.currentQuest.type == QuestType::KILL) {
            ss << "Zadanie: Zniszcz wrogie statki\n";
            ss << "Postep: " << qs.currentQuest.currentKills << " / " << qs.currentQuest.requiredKills << "\n";
            ss << "Zleceniodawca: " << qs.currentQuest.giverNPC << "\n";
        } else if (qs.currentQuest.type == QuestType::ASTEROID) {
            ss << "Zadanie: Wydobadz surowiec z Asteroidy\n";
            ss << "Postep: " << (qs.currentQuest.objectiveComplete ? "Gotowe (Wroc do pracodawcy)" : "W trakcie poszukiwan") << "\n";
        } else if (qs.currentQuest.type == QuestType::INVESTIGATE) {
            ss << "Zadanie: Zbadaj wrak statku\n";
            ss << "Postep: " << (qs.currentQuest.objectiveComplete ? "Zbadano (Wroc do pracodawcy)" : "W trakcie poszukiwan") << "\n";
        } else if (qs.currentQuest.type == QuestType::VIP) {
            ss << "Zadanie: Transport VIPa\n";
            ss << "Cel: " << qs.currentQuest.destNPC << " na planecie \n(Zlokalizuj znacznik na mapie)\n";
        }

        ss << "\nNagroda:\n";
        ss << qs.currentQuest.rewardAmt << " szt. ";
        if (qs.currentQuest.reward == RewardType::SCRAP) ss << "Zlomu\n";
        else if (qs.currentQuest.reward == RewardType::ELECTRONICS) ss << "Elektroniki\n";
        else if (qs.currentQuest.reward == RewardType::RARE_METALS) ss << "Rzadkich Metali\n";

        ss << "+" << qs.currentQuest.repReward << " Reputacji (Frakcja: " << fractions[qs.currentQuest.targetFractionID].getName() << ")";

        m_questDetailsText.setString(ss.str());

        m_questDetailsText.setString(ss.str());
        m_questDetailsText.setOrigin(0, 0);
        m_questDetailsText.setPosition(m_position.x + 50.f, m_position.y + 130.f);
    }
}

void UIQuestMenu::draw(sf::RenderWindow &window) {
    window.draw(m_bgRect);
    window.draw(m_borderRect);
    window.draw(m_titleText);
    window.draw(m_questNameText);
    window.draw(m_questDetailsText);
    window.draw(m_instructionsText);
}
