#include <algorithm>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include "../include/Gameboard.hpp"
#include "../include/Gem.hpp"

using namespace std;
using namespace sf;

Gameboard::Gameboard() : GameState(State::InitialGems) {
    Window = unique_ptr<RenderWindow>(new RenderWindow(VideoMode(Height, Width),
                                                       "Jewels",
                                                       sf::Style::Close));
    if (!this->loadTextures()) {
        Error = true;
        return;
    }

    TileMap = unique_ptr<Sprite>(new Sprite(*Textures.back().get()));
    Selection = unique_ptr<Sprite>(new Sprite(*Textures.at(Textures.size()-2).get()));
    SelectedGem = Vector2i{-1, -1};
    this->initBoard();
    GameClock = std::unique_ptr<Clock>(new Clock());
}

Gameboard::~Gameboard() = default;

bool Gameboard::areNeighbors(const Vector2i first, const Vector2i second) const {
    return (first.x+1 == second.x && first.y == second.y) 
           || (first.x-1 == second.x && first.y == second.y)
           || (first.x == second.x && first.y+1 == second.y)
           || (first.x == second.x && first.y-1 == second.y);
}

void Gameboard::drawBoard() {
    for (auto& row : Gems)
        for (auto& gem : row)
            gem->draw(Window.get());

    if ((GameState & State::GemSelected) == State::GemSelected)
        Window->draw(*Selection.get());
}

void Gameboard::gameLoop() {
    if (Error)
        return;

    while (Window->isOpen()) {
        Event event;
        while (Window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                Window->close();
            } else if(event.type == Event::MouseButtonPressed) {
                if (Mouse::isButtonPressed(Mouse::Button::Left))
                    this->processClick();
            }
        }

        this->update();

        Window->clear(Color::Green);
        Window->draw(*TileMap.get());
        this->drawBoard();
        Window->display();
    }
}

int Gameboard::generateGem(const IntPair pos, const IntPair leftGems) const{
    random_device rd;
    mt19937_64 gen(rd());
    // [0, 5]
    uniform_int_distribution<int> distribution(GemColor::Blue, GemColor::White);
    // Will cause std::out_of_range exception if error
    int current{-1};
    if (pos.second >= 2 && pos.second < static_cast<int>(this->Columns)) {
        IntPair upGems{-1, -2};
            if (pos.second >= 2) {
                upGems = IntPair(static_cast<int>(Gems.at(pos.second-1).at(pos.first)->getGemColor()),
                                 static_cast<int>(Gems.at(pos.second-2).at(pos.first)->getGemColor()));
            }

        if (upGems.first != upGems.second && leftGems.first != leftGems.second) {
            current = distribution(gen);
        } else {
            do {
                current = distribution(gen);
            } while ((leftGems.first == leftGems.second && current == leftGems.first)
                   || (upGems.first == upGems.second && current == upGems.first));
        }
    } else if (pos.second < 2) {
            do {
                current = distribution(gen);
            } while (current == leftGems.first && current == leftGems.second);
    }
    return current;
}

// Returns position (in pixels) of element found at indices (row, col)
Vector2f Gameboard::getGemPosition(const sf::Vector2i indices) const{
    return Gems.at(indices.x).at(indices.y)->getPosition();
}

// Given (x,y) pixels coordinate, return (row, col) of Gem location
Vector2i Gameboard::getMatrixIndices(const sf::Vector2i pixels) const {
    return Vector2i(pixels.y/Gem::getSize(), pixels.x/Gem::getSize());
}


bool Gameboard::isGemSelected(const Vector2i pos) {
    const auto tileBounds = TileMap->getGlobalBounds();
    return  (pos.x > tileBounds.left && pos.x < tileBounds.width
             && pos.y > tileBounds.top && pos.y < tileBounds.height);
}

