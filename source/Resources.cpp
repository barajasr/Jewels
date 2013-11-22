#include <new>
#include <iostream>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include "../include/Resources.hpp"

using namespace std;
using namespace sf;

Resources::Resources() {
    Error = !this->loadTextures();
    Error |= !this->loadSoundBuffers();

    FontType = unique_ptr<Font>(new Font());
    if (!FontType->loadFromFile(ResDirectory + FontFilename))
        Error = true;
}

Resources::~Resources() = default;

const Texture* Resources::getBackgroundTexture(int id) const {
    if (id < static_cast<int>(BackgroundTextures.size()))
        return BackgroundTextures.at(id).get();
    return nullptr;
}

const sf::SoundBuffer* Resources::getCascadeLowBuffer() const {
    return CascadeLowBuffer.get();
}

const sf::SoundBuffer* Resources::getCascadeLowestBuffer() const {
    return CascadeLowestBuffer.get();
}

const sf::SoundBuffer* Resources::getCascadeMediumBuffer() const {
    return CascadeMediumBuffer.get();
}

const sf::SoundBuffer* Resources::getCascadeHighBuffer() const {
    return CascadeHighBuffer.get();
}

const sf::SoundBuffer* Resources::getCascadeHighestBuffer() const {
    return CascadeHighestBuffer.get();
}

const Font* Resources::getFont() const {
    return FontType.get();
}

const Texture* Resources::getGemTexture(int id) const {
    if (id < static_cast<int>(GemTextures.size()))
        return GemTextures.at(id).get();
    return nullptr;
}

const sf::SoundBuffer* Resources::getMatchLowBuffer() const {
    return MatchLowBuffer.get();
}

const sf::SoundBuffer* Resources::getMatchHighBuffer() const {
    return MatchHighBuffer.get();
}

const Texture* Resources::getScoreTexture() const {
    return ScoreTexture.get();
}

const Texture* Resources::getSelectedTexture() const {
    return SelectedTexture.get();
}

const SoundBuffer* Resources::getSwapbackBuffer() const {
    return SwapbackBuffer.get();
}

const Texture* Resources::getTileMapTexture() const {
    return TileMapTexture.get();
}

bool Resources::hasLoadedSuccessfully() const {
    return !Error;
}

bool Resources::loadTextures() {
    try {    
        for (const auto& filename : BackgroundFilenames) {
            BackgroundTextures.emplace_back(unique_ptr<Texture>(new Texture()));
            if (!BackgroundTextures.back()->loadFromFile(ResDirectory + filename))
                return false;
        }
    } catch (const bad_alloc& except) {
        cerr << "Resources::BackgroundTextures allocation: " << except.what() << endl;
        return false;
    }

    try {    
        for (const auto& filename : GemFilenames) {
            GemTextures.emplace_back(unique_ptr<Texture>(new Texture()));
            if (!GemTextures.back()->loadFromFile(ResDirectory + filename))
                return false;
        }
    } catch (const bad_alloc& except) {
        cerr << "Resources::GemTextures allocation: " << except.what() << endl;
        return false;
    }

    try {
        ScoreTexture = unique_ptr<Texture>(new Texture());
        if (!ScoreTexture->loadFromFile(ResDirectory + ScoreFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::ScoreTexture allocation: " << except.what() << endl;
    }
    
    try {
        SelectedTexture = unique_ptr<Texture>(new Texture());
        if (!SelectedTexture->loadFromFile(ResDirectory + SelectedFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::SelectedTexture allocation: " << except.what() << endl;
    }

    try {
        TileMapTexture = unique_ptr<Texture>(new Texture);
        if (!TileMapTexture->loadFromFile(ResDirectory + TileMapFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::TileMapTexture allocation: " << except.what() << endl;
    }

    return true;
}

bool Resources::loadSoundBuffers() {
    try{
        CascadeLowBuffer = unique_ptr<SoundBuffer>(new SoundBuffer());
        if (!CascadeLowBuffer->loadFromFile(ResDirectory + CascadeLowFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::CascadeLowBuffer allocation: " << except.what() << endl;
    }
    
    try{
        CascadeLowestBuffer = unique_ptr<SoundBuffer>(new SoundBuffer());
        if (!CascadeLowestBuffer->loadFromFile(ResDirectory + CascadeLowestFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::CascadeLowestBuffer allocation: " << except.what() << endl;
    }
    
    try{
        CascadeMediumBuffer = unique_ptr<SoundBuffer>(new SoundBuffer());
        if (!CascadeMediumBuffer->loadFromFile(ResDirectory + CascadeMediumFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::CascadeMediumBuffer allocation: " << except.what() << endl;
    }
    
    try{
        CascadeHighBuffer = unique_ptr<SoundBuffer>(new SoundBuffer());
        if (!CascadeHighBuffer->loadFromFile(ResDirectory + CascadeHighFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::CascadeHighBuffer allocation: " << except.what() << endl;
    }
    
    try{
        CascadeHighestBuffer = unique_ptr<SoundBuffer>(new SoundBuffer());
        if (!CascadeHighestBuffer->loadFromFile(ResDirectory + CascadeHighestFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::CascadeHighestBuffer allocation: " << except.what() << endl;
    }
    
    try{
        MatchLowBuffer = unique_ptr<SoundBuffer>(new SoundBuffer());
        if (!MatchLowBuffer->loadFromFile(ResDirectory + MatchLowFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::MatchLowBuffer allocation: " << except.what() << endl;
    }
    
    try{
        MatchHighBuffer = unique_ptr<SoundBuffer>(new SoundBuffer());
        if (!MatchHighBuffer->loadFromFile(ResDirectory + MatchHighFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::MatchHighBuffer allocation: " << except.what() << endl;
    }

    try{
        SwapbackBuffer = unique_ptr<SoundBuffer>(new SoundBuffer());
        if (!SwapbackBuffer->loadFromFile(ResDirectory + SwapbackFilename))
            return false;
    } catch (const bad_alloc& except) {
        cerr << "Resources::SwapbackBuffer allocation: " << except.what() << endl;
    }
    return true;
}
