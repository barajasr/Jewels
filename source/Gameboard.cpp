#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include "../include/Cascade.hpp"
#include "../include/Gameboard.hpp"
#include "../include/Gem.hpp"
#include "../include/Icon.hpp"
#include "../include/Swap.hpp"
#include "../include/Vanish.hpp"

using namespace std;
using namespace sf;

Gameboard::Gameboard() : Generator{Rand()}, Distribution(GemColor::Blue, GemColor::White) {
    Window = unique_ptr<RenderWindow>(new RenderWindow(VideoMode(Height, Width),
                                                       "Jewels",
                                                       sf::Style::Close));
    Window->setIcon(icon_image.width, icon_image.height, icon_image.pixel_data);

    if (!this->loadTextures()) {
        Error = true;
        return;
    }

    TileMap = unique_ptr<Sprite>(new Sprite(*Textures.back().get()));
    Selection = unique_ptr<Sprite>(new Sprite(*Textures.at(Textures.size()-2).get()));
    // Match gems' origin, for simplicity
    Selection->setOrigin(Vector2f(Gem::getSize()/2, Gem::getSize()/2));
    SelectionIndices = Vector2i{-1, -1};

    this->initBoard();

    CascadingGems = unique_ptr<Cascade>(new Cascade(this, this->Columns));
    SwapQueue = unique_ptr<Swap>(new Swap(this));
    VanishQueue = unique_ptr<Vanish>(new Vanish(this));
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

    if (SelectionIndices.x > -1)
        Window->draw(*Selection.get());
}

void Gameboard::gameLoop() {
    if (Error)
        return;

    this->openingDrop();

    while (Window->isOpen()) {
        Event event;
        while (Window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                Window->close();
            } else if (event.type == Event::MouseButtonPressed) {
                if (Mouse::isButtonPressed(Mouse::Button::Left))
                    this->processClick(true);
            } else if (event.type == Event::MouseButtonReleased) {
                    this->processClick(false);
            }
        }

        this->update();

        Window->clear(Color::Green);
        Window->draw(*TileMap.get());
        this->drawBoard();
        Window->display();
    }
}

int Gameboard::generateGem() {
    return Distribution(Generator);
}

int Gameboard::generateGem(const IntPair pos, const IntPair leftGems) {
    // [0, 5]
    // Will cause std::out_of_range exception if error
    int current{-1};
    if (pos.second >= 2 && pos.second < static_cast<int>(this->Columns)) {
        IntPair upGems{-1, -2};
            if (pos.second >= 2) {
                upGems = IntPair(static_cast<int>(Gems.at(pos.second-1).at(pos.first)->getGemColor()),
                                 static_cast<int>(Gems.at(pos.second-2).at(pos.first)->getGemColor()));
            }

        if (upGems.first != upGems.second && leftGems.first != leftGems.second) {
            current = Distribution(Generator);
        } else {
            do {
                current = Distribution(Generator);
            } while ((leftGems.first == leftGems.second && current == leftGems.first)
                   || (upGems.first == upGems.second && current == upGems.first));
        }
    } else if (pos.second < 2) {
            do {
                current = Distribution(Generator);
            } while (current == leftGems.first && current == leftGems.second);
    }
    return current;
}

Gem* Gameboard::getGemPointer(const sf::Vector2i indices) const {
    if (indices.x >= 0 && indices.x < static_cast<int>(this->Rows)
       && indices.y >= 0 && indices.y < static_cast<int>(this->Columns))
        return Gems.at(indices.x).at(indices.y).get();

    return nullptr;
}

// Returns position (in pixels) of element found at indices (row, col)
Vector2f Gameboard::getGemPosition(const sf::Vector2i indices) const{
    return Gems.at(indices.x).at(indices.y)->getPosition();
}

// Given (x,y) pixels coordinate, return (row, col) of Gem location
Vector2i Gameboard::getMatrixIndices(const sf::Vector2i pixels) const {
    return Vector2i(pixels.y/Gem::getSize(), pixels.x/Gem::getSize());
}

const Texture* Gameboard::getTexture(const int id) const {
    if (id >= 0 && id < static_cast<int>(Textures.size()))
        return Textures.at(id).get();

    return nullptr;
}

bool Gameboard::isGemSelected(const Vector2i pos) {
    const auto tileBounds = TileMap->getGlobalBounds();
    return  (pos.x > tileBounds.left && pos.x < tileBounds.width
             && pos.y > tileBounds.top && pos.y < tileBounds.height);
}

