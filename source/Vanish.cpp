#include <utility>

#include <SFML/System/Clock.hpp>

#include "../include/Gameboard.hpp"
#include "../include/Gem.hpp"
#include "../include/Vanish.hpp"

using namespace std;
using namespace sf;

Vanish::Vanish(Gameboard* board) : Board{board}, VanishClock{unique_ptr<Clock>(new Clock())} {}

Vanish::~Vanish() = default;

void Vanish::addToVanish(std::vector<sf::Vector2i>&& spots) {
    bool resetClock = VanishSet.empty();
    VanishSet.emplace_back(spots);

    if (resetClock)
        VanishClock->restart();
}

bool Vanish::cascadeQueued() const{
    return !ToCascade.empty();
}

vector<Vector2i>&& Vanish::getToCascade() {
    return move(ToCascade);
}

void Vanish::update() {
    if (!VanishSet.empty()) {
        float time = VanishClock->getElapsedTime().asSeconds();
        float scale = 1 - time*20.f;    // ~0.67f on my machine

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
        VanishClock->restart();
    }
}
