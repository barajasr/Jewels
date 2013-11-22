#ifndef __SOUNDS_HPP__
#define __SOUNDS_HPP__

#include <memory>

namespace sf {
    class Sound;
}

class Resources;

class Sounds {
private:
    bool                       Error{false};
    std::unique_ptr<sf::Sound> MatchLow;
    std::unique_ptr<sf::Sound> MatchHigh;
    std::unique_ptr<sf::Sound> CascadeLowest;
    std::unique_ptr<sf::Sound> CascadeLow;
    std::unique_ptr<sf::Sound> CascadeMedium;
    std::unique_ptr<sf::Sound> CascadeHigh;
    std::unique_ptr<sf::Sound> CascadeHighest;
    std::unique_ptr<sf::Sound> Swapback;

public:
    Sounds(const Resources* resources);
    ~Sounds();
    void playCascade(const int level);
    void playMatch(bool low);
    void playSwapback();
};

#endif
