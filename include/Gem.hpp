#ifndef __GEM_HPP__
#define __GEM_HPP__

#include <memory>

#include <SFML/System/Vector2.hpp>

namespace sf {
    class RenderWindow;
    class Sprite;
    class Texture;
}

enum GemColor {
    Blue,
    Green,
    Orange,
    Purple,
    Red,
    White,
    None
};

enum GemState {
    Normal       = (1 << 0),
    Fire         = (1 << 1),
    Star         = (1 << 2),
    Swapping     = (1 << 3),
    Disappearing = (1 << 4),
    Falling      = (1 << 5)
};

class Gem {
private:
    static const size_t         Size{40};
    GemColor                    Color{GemColor::None};
    char                        State{GemState::Normal};
    std::unique_ptr<sf::Sprite> GemSprite;

public:
    Gem();
    ~Gem();
    Gem(const sf::Texture* texture, const GemColor color, const GemState state );
    void addState(const GemState state);
    void draw(sf::RenderWindow* window);
    GemColor getGemColor() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getScale() const;
    static size_t getSize();
    char getState() const;
    const sf::Texture* getTexture() const;
    void removeState(const GemState toRemove);
    void scale(const sf::Vector2f& factors);
    void setGemColor(const GemColor color, sf::Texture* texture);
    void setPosition(const sf::Vector2f& pos);
    void setScale(const sf::Vector2f& scale);
    void setState(const GemState state);
};

#endif
