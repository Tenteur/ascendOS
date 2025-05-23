/**
 * @file text.h
 * @brief This file contains everything needed to draw or manipulate text
 */

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Used to initialize the variables and modules needed for text.c
 */
signed int textInit();

/**
 * @brief Used for storing fonts
 */
typedef struct fontListStruct {
    char fontName[20];
    int fontSize;
    TTF_Font *fontPtr;
    struct fontListStruct *next;
} fontListStruct;

// Render a text at x,y coord with a SDL_Color specified and size
/**
 * @brief Draw text on screen
 * Draw a given text on screen with given coordinates
 * @param text the text that should be rendered
 * @param x x coordinates in pixels
 * @param y y coordinates in pixels
 * @param textColor color in which the text should be
 * @param useCustomSize should use the customSize ?
 * @return 0 if success, -1 if failure
 */
signed int renderTextAtCoord(SDL_Renderer *renderer, const char *text, const int x, const int y, SDL_Color *textColor, bool useCustomSize, SDL_Rect *customSizeRect);
// Generate text texture and return it as a pointer
SDL_Texture *generateTextTexture(SDL_Renderer *renderer, const char *text, const SDL_Color *textColor);
// Return a pointer of a font based on the name and size asked
/**
 * @brief Add a font to be selected
 * Add a font to the list of selectible fonts
 * @param fontName the name of the font that should be added
 * @param fontSize the size of the font that should be added
 * @return 0 if success, -1 if failure
 */
signed int addFontToList(char *fontName, int fontSize);
/**
 * @brief Set the selected font
 * Set the selected font from the list of added fonts
 * @param fontName the name of the font that should be selected
 * @param fontSize the size of the font that should be selected
 * @return 0 if success, -1 if failure
 */
signed int selectFontFromList(char *fontName, int fontSize);
signed int removeFontFromList(char fontName, int fontSize);