void Gameboard::initBoard() {
    const size_t size = Gem::getSize();
    for (size_t row{0}; row < this->Rows; ++row) {                                     
        int first{-1};
        int second{-2}; 
        std::vector<unique_ptr<Gem>> sprites;
        // Build row of sprites
        for (size_t col{0}; col < this->Columns; ++col) {
            int current = generateGem({col, row}, {first, second});
            unique_ptr<Gem> tmp = unique_ptr<Gem>(new Gem());
            tmp->setTexture(Textures.at(current).get());
            tmp->setGemColor(static_cast<GemColor>(current));
            // Place off board for drop sequence
            tmp->setPosition(Vector2f(size*col, -static_cast<int>(size*2)));
            sprites.emplace_back(move(tmp));
            second = first;
            first = current;
        } 
        Gems.emplace_back(move(sprites));
    }
}

bool Gameboard::initialDrop() {
    static vector<bool> startDrop{false, false, false,
                                  false, false, false,
                                  false, true};

    Time elapsed = GameClock->getElapsedTime();
    float dropStep{400.0f}; 
    auto offset = dropStep*elapsed.asSeconds();
    for (int row{static_cast<int>(Rows)-1}; row >= 0; --row) {
        if (startDrop.at(row)){
            Vector2f pos = Gems.at(row).at(0)->getPosition();
            // Adjust if offset went too far
            if (pos.y + offset >= Gem::getSize()*row) {
                offset = Gem::getSize()*row - pos.y;
                startDrop.at(row) = false;
            }
            for (size_t col{0}; col < Columns; ++col) {
                    pos = Gems.at(row).at(col)->getPosition();
                    pos.y += offset;
                    Gems.at(row).at(col)->setPosition(pos);
            }

            // When to start dropping row above
            if (row > 0 && pos.y < Gem::getSize() && pos.y >= Gem::getSize()/2) {
                startDrop.at(row-1) = true;
            } 
        }
    }

    GameClock->restart();
    return Gems.at(0).at(0)->getPosition().y >= 0;

}

bool Gameboard::loadTextures() {
    // Load gem images
    for (const auto& filename : TextureFiles) {
        Textures.emplace_back(new Texture());
        if (!Textures.back()->loadFromFile(ResDirectory + filename))
            return false;
    }
    return true;
}

void Gameboard::processClick() {
    if ((GameState & State::InitialGems) == State::InitialGems)
        return;

    Vector2i mousePosition{Mouse::getPosition(*Window.get())};
    if(this->isGemSelected(mousePosition)) {
        auto indices = this->getMatrixIndices(mousePosition);
        char state = Gems.at(indices.x).at(indices.y)->getState();
        if ((state & GemState::Swapping) == GemState::Swapping)
            return;

        auto newSelectionPosition = this->getGemPosition(indices);
        
        if(SelectedGem.x != newSelectionPosition.x ||
           SelectedGem.y != newSelectionPosition.y) {
            if (this->areNeighbors(SelectedGem, indices)){
                
                // Set to swap gems
                SwappingGems.emplace_back(SelectedGem,
                                       indices,
                                       this->getGemPosition(indices),
                                       this->getGemPosition(SelectedGem));
                Gems.at(indices.x).at(indices.y)->addState(GemState::Swapping);
                Gems.at(SelectedGem.x).at(SelectedGem.y)->addState(GemState::Swapping);

                SelectedGem.x = -1;
                SelectedGem.y = -1;
                // Change State and restart clock if needed
                GameState ^= State::GemSelected;
                GameState |= State::GemSwap;
                if (State::Idle == (GameState & State::Idle)) {
                    GameClock->restart();
                }
            } else {
                Selection->setPosition(newSelectionPosition);
                this->SelectedGem.x = indices.x;
                this->SelectedGem.y = indices.y;
                GameState |= State::GemSelected;
            }
        }
    }
}

