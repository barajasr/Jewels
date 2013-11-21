#ifndef __SCORE_HPP__
#define __SCORE_HPP__

#include <deque>
#include <memory>

namespace sf {
    class Font;
    class Sprite;
    class Text;
    class RenderWindow;
}

class Score {
private:
    unsigned                    Points{0};
    const std::string           Zeros{"0000000000000"};
    std::unique_ptr<sf::Sprite> Background;
    std::unique_ptr<sf::Text>   PointsText;
    std::unique_ptr<sf::Text>   PointsTitle;
    std::deque<int>                  ToAdd;
public:
    Score(const sf::Texture* backTexture, const sf::Font* font, const sf::Vector2f position);
    ~Score();
    void draw(sf::RenderWindow* window);
    void incrementScore(const int gemCount);
    void update();
};

#endif
