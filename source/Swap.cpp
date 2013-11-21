#include <iostream>

#include <SFML/System/Clock.hpp>

#include "../include/Gameboard.hpp"
#include "../include/Gem.hpp"
#include "../include/Score.hpp"
#include "../include/Swap.hpp"

using namespace sf;
using namespace std;

Swap::Swap(Gameboard* board) : Board{board}, SwapClock{unique_ptr<Clock>(new Clock())} {}

Swap::~Swap() = default;

void Swap::addToSwap(const sf::Vector2i one, const Vector2i two) {
    bool restartClock = SwapPairs.empty();

    auto gemOne = Board->getGemPointer(one);
    auto gemTwo = Board->getGemPointer(two);
    gemOne->addState(GemState::Swapping);
    gemTwo->addState(GemState::Swapping);
    SwapPairs.emplace_back(one,
                           two,
                           gemTwo->getPosition(),
                           gemOne->getPosition());

    // Maybe clock has been not called for a long time
    if (restartClock)
        SwapClock->restart();
}

std::vector<sf::Vector2i> Swap::allMatches(const sf::Vector2i& indices) {
    vector<Vector2i> result{indices},
                     vertical,
                     horizontal;
    this->upMatches(indices, vertical);
    this->downMatches(indices, vertical);
    this->leftMatches(indices, horizontal);
    this->rightMatches(indices, horizontal);

    const size_t minGoal{2};
    if (vertical.size() >= minGoal)
        result.insert(result.end(), vertical.begin(), vertical.end());
    if(horizontal.size() >= minGoal)
        result.insert(result.end(), horizontal.begin(), horizontal.end());

    if (result.size() > minGoal)
        Board->Scoreboard->incrementScore(result.size());
    
    return result;
}

void Swap::checkMatchesFromCascade(vector<Vector2i>& indices) { 
    const size_t goal{3};
    for (auto& spot : indices) {
        auto matches = move(this->allMatches(spot));
        if (!matches.empty() && matches.size() >= goal)
            for (auto& match : matches) {
                Board->getGemPointer(match)
                     ->addState(GemState::Disappearing);
                ToVanish.emplace_back(match);
            }
    }
    // Passed by ref from Cascade::ToMatch
    indices.clear();
}

void Swap::downMatches(sf::Vector2i indices, indicesVector& acc) {
    const auto color = Board->getGemPointer(indices)->getGemColor();
    for (++indices.x; indices.x < Board->Rows; ++indices.x) {
        if (!this->isMatch(indices, color))
            break;
        acc.emplace_back(indices);
    }
}

void Swap::finalizeSwap(ToSwap& gems) {
    auto one = Board->getGemPointer(gems.firstGem);
    auto two = Board->getGemPointer(gems.secondGem);
    one->setPosition(gems.firstEndPos);
    two->setPosition(gems.secondEndPos);
    one->removeState(GemState::Swapping);
    two->removeState(GemState::Swapping);
    Board->swapGemPointers(gems.firstGem, gems.secondGem);
    gems.done = true;
}

vector<Vector2i>&& Swap::getToVanish() {
   return move(ToVanish);
}

bool Swap::isMatch(const Vector2i indices, const char color) const {
    auto gem = Board->getGemPointer(indices);
    auto state = gem->getState();
    return (color == gem->getGemColor()
           && (state & GemState::Swapping) != GemState::Swapping
           && (state & GemState::Disappearing) != GemState::Disappearing
           && (state & GemState::Falling) != GemState::Falling);
}

pair<bool, vector<Vector2i>> Swap::isValidSwap() {
    const auto gems = SwapPairs.front();
    pair<bool, vector<Vector2i>> result;
    vector<Vector2i> oneMatches{this->allMatches(gems.firstGem)};
    vector<Vector2i> twoMatches{this->allMatches(gems.secondGem)};

    const size_t goal{3};
    if (oneMatches.size() >= goal) {
        result.first = true;
        result.second.insert(result.second.end(),
                             oneMatches.begin(),
                             oneMatches.end());
    }
    if (twoMatches.size() >= goal) {
        result.first = true;
        result.second.insert(result.second.end(),
                             twoMatches.begin(),
                             twoMatches.end());
    }
    return result;
}

