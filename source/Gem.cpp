#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../include/Gem.hpp"

using namespace std;
using namespace sf;

Gem::Gem() {
    try {
        GemSprite = unique_ptr<Sprite>(new Sprite());
    } catch (const std::bad_alloc& except) {
        cerr << "Failed allocation of Gem.GemSprite: " << except.what() << "\n";
        GemSprite = nullptr;
    }
}

Gem::Gem(const Texture* texture, const GemColor color, const GemState state) : Gem() {
    this->Color = color;
    this->State = state;
    if (texture)
        GemSprite->setTexture(*texture);
}

void Gem::addState(const GemState state) {
    if ((this->State & GemState::Normal) == GemState::Normal
       && (state == Fire || state == Star))
        this->State ^= GemState::Normal;
        
    this->State |= state;
}

Gem::~Gem() = default;

void Gem::draw(RenderWindow* window) {
    if (window)
        window->draw(*GemSprite.get());
}

GemColor Gem::getGemColor() const {
    return this->Color;
}
Vector2f Gem::getPosition() const {
    return GemSprite->getPosition();
}

size_t Gem::getSize() {
    return Size;
}

char Gem::getState() const {
    return this->State;
}

const Texture* Gem::getTexture() const {
    return GemSprite->getTexture();
}

void Gem::removeState(const GemState state) {
    this->State ^= state;
}
void Gem::setPosition(const Vector2f& pos) {
    GemSprite->setPosition(pos);
}

void Gem::setGemColor(const GemColor color) {
    this->Color = color;
}


void Gem::setTexture(const Texture* texture) {
    GemSprite->setTexture(*texture);
}

void Gem::update() {}
