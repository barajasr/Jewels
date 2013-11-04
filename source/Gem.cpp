#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../include/Gem.hpp"

using namespace std;
using namespace sf;

Gem::Gem() : Color{GemColor::NONE}, State(GemState::NORMAL) {
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

GemState Gem::getState() const {
    return this->State;
}

const Texture* Gem::getTexture() const {
    return GemSprite->getTexture();
}

void Gem::setPosition(const Vector2f& pos) {
    GemSprite->setPosition(pos);
}

void Gem::setGemColor(const GemColor color) {
    this->Color = color;
}

void Gem::setState(const GemState state) {
    this->State = state;
    // TODO side effects
}

void Gem::setTexture(const Texture* texture) {
    GemSprite->setTexture(*texture);
}

void Gem::update() {}