void Swap::leftMatches(Vector2i indices, indicesVector& acc) {
    const auto color = Board->getGemPointer(indices)->getGemColor();
    for (--indices.y; indices.y >= 0; --indices.y) {
        if (!this->isMatch(indices, color))
            break;
        acc.emplace_back(indices);
    }
}

void Swap::removeSwapped() {
    while (!SwapPairs.empty() && SwapPairs.front().done){
        auto justSwapped = SwapPairs.front();
        if (!justSwapped.toReset) {
            auto result = this->isValidSwap();
            if (result.first) {
                // Valid move completed
                // Set state, ready for next animation phase
                for (auto& indices : result.second) {
                    Board->getGemPointer(indices)
                         ->addState(GemState::Disappearing);
                    ToVanish.emplace_back(indices);
                }
            } else {
                // Reverse the swap just completed
                justSwapped.done = false;
                justSwapped.toReset = true;
                SwapPairs.emplace_back(justSwapped);
                // Reset state 
                Board->getGemPointer(justSwapped.firstGem)
                    ->addState(GemState::Swapping);
                Board->getGemPointer(justSwapped.secondGem)
                    ->addState(GemState::Swapping);
            }
        }
        SwapPairs.pop_front();
    }
}

void Swap::rightMatches(Vector2i indices, indicesVector& acc) {
    const auto color = Board->getGemPointer(indices)->getGemColor();
    for (++indices.y; indices.y < Board->Columns; ++indices.y) {
        if (!this->isMatch(indices, color))
            break;
        acc.emplace_back(indices);
    }
}

void Swap::upMatches(Vector2i indices, indicesVector& acc) {
    const auto color = Board->getGemPointer(indices)->getGemColor();
    for (--indices.x; indices.x >= 0; --indices.x) {
        if (!this->isMatch(indices, color))
            break;
        acc.emplace_back(indices);
    }
}

void Swap::update() {
    const auto elapsed = SwapClock->getElapsedTime();
    const float step = 250.0f;
    float offset = step * elapsed.asSeconds();
    for (auto& swappable: SwapPairs) {
        auto one = Board->getGemPointer(swappable.firstGem);
        auto onePos = one->getPosition();
        auto two = Board->getGemPointer(swappable.secondGem);
        auto twoPos = two->getPosition();

        // Swap horizontally
        if (swappable.firstGem.x == swappable.secondGem.x) {
            // Compare Column indices for correct offset 
            offset *= (swappable.firstGem.y > swappable.secondGem.y) ? -1 : 1;
            if ((onePos.x+offset < swappable.secondEndPos.x 
               && onePos.x+offset > swappable.firstEndPos.x)
               || (onePos.x+offset > swappable.secondEndPos.x
               && onePos.x+offset < swappable.firstEndPos.x)) {
                one->setPosition(Vector2f(onePos.x+offset, onePos.y));
                two->setPosition(Vector2f(twoPos.x-offset, twoPos.y));
                
            }else { 
                this->finalizeSwap(swappable);
            }
        } else {    // Swap vertically
            // Compare Row indices for correct offset 
            offset *= (swappable.firstGem.x > swappable.secondGem.x) ? -1 : 1;
            if ((onePos.y+offset < swappable.secondEndPos.y
               && onePos.y+offset > swappable.firstEndPos.y)
               || (onePos.y+offset > swappable.secondEndPos.y
               && onePos.y+offset < swappable.firstEndPos.y)) {
                one->setPosition(Vector2f(onePos.x, onePos.y+offset));
                two->setPosition(Vector2f(twoPos.x, twoPos.y-offset));
                
            } else { 
                this->finalizeSwap(swappable);
            }
        }
    }

    this->removeSwapped();
    SwapClock->restart();
}


bool Swap::vanishQueued() const {
    return !ToVanish.empty();
}
