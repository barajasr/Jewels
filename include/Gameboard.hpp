#ifndef __GAMEBOARD_HPP__
#define __GAMEBOARD_HPP__

#include <deque>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace sf {
    class Clock;
    class RenderWindow;
    class Sprite;
    class Texture;
}

class Gem;

typedef std::pair<int, int> IntPair;
typedef std::vector<std::vector<std::unique_ptr<Gem>>> gemVectors;
typedef std::vector<sf::Vector2i>                      indicesVector;
typedef std::vector<std::unique_ptr<sf::Texture>>      textureVector;
typedef std::vector<std::unique_ptr<sf::Texture>>      textureVector;
typedef std::pair<bool, std::vector<sf::Vector2i>>     solutionPair;

class Gameboard {
private:
    enum State{
        Idle             = (1 << 0),
        InitialGems      = (1 << 1),
        GemSelected      = (1 << 2),
        GemSwap          = (1 << 3),
        DisappearingGems = (1 << 4),
        FallingGems      = (1 << 5)
    };

    typedef struct SwappingGems {
        sf::Vector2i firstGem{-1, -1};
        sf::Vector2i secondGem{-1, -1};
        sf::Vector2f firstEndPos{0.0f, 0.0f};
        sf::Vector2f secondEndPos{0.0f, 0.0f};
        bool         done{false};
        bool         toReset{false};
        SwappingGems(const sf::Vector2i firstIndices, const sf::Vector2i secondIndices, const sf::Vector2f firstEnd, const sf::Vector2f secondEnd)
            : firstGem{firstIndices}, secondGem{secondIndices}, firstEndPos{firstEnd}, secondEndPos{secondEnd}, done{false}, toReset{false} {}

    } SwappingGems;

    const size_t Columns{8};
    const size_t Rows{8};
    const size_t Height{320};
    const size_t Width{320};
    const std::string                 ResDirectory{"res/"};
    const std::vector<std::string>    TextureFiles = {"blue_gem.png",
                                                      "green_gem.png",
                                                      "orange_gem.png",
                                                      "purple_gem.png",
                                                      "red_gem.png",
                                                      "white_gem.png",
                                                      "selected.png",
                                                      "tile_board_transparent.png"};
    bool                              Error{false};
    char                              GameState;
    std::unique_ptr<sf::Clock>        GameClock;
    std::unique_ptr<sf::RenderWindow> Window;
    std::unique_ptr<sf::Sprite>       TileMap;
    std::unique_ptr<sf::Sprite>       Selection;
    sf::Vector2i                      SelectedGem;
    textureVector                     Textures;
    gemVectors                        Gems;
    std::deque<SwappingGems>          SwappingGemsList;

    std::vector<sf::Vector2i> allMatches(const sf::Vector2i indices);
    bool areNeighbors(const sf::Vector2i first, const sf::Vector2i second) const;
    void downMatches(sf::Vector2i indices, indicesVector& acc);
    void drawBoard();
    void finalizeSwap(SwappingGems& gems);
    int generateGem(const IntPair pos, const IntPair leftGems) const;
    Gem* getGemPointer(const sf::Vector2i indices) const;
    sf::Vector2f getGemPosition(const sf::Vector2i indices) const;
    sf::Vector2i getMatrixIndices(const sf::Vector2i pixels) const;
    bool isGemSelected(const sf::Vector2i pos);
    void initBoard();
    bool initialDrop();
    void leftMatches(sf::Vector2i indices, indicesVector& acc);
    bool loadTextures();
    void processClick();
    void removeSwappedGems();
    void rightMatches(sf::Vector2i indices, indicesVector& acc);
    void swapAnimation();
    void upMatches(sf::Vector2i indices, indicesVector& acc);
    solutionPair isValidSwap(); 
    void update();
public:
    Gameboard();
    ~Gameboard();
    void gameLoop();
};

#endif
