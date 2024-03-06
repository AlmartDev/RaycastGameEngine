#include <cmath>
#include <iostream>
#include <vector>

#include <optional>

#include "Texture.h"
#include "IRenderer.h"

// Get both numbers.bmp and alphabet.bmp from assets/text folder
// Then define each character of the alphabet by splitting the alphabet.bmp into 26 characters (each character is 7x12), same for numbers
// Then return the character as a texture

class Text
{
    public:
        Text(IRenderer& renderer);

        std::vector<std::optional<Texture>> getCharacterTextures(const char* text);

    private:
        std::optional<Texture> numbers;
        std::optional<Texture> alphabet;

        std::vector<std::optional<Texture>> alphabetCharacters; // alphabet includes the Ñ character
        std::vector<std::optional<Texture>> numberCharacters;

};