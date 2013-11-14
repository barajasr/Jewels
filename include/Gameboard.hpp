#ifndef __GAMEBOARD_HPP__
#define __GAMEBOARD_HPP__

#include <deque>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace sf {
    class RenderWindow;
    class Sprite;
    class Texture;
}

class Cascade;
class Gem;
class Swap;
class Vanish;

typedef std::pair<int, int> IntPair;
typedef std::vector<std::vector<std::unique_ptr<Gem>>> gemVectors;
typedef std::vector<sf::Vector2i>                      indicesVector;
typedef std::pair<bool, std::vector<sf::Vector2i>>     solutionPair;
typedef std::vector<std::unique_ptr<sf::Texture>>      textureVector;

class Gameboard {
private:
    const std::string                 ResDirectory{"res/"};
    const std::vector<std::string>    TextureFiles = {"blue_gem.png",
                                                      "green_gem.png",
                                                      "orange_gem.png",
                                                      "purple_gem.png",
                                                      "red_gem.png",
                                                      "white_gem.png",
                                                      "selected.png",
                                                      "tile_board_transparent.png"};
    bool                               Error{false};
    std::random_device                 Rand;
    std::mt19937_64                    Generator;
    std::uniform_int_distribution<int> Distribution;
    std::unique_ptr<sf::RenderWindow>  Window;
    std::unique_ptr<sf::Sprite>        TileMap;
    std::unique_ptr<sf::Sprite>        Selection;
    sf::Vector2i                       SelectionIndices;
    textureVector                      Textures;
    gemVectors                         Gems;
    std::unique_ptr<Cascade>           CascadingGems;
    std::unique_ptr<Swap>              SwapQueue;
    std::unique_ptr<Vanish>            VanishQueue;

    bool areNeighbors(const sf::Vector2i first, const sf::Vector2i second) const;
    void drawBoard();
    int generateGem(const IntPair pos, const IntPair leftGems);
    sf::Vector2f getGemPosition(const sf::Vector2i indices) const;
    sf::Vector2i getMatrixIndices(const sf::Vector2i pixels) const;
    void initBoard();
    bool dropAnimation(float time);
    bool isGemSelected(const sf::Vector2i pos);
    bool loadTextures();
    void openingDrop();
    void processClick();
    void update();

public:
    const int Columns{8},
              Rows{8},
              Height{320},
              Width{320};

    Gameboard();
    ~Gameboard();
    void gameLoop();
    Gem* getGemPointer(const sf::Vector2i indices) const;
    void swapGemPointers(const sf::Vector2i one, const sf::Vector2i two);
};

#endif
