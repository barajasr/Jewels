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

enum class GemState {
    Normal,
    Fire,
    Star,
};

class Gem {
private:
    static const size_t         Size{40};
    GemColor                    Color{GemColor::None};
    GemState                    State{GemState::Normal};
    std::unique_ptr<sf::Sprite> GemSprite;

public:
    Gem();
    ~Gem();
    Gem(const sf::Texture* texture, const GemColor color, const GemState state );
    void draw(sf::RenderWindow* window);
    GemColor getGemColor() const;
    sf::Vector2f getPosition() const;
    static size_t getSize();
    GemState getState() const;
    const sf::Texture* getTexture() const;
    void setGemColor(const GemColor color);
    void setPosition(const sf::Vector2f& pos);
    void setState(const GemState state);
    void setTexture(const sf::Texture* texture);
    void update();
};

#endif
