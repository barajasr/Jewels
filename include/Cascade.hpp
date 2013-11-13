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

typedef std::map<int, std::pair<int, bool >>           CascadingColumns;
typedef std::map<int, std::pair<int, bool >>::iterator CascadingIterator;

class Cascade {
private:
    int                        Active{0};
    std::unique_ptr<sf::Clock> CascadeClock;
    CascadingColumns           Columns;
    
    void swapUp(Gameboard* board, sf::Vector2i indices);
    void finalize(Gameboard* board, int column);
public:
    Cascade();
    ~Cascade();
    void addOpenings(Gameboard* board, std::vector<sf::Vector2i>& spots);
    void update(Gameboard* board);
};

#endif
