#ifndef __RESOURCES_HPP__
#define __RESOURCES_HPP__

#include <memory>
#include <string>
#include <vector>

namespace sf {
    class Font;
    class Texture;
}

typedef std::unique_ptr<sf::Texture> UniqueTexture;

class Resources {
private:
    const std::string                 ResDirectory{"res/"};
    const std::vector<std::string>    BackgroundFilenames{"lone_tree_in_dark.png",
                                                          "baby_chameleon.png",
                                                          "ink.png",
                                                          "goodnight_tale.png",
                                                          "eruption.png",
                                                          "elizabeth.png",
                                                          "dark_sky.png"};
    const std::string                 FontFilename{"BRADHITC.TTF"};
    const std::vector<std::string>    GemFilenames{"blue_gem.png",
                                                   "green_gem.png",
                                                   "orange_gem.png",
                                                   "purple_gem.png",
                                                   "red_gem.png",
                                                   "white_gem.png"};
    const std::string                 ScoreFilename{"score.png"};
    const std::string                 SelectedFilename{"selected.png"};
    const std::string                 TileMapFilename{"tile_board_transparent.png"};
    bool                              Error{false};
    std::unique_ptr<sf::Font>         FontType;
    std::vector<UniqueTexture>        BackgroundTextures;
    std::vector<UniqueTexture>        GemTextures;
    UniqueTexture                     ScoreTexture;
    UniqueTexture                     SelectedTexture;
    UniqueTexture                     TileMapTexture;

    bool loadTextures();

public:
    Resources();
    ~Resources();
    const sf::Texture* getBackgroundTexture(int id) const;
    const sf::Font*    getFont() const;
    const sf::Texture* getGemTexture(int id) const;
    const sf::Texture* getScoreTexture() const;
    const sf::Texture* getSelectedTexture() const;
    const sf::Texture* getTileMapTexture() const;
    bool hasLoadedSuccessfully() const;
};

#endif
