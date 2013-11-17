#ifndef __RESOURCES_HPP__
#define __RESOURCES_HPP__

#include <memory>
#include <string>
#include <vector>

namespace sf {
    class Texture;
}

typedef std::unique_ptr<sf::Texture> UniqueTexture;

class Resources {
private:
    const std::string                 ResDirectory{"res/"};
    const std::vector<std::string>    BackgroundFilenames{};
    const std::vector<std::string>    GemFilenames{"blue_gem.png",
                                                   "green_gem.png",
                                                   "orange_gem.png",
                                                   "purple_gem.png",
                                                   "red_gem.png",
                                                   "white_gem.png"};
    const std::string                 SelectedFilename{"selected.png"};
    const std::string                 TileMapFilename{"tile_board_transparent.png"};
    bool                              Error{false};
    std::vector<UniqueTexture>        BackgroundTextures;
    std::vector<UniqueTexture>        GemTextures;
    UniqueTexture                     SelectedTexture;
    UniqueTexture                     TileMapTexture;

    bool loadTextures();

public:
    Resources();
    ~Resources();
    const sf::Texture* getBackgroundTexture(int id) const;
    const sf::Texture* getGemTexture(int id) const;
    const sf::Texture* getSelectedTexture() const;
    const sf::Texture* getTileMapTexture() const;
    bool hasLoadedSuccessfully() const;
};

#endif
