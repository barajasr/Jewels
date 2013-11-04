#include <algorithm>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>

#include "../include/Gameboard.hpp"
#include "../include/Gem.hpp"

using namespace std;
using namespace sf;

Gameboard::Gameboard() {
    Window = unique_ptr<RenderWindow>(new RenderWindow(VideoMode(320, 320),
                                                       "Jewels"));
    if (!this->loadTextures()) {
        Error = true;
        return;
    }

    this->initBoard();
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

        Window->clear(Color::Green);                                              
        Window->draw(background);
        this->drawBoard();
        Window->display();
    }
}

// pos : col, row
int  Gameboard::generateGem(const IntPair pos, const IntPair leftGems) {
    random_device rd;
    mt19937 gen(rd());
    // [0, 5]
    uniform_int_distribution<int> distribution(GemColor::BLUE, GemColor::WHITE);
    if (pos.second >= 2 && pos.second < this->Columns) {
        IntPair upGems{-1, -2};
            if (pos.second >= 2) {
                // accessed as col, row
                upGems = IntPair(static_cast<int>(Gems.at(pos.second-1).at(pos.first)->getGemColor()),
                                 static_cast<int>(Gems.at(pos.second-2).at(pos.first)->getGemColor()));
            }

        if (upGems.first != upGems.second && leftGems.first != leftGems.second) {
            return distribution(gen);
        } else {
            int current = distribution(gen);
            while ((leftGems.first == leftGems.second && current == leftGems.first)
                   || (upGems.first == upGems.second && current == upGems.first))
                current = distribution(gen);
            return current;
        }
    } else if (pos.second < 2) {
            int current = distribution(gen);
            while (current == leftGems.first && current == leftGems.second)
                current = distribution(gen);
            return current;
    }
}
void Gameboard::initBoard() {
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
            tmp->setPosition(Vector2f(40*col, 40*row));
            sprites.emplace_back(move(tmp));
            second = first;
            first = current;
        } 
        Gems.emplace_back(move(sprites));
    }
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
