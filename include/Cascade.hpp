#ifndef __CASCADE_HPP__
#define __CASCADE_HPP__

#include <memory>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace sf {
    class Clock;
}

class Gameboard;

class Cascade {
private:
    typedef struct ToFall{
        bool done{false};
        int  rows{-1};
    } ToFall;

    int                        Active{0};
    Gameboard*                 Board;
    std::unique_ptr<sf::Clock> CascadeClock;
    std::vector<ToFall>        Columns;
    std::vector<sf::Vector2i>  ToMatch;
    
    void swapUp(sf::Vector2i indices);
    void finalize(int column);

public:
    Cascade(Gameboard* board, size_t maxColumns);
    ~Cascade();
    void addOpennings(std::vector<sf::Vector2i>&& spots);
    std::vector<sf::Vector2i>& toCheckForMatches();
    bool toCheckForMatchesQueued() const;
    void update();
};

#endif
