#pragma once

#include "Renderer_Structs.h"

#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <array>
#include <string>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct TextureData {
    int texWidth;
    int texHeight;
    int size;
    std::vector<uint8_t> buffer;
    VkImage textureImage = VK_NULL_HANDLE;
    VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
    VkImageView textureImageView = VK_NULL_HANDLE;
};

struct Character {
    float posX = 0, posY = 0;
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
    TextureData txtData;
    std::vector<Vertex> vertices;
    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
    std::vector<VkBuffer> uniformBuffers = std::vector<VkBuffer>(VK_NULL_HANDLE);
    std::vector<VkDeviceMemory> uniformBuffersMemory = std::vector<VkDeviceMemory>(VK_NULL_HANDLE);
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets = std::vector<VkDescriptorSet>(VK_NULL_HANDLE);
};

struct Word {
    float fontSize;
    char align = 'L'; // L, C, R
    float posX = 0.0f, posY = 0.0f;
    glm::vec4 color;
    std::vector<Character> characters;
    std::string word = "";
    bool added = false;
};

class TextClass
{
public:
    TextClass();
	void loadCharacters();
    Character loadCharacter(int ascii);

    std::vector<Character> getCharacters();
    std::vector<Character>* getCharactersP();
    std::vector<Word> getWords();
    std::vector<Word>* getWordsP();
    std::vector<Word> getWordsTD();
    std::vector<Word>* getWordsTDP();

    void deleteWord(std::string word);
    void deleteWordContain(std::string word);
    void clearWords();
    Word* addWord(std::string word, float fontSize, float x, float y, char align, glm::vec4 color);

private:
	FT_Library ft;
	FT_Face face;
    FT_GlyphSlot glyphSlot;

    std::vector<Character> Characters;
    std::vector<Word> words;
    std::vector<Word> wordsToDelete;
};

