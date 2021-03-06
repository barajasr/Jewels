#include <algorithm> 

#include <SFML/System/Clock.hpp>

#include "../include/Cascade.hpp"
#include "../include/Gameboard.hpp"
#include "../include/Gem.hpp"
#include "../include/Resources.hpp"

using namespace sf;
using namespace std;

bool sortVector2i(const Vector2i one, const Vector2i two) {
    // Sort by col then row
    if (one.y < two.y)
        return true;
    else if (one.y > two.y)
        return false;
    else
        return one.x < two.x;
}

Cascade::Cascade(Gameboard* board, const size_t maxColumns) : Board{board}, Columns{maxColumns} {
   CascadeClock = unique_ptr<Clock>(new Clock());
}

Cascade::~Cascade() = default;

void Cascade::addOpenings(vector<Vector2i>& spots) {
    // Assumptions, spots.size() >= 3
    // Sort by column, row increasing
    sort(spots.begin(), spots.end(), sortVector2i);
    for (auto& indices : spots) {
        // Should reset gem first
        auto id = static_cast<GemColor>(Board->generateGem());
        const Texture* texture = Board->ResourceManager->getGemTexture(id);
        Board->getGemPointer(indices)->setGemColor(id, texture);
        Board->getGemPointer(indices)->setScale({1.0f, 1.0f});
        this->swapUp(indices);
        if (indices.x > Columns.at(indices.y).rows) {
            Columns.at(indices.y).rows = indices.x;
            Columns.at(indices.y).done= false;
        }
    }

    // Passed by ref from Vanish::ToCascade, empty it
    spots.clear();
    if (this->Active == 0) {
        for (auto colPair : Columns) {
            if (colPair.rows >= 0)
                this->Active += 1;
        }
        CascadeClock->restart();
    }
}

void Cascade::finalize(int column) {
    const int size = Gem::getSize();
    for (int row{Columns.at(column).rows}; row >= 0; --row) {
        auto gem = Board->getGemPointer({row, column});
        auto pos = gem->getPosition();
        pos.y = size*row + size/2 + Board->TileMapCorner.y;
        gem->setPosition(pos);
        gem->setState(GemState::Normal);

        ToMatch.emplace_back(row, column);
    }
    Columns.at(column).rows = -1;
    Columns.at(column).done = false;
    --this->Active;
}

vector<Vector2i>& Cascade::toCheckForMatches() {
    return ToMatch;
}

bool Cascade::toCheckForMatchesQueued() const {
    return !ToMatch.empty();
}
void Cascade::update() {
    const float step{300.0f};
    float time = this->CascadeClock->getElapsedTime().asSeconds();

    for (size_t column{0}; column < Columns.size(); ++column) {
        auto offset = time * step;
        // Cascade those with sufficients rows to cascade
        auto& current = this->Columns.at(column);
        if (current.rows >= 0) {
            for (int row{current.rows}; row >= 0; --row) {
                auto gem = Board->getGemPointer({row, static_cast<int>(column)});
                Board->invalidateSelectedIfConflict({row, static_cast<int>(column)});
                auto pos = gem->getPosition();
                if (pos.y+offset > (Gem::getSize()*row + Gem::getSize()/2 + Board->TileMapCorner.y)) {
                    current.done = true;
                }
                gem->setPosition({pos.x, pos.y+offset});
            }
            if (current.done) {
                this->finalize(column);
            }
        }
    }

    this->CascadeClock->restart();
}

// Take the highest opening from column and swaps unique_ptrs with one above
// until  reaches row 0, continue until no more openings to swap in column.
void Cascade::swapUp(Vector2i indices) {
    for (int above{indices.x-1}; above >= 0; --above, --indices.x) {
        Vector2i aboveIndices{above, indices.y};
        auto gem = Board->getGemPointer(indices);
        auto gemTwo = Board->getGemPointer(aboveIndices);
        gemTwo->addState(GemState::Falling);
        auto newPos = gemTwo->getPosition();
        newPos.y -= Gem::getSize();
        gem->setPosition(newPos);
        Board->swapGemPointers(indices, aboveIndices);
    }
}
