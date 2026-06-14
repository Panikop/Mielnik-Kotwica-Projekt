#include "../include/AppearingText.h"

AppearingText::AppearingText() : font(nullptr) {}

void AppearingText::setFont(const sf::Font& f) {
    font = &f;
}

void AppearingText::addText(const std::string& message, sf::Color color, float duration) {
    if (!font) return;

    TextElement te;
    te.text.setFont(*font);
    te.text.setString(message);
    te.text.setCharacterSize(40);
    te.text.setStyle(sf::Text::Bold);
    te.text.setFillColor(color);
    te.text.setOutlineColor(sf::Color::Black);
    te.text.setOutlineThickness(2.f);

    sf::FloatRect bounds = te.text.getLocalBounds();
    te.text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);


    te.lifeTime = duration;
    te.maxLifeTime = duration;

    texts.push_back(te);
}

void AppearingText::update(float deltaTime) {
    for (auto it = texts.begin(); it != texts.end(); ) {
        it->lifeTime -= deltaTime;
        if (it->lifeTime <= 0.f) {
            it = texts.erase(it);
        } else {

            float alpha = 255.f;
            float fadeThreshold = std::min(1.0f, it->maxLifeTime * 0.5f);
            if (it->lifeTime < fadeThreshold) {
                alpha = 255.f * (it->lifeTime / fadeThreshold);
            }
            sf::Color c = it->text.getFillColor();
            c.a = static_cast<sf::Uint8>(alpha);
            it->text.setFillColor(c);

            sf::Color outlineC = it->text.getOutlineColor();
            outlineC.a = static_cast<sf::Uint8>(alpha);
            it->text.setOutlineColor(outlineC);

            ++it;
        }
    }
}

void AppearingText::draw(sf::RenderWindow& window) {
    if (texts.empty()) return;

    sf::View currentView = window.getView();
    sf::Vector2f viewCenter = currentView.getCenter();
    sf::Vector2f viewSize = currentView.getSize();

    float yOffset = viewCenter.y - viewSize.y / 2.f + 50.f;

    for (auto& te : texts) {
        te.text.setPosition(viewCenter.x, yOffset);
        window.draw(te.text);
        yOffset += 50.f;
    }
}
