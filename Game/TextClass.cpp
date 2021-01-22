#include "TextClass.h"

void TextClass::loadCharacters(){
    bool error = false;
        
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        error = true;
    }    
    if (FT_New_Face(ft, "files/fonts/arial.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        error = true;
    }
    FT_Set_Pixel_Sizes(face, 0, 50);
    glyphSlot = face->glyph;

    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            error = true;
        }

        TextureData txtData;
        txtData.texWidth = glyphSlot->bitmap.width;
        txtData.texHeight = glyphSlot->bitmap.rows;
        txtData.size = glyphSlot->bitmap.width * glyphSlot->bitmap.rows * 4;

        if (txtData.size > 0) {
            std::vector<uint8_t> buffer(txtData.size);

            uint8_t* src = glyphSlot->bitmap.buffer;
            uint8_t* startOfLine = src;
            int dst = 0;

            for (int y = 0; y < txtData.texHeight; ++y) {
                src = startOfLine;
                for (int x = 0; x < txtData.texWidth; ++x) {
                    auto value = *src;
                    src++;

                    buffer[dst++] = 0xff;
                    buffer[dst++] = 0xff;
                    buffer[dst++] = 0xff;
                    buffer[dst++] = value;
                }
                startOfLine += glyphSlot->bitmap.pitch;
            }
            txtData.buffer = buffer;
        }

        Character character;
        character.TextureID = c;
        character.Size = glm::ivec2(glyphSlot->bitmap.width, glyphSlot->bitmap.rows);
        character.Bearing = glm::ivec2(glyphSlot->bitmap_left, glyphSlot->bitmap_top);
        character.Advance = glyphSlot->advance.x;
        character.txtData = txtData;

        Characters.push_back(character);        
    }

    if (error == false) {
        std::cout << "TextClass - OK" << std::endl;
    }
}

Character TextClass::loadCharacter(int ascii){
    if (FT_Load_Char(face, ascii, FT_LOAD_RENDER)) {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    }

    TextureData txtData;
    txtData.texWidth = glyphSlot->bitmap.width;
    txtData.texHeight = glyphSlot->bitmap.rows;
    txtData.size = glyphSlot->bitmap.width * glyphSlot->bitmap.rows * 4;

    if (txtData.size > 0) {
        std::vector<uint8_t> buffer(txtData.size);

        uint8_t* src = glyphSlot->bitmap.buffer;
        uint8_t* startOfLine = src;
        int dst = 0;

        for (int y = 0; y < txtData.texHeight; ++y) {
            src = startOfLine;
            for (int x = 0; x < txtData.texWidth; ++x) {
                auto value = *src;
                src++;

                buffer[dst++] = 0xff;
                buffer[dst++] = 0xff;
                buffer[dst++] = 0xff;
                buffer[dst++] = value;
            }
            startOfLine += glyphSlot->bitmap.pitch;
        }
        txtData.buffer = buffer;
    }

    Character character;
    character.TextureID = ascii;
    character.Size = glm::ivec2(glyphSlot->bitmap.width, glyphSlot->bitmap.rows);
    character.Bearing = glm::ivec2(glyphSlot->bitmap_left, glyphSlot->bitmap_top);
    character.Advance = glyphSlot->advance.x;
    character.txtData = txtData;

    return character;
}

std::vector<Character> TextClass::getCharacters(){
    return Characters;
}

std::vector<Character>* TextClass::getCharactersP(){
    return &Characters;
}

std::vector<Word> TextClass::getWords(){
    return words;
}

std::vector<Word>* TextClass::getWordsP(){
    return &words;
}

void TextClass::addWord(std::string wordT, float fontSize, float x, float y, char align){
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    const int length = wordT.length();
    char wchars[100];
    strcpy_s(wchars, wordT.c_str());

    Word word;
    word.posX = x;
    word.posY = y;
    word.align = align;
    word.fontSize = fontSize;

    int8_t i = 0;
    float offset = x;

    while (wchars[i] != '\0') {
        word.characters.push_back(loadCharacter(wchars[i++]));
    }

    words.push_back(word);
}
