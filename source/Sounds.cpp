#include <new>
#include <iostream>

#include <SFML/Audio/Sound.hpp>

#include "../include/Resources.hpp"
#include "../include/Sounds.hpp"

using namespace std;
using namespace sf;

Sounds::Sounds(const Resources* resources) {
    try {
        MatchLow = unique_ptr<Sound>(new Sound(*resources->getMatchLowBuffer()));
        MatchHigh = unique_ptr<Sound>(new Sound(*resources->getMatchHighBuffer()));

        CascadeLowest = unique_ptr<Sound>(new Sound(*resources->getCascadeLowestBuffer()));
        CascadeLow = unique_ptr<Sound>(new Sound(*resources->getCascadeLowBuffer()));
        CascadeMedium = unique_ptr<Sound>(new Sound(*resources->getCascadeMediumBuffer()));
        CascadeHigh = unique_ptr<Sound>(new Sound(*resources->getCascadeHighBuffer()));
        CascadeHighest = unique_ptr<Sound>(new Sound(*resources->getCascadeHighestBuffer()));

        Swapback = unique_ptr<Sound>(new Sound(*resources->getSwapbackBuffer()));

    } catch (const bad_alloc& except) {
        cerr << "Sounds::Sounds(): " << except.what() << endl;
        Error = true;
    }
}

Sounds::~Sounds() = default;

void Sounds::playCascade(const int level=0) {
    switch(level){
    default:
    case 0:
        CascadeLowest->play();
        break;
    case 1:
        CascadeLow->play();
        break;
    case 2:
        CascadeMedium->play();
        break;
    case 3:
        CascadeHigh->play();
        break;
    case 4:
        CascadeHighest->play();
        break;
    }
}

void Sounds::playMatch(bool low) {
    if (low)
        MatchLow->play();
    else 
        MatchHigh->play();
}

void Sounds::playSwapback() {
    Swapback->play();
}
