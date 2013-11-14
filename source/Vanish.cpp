#include <SFML/System/Clock.hpp>

#include "../include/Gameboard.hpp"
#include "../include/Gem.hpp"
#include "../include/Vanish.hpp"

using namespace std;
using namespace sf;

Vanish::Vanish(Gameboard* board) : Board{board}, VanishClock{unique_ptr<Clock>(new Clock())} {}

Vanish::~Vanish() = default;

void Vanish::addToVanish(std::vector<sf::Vector2i>&& spots) {
    bool resetClock = !VanishSet.empty();
    VanishSet.emplace_back(spots);

    if (resetClock)
        VanishClock->restart();
}

bool Vanish::cascadeQueued() const{
    return !ToCascade.empty();
}

std::vector<sf::Vector2i>&& Vanish::getCascade() {
    return move(ToCascade);
}

void Vanish::update() {
    // Scale down all gems in disappearing list
    float scale = 0.75f;
    //float time = VanishClock->getElapsedTime().asSeconds();
    //scale *= 1/time;

    for (auto& indicesList : VanishSet) {
        for (auto& indices : indicesList.indices) {
            auto gem = Board->getGemPointer(indices);
            if (gem->getScale().x <= 0.01f) {
                indicesList.done = true;
                break;
            }
            gem->scale({scale, scale});
        }
    }

    while (!VanishSet.empty() && VanishSet.front().done) {
        vector<Vector2i> indices{move(VanishSet.front().indices)};
        if (ToCascade.empty()) {
            ToCascade = move(indices);
        } else {
            ToCascade.insert(ToCascade.end(), indices.begin(), indices.end());
        }
        VanishSet.pop_front();
    }
}

