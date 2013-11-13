#include <iostream>

#include <SFML/System/Clock.hpp>

#include "../include/Cascade.hpp"
#include "../include/Gameboard.hpp"
#include "../include/Gem.hpp"

using namespace sf;
using namespace std;

Cascade::Cascade() {
   CascadeClock = unique_ptr<Clock>(new Clock());
   for (size_t col{0}; col < 8; ++col) {
    Columns[col] = pair<int, bool>(-1, false);
   }
}

Cascade::~Cascade() = default;

void Cascade::addOpenings(Gameboard* board, vector<Vector2i>& spots) {
    // Assumptions, spots.size() >= 3
    for (auto& indices : spots) {
        // Should reset gem first
        board->getGemPointer(indices)->setScale({1.0f, 1.0f});
        this->swapUp(board, indices);
        if (indices.x > Columns.at(indices.y).first) {
            Columns[indices.y] = pair<int, bool>(indices.x, false);
        }
    }

    if (this->Active++ == 0) {
        CascadeClock->restart();
    }
}

void Cascade::finalize(Gameboard* board, size_t column) {
    const int size = Gem::getSize();
    for (int row{Columns.at(column).first}; row >= 0; --row) {
        auto gem = board->getGemPointer({row, column});
        auto pos = gem->getPosition();
        pos.y = size*row + size/2;
        gem->setPosition(pos);
        gem->setState(GemState::Normal);
    }
    Columns[column].first = -1;
    Columns[column].second = false;
    --this->Active;
}

void Cascade::update(Gameboard* board) {
    const float step{300.0f};
    float time = this->CascadeClock->getElapsedTime().asSeconds();

    for (CascadingIterator it=Columns.begin(); it != Columns.end(); ++it) {
        auto offset = time * step;
        // Cascade those with sufficients rows to cascade
        if (it->second.first > 0) {
            for (int row{it->second.first}; row >= 0; --row) {
                auto gem = board->getGemPointer({row, it->first});
                auto pos = gem->getPosition();
                if (pos.y+offset > (Gem::getSize()*row + Gem::getSize()/2)) {
                    it->second.second = true;
                }
                gem->setPosition({pos.x, pos.y+offset});
            }
            if (it->second.second) {
                this->finalize(board, it->first);
            }
        }
    }

    this->CascadeClock->restart();
}

void Cascade::swapUp(Gameboard* board, Vector2i indices) {
    board->getGemPointer(indices)->addState(GemState::Falling);
    for (int above{indices.x-1}; above >= 0; --above, --indices.x) {
        Vector2i aboveIndices{above, indices.y};
        auto gem = board->getGemPointer(indices);
        auto gemTwo = board->getGemPointer(aboveIndices);
        gemTwo->addState(GemState::Falling);
        auto newPos = gemTwo->getPosition();
        newPos.y -= Gem::getSize();
        gem->setPosition(newPos);
        board->swapGemPointers(indices, aboveIndices);
    }
}
