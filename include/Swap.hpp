#ifndef __SWAP_HPP__
#define __SWAP_HPP__

#include <deque>
#include <memory>

#include <SFML/System/Vector2.hpp>

namespace sf {
    class Clock;
}

class Gamboard;

class Swap {
private:
    typedef struct ToSwap {
        sf::Vector2i firstGem{-1, -1};
        sf::Vector2i secondGem{-1, -1};
        sf::Vector2f firstEndPos{0.0f, 0.0f};
        sf::Vector2f secondEndPos{0.0f, 0.0f};
        bool         done{false};
        bool         toReset{false};
        ToSwap(const sf::Vector2i firstIndices, const sf::Vector2i secondIndices, const sf::Vector2f firstEnd, const sf::Vector2f secondEnd)
            : firstGem{firstIndices}, secondGem{secondIndices}, firstEndPos{firstEnd}, secondEndPos{secondEnd}, done{false}, toReset{false} {}
    } ToSwap;

    Gameboard*                 Board;
    std::unique_ptr<sf::Clock> SwapClock;
    std::deque<ToSwap>         SwapPairs;
    std::vector<sf::Vector2i>  ToVanish;

    std::vector<sf::Vector2i> allMatches(const sf::Vector2i& indices);
    bool isMatch(const sf::Vector2i indices, const char color) const;
    void downMatches(sf::Vector2i indices, indicesVector& acc);
    void leftMatches(sf::Vector2i indices, indicesVector& acc);
    void rightMatches(sf::Vector2i indices, indicesVector& acc);
    void upMatches(sf::Vector2i indices, indicesVector& acc);
    void finalizeSwap(ToSwap& gems);
    std::pair<bool, std::vector<sf::Vector2i>> isValidSwap();
    void removeSwapped();
public:
    Swap(Gameboard* board); 
    ~Swap();
    void addToSwap(const sf::Vector2i one, const sf::Vector2i two);
    void checkMatchesFromCascade(std::vector<sf::Vector2i>& indices);
    std::vector<sf::Vector2i>&& getToVanish();
    bool vanishQueued() const;
    void update();
};

#endif
