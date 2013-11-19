#include <new>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>

#include "../include/Resources.hpp"

using namespace std;
using namespace sf;

Resources::Resources() {
    Error = !this->loadTextures();
}

Resources::~Resources() = default;

const Texture* Resources::getBackgroundTexture(int id) const {
    if (id < static_cast<int>(BackgroundTextures.size()))
        return BackgroundTextures.at(id).get();
    return nullptr;
}

const Texture* Resources::getGemTexture(int id) const {
    if (id < static_cast<int>(GemTextures.size()))
        return GemTextures.at(id).get();
    return nullptr;
}

const Texture* Resources::getSelectedTexture() const {
    return SelectedTexture.get();
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
        SelectedTexture = move(unique_ptr<Texture>(new Texture()));
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

