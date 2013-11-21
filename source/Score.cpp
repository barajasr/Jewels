#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "../include/Score.hpp"

using namespace std;
using namespace sf;

Score::Score(const sf::Texture* backTexture, const sf::Font* font, const sf::Vector2f position) {
    Background = unique_ptr<Sprite>(new Sprite(*backTexture));
    PointsText = unique_ptr<Text>(new Text(Zeros, *font, 18));
    PointsTitle = unique_ptr<Text>(new Text("Points", *font, 14));
    
    Background->setPosition(position);
    PointsTitle->setPosition({position.x + 5.0f, position.y + 4.0f});
    PointsText->setPosition({position.x + 8.0f, position.y + 20.0f});
    PointsTitle->setStyle(Text::Style::Bold);
    PointsText->setStyle(Text::Style::Bold);
}

Score::~Score() = default;

void Score::draw(RenderWindow* window) {
   window->draw(*Background.get());
   window->draw(*PointsText.get());
   window->draw(*PointsTitle.get());
}

void Score::incrementScore(const int gemCount) {
    int multiplier = 100;
    if (gemCount == 4) {
        multiplier = 125;
    } else if (gemCount == 5) {
        multiplier = 150;
    } else if (gemCount > 5) {
        multiplier = 175;
    }
    ToAdd.emplace_back(gemCount * multiplier);
}

void Score::update() {
    if (!ToAdd.empty()) {
        auto remaining = ToAdd.front();
        int step = 10;
        if (remaining > step) {
            ToAdd.front() -= step;
        } else {
            step = remaining;
            ToAdd.pop_front();
        }
        Points += step;
        string NewPoints{to_string(Points)};
        int prefix = Zeros.size() - NewPoints.size();
        PointsText->setString(Zeros.substr(0, prefix) + NewPoints);
    }
    
}
