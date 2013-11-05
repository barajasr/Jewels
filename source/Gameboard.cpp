#include <algorithm>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

#include "../include/Gameboard.hpp"
#include "../include/Gem.hpp"

using namespace std;
using namespace sf;

Gameboard::Gameboard() : GameState(State::InitialGems) {
    Window = unique_ptr<RenderWindow>(new RenderWindow(VideoMode(320, 320),
                                                       "Jewels",
                                                       sf::Style::Close));
    if (!this->loadTextures()) {
        Error = true;
        return;
    }

    this->initBoard();
    GameClock = std::unique_ptr<Clock>(new Clock());
}

Gameboard::~Gameboard() = default;

void Gameboard::drawBoard() {
    for (auto& row : Gems)
        for (auto& gem : row)
            gem->draw(Window.get());
}

void Gameboard::gameLoop() {
    if (Error)
        return;

    Sprite background(*Textures.back().get());

    while (Window->isOpen()) {
        Event event;                                                             
        while (Window->pollEvent(event)) {                                            
            if (event.type == Event::Closed)                                     
                Window->close();                                                          
        }                                                                            
        this->update();

        Window->clear(Color::Green);                                              
        Window->draw(background);
        this->drawBoard();
        Window->display();
    }
}

int  Gameboard::generateGem(const IntPair pos, const IntPair leftGems) {
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
    for (int row{static_cast<int>(Rows)-1}; row >= 0; --row) {
        if (startDrop.at(row)){
            Vector2f pos = Gems.at(row).at(0)->getPosition();
            auto offset = dropStep*elapsed.asSeconds();
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

void Gameboard::update() {
    if (GameState == State::InitialGems) {
        if (this->initialDrop()) 
            GameState = State::Idle;
    }
}