void Gameboard::initBoard() {
    const auto size = Gem::getSize();
    for (int row{0}; row < this->Rows; ++row) {                                     
        int first{-1};
        int second{-2}; 
        std::vector<unique_ptr<Gem>> sprites;
        // Build row of sprites
        for (int col{0}; col < this->Columns; ++col) {
            int current = generateGem({col, row}, {first, second});
            unique_ptr<Gem> tmp = unique_ptr<Gem>(new Gem());
            tmp->setGemColor(static_cast<GemColor>(current),
                             Textures.at(current).get());
            // Place off board for drop sequence
            // Take into account sprite origin (20, 20)
            tmp->setPosition(Vector2f(size*col +size/2,
                                      -static_cast<int>(size*2)));
            sprites.emplace_back(move(tmp));
            second = first;
            first = current;
        } 
        Gems.emplace_back(move(sprites));
    }
}

// Takes float of time since last called
// Returns whether animation is complete
bool Gameboard::dropAnimation(float time) {
    static vector<bool> startDrop{false, false, false,
                                  false, false, false,
                                  false, true};

    const float dropStep{400.0f}; 
    auto offset = dropStep*time;
    const int size(Gem::getSize());
    for (int row{this->Rows-1}; row >= 0; --row) {
        if (startDrop.at(row)){
            Vector2f pos = Gems.at(row).at(0)->getPosition();
            // Adjust if offset went too far
            // Take into account sprite origin (20, 20)
            if (pos.y + offset >= size*row + size/2) {
                offset = size*row + size/2 - pos.y;
                startDrop.at(row) = false;
            }
            for (int col{0}; col < Columns; ++col) {
                    pos = Gems.at(row).at(col)->getPosition();
                    pos.y += offset;
                    Gems.at(row).at(col)->setPosition(pos);
            }

            // When to start dropping row above
            if (row > 0 && pos.y < size && pos.y >= size/2) {
                startDrop.at(row-1) = true;
            } 
        }
    }

    return Gems.at(0).at(0)->getPosition().y >= size/2;
}

void Gameboard::gemSelection(const Vector2i mousePosition, const bool leftPress) {
    auto indices = this->getMatrixIndices(mousePosition);
    char state = Gems.at(indices.x).at(indices.y)->getState();
    if ((state & GemState::Disappearing) == GemState::Disappearing
       || (state & GemState::Falling) == GemState::Falling
       || (state & GemState::Swapping) == GemState::Swapping)
        return;

    auto newSelectionPosition = this->getGemPosition(indices);
    
    if(SelectionIndices.x != newSelectionPosition.x ||
       SelectionIndices.y != newSelectionPosition.y) {
        if (this->areNeighbors(SelectionIndices, indices)){
            
            // Set to swap gems
            SwapQueue->addToSwap(SelectionIndices, indices);

            SelectionIndices.x = -1;
            SelectionIndices.y = -1;
        } else {
            if (!leftPress)
                return;
            Selection->setPosition(newSelectionPosition);
            this->SelectionIndices.x = indices.x;
            this->SelectionIndices.y = indices.y;
        }
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

void Gameboard::openingDrop() {
    unique_ptr<Clock> clock{new Clock()};
    bool done{false};
    while (!done && Window->isOpen()) {
        Event event;
        while (Window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                Window->close();
            }
        }

        done = this->dropAnimation(clock->getElapsedTime().asSeconds());
        clock->restart();
        Window->clear(Color::Green);
        Window->draw(*TileMap.get());
        this->drawBoard();
        Window->display();
    }
}

void Gameboard::processClick(const bool leftPress) {
    Vector2i mousePosition{Mouse::getPosition(*Window.get())};
    if(this->isGemSelected(mousePosition)) {
        this->gemSelection(mousePosition, leftPress);
    }
}

void Gameboard::swapGemPointers(const sf::Vector2i one, const sf::Vector2i two) {
    swap(Gems.at(one.x).at(one.y), Gems.at(two.x).at(two.y));
}

void Gameboard::update() {
    SwapQueue->update();
    if (SwapQueue->vanishQueued()) {
        VanishQueue->addToVanish(SwapQueue->getToVanish());
    }

    VanishQueue->update();
    if (VanishQueue->cascadeQueued())
        CascadingGems->addOpenings(VanishQueue->getToCascade());
        
    CascadingGems->update();
    if (CascadingGems->toCheckForMatchesQueued())
        SwapQueue->checkMatchesFromCascade(CascadingGems->toCheckForMatches());
}
