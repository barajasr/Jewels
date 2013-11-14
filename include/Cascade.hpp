#ifndef __CASCADE_HPP__
#define __CASCADE_HPP__

#include <memory>
#include <map>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace sf {
    class Clock;
}

class Gameboard;

class Cascade {
private:
    typedef struct ToFall{
        bool done;
        int  rows;
        ToFall() {
            done = false;
            rows = -1;
        }
    } ToFall;
    int                        Active{0};
    std::unique_ptr<sf::Clock> CascadeClock;
    std::vector<ToFall>        Columns;
    
    void swapUp(Gameboard* board, sf::Vector2i indices);
    void finalize(Gameboard* board, int column);
public:
    Cascade(size_t maxColumns);
    ~Cascade();
    void addOpennings(Gameboard* board, std::vector<sf::Vector2i>& spots);
    void update(Gameboard* board);
};

#endif
