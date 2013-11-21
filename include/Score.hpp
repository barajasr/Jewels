#ifndef __SCORE_HPP__
#define __SCORE_HPP__

#include <deque>
#include <memory>

namespace sf {
    class Clock;
    class Font;
    class Sprite;
    class Text;
    class RenderWindow;
}

class Score {
private:
    typedef struct GemScore {
        std::unique_ptr<sf::Text>  gemScore;
        std::unique_ptr<sf::Clock> gemScoreClock;
    } GemScore;
    
    unsigned                    Points{0};
    const std::string           Zeros{"0000000000000"};
    const sf::Font*             FontType;
    std::unique_ptr<sf::Sprite> Background;
    std::unique_ptr<sf::Text>   PointsText;
    std::unique_ptr<sf::Text>   PointsTitle;
    std::deque<int>             ToAdd;
    std::deque<GemScore>        GemScores;

    void addGemScoreToBoard(const int score, const sf::Vector2f originGem);

public:
    Score(const sf::Texture* backTexture, const sf::Font* font, const sf::Vector2f position);
    ~Score();
    void draw(sf::RenderWindow* window);
    void incrementScore(const int gemCount, const sf::Vector2f originGem);
    void update();
};

#endif
