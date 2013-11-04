#ifndef __GAMEBOARD_HPP__
#define __GAMEBOARD_HPP__

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace sf {
    class RenderWindow;
    class Sprite;
    class Texture;
}

class Gem;

typedef std::pair<int, int> IntPair;
typedef std::vector<std::vector<std::unique_ptr<Gem>>> gemVectors;
typedef std::vector<std::unique_ptr<sf::Texture>>     textureVector;

class Gameboard {
private:
    const int Columns{8},
              Rows{8};
    const std::string                 ResDirectory{"res/"};
    const std::vector<std::string>    TextureFiles = {"blue_gem.png",
                                                      "green_gem.png",
                                                      "orange_gem.png",
                                                      "purple_gem.png",
                                                      "red_gem.png",
                                                      "white_gem.png",
                                                      "tile_board_transparent.png"};
    bool                              Error{false};
    std::unique_ptr<sf::RenderWindow> Window;
    textureVector                     Textures;
    gemVectors                        Gems;

    void drawBoard();
    void initBoard();
    bool loadTextures();
    int  generateGem(const IntPair pos, const IntPair leftGems);
public:
    Gameboard();
    ~Gameboard();
    void gameLoop();
};

#endif
