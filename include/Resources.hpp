#ifndef __RESOURCES_HPP__
#define __RESOURCES_HPP__

#include <memory>
#include <string>
#include <vector>

namespace sf {
    class Font;
    class SoundBuffer;
    class Texture;
}

typedef std::unique_ptr<sf::Texture> UniqueTexture;
typedef std::unique_ptr<sf::SoundBuffer> UniqueSoundBuffer;

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
    const std::string                 CascadeLowFilename{"cascade_low.ogg"};
    const std::string                 CascadeLowestFilename{"cascade_lowest.ogg"};
    const std::string                 CascadeMediumFilename{"cascade_medium.ogg"};
    const std::string                 CascadeHighFilename{"cascade_high.ogg"};
    const std::string                 CascadeHighestFilename{"cascade_highest.ogg"};
    const std::string                 MatchHighFilename{"match_high.ogg"};
    const std::string                 MatchLowFilename{"match_low.ogg"};
    const std::string                 SwapbackFilename{"swap_back.ogg"};
    const std::string                 TileMapFilename{"tile_board_transparent.png"};
    bool                              Error{false};
    std::unique_ptr<sf::Font>         FontType;
    UniqueSoundBuffer                 CascadeLowBuffer;
    UniqueSoundBuffer                 CascadeLowestBuffer;
    UniqueSoundBuffer                 CascadeMediumBuffer;
    UniqueSoundBuffer                 CascadeHighBuffer;
    UniqueSoundBuffer                 CascadeHighestBuffer;
    UniqueSoundBuffer                 MatchLowBuffer;
    UniqueSoundBuffer                 MatchHighBuffer;
    UniqueSoundBuffer                 SwapbackBuffer;
    std::vector<UniqueTexture>        BackgroundTextures;
    std::vector<UniqueTexture>        GemTextures;
    UniqueTexture                     ScoreTexture;
    UniqueTexture                     SelectedTexture;
    UniqueTexture                     TileMapTexture;

    bool loadTextures();
    bool loadSoundBuffers();

public:
    Resources();
    ~Resources();
    const sf::Texture* getBackgroundTexture(int id) const;
    const sf::SoundBuffer* getCascadeLowBuffer() const;
    const sf::SoundBuffer* getCascadeLowestBuffer() const;
    const sf::SoundBuffer* getCascadeMediumBuffer() const;
    const sf::SoundBuffer* getCascadeHighBuffer() const;
    const sf::SoundBuffer* getCascadeHighestBuffer() const;
    const sf::Font*    getFont() const;
    const sf::Texture* getGemTexture(int id) const;
    const sf::SoundBuffer* getMatchLowBuffer() const;
    const sf::SoundBuffer* getMatchHighBuffer() const;
    const sf::Texture* getScoreTexture() const;
    const sf::Texture* getSelectedTexture() const;
    const sf::SoundBuffer* getSwapbackBuffer() const;
    const sf::Texture* getTileMapTexture() const;
    bool hasLoadedSuccessfully() const;
};

#endif