void Gameboard::swapAnimation() {
    auto elapsed = GameClock->getElapsedTime();
    const float step = 200.0f;
    const float offset = step * elapsed.asSeconds();
    for (auto& swapable: SwappingGems) {
        auto one = Gems.at(swapable.firstGem.x).at(swapable.firstGem.y).get();
        auto onePos = one->getPosition();
        auto two = Gems.at(swapable.secondGem.x).at(swapable.secondGem.y).get();
        auto twoPos = two->getPosition();

        // Swap horizontally
        if (swapable.firstGem.x == swapable.secondGem.x) {
            if (swapable.firstGem.y < swapable.secondGem.y ) {
                if (onePos.x+offset < swapable.firstEndPos.x) {
                    one->setPosition(Vector2f(onePos.x+offset, onePos.y));
                    two->setPosition(Vector2f(twoPos.x-offset, twoPos.y));
                } else { 
                    one->setPosition(swapable.firstEndPos);
                    two->setPosition(swapable.secondEndPos);
                    one->removeState(GemState::Swapping);
                    two->removeState(GemState::Swapping);
                    Gems.at(swapable.firstGem.x)
                        .at(swapable.firstGem.y)
                        .swap(Gems.at(swapable.secondGem.x)
                                  .at(swapable.secondGem.y));
                    swapable.done = true;
                }
            } else {
                if (onePos.x-offset > swapable.firstEndPos.x) {
                    one->setPosition(Vector2f(onePos.x-offset, onePos.y));
                    two->setPosition(Vector2f(twoPos.x+offset, twoPos.y));
                } else { 
                    one->setPosition(swapable.firstEndPos);
                    two->setPosition(swapable.secondEndPos);
                    one->removeState(GemState::Swapping);
                    two->removeState(GemState::Swapping);
                    Gems.at(swapable.firstGem.x)
                        .at(swapable.firstGem.y)
                        .swap(Gems.at(swapable.secondGem.x)
                                  .at(swapable.secondGem.y));
                    swapable.done = true;
                }
            }
        } else {    // Swap vertically
            if (swapable.firstGem.x < swapable.secondGem.x ) {
                if (onePos.y+offset < swapable.firstEndPos.y) {
                    one->setPosition(Vector2f(onePos.x, onePos.y+offset));
                    two->setPosition(Vector2f(twoPos.x, twoPos.y-offset));
                } else { 
                    one->setPosition(swapable.firstEndPos);
                    two->setPosition(swapable.secondEndPos);
                    one->removeState(GemState::Swapping);
                    two->removeState(GemState::Swapping);
                    Gems.at(swapable.firstGem.x)
                        .at(swapable.firstGem.y)
                        .swap(Gems.at(swapable.secondGem.x)
                                  .at(swapable.secondGem.y));
                    swapable.done = true;
                }
            } else {
                if (onePos.y-offset > swapable.firstEndPos.y) {
                    one->setPosition(Vector2f(onePos.x, onePos.y-offset));
                    two->setPosition(Vector2f(twoPos.x, twoPos.y+offset));
                } else { 
                    one->setPosition(swapable.firstEndPos);
                    two->setPosition(swapable.secondEndPos);
                    one->removeState(GemState::Swapping);
                    two->removeState(GemState::Swapping);
                    Gems.at(swapable.firstGem.x)
                        .at(swapable.firstGem.y)
                        .swap(Gems.at(swapable.secondGem.x)
                                  .at(swapable.secondGem.y));
                    swapable.done = true;
                }
            }
        }
    }

    while (!SwappingGems.empty() && SwappingGems.front().done)
        SwappingGems.pop_front();

    if (SwappingGems.empty())
        GameState ^= State::GemSwap;
    GameClock->restart();
}

void Gameboard::update() {
    if ((GameState & State::InitialGems) == State::InitialGems) {
        if (this->initialDrop()) 
            GameState = State::Idle;
    }

    if ((GameState & State::GemSwap) == State::GemSwap)
        this->swapAnimation();
}
