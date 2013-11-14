#ifndef __VANISH_HPP__
#define __VANISH_HPP__

#include <memory>
#include <vector>

#include <SFML/System/Vector2.hpp>


namespace sf {
    class Clock;
}

class Gameboard;

class Vanish {
private:
    typedef struct ToVanish {
        bool                      done{false};
        std::vector<sf::Vector2i> indices;
        ToVanish(std::vector<sf::Vector2i>& list) : indices{std::move(list)} {}
    } ToVanish;

    Gameboard*                 Board;
    std::unique_ptr<sf::Clock> VanishClock;
    std::deque<ToVanish>       VanishSet;
    std::vector<sf::Vector2i>  ToCascade;

public:
    Vanish(Gameboard* board);
    ~Vanish();
    void addToVanish(std::vector<sf::Vector2i>&& spots);
    bool cascadeQueued() const;
    std::vector<sf::Vector2i>&& getToCascade();
    void update();
};

#endif
